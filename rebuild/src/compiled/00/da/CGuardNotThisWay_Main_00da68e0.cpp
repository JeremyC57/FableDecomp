// CGuardNotThisWay::Main() -- the minion clifftop-chase guard who blocks the wrong way: take scripted control, attach Me to the active quest, face the hero, then every frame -- when the 15-tick timer has run out and the hero is within 12 units -- point aggressively at the summoner's run-off point, tell the hero "not this way" (two lines), point again and restart the timer
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA68E0, 1125 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle IsValid 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation
//   0x007E73D0 / IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0, CScriptThing copy 0x004ABE90 / dtor 0x004AA840, RegisterFilter
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CTimer dtor 0x00CD4470 (inline copies elsewhere), IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 8, 70 GetHero, 72 GetThingWithScriptName,
//   87-90 timers (through g_pGameScriptInterface 0x0143E8F8), 364-366 conversations, 465 EntityAttachToScript,
//   475 EntitySetFacingAngleTowardsThing, 655 GetActiveQuestName. Global bool 0x01375748 (animation default).
// Layout: Ego_r.pdb CGuardNotThisWay (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_MinionClifftopChaseScript), PMasterData +0x18. CTimer as the matched CGuildmaster_HS::Main 0x00D85B40.
// Shape: CTimer's __forceinline destructor expands at the loop exit and the first in-loop exit; inline_depth(0) on the four
//   animation-wait returns gives retail's out-of-line ~CTimer copy there (with a plain inline destructor the pragma sends
//   EVERY exit out of line). The loop is `for (;;) { if (IsActiveThreadTerminating()) return; ... }`.
namespace NScript { class CPredicateBase; }
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
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
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
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
__forceinline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern bool g_bScriptAnimationFlag;                     // 0x01375748
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CGuardNotThisWay : CEntityScriptBase {
    void* ParentClass;                                  // +0x14 CQ_MinionClifftopChaseScript*
    void* PMasterData;                                  // +0x18
    void Main();
};

void CGuardNotThisWay::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase guard;
    if (guard.IsValid())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
    CTimer timer;
    timer = 0;
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 12.0f)) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("M_SummonerRunOffPoint"), false);
            guard.PlayAnimation("AGGRESSIVE_POINTING", false, false, false, true, g_bScriptAnimationFlag, false, false);
            while (guard.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
            }
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            Game->AddLineToConversation(conversation, "TEXT_QST_045_GUARD_NOT_THIS_WAY_10", false, Me, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_045_GUARD_NOT_THIS_WAY_20", false, Me, Game->GetHero());
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("M_SummonerRunOffPoint"), false);
            guard.PlayAnimation("AGGRESSIVE_POINTING", false, false, false, true, g_bScriptAnimationFlag, false, false);
            while (guard.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
            }
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            timer = 15;
        }
        Game->NewScriptFrame();
    }
}