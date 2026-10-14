#include <cstdio>
#include <stddef.h>
struct CCharStringData { const char* m_pData; unsigned long m_length; };
static int g_defaultCtors = 0, g_assigns = 0;
struct CCharString {
    CCharStringData* m_pData;
    CCharString() : m_pData(0) { ++g_defaultCtors; }
    CCharString(CCharStringData* d) : m_pData(d) { }
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; ++g_assigns; return *this; }
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};
static const void* g_calcData = 0; static unsigned long g_calcLen = 99, g_calcSeed = 99;
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length) { g_calcSeed = seed; g_calcData = data; g_calcLen = length; return 0x1234 + length; }
struct CAnimFilterOfGroup {
    virtual void v00() { }
    CCharString Group; unsigned long GroupCRC;
    CAnimFilterOfGroup(const CCharString& name) : Group() { Group = name; GroupCRC = Calc_004014a0(0, name.GetPString(), name.GetLength()); }
};
int main()
{
    CCharStringData data = { "WALK", 4 };
    CCharString name(&data);
    CAnimFilterOfGroup f(name);
    if (g_defaultCtors != 1 || g_assigns != 1 || f.Group.m_pData != &data || g_calcSeed != 0 || g_calcData != data.m_pData || g_calcLen != 4 || f.GroupCRC != 0x1238
        || offsetof(CAnimFilterOfGroup, Group) != 4 || offsetof(CAnimFilterOfGroup, GroupCRC) != 8) { std::printf("0073e8f0_TEST FAIL\n"); return 1; }
    CCharString empty((CCharStringData*)0);
    CAnimFilterOfGroup g(empty);
    if (g_calcLen != 0 || g_calcData == 0 || *(const char*)g_calcData != 0 || g.GroupCRC != 0x1234) { std::printf("0073e8f0_TEST FAIL\n"); return 1; }
    std::printf("0073e8f0_TEST PASS\n"); return 0;
}