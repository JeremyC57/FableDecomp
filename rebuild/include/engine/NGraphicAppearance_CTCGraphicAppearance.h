#ifndef FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCE_H
#define FABLE_ENGINE_NGRAPHICAPPEARANCE_CTCGRAPHICAPPEARANCE_H
// New shared header (chunk 732, row 0x006AA2E0 CThingPlayerCreature::DrawPreparePrimitivesForRendering): the retail
// NGraphicAppearance::CTCGraphicAppearance over FABLE_RETAIL_CTCBASE (RTTI .?AVCTCGraphicAppearance@NGraphicAppearance@@ : CTCBase@0,
// ghidra_out/rtti_bases.tsv). Members are not modelled; only the out-of-line accessor the rows call:
// Ego_r.pdb / FableWin.pdb ?GetFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z (retail 0x004C1060; the
// float is the CEnginePrimitivePrepareInfo interpolation time, +0x04).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#endif
// Retail opt-in addition (chunk 750, rows 0x0077B290 / 0x0077B9F0 / 0x0077BE80 / 0x0077C440, the appearance-component destructors):
// define FABLE_RETAIL_CTCGRAPHICAPPEARANCE for the public non-virtual UnregisterComponent 0x004C0000 (the only spelling bound there,
// ?UnregisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXAAVCAGraphicAppearanceComponent@2@@Z, Ego_r.pdb / FableWin.pdb;
// every component destructor passes its CAGraphicAppearanceComponent subobject, `push edi` = this+0xC). A declaration only.
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT
class CCharString; class CRGBColour;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
class CCharString; class CRightHandedSet;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CENGINEGRAPHIC
#include "engine/_quarantine/CTCGraphicAppearance.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
class CTCAnimationBase;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
#include "engine/NGraphicAppearance_CMeshInfo.h"
class C3DAnimationInterpolator;class C3DMeshObject;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
class CCharString;
#endif
namespace NGraphicAppearance {
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
class CPrimitive; class CAGraphicAppearanceComponent;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE
class CAGraphicAppearanceComponent;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT
enum EAppearanceMeshEffectPriority;                          // MS forward enum (chunk 867: the row passes the value 3)
#endif
class CTCGraphicAppearance : public CTCBase {
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
protected:
 class CMeshEffectPriorityEntry;
 std::map<void*,CMeshEffectPriorityEntry> MeshEffects;
 std::map<void*,CRGBColour> TintColours;
 CTCAnimationBase* PTCAnimation;
 C3DVector CachedStaticFocalPosOffset;
 float Scale;
 std::list<CPrimitive> Primitives;
 std::list<CAGraphicAppearanceComponent*> Components;
 CEnginePrimitiveThemeChannelDesc LightingChannel;
 CEngineGraphic MainGraphic;
 unsigned short ActivePersistenceFlags;
 long FrameUpdatedWasRenderedFlag;
 unsigned char UpdatePrimitiveReasons;
 unsigned char SelfIlluminating:1,ForceResendEveryRenderFrame:1,ResendPrimsEveryRenderFrame:1,DisableOcclusionTesting:1,DrawAfterWater:1,CachedStaticFocalPos:1,RenderedLastFrame:1,PrimitivesDrawn:1;
 unsigned char Alpha;
 signed char ByPassColorFilter;
public:
 const CEngineGraphic& GetMainGraphic()const{return MainGraphic;}
 float GetScale()const{return Scale;}
#endif
public:
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
    // Retail opt-in (Codex pass17): both original PDBs; native004c0560 signature.
    bool GetHelperPoint(const CCharString&,long,unsigned long,C3DVector&,float) const;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
    // Retail opt-in (Codex pass17): original methods/access/signatures, both PDBs and debug full helper.
    void GetAllMeshes(std::vector<CMeshInfo>&)const;
protected:
    bool IsToReduceLODForDummiesAndHelpers()const;
    bool IsResourcesLoadedForMesh(const CMeshInfo&,bool)const;
    void GetMeshObject(const CMeshInfo&,float,C3DMeshObject&)const;
    bool GetAnims(C3DAnimationInterpolator&)const;
public:
#endif
    C3DVector GetFocalPos(float time) const;                 // 0x004C1060
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT
    // Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): FableWin.pdb
    // ?SetAsHighlighted@CTCGraphicAppearance@NGraphicAppearance@@QAEXMMABVCRGBColour@@_N1W4EAppearanceMeshEffectPriority@2@PAX@Z (out of
    // line at 0x004C04C0: the two widths, the colour by reference, two bools, the priority and the owner key pushed) and
    // ?ClearMeshEffect@..@@QAEXPAX@Z (0x004BFFC0, the owner key). Declarations only; no layout change.
    void SetAsHighlighted(float widthWorldSpace, float widthScreenSpace, const CRGBColour& colour, bool, bool,
                          EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C04C0
    void ClearMeshEffect(void* owner);                        // 0x004BFFC0
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE
    void UnregisterComponent(CAGraphicAppearanceComponent& component);   // 0x004C0000
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SCALE
    // Nested opt-in (chunk 802, row 0x00974AA0 CAIStateGroup_JackOfBladesStage2::Update): Ego_r.pdb
    // ?GetScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ (inline in retail: `fld dword ptr [eax+34h]`, Scale at retail +0x34, as
    // the LIGHTINGCHANNEL note above) and ?SetScale@..@@QAEXM@Z (out of line at 0x004BFA50). The layout stays opaque: GetScale reads
    // the +0x34 word in place. No layout change.
    float GetScale() const { return *reinterpret_cast<const float*>(reinterpret_cast<const unsigned char*>(this) + 0x34); }
    void SetScale(float scale);                              // 0x004BFA50
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETALPHA
    // Nested opt-in (chunk 768, row 0x0089DA70 CGameScriptInterface::EntitySetAlpha): Ego_r.pdb / FableWin.pdb
    // ?SetAlpha@CTCGraphicAppearance@NGraphicAppearance@@QAEXE@Z (the bound spelling at 0x004BFAC0). Declaration only.
    void SetAlpha(unsigned char alpha);                      // 0x004BFAC0
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
    // Retail opt-in (chunk 782, row 0x007941D0 CTCUnholyPower ctor): FableWin.pdb ?PeekLightingChannel@CTCGraphicAppearance@NGraphicAppearance@@QBEABVCEnginePrimitiveThemeChannelDesc@@XZ
    // (inline in retail: `mov edx,[ecx+40h]`) and Ego_r.pdb ?SetLightingChannel@..@@QAEXABVCEnginePrimitiveThemeChannelDesc@@_N1@Z
    // (out of line, 0x004BFB40). LightingChannel is Ego_r's member at retail +0x40: MeshEffects / TintColours (two 0xC STLport maps),
    // PTCAnimation +0x24, CachedStaticFocalPosOffset +0x28, Scale +0x34, Primitives / Components (two 4-byte STLport lists) +0x38 / +0x3C
    // precede it and are kept opaque here.
    const CEnginePrimitiveThemeChannelDesc& PeekLightingChannel() const { return LightingChannel; }
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool update_primitives, bool update_children);   // 0x004BFB40
private:
    unsigned char LayoutMeshEffectsToComponents[0x40 - 0x0C];       // +0x0C .. +0x3F (opaque)
    CEnginePrimitiveThemeChannelDesc LightingChannel;               // +0x40
public:
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT
    // Retail opt-in (chunk 794, row 0x0079ACC0 CTCSummonSpell::CreateMinion): Ego_r.pdb
    // ?HasDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@@Z (retail 0x004BE2C0) and
    // ?SetTintColour@CTCGraphicAppearance@NGraphicAppearance@@QAEXABVCRGBColour@@PAX@Z (retail 0x004C00B0), both out of line.
    // Declarations only: no layout change.
    bool HasDummyObject(const CCharString& name) const;      // 0x004BE2C0
    void SetTintColour(const CRGBColour& colour, void* owner);   // 0x004C00B0
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
    // Retail opt-in (chunk 794, row 0x007DE710 CTCWallMount::AttachObject): Ego_r.pdb
    // ?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z, out of line in
    // retail (0x004C08C0; catalog name agrees; the row pushes 1.0f, the set, the position, 0, 0, the name). Declaration only.
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& orientation,
                        float scale) const;                  // 0x004C08C0
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCALPOS
    // Nested opt-in (chunk 768, row 0x00753380 CTCLook::FrameUpdateHeadInterested): Ego_r.pdb / FableWin.pdb
    // ?GetFocalPosIfLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_NAAVC3DVector@@M@Z (out of line, retail 0x004C11D0). Declaration only.
    bool GetFocalPosIfLoaded(C3DVector& pos, float time) const;   // 0x004C11D0
#endif
};
}
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CTCGraphicAppearance)==0x5c);
#endif
#endif
