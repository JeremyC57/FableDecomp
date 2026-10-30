// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Init() -- the assassin: reset the threat state (threat distance from the script def); removed when the sheriff's state is 0, otherwise made unpushable, unkillable and undamageable, looking at the hero, informative, friends with everything and persistent
// Leaf: ?Init@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB3E40, 247 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012F00BC, which the landed
//   NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Alloc 0x00EB3C60 stores (site 0x00EB3C95); Ego_r.pdb has ?Init@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMurderTwistAssassin (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 108 RemoveThing, 745 SetIsThingForcePushable, 844 SetIsPushableByHero (Me by value), 529
//   EntitySetAsUseMovementInActions, 517 EntitySetAsKillable, 516 EntitySetAsDamageable, 70 GetHero, 498
//   EntityForceToLookAtThing, 360 SetThingHasInformation, 477 SetThingPersistent; CScriptThing slot 67
//   SetFriendsWithEverythingFlag. Data: pGlobalScriptDef 0x0143E90C, AssassinThreatDistSq at retail +0x720.
class CScriptDef {                                      // Ego_r.pdb CScriptDef; retail offsets (agent501: HeroKillsSheriffRenown +0x71c)
public:
    unsigned char _pad0[0x720];
    float AssassinThreatDistSq;                         // +0x720 (Ego_r +0x77c)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
enum ESheriffState { };
enum EAssassinState { };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0, implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CScriptGameResourceObjectScriptedThingBase {      // Ego_r.pdb, 16 bytes: vptr, base word, PImp
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    long _base;                                         // +0x04 (unnamed in the PDB layout)
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    ESheriffState SheriffState;                         // +0x48
    EAssassinState AssassinState;                       // +0x4c
    bool TaskDeclined;                                  // +0x50
    bool PreemptiveAttackInProgress;                    // +0x51
    CScriptThing Sheriff;                               // +0x54
    std::vector<CScriptThing> Guards;                   // +0x60
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;   // +0x6c
    std::vector<CScriptGameResourceObjectScriptedThingBase> seh_Guards;   // +0x7c
    class CMurderTwistAssassin;
};
class CV_MurderTwistFinalScript::CMurderTwistAssassin : public CEntityScriptBase {
public:
    CV_MurderTwistFinalScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    unsigned long NextLine;                             // +0x2c
    bool HasBeenThreatened;                             // +0x30
    float ThreatDistSq;                                 // +0x34
    bool HasGivenIntroToHero;                           // +0x38
    bool PreemptiveAttackInProgress;                    // +0x39
private:
    virtual void Init();
};

void CV_MurderTwistFinalScript::CMurderTwistAssassin::Init()
{
    HasBeenThreatened = false;
    ThreatDistSq = pGlobalScriptDef->AssassinThreatDistSq;
    NextLine = 1;
    HasGivenIntroToHero = false;
    PreemptiveAttackInProgress = false;
    if (ParentClass->SheriffState == 0) {
        Game->RemoveThing(Me, false, true);
        return;
    }
    Game->SetIsThingForcePushable(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsUseMovementInActions(Me, false);
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntityForceToLookAtThing(Me, Game->GetHero());
    Game->SetThingHasInformation(Me, false, true, false);
    Me.SetFriendsWithEverythingFlag(true);
    Game->SetThingPersistent(Me, true);
}
}