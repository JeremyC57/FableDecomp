// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?Is2DAreaInViewingFrustrum@CAIGameCameraBase@@UBE_NABVC3DVector@@M@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CAIGameCameraBase.h under FABLE_RETAIL_CAIGAMECAMERABASE with the PDB parameter / return types (was ?Is2DAreaInViewingFrustrum@CAIGameCameraBase_Methods@@QAE_NHH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CAIGAMECAMERABASE
#include "engine/CAIGameCameraBase.h"
bool CAIGameCameraBase::Is2DAreaInViewingFrustrum(const C3DVector& pos, float a1) const { return false; }
