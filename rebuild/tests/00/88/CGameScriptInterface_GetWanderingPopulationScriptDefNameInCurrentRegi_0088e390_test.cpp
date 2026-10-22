#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = ""; static long g_arg[8]; static float g_farg = 0; static bool g_valid = true; static bool g_pick = false; static long g_sex = 3;
struct CThingPhysics_Retail { virtual void EnableGravity(bool e) { std::strcat(g_seq, "G"); g_arg[0] = e; } };
struct CThing_Retail {
    virtual void SetAsDrawable(bool d) { std::strcat(g_seq, "D"); g_arg[0] = d; }
    unsigned char m_type; CThingPhysics_Retail* m_pPhysics; unsigned char m_flags;
    unsigned char m_dead : 1; unsigned char m_91_1 : 7;
    unsigned char m_92_0 : 4; unsigned char m_gameStatePersisted : 1; unsigned char m_92_5 : 1; unsigned char m_limboKeepsScript : 1;
    unsigned char m_damageable : 1; unsigned char m_bc_1 : 1; unsigned char m_respondToHit : 1;
    unsigned char m_houseLockingOverridden : 1;
    long m_shotAccuracyPercentage;
    unsigned char m_15c_0 : 6; unsigned char m_followsHeroThroughTeleporters : 1; unsigned char m_15c_7 : 1; unsigned char m_useMovementInActions : 1; unsigned char m_15d_1 : 7;
    float m_aiMaxWalkingSpeed, m_aiMaxRunningSpeed, m_playerMaxWalkingSpeed, m_playerMaxRunningSpeed;
    CThingPhysics_Retail* GetPhysics() { return m_pPhysics; }
    void SetMaxHealth(float h, bool f) { std::strcat(g_seq, "H"); g_farg = h; g_arg[0] = f; }
    bool IsPickPocketable() { std::strcat(g_seq, "P"); return g_pick; }
    void SetHiddenOnMiniMap(bool h) { std::strcat(g_seq, "M"); g_arg[0] = h; }
    void SetHasInformation(long k, bool a, bool b, bool c) { std::strcat(g_seq, "I"); g_arg[0] = k; g_arg[1] = a; g_arg[2] = b; g_arg[3] = c; }
    void SetAsGameStatePersisted(bool p) { std::strcat(g_seq, "S"); g_arg[0] = p; }
    void ResetMaxRunningSpeedToDefault() { std::strcat(g_seq, "r"); }
    void ResetMaxWalkingSpeedToDefault() { std::strcat(g_seq, "w"); }
    void RemoveMovementBlockingModes() { std::strcat(g_seq, "B"); }
    void SetCutscenePauseBehaviour(long b) { std::strcat(g_seq, "C"); g_arg[0] = b; }
    long GetSex() { std::strcat(g_seq, "X"); return g_sex; }
    void PreloadBasicAnimations() { std::strcat(g_seq, "A"); }
    void SetInLimbo(bool l) { std::strcat(g_seq, "L"); g_arg[0] = l; }
    CThing_Retail() { std::memset((char*)this + sizeof(void*), 0, sizeof(*this) - sizeof(void*)); }
};
struct CScriptThing { CThing_Retail* m_pThing; virtual CThing_Retail* GetThing() const { std::strcat(g_seq, "g"); return m_pThing; } virtual bool IsValid() const { std::strcat(g_seq, "v"); return g_valid; } };
static CThing_Retail g_thing; static CScriptThing g_script;
static void reset(unsigned char type, unsigned char flags) { g_thing = CThing_Retail(); g_thing.m_type = type; g_thing.m_flags = flags; g_script.m_pThing = &g_thing; g_seq[0] = 0; g_valid = true; }
struct CCharString { const char* m_p; };
static long g_map = 7; static bool g_loaded = true; static bool g_prox = false; static long g_region = 5; static long g_current = 5; static char g_regName[4] = "R";
struct CRegion_Retail { unsigned char _pad[0x18]; unsigned char m_name[4]; };
struct CThingManager_Retail { CThing_Retail* GetThingWithUID(long long uid) { std::strcat(g_seq, "U"); g_arg[0] = (long)uid; return uid == 77 ? &g_thing : 0; } void OnThingKill(CThing_Retail* t) { std::strcat(g_seq, "K"); g_arg[0] = (long)(t == &g_thing); } };
struct CWorldMap_Retail { virtual bool IsMapLoaded(long m) { std::strcat(g_seq, "l"); g_arg[1] = m; return g_loaded; }
    long GetMapNumberFromScriptName(const CCharString& n) { std::strcat(g_seq, "n"); g_arg[0] = (long)(n.m_p[0]); return g_map; } bool IsMapLoadedOnPlayerProximity(long m) { std::strcat(g_seq, "p"); g_arg[2] = m; return g_prox; }
    long GetCurrentRegionNumber() { std::strcat(g_seq, "c"); return g_current; } CRegion_Retail* GetRegion(long r) { std::strcat(g_seq, "R"); g_arg[0] = r; static CRegion_Retail reg; std::memcpy(reg.m_name, g_regName, 4); return &reg; }
    void SetRegionsAsConnectedInGraph(long a, long b, long c) { std::strcat(g_seq, "S"); g_arg[0] = a; g_arg[1] = b; g_arg[2] = c; } long GetRegionNumberFromName(const CCharString&) { std::strcat(g_seq, "N"); return g_region; }
    void ApplyScriptBrush(const CCharString& n, long f) { std::strcat(g_seq, "b"); g_arg[0] = (long)(n.m_p[0]); g_arg[1] = f; } };
