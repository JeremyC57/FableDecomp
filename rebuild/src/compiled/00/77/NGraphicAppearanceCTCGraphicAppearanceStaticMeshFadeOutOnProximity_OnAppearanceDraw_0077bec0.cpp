// 0077BEC0 NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity::OnAppearanceDraw
// Inline-budget: /d2inlT104 expands GetInterpolated at0x0077BFCD; T103 keeps a call. Other native calls remain calls.
// Leaf: ?OnAppearanceDraw@CTCGraphicAppearanceStaticMeshFadeOutOnProximity@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z
// Retail: 0x0077BEC0,658 bytes,whole secondary-base virtual.
// Flags: /d2inlT104
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#define FABLE_RETAIL_CTCSMFADE_PASS22
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PASS22_PROXIMITY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINEMESHEFFECT_PASS22_DISTANCEFADE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_PASS22_INTERPOLATE
#define FABLE_RETAIL_NGRAPHICAPPEARANCE_PASS22_PRIMITIVETYPE
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearanceStaticMeshFadeOutOnProximity.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/C3DPositionInterpolator.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/CEnginePrimitiveStaticMesh.h"
// Genuine GetSubDef<CAppearanceDef>0x0042B0A2 uses its original size-mode body.
#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma optimize("y", off)
#endif
template<> bool CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>* out)const
{
    const CSubDefInfo* info=GetPSubDefInfo(CAppearanceDef::StatGetTypeName());
    if(info) {
        CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#ifdef _MSC_VER
#pragma optimize("t", on)
#pragma optimize("y", on)
#endif
// Genuine constructor native0x0077BD70,177bytes; original module74 parameter order.
CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh(unsigned __int64 uid,unsigned long mesh_index,const C3DVector& world_pos,const CRightHandedSet& rhset,float size,bool lit,unsigned char alpha,CRGBColour tint_colour,float fade_start,EEngineInterpolationSet interp_set,const CEnginePrimitiveThemeChannelDesc& theme_channel_desc,bool bullet_time_affects_cloth)
    :CEnginePrimitive((EEnginePrimitiveType)8,uid,interp_set),MeshIndex(mesh_index),WorldPos(world_pos),Size(size),FadeStart(fade_start),RightHandedSet(rhset),ThemeChannelDesc(theme_channel_desc),TintColour(tint_colour),Alpha(alpha),Lit(lit),BulletTimeAffectsCloth(bullet_time_affects_cloth)
{
}
namespace NGlobalConsole { extern bool DrawProximityFadeOutMeshes; }
namespace NGraphicAppearance {
bool CTCGraphicAppearanceStaticMeshFadeOutOnProximity::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance,const CGraphicAppearanceDrawInfo& draw_info)
{
    CDefPointer<const CAppearanceDef> pappearance_def;
    GetParentThing().GetPBaseDef()->GetSubDef(&pappearance_def);
    if(pappearance_def->HasFadeOutWhenCloseGraphic) {
        const CEngineGraphic& fade_out_graphic=pappearance_def->FadeOutWhenCloseGraphic;
        float scale=tc_appearance.GetScale()*fade_out_graphic.RenderSizeX;
        float fade_dist=pappearance_def->FadeOutGraphicRenderFadeDistance;
        if(fade_dist<DELTA_NOTIONAL_ZERO)
            fade_dist=tc_appearance.GetFadeDistance();
        if(!PrimHandle)
            PrimHandle=tc_appearance.AddPersistentPrimitive(CPrimitive(GAPT_STATIC_MESH,fade_out_graphic.BankIndex));
        else if(draw_info.ReasonsForDraw==1) {
            tc_appearance.RefreshPrimitivePos(PrimHandle,*draw_info.Pos,*draw_info.Orientation);
            return true;
        }
        if(NGlobalConsole::DrawProximityFadeOutMeshes) {
            bool bullet_time_affects_cloth=GetParentThing().IsAffectedByBulletTime();
            CEnginePrimitiveStaticMesh mesh_prim(GetParentThing().GetUniqueID(),fade_out_graphic.BankIndex,draw_info.Pos->GetInterpolated(draw_info.Interpolate),draw_info.Orientation->GetInterpolated(draw_info.Interpolate),scale,true,tc_appearance.GetAlpha(),tc_appearance.GetTintColour(),fade_dist,(EEngineInterpolationSet)0,tc_appearance.PeekLightingChannel(),bullet_time_affects_cloth);
            mesh_prim.MeshEffect=CIVCountedPointer<CEngineMeshEffect>(new CEngineMeshEffectDistanceFade(pappearance_def->FadeOutGraphicNearRenderClipDistance,pappearance_def->FadeOutGraphicNearRenderFadeDistance,fade_dist-5.0f,fade_dist));
            tc_appearance.SetPrimitiveGraphics(PrimHandle,mesh_prim,0x3000);
        }
    }
    return true;
}
}