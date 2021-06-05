#include "PTZDriver.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include "Driver/MyLog4CXX.h"
#include "ThirdParty/jsoncpp/json.h"
#include <sys/time.h>

const std::string LOG_TAG("root.PTZDriver::");

PTZDriver::PTZDriver()
{
}

PTZDriver::~PTZDriver()
{

}

PTZDriver::Ptr PTZDriver::getInstance()
{
    static PTZDriver::Ptr p(new PTZDriver);
    return p;
}

int PTZDriver::init()
{
    TcpCltSession *session = TcpCltSession::getInstance();
    m_tcpClient = session->getSession(TCP_PTZ_BOARD);
   
    return 0;
}

std::string PTZDriver::spliceReq(const char* reqName, int transId)
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

std::string PTZDriver::spliceReq(const char* reqName, int transId, std::vector<PTZMotorCtrl> ctrl)
{
    std::string req;
    Json::Value val;
    Json::Value array;
    val["transId"] = std::to_string(transId);
    val["reqName"] = reqName;
    for (unsigned int i = 0; i < ctrl.size(); i++) {
        array[i]["id"] = ctrl[i].motorId;
        array[i]["cmd"] = ctrl[i].cmd;
        array[i]["angle"] = ctrl[i].angle;
    }
    val["para"] = array;
    req = val.toStyledString();
    req.erase(std::remove_if(req.begin(), req.end(), 
                [](char c) -> bool { return (c == '\n' || c == '\t' || c == '\r' || c == ' '); }), req.end());
    return req;
}

int PTZDriver::parseRes(std::string res, TcpResp& result)
{
    Json::Reader reader;
    Json::Value val;

    JSON_TRY
    result.errcode = -1;
    if (reader.parse(res, val, false)) {
        if (!val["transId"].isNull()) {
            if (result.transId != val["transId"].asString()) {
                return -1;
            }
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

int PTZDriver::parseRes(std::string res, TcpResp& result, std::vector<PTZMotorStat>& vstat)
{
    Json::Reader reader;
    Json::Value val;
    Json::Value array;

    JSON_TRY
    result.errcode = -1;
    if (reader.parse(res, val, false)) {
        if (!val["transId"].isNull()) {
            if (result.transId != val["transId"].asString()) {
                return -1;
            }
            result.transId = val["transId"].asString();
        }
        if (!val["errcode"].isNull()) {
            result.errcode = val["errcode"].asInt();
        }
        if (!val["errmsg"].isNull()) {
            result.errmsg = val["errmsg"].asString();
        }

        if ((!val["result"].isNull()) && (result.errcode == 0)) {
            Json::Value array = val["result"];
            for (unsigned int i = 0; i < array.size(); i++) {
                PTZMotorStat stat;
                stat.motorId = (PTZMotorNum)array[i]["id"].asInt();
                stat.angle = array[i]["angle"].asInt();
                stat.moveStat = (PTZMoveStat)array[i]["moveStat"].asInt();
                vstat.push_back(stat);
            }
        }
    }
    else {
        printf("%s(), parse failed!\n", __FUNCTION__);
    }
    JSON_CATCH

    return result.errcode;
}

int PTZDriver::getPTZStatus(std::vector<PTZMotorStat>& stat)
{
    auto logger = initLogger(LOG_TAG + std::string(__FUNCTION__));

    struct timeval tv;
    gettimeofday(&tv, NULL);
    std::string req = spliceReq("ptzMotorStatus", tv.tv_usec);
    std::string res;
    int nRet = m_tcpClient->request(req, res);
    if (nRet != 0) {
        return -1;
    }

    TcpResp result;
    result.transId = std::to_string(tv.tv_usec);
    nRet = parseRes(res, result, stat);
    if (nRet != 0) {
        return -1;
    }

    return 0;
}

int PTZDriver::ctrlPTZ(PTZMotorNum num, PTZMotorCmd cmd, int angle)
{
    PTZMotorCtrl ctrl;
    ctrl.motorId = num;
    ctrl.cmd = cmd;
    ctrl.angle = angle;

    std::vector<PTZMotorCtrl> vctrl;
    vctrl.clear();
    vctrl.push_back(ctrl);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    std::string req = spliceReq("ptzMotorCtrl", tv.tv_usec, vctrl);
    std::string res;

    int nRet = m_tcpClient->request(req, res);
    if (nRet != 0) {
        return -1;
    }

    TcpResp result;
    result.transId = std::to_string(tv.tv_usec);
    nRet = parseRes(res, result);
    if (nRet != 0) {
        return -1;
    }
    return 0;
}

int PTZDriver::getPTZCoordinate(std::vector<int>& coord) {
    int nRet = -1;
    std::vector<PTZMotorStat> motors_state;
    nRet = getPTZStatus(motors_state);
    coord.clear();
    for (size_t i = 0; i < motors_state.size(); i++) {
        coord.push_back(motors_state[i].angle);
    }
    return nRet;
}

int PTZDriver::gotoPreset(int preset[], int size) {
    if(preset == NULL || size < 3) {
        return -1;
    }

    int nRet = 0;
    nRet += ctrlPTZ(CAM_MOTOR, TURN_TO, preset[0]);
    nRet += ctrlPTZ(INFRA_MOTOR, TURN_TO, preset[1]);
    nRet += ctrlPTZ(HORIZ_MOTOR, TURN_TO, preset[2]);
    return nRet;
}

