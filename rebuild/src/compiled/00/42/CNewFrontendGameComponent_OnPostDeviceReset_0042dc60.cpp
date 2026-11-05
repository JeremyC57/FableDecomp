// Constant-true predicate: `mov al,1; ret`. __fastcall this=ecx, no args.
// Leaf: ?OnPostDeviceReset@CNewFrontendGameComponent@@QAE_NXZ
struct CNewFrontendGameComponent { bool OnPostDeviceReset(); };
bool CNewFrontendGameComponent::OnPostDeviceReset() { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnPostDeviceReset@CNewFrontendGameComponent@@UAE_NXZ); evidence: RTTI vtable slot + PDB name set.
