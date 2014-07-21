#ifndef GET_MACHINE_ID_H_
#define GET_MACHINE_ID_H_

//判断是否物理网卡
bool IsPhysicalAdapter(char *sz_adapter_name);

//获取第一块物理网卡的MAC地址
void GetMacAddr(char *sz_mac_addr);

void GetHardDiskSN(char *sz_hard_disk_sn);

void GetMachineID(char *sz_machine_id);

#endif