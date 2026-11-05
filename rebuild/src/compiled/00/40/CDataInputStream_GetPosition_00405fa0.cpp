// Leaf: ?GetPosition@CDataInputStream_Methods@@QAEKXZ
#include "engine/CDataInputStream.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDataInputStream_Methods : CDataInputStream {
    unsigned long GetPosition();
};
unsigned long CDataInputStream_Methods::GetPosition(){ return this->StreamPos; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetPosition@CDataInputStream@@UBEKXZ); evidence: RTTI vtable slot + PDB name set.
