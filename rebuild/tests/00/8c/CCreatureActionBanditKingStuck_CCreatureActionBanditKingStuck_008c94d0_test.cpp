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
    void SetAnimationName(const CCharString& name) { g_animName = name.m_text; }
};
class CCreatureAction_BanditKingStuck : public CCombatActionBase { public:
    long TimesToLoop;
    CIntelligentPointer PStuckEffect;
    CIntelligentPointer PObstruction;
    CCreatureAction_BanditKingStuck(CThingCreatureBase* owner, CThing* target, long timesToLoop, CThing* obstruction, CThing* stuckEffect);
};
CCreatureAction_BanditKingStuck::CCreatureAction_BanditKingStuck(CThingCreatureBase* owner, CThing* target, long timesToLoop, CThing* obstruction, CThing* stuckEffect)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , TimesToLoop(timesToLoop)
    , PStuckEffect((CBaseObject*)stuckEffect)
    , PObstruction((CBaseObject*)obstruction)
{
}
int main()
{
    CThingCreatureBase owner; owner.id = 7;
    CThing target; target.id = 9;
    CThing other; other.id = 10;
    (void)target; (void)other;
    g_base.first = 5; g_base.last = 5; g_base.target = &other;
    CCreatureAction_BanditKingStuck action(&owner, &target, 3, &other, &target);
    bool ok = g_base.owner == &owner && g_base.target == &target && g_base.code == 150 && g_base.first == 0 && g_base.last == 0 && action.TimesToLoop == 3 && action.PStuckEffect.GetPItem() == (CBaseObject*)&target && action.PObstruction.GetPItem() == (CBaseObject*)&other;
    if (!ok) { std::printf("008c94d0_TEST FAIL\n"); return 1; }
    std::printf("008c94d0_TEST PASS\n");
    return 0;
}