// NGraphicAppearance::CTCGraphicAppearance::SetPrimitive(const CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position,
//   const C3DOrientationInterpolator& orientation) -- send the appearance's primitives to the engine: publish the prepare info, build a
//   CGraphicAppearanceDrawInfo (interpolation, position / orientation, the mesh animations, the appearance-morph texture morphs, the texture
//   decals, the hero's hidden material names, the update reasons), let every component draw itself (noting whether any wants a resend every
//   render frame), retry the ones that failed once, and clear the update reasons when every retry succeeded.
// Leaf: ?SetPrimitive@CTCGraphicAppearance@NGraphicAppearance@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BD6E0, 845 bytes, __fastcall `ret 0Ch`, no SEH frame (TU built without /EHsc).
// Identity: Ego_r.pdb (protected, three const references); callers Draw 0x004BE490 / 0x004BE4E0 (landed Draw spells the same signature).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0 (the implicit C3DAnimationInterpolator / C3DAnimationBlendState
//   ctor and dtor expanded in place), CTCAnimationBase::GetMeshAnims (slot 29, through GetAnims 0x004BC000 inlined),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 / non-const 0x0040F020 (CThing::PeekTC / GetTC),
//   CTCAppearanceMorphBase::GetMeshTextureMorphs (slot 29), CTCTextureDecal::PeekEngineDecals 0x0070A570,
//   CTCHeroAttachableAppearanceModifiers::GetHiddenMaterialNames 0x00706860, CAGraphicAppearanceComponent::IsAppearanceSentEveryRenderFrame
//   (slot 4) / OnAppearanceDraw (slot 0), the set<CCharString> tree's _M_erase 0x004ABA70, malloc 0x00BFEA0E / free 0x00BFEA14;
//   the shared data's Delete (slot 1) in the inlined CIVCountedPointer release. Global 0x013B8A18 g_pCurrentPrimitivePrepareInfo (the current prepare info) is written.
// Shape: everything STLport (set ctor/clear/dtor, list ctor/push_back/dtor) is expanded in place except the recursive _M_erase
//   (FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE); the CIVCountedPointer release copies the pointer inside the null test (RELEASE_INNER);
//   HiddenMaterialsUsed is cleared before the reasons are computed (retail stores it at 0x004BD7B7, before the ternary).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_IMPLICIT
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_HIDDENMATERIALS
#include "engine/ETCInterfaceType.h"
#include "engine/CArray.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/C3DAnimationInterpolator.h"
#include "engine/CTCAnimationBase.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"

class CEngineTextureMorph;
class CEngineTextureDecal;
class CEngineTextureMorphSet { public: CArray<CEngineTextureMorph> Morphs; };   // Ego_r +0 CArray<CEngineTextureMorph>
class CTCAppearanceMorphBase : public CTCBase {
public:
    class CMeshMorphSet {
    public:
        class CMeshMorphSetParts;                            // Ego_r ??1CMeshMorphSetParts@CMeshMorphSet@CTCAppearanceMorphBase@@QAE@XZ
        CArray<CMeshMorphSetParts> MeshParts;                // +0x00 (Ego_r CArray<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts>)
        CEngineTextureMorphSet OriginalMeshTextureMorphs;    // +0x0C
    };
protected:
    virtual void DoMorphUpdate();                            // slot 28
public:
    virtual CMeshMorphSet* GetMeshTextureMorphs() const = 0; // slot 29
};
class CTCTextureDecal : public CTCBase {
public:
    const std::vector<CEngineTextureDecal>& PeekEngineDecals();   // 0x0070A570
};
class C3DPositionInterpolator; class C3DOrientationInterpolator;

// 0x013B8A18: the prepare info of the frame being drawn (no PDB name; descriptive, see NAME_FIXES.tsv for the landed readers).
extern const CEnginePrimitivePrepareInfo* g_pCurrentPrimitivePrepareInfo;   // 0x013B8A18

