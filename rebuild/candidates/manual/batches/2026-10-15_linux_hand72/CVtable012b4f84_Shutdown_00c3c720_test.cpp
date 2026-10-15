#include <cstdio>
// Behaviour model of 0x00C3C720: the derived Shutdown -- base shutdown, then the link release when the master's sink exists.
static int g_baseShutdowns = 0, g_releases = 0;
struct CMaster { void* m_pSink; };
static CMaster* g_pMaster = 0;
struct CResourceLink { void Release() { ++g_releases; } };
struct CBase {
    virtual ~CBase() {}
    unsigned long m_flags;
    virtual void Shutdown() { ++g_baseShutdowns; m_flags = 0; }
};
struct CDerived : CBase {
    CResourceLink m_link;
    virtual void Shutdown()
    {
        if (m_flags) {
            CBase::Shutdown();
            if (g_pMaster && g_pMaster->m_pSink) m_link.Release();
        }
    }
};
int main()
{
    CDerived d; d.m_flags = 0; d.Shutdown();
    if (g_baseShutdowns != 0 || g_releases != 0) { std::printf("00c3c720_TEST FAIL\n"); return 1; }
    d.m_flags = 1; d.Shutdown();               // no master: base only
    if (g_baseShutdowns != 1 || g_releases != 0 || d.m_flags != 0) { std::printf("00c3c720_TEST FAIL\n"); return 1; }
    CMaster m; m.m_pSink = 0; g_pMaster = &m; d.m_flags = 3; d.Shutdown();
    if (g_baseShutdowns != 2 || g_releases != 0) { std::printf("00c3c720_TEST FAIL\n"); return 1; }
    m.m_pSink = &m; d.m_flags = 3; static_cast<CBase&>(d).Shutdown();
    if (g_baseShutdowns != 3 || g_releases != 1) { std::printf("00c3c720_TEST FAIL\n"); return 1; }
    std::printf("00c3c720_TEST PASS\n");
    return 0;
}
