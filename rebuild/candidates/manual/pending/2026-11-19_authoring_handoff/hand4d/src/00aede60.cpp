// NParticleEngine::CParticleEmitter::SetOrientation(const C3DOrientationInterpolator& orient_interp) -- take a new start / end orientation: copy the interpolator, make both right-handed sets perpendicular, rebuild Transform from the start orientation (keeping its translation), InverseTransform as its rotation-only inverse, and EndTransform from the end orientation (keeping its translation)
// Grade: RELOCATION_MATCH (hand4d 2026-11-19, no flags). The parked local model (hand-written C3DVector / CRightHandedSet /
//   interpolator stand-ins, "mirrored fmul operand order") is replaced by the shared headers: FableWin.pdb's statement order
//   (?SetOrientation@CParticleEmitter@NParticleEngine@@QAEXABVC3DOrientationInterpolator@@@Z, parameter orient_interp) with
//   Set(const CRightHandedSet&, const C3DVector&) through the existing opt-in FABLE_RETAIL_CMATRIX3X4_SET_RHSET_INLINE (the
//   CTCShop row 0x00772BB0's force-inline Set) and SetInverseNoTranslate in its FableWin 0x02D840E0 text (det / rec locals).
// Inline-budget: retail expands Set(CRightHandedSet, translation), the four-vector Set, GetTranslation, GetDeterminant and
//   SetInverseNoTranslate, but calls the compiler-generated C3DOrientationInterpolator::operator= 0x006B0E00 out of line. No
//   /d2inlT threshold gives that mix (<=221: inverse and Set out of line, 157-433 v 796 bytes; >=222: operator= expanded too, 867 v
//   796), so Set and the inverse are FABLE_FORCEINLINE (FABLE_MATRIX_INLINE_PASS23 at its default, PLAIN_INLINE_PASS23 not opted
//   in; overlay opt-in FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_FORCEINLINE for GetDeterminant / SetInverseNoTranslate) and the
//   operator= stays at the default budget.
// Overlay: engine/CMatrix3x4.h FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_FORCEINLINE (new).
// Leaf: ?SetOrientation@CParticleEmitter@NParticleEngine@@QAEXABVC3DOrientationInterpolator@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEDE60, 796 bytes, thiscall `ret 4`.
// Callees: C3DOrientationInterpolator::operator= 0x006B0E00 (landed), CRightHandedSet::MakePerpendicular 0x00A22E10 (x2);
//   1.0f 0x0122DED8.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_CREATE2D
#define FABLE_RETAIL_CPARTICLEEMITTER_STARTFRAME_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SET_RHSET_INLINE
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_FORCEINLINE
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#include "engine/CParticleEmitter.h"
#include "engine/C3DOrientationInterpolator.h"

void NParticleEngine::CParticleEmitter::SetOrientation(const C3DOrientationInterpolator& orient_interp)
{
    OrientationInterpolator = orient_interp;
    OrientationInterpolator.StartOrientation.MakePerpendicular();
    OrientationInterpolator.EndOrientation.MakePerpendicular();
    Transform.Set(OrientationInterpolator.StartOrientation, Transform.GetTranslation());
    InverseTransform.SetInverseNoTranslate(Transform);
    EndTransform.Set(OrientationInterpolator.EndOrientation, EndTransform.GetTranslation());
}
