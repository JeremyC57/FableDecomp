// Leaf: ?SetPosition@CWADFile_Methods@@QAEXK@Z
#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWADFile_Methods : CWADFile {
    void SetPosition(unsigned long v);
};
void CWADFile_Methods::SetPosition(unsigned long v){ this->FilePos = v; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?SetPosition@CWADFile@@UAEXJ@Z); evidence: RTTI vtable slot + PDB name set.
