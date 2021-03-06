//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : T3C_SLAVE
//  @ File Name : RFID.h
//  @ Date : 2019-07-03
//  @ Author : ShunWang
//
//


#if !defined(_RFID_H)
#define _RFID_H

#include <string>

class RFID
{
public:
	int id;
	long rfidNum;
	std::string name;
	int functionId;
	int position;
	int goActionId;
	std::string goActionParam;
	int backActionId;
	std::string backActionParam;
	int robotId;
	int type;
	time_t findTime;
	void setId(int id);
	int getId();
	void setRfidNum(long rfidNum);
	long getRfidNum();
	void setName(std::string name);
	std::string getName();
	void setFunctionId(int functionId);
	int getFunctionId();
	void setPosition(int position);
	int getPosition();
	void setGoActionId(int goActionId);
	int getGoActionId();
	void setGoActionParam(std::string goActionParam);
	std::string getGoActionParam();
	void setBackActionId(int backActionId);
	int getBackActionId();
	void setBackActionParam(std::string backActionParam);
	std::string getBackActionParam();
	void setRobotId(int robotId);
	int getRobotId();
	void setType(int type);
	int getType();
	void setFindTime(time_t time);
	time_t getFindTime();
};

#endif  //_RFID_H
