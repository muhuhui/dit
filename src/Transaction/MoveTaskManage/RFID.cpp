//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : T3C_SLAVE
//  @ File Name : RFID.cpp
//  @ Date : 2019-07-03
//  @ Author : ShunWang
//
//


#include "RFID.h"

void RFID::setId(int id)
{
	this->id = id;
}

int RFID::getId()
{
	return id;
}

void RFID::setRfidNum(long rfidNum)
{
	this->rfidNum = rfidNum;
}

long RFID::getRfidNum()
{
	return this->rfidNum;
}

void RFID::setName(std::string name)
{
	this->name = name;
}

std::string RFID::getName()
{
	return this->name;
}

void RFID::setFunctionId(int functionId)
{
	this->functionId = functionId;
}

int RFID::getFunctionId()
{
	return functionId;
}

void RFID::setPosition(int position)
{
	this->position = position;
}

int RFID::getPosition()
{
	return position;
}

void RFID::setGoActionId(int goActionId)
{
	this->goActionId= goActionId;
}

int RFID::getGoActionId()
{
	return goActionId;
}

void RFID::setGoActionParam(std::string goActionParam)
{
	this->goActionParam = goActionParam;
}

std::string RFID::getGoActionParam()
{
	return goActionParam;
}

void RFID::setBackActionId(int backActionId)
{
	this->backActionId = backActionId;
}

int RFID::getBackActionId()
{
	return backActionId;
}

void RFID::setBackActionParam(std::string backActionParam)
{
	this->backActionParam = backActionParam;
}

std::string RFID::getBackActionParam()
{
	return backActionParam;
}

void RFID::setRobotId(int robotId)
{
	this->robotId = robotId;
}

int RFID::getRobotId()
{
	return robotId;
}

