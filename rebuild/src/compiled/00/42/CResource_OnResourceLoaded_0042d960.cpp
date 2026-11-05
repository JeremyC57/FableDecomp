// Empty virtual hook (CGameComponent::Init and friends): __fastcall this=ecx, no args,
// Leaf: ?OnResourceLoaded@CResource_Methods@@QAEXXZ
// no cleanup -> a bare `ret`.
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResource_Methods : CResource {
    void OnResourceLoaded();
};
void CResource_Methods::OnResourceLoaded() {}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnResourceLoaded@CResource@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
