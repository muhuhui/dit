//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : T3C_SLAVE
//  @ File Name : USSwitchCommand.h
//  @ Date : 2019-06-25
//  @ Author : ShunWang
//
//


#if !defined(_LIFTLOCKCOMMAND_H)
#define _LIFTLOCKCOMMAND_H

#include"Transaction/MoveTaskManage/Command.h"

class LiftLockCommand : public Command
{
public:
    LiftLockCommand() { name = "LiftLockCommand"; }
private:
    int exec();
};

#endif
