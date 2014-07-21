//HardInfo.h
//#include"Instdrv.c"

//#include <winioctl.h>
#include <atlstr.h>

// IOCTL控制码
//#define DFP_SEND_DRIVE_COMMAND   0x0007c084
#define DFP_SEND_DRIVE_COMMAND   CTL_CODE(IOCTL_DISK_BASE, 0x0021, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
//#define DFP_RECEIVE_DRIVE_DATA   0x0007c088
#define DFP_RECEIVE_DRIVE_DATA   CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define FILE_DEVICE_SCSI         0x0000001b
#define IOCTL_SCSI_MINIPORT_IDENTIFY ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define IOCTL_SCSI_MINIPORT 0x0004D008   // see NTDDSCSI.H for definition

// ATA/ATAPI指令
#define IDE_ATA_IDENTIFY    0xEC   // ATA的ID指令(IDENTIFY DEVICE)

// IDE设备IOCTL输出数据结构
//typedef struct _SENDCMDOUTPARAMS
//{
//	DWORD         cBufferSize;   // 缓冲区字节数
//	DRIVERSTATUS DriverStatus; // 驱动程序返回状态
//	BYTE          bBuffer[1];    // 输入缓冲区(此处象征性地包含1字节)
//} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

// IDE的ID命令返回的数据
// 共512字节(256个WORD)，这里仅定义了一些感兴趣的项
typedef struct _IDINFO
{
	USHORT wGenConfig;      // WORD 0: 基本信息字
	USHORT wNumCyls;      // WORD 1: 柱面数
	USHORT wReserved2;      // WORD 2: 保留
	USHORT wNumHeads;      // WORD 3: 磁头数
	USHORT wReserved4;        // WORD 4: 保留
	USHORT wReserved5;        // WORD 5: 保留
	USHORT wNumSectorsPerTrack;   // WORD 6: 每磁道扇区数
	USHORT wVendorUnique[3];    // WORD 7-9: 厂家设定值
	CHAR    sSerialNumber[20];    // WORD 10-19:序列号
	USHORT wBufferType;     // WORD 20: 缓冲类型
	USHORT wBufferSize;     // WORD 21: 缓冲大小
	USHORT wECCSize;      // WORD 22: ECC校验大小
	CHAR    sFirmwareRev[8];    // WORD 23-26: 固件版本
	CHAR    sModelNumber[40];    // WORD 27-46: 内部型号
	USHORT wMoreVendorUnique;    // WORD 47: 厂家设定值
	USHORT wReserved48;     // WORD 48: 保留
	struct {
		USHORT reserved1:8;
		USHORT DMA:1;      // 1=支持DMA
		USHORT LBA:1;      // 1=支持LBA
		USHORT DisIORDY:1;     // 1=可不使用IORDY
		USHORT IORDY:1;     // 1=支持IORDY
		USHORT SoftReset:1;    // 1=需要ATA软启动
		USHORT Overlap:1;     // 1=支持重叠操作
		USHORT Queue:1;     // 1=支持命令队列
		USHORT InlDMA:1;     // 1=支持交叉存取DMA
	} wCapabilities;      // WORD 49: 一般能力
	USHORT wReserved1;      // WORD 50: 保留
	USHORT wPIOTiming;      // WORD 51: PIO时序
	USHORT wDMATiming;      // WORD 52: DMA时序
	struct {
		USHORT CHSNumber:1;    // 1=WORD 54-58有效
		USHORT CycleNumber:1;    // 1=WORD 64-70有效
		USHORT UnltraDMA:1;    // 1=WORD 88有效
		USHORT reserved:13;
	} wFieldValidity;      // WORD 53: 后续字段有效性标志
	USHORT wNumCurCyls;     // WORD 54: CHS可寻址的柱面数
	USHORT wNumCurHeads;     // WORD 55: CHS可寻址的磁头数
	USHORT wNumCurSectorsPerTrack;   // WORD 56: CHS可寻址每磁道扇区数
	USHORT wCurSectorsLow;     // WORD 57: CHS可寻址的扇区数低位字
	USHORT wCurSectorsHigh;    // WORD 58: CHS可寻址的扇区数高位字
	struct {
		USHORT CurNumber:8;    // 当前一次性可读写扇区数
		USHORT Multi:1;     // 1=已选择多扇区读写
		USHORT reserved1:7;
	} wMultSectorStuff;      // WORD 59: 多扇区读写设定
	ULONG dwTotalSectors;     // WORD 60-61: LBA可寻址的扇区数
	USHORT wSingleWordDMA;     // WORD 62: 单字节DMA支持能力
	struct {
		USHORT Mode0:1;     // 1=支持模式0 (4.17Mb/s)
		USHORT Mode1:1;     // 1=支持模式1 (13.3Mb/s)
		USHORT Mode2:1;     // 1=支持模式2 (16.7Mb/s)
		USHORT Reserved1:5;
		USHORT Mode0Sel:1;     // 1=已选择模式0
		USHORT Mode1Sel:1;     // 1=已选择模式1
		USHORT Mode2Sel:1;     // 1=已选择模式2
		USHORT Reserved2:5;
	} wMultiWordDMA;      // WORD 63: 多字节DMA支持能力
	struct {
		USHORT AdvPOIModes:8;    // 支持高级POI模式数
		USHORT reserved:8;
	} wPIOCapacity;       // WORD 64: 高级PIO支持能力
	USHORT wMinMultiWordDMACycle;   // WORD 65: 多字节DMA传输周期的最小值
	USHORT wRecMultiWordDMACycle;   // WORD 66: 多字节DMA传输周期的建议值
	USHORT wMinPIONoFlowCycle;    // WORD 67: 无流控制时PIO传输周期的最小值
	USHORT wMinPOIFlowCycle;    // WORD 68: 有流控制时PIO传输周期的最小值
	USHORT wReserved69[11];    // WORD 69-79: 保留
	struct {
		USHORT Reserved1:1;
		USHORT ATA1:1;      // 1=支持ATA-1
		USHORT ATA2:1;      // 1=支持ATA-2
		USHORT ATA3:1;      // 1=支持ATA-3
		USHORT ATA4:1;      // 1=支持ATA/ATAPI-4
		USHORT ATA5:1;      // 1=支持ATA/ATAPI-5
		USHORT ATA6:1;      // 1=支持ATA/ATAPI-6
		USHORT ATA7:1;      // 1=支持ATA/ATAPI-7
		USHORT ATA8:1;      // 1=支持ATA/ATAPI-8
		USHORT ATA9:1;      // 1=支持ATA/ATAPI-9
		USHORT ATA10:1;     // 1=支持ATA/ATAPI-10
		USHORT ATA11:1;     // 1=支持ATA/ATAPI-11
		USHORT ATA12:1;     // 1=支持ATA/ATAPI-12
		USHORT ATA13:1;     // 1=支持ATA/ATAPI-13
		USHORT ATA14:1;     // 1=支持ATA/ATAPI-14
		USHORT Reserved2:1;
	} wMajorVersion;      // WORD 80: 主版本
	USHORT wMinorVersion;     // WORD 81: 副版本
	USHORT wReserved82[6];     // WORD 82-87: 保留
	struct {
		USHORT Mode0:1;     // 1=支持模式0 (16.7Mb/s)
		USHORT Mode1:1;     // 1=支持模式1 (25Mb/s)
		USHORT Mode2:1;     // 1=支持模式2 (33Mb/s)
		USHORT Mode3:1;     // 1=支持模式3 (44Mb/s)
		USHORT Mode4:1;     // 1=支持模式4 (66Mb/s)
		USHORT Mode5:1;     // 1=支持模式5 (100Mb/s)
		USHORT Mode6:1;     // 1=支持模式6 (133Mb/s)
		USHORT Mode7:1;     // 1=支持模式7 (166Mb/s) ???
		USHORT Mode0Sel:1;     // 1=已选择模式0
		USHORT Mode1Sel:1;     // 1=已选择模式1
		USHORT Mode2Sel:1;     // 1=已选择模式2
		USHORT Mode3Sel:1;     // 1=已选择模式3
		USHORT Mode4Sel:1;     // 1=已选择模式4
		USHORT Mode5Sel:1;     // 1=已选择模式5
		USHORT Mode6Sel:1;     // 1=已选择模式6
		USHORT Mode7Sel:1;     // 1=已选择模式7
	} wUltraDMA;       // WORD 88: Ultra DMA支持能力
	USHORT    wReserved89[167];    // WORD 89-255
} IDINFO, *PIDINFO;

