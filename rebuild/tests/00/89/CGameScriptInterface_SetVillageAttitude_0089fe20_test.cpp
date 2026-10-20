// Behaviour test for 0089fe20: body copied from src/0089fe20.cpp (CGameScriptInterface::SetVillageAttitude), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase_Retail;
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) { STCEntry_Retail* it = m_pFirst; while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it; return it; }
    STCEntry_Retail* LowerBound(const unsigned long& kind) const { STCEntry_Retail* it = m_pFirst; while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it; return it; }
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CScriptThing {
    bool valid; CThing_Retail* thing;
    virtual CThing_Retail* GetThing() const { return thing; }
    virtual bool IsValid() const { return valid; }
};
enum EScriptVillageAttitude { SCRIPT_VILLAGE_ATTITUDE_0, SCRIPT_VILLAGE_ATTITUDE_1, SCRIPT_VILLAGE_ATTITUDE_2 };
static int g_setCalls = 0; static long g_setValue = -1; static void* g_setThis = 0;
struct CTCVillage_Retail {
    void SetAttitude(long attitude) { ++g_setCalls; g_setValue = attitude; g_setThis = this; }
    __forceinline void SetScriptAttitude(EScriptVillageAttitude attitude)
    {
        switch (attitude) {
        case SCRIPT_VILLAGE_ATTITUDE_0: SetAttitude(0); break;
        case SCRIPT_VILLAGE_ATTITUDE_1: SetAttitude(1); break;
        case SCRIPT_VILLAGE_ATTITUDE_2: SetAttitude(2); break;
        }
    }
};
struct CGameScriptInterface {
    virtual void v000() {}
    virtual void SetVillageAttitude(const CScriptThing& village, EScriptVillageAttitude attitude) const;
};
void CGameScriptInterface::SetVillageAttitude(const CScriptThing& village, EScriptVillageAttitude attitude) const
{
    if (village.IsValid()) {
        CThing_Retail* t = village.GetThing();
        if (t && !t->m_dead && t->HasTC(0x22))
            ((CTCVillage_Retail*)GetTC(t, 0x22))->SetScriptAttitude(attitude);
    }
}
int main()
{
    CThing_Retail t; memset(&t, 0, sizeof(t)); CTCVillage_Retail village;
    STCEntry_Retail entries[2] = { { 0x10, (CTCBase_Retail*)0x99 }, { 0x22, (CTCBase_Retail*)&village } };
    t.m_tcs.m_pFirst = entries; t.m_tcs.m_pLast = entries + 2;
    CScriptThing st; st.valid = false; st.thing = &t; CGameScriptInterface gsi;
    // 1. invalid / null / dead / no component: nothing
    gsi.SetVillageAttitude(st, SCRIPT_VILLAGE_ATTITUDE_1); CHECK(g_setCalls == 0);
    st.valid = true; st.thing = 0; gsi.SetVillageAttitude(st, SCRIPT_VILLAGE_ATTITUDE_1); CHECK(g_setCalls == 0);
    st.thing = &t; t.m_dead = 1; gsi.SetVillageAttitude(st, SCRIPT_VILLAGE_ATTITUDE_1); CHECK(g_setCalls == 0);
    t.m_dead = 0; gsi.SetVillageAttitude(st, SCRIPT_VILLAGE_ATTITUDE_1); CHECK(g_setCalls == 0);
    // 2. component present: each script attitude maps to its value; an unknown one does nothing
    t.m_tcMask[0x22 >> 5] = 1UL << (0x22 & 31);
    gsi.SetVillageAttitude(st, SCRIPT_VILLAGE_ATTITUDE_0); CHECK(g_setCalls == 1); CHECK(g_setValue == 0); CHECK(g_setThis == &village);
    gsi.SetVillageAttitude(st, SCRIPT_VILLAGE_ATTITUDE_1); CHECK(g_setCalls == 2); CHECK(g_setValue == 1);
    gsi.SetVillageAttitude(st, SCRIPT_VILLAGE_ATTITUDE_2); CHECK(g_setCalls == 3); CHECK(g_setValue == 2);
    gsi.SetVillageAttitude(st, (EScriptVillageAttitude)7); CHECK(g_setCalls == 3);
    if (fails == 0) printf("0089fe20_TEST PASS\n"); else printf("0089fe20_TEST FAIL\n");
    return fails;
}