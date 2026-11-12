// NScript::CV_TalentlessBardScript::WatchForResetWanderCentre(CScriptThing bard) -- with an is-alive predicate on the bard, each time the bard wander timer runs out reset his wander centre (on the hero when he is following, else where he stands), his wander distances from the script def, put him in the wander-near state group and restart the timer
// Leaf: ?WatchForResetWanderCentre@CV_TalentlessBardScript@NScript@@QAEXVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED94D0, 0x2AF bytes, `ret 0Ch` (the by-value CScriptThing).
// Identity: Ego_r.pdb ?WatchForResetWanderCentre@CV_TalentlessBardScript@NScript@@QAEXVCScriptThing@@@Z; the bard script's
//   spawned watchers (sibling WatchForAttack 0x00ED9790, the same predicate + yield prologue).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90, CScriptBase::SetPredicate 0x00CB7920, ~CIsThingAlive 0x00E241B0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing
//   0x004AA840 (out of line at the four plain-`if` returns); Game (+0x40) slots 7 NewScriptFrame, 70 GetHero, 763
//   SetWanderCentrePoint, 766 SetWanderMinDistance, 769 SetWanderMaxDistance, 784 SetScriptingStateGroup (ESSG_WANDER_NEAR = 4);
//   CTimer::GetTime / operator= expanded (pDSTGame 0x0143E8F8 slots 90 GetTimer / 89 SetTimer); the hero's GetPos through its
//   vtable (slot 6), the bard's expanded (null PImp: CScriptThing::NullCoord 0x0143E8E0); CScriptThing::IsAlive expanded (the
//   impl's slot 75). pGlobalScriptDef 0x0143E90C BardWanderDistanceMin +0x7c0 / Max +0x7c4 / BardWanderResetTime +0x7c8.
// Layout: hdr engine/NScript_CV_TalentlessBardScript.h (BardFollowing +0x62, BardWanderTime +0x64).
// Shape: the sibling WatchForAttack's (landed 0x00ED9790): plain-inline ~CScriptThing; the exits retail expands the dtor at (the
//   prologue, the loop condition, the wait loop's return) are `do { if (...) return; } while (0);`, the others plain `if`.
//   IsAlive is the `PImp.Get() && PImp->IsAlive()` form (retail `test al,al`; the two-return form gives `cmp al,bl`), and the
//   CIsThingAlive ctor is defined inline (retail stores the vtable 0x012C32F0 and copies Thing in place).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/NScript_CV_TalentlessBardScript.h"
#include "engine/fable_script_def_global.h"
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Get() && PImp->IsAlive(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CV_TalentlessBardScript::WatchForResetWanderCentre(CScriptThing bard)
{
    SetPredicate(CIsThingAlive(bard));
    Game.NewScriptFrame();
    do { if (IsActiveThreadTerminating()) return; } while (0);
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        while (BardWanderTime.GetTime() != 0) {
            Game.NewScriptFrame();
            do { if (IsActiveThreadTerminating()) return; } while (0);
        }
        if (IsActiveThreadTerminating())
            return;
        if (bard.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (BardFollowing) {
                if (IsActiveThreadTerminating())
                    return;
                Game.SetWanderCentrePoint(bard, Game.GetHero().GetPos());
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game.SetWanderCentrePoint(bard, bard.GetPos());
            }
            Game.SetWanderMinDistance(bard, pGlobalScriptDef->BardWanderDistanceMin);
            Game.SetWanderMaxDistance(bard, pGlobalScriptDef->BardWanderDistanceMax);
            Game.SetScriptingStateGroup(bard, ESSG_WANDER_NEAR);
            BardWanderTime = pGlobalScriptDef->BardWanderResetTime;
        }
        Game.NewScriptFrame();
    }
}