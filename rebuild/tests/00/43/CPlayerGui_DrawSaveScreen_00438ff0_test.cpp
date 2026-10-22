// Behaviour test for 00438ff0: body copied from src/00438ff0.cpp (CPlayerGui::DrawSaveScreen), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_drawCalls = 0; static long g_drawArg = -1; static void* g_drawThis = 0;
struct CTCBase_Retail {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {}
    virtual void DrawSaveScreen(long prepareInfo) { ++g_drawCalls; g_drawArg = prepareInfo; g_drawThis = this; }
};
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) { STCEntry_Retail* it = m_pFirst; while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it; return it; }
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc = 0;
    if (thing->HasTC(kind))
        tc = thing->m_tcs.Find(kind);
    return tc;
}
static int g_uidCalls = 0; static unsigned __int64 g_uidSeen = 0; static CThing_Retail* g_uidResult = 0;
struct CThingManager_Retail {
    CThing_Retail* GetThingWithUID(unsigned __int64 uid) { ++g_uidCalls; g_uidSeen = uid; return g_uidResult; }
};
struct CWorld_Retail {
    CThingManager_Retail manager;
    virtual void v00() {} virtual void v01() {} virtual void v02() {}
    virtual CThingManager_Retail* GetThingManager() { return &manager; }
};
struct CPlayerGui {
    void*            __vftable;
    CWorld_Retail*   World;
    unsigned char    _pad008[0x2a8 - 0x008];
    unsigned __int64 HeroCentreUID;
    void DrawSaveScreen(long prepareInfo);
};
void CPlayerGui::DrawSaveScreen(long prepareInfo)
{
    if (HeroCentreUID > 0) {
        CThing_Retail* thing = World->GetThingManager()->GetThingWithUID(HeroCentreUID);
        if (thing) {
            if (CTCBase_Retail* tc = GetTCIfPresent(thing, 0x6a))
                tc->DrawSaveScreen(prepareInfo);
        }
    }
}
int main()
{
    CThing_Retail thing; memset(&thing, 0, sizeof(thing)); CTCBase_Retail tc;
    STCEntry_Retail entries[2] = { { 0x10, (CTCBase_Retail*)0x99 }, { 0x6a, &tc } };
    thing.m_tcs.m_pFirst = entries; thing.m_tcs.m_pLast = entries + 2;
    CWorld_Retail world; CPlayerGui gui; memset(&gui, 0, sizeof(gui)); gui.World = &world;
    // 1. no hero-centre UID: nothing
    gui.DrawSaveScreen(3); CHECK(g_uidCalls == 0);
    // 2. UID set but no such thing: looked up, nothing drawn
    gui.HeroCentreUID = 0x1122334455667788ULL; g_uidResult = 0; gui.DrawSaveScreen(3); CHECK(g_uidCalls == 1); CHECK(g_uidSeen == 0x1122334455667788ULL); CHECK(g_drawCalls == 0);
    // 3. thing without the component: nothing
    g_uidResult = &thing; gui.DrawSaveScreen(3); CHECK(g_uidCalls == 2); CHECK(g_drawCalls == 0);
    // 4. component present: its virtual DrawSaveScreen gets the argument
    thing.m_tcMask[0x6a >> 5] = 1UL << (0x6a & 31); gui.DrawSaveScreen(9); CHECK(g_drawCalls == 1); CHECK(g_drawArg == 9); CHECK(g_drawThis == &tc);
    if (fails == 0) printf("00438ff0_TEST PASS\n"); else printf("00438ff0_TEST FAIL\n");
    return fails;
}