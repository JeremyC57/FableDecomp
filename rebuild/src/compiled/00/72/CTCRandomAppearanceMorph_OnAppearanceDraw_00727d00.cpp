// CTCRandomAppearanceMorph::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) -- per-frame draw of a random
// appearance morph: on the first draw add one persistent primitive per morphed mesh part; afterwards, when only the animation / position
// changed (ReasonsForDraw 1..3) refresh the primitive's interpolators, otherwise rebuild a CEnginePrimitiveMorphedAnimatingMesh for the
// part (owner id, mesh, texture morphs, an empty hidden-material set, the draw interpolators, scale, alpha, tint, fade distance,
// lighting channel), give it the appearance's mesh effect and hand it to SetPrimitiveGraphics.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00727D00, 760 bytes, thiscall `ret 8`; vtable 0x01266520 slot 0 (the
//   NGraphicAppearance::CAGraphicAppearanceComponent side at +0x0C, so ParentThing is read as [this-8]).
// Leaf: ?OnAppearanceDraw@CTCRandomAppearanceMorph@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z
// Callees: CTCGraphicAppearance::GetFullScale 0x004BBFF0, GetFadeDistance 0x004BC180, operator new / free 0x00BFEA0E / 0x00BFEA14 (the
//   inline set header), vector<CPrimitive*>::_M_fill_insert 0x00642440, memmove 0x00BFEAE6 (inline resize), CBaseClass::CBaseClass
//   0x0099A2D0 (inline CPrimitive ctor), AddPersistentPrimitive 0x004C0050, CPrimitive::~CPrimitive 0x004C4160, GetTintColour 0x004BD040,
//   CEnginePrimitiveMorphedAnimatingMesh ctor 0x00709A50 / dtor 0x00709B10, GetPMeshEffect 0x004BCF50, SetPrimitiveGraphics 0x004BD620,
//   RefreshPrimitiveAnimationAndPos 0x004BCFE0, _Rb_tree<CCharString>::_M_erase 0x004ABA70.
// Shape: the part index is advanced explicitly at the end of each path (the `continue` path and after SetPrimitiveGraphics), which gives
//   retail's two strength-reduced counters; the mesh effect is assigned through CIVCountedPointer's inline operator= whose release loads the
//   count before copying the pointer into ecx (FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER + CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES).
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
class CEngineTextureMorph;
class CEngineTextureDecal;
// No shared header: CGraphicAppearanceDrawInfo (Ego_r 36 bytes), CTCAppearanceMorphBase, CTCRandomAppearanceMorph,
// CEnginePrimitiveAnimatingMesh (0x90) and CEnginePrimitiveMorphedAnimatingMesh (0xB4 in retail: 12-byte STLport vectors / set).
namespace NGraphicAppearance {
class CGraphicAppearanceDrawInfo {
public:
    float Interpolate;                                      // +0x00
    C3DPositionInterpolator* Pos;                           // +0x04
    C3DOrientationInterpolator* Orientation;                // +0x08
    C3DAnimationInterpolator* Anim;                         // +0x0C
    std::vector<CEngineTextureMorph>* PTextureMorphs;       // +0x10
    std::vector<CEngineTextureDecal>* PTextureDecals;       // +0x14
    std::set<CCharString>* HiddenMaterials;                 // +0x18
    unsigned long ReasonsForDraw;                           // +0x1C
    bool HiddenMaterialsUsed;                               // +0x20
    bool TextureMorphsUsed;                                 // +0x21
};
}
class CEnginePrimitiveAnimatingMesh : public CEnginePrimitive {
public:
    unsigned long MeshIndex;                                // +0x0C
    float Size;                                             // +0x10
    float FadeStart;                                        // +0x14
    unsigned char Alpha;                                    // +0x18
    unsigned char Lit;                                      // +0x19 (bitfield)
    unsigned char WorldPos[0x1C];                           // +0x1C C3DPositionInterpolator
    unsigned char Orientation[0x34];                        // +0x38 C3DOrientationInterpolator
    unsigned char Anim[0x14];                               // +0x6C C3DAnimationInterpolator
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;        // +0x80
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;      // +0x84
    CRGBColour TintColour;                                  // +0x88
    bool BulletTimeAffectsCloth;                            // +0x8C
};
class CEnginePrimitiveMorphedAnimatingMesh : public CEnginePrimitiveAnimatingMesh {
public:
    CEnginePrimitiveMorphedAnimatingMesh(unsigned __int64 id, unsigned long mesh, const std::vector<CEngineTextureMorph>& texture_morphs,
        const std::set<CCharString>& hidden_materials, const C3DAnimationInterpolator& anim, const C3DPositionInterpolator& pos,
        const C3DOrientationInterpolator& orientation, float scale, bool dynamic, unsigned char alpha, CRGBColour colour,
        float draw_distance, EEngineInterpolationSet interpolation_set, const CEnginePrimitiveThemeChannelDesc& theme);   // 0x00709A50
    ~CEnginePrimitiveMorphedAnimatingMesh();                // 0x00709B10
    std::vector<CEngineTextureMorph> TextureMorphs;         // +0x90
    std::vector<CEngineTextureDecal> TextureDecals;         // +0x9C
    std::set<CCharString> HiddenMaterialNames;              // +0xA8
};
// Ego_r.pdb (struct_layouts_egor.tsv): CEngineTextureMorphSet { CArray<CEngineTextureMorph> Morphs +0 }, and CMeshMorphSetParts is
// nested in CTCAppearanceMorphBase::CMeshMorphSet (CArray<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts> MeshParts),
// as the landed sibling GetMeshesUsed 0x00727C20 models them. No shared header for these classes.
class CEngineTextureMorphSet {
public:
    CArray<CEngineTextureMorph> Morphs;                     // +0x00
};
class CTCAppearanceMorphBase : public CTCBase {
public:
    class CMeshMorphSet {
    public:
        class CMeshMorphSetParts {
        public:
            unsigned long MeshIndex;                        // +0x00
            CEngineTextureMorphSet TextureMorphs;           // +0x04
        };
        CArray<CMeshMorphSetParts> MeshParts;               // +0x00
    };
};
class CTCRandomAppearanceMorph : public CTCAppearanceMorphBase, public NGraphicAppearance::CAGraphicAppearanceComponent {
public:
    CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet;   // +0x10
    unsigned long Seed;                                     // +0x18
    std::vector<NGraphicAppearance::CPrimitive*> PrimHandles;   // +0x1C
    virtual bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& tc_appearance, const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info);
    virtual void OnAppearanceClear();
};

