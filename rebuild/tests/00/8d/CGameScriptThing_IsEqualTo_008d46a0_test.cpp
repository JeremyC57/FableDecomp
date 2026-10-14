#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = ""; static long g_arg[4];
struct CCharString { const char* m_p; CCharString() : m_p("") { } CCharString(const CCharString& o) : m_p(o.m_p) { std::strcat(g_seq, "c"); } CCharString(const char* t, long) : m_p(t) { std::strcat(g_seq, "e"); } CCharString& operator=(const CCharString& o) { m_p = o.m_p; std::strcat(g_seq, "="); return *this; } };
struct CTCGraphicAppearance_Retail { int x; };
struct CThing_Retail { unsigned char m_type; CTCGraphicAppearance_Retail* m_pAppearance; unsigned long m_flags; CCharString m_dataString; bool m_dead;
    void SetToKillOnLevelUnload(long k) { std::strcat(g_seq, "K"); g_arg[0] = k; } void OnLevelUnloadKill_008368f0(long k) { std::strcat(g_seq, "K"); g_arg[0] = k; } void KillOnLevelUnload_008368d0(long k) { std::strcat(g_seq, "K"); g_arg[0] = k; }
    long GetCurrentStateGroupType() { std::strcat(g_seq, "S"); return 77; } void SetDataString(const CCharString& t) { m_dataString = t; } };
static CThing_Retail g_thing; static CThing_Retail* g_item = &g_thing;
struct CIntelligentPointer_Retail { virtual void v00() { } void* m_pItem; CThing_Retail* GetPItem() { return (CThing_Retail*)m_pItem; } };
static void reset() { std::memset(&g_thing, 0, sizeof g_thing); g_seq[0] = 0; g_item = &g_thing; }
struct CScriptThing { virtual void v00() { } unsigned char _pad04[8]; CIntelligentPointer_Retail m_thing; };
struct CGameScriptThing : CScriptThing { virtual bool IsEqualTo(const CScriptThing& other) const { return const_cast<CScriptThing&>(other).m_thing.GetPItem() == const_cast<CGameScriptThing*>(this)->m_thing.GetPItem(); } };
int main() {
    CGameScriptThing a, b; CThing_Retail other; g_item = &g_thing; a.m_thing.m_pItem = &g_thing; b.m_thing.m_pItem = &g_thing; if (!a.IsEqualTo(b)) { std::printf("008d46a0_TEST FAIL\n"); return 1; } b.m_thing.m_pItem = &other; if (a.IsEqualTo(b)) { std::printf("008d46a0_TEST FAIL\n"); return 1; }
    std::printf("008d46a0_TEST PASS\n"); return 0;
}