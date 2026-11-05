// Leaf: ?IsInMovieSequence@CGameScriptInterface_Methods@@QAEEXZ
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CGameScriptInterface_Methods : CGameScriptInterface {
    unsigned char IsInMovieSequence();
};
unsigned char CGameScriptInterface_Methods::IsInMovieSequence(){ return this->InMovieSequence; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsInMovieSequence@CGameScriptInterface@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
