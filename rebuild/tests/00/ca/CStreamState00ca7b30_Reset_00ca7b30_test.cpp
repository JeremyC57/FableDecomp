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
// Behaviour test for 00ca7b30: Reset re-reads two values from the source, zeroes the position, empties the node list and returns S_OK.
static int g_removed = 0;
struct CStreamSource {
    long m_a, m_b;
    virtual long GetValue14() { return m_a; }
    virtual long GetValue18() { return m_b; }
};
struct CNodeList {
    void* m_pFirst; void* m_pLast; long m_count;
    void RemoveAll() { ++g_removed; m_pFirst = m_pLast = 0; m_count = 0; }
};
struct CStreamState {
    long m_position;
    long m_value8;
    CStreamSource* m_pSource;
    long m_value10;
    long m_refCount;
    CNodeList m_nodes;
    long __stdcall Reset();
};
long __stdcall CStreamState::Reset()
{
    m_value10 = m_pSource->GetValue14();
    m_value8 = m_pSource->GetValue18();
    m_position = 0;
    m_nodes.RemoveAll();
    return 0;
}
int main()
{
    CStreamSource src; src.m_a = 11; src.m_b = 22;
    CStreamState s; s.m_position = 99; s.m_value8 = 0; s.m_value10 = 0; s.m_pSource = &src; s.m_refCount = 1;
    s.m_nodes.m_pFirst = &s; s.m_nodes.m_pLast = &s; s.m_nodes.m_count = 1;
    long hr = s.Reset();
    if (hr != 0 || s.m_value10 != 11 || s.m_value8 != 22 || s.m_position != 0 || s.m_refCount != 1) { std::printf("00ca7b30_TEST FAIL\n"); return 1; }
    if (1 && (g_removed != 1 || s.m_nodes.m_count != 0 || s.m_nodes.m_pFirst != 0)) { std::printf("00ca7b30_TEST FAIL\n"); return 1; }
    if (!1 && (g_removed != 0 || s.m_nodes.m_count != 1)) { std::printf("00ca7b30_TEST FAIL\n"); return 1; }
    std::printf("00ca7b30_TEST PASS\n");
    return 0;
}