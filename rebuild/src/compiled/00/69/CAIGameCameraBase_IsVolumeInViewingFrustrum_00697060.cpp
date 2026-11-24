// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsVolumeInViewingFrustrum@CAIGameCameraBase@@UBE_NABVC3DVector@@M@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CAIGameCameraBase.h under FABLE_RETAIL_CAIGAMECAMERABASE with the PDB parameter / return types (was ?IsVolumeInViewingFrustrum@CAIGameCameraBase_Methods@@QAE_NHH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CAIGAMECAMERABASE
#include "engine/CAIGameCameraBase.h"
bool CAIGameCameraBase::IsVolumeInViewingFrustrum(const C3DVector& pos, float a1) const { return false; }
