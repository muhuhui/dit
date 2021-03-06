//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : T3C_SLAVE
//  @ File Name : DriveModeCommand.cpp
//  @ Date : 2019-06-25
//  @ Author : ShunWang
//
//


#include "DriveModeCommand.h"
#include "Transaction/Robot.h"
#include "Transaction/Motor.h"

int DriveModeCommand::exec()
{
	int driveMode = _paraJson["driveMode"].asInt();
	Robot *robot = Robot::getInstance();
	if(_exeRet != ST_SUCCESS && _exeRet != ST_FAIL)
	{
        robot->m_pMotor->setMotionMode((RunMode)driveMode);
        _exeRet =ST_SUCCESS;

	}
	return _exeRet;	
}

