// CCreatureAction_CastStrengthMultiStrike::FinishAction() -- base FinishAction; while strikes remain and owner and target are alive, chain a fresh multi-strike action with one strike fewer as the owner's current action, else remove the owner's interface-0x8c component
// Leaf: ?FinishAction@CCreatureAction_CastStrengthMultiStrike@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6AE0, length 202 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCombatActionBase::FinishAction 0x00859A20 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50 (owner +8, target +0xa8),
// CCreatureAction_CastStrengthMultiStrike::CCreatureAction_CastStrengthMultiStrike(CThingCreatureBase*, CThing*, long) 0x008E6A70,
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0, the temporary's destructor 0x0062E900 (a shared
// byte-identical body; manifest label "~CTCLightningOrb" is propagated), CThing::RemoveTC(long) 0x004C9840 (manifest label
// "CBaseObject::OnPropertyChanged" is propagated; named as in the landed 0x008C4020).
// Shape: the 0x118-byte action temporary fills the whole frame; the constant 1 lives in ebx for both the `> 1` compare and the
// dead-flag tests; target, owner and the call's owner are three named locals fetched in that order before the temporary is built
// (an inline `Owner.GetPItem()->SetCurrentAction(T(Owner.GetPItem(), ...))` fetches `this` after the temporary and drops the ebx 1).
// Layout: NumberOfStrikes at retail +0x114 (PDB +0x12c); thing byte +0x91 bit 0 = dead.
// Port (NF93, q11261 / q11262): the local CThingCreatureBase / CCreatureActionBase / CCombatActionBase (Owner / Target stand-in member names) /
//   CCreatureAction_CastStrengthMultiStrike stand-ins replaced by the shared headers (the 0x008C2980 recipe) with the NF75 _CLASS block and the
//   NF93 opt-in CCREATUREACTION_CASTSTRENGTHMULTISTRIKE_CTOR: SetCurrentAction 0x006644F0, the ctor 0x008E6A70 and ~CCombatActionBase 0x0062E900
//   now bind the PDB spellings; members are the Ego_r PCreature (+8) / PTarget (+0xa8).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCREATUREACTION_CASTSTRENGTHMULTISTRIKE_CLASS
#define FABLE_RETAIL_CCREATUREACTION_CASTSTRENGTHMULTISTRIKE_CTOR
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_CastStrengthMultiStrike.h"

void CCreatureAction_CastStrengthMultiStrike::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (NumberOfStrikes > 1 && PCreature.Get() && !PCreature.Get()->IsDead()
        && PTarget.Get() && !PTarget.Get()->IsDead()) {
        CThing* ptarget = PTarget.Get();
        CThingCreatureBase* powner = PCreature.Get();
        CThingCreatureBase* pcreature = PCreature.Get();
        pcreature->SetCurrentAction(CCreatureAction_CastStrengthMultiStrike(*powner, *ptarget, NumberOfStrikes - 1));
    } else {
        PCreature.Get()->RemoveTC((ETCInterfaceType)(0x8c));
    }
}