// SCSI驱动所需的输入输出共用的结构
typedef struct _SRB_IO_CONTROL
{
	ULONG HeaderLength;   // 头长度
	UCHAR Signature[8];   // 特征名称
	ULONG Timeout;    // 超时时间
	ULONG ControlCode;   // 控制码
	ULONG ReturnCode;   // 返回码
	ULONG Length;    // 缓冲区长度
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

// 打开设备
// filename: 设备的“文件名”
HANDLE OpenDevice(LPCTSTR filename)
{
	HANDLE hDevice;

	// 打开设备
	hDevice= CreateFile(filename,      // 文件名
		GENERIC_READ | GENERIC_WRITE,    // 读写方式
		FILE_SHARE_READ | FILE_SHARE_WRITE,   // 共享方式
		NULL,          // 默认的安全描述符
		OPEN_EXISTING,        // 创建方式
		0,           // 不需设置文件属性
		NULL);          // 不需参照模板文件

	return hDevice;
}

// 向驱动发“IDENTIFY DEVICE”命令，获得设备信息
// hDevice: 设备句柄
// pIdInfo: 设备信息结构指针
BOOL IdentifyDevice(HANDLE hDevice, PIDINFO pIdInfo)
{
	PSENDCMDINPARAMS pSCIP;   // 输入数据结构指针
	PSENDCMDOUTPARAMS pSCOP; // 输出数据结构指针
	DWORD dwOutBytes;    // IOCTL输出数据长度
	BOOL bResult;     // IOCTL返回值

	// 申请输入/输出数据结构空间
	pSCIP = (PSENDCMDINPARAMS)::GlobalAlloc(LMEM_ZEROINIT, sizeof(SENDCMDINPARAMS)-1);
	pSCOP = (PSENDCMDOUTPARAMS)::GlobalAlloc(LMEM_ZEROINIT, sizeof(SENDCMDOUTPARAMS)+sizeof(IDINFO)-1);

	// 指定ATA/ATAPI命令的寄存器值
	// pSCIP->irDriveRegs.bFeaturesReg = 0;
	// pSCIP->irDriveRegs.bSectorCountReg = 0;
	// pSCIP->irDriveRegs.bSectorNumberReg = 0;
	// pSCIP->irDriveRegs.bCylLowReg = 0;
	// pSCIP->irDriveRegs.bCylHighReg = 0;
	// pSCIP->irDriveRegs.bDriveHeadReg = 0;
	pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;

	// 指定输入/输出数据缓冲区大小
	pSCIP->cBufferSize = 0;
	pSCOP->cBufferSize = sizeof(IDINFO);

	// IDENTIFY DEVICE
	bResult = DeviceIoControl(hDevice,       // 设备句柄
		DFP_RECEIVE_DRIVE_DATA,          // 指定IOCTL
		pSCIP, sizeof(SENDCMDINPARAMS) - 1,       // 输入数据缓冲区
		pSCOP, sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1, // 输出数据缓冲区
		&dwOutBytes,            // 输出数据长度
		(LPOVERLAPPED)NULL);          // 用同步I/O

	// 复制设备参数结构
	memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO));

	// 释放输入/输出数据空间
	GlobalFree(pSCOP);
	GlobalFree(pSCIP);

	return bResult;
}

