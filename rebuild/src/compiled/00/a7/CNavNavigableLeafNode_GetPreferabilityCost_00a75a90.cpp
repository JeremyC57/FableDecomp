// Leaf: ?GetPreferabilityCost@CNavNavigableLeafNode_Methods@@QAEEXZ
#include "engine/CNavNavigableLeafNode.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavNavigableLeafNode_Methods : CNavNavigableLeafNode {
    unsigned char GetPreferabilityCost();
};
unsigned char CNavNavigableLeafNode_Methods::GetPreferabilityCost(){ return this->PreferabilityCost; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetPreferabilityCost@CNavNavigableLeafNode@@UBEEXZ); evidence: RTTI vtable slot + PDB name set.