namespace NGraphicAppearance {
class CGraphicAppearanceDrawInfo {
public:
    float Interpolate;
    C3DPositionInterpolator* Pos;
    C3DOrientationInterpolator* Orientation;
    C3DAnimationInterpolator* Anim;
    std::vector<CEngineTextureMorph>* PTextureMorphs;
    std::vector<CEngineTextureDecal>* PTextureDecals;
    std::set<CCharString>* HiddenMaterials;
    unsigned long ReasonsForDraw;
    bool HiddenMaterialsUsed;
    bool TextureMorphsUsed;
};

inline bool CTCGraphicAppearance::GetAnims(C3DAnimationInterpolator& interpolator) const
{
    if (PTCAnimation) {
        PTCAnimation->GetMeshAnims(interpolator.StartState, interpolator.EndState);
        return true;
    }
    return false;
}

void CTCGraphicAppearance::SetPrimitive(const CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position,
                                        const C3DOrientationInterpolator& orientation)
{
    g_pCurrentPrimitivePrepareInfo = &info;
    float interpolate = info.Interpolate.Current.WFInterpolate;
    if (Components.empty())
        return;
    C3DAnimationInterpolator anims;
    GetAnims(anims);
    std::set<CCharString> hidden_materials;
    CGraphicAppearanceDrawInfo draw_info;
    draw_info.Interpolate = interpolate;
    draw_info.Pos = const_cast<C3DPositionInterpolator*>(&position);
    draw_info.Orientation = const_cast<C3DOrientationInterpolator*>(&orientation);
    draw_info.Anim = &anims;
    draw_info.PTextureMorphs = 0;
    draw_info.PTextureDecals = 0;
    draw_info.HiddenMaterials = &hidden_materials;
    draw_info.HiddenMaterialsUsed = false;
    draw_info.ReasonsForDraw = GetUpdatePrimitiveReasons();
    const CTCAppearanceMorphBase* morph;
    if (ParentThing->PeekTC(TCI_APPEARANCE_MORPH, &morph)) {
        CTCAppearanceMorphBase::CMeshMorphSet* set = morph->GetMeshTextureMorphs();
        if (!set->OriginalMeshTextureMorphs.Morphs.empty())
            draw_info.PTextureMorphs = &set->OriginalMeshTextureMorphs.Morphs;
    }
    CTCTextureDecal* decal;
    if (ParentThing->GetTC(TCI_TEXTURE_DECAL, &decal)) {
        const std::vector<CEngineTextureDecal>& decals = decal->PeekEngineDecals();
        if (!decals.empty())
            draw_info.PTextureDecals = const_cast<std::vector<CEngineTextureDecal>*>(&decals);
    }
    const CTCHeroAttachableAppearanceModifiers* modifiers;
    if (ParentThing->PeekTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &modifiers)) {
        modifiers->GetHiddenMaterialNames(hidden_materials);
        if (hidden_materials.size() != 0)
            draw_info.HiddenMaterialsUsed = true;
    }
    bool all_drawn = true;
    std::list<CAGraphicAppearanceComponent*> retry;
    for (std::list<CAGraphicAppearanceComponent*>::iterator it = Components.begin(); it != Components.end(); ++it) {
        CAGraphicAppearanceComponent* component = *it;
        bool sent = component->IsAppearanceSentEveryRenderFrame();
        ResendPrimsEveryRenderFrame = ResendPrimsEveryRenderFrame || sent;
        if (!component->OnAppearanceDraw(*this, draw_info))
            retry.push_back(component);
    }
    for (std::list<CAGraphicAppearanceComponent*>::iterator it2 = retry.begin(); it2 != retry.end(); ++it2)
        if (!(*it2)->OnAppearanceDraw(*this, draw_info))
            all_drawn = false;
    if (all_drawn)
        UpdatePrimitiveReasons = 0;
}
}