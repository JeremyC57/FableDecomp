#ifndef FABLE_ENGINE_CQUATERNION_H
#define FABLE_ENGINE_CQUATERNION_H
#include "engine/CMatrix3x4.h"
#ifdef FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#include "engine/CRightHandedSet.h"
#endif
inline float FABLE_FASTCALL GFSqr(float value){return value*value;}
#ifdef FABLE_RETAIL_CQUATERNION_MATRIX4
#include "engine/CMatrix4x4.h"
#endif
#ifdef FABLE_RETAIL_CQUATERNION_MATRIX4_TRANSPOSED
#include "engine/CMatrix4x4.h"
#endif
class CQuaternion {
public:
 float X,Y,Z,W;
 CQuaternion(){}
#ifdef FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
 // Both original PDB signatures; native A88C10/A88DE0, donor Update call sites.
 void ToRightHandedSet(CRightHandedSet& dest)const {
  float d=GFSqr(X)+GFSqr(Y)+GFSqr(Z)+GFSqr(W);
  float s=(d!=0.0f)?2.0/d:1.0;
  float xs=X*s,ys=Y*s,zs=Z*s;
  float wx=W*xs,wy=W*ys,wz=W*zs;
  float xx=X*xs,xy=X*ys,xz=X*zs;
  float yy=Y*ys,yz=Y*zs,zz=Z*zs;
  C3DVector forward,up;
  forward.X=xy+wz;forward.Y=1.0f-(xx+zz);forward.Z=yz-wx;
  up.X=xz-wy;up.Y=yz+wx;up.Z=1.0f-(xx+yy);
  dest.Init(up,forward);
 }
 const CQuaternion& operator*=(const CQuaternion&);
 void SetFromRotationMatrix(const CMatrix3x4&);
 void SetIdentity(){X=0.0f;Y=0.0f;Z=0.0f;W=1.0f;}
#endif
 void SetFromAxisAngle(const C3DVector& axis,float angle);
#ifdef FABLE_RETAIL_CQUATERNION_INLINE_TOMATRIX
 // Native00B0EAB8 begins this expanded math body after axis-angle call00B0EAB3.
 // Natural,T200/T400 and scoped
 // depth255 leave it called; T400 additionally expands the wrong list site.
#endif
#ifdef FABLE_RETAIL_CQUATERNION_SLERP
 // Retail opt-in (chunk 2001, row 0x00A9EB90 C3DAnimationManager::BlendAnimations): FableWin.pdb ?Dot@CQuaternion@@QBEMABV1@@Z,
 // ?SetSlerp@CQuaternion@@QAEXABV1@0M@Z and the static ?CalculateSlerpWeights@CQuaternion@@SIXAAV1@ABV1@1M@Z (out-of-line copy
 // 0x00AA3390, landed), all expanded in retail. Declarations only: the includer defines the bodies (they need the engine's
 // GFFastArcCos / GFFastSin table helpers and the shared engine/GFFloatToLongNear.h; VC7.1 never auto-inlines a body holding
 // the expanded asm helper, so a caller that retail expanded defines SetSlerp / CalculateSlerpWeights FABLE_FORCEINLINE).
 float Dot(const CQuaternion& q) const;
 void SetIdentity();
 void SetSlerp(const CQuaternion& from, const CQuaternion& to, float t);
 static void FABLE_FASTCALL CalculateSlerpWeights(CQuaternion& weights, const CQuaternion& from, const CQuaternion& to, float t);
#endif
#ifdef FABLE_RETAIL_CQUATERNION_SLERPFROMIDENTITY
 // Retail opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540 C3DAnimationManager::Calc[Blended]MeshTransformations): FableWin.pdb
 // ?SetSlerpFromIdentity@CQuaternion@@QAEXABV1@M@Z (retail out-of-line copy 0x00AA3560, landed, uncalled; expanded in the rows through
 // C3DKeyframe::PostApplyRelativeAnimationAlpha). Declaration only: the includer defines the FABLE_FORCEINLINE body (landed 0x00AA3560's).
 void SetSlerpFromIdentity(const CQuaternion& q, float t);
#endif
#ifdef FABLE_RETAIL_CQUATERNION_MULASSIGN
 // Retail opt-in (chunk 2005, row 0x00A4D920 C3DAnimation2::CalcLocalBoneTransformationRelative): Ego_r.pdb + FableWin.pdb
 // ??DCQuaternion@@QBE?AV0@ABV0@@Z (retail 0x00A88B60) and ??XCQuaternion@@QAEABV0@ABV0@@Z (retail 0x00A88C10, landed; `ret 4`,
 // returns this), both out of line.
 CQuaternion operator*(const CQuaternion& q) const;
 const CQuaternion& operator*=(const CQuaternion& q);
#endif
#ifdef FABLE_RETAIL_CQUATERNION_MULASSIGN_INLINE
 // the inline header body (retail 0x00A88C10 is its emitted COMDAT copy): visible, so callers know it does not capture
 CQuaternion operator*(const CQuaternion& q) const;            // 0x00A88B60 (out of line)
 const CQuaternion& operator*=(const CQuaternion& q);
#endif
#ifdef FABLE_RETAIL_CQUATERNION_TOMATRIX_FORCEINLINE
 // Retail opt-in (chunk 2014, row 0x00A4E300 C3DAnimation2::GetDeltaMovement): the same body forced inline (FableWin.pdb
 // ?ToMatrix@CQuaternion@@QBEXAAVCMatrix3x4@@ABVC3DVector@@@Z has no retail out-of-line copy; plain inline leaves both of the row's
 // sites called, retail expands them, 0x00A4E3E1 / 0x00A4E578). Owner 2026-11-01 helper rule.
 FABLE_FORCEINLINE
#endif
 void ToMatrix(CMatrix3x4& dest,const C3DVector& translation)const {
  float d=GFSqr(X)+GFSqr(Y)+GFSqr(Z)+GFSqr(W);
  float s=(d!=0.0f)?2.0f/d:1.0f;
  float xs=X*s,ys=Y*s,zs=Z*s;
  float wx=W*xs,wy=W*ys,wz=W*zs;
  float xx=X*xs,xy=X*ys,xz=X*zs;
  float yy=Y*ys,yz=Y*zs,zz=Z*zs;
  dest.E11=1.0f-(yy+zz);dest.E12=xy-wz;dest.E13=xz+wy;
  dest.E21=xy+wz;dest.E22=1.0f-(xx+zz);dest.E23=yz-wx;
  dest.E31=xz-wy;dest.E32=yz+wx;dest.E33=1.0f-(xx+yy);
  dest.E41=translation.X;dest.E42=translation.Y;dest.E43=translation.Z;
 }
#ifdef FABLE_RETAIL_CQUATERNION_MATRIX4
 // Retail opt-in (Codex pass20): both TPI overloads and original0337fd70 body/locals.
#ifdef FABLE_RETAIL_CQUATERNION_MATRIX4_FORCEINLINE
 // (chunk 2047, row 0x00A9E1E0) forced: the row expands all four 4x4 conversions (c2 plain-inline scores 283..286, default
 // threshold 74); same body.
 FABLE_FORCEINLINE
#endif
 void ToMatrix(CMatrix4x4& dest,const C3DVector& translation)const {
  float d=GFSqr(X)+GFSqr(Y)+GFSqr(Z)+GFSqr(W);
  float s=(d!=0.0f)?2.0f/d:1.0f;
  float xs=X*s,ys=Y*s,zs=Z*s;
  float wx=W*xs,wy=W*ys,wz=W*zs;
  float xx=X*xs,xy=X*ys,xz=X*zs;
  float yy=Y*ys,yz=Y*zs,zz=Z*zs;
  dest(0,0)=1.0f-(yy+zz);dest(0,1)=xy-wz;dest(0,2)=xz+wy;dest(0,3)=0.0f;
  dest(1,0)=xy+wz;dest(1,1)=1.0f-(xx+zz);dest(1,2)=yz-wx;dest(1,3)=0.0f;
  dest(2,0)=xz-wy;dest(2,1)=yz+wx;dest(2,2)=1.0f-(xx+yy);dest(2,3)=0.0f;
  dest(3,0)=translation.X;dest(3,1)=translation.Y;dest(3,2)=translation.Z;dest(3,3)=1.0f;
 }
#endif
#ifdef FABLE_RETAIL_CQUATERNION_MATRIX4_TRANSPOSED
 // Retail opt-in (chunk 2047, row 0x00A9E1E0 C3DAnimationManager::ApplyParentTransformations): FableWin.pdb
 // ?ToMatrixTransposed@CQuaternion@@QBEXAAVCMatrix4x4@@ABVC3DVector@@@Z (0x03382B80, the COMDAT after ApplyParentTransformations, the
 // same debug size as ToMatrix 4x4: 676 bytes / 228 instructions), inline in retail: the ToMatrix body above with the rotation part
 // transposed and the translation in the fourth column (retail out-of-line copy of the C3DKeyframe wrapper: 0x00AA3AF0).
#ifdef FABLE_RETAIL_CQUATERNION_MATRIX4_FORCEINLINE
 FABLE_FORCEINLINE
#endif
 void ToMatrixTransposed(CMatrix4x4& dest,const C3DVector& translation)const {
  float d=GFSqr(X)+GFSqr(Y)+GFSqr(Z)+GFSqr(W);
  float s=(d!=0.0f)?2.0f/d:1.0f;
  float xs=X*s,ys=Y*s,zs=Z*s;
  float wx=W*xs,wy=W*ys,wz=W*zs;
  float xx=X*xs,xy=X*ys,xz=X*zs;
  float yy=Y*ys,yz=Y*zs,zz=Z*zs;
  dest(0,0)=1.0f-(yy+zz);dest(0,1)=xy+wz;dest(0,2)=xz-wy;dest(0,3)=translation.X;
  dest(1,0)=xy-wz;dest(1,1)=1.0f-(xx+zz);dest(1,2)=yz+wx;dest(1,3)=translation.Y;
  dest(2,0)=xz+wy;dest(2,1)=yz-wx;dest(2,2)=1.0f-(xx+yy);dest(2,3)=translation.Z;
  dest(3,0)=0.0f;dest(3,1)=0.0f;dest(3,2)=0.0f;dest(3,3)=1.0f;
 }
#endif
};
#ifdef FABLE_RETAIL_CQUATERNION_MULASSIGN_INLINE
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline const CQuaternion& CQuaternion::operator*=(const CQuaternion& q) { *this = *this * q; return *this; }
#endif
#endif
