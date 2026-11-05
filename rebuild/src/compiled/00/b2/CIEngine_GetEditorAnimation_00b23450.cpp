// Leaf: ?GetEditorAnimation@CIEngine_Methods@@QAEHH@Z
#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    int GetEditorAnimation(int text);
};
int CIEngine_Methods::GetEditorAnimation(int text) { return 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetEditorAnimation@CIEngine@@UBEPBVC3DAnimationInfo@@ABVCWideString@@@Z); evidence: RTTI vtable slot + PDB name set.
