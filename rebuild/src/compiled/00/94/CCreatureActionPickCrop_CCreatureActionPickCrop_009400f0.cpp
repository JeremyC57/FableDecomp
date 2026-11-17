// CCreatureAction_PickCrop::~CCreatureAction_PickCrop() -- the implicit destructor's own COMDAT: destroy PObjectToUse (+0xA8,
// CIntelligentPointer<CThingObject>: ~CBaseIntelligentPointer 0x00A01C10), then tail-jump to ~CCreatureActionBase 0x00693EF0 (no vptr store)
// Leaf: ??1CCreatureAction_PickCrop@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009400F0, 22 bytes (push esi ; mov esi,ecx ; lea ecx,[esi+0A8h] ; call 0x00A01C10 ;
//   mov ecx,esi ; pop esi ; jmp 0x00693EF0).
// Identity (agentNF77, q10884): the owner is CCreatureAction_PickCrop. (1) Its one caller, the unlanded CAIStateGroup_VillageTaskPickCrops::Update
//   (0x00940310; call at 0x0094049E), destroys with it the action temporary it builds with the CCreatureAction_PickCrop constructor
//   0x009405F0 (`lea ecx,[esp+2Ch] ; call 0x009405F0` with two pushes before, then `lea ecx,[esp+20h] ; call 0x009400F0`: the same object).
//   (2) The body destroys exactly Ego_r.pdb's one CCreatureAction_PickCrop member, PObjectToUse (CIntelligentPointer<CThingObject>, Ego_r +0xB4 =
//   retail +0xA8), then the RTTI base (.?AVCCreatureAction_PickCrop@@ : CCreatureActionBase); the class's ??_G 0x00940680 expands the same body.
//   (3) FableWin.pdb lists ??1CCreatureAction_PickCrop@@UAE@XZ (0x02B98AA0, the PDB name of the generated destructor; Ego_r.pdb has the class's
//   constructor 0x009643F0, PerformAction and Clone but no ??1 copy). The catalog's
//   _global::Dtor_CCreatureActionBaseBase_009400f0 and the old Obj / Sub stand-ins (?DestVal_A8_G@Obj@@QAEXXZ) named no class.
// Layout: engine/CCreatureAction_PickCrop.h (overlay retail opt-in FABLE_RETAIL_CCREATUREACTION_PICKCROP: on CCreatureActionBase, PObjectToUse
//   +0xA8, sizeof 0xB0, no user-declared destructor), engine/CCreatureActionBase.h and engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER:
//   the member has no destructor of its own, so ~CBaseIntelligentPointer is called at its address).
// Shape: no vtable store, VC7.1's implicit destructor (a user-declared one stores the vptr first, GOTCHAS 73); DestroyOne (a destruction by
//   name, ungraded) makes VC7.1 emit the out-of-line implicit ??1 (as the landed ~CTCTargetingBase 0x004DA8B7).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PICKCROP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_PickCrop.h"

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CCreatureAction_PickCrop* object)
{
    object->CCreatureAction_PickCrop::~CCreatureAction_PickCrop();
}
