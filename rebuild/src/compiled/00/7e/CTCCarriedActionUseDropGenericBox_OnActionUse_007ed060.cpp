// CTCCarriedActionUseDropGenericBox::OnActionUse(CThing& user) [Ego_r.pdb; was vfunc_28(CThingAICreature*)] -- runs the base carried-action hook, then, when the creature's flag 8 (+0x6c) is set, makes it perform a drop-generic-box action
// Leaf: ?OnActionUse@CTCCarriedActionUseDropGenericBox@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED060, length 63 bytes. CTCCarriedActionUseDropGenericBox vtable 0x0123FA2C slot 28.
// Callees: the base hook @ 0x007EAB90 (empty `ret 4`, another TU), CCreatureAction_DropGenericBox::CCreatureAction_DropGenericBox
// (CThingCreatureBase&) @ 0x00842150 (agentNF16: the object stores vtable 0x012731E4, RTTI CCreatureAction_DropGenericBox; Ego_r + FableWin
// ??0CCreatureAction_DropGenericBox@@QAE@AAVCThingCreatureBase@@@Z; was the stand-in N_00842150::CCreatureAction_AlertToPlayer(CThingAICreature*)), CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0
// (manifest), the action's destructor @ 0x00693EF0 (the manifest's "~CPhysicsMeshInfo" label is propagated).
// Shape: the 0xA8-byte action is a temporary built in the frame and passed by address straight from the
// constructor's returned `this` (`push eax`), the GOTCHAS `PerformActionAndWait(&CAction(creature))`
// form; it is destroyed right after the call. No EH frame: the TU is built without /EHsc.
// Port (NF93, q11261): the local CThingCreatureBase / CCreatureActionBase / CCreatureAction_DropGenericBox / CThingAICreature / CTCActionUseBase /
//   CTCCarriedActionUseBase stand-ins replaced by the shared headers (the 0x007EB9B0 BlowHorn recipe) with the existing
//   CCREATUREACTION_GENERICBOX retail block: SetCurrentAction 0x006644F0 (by reference) and ~CCreatureActionBase 0x00693EF0 now bind the
//   PDB spellings. The +0x6c flag 8 test is CThing::GetTypeFlags() & 8 (the CThingCreatureBase type bit, as the landed 0x008C92C0 /
//   0x00839100). CTCCarriedActionUseDropGenericBox has no shared header: it stays row-local on the shared CTCCarriedActionUseBase.
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
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_GENERICBOX
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_DropGenericBox.h"
#include "engine/CTCCarriedActionUseBase.h"

class CTCCarriedActionUseDropGenericBox : public CTCCarriedActionUseBase {   // no shared header
public:
    virtual void OnActionUse(CThing& user);                              // slot 28 0x007ED060
};

void CTCCarriedActionUseDropGenericBox::OnActionUse(CThing& thing)
{
    CTCCarriedActionUseBase::OnActionUse(thing);
    if (thing.GetTypeFlags() & 8)
        static_cast<CThingCreatureBase&>(thing).SetCurrentAction(CCreatureAction_DropGenericBox(static_cast<CThingCreatureBase&>(thing)));
}
