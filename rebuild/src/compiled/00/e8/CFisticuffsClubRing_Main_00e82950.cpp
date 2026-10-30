// CFisticuffsClubRing::Main() -- the Fisticuffs Club ring trigger: every frame track whether the hero is inside the ring radius; inside, cancel the "step outside" warning and, while an active club fight is idle, have a club member tell the hero to stop interrupting (rate-limited); stepping out while in the club triggers Tyler's "step outside" warning with a HUD clock; while in the club, leaving the ring during a fight raises the boo meter and times the hero out, otherwise fights raise the cheer meter; both meters are clamped to 0..12
// Leaf: ?Main@CFisticuffsClubRing@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E82950, 1574 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CIsThingAlive dtor 0x00E241B0, RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CFisticuffsClubRing helper 0x00E83230 (manifest label "HandleVillageSelection" is a
//   propagated one), CV_FisticuffsClubScript::InitialiseVariables 0x00E82650 / GetQuestText 0x00E82F80, CTimer dtor 0x00CD4470,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA, __ftol2 0x00BFEA70, CCharString 0x0099EBF0 / 0x0099EAE0;
//   GameCalls slots 7, 70, 72, 77, 87-90 (timers through g_pGameScriptInterface 0x0143E8F8), 326 AddQuestInfoTimer,
//   338 RemoveQuestInfoElement, 364-366.
// Layout: Ego_r.pdb CFisticuffsClubRing: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CV_FisticuffsClubScript,
//   retail = PDB - 0x14), PMasterData +0x18. CScriptDef (pGlobalScriptDef 0x0143E90C, retail = PDB - 0x5c here): FC_OutsideRingRange
//   +0x5e4, FC_RingTimeOut +0x5e8, FC_RingRadius +0x61c, FC_InsideRingMessageDelay +0x620.
namespace NScript { class CPredicateBase; }
class CTimer;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBase(), PImp(other.PImp) { }
    ~CScriptThing();                                    // 0x004AA840
};
struct CBaseIntelligentPointer {                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
    CBaseIntelligentPointer();                          // 0x0099A380
    virtual ~CBaseIntelligentPointer();                 // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseIntelligentPointer { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CScriptThingCopy : CBase {                       // CScriptThing as the filter holds it: out-of-line copy and destructor
    virtual void s001();
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThingCopy(const CScriptThing& other);        // 0x004ABE90
    ~CScriptThingCopy();                                // 0x004AA840
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
    CScriptThingCopy m_thing;                           // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                   // 0x00E241B0
};
}
using NScript::CIsThingAlive;
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
class CScriptDef { public:
    unsigned char _pad0[0x5e4];
    float FC_OutsideRingRange;                          // +0x5e4
    float FC_RingTimeOut;                               // +0x5e8
    unsigned char _pad1[0x61c - 0x5ec];
    float FC_RingRadius;                                // +0x61c
    long  FC_InsideRingMessageDelay;                    // +0x620
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
struct CV_FisticuffsClubScript {
    unsigned char _pad0[0x4c];
    bool  FighterOneInRing;                             // +0x4c
    bool  FighterTwoInRing;                             // +0x4d
    unsigned char _pad1[0x50 - 0x4e];
    bool  HeroInsideRing;                               // +0x50
    unsigned char _pad2;
    bool  HeroPaidEntryFee;                             // +0x52
    bool  HeroInFisticuffsClub;                         // +0x53
    bool  TylerInFight;                                 // +0x54
    unsigned char _pad3;
    bool  TimeOut;                                      // +0x56
    unsigned char _pad4;
    bool  RoundStarted;                                 // +0x58
    unsigned char _pad5[0x8c - 0x59];
    bool  EverybodyAttack;                              // +0x8c
    bool  LostWhileOutside;                             // +0x8d
    bool  FisticuffsActive;                             // +0x8e
    unsigned char _pad6[0xac - 0x8f];
    float CheerNumber;                                  // +0xac
    float BooNumber;                                    // +0xb0
    void InitialiseVariables();                         // 0x00E82650
    CCharString GetQuestText(CCharString suffix);       // 0x00E82F80
};
inline void ClampMeter(float& meter)
{
    if (meter < 0.0f)
        meter = 0.0f;
    else if (meter > 12.0f)
        meter = 12.0f;
}
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CFisticuffsClubRing : CEntityScriptBase {
    CV_FisticuffsClubScript* ParentClass;               // +0x14
    void*                PMasterData;                   // +0x18
    void Main();
    void SetUpRing();                                   // 0x00E83230
};

void CFisticuffsClubRing::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    SetUpRing();
    ParentClass->InitialiseVariables();
    CTimer warningTimer;
    CTimer interruptTimer;
    float ringRadius = pGlobalScriptDef->FC_RingRadius;
    long warningInfo = 0;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), ringRadius)) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->HeroInsideRing = true;
            if (warningTimer.GetTime() != 0) {
                if (IsActiveThreadTerminating())
                    return;
                Game->RemoveQuestInfoElement(warningInfo);
                warningTimer = 0;
            }
            if (ParentClass->FisticuffsActive && !ParentClass->HeroInFisticuffsClub && !ParentClass->HeroPaidEntryFee
                && !ParentClass->EverybodyAttack && interruptTimer.GetTime() < 1) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                CScriptThing member = Game->GetNearestWithScriptName(Me, "FisticuffsMember");
                long conversation = Game->AddNewConversation(member, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, ParentClass->GetQuestText("INTERRUPT_FIGHT_10"), false, member, Game->GetHero());
                interruptTimer = pGlobalScriptDef->FC_InsideRingMessageDelay;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->HeroInFisticuffsClub && ParentClass->HeroInsideRing) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptThing tyler = Game->GetThingWithScriptName("Tyler");
                long conversation = Game->AddNewConversation(tyler, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, ParentClass->GetQuestText("STEP_OUTSIDE_RING_10"), false, tyler, Game->GetHero());
                warningTimer = (long)pGlobalScriptDef->FC_RingTimeOut;
                warningInfo = Game->AddQuestInfoTimer(warningTimer, "HUD_CLOCK_ICON", 1.0f);
            }
            ParentClass->HeroInsideRing = false;
        }
        if (ParentClass->HeroInFisticuffsClub) {
            if (IsActiveThreadTerminating())
                return;
            if ((ParentClass->RoundStarted || ParentClass->TylerInFight)
                && IsDistanceBetweenThingsOver(Me, Game->GetHero(), ringRadius)) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                ParentClass->BooNumber += 1.0f / 15.0f;
                if (warningTimer.GetTime() == 0
                    || IsDistanceBetweenThingsOver(Me, Game->GetHero(), pGlobalScriptDef->FC_OutsideRingRange)) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    ParentClass->LostWhileOutside = true;
                    ParentClass->TimeOut = true;
                    Game->RemoveQuestInfoElement(warningInfo);
                }
            } else if (ParentClass->TylerInFight) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                ParentClass->CheerNumber += 1.0f / 15.0f;
            } else {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                ParentClass->CheerNumber += 1.0f / 30.0f;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->FighterTwoInRing) {
                if (ParentClass->FighterOneInRing) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    ParentClass->CheerNumber += 1.0f / 75.0f;
                }
            } else if (ParentClass->FighterOneInRing) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                ParentClass->CheerNumber += 1.0f / 150.0f;
            }
        }
        ClampMeter(ParentClass->CheerNumber);
        ClampMeter(ParentClass->BooNumber);
        Game->NewScriptFrame();
        continue;
    }
}