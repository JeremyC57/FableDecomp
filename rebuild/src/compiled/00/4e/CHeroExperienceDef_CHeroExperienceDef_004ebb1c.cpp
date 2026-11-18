// CHeroExperienceDef::~CHeroExperienceDef() -- the destructor (modelled as the compiler-generated one): the attack-combo vector and the seven maps destroyed out of line, the three long vectors freed, then the inlined def-base destructors (the CDefPointeeBase vtable store and the tail jump to ~CResource)
// Leaf: ??1CHeroExperienceDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EBB1C, 135 bytes, thiscall; size-optimised def TU (`pop ecx` after free). Its only caller is the
//   scalar deleting destructor 0x004EBB00 (vtable 0x0124390C slot 0).
// Name: Ego_r.pdb 0x0050C568 + FableWin.pdb 0x01B9C01C ??1CHeroExperienceDef@@UAE@XZ.
// Callees: ~vector<CAttackHistoryCombo> 0x004E81C1, ~_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData> ..>
//   0x004E81FB (q11179 / NF90), ~_Rb_tree<long, pair<const long, CDefString> ..> 0x004DD21C (x5), ~_Rb_tree<long, pair<const long,
//   CComboMultiplierData> ..> 0x004E81E4, free 0x00BFEA14 (x3, the inline ~vector<long>), CResource::~CResource 0x009FC550 (tail jump).
// Layout: engine/CHeroExperienceDef.h, FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER (the Ego_r.pdb STLport member types, retail 0xB4).
// Shape: the compiler-generated destructor (no vptr store of its own, family K050 as 0x00F26DF0): members in reverse order, then the
//   CDefPointeeBase vptr 0x01230BA0 and `jmp ~CResource`; the tree destructors stay calls (TREE_CTOR_DTOR_OUTOFLINE / TREE_CLEAR_OUTOFLINE,
//   as the 0x004E81E4 row).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CATTACKHISTORYCOMBO
#define FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER
#define FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/CHeroExperienceDef.h"

namespace {
// Fidelity: scaffolding, not retail: the shared header declares no destructor, so the destructor is the compiler-generated one (retail
// stores no CHeroExperienceDef vptr in it, the K050 family's implicit-destructor shape; whether the original source declared one is not
// recorded here: the PDB name sets list ??1 either way, and 0x004F9CFC gets the same shape from a declared one under novtable). This TU emits its
// out-of-line COMDAT through a qualified call (too large for the size-optimised inliner); retail's emitter is the scalar deleting destructor 0x004EBB00.
void DestroyHeroExperienceDef(CHeroExperienceDef& def)
{ def.CHeroExperienceDef::~CHeroExperienceDef(); }
}
