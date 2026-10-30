// NScript::CQ_HeroSoulsNostroScript::CNostro::Main() -- Nostro entity script: while the thread runs, make Nostro invulnerable and faded, play his sound, wait for the vulnerability timer, then make him vulnerable and visible until the timer runs out again
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D89380, 0x1F8 bytes (padded to 512), `ret`.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (CIsThingAlive predicate, vtable
//   0x012C32F0), CEntityScriptBase::SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, FadeOutThing
//   0x00CBE814 (__fastcall thing, 1.0f, 0.3f), FadeInThing 0x00CBE7AA (__fastcall thing, 1.0f), CCharString 0x0099EBF0 /
//   0x0099EAE0. Entity interface (+4) slots 7 NewScriptFrame, 515 EntitySetAsRespondToHit, 516 EntitySetAsDamageable,
//   517 EntitySetAsKillable, 526 EntitySetAsToAddToComboMultiplierWhenHit, 690 PlaySoundOnThing, 745
//   SetIsThingForcePushable; global interface 0x0143E8F8 slot 90 GetTimer through the parent's CTimer.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14; the parent's NostroVulnerableTimer is at +0x54
//   (landed CSUMMONED_CREATURE::OnPredicateFail 0x00D89160: PDB 104, retail 0x14 lower).
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString { public:
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
void __fastcall FadeOutThing(const CScriptThing& thing, float time, float alpha);   // 0x00CBE814
void __fastcall FadeInThing(const CScriptThing& thing, float time);                 // 0x00CBE7AA
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_HeroSoulsNostroScript { public:
    unsigned char _pad[0x54];
    CTimer        NostroVulnerableTimer;           // +0x54

    class CNostro : public CEntityScriptBase {
    public:
                CQ_HeroSoulsNostroScript* ParentClass;     // +0x14
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
    };
};
void CQ_HeroSoulsNostroScript::CNostro::Main()
{
    {
        CIsThingAlive predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        Game->SetIsThingForcePushable(Me, false);
        Game->EntitySetAsKillable(Me, false, false);
        Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
        Game->EntitySetAsRespondToHit(Me, false);
        Game->EntitySetAsDamageable(Me, false);
        FadeOutThing(Me, 1.0f, 0.3f);
        {
            CCharString sound("SOUND");
            Game->PlaySoundOnThing(Me, sound);
        }
        while (ParentClass->NostroVulnerableTimer.GetTime() <= 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsKillable(Me, true, true);
        Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, true);
        Game->EntitySetAsRespondToHit(Me, true);
        Game->EntitySetAsDamageable(Me, true);
        FadeInThing(Me, 1.0f);
        while (ParentClass->NostroVulnerableTimer.GetTime()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}
}