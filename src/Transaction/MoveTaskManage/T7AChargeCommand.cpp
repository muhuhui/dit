//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : T3C_SLAVE
//  @ File Name : T7AGotoCommand.cpp
//  @ Date : 2019-06-27
//  @ Author : ShunWang
//
//


#include "Transaction/MoveTaskManage/T7AChargeCommand.h"
#include "Transaction/Motor.h"
#include "Driver/BatteryDriver.h"
#include "Driver/MyLog4CXX.h"
#include "Transaction/ConfigServer.h"
#include "Transaction/Robot.h"

const std::string LOG_TAG("root.T7AChargeCommand::");

int T7AChargeCommand::exec()
{
    auto logger = initLogger(LOG_TAG + std::string(__FUNCTION__));
	//解析JSON参数
    ConfigServer* m_pConfigServer = ConfigServer::getInstance();
    const string stepBackposName = m_pConfigServer->getFrontPoint();
    BatteryDriver* batteryDriver = BatteryDriver::getInstance();
    std::string posName = _paraJson["destPosName"].asString();
    Robot *pRobot = Robot::getInstance();
    Motor *pMotor = Motor::getInstance();
    Json::Value *sharedJson = getSharedParaJson();
    static int try_num = 0;
    if(sharedJson == NULL) {
        printf("T7AChargeCommand::%s(), can't get json value!\n", __FUNCTION__);
        LOG4CPLUS_ERROR(logger, " can't get json value! ");
        setExeRet(ST_FAIL);
        return _exeRet;
    }
    (*sharedJson)[JSON_EXEC_RESULT] = ST_FAIL;

	if(_exeRet != ST_SUCCESS && _exeRet != ST_FAIL)
	{
	    switch(_exeStatus)
		{
			case T7ACHARGE_ST_SNDCMD:
			{
                int nRet = -1, nTry = 0;
/*                 while (nRet != 0 && nTry < 3) {
                    nTry++;
                    int presetNumber = 1;
                    nRet = pRobot->m_pIndustrialCamera->presetOperation(GOTO_PRESET, presetNumber);
                    if (pRobot->m_pCapabilities->isSupportPDDetect()){
                        //nRet = pRobot->m_pPDD->gotoPosition(-1000);
                    }
                    printf("T7AChargeCommand::%s(), presetOperation, number: %d, nRet: %d\n", __FUNCTION__, presetNumber, nRet);
                    LOG4CPLUS_INFO(logger, " presetOperation, number: " << presetNumber << ", nRet: " << nRet);
                } */
                if (!pRobot->m_pCapabilities->isSupportSeerBattery()) {
                    if(batteryDriver->getCurrent() > 0.5) {  //正在充电，直接退出
                        _tryCnt = 0;
                        try_num = 0;
                        setExeRet(ST_SUCCESS);
                        return _exeRet;
                    }
                }
                else {
                    if(batteryDriver->isCharge()) {  //正在充电，直接退出
                        _tryCnt = 0;
                        try_num = 0;
                        setExeRet(ST_SUCCESS);
                        return _exeRet;
                    }
                }
                nRet = pMotor->goTo(0, posName);
                printf("T7AChargeCommand::%s(), go to %s, nRet: %d\n", __FUNCTION__, posName.c_str(), nRet);
	            LOG4CPLUS_INFO(logger, " go to " << posName << ", nRet: " << nRet);
				if(nRet == RET_SUCCESS) {
					_tryCnt = 0;
                    clearTimer();
	                _exeStatus = T7ACHAGRE_ST_WAITRESULT;
				}
				else {
	                if(_tryCnt <= 5) {
                        printf("T7AChargeCommand::%s(), go to %s failed, try again!\n", __FUNCTION__, posName.c_str());
	            		LOG4CPLUS_INFO(logger, " go to " << posName << " failed, try again!");
                        _tryCnt++;
					}
					else {
                        printf("T7AChargeCommand::%s(), go to %s failed, exit!\n", __FUNCTION__, posName.c_str());
						LOG4CPLUS_ERROR(logger, " go to " << posName << " failed, exit!");
						setExeRet(ST_FAIL); 
					}
				}
				break;
			}
			case T7ACHAGRE_ST_WAITRESULT:
			{
                if(0 != sleeepWaitTimeOut(10)) { break; }
				if(STOP_STATE == pMotor->getCurStatus()) //当前状态为停止
				{
                    _exeStatus = T7ACHAGRE_ST_CHARGERESULT;
                }
				break;
			}
            case T7ACHAGRE_ST_CHARGERESULT:
			{
                if(0 != sleeepWaitTimeOut(300)) { break; }
                if(!pRobot->m_pCapabilities->isSupportSeerBattery())
                {
                    if(batteryDriver->getCurrent() > 0.5) {  //正在充电，直接退出
                        (*sharedJson)[JSON_EXEC_RESULT] = ST_SUCCESS;
                        printf("T7AChargeCommand::%s(), go to charge point succeeded!\n", posName.c_str());
                        LOG4CPLUS_INFO(logger, " T7AChargeCommand succeeded to excute! " );
                        //2020-10-12 
                        int nRet = pMotor->disableSeerMotor();
                        LOG4CPLUS_INFO(logger, " SeerMotor disable Command!!!!!! " );

                        _tryCnt = 0;
                        try_num = 0;
                        clearTimer();
                        setExeRet(ST_SUCCESS);
                        return _exeRet;
                    }
                    else{
                        printf("T7AChargeCommand, motor stand still succeeded! try_num: %d\n", try_num);
                        LOG4CPLUS_INFO(logger, " T7AChargeCommand stand still! " );
                        try_num++;
                        if(try_num > 5){
                            try_num = 0;
                            _tryCnt = 0;
                            printf("T7AChargeCommand, go to %s failed!\n", posName.c_str());
                            LOG4CPLUS_INFO(logger, " T7AChargeCommand failed to excute! " );
                            clearTimer();
                            setExeRet(ST_FAIL);
                            return _exeRet;
                        }
                        _exeStatus = T7ACHAGRE_ST_STEPBACK;
                    }
                }
                else{
                    if(batteryDriver->isCharge()) {  //正在充电，直接退出
                        (*sharedJson)[JSON_EXEC_RESULT] = ST_SUCCESS;
                        printf("T7AChargeCommand::%s(), go to charge point succeeded!\n", posName.c_str());
                        LOG4CPLUS_INFO(logger, " T7AChargeCommand succeeded to excute! " );
                        //2020-10-12 
                        int nRet = pMotor->disableSeerMotor();
                        LOG4CPLUS_INFO(logger, " SeerMotor disable Command!!!!!! " );

                        _tryCnt = 0;
                        try_num = 0;
                        clearTimer();
                        setExeRet(ST_SUCCESS);
                        return _exeRet;
                    }
                    else{
                        printf("T7AChargeCommand, motor stand still succeeded! try_num: %d\n", try_num);
                        LOG4CPLUS_INFO(logger, " T7AChargeCommand stand still! " );
                        try_num++;
                        if(try_num > 5){
                            try_num = 0;
                            _tryCnt = 0;
                            printf("T7AChargeCommand, go to %s failed!\n", posName.c_str());
                            LOG4CPLUS_INFO(logger, " T7AChargeCommand failed to excute! " );
                            clearTimer();
                            setExeRet(ST_FAIL);
                            return _exeRet;
                        }
                        _exeStatus = T7ACHAGRE_ST_STEPBACK;
                    }
                }
				break;
			}
            case T7ACHAGRE_ST_STEPBACK:
			{
                int nRet = pMotor->goTo(0, stepBackposName);
                printf("T7AChargeCommand::%s(), go back to %s, nRet: %d\n", __FUNCTION__, stepBackposName.c_str(), nRet);
	            LOG4CPLUS_INFO(logger, " go back to " << stepBackposName << ", nRet: " << nRet);
				if(nRet == RET_SUCCESS) {
                    _tryCnt = 0;
	                _exeStatus = T7ACHAGRE_ST_STEPBACKRESULT;
				}
				else {
	                if(_tryCnt <= 5) {
                        printf("T7AChargeCommand::%s(), go back to %s failed, try again!\n", __FUNCTION__, stepBackposName.c_str());
	            		LOG4CPLUS_INFO(logger, " go to " << stepBackposName << " failed, try again!");
                        _tryCnt++;
					}
					else {
                        printf("T7AChargeCommand::%s(), go back to %s failed, exit!\n", __FUNCTION__, stepBackposName.c_str());
						LOG4CPLUS_ERROR(logger, " go to " << stepBackposName << " failed, exit!");
						setExeRet(ST_FAIL); 
					}
				}
				break;
			}
            case T7ACHAGRE_ST_STEPBACKRESULT:
            {
                if(0 != sleeepWaitTimeOut(10)) { break; }
				if(STOP_STATE == pMotor->getCurStatus()) //当前状态为停止
				{
                    printf("T7AChargeCommand::%s(), step back to %s succeeded\n", __FUNCTION__, stepBackposName.c_str());
                    LOG4CPLUS_ERROR(logger, " step back to " << stepBackposName << " succeeded, exit!");
                    _tryCnt = 0;
                    _exeStatus = T7ACHARGE_ST_SNDCMD;
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

