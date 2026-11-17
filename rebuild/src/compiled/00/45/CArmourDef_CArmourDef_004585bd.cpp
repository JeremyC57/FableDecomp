// CArmourDef::~CArmourDef() -- destroy ArmourThresholds (+0x54) and DamageTypeResponse (+0x48) through the map<EDamageAttribute,float>
// tree destructor 0x00451AC6, AugmentationResponse (+0x3C) through the map<EObjectAugmentationType,float> tree destructor 0x00451A85,
// then the CParentDefClassBase destructor 0x00430300 (a tail jump; the CArmourDef vptr store is dead before it, as the base
// destructor stores its own)
// Leaf: ??1CArmourDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004585BD, 35 bytes; size-optimised def TU (the scalar deleting destructor 0x004585A1,
//   its only caller, is the next function down).
// Identity (NF84, round 7, q11044 binder): Ego_r.pdb 0x00465A8E / FableWin.pdb 0x0194A5A8 ??1CArmourDef@@UAE@XZ; its one caller is
//   CArmourDef's scalar deleting destructor 0x004585A1 (landed, binds this address as CArmourDef_Destruct); Ego_r's CArmourDef has
//   three maps after the CParentDefClassBase part (AugmentationResponse map<EObjectAugmentationType,float>, DamageTypeResponse /
//   ArmourThresholds map<EDamageAttribute,float>), which with STLport's 12-byte maps sit at +0x3C / +0x48 / +0x54, the members this
//   body destroys. Was the generated stand-in _unattributed::~CAggregate_004585bd (local member and base models; +0x48 / +0x54 called
//   0x00451AC6 as ??1CMember_00451ac6@@QAE@XZ).
// Port (NF84): engine/CArmourDef.h with the NF84 opt-in FABLE_RETAIL_CARMOURDEF_DTOR (the declared virtual destructor and the three
//   real map members), the tree destructors kept out of line (FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARMOURDEF_DTOR
#define FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
#include "fable_string.h"
#include "engine/CArmourDef.h"

CArmourDef::~CArmourDef()
{
}
