// Leaf: ?GetDummyObjectAssumingObjectAt@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@ABVC3DVector@@ABVCRightHandedSet@@JKAAV4@AAV5@@Z
// Inline-budget: animated Init calls the shared body at 004BBE10, static Init expands; verified against both original call sites.
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_GRAPHIC_DUMMY_PASS29
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_C3DMESHOBJECT_STATIC_DUMMY_PASS26
#define FABLE_RETAIL_C3DMESHOBJECT_DUMMY_PAIR_PASS24
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_PEEKINFO
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHOBJECT_INLINE
#include "engine/ETCInterfaceType.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
#include "engine/NGraphicAppearance_CMeshInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"

namespace NGraphicAppearance {
inline void CTCGraphicAppearance::GetAllMeshes(std::vector<CMeshInfo>& meshes) const
{
    meshes.reserve(4);
    for (std::list<CAGraphicAppearanceComponent*>::const_iterator it = Components.begin(); it != Components.end(); ++it)
        (*it)->GetMeshesUsed(meshes);
}
inline bool CTCGraphicAppearance::IsResourcesLoadedForMesh(const CMeshInfo& mesh, bool begin_loading) const
{
    const C3DMeshInfo* mesh_info = PMeshBank->PeekMeshInfo(mesh.Index);
    bool loaded = mesh_info->IsLoaded();
    if (!loaded && begin_loading)
        mesh_info->BeginLoading((unsigned long)-1, false, false);
    return loaded;
}
}
#include "engine/CThing.h"
#ifndef FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#endif
#include "engine/CTCAnimationComplex.h"
#include "engine/C3DAnimationInterpolator.h"
#include "engine/C3DMeshObject.h"
#include "engine/CWorld.h"
#include "stlport/fable_algorithm.h"
namespace { C3DVector FABLE_FASTCALL GetRenderPositionForThing(const CThing&,float); }

#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
bool NGraphicAppearance::CTCGraphicAppearance::GetDummyObjectAssumingObjectAt(const CCharString& name,const C3DVector& at_pos,const CRightHandedSet& at_rhs,long n,unsigned long flags,C3DVector& point,CRightHandedSet& rhset)const {
 bool force_accurate=true;
 if(flags&1)force_accurate=false;
 bool ignore_animation=false;
 if(flags&4)ignore_animation=true;
 bool reduced_lod=false;
 if(!force_accurate&&IsToReduceLODForDummiesAndHelpers()) {
  const CTCAnimationComplex* ptc_anim;
  if(GetParentThing().PeekTC(&ptc_anim)) {
   if(ptc_anim->PeekAnimation().AnimsCanBeIgnored())reduced_lod=true;
  }
 }
 std::vector<CMeshInfo> meshes_used;
 GetAllMeshes(meshes_used);
 if(flags&8)std::sort(meshes_used.begin(),meshes_used.end(),CSortPrimaryMeshesFirst());
 else if(flags&16)std::sort(meshes_used.begin(),meshes_used.end(),CSortPrimaryMeshesLast());
 for(std::vector<CMeshInfo>::iterator it=meshes_used.begin();it!=meshes_used.end();++it){
  CMeshInfo& mesh_info=*it;
  if(mesh_info.Index!=0) {
   bool start_loading=(flags&2)==0;
   if(!IsResourcesLoadedForMesh(mesh_info,start_loading)&&!start_loading)return false;
   C3DMeshObject object;
   GetMeshObjectAssumingObjectAt(mesh_info,at_pos,at_rhs,object);
   if(object.HasDummyObject(name)){
    C3DVector look,up;
    
    if(object.IsAnimated()&&!ignore_animation){
     if(reduced_lod){
      point=GetRenderPositionForThing(GetParentThing(),1.0f);
      rhset=GetParentThing().GetPhysicsTC()->GetRHSet();
     }else{
      C3DAnimationInterpolator anims;
      const bool success=GetAnims(anims);
      FABLE_RETAIL_ASSERT(success);
      object.GetDummyObjectPositionAnimated(name,n,anims.StartState,anims.EndState,1.0f,*PeekWorld().GetAnimationManager(),point,look,up,false);
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
      rhset.Init(up,look);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
     }
    }else{
     object.GetDummyObjectPositionStatic(name,n,point,look,up);
     rhset.Init(up,look);
    }
    return true;
   }
  }

 }
 return false;
}