// CTCActionUseBlowHorn::OnActionUse(CThing& user) [Ego_r.pdb; was vfunc_28(CThingCreatureBase_Retail*)] -- run the base slot, then make a live creature blow this horn
// Leaf: ?OnActionUse@CTCActionUseBlowHorn@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EB9B0, length 90 bytes. Slot 28 of the CTCActionUseBlowHorn
// vtable 0x0123F564 (rtti-vtables.tsv).
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (an empty `ret 4`, called non-virtually),
// CCreatureAction_BlowHorn::CCreatureAction_BlowHorn(CThingCreatureBase&, CThing&) @ 0x00842EC0,
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0, CBaseIntelligentPointer::
// ~CBaseIntelligentPointer @ 0x00A01C10 (the action's PObjectToUse), CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0.
// Identity (agentNF77, q10884): Ego_r.pdb / FableWin.pdb ?OnActionUse@CTCActionUseBlowHorn@@UAEXAAVCThing@@@Z. The action temporary's
//   destruction ends in ??1CCreatureActionBase@@UAE@XZ (both PDBs; RTTI CCreatureAction_BlowHorn : CCreatureActionBase), formerly bound as a
//   local ??1CCreatureActionBase_Retail@@QAE@XZ; SetCurrentAction now binds its PDB spelling ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z.
// Layout: engine/CCreatureAction_BlowHorn.h (overlay retail opt-in FABLE_RETAIL_CCREATUREACTION_BLOWHORN: on CCreatureActionBase, PObjectToUse
//   +0xA8, sizeof 0xB0, no user-declared destructor, as retail's compiler-generated one), engine/CCreatureActionBase.h, CThingCreatureBase.h,
//   CTCActionUseBase.h (VSLOTS: slot 28 OnActionUse) over CTCBase.h (ParentThing +0x04). CTCActionUseBlowHorn has no shared header: local, on
//   the shared base (Ego_r.pdb: no own members). Was a local CCreatureActionBase_Retail / CThingCreatureBase_Retail /
//   CCreatureAction_BlowHorn_Retail / CTCActionUseBase model.
// Shape: the creature argument is held in esi across the base call; a null or dead creature does nothing; otherwise the action is a
// temporary in the call, built in the 0xb0-byte frame and destroyed after the call through the derived class's implicit destructor
// (member pointer, then the base; no vtable store of its own). The horn (ParentThing) is read into a named local: retail loads it into
// edi, the dead `this` register (`mov edi,[edi+4] ; push edi`).
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
#define FABLE_RETAIL_CCREATUREACTION_BLOWHORN
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_BlowHorn.h"
#include "engine/CTCActionUseBase.h"

// CTCActionUseBlowHorn (no shared header; RTTI .?AVCTCActionUseBlowHorn@@ : CTCActionUseBase, Ego_r.pdb: no own members).
class CTCActionUseBlowHorn : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);                              // slot 28 0x007EB9B0
};

void CTCActionUseBlowHorn::OnActionUse(CThing& thing)
{
    CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(&thing);
    CTCActionUseBase::OnActionUse(thing);
    if (creature != 0 && !creature->IsDead()) {
        CThing& horn = GetParentThing();
        creature->SetCurrentAction(CCreatureAction_BlowHorn(*creature, horn));
    }
}
