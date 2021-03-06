//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : T3C_SLAVE
//  @ File Name : MoveTaskComponent.h
//  @ Date : 2019-06-25
//  @ Author : ShunWang
//
//


#if !defined(_MOVETASKCOMPONENT_H)
#define _MOVETASKCOMPONENT_H
#include <iostream>
#include <ctime>
#include "ThirdParty/jsoncpp/json.h"

#define JSON_EXEC_ID "g_execId"
#define JSON_EXEC_RESULT "g_execResult"

class MoveTaskComponent {
public:
    MoveTaskComponent();

	time_t _createTime;
	Json::Value _paraJson;          //command共用参数

	void setParaJson(Json::Value& paraJson);
	Json::Value& getParaJson();

	MoveTaskComponent *_parent;
	virtual void add(MoveTaskComponent *taskComponent);
	virtual void remove(MoveTaskComponent *taskComponent);
	virtual void init();
	virtual int getChildSize();
	virtual MoveTaskComponent* findLeftMostChild();
	virtual MoveTaskComponent* getChild(int index);
	void setParent(MoveTaskComponent* parent);
	MoveTaskComponent* getParent();	
	void setCreateTime(time_t createTime);
	time_t getCreateTime();
	long cacuSurvivalTime();
	static void deleteDownDeep(MoveTaskComponent* component); //向下深度删除
	static void clearNullParent(MoveTaskComponent* component); //清理空的父节点
	static void iterativeDelete(MoveTaskComponent* component); //迭代删除
};

#endif  //_MOVETASKCOMPONENT_H
