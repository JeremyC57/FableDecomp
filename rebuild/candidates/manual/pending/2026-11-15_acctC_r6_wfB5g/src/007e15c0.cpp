// NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) -- draw every mesh of the multi-static-mesh def as a static mesh primitive
// Leaf: ?OnAppearanceDraw@CTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E15C0, 963 bytes, thiscall on the CAGraphicAppearanceComponent subobject (+0x0C), `ret 8`.
// On first draw one persistent static-mesh primitive is added per def entry; a position-only redraw refreshes their positions; otherwise
// every entry with a mesh gets its lighting channel (the entry's override index, else the appearance's channel), entries on a non-zero
// channel are skipped while the debug draw filter is >= 0, and the rest are rebuilt as CEnginePrimitiveStaticMesh packets (fade distance
// overridable per entry, bullet-time flag of the parent thing, the appearance's mesh effect) and set as the primitive's graphics.
// EVIDENCED: CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh (0x0077BD70) is a header-inline COMDAT in retail (it sits between the
//   CTCGraphicAppearanceStaticMesh functions 0x0077BC80..0x0077BD20 and ..FadeOutOnProximity 0x0077BE30), so its body is visible in this TU;
//   it is defined `inline` below with the landed row's initializer list (Lit a 1-bit field: retail `xor / and 1 / xor` 0x0077BE01; id unused).
//   With the body visible the position temporary is not address-exposed (retail x87 order 0x007E17E6..0x007E1848) and the default
//   channel ctor's index-0 store is dead (retail stores +1 / +2 only, 0x007E171B / 0x007E1720).
// EVIDENCED: the filter test guards the drawing block (`if (filter < 0 || index0 == 0) { ... }`) and every entry access is spelled
//   `PDef->Meshes[i]`: retail re-reads Meshes._M_start after the channel block (0x007E17A1, PDef.Object kept in ecx) and sinks the fade
//   store below it (0x007E17A4); the `continue` form or an entry reference keeps the entry pointer in a register (960 / 961 bytes).
//   (This block form was found by grading; the 0x007E15C0 notes list the ~250 grades of the earlier forms.)
// Names: g_LightingChannelDrawFilter (0x013756F0, .data, initialised -1, read only here and at 0x0077BAC7) is DESCRIPTIVE (no symbol
//   in Ego_r / FableWin / Anniversary PDBs, not in reloc_bindings).
#define FABLE_RETAIL_CENGINEPRIMITIVETHEMECHANNELDESC_INDICES
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMULTISTATICMESHENTRYDEF
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_INTERPOLATE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_REFRESHPRIMITIVEPOS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/NGraphicAppearance_CPrimitive.h"
#include "engine/C3DPositionInterpolator.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CMultiStaticMeshEntryDef.h"
#include "engine/CDefPointer.h"
#include "engine/CArray.h"

