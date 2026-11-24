// Empty member hook. __fastcall this=ecx.
// Leaf: ?OnResourceLoadAborted@CResource@@UAEXXZ
#define FABLE_RETAIL_DEF_CLASS_BASE   // alias route: engine/CResource.h declares OnResourceLoadAborted under this class-model switch
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
void CResource::OnResourceLoadAborted() {}
// Was defined on a stand-in alias class (owner rule 2026-11-05); now the real member, through the class-model switch above (alias route, 2026-11-23).
