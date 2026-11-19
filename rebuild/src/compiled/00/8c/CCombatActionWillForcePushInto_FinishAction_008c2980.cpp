// CCombatAction_WillForcePushInto::FinishAction() -- base FinishAction, then hand the creature a temporary CCreatureAction_CastWillForceSpell(creature, 1, true, "ST_FORCE_PUSH_RELEASE") as its current action
// Leaf: ?FinishAction@CCombatAction_WillForcePushInto@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C2980, 112-byte slot (97 bytes of code). FinishAction is vtable slot 4.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, fetched into edi BEFORE the base call:
//          a named local), CCombatActionBase::FinishAction @ 0x00859A20 (direct base call),
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0,
//          CCreatureAction_CastWillForceSpell::CCreatureAction_CastWillForceSpell(CThingCreatureBase*, long, bool, const CCharString&)
//          @ 0x008BB200, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0,
//          the action temporary's destructor @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated),
//          CCharString::~CCharString @ 0x0099EAE0.
// Shape: the landed WillEnflameInto::FinishAction (0x008C2A80) with a named animation string: the 0x120-byte frame
// holds the string word below the 0x11c-byte action temporary; the temporary dies first, then the string.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
// Port (NF93, q11261 / q11262): the local CThingCreatureBase_Calls / CCreatureAction_CastWillForceSpell_Retail / CCombatActionBase /
//   CCharString stand-ins replaced by the shared headers (the WillHealInto 0x008C2AD0 recipe) with the NF93 opt-ins
//   CCREATUREACTION_CASTWILLFORCESPELL_CLASS and CCOMBATACTION_WILLFORCEPUSHINTO_FINISHACTION: SetCurrentAction 0x006644F0 and the
//   temporary's destructor 0x0062E900 (~CCombatActionBase) now bind the PDB spellings.
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
#define FABLE_RETAIL_CCOMBATACTION_WILLFORCEPUSHINTO_FINISHACTION
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLFORCESPELL_CLASS
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
#include "engine/CCombatAction_WillForcePushInto.h"
#include "engine/CCreatureAction_CastWillForceSpell.h"

void CCombatAction_WillForcePushInto::FinishAction()
{
    CThingCreatureBase* pcreature = PCreature.Get();
    CCombatActionBase::FinishAction();
    CCharString anim_name("ST_FORCE_PUSH_RELEASE", -1);
    pcreature->SetCurrentAction(CCreatureAction_CastWillForceSpell(*pcreature, 1, true, anim_name));
}
