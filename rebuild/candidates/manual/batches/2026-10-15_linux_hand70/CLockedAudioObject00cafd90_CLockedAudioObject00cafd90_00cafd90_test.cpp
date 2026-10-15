// Behaviour test for CLockedAudioObject00cafd90::~CLockedAudioObject00cafd90 @ 0x00CAFD90: lock deleted first, then the second base.
#include <cstdio>

static int g_order[4]; static int g_n = 0;
struct CRITICAL_SECTION_00cafd90 { int m_live; };
static void DeleteCriticalSection(CRITICAL_SECTION_00cafd90* s) { s->m_live = 0; g_order[g_n++] = 1; }
struct CAudioRoot00cafd90 { virtual void v00() {} virtual ~CAudioRoot00cafd90() {} };
struct CAudioObjectBase00caefb0 { virtual ~CAudioObjectBase00caefb0() { g_order[g_n++] = 2; } long m_body[4]; };
struct CLockedAudioObject00cafd90 : CAudioRoot00cafd90, CAudioObjectBase00caefb0 {
    CRITICAL_SECTION_00cafd90 m_lock;
    CLockedAudioObject00cafd90() { m_lock.m_live = 1; }
    virtual ~CLockedAudioObject00cafd90() { DeleteCriticalSection(&m_lock); }
};

int main()
{
    CAudioObjectBase00caefb0* p = new CLockedAudioObject00cafd90;
    delete p;
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("00cafd90_TEST FAIL\n"); return 1; }
    std::printf("00cafd90_TEST PASS\n");
    return 0;
}
