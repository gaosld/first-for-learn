#include "stdafx.h"
#include <WinSock2.h>
#include <IPHlpApi.h>
#include "GetMachineID.h"
#include "HardDiskInfo.h"
#include "MD5.h"

#pragma comment(lib, "IPHlpApi.lib")

void GetMacAddr( char *sz_mac_addr )
{
	PIP_ADAPTER_INFO p_ip_adapter_info = new IP_ADAPTER_INFO();
	unsigned long size_ip_adapter_info = sizeof(PIP_ADAPTER_INFO);
	int i_ret = GetAdaptersInfo(p_ip_adapter_info, &size_ip_adapter_info);
	int num_ip_adapter = 0;
	if(ERROR_BUFFER_OVERFLOW == i_ret)
	{
		delete p_ip_adapter_info;
		p_ip_adapter_info = (PIP_ADAPTER_INFO)new char[size_ip_adapter_info];
		i_ret = GetAdaptersInfo(p_ip_adapter_info, &size_ip_adapter_info);
	}
	if(ERROR_SUCCESS == i_ret)
	{
		while(p_ip_adapter_info)
		{
			if(IsPhysicalAdapter(p_ip_adapter_info->AdapterName))
			{
				for(DWORD i = 0; i < p_ip_adapter_info->AddressLength; i++)
				{
					if(i < p_ip_adapter_info->AddressLength - 1)
					{
						sprintf(sz_mac_addr + i * 3, "%02X-", p_ip_adapter_info->Address[i]);
					}
					else
					{
						sprintf(sz_mac_addr + i * 3, "%02X", p_ip_adapter_info->Address[i]);
					}
				}
				return;
			}
			p_ip_adapter_info = p_ip_adapter_info->Next;
		}
	}
	strcpy(sz_mac_addr, "00-00-00-00-00-00");
}

void GetHardDiskSN( char *sz_hard_disk_sn )
{
	IDINFO IdInfo; 
	int i = 0, j = 0;     
	if(GetPhysicalDriveInfoInNT(i,&IdInfo))
	{
		for(j = 0; j < 20; j++)
		{
			sz_hard_disk_sn[j] = IdInfo.sSerialNumber[j];
		}
		sz_hard_disk_sn[j] = '\0';
		//printf(" serial numbers:%s\n",sz_hard_disk_sn); 
	} 
	else if(GetIdeDriveAsScsiInfoInNT(i,&IdInfo))
	{
		for(j = 0; j < 20; j++)
		{
			sz_hard_disk_sn[j] = IdInfo.sSerialNumber[i];
		}
		sz_hard_disk_sn[j] = '\0';
		//printf("Serial Numbers:%s\n",sz_hard_disk_sn);                                       
	}
}

void GetMachineID( char *sz_machine_id )
{
	char sz_mac_addr[18], sz_harddisk_sn[21], sz_machine[38];
	GetMacAddr(sz_mac_addr);
	GetHardDiskSN(sz_harddisk_sn);
	strcpy(sz_machine, sz_mac_addr);
	strcat(sz_machine, sz_harddisk_sn);
	//printf("mac addr + hd sn = %s\n", sz_machine);
	strcpy(sz_machine_id, md5(sz_machine).c_str());
}

bool IsPhysicalAdapter( char* sz_adapter_name )
{
	HKEY h_key;
	LPCTSTR sz_sub_key = TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}");
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, sz_sub_key, 0, KEY_READ, &h_key) != ERROR_SUCCESS)
	{
		printf("RegOpenKeyEx error");
		return false;
	}

	DWORD count_key = 0, dw_max_key_len = 0;
	if(RegQueryInfoKey(h_key, NULL, NULL, NULL, &count_key, &dw_max_key_len, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		printf("RegQueryInfoKey error");
		RegCloseKey(h_key);
		return false;
	}

	DWORD dw_key_len = 0, dw_value_len = 0;
	LPTSTR sz_key_name = NULL;
	TCHAR sz_value_guid[40];

	DWORD dw_key_index = 0;
	for(; dw_key_index < count_key; dw_key_index++)
	{
		dw_key_len = dw_max_key_len + 1;
		sz_key_name = (LPTSTR)malloc(dw_key_len);
		if(RegEnumKeyEx(h_key, dw_key_index, sz_key_name, &dw_key_len, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
		{
			continue;
		}
		if(RegGetValue(h_key, sz_key_name, TEXT("NetCfgInstanceId"), RRF_RT_ANY, NULL, sz_value_guid, &dw_value_len) != ERROR_SUCCESS)
		{
			continue;
		}
		char temp[40];
		wcstombs(temp, sz_value_guid, 40);
		if(strcmp(sz_adapter_name, temp) == 0)
		{
			DWORD dw_value;
			RegGetValue(h_key, sz_key_name, TEXT("Characteristics"), RRF_RT_REG_DWORD, NULL, &dw_value, &dw_value_len);
			if((dw_value & 0x4) == 0x4)
			{
				free(sz_key_name);
				return true;
			}
		}
		free(sz_key_name);
	}
	return false;
}



