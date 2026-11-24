// Constant-false predicate: `xor al,al; ret`. __fastcall this=ecx, no args.
// Leaf: ?UnloadResource@CResource@@UAE_NXZ
#define FABLE_RETAIL_DEF_CLASS_BASE   // alias route: engine/CResource.h declares UnloadResource under this class-model switch
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
bool CResource::UnloadResource() { return false; }
// Was defined on a stand-in alias class (owner rule 2026-11-05); now the real member, through the class-model switch above (alias route, 2026-11-23).
