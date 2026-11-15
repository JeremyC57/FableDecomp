#ifndef FABLE_ENGINE_CENGINEPRIMITIVEANIMATINGMESH_H
#define FABLE_ENGINE_CENGINEPRIMITIVEANIMATINGMESH_H
// CEnginePrimitiveAnimatingMesh / CEnginePrimitiveMorphedAnimatingMesh (agentC37, row 0x0077B350): the animated-mesh draw primitives the
// graphic-appearance components build on the stack. Layout as the landed row 0x00727D00 models them (Ego_r.pdb members; retail sizes
// 0x90 / 0xB4 with the 12-byte STLport vectors and set). Constructor signatures: reloc_bindings.tsv
// ??0CEnginePrimitiveAnimatingMesh@@QAE@_KKABVC3DAnimationInterpolator@@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@M_NEVCRGBColour@@MW4EEngineInterpolationSet@@ABVCEnginePrimitiveThemeChannelDesc@@4@Z
// (0x007088F0) and ??0CEnginePrimitiveMorphedAnimatingMesh@@QAE@_KKABV?$vector@VCEngineTextureMorph@@..@std@@ABV?$set@VCCharString@@..@2@ABVC3DAnimationInterpolator@@.. (0x00709A50).
// Needs FABLE_RETAIL_CIVCOUNTEDPOINTER and the CEnginePrimitive opt-ins of the including row.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEngineTextureMorph.h"
#include "engine/CEngineTextureDecal.h"
class C3DAnimationInterpolator;
class C3DPositionInterpolator;
class C3DOrientationInterpolator;
class CEnginePrimitiveAnimatingMesh : public CEnginePrimitive {
public:
    CEnginePrimitiveAnimatingMesh(unsigned __int64 id, unsigned long mesh, const C3DAnimationInterpolator& anim,
        const C3DPositionInterpolator& pos, const C3DOrientationInterpolator& orientation, float size, bool lit, unsigned char alpha,
        CRGBColour tint, float fade, EEngineInterpolationSet interpolation_set, const CEnginePrimitiveThemeChannelDesc& theme,
        bool bullet_time_affects_cloth);                    // 0x007088F0
    ~CEnginePrimitiveAnimatingMesh();                       // 0x00708A30
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
        const C3DOrientationInterpolator& orientation, float size, bool lit, unsigned char alpha, CRGBColour tint,
        float fade, EEngineInterpolationSet interpolation_set, const CEnginePrimitiveThemeChannelDesc& theme);   // 0x00709A50
    ~CEnginePrimitiveMorphedAnimatingMesh();                // 0x00709B10
    std::vector<CEngineTextureMorph> TextureMorphs;         // +0x90
    std::vector<CEngineTextureDecal> TextureDecals;         // +0x9C
    std::set<CCharString> HiddenMaterialNames;              // +0xA8
};
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveAnimatingMesh, MeshEffect) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveMorphedAnimatingMesh, TextureDecals) == 0x9c);
#endif // FABLE_ENGINE_CENGINEPRIMITIVEANIMATINGMESH_H
