#include "Driver/AlarmDriver.h"
#include "ThirdParty/jsoncpp/json.h"
#include <algorithm>
#include <stdio.h>
#include <unistd.h>


AlarmDriver::~AlarmDriver()
{
    m_bThreadRun = false;
}

AlarmDriver *AlarmDriver::getInstance()
{
    return SingleTon<AlarmDriver>::GetInstance();
}

int AlarmDriver::init()
{
    TcpCltSession *session = TcpCltSession::getInstance();
    m_tcpClient = session->getSession(TCP_MAIN_BOARD);

    m_bThreadRun = true;
    return 0;
}

std::string AlarmDriver::spliceReq(const char* reqName, int transId)
{
    std::string req;
    Json::Value val;
    Json::Value array;
    val["transId"] = std::to_string(transId);
    val["reqName"] = reqName;
    req = val.toStyledString();
    req.erase(std::remove_if(req.begin(), req.end(), 
                [](char c) -> bool { return (c == '\n' || c == '\t' || c == '\r' || c == ' '); }), req.end());
    return req;
}

std::string AlarmDriver::spliceReq(const char* reqName, int transId, int sw)
{
    std::string req;
    Json::Value val;
    Json::Value array;
    val["transId"] = std::to_string(transId);
    val["reqName"] = reqName;
    val["switch"] = sw;
    req = val.toStyledString();
    req.erase(std::remove_if(req.begin(), req.end(), 
                [](char c) -> bool { return (c == '\n' || c == '\t' || c == '\r' || c == ' '); }), req.end());
    return req;
}

int AlarmDriver::parseRes(std::string res, TcpResp& result)
{
    Json::Reader reader;
    Json::Value val;
    Json::Value array;

    JSON_TRY
    result.errcode = -1;
    if (reader.parse(res, val, false)) {
        if (!val["transId"].isNull()) {
            result.transId = val["transId"].asString();
        }
        if (!val["errcode"].isNull()) {
            result.errcode = val["errcode"].asInt();
        }
        if (!val["errmsg"].isNull()) {
            result.errmsg = val["errmsg"].asString();
        }
    }
    else {
        printf("%s(), parse failed!\n", __FUNCTION__);
    }
    JSON_CATCH
    return result.errcode;
}

int AlarmDriver::parseRes(std::string res, TcpResp& result, int& sw)
{
    Json::Reader reader;
    Json::Value val;
    Json::Value array;

    JSON_TRY
    result.errcode = -1;
    if (reader.parse(res, val, false)) {
        if (!val["transId"].isNull()) {
            result.transId = val["transId"].asString();
        }
        if (!val["errcode"].isNull()) {
            result.errcode = val["errcode"].asInt();
        }
        if (!val["errmsg"].isNull()) {
            result.errmsg = val["errmsg"].asString();
        }

        if ((!val["switch"].isNull()) && (result.errcode == 0)) {
            sw = val["switch"].asInt();
        }
    }
    else {
        printf("%s(), parse failed!\n", __FUNCTION__);
    }
    JSON_CATCH
    return result.errcode;
}

int AlarmDriver::swCtrl(int sw)
{
    std::string req = spliceReq("alarmSwCtrl", (int)time(0), sw);
    std::string res;
    int nRet = m_tcpClient->request(req, res);
    if (nRet != 0) {
        return -1;
    }

    TcpResp result;
    nRet = parseRes(res, result);
    if (nRet != 0) {
        return -1;
    }
    return 0;
}

int AlarmDriver::getSw(int& sw)
{
    std::string req = spliceReq("alarmSwStatus", (int)time(0));
    std::string res;
    int nRet = m_tcpClient->request(req, res);
    if (nRet != 0) {
        return -1;
    }

    TcpResp result;
    nRet = parseRes(res, result, sw);
    if (nRet != 0) {
        return -1;
    }
    return 0;
}

