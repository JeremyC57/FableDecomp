// Behaviour test for 0x009717A0 CCreatureAction_DuckAndCover constructor: base gets owner, code 150 and the
// SWORD_SWING sound pair; the position argument is copied into the three position members.
#include <cstdio>
#include <cstring>

struct CThingCreatureBase { int id; };

static int g_liveStrings = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
static CSoundPair GetSoundPair(CThingCreatureBase* owner, const CCharString& name)
{
    CSoundPair p;
    p.FirstSound = owner->id;
    p.LastSound = std::strcmp(name.m_pData, "SWORD_SWING") == 0 ? 77 : -1;
    return p;
}
struct C3DVector { float X, Y, Z; };
struct CCombatActionBase {
    CThingCreatureBase* m_owner; long m_code; CSoundPair m_sound; int m_stringsAlive;
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
        : m_owner(owner), m_code(actionCode), m_sound(sound), m_stringsAlive(g_liveStrings) { }
};
struct CCreatureAction_DuckAndCover : CCombatActionBase {
    float PosX, PosY, PosZ;
    CCreatureAction_DuckAndCover(CThingCreatureBase* owner, const C3DVector& pos)
        : CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING")), PosX(pos.X), PosY(pos.Y), PosZ(pos.Z) { }
};

int main()
{
    CThingCreatureBase owner = { 8 };
    C3DVector pos = { 1.5f, -2.0f, 10.25f };
    CCreatureAction_DuckAndCover a(&owner, pos);
    if (a.m_owner != &owner || a.m_code != 150 || a.m_sound.FirstSound != 8 || a.m_sound.LastSound != 77 ||
        a.m_stringsAlive != 1 || g_liveStrings != 0 || a.PosX != 1.5f || a.PosY != -2.0f || a.PosZ != 10.25f) {
        std::printf("009717a0_TEST FAIL\n"); return 1;
    }
    std::printf("009717a0_TEST PASS\n");
    return 0;
}