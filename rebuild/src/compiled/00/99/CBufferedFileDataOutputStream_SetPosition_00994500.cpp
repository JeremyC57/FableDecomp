// Leaf: ?SetPosition@CBufferedFileDataOutputStream_Methods@@QAEXK@Z
#include "engine/CBufferedFileDataOutputStream.h"  // retyped onto the PDB layout; byte parity re-verified
struct CBufferedFileDataOutputStream_Methods : CBufferedFileDataOutputStream {
    void SetPosition(unsigned long v);
};
void CBufferedFileDataOutputStream_Methods::SetPosition(unsigned long v){ this->FilePos = v; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?SetPosition@CBufferedFileDataOutputStream@@UAEXK@Z); evidence: RTTI vtable slot + PDB name set.
