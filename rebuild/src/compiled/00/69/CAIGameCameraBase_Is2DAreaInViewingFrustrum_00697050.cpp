// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?Is2DAreaInViewingFrustrum@CAIGameCameraBase_Methods@@QAE_NHH@Z
#include "engine/CAIGameCameraBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIGameCameraBase_Methods : CAIGameCameraBase {
    bool Is2DAreaInViewingFrustrum(int pos, int a1);
};
bool CAIGameCameraBase_Methods::Is2DAreaInViewingFrustrum(int pos, int a1) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Is2DAreaInViewingFrustrum@CAIGameCameraBase@@UBE_NABVC3DVector@@M@Z); evidence: RTTI vtable slot + PDB name set.
