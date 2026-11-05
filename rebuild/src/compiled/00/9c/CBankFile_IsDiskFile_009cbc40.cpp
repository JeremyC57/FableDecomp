// Leaf: ?IsDiskFile@CBankFile_Methods@@QAE_NHH@Z
#include "engine/CBankFile.h"  // retyped onto the PDB layout; byte parity re-verified
struct CBankFile_Methods : CBankFile {
    bool IsDiskFile(int x0, int x1);
};
bool CBankFile_Methods::IsDiskFile(int x0, int x1) { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsDiskFile@CBankFile@@MAE_NKK@Z); evidence: RTTI vtable slot + PDB name set.
