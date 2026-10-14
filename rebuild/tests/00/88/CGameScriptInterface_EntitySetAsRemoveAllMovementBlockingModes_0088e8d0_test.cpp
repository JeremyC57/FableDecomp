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
struct CDisplayEngine_Retail; struct CPlayerManager_Retail; struct CWorld;
struct CGameScriptInterface { virtual void v000() { } CWorld* World; long m_08; CDisplayEngine_Retail* DisplayEngine; long m_10; CPlayerManager_Retail* PlayerManager; unsigned char _pad18[0x15]; bool AllowScreenFadingIfAlreadyFaded;
    void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing& thing) const {if (thing.IsValid()) { if (thing.GetThing()->m_flags & 8) thing.GetThing()->RemoveMovementBlockingModes(); }}
};

int main() {
    CGameScriptInterface i; reset(2, 8); i.EntitySetAsRemoveAllMovementBlockingModes(g_script); if (std::strcmp(g_seq, "vggB")) { std::printf("0088e8d0_TEST FAIL\n"); return 1; } reset(2, 2); i.EntitySetAsRemoveAllMovementBlockingModes(g_script); if (std::strcmp(g_seq, "vg")) { std::printf("0088e8d0_TEST FAIL\n"); return 1; }
    std::printf("0088e8d0_TEST PASS\n"); return 0;
}