#ifndef FABLE_ENGINE_C3DKEYFRAME_H
#define FABLE_ENGINE_C3DKEYFRAME_H
#include "engine/CQuaternion.h"
#ifdef FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY
#ifndef FABLE_RETAIL_CQUATERNION_MULASSIGN
#define FABLE_RETAIL_CQUATERNION_MULASSIGN
#endif
#ifndef FABLE_RETAIL_C4DVECTOR_ADDASSIGN
#define FABLE_RETAIL_C4DVECTOR_ADDASSIGN
#endif
#endif
#include "engine/C4DVector.h"
class CMatrix4x4;
// Both original PDBs: complete48-byte value; Rotation0,Position16,Scaling32.
class C3DKeyframe {
public:
 CQuaternion Rotation;
 C4DVector Position;
 C4DVector Scaling;
#ifdef FABLE_RETAIL_C3DKEYFRAME_CTOR
 // Retail opt-in (chunk 2005): FableWin.pdb ??0C3DKeyframe@@QAE@XZ (user default constructor; no stores in retail).
 C3DKeyframe() { }
#endif
 C3DVector GetPosition()const { return Position.ToC3DVector(); }
 void ToMatrix(CMatrix4x4& matrix,bool applyScaling)const;
#ifdef FABLE_RETAIL_C3DKEYFRAME_TOMATRIX_INLINE
 // Retail opt-in (chunk 2014, row 0x00A4E300): Ego_r.pdb / FableWin.pdb ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix3x4@@_N@Z is an inline
 // header member: FableWin emits its COMDAT (0x03228BF0, 0x70 bytes: the quaternion's ToMatrix with the position, then PreScale(MMM))
 // right after GetDeltaMovement, and retail keeps the out-of-line COMDAT copy 0x00A52450 inside this TU while GetDeltaMovement
 // expands both of its calls (no call in 0x00A4E300).
 FABLE_FORCEINLINE void ToMatrix(CMatrix3x4& matrix,bool applyScaling)const {
  Rotation.ToMatrix(matrix,GetPosition());
  if (applyScaling) matrix.PreScale(Scaling.X,Scaling.Y,Scaling.Z);
 }
#else
 void ToMatrix(CMatrix3x4& matrix,bool applyScaling)const;
#endif
 void ToMatrixTransposed(CMatrix4x4& matrix,bool applyScaling)const;
#ifdef FABLE_RETAIL_C3DKEYFRAME_BLEND
 // Retail opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540): FableWin.pdb ?PostApplyRelativeAnimationAlpha@C3DKeyframe@@QAEXABV1@M@Z and
 // ?BlendWith@C3DKeyframe@@QAEXABV1@M@Z (retail out-of-line copy 0x00AA3C50, landed, uncalled), both expanded in the rows. Declarations
 // only: their bodies need the engine's GFFastArcCos / GFFastSin and the asm helper engine/GFFloatToLongNear.h, so the includer defines
 // them FABLE_FORCEINLINE (VC7.1 never auto-inlines a body holding inline asm).
 void PostApplyRelativeAnimationAlpha(const C3DKeyframe& other, float alpha);
 void BlendWith(const C3DKeyframe& other, float t);
#endif
#ifdef FABLE_RETAIL_C3DKEYFRAME_GETROTATION
 // Retail opt-in (chunk 2044): FableWin.pdb ?GetRotation@C3DKeyframe@@QBE?AVCQuaternion@@XZ (inline, by value).
 CQuaternion GetRotation() const { return Rotation; }
#endif
#ifdef FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY
 // Retail opt-in (chunk 2005, row 0x00A4D920): FableWin.pdb ?PostApplyRelativeAnimation@C3DKeyframe@@QAEXABV1@@Z (expanded in
 // retail: the out-of-line CQuaternion::operator*= 0x00A88C10, then C4DVector::operator+= ??YC4DVector@@QAEABV0@ABV0@@Z inline).
 void PostApplyRelativeAnimation(const C3DKeyframe& other) { Rotation *= other.Rotation; Position += other.Position; }
#endif
};
#endif
