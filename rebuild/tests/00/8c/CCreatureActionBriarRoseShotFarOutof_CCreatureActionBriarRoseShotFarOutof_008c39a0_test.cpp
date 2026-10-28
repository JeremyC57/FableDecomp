#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
class CThingCreatureBase { public: int id; };
class CThing { public: int id; };
class CBaseObject { public: int id; };
class CCharString { public:
    const char* m_text;
    CCharString(const char* text, long length = -1) : m_text(text) { (void)length; }
};
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static const char* g_soundName = 0;
static CThingCreatureBase* g_soundOwner = 0;
CSoundPair FASTCALL GetSoundPair(CThingCreatureBase* owner, const CCharString& name)
{
    g_soundOwner = owner; g_soundName = name.m_text;
    CSoundPair p; p.FirstSound = 11; p.LastSound = 22; return p;
}
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct SBaseArgs { CThingCreatureBase* owner; CThing* target; long code; long first; long last; };
static SBaseArgs g_base;
static const char* g_animName = 0;
class CCombatActionBase { public:
    virtual ~CCombatActionBase() { }
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
    { g_base.owner = owner; g_base.target = 0; g_base.code = actionCode; g_base.first = sound.FirstSound; g_base.last = sound.LastSound; }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    { g_base.owner = owner; g_base.target = target; g_base.code = actionCode; g_base.first = sound.FirstSound; g_base.last = sound.LastSound; }
    void SetCombatAnimation(const CCharString& name) { g_animName = name.m_text; }
};
class CCreatureAction_BriarRoseShotFarOutof : public CCombatActionBase { public:
    CIntelligentPointer PShot;
    CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase* owner, CThing* target);
};
CCreatureAction_BriarRoseShotFarOutof::CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING"))
    , PShot()
{
    SetCombatAnimation("STRIKE_SHOT_FAR_OUTOF");
}
int main()
{
    CThingCreatureBase owner; owner.id = 7;
    CThing target; target.id = 9;
    CThing other; other.id = 10;
    (void)target; (void)other;
    g_base.first = 5; g_base.last = 5; g_base.target = &other;
    CCreatureAction_BriarRoseShotFarOutof action(&owner, &target);
    bool ok = g_base.owner == &owner && g_base.target == &target && g_base.code == 250 && g_base.first == 11 && g_base.last == 22 && g_soundOwner == &owner && std::strcmp(g_soundName, "SWORD_SWING") == 0 && action.PShot.GetPItem() == 0 && std::strcmp(g_animName, "STRIKE_SHOT_FAR_OUTOF") == 0;
    if (!ok) { std::printf("008c39a0_TEST FAIL\n"); return 1; }
    std::printf("008c39a0_TEST PASS\n");
    return 0;
}