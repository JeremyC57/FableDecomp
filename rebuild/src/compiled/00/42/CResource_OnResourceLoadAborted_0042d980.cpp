// Empty member hook. __fastcall this=ecx.
// Leaf: ?OnResourceLoadAborted@CResource_Methods@@QAEXXZ
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResource_Methods : CResource {
    void OnResourceLoadAborted();
};
void CResource_Methods::OnResourceLoadAborted() {}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnResourceLoadAborted@CResource@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
