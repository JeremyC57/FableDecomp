// CCombatAction_WillEnflameInto::FinishAction() -- base FinishAction, then hand the creature a temporary CCreatureAction_CastWillEnflameSpell(creature, Level) as its current action
// Leaf: ?FinishAction@CCombatAction_WillEnflameInto@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C2A80, length 70 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, fetched into edi BEFORE the base
// call: a named local), CCombatActionBase::FinishAction @ 0x00859A20 (base virtual, non-virtual call),
// CCreatureAction_CastWillEnflameSpell::CCreatureAction_CastWillEnflameSpell(CThingCreatureBase*, long)
// @ 0x008BB450 (the 0x11c-byte temporary at the bottom of the frame), CThingCreatureBase::SetCurrentAction
// (const CCreatureActionBase&) @ 0x006644F0 (the temporary passed by reference: `push eax` of the constructor's
// returned this), its destructor @ 0x0062E900 (manifest "~CTCLightningOrb" is a propagated label).
// Level is the long at +0x114 (Ego_r.pdb name; the engine header has it at +0x12c for the PDB build, retail's
// CCombatActionBase is 0x114 bytes, so the layout is modelled locally).
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
// Port (NF93, q11261 / q11262): the local CThingCreatureBase_Calls / CCreatureAction_CastWillEnflameSpell_Retail / CCombatActionBase
//   stand-ins replaced by the shared headers (the 0x008C2980 / WillHealInto 0x008C2AD0 recipe) with the WillEnflameInto _MAZE block and
//   the NF93 opt-ins CCOMBATACTION_WILLENFLAMEINTO_FINISHACTION / CCREATUREACTION_CASTWILLENFLAMESPELL_CLASS: SetCurrentAction 0x006644F0
//   and the temporary's destructor 0x0062E900 (~CCombatActionBase) now bind the PDB spellings; own leaf QAE -> UAE (slot-4 override).
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
#define FABLE_RETAIL_CCOMBATACTION_WILLENFLAMEINTO_MAZE
#define FABLE_RETAIL_CCOMBATACTION_WILLENFLAMEINTO_FINISHACTION
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLENFLAMESPELL_CLASS
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
#include "engine/CCombatAction_WillEnflameInto.h"
#include "engine/CCreatureAction_CastWillEnflameSpell.h"

void CCombatAction_WillEnflameInto::FinishAction()
{
    CThingCreatureBase* pcreature = PCreature.Get();
    CCombatActionBase::FinishAction();
    pcreature->SetCurrentAction(CCreatureAction_CastWillEnflameSpell(*pcreature, Level));
}
