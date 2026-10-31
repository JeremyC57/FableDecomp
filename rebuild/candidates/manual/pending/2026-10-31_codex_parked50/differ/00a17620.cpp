// 00a17620 CPhysicsMesh::HasBoundingSphereIntersectedFromTo
// Codex parked-50 best attempt: DIFFER(99v99); residue: 99/99 bytes; radius/member versus argument x87 load order remains different.
// Flags:
// Leaf: ?HasBoundingSphereIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class C3DVector{public:float X,Y,Z;__forceinline C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
// Layout: original donor Center0, Radius12 and 16-byte size.
class C3DBoundingSphere{public:C3DVector Center;float Radius;};
// Layout: RTTI solebase CIVCountedPointeeBase8. Unread container prefix shrinks
// donor BoundingSphere144 to retail116. No unused field identity is invented.
class CPhysicsMesh:public CIVCountedPointeeBase{unsigned char pass9_unread_prefix[108];public:C3DBoundingSphere BoundingSphere;
 bool HasBoundingSphereIntersectedFromTo(float,const C3DVector&,const C3DVector&)const;
};
float __fastcall GFGetShortestDistanceSquaredBetweenPointAndLine(const C3DVector&,const C3DVector&,const C3DVector&,float&);
bool CPhysicsMesh::HasBoundingSphereIntersectedFromTo(float radius,const C3DVector&from,const C3DVector&to)const{
 const float* sphereRadius=&BoundingSphere.Radius;
 C3DVector center=BoundingSphere.Center;
 float reach=*sphereRadius+radius+15.0f;
 float parameter;
 if(GFGetShortestDistanceSquaredBetweenPointAndLine(from,to,center,parameter)<=reach*reach)return true;
 return false;
}
