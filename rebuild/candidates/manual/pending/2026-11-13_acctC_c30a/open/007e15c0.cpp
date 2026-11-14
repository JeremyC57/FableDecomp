// NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) -- draw every mesh of the multi-static-mesh def as a static mesh primitive
// Grade: DIFFER(969v963) -- `this` spilled at entry (retail keeps it in ebp all through and spills the loop-1 index/offset instead); channel desc 3 byte stores v retail 2; position FP order
// Leaf: ?OnAppearanceDraw@CTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@UAE_NAAVCTCGraphicAppearance@2@ABVCGraphicAppearanceDrawInfo@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E15C0, 963 bytes, thiscall on the CAGraphicAppearanceComponent subobject (+0x0C), `ret 8`.
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
    unsigned char Lit;                                          // +0x49
    bool BulletTimeAffectsCloth;                                // +0x4A
};
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
            CPrimitive* primitive = tc_appearance.AddPersistentPrimitive(CPrimitive((EPrimitiveType)1, PDef->Meshes[i].Mesh));
            PrimHandles.push_back(primitive);
        }
    } else if (draw_info.ReasonsForDraw == 1) {
        for (unsigned long i = 0; i < PrimHandles.size(); ++i)
            tc_appearance.RefreshPrimitivePos(PrimHandles[i], *draw_info.Pos, *draw_info.Orientation);
        return true;
    }
    for (i = 0; i < PrimHandles.size(); ++i) {
        const CMultiStaticMeshEntryDef& entry = PDef->Meshes[i];
        if (entry.Mesh == 0)
            continue;
        CEnginePrimitiveThemeChannelDesc channel;
        if (entry.OverrideLightingChannel)
            channel.ChannelIndex0 = (unsigned char)entry.LightingChannel;
        else
            channel = tc_appearance.PeekLightingChannel();
        if (g_LightingChannelDrawFilter >= 0 && channel.ChannelIndex0 != 0)
            continue;
        float fade = fade_distance;
        if (entry.OverrideRenderFadeDistance)
            fade = entry.RenderFadeDistance;
        bool bullet_time_affects_cloth = GetParentThing().IsAffectedByBulletTime();
        CEnginePrimitiveStaticMesh mesh(GetParentThing().GetUniqueID(), entry.Mesh,
            draw_info.Pos->GetInterpolated(draw_info.Interpolate), draw_info.Orientation->GetInterpolated(draw_info.Interpolate),
            size, true, tc_appearance.GetAlpha(), tc_appearance.GetTintColour(), fade, (EEngineInterpolationSet)0, channel,
            bullet_time_affects_cloth);
        mesh.MeshEffect = tc_appearance.GetPMeshEffect();
        tc_appearance.SetPrimitiveGraphics(PrimHandles[i], mesh, 0);
    }
    return true;
}
}
