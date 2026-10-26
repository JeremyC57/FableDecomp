#include <cstdio>
#include <new>

// Model of CIntelligentPointer_01238c6c::CIntelligentPointer_01238c6c() @ 0x008840B0
class CBaseObject;
static int g_baseCtorCalls = 0; static CBaseObject* g_setItem = (CBaseObject*)1; static int g_setCalls = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem((CBaseObject*)0xdead) { ++g_baseCtorCalls; }
    void SetPItem(CBaseObject* p) { m_pItem = p; g_setItem = p; ++g_setCalls; }
};
struct CIntelligentPointer_01238c6c : CBaseIntelligentPointer {
    unsigned char m_locked;
    virtual void v00() {}
    CIntelligentPointer_01238c6c() { SetPItem(0); m_locked = 0; }
};
int main()
{
    unsigned char raw[sizeof(CIntelligentPointer_01238c6c)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xff;
    CIntelligentPointer_01238c6c* p = new (raw) CIntelligentPointer_01238c6c();
    if (g_baseCtorCalls != 1 || g_setCalls != 1 || g_setItem != 0 || p->m_pItem != 0 || p->m_locked != 0) { std::printf("008840b0_TEST FAIL\n"); return 1; }
    std::printf("008840b0_TEST PASS\n"); return 0;
}