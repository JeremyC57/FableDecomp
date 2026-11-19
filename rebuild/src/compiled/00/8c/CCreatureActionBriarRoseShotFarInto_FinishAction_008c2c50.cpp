// CCreatureAction_BriarRoseShotFarInto::FinishAction() -- base FinishAction, then hand the creature a temporary CCreatureAction_BriarRoseShotFarOutof(creature, target) as its current action
// Leaf: ?FinishAction@CCreatureAction_BriarRoseShotFarInto@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C2C50, length 91 bytes. Vtable slot 4 (FinishAction) of
// CCreatureAction_BriarRoseShotFarInto (0x01294A2C).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8 into edi, PTarget at +0xa8 into ebx: both named
// locals fetched BEFORE the base call), CCombatActionBase::FinishAction @ 0x00859A20 (base virtual, non-virtual call),
// CCreatureAction_BriarRoseShotFarOutof::CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase*, CThingCreatureBase*)
// @ 0x008C39A0 (the 0x11c-byte temporary at the bottom of the frame), CThingCreatureBase::SetCurrentAction(const
// CCreatureActionBase&) @ 0x006644F0 (`push eax` of the constructor's returned this), then the temporary's inlined
// destructor: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 on its PTarget member (+0x114) and
// CCombatActionBase::~CCombatActionBase @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: the implicit destructor of the temporary is inlined as member destructor + base destructor with NO vtable
// stores: both the temporary's class and its CIntelligentPointer member are modelled `__declspec(novtable)` (their own
// constructors are out of line, so only the destructor's dead vtable stores are affected).
// Port (NF93, q11261 / q11262): the local CBaseIntelligentPointer / novtable CIntelligentPointer / CCombatActionBase / CThingCreatureBase_Calls /
//   CCreatureAction_BriarRoseShotFarOutof stand-ins replaced by the shared headers (the 0x008C2980 recipe) with the NF93 opt-ins
//   CCREATUREACTION_BRIARROSESHOTFARINTO_CLASS / CCREATUREACTION_BRIARROSESHOTFAROUTOF_CLASS: SetCurrentAction 0x006644F0 and ~CCombatActionBase
//   0x0062E900 now bind the PDB spellings. The Ego_r ctor takes (CThingCreatureBase&, CThingCreatureBase&): the target is PTarget's item as a creature.
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
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFARINTO_CLASS
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFAROUTOF_CLASS
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
#include "engine/CCreatureAction_BriarRoseShotFarInto.h"
#include "engine/CCreatureAction_BriarRoseShotFarOutof.h"

void CCreatureAction_BriarRoseShotFarInto::FinishAction()
{
    CThingCreatureBase* pcreature = PCreature.Get();
    CThingCreatureBase* ptarget = static_cast<CThingCreatureBase*>(PTarget.Get());
    CCombatActionBase::FinishAction();
    pcreature->SetCurrentAction(CCreatureAction_BriarRoseShotFarOutof(*pcreature, *ptarget));
}
