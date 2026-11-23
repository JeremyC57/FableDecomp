// Empty virtual hook (CGameComponent::Init and friends): __fastcall this=ecx, no args,
// Leaf: ?OnResourceLoaded@CResource@@UAEXXZ
// no cleanup -> a bare `ret`.
#define FABLE_RETAIL_DEF_CLASS_BASE   // alias route: engine/CResource.h declares OnResourceLoaded under this class-model switch
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
void CResource::OnResourceLoaded() {}
// Was defined on a stand-in alias class (owner rule 2026-11-05); now the real member, through the class-model switch above (alias route, 2026-11-23).
