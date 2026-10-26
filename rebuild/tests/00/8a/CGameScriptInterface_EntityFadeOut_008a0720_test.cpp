// Behaviour test for 008a0720: body copied from src/008a0720.cpp (CGameScriptInterface::EntityFadeOut), the callees stubbed.
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
class CScriptThing { public:
    bool valid; CThing_Retail* thing;
    virtual CThing_Retail* GetThing() const { return thing; }
    virtual bool IsValid() const { return valid; }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length) : m_pData((void*)text) {}
    ~CCharString() {}
};
class CTCParamsBase;
static int g_fadeCalls = 0; static float g_fadeTime = -1.0f; static int g_fadeIn = -1; static void* g_fadeThis = 0;
struct CTCFadeOutAndIn_Retail {
    void Fade(float time, bool fadeIn) { ++g_fadeCalls; g_fadeTime = time; g_fadeIn = fadeIn; g_fadeThis = this; }
    __forceinline void FadeOut(float time) { Fade(time, false); }
};
static int g_addCalls = 0; static const char* g_addName = 0; static int g_addActivate = -1; static CTCBase_Retail* g_addResult = 0;
struct CThingWithAddTC_Retail : CThing_Retail {
    CTCBase_Retail* AddTC(const CCharString& name, bool activate, CTCParamsBase* params) { ++g_addCalls; g_addName = (const char*)name.m_pData; g_addActivate = activate; return g_addResult; }
};
class CGameScriptInterface { public:
    virtual void v000() {}
    virtual void EntityFadeOut(const CScriptThing& thing, float time) const;
};
void CGameScriptInterface::EntityFadeOut(const CScriptThing& thing, float time) const
{
    if (thing.IsValid()) {
        CThingWithAddTC_Retail* t = (CThingWithAddTC_Retail*)thing.GetThing();
        CTCFadeOutAndIn_Retail* fade;
        if (t->HasTC(0xf9))
            fade = (CTCFadeOutAndIn_Retail*)GetTC(t, 0xf9);
        else {
            CTCBase_Retail* added;
            {
                CCharString name("CTCFadeOutAndIn", -1);
                added = t->AddTC(name, false, 0);
            }
            if (!added)
                return;
            fade = (CTCFadeOutAndIn_Retail*)added;
        }
        if (fade)
            fade->FadeOut(time);
    }
}
int main()
{
    CThingWithAddTC_Retail t; memset(&t, 0, sizeof(t)); CTCFadeOutAndIn_Retail fade, added;
    STCEntry_Retail entries[2] = { { 0x10, (CTCBase_Retail*)0x99 }, { 0xf9, (CTCBase_Retail*)&fade } };
    t.m_tcs.m_pFirst = entries; t.m_tcs.m_pLast = entries + 2;
    CScriptThing st; st.valid = false; st.thing = &t;
    CGameScriptInterface gsi;
    // 1. invalid script thing: nothing
    gsi.EntityFadeOut(st, 1.0f); CHECK(g_fadeCalls == 0); CHECK(g_addCalls == 0);
    // 2. valid, no fade component, AddTC fails: nothing
    st.valid = true; g_addResult = 0; gsi.EntityFadeOut(st, 1.0f); CHECK(g_addCalls == 1); CHECK(strcmp(g_addName, "CTCFadeOutAndIn") == 0); CHECK(g_addActivate == 0); CHECK(g_fadeCalls == 0);
    // 3. AddTC succeeds: the new component fades out
    g_addResult = (CTCBase_Retail*)&added; gsi.EntityFadeOut(st, 2.5f); CHECK(g_addCalls == 2); CHECK(g_fadeCalls == 1); CHECK(g_fadeThis == &added); CHECK(g_fadeTime == 2.5f); CHECK(g_fadeIn == 0);
    // 4. component present: looked up, no AddTC
    t.m_tcMask[0xf9 >> 5] = 1UL << (0xf9 & 31); gsi.EntityFadeOut(st, 0.5f); CHECK(g_addCalls == 2); CHECK(g_fadeCalls == 2); CHECK(g_fadeThis == &fade); CHECK(g_fadeTime == 0.5f);
    // 5. present bit set but the map entry is null: no fade
    entries[1].m_pTC = 0; gsi.EntityFadeOut(st, 0.5f); CHECK(g_fadeCalls == 2);
    if (fails == 0) printf("008a0720_TEST PASS\n"); else printf("008a0720_TEST FAIL\n");
    return fails;
}