// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsSwitchableOn@CANavQuadTreeNode_Methods@@QAE_NHH@Z
#include "engine/CANavQuadTreeNode.h"  // retyped onto the PDB layout; byte parity re-verified
struct CANavQuadTreeNode_Methods : CANavQuadTreeNode {
    bool IsSwitchableOn(int a0, int a1);
};
bool CANavQuadTreeNode_Methods::IsSwitchableOn(int a0, int a1) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsSwitchableOn@CANavQuadTreeNode@@UBE_N_K@Z); evidence: RTTI vtable slot + PDB name set.
