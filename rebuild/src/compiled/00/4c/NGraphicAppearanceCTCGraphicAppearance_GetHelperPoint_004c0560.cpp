// NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint(const CCharString& name, long n, unsigned long flags, C3DVector& point,
//   float interpolate) const -- find helper point `name` #n on the first of the appearance's meshes that has it (meshes optionally
//   sorted primary-first (flags&8) / primary-last (flags&16)); loads the mesh if needed (flags&2 = do not start loading, flags&32 =
//   fail when not loaded), then returns its animated position (unless flags&4 or the mesh is static), the parent's render position
//   when animations can be ignored at reduced LOD (flags&1 = LOD allowed), or its static position. False when no mesh has it.
// Retail: Fable.exe 0x004C0560, 851 bytes, __fastcall, `ret 14h`.
// Callees: IsToReduceLODForDummiesAndHelpers 0x004BD2E0; CVectorMap::LowerBound 0x004365B0 (inline CThing::PeekTC);
//   CTCAnimationComplex::PeekAnimation 0x0070B470; CAnimateThing::AnimsCanBeIgnored 0x0070C100; vector<CMeshInfo>::reserve
//   0x004C46C0 + CAGraphicAppearanceComponent::GetMeshesUsed (slot 2) via inline GetAllMeshes (body as in landed 0x004BE3E0);
//   std::sort<CMeshInfo*,CSortPrimaryMeshesFirst/Last> 0x004C5C80 / 0x004C5CE0 (__fastcall); CMeshDataBank::PeekMeshInfo
//   0x00A24260 on g_pMeshDataBank 0x013B8A04, C3DMeshInfo::IsLoaded 0x00A23E40 / BeginLoading 0x00A256F0 via inline
//   IsResourcesLoadedForMesh (out-of-line copy 0x004BC090); CBaseClass ctor 0x0099A2D0 / dtor 0x0099A2E0 (inline
//   C3DMeshObject ctor / dtor); GetMeshObject 0x004BC4F0; C3DMeshObject::HasHelperPoint 0x00A36100, GetNoHelperPoints 0x00A36140,
//   GetHelperPointPositionStatic 0x00A361A0, GetHelperPointPositionAnimated 0x00A36350, ~C3DMeshObject 0x004C3200;
//   GetRenderPositionForThing 0x004BBF00 (file-static __fastcall); C3DAnimationInterpolator ctor 0x004C2E20 / dtor 0x004C2E90;
//   GetAnims 0x004BC000; CTCBase::PeekWorld 0x00686A80; operator delete 0x00BFE9BC; free 0x00BFEA14.
// Shape: the mesh loop is a `while` with `++it` as the last statement of the body. With that loop VC7.1 expands the inline
//   ~C3DMeshObject at the loop continue and calls its COMDAT 0x004C3200 at both returns, and homes `this` at [esp+24h]
//   (the `for` spelling of the shared HelperPoint header expands it at all three sites, 1001 v 851 bytes).
// Header overlay: C3DMeshObject.h FABLE_RETAIL_C3DMESHOBJECT_INLINE (inline ctor / empty dtor / IsAnimated),
//   C3DMeshStats.h FABLE_RETAIL_C3DMESHSTATS_ISANIMATED. Same-TU real inline bodies: GetAllMeshes, IsResourcesLoadedForMesh.
//   Both bodies are copied from landed 0x004BE3E0 (NGraphicAppearanceCTCGraphicAppearance_IsResourcesLoaded_004be3e0.cpp), where
//   they are byte-proven as that row's inline callees. Landed 0x004BD420 is a different, non-const spelling of GetAllMeshes and is
//   not the body used here; the out-of-line copy of IsResourcesLoadedForMesh at 0x004BC090 has not landed.
// Leaf: ?GetHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@M@Z
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
extern CMeshDataBank* g_pMeshDataBank;                                   // 0x013B8A04
namespace NGraphicAppearance {
inline void CTCGraphicAppearance::GetAllMeshes(std::vector<CMeshInfo>& meshes) const
{
    meshes.reserve(4);
    for (std::list<CAGraphicAppearanceComponent*>::const_iterator it = Components.begin(); it != Components.end(); ++it)
        (*it)->GetMeshesUsed(meshes);
}
inline bool CTCGraphicAppearance::IsResourcesLoadedForMesh(const CMeshInfo& mesh, bool begin_loading) const
{
    const C3DMeshInfo* mesh_info = g_pMeshDataBank->PeekMeshInfo(mesh.Index);
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
 std::vector<CMeshInfo>::iterator it=meshes_used.begin();
 while(it!=meshes_used.end()) {
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
  ++it;
 }
 return false;
}