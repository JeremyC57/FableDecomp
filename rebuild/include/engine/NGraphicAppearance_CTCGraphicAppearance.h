// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
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
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#include "engine/NGraphicAppearance_CPrimitive.h"
class CEnginePrimitive;
#endif
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
#if defined(FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC) || defined(FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE)
// agentNF9 (SETMAINGRAPHIC) / Codex pass 20 (CTCWEAPONTRAIL_UPDATE): the members use the shared engine/CEngineGraphic.h (same 14-byte
// packed layout) instead of the quarantine header's copy, so a row can include both this header and a def header that embeds CEngineGraphic.
#include "engine/CEngineGraphic.h"
#else
#define FABLE_RETAIL_CENGINEGRAPHIC
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#include "engine/CEngineGraphic.h"
#else
#include "engine/_quarantine/CTCGraphicAppearance.h"
#endif
#endif
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
class CTCAnimationBase;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETPRIMITIVE
class CEnginePrimitivePrepareInfo; class C3DPositionInterpolator; class C3DOrientationInterpolator;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
#include "engine/NGraphicAppearance_CMeshInfo.h"
class C3DAnimationInterpolator;class C3DMeshObject;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
class CCharString;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MESHEFFECT
#include "engine/CIVCountedPointer.h"
class CEngineMeshEffect;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#include "engine/CIVCountedPointer.h"
class CEngineMeshEffect; class C3DAnimationInterpolator; class C3DPositionInterpolator; class C3DOrientationInterpolator;
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
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
public:
    // Both original PDBs and native004c11f0: const owner, output reference.
    bool GetStaticFocalPosIfLoaded(C3DVector&) const;
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    C3DVector GetCameraFocalPos(float)const;
    C3DVector GetStaticCameraFocalPos()const{return GetCameraFocalPos(1.0f);}
#endif
#endif
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
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETPRIMITIVE
 // Opt-in (row 0x004BD6E0): Ego_r.pdb ?SetPrimitive@CTCGraphicAppearance@NGraphicAppearance@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@@Z
 // (protected, out of line at 0x004BD6E0) and the protected inline GetUpdatePrimitiveReasons() const (0x004BD7BC: `mov eax,4` when no
 // reason is set and ResendPrimsEveryRenderFrame is). Declarations only: no layout change.
protected:
 unsigned long GetUpdatePrimitiveReasons()const{if(!UpdatePrimitiveReasons&&ResendPrimsEveryRenderFrame)return 4;return UpdatePrimitiveReasons;}
 void SetPrimitive(const CEnginePrimitivePrepareInfo&,const C3DPositionInterpolator&,const C3DOrientationInterpolator&);
public:
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETFORCERESEND
 // Overlay opt-in (row 0x007BF790 CTCShotDefinitionBase::Init): FableWin.pdb ?SetForceResendEveryRenderFrame@CTCGraphicAppearance@NGraphicAppearance@@QAEX_N@Z
 // (0x017DD73C), inline in retail (`or byte ptr [eax+59h],2`, 0x007BF9FE).
 void SetForceResendEveryRenderFrame(bool force){ForceResendEveryRenderFrame=force;}
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_BYPASSCOLORFILTER
 // Row 0x00881A40 opt-in: Ego_r.pdb / FableWin.pdb ?GetByPassColorFilter@CTCGraphicAppearance@NGraphicAppearance@@QBE_NXZ (inline in
 // retail: `mov al,[ebp+5Bh] ; test al,al ; setg cl`, 0x00881BF3) and ?SetByPassColorFilter@..@@QAEX_N@Z (out of line 0x004BFB00).
 bool GetByPassColorFilter()const{return ByPassColorFilter>0;}
 void SetByPassColorFilter(bool bypass);                    // 0x004BFB00
#endif
 float GetScale()const{return Scale;}
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
 // Nested opt-in (agentNF9, row 0x0082D6F0 CThingBuilding::OnCreate): Ego_r.pdb / FableWin.pdb
 // ?SetMainGraphic@CTCGraphicAppearance@NGraphicAppearance@@QAEXABVCEngineGraphic@@@Z, inline in retail (the 14-byte copy into +0x44,
 // `lea eax,[ebx+44h]` then three dwords and a word, 0x0082D72A), and ?SetScale@..@@QAEXM@Z (out of line at 0x004BFA50; declared
 // here unless FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SCALE already does). No layout change.
 void SetMainGraphic(const CEngineGraphic& graphic){MainGraphic=graphic;}
