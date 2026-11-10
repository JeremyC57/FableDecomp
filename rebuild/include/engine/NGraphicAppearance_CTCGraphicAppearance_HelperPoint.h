#ifndef FABLE_ENGINE_CTCGRAPHICAPPEARANCE_HELPERPOINT_BODY_H
#define FABLE_ENGINE_CTCGRAPHICAPPEARANCE_HELPERPOINT_BODY_H
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
// Original complete ordinary definition, donor01D0C5D0 and native004C0560..004C08B3.
// Diagnostic-only debug reporting is absent in retail; GetAnims still executes.
bool NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint(const CCharString& name,long n,unsigned long flags,C3DVector& point,float interpolate)const {
 bool reduced_lod=false;
 bool force_accurate=true;
 if(flags&1)force_accurate=false;
 bool ignore_animation=false;
 if(flags&4)ignore_animation=true;
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
 for(std::vector<CMeshInfo>::iterator it=meshes_used.begin();it!=meshes_used.end();++it) {
  CMeshInfo& mesh_info=*it;
  if(mesh_info.Index!=0) {
   bool start_loading=(flags&2)==0;
   bool return_if_not_loaded=!start_loading||(flags&32)!=0;
   if(!IsResourcesLoadedForMesh(mesh_info,start_loading)&&return_if_not_loaded)return false;
   C3DMeshObject object;
   GetMeshObject(mesh_info,interpolate,object);
   if(object.HasHelperPoint(name)) {
    if(n>=object.GetNoHelperPoints(name))return false;
    if(object.IsAnimated()&&!ignore_animation) {
     if(reduced_lod)point=GetRenderPositionForThing(GetParentThing(),1.0f);
     else {
      C3DAnimationInterpolator anims;
      GetAnims(anims);
      C3DAnimationManager* anim_manager=PeekWorld().GetAnimationManager();
      point=object.GetHelperPointPositionAnimated(name,n,anims.StartState,anims.EndState,interpolate,*anim_manager);
     }
    }else point=object.GetHelperPointPositionStatic(name,n);
    return true;
   }
  }
 }
 return false;
}
#endif
