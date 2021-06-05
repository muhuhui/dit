//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : T3C_SLAVE
//  @ File Name : RechargeCommand.cpp
//  @ Date : 2019-06-25
//  @ Author : ShunWang
//
//


#include "RechargeCommand.h"
#include "Driver/BatteryDriver.h"
#include "Transaction/Motor.h"
#include "Transaction/ConfigServer.h"
#include "Transaction/SettingService.h"
#include "Driver/ChargePileDriver.h"
#include "Driver/MyLog4CXX.h"
#include <unistd.h>

const std::string LOG_TAG("root.RechargeCommand::");

/*
int RechargeCommand::exec()
{
    auto logger = initLogger(LOG_TAG + std::string(__FUNCTION__));
	PowerDriver *powerDriver = PowerDriver::getInstance();
	BatteryDriver* batteryDriver = BatteryDriver::getInstance();
	Motor *motor = Motor::getInstance();
    SettingService *settingService = SettingService::getInstance();
    float quantityMinLimit = settingService->m_setting.getBatteryQuantityMin();
    int chargeType = _paraJson["charge_type"].asInt();
    Json::Value *sharedJson = getSharedParaJson();
    
	if(_exeRet != ST_SUCCESS && _exeRet != ST_FAIL)
	{
    	switch(_exeStatus)
		{
            case RECHARGE_ST_RELAYOFF:
            {
                if(sharedJson != NULL) {
                    printf("RechargeCommand::exec(), get nRet: %d!\n", (*sharedJson)[JSON_EXEC_RESULT].asInt());
                    LOG4CPLUS_INFO(logger, "get nRet: " << (*sharedJson)[JSON_EXEC_RESULT].asInt());
                    if((*sharedJson)[JSON_EXEC_RESULT] != ST_SUCCESS) {
                        setExeRet(ST_FAIL);
                        return _exeRet;
                    }
                }
                if(batteryDriver->getCurrent() > 0.5) {  //正在充电，直接退出
                    setExeRet(ST_SUCCESS);
                    return _exeRet;
                }
                clearTimer();
                printf("RechargeCommand::exec(), relay off\n");
                LOG4CPLUS_INFO(logger, "relay off");
                powerDriver->switchPowerCharge(false);
                _exeStatus = RECHARGE_ST_RELAYON;
                break;
            }
			case RECHARGE_ST_RELAYON:
			{
                if(0 == sleeepWaitTimeOut(10))
                {
                    printf("RechargeCommand::exec(), relay on\n");
                    LOG4CPLUS_INFO(logger, "relay on");
                    powerDriver->switchPowerCharge(false);
                    sleep(1);
                    powerDriver->switchPowerCharge(true);
                    _exeStatus = RECHARGE_ST_DETECTCUR;
                }
				break;
			}
			case RECHARGE_ST_DETECTCUR:
			{
				if(_tryCnt < 3)               //重连三次无用则退出重新执行 
				{
                    if(batteryDriver->getCurrent() < 0.5) {    //持续60秒未充电，重新打开
                        if(0 == sleeepWaitTimeOut(600))
                        {
                            printf("RechargeCommand::exec(), no detected current, reconnecting...\n");
                            LOG4CPLUS_INFO(logger, "no detected current, reconnecting...");
                            _tryCnt++;
                            _exeStatus = RECHARGE_ST_RELAYOFF;
                        }
                    }
                    else {
                        //手动充电充上后直接退出
                        if(chargeType == MANUAL_CHARGE) {       
                            setExeRet(ST_SUCCESS);
                            return _exeRet;
                        }    
                        //最低电量+10，结束任务
                        if(batteryDriver->getQuantity() >= (quantityMinLimit + 10) || batteryDriver->getQuantity() >= 98) 
                        {
                            setExeRet(ST_SUCCESS);
                            return _exeRet;
                        }
                        //充电过程机器突然运动了
                        if(STOP_STATE != motor->getCurStatus())
                        {
                            powerDriver->switchPowerCharge(false); //将继电器关闭
                            setExeRet(ST_FAIL);
                            return _exeRet;
                        }
                    }
				}
				else        //执行失败，退出充电
				{
                    setExeRet(ST_FAIL);
                    return _exeRet;
				}

				break;
			}
			default:
			{
				break;
			}
		}
	}
	return _exeRet;
}
*/

