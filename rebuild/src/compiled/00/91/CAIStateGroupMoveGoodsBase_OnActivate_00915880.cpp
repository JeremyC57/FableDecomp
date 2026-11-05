// Leaf: ?OnActivate@CAIStateGroup_MoveGoodsBase_Methods@@QAEXXZ
#include "engine/CAIStateGroup_MoveGoodsBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MoveGoodsBase_Methods : CAIStateGroup_MoveGoodsBase {
    void OnActivate();
};
void CAIStateGroup_MoveGoodsBase_Methods::OnActivate(){ this->DroppingGoods = 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnActivate@CAIStateGroup_MoveGoodsBase@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
