#ifndef GET_MACHINE_ID_H_
#define GET_MACHINE_ID_H_

//�ж��Ƿ���������
bool IsPhysicalAdapter(char *sz_adapter_name);

//��ȡ��һ������������MAC��ַ
void GetMacAddr(char *sz_mac_addr);

void GetHardDiskSN(char *sz_hard_disk_sn);

void GetMachineID(char *sz_machine_id);

#endif