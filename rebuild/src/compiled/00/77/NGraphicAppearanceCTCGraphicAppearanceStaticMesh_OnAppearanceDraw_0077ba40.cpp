#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_INLINE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_CTOR_CALLED_PASS26
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
// Leaf: ?OnAppearanceDraw@CTCGraphicAppearanceStaticMesh@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z
// Flags: /d2inlT200
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_INTERPOLATE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_PASS22_INTERPOLATE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PASS22_PROXIMITY
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_NGRAPHICAPPEARANCE_PASS22_PRIMITIVETYPE
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearanceStaticMesh.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/CEnginePrimitiveStaticMesh.h"
#include "engine/C3DPositionInterpolator.h"
#include "engine/C3DOrientationInterpolator.h"
namespace NGlobalConsole {extern long MultipleStaticMeshesToDraw;}
namespace NGraphicAppearance {
bool CTCGraphicAppearanceStaticMesh::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance,const CGraphicAppearanceDrawInfo& draw_info)
{
 const CEngineGraphic& graphic=tc_appearance.GetMainGraphic();
 if(graphic.BankIndex)
 {
  float scale=tc_appearance.GetScale()*graphic.RenderSizeX;
  float fade_dist=tc_appearance.GetFadeDistance();
  if(!PrimHandle)
   PrimHandle=tc_appearance.AddPersistentPrimitive(CPrimitive(GAPT_STATIC_MESH,graphic.BankIndex));
  else if(draw_info.ReasonsForDraw==1) {
   tc_appearance.RefreshPrimitivePos(PrimHandle,*draw_info.Pos,*draw_info.Orientation);
   return true;
  }
  CEnginePrimitiveThemeChannelDesc lighting_theme_channel=tc_appearance.PeekLightingChannel();
  if(NGlobalConsole::MultipleStaticMeshesToDraw<0 || lighting_theme_channel.ChannelIndex0==0)
  {
   bool bullet_time_affects_cloth=GetParentThing().IsAffectedByBulletTime();
   CEnginePrimitiveStaticMesh mesh_prim(GetParentThing().GetUniqueID(),graphic.BankIndex,
    draw_info.Pos->GetInterpolated(draw_info.Interpolate),draw_info.Orientation->GetInterpolated(draw_info.Interpolate),
    scale,true,tc_appearance.GetAlpha(),tc_appearance.GetTintColour(),fade_dist,(EEngineInterpolationSet)0,
    tc_appearance.PeekLightingChannel(),bullet_time_affects_cloth);
   mesh_prim.MeshEffect=tc_appearance.GetPMeshEffect();
   tc_appearance.SetPrimitiveGraphics(PrimHandle,mesh_prim,0);
  }
 }
 return true;
}
}