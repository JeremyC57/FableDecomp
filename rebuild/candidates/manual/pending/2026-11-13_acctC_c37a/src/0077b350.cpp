// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) -- draw the thing's main graphic as an animated mesh primitive (a morphed one when the draw has texture morphs, decals or hidden materials); position-only draws just refresh the interpolators
// Leaf: ?OnAppearanceDraw@CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077B350, 767 bytes, thiscall on the CAGraphicAppearanceComponent subobject (+0x0C), three `ret 8` (last 0x0077B64C).
// Name: Ego_r.pdb 0x00784BC0 ?OnAppearanceDraw@CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z
//   (donor order ctor 0x00784B00 / dtor / OnCreate / .. / OnAppearanceDraw = retail 0x0077B250 / 0x0077B290 / 0x0077B2D0 / .. / 0x0077B350) + the
//   CAGraphicAppearanceComponent vtable slot 0 with `ret 8` and a bool result, as the landed sibling 0x00727D00 (CTCRandomAppearanceMorph).
// Callees: GetFadeDistance 0x004BC180, CBaseClass 0x0099A2D0 (inline CPrimitive ctor), AddPersistentPrimitive 0x004C0050, ~CPrimitive 0x004C4160,
//   RefreshPrimitiveAnimationAndPos 0x004BCFE0, CThing::IsAffectedByBulletTime 0x004C9920, GetTintColour 0x004BD040, CEnginePrimitiveAnimatingMesh
//   0x007088F0 / ~ 0x00708A30, CEnginePrimitiveMorphedAnimatingMesh 0x00709A50 / ~ 0x00709B10, GetPMeshEffect 0x004BCF50, SetPrimitiveGraphics
//   0x004BD620, vector<CEngineTextureDecal>::operator= 0x0077B860, ~vector<CEngineTextureMorph> 0x005BC0D3 (both out of line by the budget).
// Shape: a named reference to the main graphic (`fld Scale ; fmul RenderSizeX`, 0x0077B360: with GetMainGraphic() spelled inside the product
//   VC7.1 loads RenderSizeX first, as the out-of-line GetFullScale 0x004BBFF0 does); a named bool for IsAffectedByBulletTime (called before
//   the argument pushes, 0x0077B40C); the texture-morph vector defaults to an empty local (0x0077B503); CIVCountedPointer::operator= expanded.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/NGraphicAppearance_CPrimitive.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEnginePrimitiveAnimatingMesh.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearanceAnimatedMesh.h"

namespace NGraphicAppearance {
bool CTCGraphicAppearanceAnimatedMesh::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance, const CGraphicAppearanceDrawInfo& draw_info)
{
    const CEngineGraphic& graphic = tc_appearance.GetMainGraphic();
    float size = tc_appearance.GetScale() * graphic.RenderSizeX;
    float fade_distance = tc_appearance.GetFadeDistance();
    if (PrimHandle == 0) {
        PrimHandle = tc_appearance.AddPersistentPrimitive(CPrimitive((EPrimitiveType)2, graphic.BankIndex));
    } else if (draw_info.ReasonsForDraw == 2 || draw_info.ReasonsForDraw == 1 || draw_info.ReasonsForDraw == 3) {
        tc_appearance.RefreshPrimitiveAnimationAndPos(PrimHandle, *draw_info.Anim, *draw_info.Pos, *draw_info.Orientation);
        return true;
    }
    if (draw_info.PTextureMorphs == 0 && draw_info.PTextureDecals == 0 && !draw_info.HiddenMaterialsUsed) {
        bool bullet_time_affects_cloth = GetParentThing().IsAffectedByBulletTime();
        CEnginePrimitiveAnimatingMesh mesh(GetParentThing().GetUniqueID(), graphic.BankIndex,
            *draw_info.Anim, *draw_info.Pos, *draw_info.Orientation, size, true, tc_appearance.GetAlpha(), tc_appearance.GetTintColour(),
            fade_distance, (EEngineInterpolationSet)0, tc_appearance.PeekLightingChannel(), bullet_time_affects_cloth);
        mesh.MeshEffect = tc_appearance.GetPMeshEffect();
        tc_appearance.SetPrimitiveGraphics(PrimHandle, mesh, 0);
    } else {
        const std::vector<CEngineTextureMorph>* texture_morphs = draw_info.PTextureMorphs;
        std::vector<CEngineTextureMorph> no_texture_morphs;
        if (texture_morphs == 0)
            texture_morphs = &no_texture_morphs;
        CEnginePrimitiveMorphedAnimatingMesh mesh(GetParentThing().GetUniqueID(), graphic.BankIndex,
            *texture_morphs, *draw_info.HiddenMaterials, *draw_info.Anim, *draw_info.Pos, *draw_info.Orientation, size, true,
            tc_appearance.GetAlpha(), tc_appearance.GetTintColour(), fade_distance, (EEngineInterpolationSet)0,
            tc_appearance.PeekLightingChannel());
        mesh.MeshEffect = tc_appearance.GetPMeshEffect();
        if (draw_info.PTextureDecals)
            mesh.TextureDecals = *draw_info.PTextureDecals;
        tc_appearance.SetPrimitiveGraphics(PrimHandle, mesh, 0);
    }
    return true;
}
}