int RechargeCommand::exec()
{
    auto logger = initLogger(LOG_TAG + std::string(__FUNCTION__));
	BatteryDriver* batteryDriver = BatteryDriver::getInstance();
    SettingService *settingService = SettingService::getInstance();
    ChargePileDriver *pChargeDrvier = ChargePileDriver::getInstance();
    float quantityMinLimit = settingService->m_setting.getBatteryQuantityMin();
    int chargeType = _paraJson["charge_type"].asInt();
    float destPos = _paraJson["destPos"].asFloat();
    
    if (_exeRet != ST_SUCCESS && _exeRet != ST_FAIL)
    {
        switch(_exeStatus)
        {
            case RECHARGE_START:
            {
                if(batteryDriver->getCurrent() > 0.5) {  //正在充电，直接退出
                    printf("RechargeCommand::exec(), already in charging\n");
                    LOG4CPLUS_INFO(logger, "already in charging");
                    setExeRet(ST_SUCCESS);
                    return _exeRet;
                }
                if(0 == sleeepWaitTimeOut(5)) {
                    int nRet = pChargeDrvier->gotoCharge((int)destPos);
                    printf("RechargeCommand::exec(), gotocharge pos: %f, nRet: %d\n", destPos, nRet);
                    LOG4CPLUS_INFO(logger, "gotocharge pos: " << destPos << ", nRet: " << nRet);
                    if (nRet != 0) {
                        _tryCnt++;
                        if (_tryCnt >= 5) {
                            setExeRet(ST_FAIL);
                            _tryCnt = 0;
                        }
                        printf("RechargeCommand::exec(), gotocharge failed, retry: %d\n", _tryCnt);
                        LOG4CPLUS_INFO(logger, "gotocharge failed, retry: " << _tryCnt);
                        break;
                    }
                    _tryCnt = 0;
                    clearTimer();
                    _exeStatus = RECHARGE_CHECK;
                }
            }
            break;
            case RECHARGE_CHECK:
            {
                if(0 == sleeepWaitTimeOut(20)) {
                    ChargeInfo info;
                    int nRet = pChargeDrvier->getChargeInfo(info);
                    if (nRet == 0) {
                        if (info.chargeStat == CP_IN_SEARCHING) {
                            _tryCnt++;
                            printf("RechargeCommand::exec(), search for charging, stat: %d\n", info.chargeStat);
                            LOG4CPLUS_INFO(logger, "search for charging, stat: " << info.chargeStat);
                            if(_tryCnt > 150) {
                                _tryCnt = 0;
                                int nRet = pChargeDrvier->gotoCharge((int)destPos);
                                printf("RechargeCommand::exec(), gotocharge pos: %f, nRet: %d\n", destPos, nRet);
                                LOG4CPLUS_INFO(logger, "gotocharge pos: " << destPos << ", nRet: " << nRet);
                            }
                        }
                        else if (info.chargeStat == CP_IN_CHARGING) {
                            _tryCnt = 0;
                            if (chargeType == MANUAL_CHARGE) {
                                setExeRet(ST_SUCCESS);
                                return _exeRet;
                            }
                            //最低电量+10，结束任务
                            if (batteryDriver->getQuantity() >= (quantityMinLimit + 10) || batteryDriver->getQuantity() >= 98) {
                                printf("RechargeCommand::exec(), charge over, stat: %d\n", info.chargeStat);
                                LOG4CPLUS_INFO(logger, "charge over, stat: " << info.chargeStat);
                                setExeRet(ST_SUCCESS);
                                return _exeRet;
                            }
                        }
                        else if (info.chargeStat == CP_CHARGE_FULL) {
                            _tryCnt = 0;
                            printf("RechargeCommand::exec(), charge full, stat: %d\n", info.chargeStat);
                            LOG4CPLUS_INFO(logger, "charge full, stat: " << info.chargeStat);
                            setExeRet(ST_SUCCESS);
                            return _exeRet;
                        }
                        else if (info.chargeStat == CP_CHARGE_FAIL || info.chargeStat == CP_NOT_IN_CHARGE) {
                            _tryCnt = 0;
                            printf("RechargeCommand::exec(), charge failed, stat: %d\n", info.chargeStat);
                            LOG4CPLUS_INFO(logger, "charge failed, stat: " << info.chargeStat);
                            setExeRet(ST_FAIL);
                            return _exeRet;
                        }
                        else {
                            _tryCnt = 0;
                            setExeRet(ST_FAIL);
                            return _exeRet;
                        }
                    }
                }
            }
            break;
            default:
            break;
        }
    }
    return _exeRet;
}