bool CTCRandomAppearanceMorph::OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& tc_appearance, const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info)
{
    float scale = tc_appearance.GetFullScale();
    float fade_distance = tc_appearance.GetFadeDistance();
    bool first_draw = PrimHandles.empty();
    std::set<CCharString> hidden_materials;
    unsigned long i = 0;
    for (CArray<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts>::iterator it = PMeshMorphSet->MeshParts.begin(); it != PMeshMorphSet->MeshParts.end(); ++it) {
        if (first_draw) {
            PrimHandles.resize(i + 1);
            PrimHandles[i] = tc_appearance.AddPersistentPrimitive(NGraphicAppearance::CPrimitive((NGraphicAppearance::EPrimitiveType)2, it->MeshIndex));
        } else if (draw_info.ReasonsForDraw == 2 || draw_info.ReasonsForDraw == 1 || draw_info.ReasonsForDraw == 3) {
            tc_appearance.RefreshPrimitiveAnimationAndPos(PrimHandles[i], *draw_info.Anim, *draw_info.Pos, *draw_info.Orientation);
            ++i;
            continue;
        }
        CEnginePrimitiveMorphedAnimatingMesh mesh(GetParentThing().GetUniqueID(), it->MeshIndex, it->TextureMorphs.Morphs, hidden_materials,
            *draw_info.Anim, *draw_info.Pos, *draw_info.Orientation, scale, true, tc_appearance.GetAlpha(), tc_appearance.GetTintColour(),
            fade_distance, (EEngineInterpolationSet)0, tc_appearance.PeekLightingChannel());
        mesh.MeshEffect = tc_appearance.GetPMeshEffect();
        tc_appearance.SetPrimitiveGraphics(PrimHandles[i], mesh, 0);
        ++i;
    }
    return true;
}