#ifndef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SCALE
 void SetScale(float scale);                                // 0x004BFA50
#endif
#endif
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
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
    // Retail opt-in (Codex pass20): both PDBs and donor0222b060 call sequence.
    float GetFadeDistance() const;
    CPrimitive* AddPersistentPrimitive(const CPrimitive&);
    void SetPrimitiveGraphics(CPrimitive*,const CEnginePrimitive&,unsigned short);
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
    // Nested opt-in (row 0x00727D00 CTCRandomAppearanceMorph::OnAppearanceDraw; needs _MOVEMENT and _FLARE; GetFullScale 0x004BBFF0
    // comes from _FULLSCALE): Ego_r.pdb ?GetAlpha@..@@QBEEXZ (inline in retail: `mov dl,[esi+5Ah]`), ?GetTintColour@..@@QBE?BVCRGBColour@@XZ
    // (0x004BD040), ?GetPMeshEffect@..@@QAE?AV?$CIVCountedPointer@VCEngineMeshEffect@@@@XZ (0x004BCF50) and
    // ?RefreshPrimitiveAnimationAndPos@..@@QAEXPAVCPrimitive@2@ABVC3DAnimationInterpolator@@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@@Z
    // (0x004BCFE0), all out of line except GetAlpha. Declarations only: no layout change.
    unsigned char GetAlpha() const { return Alpha; }
#ifndef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
    const CEnginePrimitiveThemeChannelDesc& PeekLightingChannel() const { return LightingChannel; }   // FableWin.pdb, inline
#endif
    const CRGBColour GetTintColour() const;                      // 0x004BD040
    CIVCountedPointer<CEngineMeshEffect> GetPMeshEffect();       // 0x004BCF50
    void RefreshPrimitiveAnimationAndPos(CPrimitive* primitive, const C3DAnimationInterpolator& anim, const C3DPositionInterpolator& pos,
                                         const C3DOrientationInterpolator& orientation);   // 0x004BCFE0
