// Leaf: ?OnAppearanceClear@CTCLook@@QAEXXZ
struct CTCLook { char pad0[0x78]; unsigned long f78; unsigned long f7c; void OnAppearanceClear(); };
void CTCLook::OnAppearanceClear(){ this->f78 = 0u; this->f7c = 0u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnAppearanceClear@CTCLook@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
