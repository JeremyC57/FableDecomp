// Behaviour test for 0x00CA8370 CBasePin::ConnectionMediaType (standalone copy of the logic; lock and media-type helpers stubbed)
#include <cstdio>
#include <cstring>
typedef int HRESULT;
struct MT { int major; int sampleSize; int fixed; };
static int g_locked, g_maxLocked;
struct Lock { Lock() { ++g_locked; if (g_locked > g_maxLocked) g_maxLocked = g_locked; } ~Lock() { --g_locked; } };
static HRESULT CopyMediaType(MT* t, const MT* s) { *t = *s; return 0; }
static void InitMediaType(MT* t) { std::memset(t, 0, sizeof(*t)); t->sampleSize = 1; t->fixed = 1; }
struct Pin {
    void* m_Connected; MT m_mt;
    HRESULT ConnectionMediaType(MT* pmt)
    {
        if (pmt == 0) return (HRESULT)0x80004003L;
        Lock cObjectLock;
        if (m_Connected) { CopyMediaType(pmt, &m_mt); return 0; }
        InitMediaType(pmt);
        return (HRESULT)0x80040209L;
    }
};
int main()
{
    Pin p; p.m_Connected = 0; p.m_mt.major = 7; p.m_mt.sampleSize = 9; p.m_mt.fixed = 0;
    MT out = { 3, 3, 3 };
    if (p.ConnectionMediaType(0) != (HRESULT)0x80004003L || g_maxLocked != 0) { std::printf("00ca8370_TEST FAIL\n"); return 1; }
    if (p.ConnectionMediaType(&out) != (HRESULT)0x80040209L || out.major != 0 || out.sampleSize != 1) { std::printf("00ca8370_TEST FAIL\n"); return 1; }
    p.m_Connected = &p;
    if (p.ConnectionMediaType(&out) != 0 || out.major != 7 || out.sampleSize != 9 || g_locked != 0) { std::printf("00ca8370_TEST FAIL\n"); return 1; }
    std::printf("00ca8370_TEST PASS\n");
    return 0;
}