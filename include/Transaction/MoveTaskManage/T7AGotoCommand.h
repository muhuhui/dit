//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : T3C_SLAVE
//  @ File Name : GotoCommand.h
//  @ Date : 2019-06-27
//  @ Author : ShunWang
//
//


#if !defined(_T7AGOTOCOMMAND_H)
#define _T7AGOTOCOMMAND_H

#include "Command.h"
enum T7AGOTO_STATUS
{
	T7AGOTO_ST_SNDCMD = 0,   //发送goto指令
	T7AGOTO_ST_WAITRESULT    //等待执行结果
};

class T7AGotoCommand : public Command
{
public:
    T7AGotoCommand() { name = "T7AGotoCommand"; }
private:
	int exec();
};

#endif  //_T7AGOTOCOMMAND_H
