#ifndef FABLE_ENGINE_NGRAPHICAPPEARANCE_CGRAPHICAPPEARANCEDRAWINFO_H
#define FABLE_ENGINE_NGRAPHICAPPEARANCE_CGRAPHICAPPEARANCEDRAWINFO_H
// New header (chunk 2020, row 0x006414A0 CTCLight::DrawLight): NGraphicAppearance::CGraphicAppearanceDrawInfo, the per-draw record the
// graphic appearance hands to its components (CAGraphicAppearanceComponent::OnAppearanceDraw / CTCLight::DrawLight / DrawFlare take it by
// const reference; the mangled names spell it ABVCGraphicAppearanceDrawInfo@NGraphicAppearance@@, a class). Layout: Ego_r.pdb
// (struct_layouts_egor.tsv, 36 bytes; only pointers and scalars, so the retail offsets are the PDB's): Interpolate +0x00 (DrawLight reads it
// twice at 0x00641594 / 0x0064159C to blend the two flicker factors), Pos +0x04, Orientation +0x08, Anim +0x0C, PTextureMorphs +0x10,
// PTextureDecals +0x14, HiddenMaterials +0x18, ReasonsForDraw +0x1C, HiddenMaterialsUsed +0x20, TextureMorphsUsed +0x21.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
class C3DPositionInterpolator; class C3DOrientationInterpolator; class C3DAnimationInterpolator;
class CEngineTextureMorph; class CEngineTextureDecal;
namespace NGraphicAppearance {
class CGraphicAppearanceDrawInfo {
public:
    float                                Interpolate;            // +0x00
    C3DPositionInterpolator*             Pos;                    // +0x04
    C3DOrientationInterpolator*          Orientation;            // +0x08
    C3DAnimationInterpolator*            Anim;                   // +0x0C
    std::vector<CEngineTextureMorph>*    PTextureMorphs;         // +0x10
    std::vector<CEngineTextureDecal>*    PTextureDecals;         // +0x14
    std::set<CCharString>*               HiddenMaterials;        // +0x18
    unsigned long                        ReasonsForDraw;         // +0x1C
    bool                                 HiddenMaterialsUsed;    // +0x20
    bool                                 TextureMorphsUsed;      // +0x21
};
}
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CGraphicAppearanceDrawInfo) == 36);
#endif
