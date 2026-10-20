// Behaviour test for 0089bbc0: body copied from src/0089bbc0.cpp (CGameScriptInterface::EntityPlayObjectAnimation), the callees stubbed.
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
struct CCharString { void* m_pData; };
static int g_playCalls = 0; static const CCharString* g_playAnim = 0; static long g_playId = -99; static int g_playLoop = -1; static void* g_playThis = 0;
struct CTCAnimatingObject_Retail {
    unsigned char _pad[0xc];
    bool m_hasId;
    unsigned char _pad0d[3];
    long m_id;
    __forceinline long GetId() const { return m_hasId ? m_id : -1; }
    void PlayAnimation(const CCharString& anim, long id, bool loop) { ++g_playCalls; g_playAnim = &anim; g_playId = id; g_playLoop = loop; g_playThis = this; }
    __forceinline void PlayNextAnimation(const CCharString& anim, bool loop) { PlayAnimation(anim, GetId() + 1, loop); }
};
struct CGameScriptInterface {
    virtual void v000() {}
    virtual void EntityPlayObjectAnimation(const CScriptThing& thing, const CCharString& anim, bool loop) const;
};
void CGameScriptInterface::EntityPlayObjectAnimation(const CScriptThing& thing, const CCharString& anim, bool loop) const
{
    if (thing.IsValid()) {
        CThing_Retail* t = thing.GetThing();
        if (t->HasTC(0x83)) {
            ((CTCAnimatingObject_Retail*)GetTC(t, 0x83))->PlayNextAnimation(anim, loop);
        }
    }
}
int main()
{
    CThing_Retail t; memset(&t, 0, sizeof(t)); CTCAnimatingObject_Retail ao; memset(&ao, 0, sizeof(ao));
    STCEntry_Retail entries[2] = { { 0x10, (CTCBase_Retail*)0x99 }, { 0x83, (CTCBase_Retail*)&ao } };
    t.m_tcs.m_pFirst = entries; t.m_tcs.m_pLast = entries + 2;
    CScriptThing st; st.valid = false; st.thing = &t; CCharString anim = { (void*)"walk" }; CGameScriptInterface gsi;
    // 1. invalid thing: nothing
    gsi.EntityPlayObjectAnimation(st, anim, true); CHECK(g_playCalls == 0);
    // 2. valid thing without the component: nothing
    st.valid = true; gsi.EntityPlayObjectAnimation(st, anim, true); CHECK(g_playCalls == 0);
    // 3. component present, no current id: plays id 0
    t.m_tcMask[0x83 >> 5] = 1UL << (0x83 & 31);
    gsi.EntityPlayObjectAnimation(st, anim, true); CHECK(g_playCalls == 1); CHECK(g_playThis == &ao); CHECK(g_playAnim == &anim); CHECK(g_playId == 0); CHECK(g_playLoop == 1);
    // 4. with a current id: the next one
    ao.m_hasId = true; ao.m_id = 6; gsi.EntityPlayObjectAnimation(st, anim, false); CHECK(g_playCalls == 2); CHECK(g_playId == 7); CHECK(g_playLoop == 0);
    if (fails == 0) printf("0089bbc0_TEST PASS\n"); else printf("0089bbc0_TEST FAIL\n");
    return fails;
}