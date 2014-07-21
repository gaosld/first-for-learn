// MachineIDToMD5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "GetMachineID.h"
#include "MD5.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char machine_id[33];
	GetMachineID(machine_id);
	std::cout << machine_id<< std::endl;
	getchar();
	return 0;
}

