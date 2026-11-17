// CTCTargetingAI::~CTCTargetingAI() -- the implicit destructor: ThingsToNotAttack (+0x48) through ~list<CIntelligentPointer<CThing> > 0x004B8940, PScriptTargetOverride (+0x40) and PBestEnemyCreatureForAttack (+0x38) through ~CBaseIntelligentPointer 0x00A01C10, then ~CTCTargetingBase 0x004DA8B7 (tail jump)
// Leaf: ??1CTCTargetingAI@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0080, length 35 bytes; thiscall, size-optimised TU.
// Identity (agentNF69, found while porting q10855's binders): its only caller is the CTCTargetingAI ??_G 0x004E0064 (slot 0 of the
//   CTCTargetingAI vtable; RTTI CTCTargetingAI : CTCTargetingBase), and the members it destroys are Ego_r.pdb's CTCTargetingAI members
//   (struct_layouts_egor.tsv: CIntelligentPointer<CThingCreatureBase> PBestEnemyCreatureForAttack 56, PScriptTargetOverride 64,
//   list<CIntelligentPointer<CThing> > ThingsToNotAttack 72) at the same offsets; Ego_r.pdb / FableWin.pdb ??1CTCTargetingAI@@UAE@XZ.
//   Was the generated _unattributed::~CAggregate_004e0080 stand-in (CMember_004da8b7 / CMember_004b8940 members).
// Layout: engine/CTCTargetingAI.h (FABLE_RETAIL_CTCTARGETINGAI_ATTACK) on engine/CTCTargetingBase.h (FABLE_RETAIL_UPDATE_NEAREST_INTERESTING).
// Shape: no vtable store, the implicit destructor (hdr/ overlay opt-ins FABLE_RETAIL_CTCTARGETINGAI_IMPLICIT_DTOR and
//   FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR leave out the PDB-generated user declarations); ~list out of line
//   (FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE, the landed COMDAT 0x004B8940); DestroyOne (a destruction by name, ungraded) makes VC7.1 emit
//   the out-of-line implicit ??1.
#pragma optimize("s", on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CTCTARGETINGAI_IMPLICIT_DTOR
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#include "engine/CThing.h"
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
#include "engine/CTCTargetingAI.h"

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCTargetingAI* object)
{
    object->CTCTargetingAI::~CTCTargetingAI();
}
