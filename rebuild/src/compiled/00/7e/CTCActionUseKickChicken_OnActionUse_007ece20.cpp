// CTCActionUseKickChicken::OnActionUse(CThing& user) -- the user (a creature) is handed a CCombatAction_KickThingOnGround(user, this component's thing) temporary as its current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ECE20, length 55 bytes.
// Leaf: ?OnActionUse@CTCActionUseKickChicken@@UAEXAAVCThing@@@Z
// Callees: CCombatAction_KickThingOnGround::CCombatAction_KickThingOnGround(CThingCreatureBase*, CThing*) @ 0x0087FD20,
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, the action's destructor @ 0x0062E900 (the
// manifest's "~CTCLightningOrb" is propagated). Slot 28 is CTCActionUseBase::OnActionUse(CThing&) (Ego_r.pdb
// `?OnActionUse@CTCActionUseKickChicken@@UAEXAAVCThing@@@Z`); the chunk's "vfunc_28" is that slot.
// Shape: the 0x130-byte action is a temporary built in the call (`&CAction(..)` reuses the constructor's returned eax as
// the pushed argument) and destroyed after it. The owner thing is read through an inline CTCBase accessor: a direct
// `ParentThing` read is allocated to eax, the accessor lands it in ecx (`mov ecx,[ecx+4]`) as retail has it.
// engine/CThingCreatureBase.h / CCombatAction_KickThingOnGround.h / CCreatureActionBase.h are flattened PDB PODs; the
// calls need member declarations, so the classes are modelled locally.
// Port (NF93, q11261 / q11262): the local CCreatureActionBase / CThingCreatureBase / CCombatAction_KickThingOnGround / CTCBase / CTCActionUseBase
//   stand-ins replaced by the shared headers (the landed 0x007EB9B0 BlowHorn recipe) with the existing CCOMBATACTION_KICKTHINGONGROUND
//   retail block: SetCurrentAction 0x006644F0 (by reference), the ctor 0x0087FD20 ((CThingCreatureBase&, CThing&), Ego_r.pdb) and
//   ~CCombatActionBase 0x0062E900 now bind the PDB spellings. CTCActionUseKickChicken has no shared header: it stays row-local on the
//   shared CTCActionUseBase.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTION_KICKTHINGONGROUND
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatAction_KickThingOnGround.h"
#include "engine/CTCActionUseBase.h"

class CTCActionUseKickChicken : public CTCActionUseBase {             // no shared header
public:
    virtual void OnActionUse(CThing& user);                              // slot 28 0x007ECE20
};

void CTCActionUseKickChicken::OnActionUse(CThing& user)
{
    CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(user);
    creature.SetCurrentAction(CCombatAction_KickThingOnGround(creature, GetParentThing()));
}
