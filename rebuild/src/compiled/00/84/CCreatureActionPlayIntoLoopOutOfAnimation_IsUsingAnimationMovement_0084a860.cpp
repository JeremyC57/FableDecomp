// Leaf: ?IsUsingAnimationMovement@CCreatureAction_PlayIntoLoopOutOfAnimation@@QAEEXZ
struct CCreatureAction_PlayIntoLoopOutOfAnimation { char pad[0xb8]; unsigned char f; unsigned char IsUsingAnimationMovement(); };
unsigned char CCreatureAction_PlayIntoLoopOutOfAnimation::IsUsingAnimationMovement(){ return this->f; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsUsingAnimationMovement@CCreatureAction_PlayIntoLoopOutOfAnimation@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
