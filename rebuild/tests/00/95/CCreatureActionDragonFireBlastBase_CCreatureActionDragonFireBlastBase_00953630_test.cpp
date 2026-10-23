// Behaviour test for 0x00953630 CCreatureAction_DragonFireBlastBase constructor: base gets owner, target, code 250 and the SWORD_SWING sound pair;
// the intelligent pointer member starts empty; the primitive handle is zeroed; the CCharString temporary is gone afterwards.
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
namespace N_0099a2d0 { struct CBase { int m_kind; CBase() : m_kind(1) { } }; } using N_0099a2d0::CBase;
struct CEnginePrimitiveHandle : CBase {
    long m_id;
    CEnginePrimitiveHandle() : m_id(0) { m_kind = 2; }
};
struct CCombatActionBase {
    CThingCreatureBase* m_owner; CThing* m_target; long m_code; CSoundPair m_sound; int m_stringsAlive;
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
        : m_owner(owner), m_target(target), m_code(actionCode), m_sound(sound), m_stringsAlive(g_liveStrings) { }
};
struct CCreatureAction_DragonFireBlastBase : CCombatActionBase {
    CIntelligentPointer PTarget;
    CEnginePrimitiveHandle FireHandle;
    CCreatureAction_DragonFireBlastBase(CThingCreatureBase* owner, CThing* target)
        : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING")) { }
};

int main()
{
    CThingCreatureBase owner = { 5 };
    CThing target = { 9 };
    CCreatureAction_DragonFireBlastBase a(&owner, &target);
    if (a.m_owner != &owner || a.m_target != &target || a.m_code != 250 || a.m_sound.FirstSound != 5 ||
        a.m_sound.LastSound != 77 || a.m_stringsAlive != 1 || g_liveStrings != 0 || a.PTarget.m_pItem != 0 || a.FireHandle.m_id != 0 || a.FireHandle.m_kind != 2) {
        std::printf("00953630_TEST FAIL\n"); return 1;
    }
    std::printf("00953630_TEST PASS\n");
    return 0;
}