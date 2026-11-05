// Leaf: ?IsTimingCritical@CAIStateGroup_CreatureUseCoverBase_Methods@@QAEEXZ
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_CreatureUseCoverBase_Methods : CAIStateGroup_CreatureUseCoverBase {
    unsigned char IsTimingCritical();
};
unsigned char CAIStateGroup_CreatureUseCoverBase_Methods::IsTimingCritical(){ return this->TimingCritical; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsTimingCritical@CAIStateGroup_CreatureUseCoverBase@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
