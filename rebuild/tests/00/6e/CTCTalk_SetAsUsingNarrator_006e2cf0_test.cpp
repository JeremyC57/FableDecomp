// Behaviour test for 006e2cf0: body copied from src/006e2cf0.cpp (CTCTalk::SetAsUsingNarrator), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CCharString { void* m_pData; };
struct SRandomAccessTag { SRandomAccessTag() {} };
static int g_findCalls = 0;
CCharString* __fastcall FindString(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessTag& tag)
{
    ++g_findCalls;
    for (CCharString* it = first; it != last; ++it)
        if (it->m_pData == value.m_pData)
            return it;
    return last;
}
struct SNarratorRecord {
    unsigned char _pad_0x00[0x100];
    CCharString*  m_pNamesFirst;
    CCharString*  m_pNamesLast;
};
static SNarratorRecord g_narrators;
struct CGameDefinitionManager {
    unsigned char    _pad_0x00[0xd0];
    SNarratorRecord* m_pNarrators;
    static CGameDefinitionManager& __fastcall Get() { static CGameDefinitionManager m; m.m_pNarrators = &g_narrators; return m; }
};
struct CTCTalk {
    unsigned char _base[0x34];
    bool          IsPureAISpeaker;
    void SetAsUsingNarrator(const CCharString& narrator);
};
void CTCTalk::SetAsUsingNarrator(const CCharString& narrator)
{
    if (IsPureAISpeaker) {
        SNarratorRecord* narrators = CGameDefinitionManager::Get().m_pNarrators;
        CCharString* last = narrators->m_pNamesLast;
        if (FindString(narrators->m_pNamesFirst, last, narrator, SRandomAccessTag()) == last)
            IsPureAISpeaker = false;
    }
}
int main()
{
    CCharString names[2] = { { (void*)"a" }, { (void*)"b" } };
    g_narrators.m_pNamesFirst = names; g_narrators.m_pNamesLast = names + 2;
    CCharString known = { names[1].m_pData }, unknown = { (void*)"zzz" };
    CTCTalk t; memset(&t, 0, sizeof(t));
    // 1. not a pure AI speaker: no lookup
    t.SetAsUsingNarrator(unknown); CHECK(g_findCalls == 0); CHECK(!t.IsPureAISpeaker);
    // 2. pure AI speaker, narrator in the list: stays
    t.IsPureAISpeaker = true; t.SetAsUsingNarrator(known); CHECK(g_findCalls == 1); CHECK(t.IsPureAISpeaker);
    // 3. narrator not in the list: no longer a pure AI speaker
    t.SetAsUsingNarrator(unknown); CHECK(g_findCalls == 2); CHECK(!t.IsPureAISpeaker);
    if (fails == 0) printf("006e2cf0_TEST PASS\n"); else printf("006e2cf0_TEST FAIL\n");
    return fails;
}