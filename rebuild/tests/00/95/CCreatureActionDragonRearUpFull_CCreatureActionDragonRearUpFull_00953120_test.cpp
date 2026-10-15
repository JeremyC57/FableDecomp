// Behaviour test for 0x00953120 CCreatureAction_DragonRearUpFull constructor: base gets owner, target, code 250 and the SWORD_SWING sound pair;
// the intelligent pointer member starts empty; the CCharString temporary is gone afterwards.
#include <cstdio>
#include <cstring>

struct CThingCreatureBase { int id; };
struct CThing { int id; };

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
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem((void*)1) { m_pItem = 0; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() { } };
struct CBase { int m_kind; CBase() : m_kind(1) { } };
struct CEnginePrimitiveHandle : CBase {
    long m_id;
    CEnginePrimitiveHandle() : m_id(0) { m_kind = 2; }
};
struct CCombatActionBase {
    CThingCreatureBase* m_owner; CThing* m_target; long m_code; CSoundPair m_sound; int m_stringsAlive;
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
        : m_owner(owner), m_target(target), m_code(actionCode), m_sound(sound), m_stringsAlive(g_liveStrings) { }
};
struct CCreatureAction_DragonRearUpFull : CCombatActionBase {
    CIntelligentPointer PTarget;

    CCreatureAction_DragonRearUpFull(CThingCreatureBase* owner, CThing* target)
        : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING")) { }
};

int main()
{
    CThingCreatureBase owner = { 5 };
    CThing target = { 9 };
    CCreatureAction_DragonRearUpFull a(&owner, &target);
    if (a.m_owner != &owner || a.m_target != &target || a.m_code != 250 || a.m_sound.FirstSound != 5 ||
        a.m_sound.LastSound != 77 || a.m_stringsAlive != 1 || g_liveStrings != 0 || a.PTarget.m_pItem != 0) {
        std::printf("00953120_TEST FAIL\n"); return 1;
    }
    std::printf("00953120_TEST PASS\n");
    return 0;
}