// Leaf: ?IsToFaceTargetAtStart@CCreatureAction_RiseUp@@QAEEXZ
struct CCreatureAction_RiseUp { char pad[0x114]; unsigned char f; unsigned char IsToFaceTargetAtStart(); };
unsigned char CCreatureAction_RiseUp::IsToFaceTargetAtStart(){ return this->f; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsToFaceTargetAtStart@CCreatureAction_RiseUp@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