#endif
    C3DVector GetFocalPos(float time) const;                 // 0x004C1060
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES
    // Retail opt-in (chunk Q3, row 0x0076EB00 CTCWeaponTrail::InitialiseTrails): Ego_r.pdb / FableWin.pdb spellings of the four
    // out-of-line helper-point queries the row calls (declarations only, no layout change):
    // ?IsResourcesLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_N_N@Z 0x004BE3E0,
    // ?HasHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@@Z 0x004BE170,
    // ?GetAllHelperPointNames@...@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z 0x004BDA30,
    // ?GetNoHelperPoints@CTCGraphicAppearance@NGraphicAppearance@@QBEJABVCCharString@@M@Z 0x004BDE90.
    bool IsResourcesLoaded(bool) const;
    bool HasHelperPoint(const CCharString&) const;
    void GetAllHelperPointNames(std::vector<CCharString>&) const;
    long GetNoHelperPoints(const CCharString&, float) const;
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_WASRENDERED
    // Opt-in (chunk 1024, row 0x006E2ED0): Ego_r.pdb / FableWin.pdb ?WasRenderedLastFrame@CTCGraphicAppearance@NGraphicAppearance@@QBE_NXZ,
    // out of line in retail at 0x004BD200 (landed). Declaration only.
    bool WasRenderedLastFrame() const;                       // 0x004BD200
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT
    // Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): FableWin.pdb
    // ?SetAsHighlighted@CTCGraphicAppearance@NGraphicAppearance@@QAEXMMABVCRGBColour@@_N1W4EAppearanceMeshEffectPriority@2@PAX@Z (out of
    // line at 0x004C04C0: the two widths, the colour by reference, two bools, the priority and the owner key pushed) and
    // ?ClearMeshEffect@..@@QAEXPAX@Z (0x004BFFC0, the owner key). Declarations only; no layout change.
    void SetAsHighlighted(float widthWorldSpace, float widthScreenSpace, const CRGBColour& colour, bool, bool,
                          EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C04C0
    void ClearMeshEffect(void* owner);                        // 0x004BFFC0
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MESHEFFECT
    // Nested opt-in (agentNF11, row 0x004C04C0 SetAsHighlighted; needs FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT): FableWin.pdb
    // ?SetPMeshEffect@CTCGraphicAppearance@NGraphicAppearance@@QAEXV?$CIVCountedPointer@VCEngineMeshEffect@@@@_N11W4EAppearanceMeshEffectPriority@2@PAX@Z
    // (out of line at 0x004C0240, landed: the effect by value, three bools, the priority and the owner key). Declaration only.
    void SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect> effect, bool, bool, bool, EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C0240
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
// (chunk 2020 overlay: with FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT the class above already lays out LightingChannel at +0x40, so the
// opaque layout is skipped -- PeekLightingChannel then reads that member; the two macros together otherwise redefine it, C2086.)
#if !defined(FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT)
    unsigned char LayoutMeshEffectsToComponents[0x40 - 0x0C];       // +0x0C .. +0x3F (opaque)
    CEnginePrimitiveThemeChannelDesc LightingChannel;               // +0x40
#endif
#ifdef FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_GRAPHIC_APPEARANCE_NEW;}
    void SetAsChanged(unsigned long); // Original both PDBs, native004bf9e0.
#endif
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
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_REMOVETINT
    // Retail opt-in (row 0x007E6450 CTCOracleMinigame::UpdateTints): Ego_r.pdb ?RemoveTintColour@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAX@Z
    // (retail 0x004BFAE0, out of line; FableWin 0x01D0A000). Declaration only: no layout change.
    void RemoveTintColour(void* owner);                       // 0x004BFAE0
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
    // Retail opt-in (chunk 794, row 0x007DE710 CTCWallMount::AttachObject): Ego_r.pdb
    // ?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z, out of line in
    // retail (0x004C08C0; catalog name agrees; the row pushes 1.0f, the set, the position, 0, 0, the name). Declaration only.
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& orientation,
                        float scale) const;                  // 0x004C08C0
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECTNAMES
    // Retail opt-in (chunk Q23, row 0x00814260 CTCBoastingPodium::OnInitialActivate): Ego_r.pdb
    // ?GetAllDummyObjectNames@CTCGraphicAppearance@NGraphicAppearance@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z,
    // out of line in retail at 0x004BDB40 (catalog name agrees; `lea ecx,[names] ; push ecx ; mov ecx,appearance ; call`). Needs
    // std::vector (stlport/fable_containers.h) and CCharString at the point of use. Declaration only: no layout change.
    void GetAllDummyObjectNames(std::vector<CCharString>& names) const;   // 0x004BDB40
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE
 // Row 0x00724630: Ego_r 0x004BF740 / FableWin 0x01D0A3C0 ?GetFullScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ, out of line at
 // retail 0x004BBFF0 (`fld [ecx+4Ch] ; fmul [ecx+34h]`, Scale at +0x34). Declaration only: no layout change.
 float GetFullScale()const;                                 // 0x004BBFF0
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCALPOS
    // Nested opt-in (chunk 768, row 0x00753380 CTCLook::FrameUpdateHeadInterested): Ego_r.pdb / FableWin.pdb
    // ?GetFocalPosIfLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_NAAVC3DVector@@M@Z (out of line, retail 0x004C11D0). Declaration only.
    bool GetFocalPosIfLoaded(C3DVector& pos, float time) const;   // 0x004C11D0
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual this declaration lacks, declaration only (no layout change; the vtable exists). Slot 10 of the TLC vtable
// (agentFAM03 proof, re-verified by agentFAM05); spelling from the Ego_r / FableWin name sets.
public:
    virtual void BulletTimeFrameUpdate();                    // slot 10, 0x004C5D70
#endif // FABLE_RETAIL_CTCGRAPHICAPPEARANCE_VSLOTS3
};
}
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CTCGraphicAppearance)==0x5c);
#endif
#endif
