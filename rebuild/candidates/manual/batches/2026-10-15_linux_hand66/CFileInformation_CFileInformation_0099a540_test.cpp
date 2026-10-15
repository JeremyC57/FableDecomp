#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 0099a540: CFileInformation's constructor default-constructs its three CDateAndTime members (zeroed) in order.
static int g_order = 0;
struct CDateAndTime {
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    long m_order;
    CDateAndTime() : Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0), Millisecond(0), m_order(++g_order) {}
};
struct CFileInformation {
    CDateAndTime CreationTime;
    CDateAndTime LastAccessTime;
    CDateAndTime LastWriteTime;
    bool ReadOnly;
    CFileInformation();
};
CFileInformation::CFileInformation()
{
}
int main()
{
    CFileInformation info;
    if (info.CreationTime.m_order != 1 || info.LastAccessTime.m_order != 2 || info.LastWriteTime.m_order != 3 ||
        info.LastWriteTime.Year != 0 || info.CreationTime.Millisecond != 0) { std::printf("0099a540_TEST FAIL\n"); return 1; }
    std::printf("0099a540_TEST PASS\n");
    return 0;
}
