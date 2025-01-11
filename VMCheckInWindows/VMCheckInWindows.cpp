// VMCheckInWindows.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <Windows.h>
#include <versionhelpers.h>
#include <intrin.h>

int CheckVirtualStatus()
{
    int cpuinfo[4] = { 0 };// EAX、EBX、ECX 和 EDX
    int vendor[3] = { 0 };
    int flag = 0;
    __cpuid(cpuinfo, 1);
    if (cpuinfo[2] < 0)
    {
        __cpuid(cpuinfo, 0x40000000);
        vendor[0] = cpuinfo[1];
        vendor[1] = cpuinfo[2];
        vendor[2] = cpuinfo[3];
        if (!strcmp((char*)vendor, "Microsoft Hv"))
        {
            __cpuid(cpuinfo, 0x40000003);
            return (cpuinfo[1] & 1) + 1;
        }
        else
        {
            return 1;
        }
    }
    __cpuid(cpuinfo, 0);
    if (cpuinfo[1] == 'uneG' && cpuinfo[3] == 'Ieni' && cpuinfo[2] == 'letn') //GenuineIntel
    {
        __cpuid(cpuinfo, 1);
        flag = (cpuinfo[2] >> 5) & 1;
    }
    else if (cpuinfo[1] == 'htuA' && cpuinfo[3] == 'itne' && cpuinfo[2] == 'DMAc') // AuthenticAMD
    {
        __cpuid(cpuinfo, 0x80000001);
        flag = (cpuinfo[2] >> 2) & 1;
    }
    if (flag)
    {
        if (IsProcessorFeaturePresent(0x15))
        {
            return 3;
        }
        else if (IsWindowsServer() || IsProcessorFeaturePresent(0x14))
        {
            return 6 - IsProcessorFeaturePresent(0xC);
        }
        else
        {
            return 6;
        }
    }
    return 0;
}
int main()
{
    printf("VirtualStatus is %d\n", CheckVirtualStatus());
    getchar();
    return 0;
}
