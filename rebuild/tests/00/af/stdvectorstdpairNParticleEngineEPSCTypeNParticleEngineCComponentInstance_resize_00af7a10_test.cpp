// behaviour test for vector<pair<EPSCType, CComponentInstance> >::resize(n) (0x00AF7A10)
#include <cstdio>
static int g_bases = 0;
struct SCountBlock { int m_references; };
struct CCountedPointer { void* Data; SCountBlock* m_pCount; CCountedPointer() : Data(0), m_pCount(0) {}
    ~CCountedPointer() { if (m_pCount) --m_pCount->m_references; Data = 0; m_pCount = 0; } };
struct CBaseClass { CBaseClass() { ++g_bases; } virtual ~CBaseClass() { --g_bases; } };
struct CComponentInstance : CBaseClass { CCountedPointer Component; bool Enabled; unsigned int InstanceName; CComponentInstance() : Enabled(true) {} };
struct SComponentPair { int first; CComponentInstance second; SComponentPair() : first(0), second() {} };
static unsigned int g_n = 0; static bool g_ok = false;
struct CComponentInstanceVector {
    void resize(unsigned int n, const SComponentPair& v) { g_n = n; g_ok = v.first == 0 && v.second.Enabled && v.second.Component.Data == 0 && v.second.Component.m_pCount == 0 && g_bases == 1; }
    void resize(unsigned int n) { resize(n, SComponentPair()); }
};
int main()
{
    CComponentInstanceVector v; v.resize(5);
    if (g_n != 5 || !g_ok || g_bases != 0) { std::printf("00af7a10_TEST FAIL\n"); return 1; }
    std::printf("00af7a10_TEST PASS\n");
    return 0;
}