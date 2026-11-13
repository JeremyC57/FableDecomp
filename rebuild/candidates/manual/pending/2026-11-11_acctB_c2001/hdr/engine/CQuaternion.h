#ifndef FABLE_ENGINE_CQUATERNION_H
#define FABLE_ENGINE_CQUATERNION_H
#include "engine/CMatrix3x4.h"
inline float FABLE_FASTCALL GFSqr(float value){return value*value;}
#ifdef FABLE_RETAIL_CQUATERNION_MATRIX4
#include "engine/CMatrix4x4.h"
#endif
class CQuaternion {
public:
 float X,Y,Z,W;
 CQuaternion(){}
 void SetFromAxisAngle(const C3DVector& axis,float angle);
#ifdef FABLE_RETAIL_CQUATERNION_SLERP
 // Retail opt-in (chunk 2001, row 0x00A9EB90 C3DAnimationManager::BlendAnimations): FableWin.pdb ?Dot@CQuaternion@@QBEMABV1@@Z,
 // ?SetSlerp@CQuaternion@@QAEXABV1@0M@Z and the static ?CalculateSlerpWeights@CQuaternion@@SIXAAV1@ABV1@1M@Z (out-of-line copy
 // 0x00AA3390, landed), all expanded in retail. Declarations only: the includer defines the bodies (they need the engine's
 // GFFastArcCos / GFFastSin table helpers and the shared engine/GFFloatToLongNear.h; VC7.1 never auto-inlines a body holding
 // the expanded asm helper, so a caller that retail expanded defines SetSlerp / CalculateSlerpWeights FABLE_FORCEINLINE).
 float Dot(const CQuaternion& q) const;
 void SetSlerp(const CQuaternion& from, const CQuaternion& to, float t);
 static void FABLE_FASTCALL CalculateSlerpWeights(CQuaternion& weights, const CQuaternion& from, const CQuaternion& to, float t);
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
};
#endif
