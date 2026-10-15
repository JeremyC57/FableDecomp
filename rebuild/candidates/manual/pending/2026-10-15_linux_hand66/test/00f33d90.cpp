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
// Behaviour test for 00f33d90: resize(count) forwards to resize(count, value) with a default-constructed temporary that dies afterwards.
static int g_constructed = 0;
static int g_destroyed = 0;
static unsigned long g_count = 0;
static long g_valueTag = 0;
static int g_liveDuringCall = 0;
struct CDefRecord34 {
    long m_tag;
    CDefRecord34() : m_tag(77) { ++g_constructed; }
    ~CDefRecord34() { ++g_destroyed; }
};
struct CVectorOfCDefRecord34 {
    CDefRecord34* m_pFirst; CDefRecord34* m_pLast; CDefRecord34* m_pEnd;
    void Resize(unsigned long count, const CDefRecord34& value) { g_count = count; g_valueTag = value.m_tag; g_liveDuringCall = g_constructed - g_destroyed; }
    void Resize(unsigned long count);
};
void CVectorOfCDefRecord34::Resize(unsigned long count)
{
    Resize(count, CDefRecord34());
}
int main()
{
    CVectorOfCDefRecord34 v; v.m_pFirst = v.m_pLast = v.m_pEnd = 0;
    v.Resize(12);
    if (g_count != 12 || g_valueTag != 77 || g_liveDuringCall != 1 || g_constructed != 1 || g_destroyed != 1) { std::printf("00f33d90_TEST FAIL\n"); return 1; }
    std::printf("00f33d90_TEST PASS\n");
    return 0;
}
