// CSpectator::Main() -- hero-duel spectator: put Me in limbo until the duel is set up, take Me under scripted control, wait for the duel start, then until the duel ends: play a random cheer animation whenever Me is idle, shout a random cheering line (from the last five once the hero has been jeered) whenever the cheer timer runs out, alternately facing the hero and Thunder, and count the hero's hits on Me -- the third hit turns the crowd to jeering
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E998F0, 2019 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Literals "Thunder" 0x012ED164, "SCRIPT_CHEER_1/2/3" 0x01292930 / 0x01292910 / 0x01292920, "TEXT_QST_044_HERO_DUEL_CHEERING_10..100"
//   0x012ED34C..0x012ED208, "TEXT_QST_044_HERO_DUEL_JEERING_10" 0x012ED1E4, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, ~CIsThingAlive 0x00E241B0, RegisterFilter 0x00F35B10, IsActiveThreadTerminating
//   0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, handle IsScripting 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation 0x007E73D0 /
//   IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0, CTimer ctor 0x00CD4450 / dtor 0x00CD4470, rand 0x00BFEB16, the eh vector
//   destructor iterator 0x00401400 (the ten cheering lines); GameCalls slots 7, 8, 70, 72, 87-90 timers (g_pGameScriptInterface
//   0x0143E8F8), 364 AddNewConversation, 366 AddLineToConversation, 453 EntitySetInLimbo, 475 EntitySetFacingAngleTowardsThing.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (duel quest: set-up +0x4b, started +0x4c, cheer due +0x50,
//   jeering +0x51, duel over +0x52, spectators released +0x53, hero hit count +0x54), PMasterData +0x18, facing-hero toggle +0x1c.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
int __cdecl rand();                                     // 0x00BFEB16 (MSVCR71)
class CCPPointerInfo;
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const;
    virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const; virtual void s031() const;
    virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const; virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    void* PImpData;                                     // +0x04
    void* PImpInfo;                                     // +0x08
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                   // 0x00E241B0
};
}
using NScript::CIsThingAlive;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CBaseObject { public:                        // vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    void* PImpData;                                     // +0x08
    void* PImpInfo;                                     // +0x0c
    CScriptGameResourceObjectScriptedThingBase() { PImpData = 0; PImpInfo = 0; }
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
public:
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
    virtual void PlayAnimation(const CCharString&, bool, bool, bool, bool, bool, bool, bool);   // 0x007E73D0
};
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
extern bool g_bScriptAnimationFlag;                    // 0x01375748
struct CQ_HeroDuelScript {
    unsigned char _pad00[0x4b];
    bool DuelSetUp;                                     // +0x4b
    bool DuelStarted;                                   // +0x4c
    unsigned char _pad4d[3];
    bool CheerDue;                                      // +0x50
    bool Jeering;                                       // +0x51
    bool DuelOver;                                      // +0x52
    bool SpectatorsReleased;                            // +0x53
    long HeroHits;                                      // +0x54
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CSpectator : CEntityScriptBase {
    CQ_HeroDuelScript* ParentClass;                     // +0x14
    void*              PMasterData;                     // +0x18
    bool               FacingHero;                      // +0x1c
    void Main();
};

void CSpectator::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    if (!ParentClass->DuelSetUp) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetInLimbo(Me, true, false);
    }
    CScriptGameResourceObjectScriptedThingBase face;
    if (face.IsScripting())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->DuelSetUp) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetInLimbo(Me, false, true);
    if (face.IsScripting())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->DuelStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer cheerTimer;
    cheerTimer.SetTime(rand() % 60 + 2);
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("Thunder"), true);
    while (!ParentClass->DuelOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (cheerTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->CheerDue = true;
        }
        while (face.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        switch (rand() % 3) {
        case 0:
            face.PlayAnimation("SCRIPT_CHEER_1", false, false, false, true, g_bScriptAnimationFlag, false, false);
            break;
        case 1:
            face.PlayAnimation("SCRIPT_CHEER_2", false, false, false, true, g_bScriptAnimationFlag, false, false);
            break;
        case 2:
            face.PlayAnimation("SCRIPT_CHEER_3", false, false, false, true, g_bScriptAnimationFlag, false, false);
            break;
        default:
            face.PlayAnimation("SCRIPT_CHEER_1", false, false, false, true, g_bScriptAnimationFlag, false, false);
            break;
        }
        if (ParentClass->CheerDue) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->CheerDue = false;
            CCharString lines[10] = {
                "TEXT_QST_044_HERO_DUEL_CHEERING_10", "TEXT_QST_044_HERO_DUEL_CHEERING_20",
                "TEXT_QST_044_HERO_DUEL_CHEERING_30", "TEXT_QST_044_HERO_DUEL_CHEERING_40",
                "TEXT_QST_044_HERO_DUEL_CHEERING_50", "TEXT_QST_044_HERO_DUEL_CHEERING_60",
                "TEXT_QST_044_HERO_DUEL_CHEERING_70", "TEXT_QST_044_HERO_DUEL_CHEERING_80",
                "TEXT_QST_044_HERO_DUEL_CHEERING_90", "TEXT_QST_044_HERO_DUEL_CHEERING_100"
            };
            long line;
            if (ParentClass->Jeering) {
                if (IsActiveThreadTerminating())
                    return;
                line = rand() % 5 + 5;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                line = rand() % 10;
            }
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddLineToConversation(conversation, lines[line], false, Me, Game->GetHero());
            if (FacingHero) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), true);
                FacingHero = false;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("Thunder"), true);
                FacingHero = true;
            }
            cheerTimer.SetTime(rand() % 60 + 2);
        }
        if (!ParentClass->Jeering) {
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return;
                ++ParentClass->HeroHits;
                if (ParentClass->HeroHits >= 3) {
                    if (IsActiveThreadTerminating())
                        return;
                    ParentClass->Jeering = true;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddLineToConversation(conversation, "TEXT_QST_044_HERO_DUEL_JEERING_10", false, Me, Game->GetHero());
                }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass->SpectatorsReleased) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->SpectatorsReleased = true;
    }
}