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
struct CIntelligentPointer_Retail { virtual void v00() { } void* m_pItem; CThing_Retail* GetPItem() { std::strcat(g_seq, "g"); return g_item; } CThing_Retail* GetPItem() const { std::strcat(g_seq, "g"); return g_item; } };
static void reset() { std::memset(&g_thing, 0, sizeof g_thing); g_seq[0] = 0; g_item = &g_thing; }
static long long g_uid = 0; static CThing_Retail* g_init = 0;
struct CThingManager_Retail { CThing_Retail* GetThingWithUID(long long uid) { std::strcat(g_seq, "U"); g_uid = uid; return uid == 77 ? &g_thing : 0; } }; static CThingManager_Retail g_tm;
struct CWorld { virtual CThingManager_Retail* GetThingManager() { return &g_tm; } }; static CWorld g_world;
struct CGameScriptThing { virtual void v00() { } CIntelligentPointer_Retail m_thing; long long m_uid; CWorld* m_pWorld; void InitFromThing(CThing_Retail* t) { std::strcat(g_seq, "I"); g_init = t; }
    void UpdateThingAttachment() { if (m_thing.GetPItem() == 0 || m_thing.GetPItem()->m_dead) { CThing_Retail* found = m_pWorld->GetThingManager()->GetThingWithUID(m_uid); if (found) InitFromThing(found); } } };
int main() {
    CGameScriptThing s; s.m_pWorld = &g_world; s.m_uid = 77; reset(); s.UpdateThingAttachment(); if (std::strcmp(g_seq, "gg")) { std::printf("008d0200_TEST FAIL\n"); return 1; }
    reset(); g_thing.m_dead = true; s.UpdateThingAttachment(); if (std::strcmp(g_seq, "ggUI") || g_uid != 77 || g_init != &g_thing) { std::printf("008d0200_TEST FAIL\n"); return 1; }
    reset(); g_item = 0; s.m_uid = 1; s.UpdateThingAttachment(); if (std::strcmp(g_seq, "gU") || g_uid != 1) { std::printf("008d0200_TEST FAIL\n"); return 1; }
    std::printf("008d0200_TEST PASS\n"); return 0;
}
