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
