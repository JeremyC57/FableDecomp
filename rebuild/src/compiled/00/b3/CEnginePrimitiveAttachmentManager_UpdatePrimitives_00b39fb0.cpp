// 00b39fb0 CEnginePrimitiveAttachmentManager::UpdatePrimitives -- update visible attached primitives and remove expired records.
// Leaf: ?UpdatePrimitives@CEnginePrimitiveAttachmentManager@@QAEXABV?$CArray@PAVCEngineCamera@@@@M@Z
// Retail:00b39fb0,284 bytes,__fastcall.
// Flags: none
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_LIST
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_UPDATE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CPLANE_LINE
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_STLP_LIST
#include "engine/CEnginePrimitiveAttachmentManager.h"
#include "engine/CArray.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/C3DMeshInfo.h"
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
bool CEngineInternalAttachedPrimitive::GetParentWorldSpaceBoundingSphereAnimatedMesh(C3DBoundingSphere& dest,float interp)
{
 CEnginePersistentPrimitiveBase* prim_base=GetParentPrimitive();
 if(!prim_base)return false;
 const CEngineInternalPrimitiveAnimatedMesh& parent_prim=*static_cast<const CEngineInternalPrimitiveAnimatedMesh*>(prim_base);
 CMatrix3x4 transform;
 parent_prim.CalcInterpolatedObjectTransform(transform,interp,true);
 dest.Center=parent_prim.MeshInfo->GetBoundingSphere().GetCenter();
 transform.TransformPoint(dest.Center);
 dest.Radius=parent_prim.GetBoundingSphereRadiusWorldSpace();
 return true;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
bool CEngineInternalAttachedPrimitiveInfo::GetOuterBoundingSphere(C3DBoundingSphere& dest,float interp){
  CEngineInternalAttachedPrimitive& attach_prim=GetPrimitive();
  CEngineInternalPrimitiveBase& output_prim=attach_prim.GetOutputPrimitive();
  if(attach_prim.GetParentWorldSpaceBoundingSphere(dest,interp)){
   float child_radius;
   if(output_prim.GetBoundingSphereRadiusWorldSpace(child_radius)){
    dest.Radius+=child_radius*2.0;
    return true;
   }
  }
  return false;
 }

bool CEngineInternalAttachedPrimitive::GetParentWorldSpaceBoundingSphere(C3DBoundingSphere& dest,float interp){
 switch(ParentType){case PARENT_TYPE_ANIMATED_MESH:return GetParentWorldSpaceBoundingSphereAnimatedMesh(dest,interp);}
 return false;
}
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
// Original same-TU no-capture body (FableWin02c9df20, module968).
bool CEngineCamera::ClipSphere(const C3DVector& center,float radius)const
{
 C3DVector vec=center-CameraDesc.WorldPosition;
 float zdist=vec.Dot(LookVector);
 if(zdist+radius<CameraDesc.NearZPlane||zdist-radius>CameraDesc.FarZPlane)return false;
 if(vec.Dot(FrustrumPlanes[2].GetNormal())>radius)return false;
 if(vec.Dot(FrustrumPlanes[3].GetNormal())>radius)return false;
 if(vec.Dot(FrustrumPlanes[0].GetNormal())>radius)return false;
 if(vec.Dot(FrustrumPlanes[1].GetNormal())>radius)return false;
 return true;
}
bool CEnginePrimitiveAttachmentManager::IsVisible(const C3DBoundingSphere& bsphere,float max_dist,const CArray<CEngineCamera*>& cameras)const
{
 CArray<CEngineCamera*>::const_iterator camera_itr;
 for(camera_itr=cameras.begin();camera_itr!=cameras.end();++camera_itr){
  CEngineCamera* camera=*camera_itr;
  if((camera->PeekCameraPos()-bsphere.Center).GetMagnitude()<max_dist-bsphere.Radius&&camera->ClipSphere(bsphere.Center,bsphere.Radius))return true;
 }
 return false;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void CEnginePrimitiveAttachmentManager::UpdatePrimitives(const CArray<CEngineCamera*>& cameras,float interp)
{
 TAttachedPrimitiveList::iterator itr;
 for(itr=AttachedPrimitives.begin();itr!=AttachedPrimitives.end();){
  TAttachedPrimitiveList::iterator next_itr=itr;
  ++next_itr;
  CEngineInternalAttachedPrimitiveInfo& prim_info=*itr;
  if(prim_info.IsAlive()){
   bool visible=false;
   C3DBoundingSphere bsphere;
   if(prim_info.GetOuterBoundingSphere(bsphere,interp)){
    CEngineInternalPrimitiveBase& out_prim=prim_info.GetPrimitive().GetOutputPrimitive();
    float max_dist=out_prim.FadeDistance.MaxDrawDistance;
    if(IsVisible(bsphere,max_dist,cameras)){
     prim_info.GetPrimitive().UpdateOutputPrimitive(interp);
     visible=true;
    }
   }
   prim_info.GetPrimitive().GetOutputPrimitive().SetEnable(visible);
  }else AttachedPrimitives.erase(itr);
  itr=next_itr;
 }
}