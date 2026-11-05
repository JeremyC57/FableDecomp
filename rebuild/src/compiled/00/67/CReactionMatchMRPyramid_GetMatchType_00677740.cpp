// Constant-return member: returns 0x3. __fastcall this=ecx.
// Leaf: ?GetMatchType@CReactionMatch_MRPyramid_Methods@@QAEHXZ
#include "engine/CReactionMatch_MRPyramid.h"  // retyped onto the PDB layout; byte parity re-verified
struct CReactionMatch_MRPyramid_Methods : CReactionMatch_MRPyramid {
    int GetMatchType();
};
int CReactionMatch_MRPyramid_Methods::GetMatchType() { return 0x3; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetMatchType@CReactionMatch_MRPyramid@@UBE?AW4EReactionMatchType@@XZ); evidence: RTTI vtable slot + PDB name set.
