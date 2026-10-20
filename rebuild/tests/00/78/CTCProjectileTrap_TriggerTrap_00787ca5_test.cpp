// Behaviour test for CTCProjectileTrap::TriggerTrap (0x00787CA5): ShotTimer = (long)(fps * SecondsBetweenShots), one more shot
// counted, CreateShot(index of SHOT_ARROW_SINGLE, trap thing, 0, 0, 100, 1, 1, &empty ability, 0, 0), the name and ability
// released, SetTransitionParams(0, 0).
#include <cstdio>
#include <cstring>
struct C3DVector;
struct CRightHandedSet;
static int g_strings = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CGameDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) const { return std::strcmp(name.m_pData, "SHOT_ARROW_SINGLE") == 0 ? 77 : -1; }
};
static CGameDefinitionManager g_dm;
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
static int g_destroyed = 0;
struct CDefVirtual { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } };
struct CMeleeCombatAbilityDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
};
class CThing { public: int id; };
static long g_shotIndex = 0; static CThing* g_shooter = 0; static int g_shotArgs = 0; static CMeleeCombatAbilityDef g_ability;
CThing* CreateShot(long defIndex, CThing* shooter, CThing* origin, CThing* target, float accuracy, float a, float b,
                   CDefPointer<CMeleeCombatAbilityDef>* ability, C3DVector* position, CRightHandedSet* frame)
{
    g_shotIndex = defIndex; g_shooter = shooter;
    g_shotArgs = origin == 0 && target == 0 && accuracy == 100.0f && a == 1.0f && b == 1.0f && ability->m_p == 0 && position == 0 && frame == 0;
    g_ability.m_count = 1; ability->m_p = &g_ability;     // the callee hands a reference back
    return 0;
}
long g_framesPerSecond = 30;
struct CTrapDef { float SecondsBetweenShots; };
static float g_t1 = -1, g_t2 = -1;
struct CTCTrapBase {
    CThing*       ParentThing;
    const CTrapDef* PTrapDef;
    const CGameDefinitionManager& GetDefinitionManager() const { return g_dm; }
    void SetTransitionParams(float a, float b) { g_t1 = a; g_t2 = b; }
};
struct CTCProjectileTrap : CTCTrapBase {
    long ShotTimer;
    long IdleTimer;
    long NumShotsThisRound;
    void TriggerTrap();
};

void CTCProjectileTrap::TriggerTrap()
{
    ShotTimer = (long)(g_framesPerSecond * PTrapDef->SecondsBetweenShots);
    ++NumShotsThisRound;
    long shotIndex;
    {
        CCharString name("SHOT_ARROW_SINGLE", -1);
        shotIndex = GetDefinitionManager().GetDefGlobalIndexFromName(name);
    }
    {
        CDefPointer<CMeleeCombatAbilityDef> ability;
        CreateShot(shotIndex, ParentThing, 0, 0, 100.0f, 1.0f, 1.0f, &ability, 0, 0);
    }
    SetTransitionParams(0.0f, 0.0f);
}

int main()
{
    int bad = 0;
    CThing trapThing; CTrapDef def; def.SecondsBetweenShots = 1.5f;
    CTCProjectileTrap t; t.ParentThing = &trapThing; t.PTrapDef = &def; t.ShotTimer = 0; t.NumShotsThisRound = 2;
    t.TriggerTrap();
    if (t.ShotTimer != 45 || t.NumShotsThisRound != 3) bad |= 1;
    if (g_shotIndex != 77 || g_shooter != &trapThing || !g_shotArgs) bad |= 2;
    if (g_strings != 0 || g_destroyed != 1 || g_ability.m_count != 0) bad |= 4;
    if (g_t1 != 0.0f || g_t2 != 0.0f) bad |= 8;
    if (bad) { std::printf("00787ca5_TEST FAIL\n"); return 1; }
    std::printf("00787ca5_TEST PASS\n");
    return 0;
}