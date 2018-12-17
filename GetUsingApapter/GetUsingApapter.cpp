// GetUsingApapter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "NetWork.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << NetWork::GetSelectedAdapter() << std::endl;
	return 0;
}

