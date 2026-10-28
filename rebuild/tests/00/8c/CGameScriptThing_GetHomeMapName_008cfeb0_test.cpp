#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = ""; static long g_arg[4];
class CCharString { public: const char* m_p; CCharString() : m_p("") { } CCharString(const CCharString& o) : m_p(o.m_p) { std::strcat(g_seq, "c"); } CCharString(const char* t, long) : m_p(t) { std::strcat(g_seq, "e"); } const CCharString& operator=(const CCharString& o) { m_p = o.m_p; std::strcat(g_seq, "="); return *this; } };
struct CTCGraphicAppearance_Retail { int x; };
struct CThing_Retail { unsigned char m_type; CTCGraphicAppearance_Retail* m_pAppearance; unsigned long m_flags; CCharString m_dataString; bool m_dead;
    void SetToKillOnLevelUnload(long k) { std::strcat(g_seq, "K"); g_arg[0] = k; } void OnLevelUnloadKill_008368f0(long k) { std::strcat(g_seq, "K"); g_arg[0] = k; } void KillOnLevelUnload_008368d0(long k) { std::strcat(g_seq, "K"); g_arg[0] = k; }
    long GetCurrentStateGroupType() { std::strcat(g_seq, "S"); return 77; } void SetDataString(const CCharString& t) { m_dataString = t; } };
static CThing_Retail g_thing; static CThing_Retail* g_item = &g_thing;
struct CIntelligentPointer_Retail { virtual void v00() { } void* m_pItem; CThing_Retail* GetPItem() { std::strcat(g_seq, "g"); return g_item; } CThing_Retail* GetPItem() const { std::strcat(g_seq, "g"); return g_item; } };
static void reset() { std::memset(&g_thing, 0, sizeof g_thing); g_seq[0] = 0; g_item = &g_thing; }
static long g_map = 5; static const char* g_name = "level_x"; static CCharString g_nameStr; static const void* g_rec = 0; static long g_asked = 0;
static const char g_emptyString_0122d70e[] = "";
struct CWorldMap_Retail { virtual long GetMapNumberForRecord(const void* r) { g_rec = r; return g_map; } const CCharString& GetLevelScriptName(long m) { g_asked = m; g_nameStr.m_p = g_name; return g_nameStr; } }; static CWorldMap_Retail g_wm;
class CWorld { public: virtual CWorldMap_Retail* GetWorldMap() { return &g_wm; } }; static CWorld g_world;
class CGameScriptThing { public: virtual void v00() { } unsigned char _pad04[0x30]; unsigned char m_record[4]; unsigned char _pad[0xc]; CWorld* m_pWorld;
    CCharString GetHomeMapName() const { CWorldMap_Retail* wm = m_pWorld->GetWorldMap(); long map = wm->GetMapNumberForRecord(m_record); if (map) return CCharString(wm->GetLevelScriptName(map)); return CCharString(g_emptyString_0122d70e, -1); } };
int main() {
    CGameScriptThing s; s.m_pWorld = &g_world; g_map = 5; CCharString r = s.GetHomeMapName(); if (std::strcmp(r.m_p, "level_x") || g_asked != 5 || g_rec != s.m_record) { std::printf("008cfeb0_TEST FAIL\n"); return 1; }
    g_map = 0; r = s.GetHomeMapName(); if (std::strcmp(r.m_p, "")) { std::printf("008cfeb0_TEST FAIL\n"); return 1; }
    std::printf("008cfeb0_TEST PASS\n"); return 0;
}