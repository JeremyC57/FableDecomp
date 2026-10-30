// NScript::CV_BodyGuardScript::CBodyGuard::Init() -- set a henchman up: no health bar, brain state 1, health states and freak-out cleared, weapons out, bodyguard faction, follows the hero through teleporters, has information, persistent, may follow the hero, wants no money, ignores follow/wait expressions, wanders 0..10 around its home position, and the anger timer reset
// Leaf: ?Init@CBodyGuard@CV_BodyGuardScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5DE80, 364 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): the row is referenced once, from slot 2 (Init) of the RTTI-less entity vtable 0x012E701C (0x012E7024),
//   which the landed NScript::CV_BodyGuardScript::CBodyGuard::CBodyGuard 0x00E5DDE0 stores and whose slots 4 / 5 are the
//   landed CBodyGuard::OnPersist 0x00E5F190 / OnPredicateFail 0x00E5FB80; Ego_r.pdb
//   ?Init@CBodyGuard@CV_BodyGuardScript@NScript@@EAEXXZ (0x00EB5930) right after ?Alloc@CBodyGuard@... (0x00EB5820), as
//   retail Init follows Alloc 0x00E5DD60 / ctor 0x00E5DDE0. Entity slot order (landed CTyler::Init 0x00E836F0): 0 dtor,
//   1 Main, 2 Init, 3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted. Catalog label sub_00e5de80.
// Callees (Game slots, ghidra_out/cgsi_slots_aligned.tsv): 506 EntitySheatheWeapons, 601 EntitySetInFaction, 481
//   SetEntityAsFollowingHeroThroughTeleporters, 360 SetThingHasInformation, 477 SetThingPersistent, 235
//   EntitySetAsAllowedToFollowHero, 478 EntitySetThingAsWantingMoney, 237 EntitySetAsRespondingToFollowAndWaitExpressions,
//   766 SetWanderMinDistance / 769 SetWanderMaxDistance / 763 SetWanderCentrePoint (CScriptThing by value); CScriptThing slot 7
//   GetHomePos (C3DVector built in the argument slot); the global interface 0x0143E8F8 slot 89 SetTimer (inline
//   CTimer::operator=(long), Ego_r ??4CTimer@@QAEAAV0@J@Z); CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0.
// Literal: "FACTION_BODYGUARD" 0x012D649C.
// Layout: Ego_r.pdb CBodyGuard (entity offsets equal retail, as the landed ctor has them): Me +0x08, BrainState +0x20,
//   CurrentHealthState +0x24, PreviousHealthState +0x28, FreakedOut +0x2c, AngerTimer +0x30, MyHealthBarID +0x38.
// Shape: the landed CTyler::Init model (CScriptThing with inline CBaseClass ctor and inline copy ctor: the by-value Me is
//   built in its argument slot with both vtable stores); retail stores MyHealthBarID first.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers
//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface; no PDB data symbol)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BodyGuardScript {
public:
    class CBodyGuard;
};
class CV_BodyGuardScript::CBodyGuard : public CEntityScriptBase {
public:
    CV_BodyGuardScript*     ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    long                    HenchManType;               // +0x1c
    long                    BrainState;                 // +0x20
    long                    CurrentHealthState;         // +0x24
    long                    PreviousHealthState;        // +0x28
    bool                    FreakedOut;                 // +0x2c
    CTimer                  AngerTimer;                 // +0x30
    CTimer                  AttackedBySummonersTimer;   // +0x34
    long                    MyHealthBarID;              // +0x38
private:
    virtual void Init();
};

void CV_BodyGuardScript::CBodyGuard::Init()
{
    MyHealthBarID = -1;
    BrainState = 1;
    CurrentHealthState = 0;
    PreviousHealthState = 0;
    FreakedOut = false;
    Game->EntitySheatheWeapons(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_BODYGUARD");
    Game->SetEntityAsFollowingHeroThroughTeleporters(Me, true);
    Game->SetThingHasInformation(Me, false, true, false);
    Game->SetThingPersistent(Me, true);
    Game->EntitySetAsAllowedToFollowHero(Me, false);
    Game->EntitySetThingAsWantingMoney(Me, 0);
    Game->EntitySetAsRespondingToFollowAndWaitExpressions(Me, false);
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, 10.0f);
    Game->SetWanderCentrePoint(Me, Me.GetHomePos());
    AngerTimer = 0;
}
}   // namespace NScript