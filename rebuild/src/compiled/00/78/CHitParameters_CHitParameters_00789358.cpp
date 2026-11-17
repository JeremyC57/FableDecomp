// CHitParameters::~CHitParameters() -- destroy PWeapon (+0x58) and PThingHitting (+0x50) (CBaseIntelligentPointer dtor 0x00A01C10),
// release PHitMaterialDef (+0x4C, CDefPointer), free Augmentations' buffer (+0x3C, STLport vector), then DamageMap (+0x30) through the
// map<EDamageAttribute,float> tree destructor 0x00451AC6 (a tail jump)
// Leaf: ??1CHitParameters@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789358, 63 bytes; size-optimised module (`and dword ptr [esi+4Ch],0`, `pop ecx` after free).
// Identity (NF84, round 7, q11044 binder): Ego_r.pdb 0x00790457 / FableWin.pdb ??1CHitParameters@@QAE@XZ; Ego_r's CHitParameters
//   (DamageMap +0x30 map<EDamageAttribute,float>, Augmentations vector<EObjectAugmentationType>, PHitLocation, PHitMaterialDef
//   CDefPointer<const CMaterialDef>, PThingHitting / PWeapon CIntelligentPointer<CThing>) with STLport's 12-byte containers gives
//   exactly the members this body destroys (+0x30 / +0x3C / +0x4C / +0x50 / +0x58, sizeof 0x60); engine/CHitParameters.h's _CTOR
//   block already declares this destructor at 0x00789358, and the landed callers (0x007E2600, 0x007C4430, 0x007CCE20, ...) bind
//   ??1CHitParameters@@QAE@XZ here. Was the stand-in CHitParameters_00789358::~CHitParameters_00789358 (local member models; the
//   +0x30 member called 0x00451AC6 as ??1CMember12_00451ac6@@QAE@XZ).
// Port (NF84): the shared header with the real member types (FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS / _DAMAGEMAP / _AUGMENTATIONS)
//   and the declared destructor (_CTOR); the tree destructor kept out of line (FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE), so the
//   +0x30 member binds the STLport ~_Rb_tree<EDamageAttribute, ...> 0x00451AC6 (NF83).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
#include "fable_compiler.h"
#include <stddef.h>
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CMaterialDef.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CHitParameters.h"

CHitParameters::~CHitParameters()
{
}
