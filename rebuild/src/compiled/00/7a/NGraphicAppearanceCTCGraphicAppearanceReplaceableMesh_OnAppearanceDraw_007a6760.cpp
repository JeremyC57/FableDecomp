// 007A6760 NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::OnAppearanceDraw -- preload and select replacement meshes.
// Leaf: ?OnAppearanceDraw@CTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z
// Retail: 0x007A6760,645 bytes,whole secondary-base virtual.
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CREPLACEABLEMESHDEF_PASS22
#define FABLE_RETAIL_CTCREPLACEABLEMESH_PASS22
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PASS22_PROXIMITY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVECHANGER_PASS22
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_NGRAPHICAPPEARANCE_PASS22_PRIMITIVETYPE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearanceReplaceableMesh.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/CEnginePrimitiveChanger.h"
#include "engine/CEnginePrimitiveStaticMesh.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh(unsigned __int64 uid,unsigned long mesh_index,const C3DVector& world_pos,const CRightHandedSet& rhset,float size,bool lit,unsigned char alpha,CRGBColour tint_colour,float fade_start,EEngineInterpolationSet interp_set,const CEnginePrimitiveThemeChannelDesc& theme_channel_desc,bool bullet_time_affects_cloth)
    :CEnginePrimitive((EEnginePrimitiveType)8,uid,interp_set),MeshIndex(mesh_index),WorldPos(world_pos),Size(size),FadeStart(fade_start),RightHandedSet(rhset),ThemeChannelDesc(theme_channel_desc),TintColour(tint_colour),Alpha(alpha),Lit(lit),BulletTimeAffectsCloth(bullet_time_affects_cloth)
{
}
namespace NGraphicAppearance {
bool CTCGraphicAppearanceReplaceableMesh::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance,const CGraphicAppearanceDrawInfo& draw_info)
{
    const CEngineGraphic& graphic=tc_appearance.GetMainGraphic();
    if(!PrimHandle) {
        PrimHandle=tc_appearance.AddPersistentPrimitive(CPrimitive(GAPT_STATIC_MESH,graphic.BankIndex));
        CEnginePrimitiveChanger prim_changer(MeshIndex,0,true,(EEngineInterpolationSet)0);
        tc_appearance.SetPrimitiveGraphics(PrimHandle,prim_changer,0);
        unsigned long child_index=0;
        bool bullet_time_affects_cloth=GetParentThing().IsAffectedByBulletTime();
        for(CArray<CEngineGraphic>::const_iterator graphic_iterator=PDef->Meshes.Vector.begin();graphic_iterator!=PDef->Meshes.Vector.end();graphic_iterator++) {
            CEnginePrimitiveStaticMesh loop_graphic(0,graphic_iterator->BankIndex,GetParentThing().GetPos(),GetParentThing().GetPhysicsTC()->GetRHSet(),1.0f,true,255,CRGBColour(255,255,255),0.0f,(EEngineInterpolationSet)0,tc_appearance.PeekLightingChannel(),bullet_time_affects_cloth);
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetChildPrimitive(PrimHandle->PrimitiveHandle,child_index,loop_graphic,0);
            ++child_index;
        }
    }
    {
        CEnginePrimitiveChanger prim_changer(MeshIndex,0,true,(EEngineInterpolationSet)0);
        prim_changer.MeshEffect=tc_appearance.GetPMeshEffect();
        tc_appearance.SetPrimitiveGraphics(PrimHandle,prim_changer,0);
        tc_appearance.RefreshPrimitivePos(PrimHandle,*draw_info.Pos,*draw_info.Orientation);
        Replaced=false;
    }
    return true;
}
}