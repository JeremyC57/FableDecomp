#include <cstdio>
#include <cstring>
#include <new>

struct CThingCreatureBase { int x; };
static CThingCreatureBase* g_c = 0; static CThingCreatureBase* g_t = 0; static long g_id = 0; static long g_p = 0;
struct CCreatureAction_BlockBase {
    virtual ~CCreatureAction_BlockBase() {}
    CCreatureAction_BlockBase(CThingCreatureBase* c, CThingCreatureBase* t, long id, long p) { g_c = c; g_t = t; g_id = id; g_p = p; }
};
struct CCreatureAction_BlockRespond : CCreatureAction_BlockBase {
    bool m_useHitPos; unsigned char m_hitPos[0xc]; bool m_faceTargetAtStart;
    CCreatureAction_BlockRespond(CThingCreatureBase* c, CThingCreatureBase* t, long p)
        : CCreatureAction_BlockBase(c, t, 0x96, p), m_useHitPos(false), m_faceTargetAtStart(false) {}
};
int main()
{
    CThingCreatureBase c, t;
    unsigned char raw[sizeof(CCreatureAction_BlockRespond)]; std::memset(raw, 0xaa, sizeof raw);
    CCreatureAction_BlockRespond* a = new (raw) CCreatureAction_BlockRespond(&c, &t, 42);
    if (g_c != &c || g_t != &t || g_id != 0x96 || g_p != 42 || a->m_useHitPos || a->m_faceTargetAtStart) { std::printf("00855b50_TEST FAIL\n"); return 1; }
    a->~CCreatureAction_BlockRespond();
    std::printf("00855b50_TEST PASS\n");
    return 0;
}
