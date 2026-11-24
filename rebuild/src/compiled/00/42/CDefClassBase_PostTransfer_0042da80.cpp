// Empty virtual hook (CGameComponent::Init and friends): __fastcall this=ecx, no args,
// Leaf: ?PostTransfer@CDefClassBase@@UAEXXZ
// no cleanup -> a bare `ret`.
#define FABLE_RETAIL_DEF_CLASS_BASE   // alias route: engine/CDefClassBase.h declares PostTransfer under this class-model switch
#include "engine/CDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
void CDefClassBase::PostTransfer() {}
// Was defined on a stand-in alias class (owner rule 2026-11-05); now the real member, through the class-model switch above (alias route, 2026-11-23).
