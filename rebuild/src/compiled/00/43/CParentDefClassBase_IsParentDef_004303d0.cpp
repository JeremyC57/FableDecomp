// Constant-true predicate: `mov al,1; ret`. __fastcall this=ecx, no args.
// Leaf: ?IsParentDef@CParentDefClassBase@@UBE_NXZ
#define FABLE_RETAIL_DEF_CLASS_BASE   // alias route: engine/CParentDefClassBase.h declares IsParentDef under this class-model switch
#include "engine/CParentDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
bool CParentDefClassBase::IsParentDef() const { return true; }
// Was defined on a stand-in alias class (owner rule 2026-11-05); now the real member, through the class-model switch above (alias route, 2026-11-23).
