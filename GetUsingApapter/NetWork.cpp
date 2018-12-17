#include "stdafx.h"
#include <WinSock2.h>
#include "NetWork.h"
#include <iphlpapi.h>
#include <set>
#include <string>
#include <vector>

#pragma comment (lib, "IPHLPAPI.lib")
namespace NetWork{
	std::set<std::string> set_des{};
	std::vector<int> vec_index_adapter{};

	void GetAdapterDescription()
	{
		PIP_ADAPTER_INFO pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[1024 * 10];
		memset(pIpAdapterInfo, 0, 10240);
		unsigned long size = 1024 * 10;
		int ret = GetAdaptersInfo(pIpAdapterInfo, &size);
		if (ERROR_INSUFFICIENT_BUFFER == ret)
		{
			delete pIpAdapterInfo;
			pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[size];
			ret = GetAdaptersInfo(pIpAdapterInfo, &size);
		}
		PIP_ADAPTER_INFO pIpAdapterInfoHead = pIpAdapterInfo;
		if (ret == ERROR_SUCCESS)
		{
			while (pIpAdapterInfoHead)
			{
				set_des.insert(pIpAdapterInfoHead->Description);
				pIpAdapterInfoHead = pIpAdapterInfoHead->Next;
			}
		}
		if (pIpAdapterInfo != nullptr)
		{
			delete[] pIpAdapterInfo;
			pIpAdapterInfo = nullptr;
		}
	}

	int GetSelectedAdapter()
	{
		GetAdapterDescription();
		vec_index_adapter.clear();
		PMIB_IFTABLE pmib_iftable = nullptr;
		DWORD adapters = 0;
		int connect_selected = 0;
		unsigned int max_in_out_bytes = 0;
		unsigned int in_out_bytes = 0;

		unsigned long ret = GetIfTable(pmib_iftable, &adapters, false);
		if (ret == ERROR_NOT_SUPPORTED)
		{
			return -1;
		}
		if (ERROR_INSUFFICIENT_BUFFER == ret)
		{
			if (pmib_iftable == nullptr)
			{
				pmib_iftable = (PMIB_IFTABLE) new BYTE[adapters];
			}
		}
		if (GetIfTable(pmib_iftable, &adapters, false) == ERROR_SUCCESS)
		{
			for (int i = 0; i < pmib_iftable->dwNumEntries; ++i)
			{
				MIB_IFROW row = pmib_iftable->table[i];
				std::string des = (const char*)row.bDescr;
				if (set_des.find(des) != set_des.end())
				{
					vec_index_adapter.push_back(i);
				}
			}
		}
		else
		{
			return -1;
		}
		for (auto i = 0; i < vec_index_adapter.size(); ++i)
		{
			in_out_bytes = pmib_iftable->table[vec_index_adapter.at(i)].dwOutOctets + pmib_iftable->table[vec_index_adapter.at(i)].dwOutOctets;
			if (in_out_bytes > max_in_out_bytes)
			{
				max_in_out_bytes = in_out_bytes;
				connect_selected = i;
			}
		}
		return connect_selected;
	}

}

