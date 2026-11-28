#ifndef FABLE_ENGINE_CQUATERNION_H
#define FABLE_ENGINE_CQUATERNION_H
#include "engine/CMatrix3x4.h"
#ifdef FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#include "engine/CRightHandedSet.h"
#endif
#ifndef FABLE_RETAIL_CQUATERNION_PARTICLE_ADD_SHARED_SQUARE
inline float FABLE_FASTCALL GFSqr(float value){return value*value;}
#endif
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
#ifdef FABLE_RETAIL_RIGHTHANDEDSET_INTERPOLATE_PASS30
 float Dot(const CQuaternion& q) const { return X*q.X + Y*q.Y + Z*q.Z + W*q.W; }
 void SetSlerp(const CQuaternion& from, const CQuaternion& to, float t);
#endif
#ifdef FABLE_RETAIL_CQUATERNION_PARTICLE_ADD
 CQuaternion operator*(const CQuaternion& q)const;
#endif
#ifdef FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
 // Both original PDB signatures; native A88C10/A88DE0, donor Update call sites.
#ifdef FABLE_RETAIL_CQUATERNION_RHS_EARLY_PASS25
 // Inline-budget: test receiver-address visibility before dead old_rhs removal.
 FABLE_FORCEINLINE
#endif
 void ToRightHandedSet(CRightHandedSet& dest)const {
  float d=GFSqr(X)+GFSqr(Y)+GFSqr(Z)+GFSqr(W);
#if defined(FABLE_RETAIL_CQUATERNION_PARTICLE_ADD_FLOAT_SCALE) || defined(FABLE_RETAIL_CQUATERNION_TORIGHTHANDEDSET_FLOAT)
  // Opt-in (wfD6, row 0x00B01900): single-precision literals; retail 0x00B02493 `fdivr dword ptr [2.0f]` / 0x00B0249D `fld dword ptr [1.0f]`.
  float s=(d!=0.0f)?2.0f/d:1.0f;
#else
  float s=(d!=0.0f)?2.0/d:1.0;
#endif
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
#if defined(FABLE_RETAIL_KEYFRAME_MATRIX_ROWS_PASS29) && !defined(FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE)
 void SetFromRotationMatrix(const CMatrix3x4&);
#endif
 void SetFromAxisAngle(const C3DVector& axis,float angle);
#ifdef FABLE_RETAIL_CQUATERNION_EQUALS
 // Retail opt-in (p25C, row 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData): Ego_r.pdb / FableWin.pdb
 // ?Equals@CQuaternion@@QBE_NABV1@@Z (out of line in retail, 0x00A88C50) and ?IsIdentity@CQuaternion@@QBE_NXZ (0x00A88CB0).
 // Declarations only.
 bool Equals(const CQuaternion& other) const;
 bool IsIdentity() const;
#endif
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
#ifdef FABLE_RETAIL_CQUATERNION_MUL
 // Opt-in (wfD6, row 0x00B01900): Ego_r / FableWin ??DCQuaternion@@QBE?AV0@ABV0@@Z alone (out of line at 0x00A88B60), for TUs that
 // already declare operator*= through FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE (MULASSIGN would declare it twice).
 CQuaternion operator*(const CQuaternion& q) const;
#endif
#ifdef FABLE_RETAIL_CQUATERNION_MULASSIGN_INLINE
 // the inline header body (retail 0x00A88C10 is its emitted COMDAT copy): visible, so callers know it does not capture
 CQuaternion operator*(const CQuaternion& q) const;            // 0x00A88B60 (out of line)
 const CQuaternion& operator*=(const CQuaternion& q);
#endif
#if defined(FABLE_RETAIL_CQUATERNION_TOMATRIX_FORCEINLINE) || defined(FABLE_RETAIL_CQUATERNION_INLINE_TOMATRIX)   // INLINE_TOMATRIX: the older opt-in for the same marking (its note is above SLERP)
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
#ifdef FABLE_RETAIL_ANIMATION_KEYFRAME_TRACKS_PASS30
#include "engine/GFFastArcCos.h"
#include "engine/GFFastSin.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
inline float CQuaternion::Dot(const CQuaternion& q2) const
{
    return X * q2.X + Y * q2.Y + Z * q2.Z + W * q2.W;
}
inline void CQuaternion::SetIdentity()
{
    X = 0.0f; Y = 0.0f; Z = 0.0f; W = 1.0f;
}
// Inline-budget: the native interpolation expands both the trigonometric and linear branches.
FABLE_FORCEINLINE void CQuaternion::SetSlerp(const CQuaternion& q1, const CQuaternion& q2, float weight2)
{
    float scale1;
    float scale2;
    float cosa = q1.Dot(q2);
    float sign = 1.0f;
    if (cosa < 0.0f)
    {
        cosa = -cosa;
        sign = -1.0f;
    }
    if (1.0f - cosa > 0.05f)
    {
        float angle = GFFastArcCos(cosa);
        float sina = (float)sqrt(1.0f - cosa * cosa);
        scale1 = GFFastSin((1.0f - weight2) * angle) / sina;
        scale2 = GFFastSin(weight2 * angle) / sina;
    }
    else
    {
        scale1 = 1.0f - weight2;
        scale2 = weight2;
    }
    scale2 *= sign;
    X = scale1 * q1.X + scale2 * q2.X;
    Y = scale1 * q1.Y + scale2 * q2.Y;
    Z = scale1 * q1.Z + scale2 * q2.Z;
    W = scale1 * q1.W + scale2 * q2.W;
}
#endif
#ifdef FABLE_RETAIL_RIGHTHANDEDSET_INTERPOLATE_PASS30
#include "engine/GFFastArcCos.h"
#include "engine/GFFastSin.h"
extern "C" double __cdecl sqrt(double);
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
// Inline-budget: the three native rounding sites in Interpolate expand this member. Plain inline leaves a SetSlerp call.
FABLE_FORCEINLINE void CQuaternion::SetSlerp(const CQuaternion& from, const CQuaternion& to, float t)
{
    float cosom = from.Dot(to);
    float sign = 1.0f;
    if (cosom < 0.0f) {
        sign = -1.0f;
        cosom = -cosom;
    }
    float scale0, scale1;
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)sqrt(1.0f - cosom * cosom);
        scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        scale1 = GFFastSin(omega * t) * (1.0f / sinom);
    } else {
        scale0 = 1.0f - t;
        scale1 = t;
    }
    scale1 *= sign;
    X = scale0 * from.X + scale1 * to.X;
    Y = scale0 * from.Y + scale1 * to.Y;
    Z = scale0 * from.Z + scale1 * to.Z;
    W = scale0 * from.W + scale1 * to.W;
}
#endif
#ifdef FABLE_RETAIL_CQUATERNION_MULASSIGN_INLINE
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline const CQuaternion& CQuaternion::operator*=(const CQuaternion& q) { *this = *this * q; return *this; }
#endif
#endif