// 向SCSI MINI-PORT驱动发“IDENTIFY PACKET DEVICE”命令，获得设备信息
// hDevice: 设备句柄
// pIdInfo: 设备信息结构指针
BOOL IdentifyDeviceAsScsi(HANDLE hDevice, int nDrive, PIDINFO pIdInfo)
{
	PSENDCMDINPARAMS pSCIP;   // 输入数据结构指针
	PSENDCMDOUTPARAMS pSCOP; // 输出数据结构指针
	PSRB_IO_CONTROL pSRBIO;   // SCSI输入输出数据结构指针
	DWORD dwOutBytes;    // IOCTL输出数据长度
	BOOL bResult;     // IOCTL返回值

	// 申请输入/输出数据结构空间
	pSRBIO = (PSRB_IO_CONTROL)::GlobalAlloc(LMEM_ZEROINIT, sizeof(SRB_IO_CONTROL)+sizeof(SENDCMDOUTPARAMS)+sizeof(IDINFO)-1);
	pSCIP = (PSENDCMDINPARAMS)((char *)pSRBIO+sizeof(SRB_IO_CONTROL));
	pSCOP = (PSENDCMDOUTPARAMS)((char *)pSRBIO+sizeof(SRB_IO_CONTROL));

	// 填充输入/输出数据
	pSRBIO->HeaderLength = sizeof(SRB_IO_CONTROL);
	pSRBIO->Timeout = 10000;
	pSRBIO->Length = sizeof(SENDCMDOUTPARAMS)+sizeof(IDINFO)-1;
	pSRBIO->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
	::strncpy ((char *)pSRBIO->Signature, "SCSIDISK", 8);

	// 指定ATA/ATAPI命令的寄存器值
	// pSCIP->irDriveRegs.bFeaturesReg = 0;
	// pSCIP->irDriveRegs.bSectorCountReg = 0;
	// pSCIP->irDriveRegs.bSectorNumberReg = 0;
	// pSCIP->irDriveRegs.bCylLowReg = 0;
	// pSCIP->irDriveRegs.bCylHighReg = 0;
	// pSCIP->irDriveRegs.bDriveHeadReg = 0;
	pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
	pSCIP->bDriveNumber = nDrive;

	// IDENTIFY DEVICE
	bResult = DeviceIoControl(hDevice,       // 设备句柄
		IOCTL_SCSI_MINIPORT,          // 指定IOCTL
		pSRBIO, sizeof(SRB_IO_CONTROL) +sizeof(SENDCMDINPARAMS) - 1,       // 输入数据缓冲区
		pSRBIO, sizeof(SRB_IO_CONTROL) +sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1, // 输出数据缓冲区
		&dwOutBytes,            // 输出数据长度
		(LPOVERLAPPED)NULL);          // 用同步I/O

	// 复制设备参数结构
	memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO));

	// 释放输入/输出数据空间
	GlobalFree(pSRBIO);

	return bResult;
}