class CMultiStaticMeshDef : public CDefClassBase {
public:
    std::vector<CMultiStaticMeshEntryDef> Meshes;               // +0x28
};
class CEnginePrimitiveStaticMesh : public CEnginePrimitive {
public:
    CEnginePrimitiveStaticMesh(unsigned __int64 id, unsigned long mesh, const C3DVector& pos, const CRightHandedSet& rhs, float size,
        bool lit, unsigned char alpha, CRGBColour tint, float fade, EEngineInterpolationSet interpolation_set,
        const CEnginePrimitiveThemeChannelDesc& theme, bool bullet_time_affects_cloth);   // 0x0077BD70
    unsigned long MeshIndex;                                    // +0x0C
    C3DVector WorldPos;                                         // +0x10
    float Size;                                                 // +0x1C
    float FadeStart;                                            // +0x20
    CRightHandedSet RightHandedSet;                             // +0x24
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;            // +0x3C
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;          // +0x40
    CRGBColour TintColour;                                      // +0x44
    unsigned char Alpha;                                        // +0x48
    unsigned char Lit : 1;                                      // +0x49 bit 0 (retail ctor 0x0077BE01: xor / and 1 / xor)
    unsigned char Reserved : 7;
    bool BulletTimeAffectsCloth;                                // +0x4A
};
inline CEnginePrimitiveStaticMesh::CEnginePrimitiveStaticMesh(unsigned __int64 id, unsigned long mesh, const C3DVector& pos, const CRightHandedSet& rhs,
    float size, bool lit, unsigned char alpha, CRGBColour tint, float fade, EEngineInterpolationSet interpolation_set,
    const CEnginePrimitiveThemeChannelDesc& theme, bool bullet_time_affects_cloth)
    : CEnginePrimitive((EEnginePrimitiveType)8, id, interpolation_set), MeshIndex(mesh), WorldPos(pos), Size(size), FadeStart(fade), RightHandedSet(rhs),
      MeshEffect(), ThemeChannelDesc(theme), TintColour(tint), Alpha(alpha), Lit(lit), BulletTimeAffectsCloth(bullet_time_affects_cloth)
{
}
extern long g_LightingChannelDrawFilter;                        // 0x013756F0 (descriptive)

namespace NGraphicAppearance {
class CTCGraphicAppearanceMultipleStaticMeshes : public CTCBase, public CAGraphicAppearanceComponent {
public:
    std::vector<CPrimitive*> PrimHandles;                       // +0x10
    CDefPointer<const CMultiStaticMeshDef> PDef;                // +0x1C
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& tc_appearance, const CGraphicAppearanceDrawInfo& draw_info);
    virtual void OnAppearanceClear();
};

bool CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceDraw(CTCGraphicAppearance& tc_appearance, const CGraphicAppearanceDrawInfo& draw_info)
{
    float size = tc_appearance.GetMainGraphic().RenderSizeX * tc_appearance.GetScale();
    float fade_distance = tc_appearance.GetFadeDistance();
    unsigned long i;
    if (PrimHandles.empty()) {
        for (i = 0; i < PDef->Meshes.size(); ++i)
        {
            PrimHandles.push_back(tc_appearance.AddPersistentPrimitive(CPrimitive((EPrimitiveType)1, PDef->Meshes[i].Mesh)));
        }
    } else if (draw_info.ReasonsForDraw == 1) {
        for (unsigned long i = 0; i < PrimHandles.size(); ++i)
            tc_appearance.RefreshPrimitivePos(PrimHandles[i], *draw_info.Pos, *draw_info.Orientation);
        return true;
    }
    for (i = 0; i < PrimHandles.size(); ++i) {
        if (PDef->Meshes[i].Mesh == 0)
            continue;
        CEnginePrimitiveThemeChannelDesc channel;
        if (PDef->Meshes[i].OverrideLightingChannel)
            channel.ChannelIndex0 = (unsigned char)PDef->Meshes[i].LightingChannel;
        else
            channel = tc_appearance.PeekLightingChannel();
        if (g_LightingChannelDrawFilter < 0 || channel.ChannelIndex0 == 0) {
            float fade = fade_distance;
            if (PDef->Meshes[i].OverrideRenderFadeDistance)
                fade = PDef->Meshes[i].RenderFadeDistance;
            bool bullet_time_affects_cloth = GetParentThing().IsAffectedByBulletTime();
            CEnginePrimitiveStaticMesh mesh(GetParentThing().GetUniqueID(), PDef->Meshes[i].Mesh,
                draw_info.Pos->GetInterpolated(draw_info.Interpolate), draw_info.Orientation->GetInterpolated(draw_info.Interpolate),
                size, true, tc_appearance.GetAlpha(), tc_appearance.GetTintColour(), fade, (EEngineInterpolationSet)0, channel,
                bullet_time_affects_cloth);
            mesh.MeshEffect = tc_appearance.GetPMeshEffect();
            tc_appearance.SetPrimitiveGraphics(PrimHandles[i], mesh, 0);
        }
    }
    return true;
}
}
