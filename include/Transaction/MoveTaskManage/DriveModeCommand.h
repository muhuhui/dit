//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : T3C_SLAVE
//  @ File Name : DriveModeCommand.h
//  @ Date : 2019-06-25
//  @ Author : ShunWang
//
//


#if !defined(_DRIVEMODECOMMAND_H)
#define _DRIVEMODECOMMAND_H

#include "Command.h"

class DriveModeCommand : public Command
{
public:
    DriveModeCommand() { name = "DriveModeCommand"; }
private:
	int exec();
};

#endif  //_DRIVEMODECOMMAND_H
