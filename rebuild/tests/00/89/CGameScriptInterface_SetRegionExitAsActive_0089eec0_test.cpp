#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = ""; static long g_arg[4]; static bool g_valid = true; static long g_lbKind = -1;
struct CTCBase_Retail { int m_tag; };
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail { STCEntry_Retail* m_pFirst; STCEntry_Retail* m_pLast; STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const { g_lbKind = (long)kind; STCEntry_Retail* it = m_pFirst; while (it != m_pLast && it->m_kind < (long)kind) ++it; return it; }
    CTCBase_Retail* Find(long kind) const { STCEntry_Retail* it = LowerBound((unsigned long)kind); if (it == m_pLast || kind < it->m_kind) it = m_pLast; return it->m_pTC; } };
struct CThing_Retail { unsigned char m_type; unsigned long m_tcMask[8]; CTCMap_Retail m_tcs; bool m_dead; void* m_pRegionLink;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); } };
static CTCBase_Retail* GetTC(CThing_Retail* t, long kind) { return t->m_tcs.Find(kind); }
static CTCBase_Retail* GetTCConst(const CThing_Retail* t, long kind) { return t->m_tcs.Find(kind); }
class CScriptThing { public: CThing_Retail* m_pThing; virtual CThing_Retail* GetThing() const { std::strcat(g_seq, "g"); return m_pThing; } virtual bool IsValid() const { std::strcat(g_seq, "v"); return g_valid; } };
static CThing_Retail g_thing; static CScriptThing g_script; static STCEntry_Retail g_entries[4]; static CTCBase_Retail g_tcs[4]; static CTCBase_Retail g_endTC;
static void reset(long kind, bool present) { std::memset(&g_thing, 0, sizeof g_thing); g_seq[0] = 0; g_valid = true; g_lbKind = -1; g_script.m_pThing = &g_thing;
    g_entries[0].m_kind = 1; g_entries[0].m_pTC = &g_tcs[0]; g_entries[1].m_kind = kind; g_entries[1].m_pTC = &g_tcs[1]; g_entries[2].m_kind = 999; g_entries[2].m_pTC = &g_tcs[2]; g_entries[3].m_kind = 0; g_entries[3].m_pTC = &g_endTC;
    g_thing.m_tcs.m_pFirst = g_entries; g_thing.m_tcs.m_pLast = g_entries + 3; g_thing.m_tcs.m_pEnd = g_entries + 4; if (present) g_thing.m_tcMask[kind >> 5] |= 1UL << (kind & 31); }
struct CGameScriptInterface_Base { virtual void v000() { } };

struct CPlayer_Retail { CThing_Retail* GetHeroCreature() { std::strcat(g_seq, "h"); return &g_thing; } }; static CPlayer_Retail g_player;
struct CPlayerManager_Retail { CPlayer_Retail* GetMainPlayer() { std::strcat(g_seq, "p"); return &g_player; } }; static CPlayerManager_Retail g_pm;
struct CRegionExit_Retail { unsigned char _pad[0xc]; bool m_active; };
static long g_refresh = -1; struct CPlayerGui_Retail { void Refresh(long f) { std::strcat(g_seq, "R"); g_refresh = f; } }; static CPlayerGui_Retail g_gui; static CPlayerGui_Retail* g_pPlayerGui_013b8790 = &g_gui;
class CGameScriptInterface : public CGameScriptInterface_Base { public: void* World; long Component; void* DisplayEngine; long DefinitionManager; CPlayerManager_Retail* PlayerManager;
    void SetRegionExitAsActive(const CScriptThing& thing, bool active) const { if (thing.IsValid()) { CThing_Retail* t = thing.GetThing(); if (t->HasTC(100)) { ((CRegionExit_Retail*)t->m_pRegionLink)->m_active = active; g_pPlayerGui_013b8790->Refresh(0); } } } };
int main() {
    CGameScriptInterface i; reset(100, true); CRegionExit_Retail x; x.m_active = false; g_thing.m_pRegionLink = &x; i.SetRegionExitAsActive(g_script, true); if (std::strcmp(g_seq, "vgR") || !x.m_active || g_refresh != 0) { std::printf("0089eec0_TEST FAIL\n"); return 1; }
    reset(100, false); g_thing.m_pRegionLink = &x; g_seq[0] = 0; i.SetRegionExitAsActive(g_script, false); if (std::strcmp(g_seq, "vg") || !x.m_active) { std::printf("0089eec0_TEST FAIL\n"); return 1; }
    std::printf("0089eec0_TEST PASS\n"); return 0;
}