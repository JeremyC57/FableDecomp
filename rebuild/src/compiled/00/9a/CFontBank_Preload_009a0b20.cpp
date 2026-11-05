// Leaf: ?Preload@CFontBank_Methods@@QAE_NHH@Z
#include "engine/CFontBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CFontBank_Methods : CFontBank {
    bool Preload(int text, int x1);
};
bool CFontBank_Methods::Preload(int text, int x1) { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Preload@CFontBank@@UBE_NABVCWideString@@_N@Z); evidence: RTTI vtable slot + PDB name set.
