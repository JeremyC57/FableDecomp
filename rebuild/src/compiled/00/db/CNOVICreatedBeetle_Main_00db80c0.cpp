// CNOVI_CreatedBeetle::Main() -- the beetle spawned in Oakvale's intro: register an is-alive functor over Me, run a frame, then give it five seconds on a script timer and remove it (with effects) unless the script finished first
// Grade: RELOCATION_MATCH -- the closing test is written `if (IsActiveThreadTerminating()) return; Game->RemoveThing(...)`: an early return there keeps a second CTimer-dtor + epilogue copy like retail, where `if (!IsActiveThreadTerminating()) RemoveThing` let VC cross-jump the loop exit into the final DeregisterTimer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB80C0, 232 bytes (+ int3 padding). __fastcall, `ret`.
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (the functor's member; the functor's inline dtor re-stores its
//   vtable 0x012C2FE8 first), the entity script's functor registration 0x00F35B10 and IsDone 0x00F35B30 (on `this`; manifest
//   labels "CWorldMap::AllocateMapThingUID" / "CCreatureAction_TrollWhackGroundBase::Initialise" are propagated, see landed
//   CHTE_Prisoner::Main 0x00D69F50); the script's own interface (+4) NewScriptFrame 7 (+0x1c) 0x006E7410 and RemoveThing 108
//   (+0x1b0) 0x008910D0; the global g_pGameScriptInterface (0x0143E8F8) RegisterTimer 87 (+0x15c) 0x006E79E0, DeregisterTimer
//   88 (+0x160) 0x006E7690, SetTimer 89 (+0x164) 0x006E76E0, GetTimer 90 (+0x168) 0x006E7710.
// Layout: Ego_r.pdb CNOVI_CreatedBeetle (28 bytes) = CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
//   PMasterData +0x18.
// Shape: the functor is a scoped 0x10-byte frame object; the timer index stays in edi; the early exit inside the wait loop
//   deregisters the timer itself (its own copy of the epilogue), the normal path falls through to the same call.
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CScriptThingFunctor_00ef7ab0 : public NScript::CPredicateBase {                  // vtable 0x012C2FE8: an is-alive functor holding a script thing
    CScriptThing m_thing;                              // +0x04
    CScriptThingFunctor_00ef7ab0(const CScriptThing& thing) : m_thing(thing) { }
    ~CScriptThingFunctor_00ef7ab0() { }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:                                        // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer(long time) { TimerIndex = g_pGameScriptInterface->RegisterTimer(); g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
struct CQ_NewOakValeIntroScript;
struct CQ_SunnyvaleMasterData;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

struct CNOVI_CreatedBeetle : public CEntityScriptBase {
        CQ_NewOakValeIntroScript* ParentClass;             // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;             // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};

void CNOVI_CreatedBeetle::Main()
{
    {
        CScriptThingFunctor_00ef7ab0 functor(Me);
        SetPredicate(functor);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CTimer timer(5);
    while (timer.GetTime() != 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, true, true);
}