// GetUsingApapter.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "NetWork.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << NetWork::GetSelectedAdapter() << std::endl;
	return 0;
}