// 将串中的字符两两颠倒
// 原因是ATA/ATAPI中的WORD，与Windows采用的字节顺序相反
// 驱动程序中已经将收到的数据全部反过来，我们来个负负得正
void AdjustString(char* str, int len)
{
	char ch;
	int i;

	// 两两颠倒
	for(i=0;i<len;i+=2)
	{
		ch = str[i];
		str[i] = str[i+1];
		str[i+1] = ch;
	}

	// 若是右对齐的，调整为左对齐 (去掉左边的空格)
	i=0;
	while(i<len && str[i]==' ') i++;

	if(i)
	{
		memmove(str, &str[i], len-i);
		for(int j = len - i; j < len; j++)
		{
			str[j] = ' ';
		}
	}

	// 去掉右边的空格
	i = len - 1;
	while(i>=0 && str[i]==' ')
	{
		str[i] = '\0';
		i--;
	}
}

// 读取IDE硬盘的设备信息，必须有足够权限
// nDrive: 驱动器号(0=第一个硬盘，1=0=第二个硬盘，......)
// pIdInfo: 设备信息结构指针
BOOL GetPhysicalDriveInfoInNT(int nDrive, PIDINFO pIdInfo)
{
	HANDLE hDevice;    // 设备句柄
	BOOL bResult;    // 返回结果
	char szFileName[20]; // 文件名

	sprintf(szFileName,"\\\\.\\PhysicalDrive%d", nDrive);
	
	CString str(szFileName);
	hDevice = OpenDevice(str);

	if(hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	// IDENTIFY DEVICE
	bResult = IdentifyDevice(hDevice, pIdInfo);

	if(bResult)
	{
		// 调整字符串
		AdjustString(pIdInfo->sSerialNumber, 20);
		AdjustString(pIdInfo->sModelNumber, 40);
		AdjustString(pIdInfo->sFirmwareRev, 8);
	}

	CloseHandle (hDevice);

	return bResult;
}

// 用SCSI驱动读取IDE硬盘的设备信息，不受权限制约
// nDrive: 驱动器号(0=Primary Master, 1=Promary Slave, 2=Secondary master, 3=Secondary slave)
// pIdInfo: 设备信息结构指针
BOOL GetIdeDriveAsScsiInfoInNT(int nDrive, PIDINFO pIdInfo)
{
	HANDLE hDevice;    // 设备句柄
	BOOL bResult;    // 返回结果
	char szFileName[20]; // 文件名

	sprintf(szFileName,"\\\\.\\Scsi%d", nDrive/2);
	CString str(szFileName);
	hDevice = OpenDevice(str);

	if(hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	// IDENTIFY DEVICE
	bResult = IdentifyDeviceAsScsi(hDevice, nDrive%2, pIdInfo);

	// 检查是不是空串
	if(pIdInfo->sModelNumber[0]=='\0')
	{
		bResult = FALSE;
	}

	if(bResult)
	{
		// 调整字符串
		AdjustString(pIdInfo->sSerialNumber, 20);
		AdjustString(pIdInfo->sModelNumber, 40);
		AdjustString(pIdInfo->sFirmwareRev, 8);
	}
	CloseHandle (hDevice);
	return bResult;
}