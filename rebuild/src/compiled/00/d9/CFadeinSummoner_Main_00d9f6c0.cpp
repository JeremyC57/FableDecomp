// CFadeinSummoner::Main() -- a Minion Camp summoner that fades in: take scripted control, make Me an inert neutral prop with an altar effect, wait for mission stage 2, then while the stage is 2 or 3: become the hero's ally when he hits Me, keep fading (PhaseMe), re-create the altar effect when it died, and every 20-25 seconds play one of two special idles before going back to breathing; afterwards remove the effect and idle unscripted until the thread ends
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9F6C0, 1720 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 / operator= 0x008AB980, RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle IsScripting 0x00CD23B9 /
//   Release 0x00CD2770 / PlayAnimation 0x007E73D0 / PlayLoopingAnimation 0x007E73F0 / ClearAllActionsIncludingLoopingAnimations
//   0x007E7410 / dtor 0x007E74D0, CTimer ctor 0x00CD4450 / dtor 0x00CD4470, CFadeinSummoner::PhaseMe 0x00D9FD80, rand 0x00BFEB16,
//   CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 8, 70, 87-90 (timers through g_pGameScriptInterface 0x0143E8F8),
//   100 CreateEffect, 108 RemoveThing, 352, 469, 515-517, 526, 599, 601, 745, 844; CScriptThing slots 6, 21, 41, 42, 75 (IsAlive).
//   Global bool 0x01375748 (animation default).
// Layout: Ego_r.pdb CFadeinSummoner (40 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_MinionCampScript:
//   CurrentStage PDB +0x88 = retail +0x74, the usual 0x14 quest-script shift), PMasterData +0x18, MyAlpha +0x1c, AlphaGradient
//   +0x20, AlphaMax +0x24 (used by PhaseMe). CScriptThing copy is expanded inline at the by-value SetIsPushableByHero argument.
namespace NScript { class CPredicateBase; }
enum EHeroAbility { HERO_ABILITY_NONE };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const;
    virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const;
    virtual void s030() const; virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const;
    virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    virtual void s043() const; virtual void s044() const; virtual void s045() const; virtual void s046() const; virtual void s047() const; virtual void s048() const; virtual void s049() const; virtual void s050() const;
    virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const; virtual void s055() const; virtual void s056() const; virtual void s057() const; virtual void s058() const;
    virtual void s059() const; virtual void s060() const; virtual void s061() const; virtual void s062() const; virtual void s063() const; virtual void s064() const; virtual void s065() const; virtual void s066() const;
    virtual void s067() const; virtual void s068() const; virtual void s069() const; virtual void s070() const; virtual void s071() const; virtual void s072() const; virtual void s073() const; virtual void s074() const;
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    virtual bool IsDead() const { return !IsAlive(); }   // slot 76 (+0x130)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other) : CBase(), PImp(other.PImp) { }   // out-of-line copy 0x004ABE90
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBase(), PImp(other.PImp) { }
    virtual ~CScriptThing();                                    // 0x004AA840
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
namespace NScript {
class __declspec(novtable) CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04 (copied and destroyed out of line)
    CIsThingAlive(const CScriptThing& thing);
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
// The filter's constructor is inlined into Main, but its CScriptThing copy stays an out-of-line call (0x004ABE90) while
// the by-value SetIsPushableByHero argument expands the same copy constructor inline.
#pragma inline_depth(0)
inline CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
#pragma inline_depth()
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer();                                           // 0x00CD4450
    ~CTimer();                                          // 0x00CD4470
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
#pragma auto_inline(off)
CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#pragma auto_inline(on)
extern "C" int __cdecl rand(void);                    // 0x00BFEB16
extern bool g_bScriptAnimationFlag;                     // 0x01375748
struct CQ_MinionCampScript {
    unsigned char _pad[0x74];
    long CurrentStage;                                  // +0x74 (PDB +0x88, MissionStage)
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CFadeinSummoner : CEntityScriptBase {
    CQ_MinionCampScript* ParentClass;                   // +0x14
    void*                PMasterData;                   // +0x18
    float                MyAlpha;                       // +0x1c
    float                AlphaGradient;                 // +0x20
    bool                 AlphaMax;                      // +0x24
    void Main();
    void PhaseMe();                                     // 0x00D9FD80
};

void CFadeinSummoner::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase summoner;
    if (summoner.IsValid())
        summoner.Release();
    while (!Game->StartScriptingEntity(Me, summoner, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetIsPushableByHero(Me, false);
    Game->SetIsThingForcePushable(Me, false);
    Game->EntitySetAsRespondToHit(Me, false);
    Game->EntitySetAsDamageable(Me, false);
    Game->MiniMapRemoveMarker(Me);
    Game->EntitySetTargetable(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_NEUTRAL");
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    CScriptThing effect = Game->CreateEffect("SUMMONER_ALTER_ACTIVE", Me.GetPos(), "", 0.0f, false, false);
    while (ParentClass->CurrentStage != 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    summoner.PlayLoopingAnimation("BREATHE_IDLE", -1, false, false, false, true, g_bScriptAnimationFlag, false, false);
    CTimer idleTimer;
    idleTimer.SetTime(25);
    CTimer effectTimer;
    effectTimer.SetTime(12);
    CTimer specialTimer;
    bool playingSpecial = false;
    while (ParentClass->CurrentStage == 2 || ParentClass->CurrentStage == 3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
        }
        PhaseMe();
        if (effectTimer.GetTime() == 0 && effect.IsDead()) {
            if (IsActiveThreadTerminating())
                return;
            effect = Game->CreateEffect("SUMMONER_ALTAR_ACTIVE", Me.GetPos(), "", 0.0f, false, false);
            effectTimer.SetTime(8);
        }
        if (idleTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            summoner.ClearAllActionsIncludingLoopingAnimations();
            if (rand() % 2 == 0) {
                if (IsActiveThreadTerminating())
                    return;
                summoner.PlayAnimation("MC_SPECIAL_IDLE_1", false, true, false, false, g_bScriptAnimationFlag, false, false);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                summoner.PlayAnimation("MC_SPECIAL_IDLE_2", false, true, false, false, g_bScriptAnimationFlag, false, false);
            }
            idleTimer.SetTime(20);
            playingSpecial = true;
            specialTimer.SetTime(4);
        }
        if (playingSpecial && specialTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            summoner.PlayLoopingAnimation("BREATHE_IDLE", -1, false, false, false, true, g_bScriptAnimationFlag, false, false);
            playingSpecial = false;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(effect, false, true);
    if (summoner.IsValid())
        summoner.Release();
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}