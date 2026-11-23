// Constant-false predicate: `xor al,al; ret`. __fastcall this=ecx, no args.
// Leaf: ?IsResourceLoaded@CResource@@UBE_NXZ
#define FABLE_RETAIL_DEF_CLASS_BASE   // alias route: engine/CResource.h declares IsResourceLoaded under this class-model switch
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
bool CResource::IsResourceLoaded() const { return false; }
// Was defined on a stand-in alias class (owner rule 2026-11-05); now the real member, through the class-model switch above (alias route, 2026-11-23).