struct CEnvironmentSpellEffectInterpolator_Retail { int x; };
struct CEnvironment_Retail { CEnvironmentSpellEffectInterpolator_Retail* m_pSpellEffects; void TransitionThemeToDefault(long p, long f, float t) { std::strcat(g_seq, "T"); g_arg[p] = f; g_farg = t; } };
struct COpinionReactionManager_Retail { int x; };
struct CScriptConversationManager_Retail { long MakeNewConversation(CThing_Retail* t, bool a, bool b) { std::strcat(g_seq, "C"); g_arg[0] = (long)(t == &g_thing); g_arg[1] = a; g_arg[2] = b; return 42; } void AddPersonToConversation(long c, CThing_Retail* t) { std::strcat(g_seq, "P"); g_arg[0] = c; g_arg[1] = (long)(t == &g_thing); } };
static CThingManager_Retail g_tm; static CWorldMap_Retail g_wm; static CEnvironment_Retail g_env; static CScriptConversationManager_Retail g_conv;
struct CWorld { virtual CThingManager_Retail* GetThingManager() { std::strcat(g_seq, "t"); return &g_tm; } virtual CWorldMap_Retail* GetWorldMapForEdit() { std::strcat(g_seq, "e"); return &g_wm; } virtual CWorldMap_Retail* GetWorldMap() { std::strcat(g_seq, "m"); return &g_wm; }
    CWorldMap_Retail* PWorldMap_ptr; CEnvironment_Retail* PEnvironment_ptr; COpinionReactionManager_Retail* POpinionReactionManager_Data; CScriptConversationManager_Retail* PScriptConversationManager_ptr; CEnvironment_Retail* GetEnvironment() { return PEnvironment_ptr; } };
static CWorld g_world;
static const char* g_wander = "wander"; static char g_regIn[4];
struct CGameScriptInterface_Base { };
struct CDisplayEngine_Retail; struct CPlayerManager_Retail;
struct CGameScriptInterface { virtual void v000() { } CWorld* World; long Component; CDisplayEngine_Retail* DisplayEngine; long DefinitionManager; CPlayerManager_Retail* PlayerManager; unsigned char _pad18[0x15]; bool AllowScreenFadingIfAlreadyFaded;
    void GetWanderingPopulationScriptDefName(const CCharString& region, CCharString& name) const { std::strcat(g_seq, "W"); std::memcpy(g_regIn, &region, 4); name.m_p = g_wander; }
    void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString& name) const {CRegion_Retail* region = World->GetWorldMap()->GetRegion(World->GetWorldMap()->GetCurrentRegionNumber()); GetWanderingPopulationScriptDefName(*(const CCharString*)region->m_name, name);}
};

int main() {
    CGameScriptInterface i; i.World = &g_world; CCharString out = { 0 }; g_seq[0] = 0; g_current = 9; i.GetWanderingPopulationScriptDefNameInCurrentRegion(out); if (std::strlen(g_seq) != 5 || g_seq[4] != 'W' || g_arg[0] != 9 || out.m_p != g_wander || g_regIn[0] != 'R') { std::printf("0088e390_TEST FAIL\n"); return 1; }
    std::printf("0088e390_TEST PASS\n"); return 0;
}