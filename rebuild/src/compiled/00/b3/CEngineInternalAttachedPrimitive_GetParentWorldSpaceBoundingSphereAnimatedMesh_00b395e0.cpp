// CEngineInternalAttachedPrimitive::GetParentWorldSpaceBoundingSphereAnimatedMesh -- transform parent mesh sphere center and safe radius
// Retail00B395E0,254bytes. Actual primitive base, mesh base and C3DMeshInfo headers.
// No attached header available; PDB fields: PrimitiveTransform48,ParentHandle80,ParentGroupIndex88.
// ParentHandle firstword opaque. Native slots12 GetChildPrimitive(groupVT12A66E4->BC61C0),
// 33 CalcInterpolatedObjectTransform(animatedVT12A79B4->BCEF80,out,float,bool)const. Native uses true; const verified donorA836E0 PDB signature.
// Leaf: GetParentWorldSpaceBoundingSphereAnimatedMesh
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/_quarantine/CEngineInternalPrimitiveMeshBase.h"
#include "engine/C3DMeshInfo.h"
struct C3DVector {float X,Y,Z;C3DVector(){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
struct C3DBoundingSphere {C3DVector Center;float Radius;C3DVector GetPos()const{return C3DVector(Center.X,Center.Y,Center.Z);}void SetPos(const C3DVector& p){Center=p;}};
struct MeshInfoMethods:C3DMeshInfo {C3DBoundingSphere GetBoundingSphere()const{return *reinterpret_cast<const C3DBoundingSphere*>(BoundingSphere);}};
struct CMatrix3x4 {C3DVector X,Y,Z,Pos;
 __forceinline void Transform(C3DVector& v)const{float x=v.X,y=v.Y,z=v.Z;v.X=X.X*x+Y.X*y+Z.X*z+Pos.X;v.Y=X.Y*x+Y.Y*y+Z.Y*z+Pos.Y;v.Z=X.Z*x+Y.Z*y+Z.Z*z+Pos.Z;}
};
struct MeshMethods;
#pragma pointers_to_members(full_generality,single_inheritance)
union PrimitiveSlot {void* Address;MeshMethods*(MeshMethods::*Child)(unsigned long);void(MeshMethods::*Transform)(CMatrix3x4&,float,bool)const;};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(PrimitiveSlot)==4);
#endif
struct MeshMethods:CEngineInternalPrimitiveMeshBase {
 MeshMethods* GetChildPrimitive(unsigned long i){return(this->*reinterpret_cast<const PrimitiveSlot*>(__vftable)[12].Child)(i);}
 void CalcInterpolatedObjectTransform(CMatrix3x4& out,float t,bool scale)const{(this->*reinterpret_cast<const PrimitiveSlot*>(__vftable)[33].Transform)(out,t,scale);}
};
struct NativeHandle {unsigned int UnusedHandleWord;MeshMethods* Primitive;};
struct CEngineInternalAttachedPrimitive:CEngineInternalPrimitiveBase {CMatrix3x4 PrimitiveTransform;int ParentType,ChildType;NativeHandle ParentHandle;int ParentGroupIndex;
 bool GetParentWorldSpaceBoundingSphereAnimatedMesh(C3DBoundingSphere&,float);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CEngineInternalAttachedPrimitive,ParentHandle)==0x80);
FABLE_STATIC_ASSERT(offsetof(CEngineInternalAttachedPrimitive,ParentGroupIndex)==0x88);
#endif
bool CEngineInternalAttachedPrimitive::GetParentWorldSpaceBoundingSphereAnimatedMesh(C3DBoundingSphere& out,float frame){
 MeshMethods* parent=ParentGroupIndex!=-1?ParentHandle.Primitive->GetChildPrimitive(ParentGroupIndex):ParentHandle.Primitive;
 if(!parent)return false;
 CMatrix3x4 transform;parent->CalcInterpolatedObjectTransform(transform,frame,true);
 out.SetPos(static_cast<const MeshInfoMethods*>(parent->MeshInfo)->GetBoundingSphere().GetPos());
 transform.Transform(out.Center);out.Radius=parent->MeshInfo->SafeBoundingRadius*parent->Scale;return true;
}