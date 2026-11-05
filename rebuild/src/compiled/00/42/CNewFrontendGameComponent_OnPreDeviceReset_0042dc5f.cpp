// Empty virtual hook (CGameComponent::Init and friends): __fastcall this=ecx, no args,
// Leaf: ?OnPreDeviceReset@CNewFrontendGameComponent@@QAEXXZ
// no cleanup -> a bare `ret`.
struct CNewFrontendGameComponent { void OnPreDeviceReset(); };
void CNewFrontendGameComponent::OnPreDeviceReset() {}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnPreDeviceReset@CNewFrontendGameComponent@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
