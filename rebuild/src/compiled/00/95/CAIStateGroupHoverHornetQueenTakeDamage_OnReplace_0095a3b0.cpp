// Leaf: ?OnReplace@CAIStateGroup_HoverHornetQueenTakeDamage_Methods@@QAEXXZ
#include "engine/CAIStateGroup_HoverHornetQueenTakeDamage.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_HoverHornetQueenTakeDamage_Methods : CAIStateGroup_HoverHornetQueenTakeDamage {
    void OnReplace();
};
void CAIStateGroup_HoverHornetQueenTakeDamage_Methods::OnReplace(){ this->RangeAttackCount = 0u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnReplace@CAIStateGroup_HoverHornetQueenTakeDamage@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
