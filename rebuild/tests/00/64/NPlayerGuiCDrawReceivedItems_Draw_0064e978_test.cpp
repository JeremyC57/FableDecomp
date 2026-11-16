// Self-contained behavior fixture: actual shared models, normal lifetimes, recording callee doubles.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#ifdef _MSC_VER
__declspec(noreturn)
#else
__attribute__((noreturn))
#endif
static void fixtureAbort(int line){fprintf(stderr,"abort at fixture line %d\n",line);abort();}
#define abort() fixtureAbort(__LINE__)
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum ESaveGameLoadStatus {};enum ERegionLoadStatus {};enum EPlayerSpawnStatus {};enum ESaveType {};
enum ESex {};enum ECombatZone {};enum EControlledMovementType {};enum EMovementBandType {};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_CTOR_PASS23
#define FABLE_RETAIL_CWORLD_DEFINITION_REFERENCE_PASS23
#define FABLE_RETAIL_ELECTROCUTED_LIFETIMES_PASS23
#define FABLE_RETAIL_CSYSTEMMANAGER
enum ELightingChannel {LIGHTING_CHANNEL_MAIN=0,LIGHTING_CHANNEL_INDOORS=1,LIGHTING_CHANNEL_INDOORS_2=2,LIGHTING_CHANNEL_INDOORS_3=3,LIGHTING_CHANNEL_INDOORS_4=4,LIGHTING_CHANNEL_INDOORS_LAST=4,LIGHTING_CHANNEL_EPIC_SPELL=5,LIGHTING_CHANNEL_COUNT=6};
#include "engine/EPlayerMode.h"
#define private public
#define protected public
// 0064e978 NPlayerGui::CDrawReceivedItems::Draw
// Leaf: ?Draw@CDrawReceivedItems@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_HIGHLIGHT_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWRECEIVEDITEMS_PASS23
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_GLOBALUPDATE
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_DRAW_ACCESSORS_PASS23
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GENERATED_COPY_PASS23
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR
#define FABLE_RETAIL_C3DANIMATIONSTATE
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_GENERATED_SPECIALS_PASS23
#define FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_GENERATED_DTOR_PASS23
#define FABLE_RETAIL_C3DANIMATIONSTATE_ORIGINAL_CTORS_PASS23
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_C3DANIMATIONSTATEALPHA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_C3DANIMATIONSTATEBASE_VIRTUALS
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_EFFECT_ENTRY_PASS23
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CENGINEPRIMITIVE2DANIMATINGMESH_IMPLICIT_DTOR_PASS23
#define FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED
#include "engine/EEnginePrimitiveType.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/NPlayerGui_CDrawReceivedItems.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingObject.h"
#include "engine/CTCHeroReceiveItems.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/GFGetInterpolatedValue.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/CEnginePrimitive2DStaticMesh.h"
#include "engine/CEnginePrimitive2DAnimatingMesh.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/C3DAnimationState.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CMeshInfo.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;
// Genuine body composition from original FableWin01e1dfa4/01e1decf.
// Plain inline context only; native callers retain out-of-line calls.
inline C3DAnimationInterpolator::C3DAnimationInterpolator(const C3DAnimationBlendState& a,const C3DAnimationBlendState& b)
 :StartState(a),EndState(b){}
inline CEnginePrimitive2DAnimatingMesh::CEnginePrimitive2DAnimatingMesh(unsigned __int64 id,unsigned long mesh,const C3DAnimationBlendState& anim,const C2DBoxF& box,const C3DBoundingSphere& sphere,const C3DOrientationInterpolator& orientation,EEngineInterpolationSet interpolation,unsigned char alpha,CRGBColour tint)
 :CEnginePrimitive(ENGINE_PRIMITIVE_2D_ANIMATING_MESH,id,interpolation),ClipBox(box),MeshIndex(mesh),Alpha(alpha),Orientation(orientation),Anim(anim,anim),BoundingSphere(sphere),TintColour(tint){}
void NPlayerGui::CDrawReceivedItems::Draw(const CEnginePrimitivePrepareInfo& info)
{
 const CThingPlayerCreature* phero=GetPlayer().PeekPControlledCreature();
 if(IS_THING_ALIVE(phero)){
 const CTCHeroReceiveItems* ptc_receive_items;
 if(phero->PeekTC(&ptc_receive_items)&&ptc_receive_items->IsDisplayingItems()){
  Drawing=true;
  PrimitiveHandles.resize(2);
  C2DExtentsI screen_extents=GPDisplayManager->GetRenderTargetDimensions();
  C2DBoxF background_box(0.0f,0.0f,(float)screen_extents.X,(float)screen_extents.Y);
  const CRGBColour& colour=CPlayerGui::PeekGuiDef().ContainerItemDisplayBackgroundColour;
  CEnginePrimitive2DSprite background_sprite(0,GFToScreen(background_box),ENGINE_2D_LAYER_7,0,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,colour.A,colour.R,colour.G,colour.B,0,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(PrimitiveHandles[0],background_sprite,0xc0);
  CThingObject* pthing=ptc_receive_items->GetPThingObject();
  if(IS_THING_ALIVE(pthing)){
   float interpolate=info.Interpolate.PausedCurrent.WFInterpolate;
   const C2DBoxF& old_area=ptc_receive_items->GetOldAreaItemTaking();
   const C2DBoxF& new_area=ptc_receive_items->GetCurrentAreaItemTaking();
   C2DBoxF clip_box;
   clip_box.TLX=GFGetInterpolatedValue(old_area.TLX,new_area.TLX,interpolate);
   clip_box.TLY=GFGetInterpolatedValue(old_area.TLY,new_area.TLY,interpolate);
   clip_box.BRX=GFGetInterpolatedValue(old_area.BRX,new_area.BRX,interpolate);
   clip_box.BRY=GFGetInterpolatedValue(old_area.BRY,new_area.BRY,interpolate);
   C3DOrientationInterpolator orientation(ptc_receive_items->GetOldRHS(),ptc_receive_items->GetCurrentRHS());
   std::vector<NGraphicAppearance::CMeshInfo> meshes;
   pthing->GetTCGraphicAppearance()->GetAllMeshes(meshes);
   if(pthing->GetTCGraphicAppearance()->GetMainGraphic().Type==5){
    CEnginePrimitive2DStaticMesh item_mesh(0,meshes[0].Index,clip_box,orientation,ENGINE_INTERPOLATION_SET_1,255,CRGBColour(255,255,255,255));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1],item_mesh,0xc0);
   }else if(pthing->GetTCGraphicAppearance()->GetMainGraphic().Type==4){
    CDefPointer<const CThingObjectDef> pobject_def;
    CMainGameComponent::Get()->PeekDefinitionManager().GetDef(pthing->GetDefGlobalIndex(),&pobject_def);
    CDefPointer<const CAppearanceDef> pappearance_def;
    pobject_def->GetSubDef(&pappearance_def);
    C3DAnimationBlendState anim_blend;
    anim_blend.SetBaseAnimation(C3DAnimationState(pappearance_def->Animation.GetAnim("DEFAULT",0,CAnimFilterNoGroup()).BankIndex,0.0f));
    C3DBoundingSphere bounding_sphere(C3DVector(0,0,0),1.0f);
    CEnginePrimitive2DAnimatingMesh item_mesh(0,meshes[0].Index,anim_blend,clip_box,bounding_sphere,orientation,ENGINE_INTERPOLATION_SET_1,255,CRGBColour(255,255,255,255));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1],item_mesh,0xc0);
   }
  }
 }
}
}

#include "engine/CDefString.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CWorldInit.h"
#include "engine/CWorld.h"
#include "engine/CSystemManager.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CDateAndTime.h"
#define FABLE_RETAIL_NGRAPHICAPPEARANCE_PASS22_PRIMITIVETYPE
#include "engine/NGraphicAppearance_CPrimitive.h"
// Unused incomplete-element containers stay empty through ordinary member lifetimes.
namespace std {
template<> _Vector_base<CGraphicInfo,allocator<CGraphicInfo> >::~_Vector_base(){assert(!_M_start);}
template<> vector<CGraphicInfo>::~vector(){assert(!_M_start&&!_M_finish);}
template<> _Vector_base<CGraphicInfoFrame,allocator<CGraphicInfoFrame> >::~_Vector_base(){assert(!_M_start);}
template<> vector<CGraphicInfoFrame>::~vector(){assert(!_M_start&&!_M_finish);}
}
namespace boost {
template<> scoped_ptr<CRenderManager>::~scoped_ptr(){assert(!ptr);}
template<> scoped_ptr<CVsyncCallbackTimer>::~scoped_ptr(){assert(!ptr);}
}
#undef private
#undef protected

// Unrelated virtual callees fail if reached.
void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
bool CMainGameComponent::Run(CGameComponent**) { abort(); }
void CMainGameComponent::ChangeTextureColourDepth(long) { abort(); }
void CMainGameComponent::OnPreDeviceReset() { abort(); }
bool CMainGameComponent::OnPostDeviceReset() { abort(); }
void CIEngine::Validate() const { abort(); }
bool CIEngine::Initialise(const CEngineInit&) { abort(); }
void CIEngine::Release() { abort(); }
void CIEngine::SetEnvironmentalEffectsAlwaysUpdate(bool) { abort(); }
void CIEngine::SetWorld(const CIDrawWorld*) { abort(); }
void CIEngine::WorldFrameUpdate(bool) { abort(); }
void CIEngine::PausedWorldFrameUpdate() { abort(); }
bool CIEngine::SetVideoOptions(const CEngineVideoOptions&) { abort(); }
void CIEngine::Refresh() { abort(); }
bool CIEngine::CheckPrimitiveVisibility(unsigned long long) { abort(); }
float CIEngine::GetPrimitiveDrawDistanceFactor() const { abort(); }
void CIEngine::AddPrimitive(const CEnginePrimitive&) { abort(); }
bool CIEngine::ClipSphere(const C3DBoundingSphere&) const { abort(); }
bool CIEngine::ClipBox(const C3DBoundingBox&) const { abort(); }
void CIEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle&, const CEnginePrimitiveAnimation&) { abort(); }
void CIEngine::SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&) { abort(); }
unsigned long CIEngine::AddChildPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) { abort(); }
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, const CEnginePrimitive&, unsigned long) { abort(); }
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) { abort(); }
void CIEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&) { abort(); }
void CIEngine::ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long) { abort(); }
void CIEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle&) { abort(); }
void CIEngine::SetAttachedPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, const CEnginePrimitiveAttachmentInfo&, const CEnginePrimitiveHandle&, long, unsigned long) { abort(); }
void CIEngine::CollectPrimitiveStatsForArea(std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, const C2DBoxF&, EEnginePrimitiveStatIndex, unsigned long) const { abort(); }
void CIEngine::ProcessPrimitiveStats(const std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, EEnginePrimitiveStatIndex, unsigned long&, CArray<CCharString>&) const { abort(); }
const C3DAnimationInfo* CIEngine::GetEditorAnimation(const CWideString&) const { abort(); }
unsigned long CIEngine::AddCustomMorphTexture(CTexture&, const CCharString&) const { abort(); }
long CIEngine::GetStaticMapUpdatePassCount() { abort(); }
void CIEngine::StartStaticMapUpdatePass(const CWideString&, long, const CStaticMapGenerationVisibiilityInfo&, const CArray<CCharString>&) { abort(); }
bool CIEngine::IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const { abort(); }
void CIEngine::UpdateStaticMap(long, const CCharString&, unsigned long, long) { abort(); }
void CIEngine::CloseStaticMapFile() { abort(); }
void CIEngine::AddMap(long, long) { abort(); }
void CIEngine::RemoveMap(long) { abort(); }
void CIEngine::SetMapAsLoaded(long) { abort(); }
void CIEngine::SetMapAsUnloaded(long) { abort(); }
void CIEngine::SetMapVisibilityAllMaps(bool) { abort(); }
void CIEngine::SetMapVisibility(long, bool) { abort(); }
void CIEngine::AreaChanged(const C2DBoxI&) { abort(); }
void CIEngine::OnActivate() { abort(); }
void CIEngine::OnDeactivate() { abort(); }
bool CIEngine::IsActive() const { abort(); }
void CIEngine::SetStaticMapFileForUse(const CWideString&) { abort(); }
void CIEngine::SetLandscapeDebugTintColour(long, CRGBColour) { abort(); }
void CIEngine::ReloadLocalDetailDefs() { abort(); }
void CIEngine::StartNewScene(unsigned long, float) { abort(); }
void CIEngine::CutscenePreload(bool, bool) { abort(); }
void CIEngine::CutsceneAddCameraAngle(const CCamera&) { abort(); }
void CIEngine::ChangeCameraAngle() { abort(); }
bool CIEngine::IsPreloaded() const { abort(); }
void CIEngine::UpdateTextureMorphCache(const CArray<CEngineTextureMorph>&, bool, float, const std::set<long, std::less<long>, std::allocator<long> >*) { abort(); }
void CIEngine::SetScreenColourFilter(const CEngineColourFilterSettings&) { abort(); }
void CIEngine::SetSpotlightShadowMode(bool) { abort(); }
void CIEngine::SetSeaBodyToRender(CCharString) { abort(); }
void CIEngine::UpdateDebugOverlay(long, const CRGBColour*, const C2DBoxI&) { abort(); }
void CIEngine::EnableDebugOverlay(long, bool, bool) { abort(); }
void CIEngine::SetGlobalLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&) { abort(); }
void CIEngine::Set2DPrimitiveLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&) { abort(); }
void CIEngine::SetRenderTargetTexture(const CTexture*) { abort(); }
CTexture* CIEngine::AllocateTextureFromEngineResourcePools(unsigned long, const C2DExtentsI&, long, const CPixelFormat&) { abort(); }
void CIEngine::FreeTextureFromEngineResourcePools(CTexture*) { abort(); }
CMovableResourceMemoryPool* CIEngine::GetLoadingMemoryPool() { abort(); }
CMovableResourceMemoryPool* CIEngine::GetAnimationMemoryPool() { abort(); }
CGPUScratchBufferTextureManager* CIEngine::GetScratchBufferTextureManager() { abort(); }
bool CIEngine::DeallocateShadowBuffer() { abort(); }
void CIEngine::ReallocateShadowBuffer() { abort(); }
void CIEngine::Enable3DSceneRendering(bool) { abort(); }
bool CIEngine::Is3DSceneRenderingEnabled() { abort(); }
void CIEngine::Enable2DSceneRendering(bool) { abort(); }
bool CIEngine::Is2DSceneRenderingEnabled() { abort(); }
void CIEngine::OverrideRain(float) { abort(); }
void CIEngine::OverrideSnow(float) { abort(); }
void CIEngine::RenderColorFilteredQuad(float, float, const C4DVector&, float, float, CTexture*) { abort(); }
const CRGBFloatColour CIEngine::PeekGlobalDiffuseColour() const { abort(); }
const CRGBFloatColour CIEngine::PeekGlobalAmbientColour() const { abort(); }
bool CBankFile::OpenReadOnly(const CCharString&,unsigned long) { abort(); }
void CBankFile::OpenReadWrite(const CCharString&,unsigned long,unsigned long) { abort(); }
bool CBankFile::OpenFileReadOnly(const CWideString&,unsigned long) { abort(); }
void CBankFile::OpenFileReadWrite(const CWideString&,unsigned long,unsigned long) { abort(); }
void CBankFile::Close() { abort(); }
void CBankFile::AddEntryMultipleFiles(const CCharString&,unsigned long,const CArray<CCharString>&,const CCountedPointer<CBankStateBlock>) { abort(); }
void CBankFile::Update() { abort(); }
unsigned long CBankFile::AdjustUpdateEntries() { abort(); }
void CBankFile::NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount>&) { abort(); }
void CBankFile::NotifyBankOpened() { abort(); }
void CBankFile::NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&) { abort(); }
void CBankFile::NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char>&) { abort(); }
void CBankFile::NotifyEntryRemoved(unsigned long) { abort(); }
void CBankFile::NotifyPostEntriesLoaded() { abort(); }
void CBankFile::PrepareDataForInclusion(unsigned long,const CArray<CCharString>&,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream&,CDataOutputStream&) { abort(); }
bool CBankFile::IsDiskFile(unsigned long,unsigned long) { abort(); }
CDateAndTime CBankFile::GetLastModifiedTime(const CCharString&) { abort(); }
CCharString CBankFile::GetEnumerationType(unsigned long) const { abort(); }
bool CBankFileAsync::OpenReadOnly(const CCharString&,unsigned long) { abort(); }
void CBankFileAsync::OpenReadWrite(const CCharString&,unsigned long,unsigned long) { abort(); }
bool CBankFileAsync::OpenFileReadOnly(const CWideString&,unsigned long) { abort(); }
void CBankFileAsync::OpenFileReadWrite(const CWideString&,unsigned long,unsigned long) { abort(); }
void CBankFileAsync::Close() { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
unsigned long CGraphicDataBank::GetNoFramesInGraphic(unsigned long) const { abort(); }
unsigned long CGraphicDataBank::GetGraphicFrameWidth(unsigned long,unsigned long) const { abort(); }
unsigned long CGraphicDataBank::GetGraphicFrameHeight(unsigned long,unsigned long) const { abort(); }
unsigned long CGraphicDataBank::GetGraphicMaxWidth(unsigned long) const { abort(); }
unsigned long CGraphicDataBank::GetGraphicMaxHeight(unsigned long) const { abort(); }
bool CGraphicDataBank::IsUsingSurfaces() const { abort(); }
void CGraphicDataBank::Close() { abort(); }
unsigned long CGraphicDataBank::GetDataFormatVersion() { abort(); }
void CGraphicDataBank::NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount>&) { abort(); }
void CGraphicDataBank::NotifyBankOpened() { abort(); }
void CGraphicDataBank::NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char>&) { abort(); }
void CGraphicDataBank::NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&) { abort(); }
void CGraphicDataBank::NotifyEntryRemoved(unsigned long) { abort(); }
void CGraphicDataBank::PrepareDataForInclusion(unsigned long,const CArray<CCharString>&,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream&,CDataOutputStream&) { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
CDefString CParentDefClassBase::GetInstantiationName() const { abort(); }
void CParentDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
void CPlayerGuiDef::Transfer(CPersistContext&) { abort(); }
void CPlayerGuiDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CPlayerGuiDef::GetSizeofClass() { abort(); }
void CWorld::PostLoadInit() { abort(); }
void CWorld::LoadWorldFromFile(CAFile& file, long version) { abort(); }
CThingManager& CWorld::GetThingManager() { abort(); }
const CThingManager& CWorld::PeekThingManager() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo& info) const { abort(); }
void CWorld::PostInit(CDisplayEngine& displayEngine) { abort(); }
C3DVector CWorld::GetGameLODCentre() const { abort(); }
void CWorld::Init() { abort(); }
void CWorld::Uninitialise() { abort(); }
void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info) { abort(); }
CWorldMap& CWorld::GetWorldMap() { abort(); }
const CWorldMap& CWorld::PeekWorldMap() const { abort(); }
const CProceduralTexture* CWorld::DrawGetProceduralTexture() const { abort(); }
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const { abort(); }
const CIDrawEnvironment* CWorld::DrawGetEnvironment() const { abort(); }
void CGameDefinitionManager::InitAndCompile() { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
bool CThingPlayerCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
bool CThingPlayerCreature::SetActionForMelee(const CCreatureAbilityDef& ability) { abort(); }
void CTCBase::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCBase::OnRemove() { abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase&) { abort(); }
void CTCBase::OnCreate() { abort(); }
void CTCBase::InitialActivate() { abort(); }
void CTCBase::OnInitialActivate() { abort(); }
void CTCBase::RenderUpdate(float) { abort(); }
bool CTCBase::HasFrameUpdate() const { abort(); }
void CTCBase::FrameUpdate() { abort(); }
void CTCBase::BulletTimeFrameUpdate() { abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCBase::Copy(const CTCBase&) { abort(); }
void CTCBase::OnDie() { abort(); }
void CTCBase::OnKill() { abort(); }
void CTCBase::OnLevelUnload() { abort(); }
void CTCBase::OnSetInLimbo(bool) { abort(); }
bool CTCBase::Validate(CCharString&) const { abort(); }
bool CTCBase::IsFreeForDeletion() const { abort(); }
bool CTCBase::IsSerialiseCreatable() const { abort(); }
bool CTCBase::IsSerialisableForHeroState() const { abort(); }
bool CTCBase::IsComponentSerialisable() const { abort(); }
bool CTCBase::IsDriver() const { abort(); }
bool CTCBase::IsCopyable() const { abort(); }
void CTCBase::GlobalUpdate(bool) { abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) { abort(); }
void CTCHeroReceiveItems::OnSerialise(CPersistContext& context, bool in_game) { abort(); }
void CTCHeroReceiveItems::GlobalUpdate(bool paused) { abort(); }
void CThingObjectDef::Copy(const CDefClassBase * p0) { abort(); }
unsigned long CThingObjectDef::GetSizeofClass() { abort(); }
void CThingObjectDef::Validate(const CValidateDefinitionInitBase & p0, const CCharString & p1) const { abort(); }
void CThingObjectDef::Transfer(CPersistContext & p0) { abort(); }

static char events[512];static unsigned eventCount;static bool recording;
static void event(char c){if(recording){assert(eventCount<511);events[eventCount++]=c;events[eventCount]=0;}}
static unsigned stringsLive,submissions,meshCalls,orientationCalls,lookupCalls,animationCalls;
static NPlayerGui::CDrawReceivedItems* activeDrawer;static CPlayerGuiDef* activeDef;
static CPlayerManager* activePlayers;static CPlayer* activePlayer;static CThingPlayerCreature* activeHero;
static CWorld* activeWorld;static CMainGameComponent* activeComponent;static CGraphicDataBank* activeBank;
static CTCHeroReceiveItems* activeReceive;static CThingObject* activeItem;
static NGraphicAppearance::CTCGraphicAppearance* activeAppearance;
static CDefinitionManager* activeDefinitions;static CThingObjectDef* activeObjectDef;static CAppearanceDef* activeAppearanceDef;
static C3DAnimationBlendState::CSharedData* activeShared;static C3DAnimationState* allocatedState;
static CAnimationEntry* activeAnimation;static unsigned long itemType;static float interpolation;
static const char* chars(const CCharString&s){return s.PStringData?reinterpret_cast<const char*>(s.PStringData):"";}
static CCharStringData* copyText(const char*x,long n=-1){if(!*x)return 0;if(n<0)n=(long)strlen(x);char*p=(char*)malloc(n+1);assert(p);memcpy(p,x,n);p[n]=0;++stringsLive;return reinterpret_cast<CCharStringData*>(p);}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*x,long n):PStringData(copyText(x,n)){assert(strcmp(x,"DEFAULT")==0||strcmp(x,"CAppearanceDef")==0);event(*x=='D'?'S':'D');}
CCharString::CCharString(const CCharString&x):PStringData(copyText(chars(x))){}
CCharString::~CCharString(){if(PStringData){event(*chars(*this)=='D'?'s':'d');free(PStringData);--stringsLive;}}
CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){assert(!PStringData);}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CGame::CGame():CurrentGameComponent(0),Quit(false){memset(ParameterBuffer,0,sizeof ParameterBuffer);}CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){}CGameComponent::~CGameComponent(){}
void CGameComponent::Init(){}void CGameComponent::SetQuit(){Quit=true;}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){}CMainGameComponent::~CMainGameComponent(){}
CMainGameComponent* CMainGameComponent::PMainGameComponent=0;
CMainGameComponentInit::CMainGameComponentInit(){}CMainGameComponentInit::~CMainGameComponentInit(){}
template<> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::~CTBaseSingletonNoDefaultAlloc(){}
template<> CCountedPointer<CPlayerGui> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass=CCountedPointer<CPlayerGui>();
CInterpolationInfo::CInterpolationInfo(){Current.GTPredictedTimeSinceLastRenderFrame=Current.WFInterpolate=Last.GTPredictedTimeSinceLastRenderFrame=Last.WFInterpolate=PausedCurrent.GTPredictedTimeSinceLastRenderFrame=PausedCurrent.WFInterpolate=PausedLast.GTPredictedTimeSinceLastRenderFrame=PausedLast.WFInterpolate=BulletTimeCurrent.GTPredictedTimeSinceLastRenderFrame=BulletTimeCurrent.WFInterpolate=BulletTimeLast.GTPredictedTimeSinceLastRenderFrame=BulletTimeLast.WFInterpolate=0;WFServerCurrentTime=0;}
CEnginePrimitivePrepareInfo::CEnginePrimitivePrepareInfo(const CInterpolationInfo&i,CIEngine&e,bool edit,const CCamera&c):Interpolate(i),Engine(e),Edit2DEngine(edit),Camera(c){}
CIEngine::~CIEngine(){}
CBankFile::~CBankFile(){}CBankFileAsync::~CBankFileAsync(){}CResourceBank::CResourceBank(){}CResourceBank::~CResourceBank(){}
void CIVCountedPointeeBase::Delete()const{assert(this==allocatedState);delete allocatedState;allocatedState=0;}CFailedAllocationHandler::~CFailedAllocationHandler(){}
CResource::~CResource(){}CResourceList::~CResourceList(){}CGraphicDataBank::~CGraphicDataBank(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CDefinitionManager::~CDefinitionManager(){}CGameDefinitionManager::CGameDefinitionManager(){}CGameDefinitionManager::~CGameDefinitionManager(){}
CWorldInit::CWorldInit():Component(0),ScavengeWorld(0),PlayerManager(0),DefinitionManager(0),DisplayEngine(0){}
CWorld::CWorld(const CWorldInit&i):Component(i.Component),PlayerManager(i.PlayerManager),DefinitionManager(*i.DefinitionManager){Paused=false;}
CWorld::~CWorld(){}CIDrawWorld::~CIDrawWorld(){}
CPlayerManager::CPlayerManager(CMainGameComponent&c,const CGameDefinitionManager&d):Component(&c),DefinitionManager(const_cast<CGameDefinitionManager*>(&d)){}
CPlayer& CPlayerManager::GetMainPlayer(){assert(this==activePlayers);event('P');return*activePlayer;}
const CThingPlayerCreature* CPlayer::PeekPControlledCreature()const{assert(this==activePlayer);event('H');return activeHero;}
CThingPlayerCreature::CThingPlayerCreature(){}
CTCBase::CTCBase(CThing&h):ParentThing(&h){}CTCBase::~CTCBase(){}

namespace std {template<size_t N> bool bitset<N>::test(size_t n)const{assert(n==TCI_HERO_RECEIVE_ITEMS);event('B');return _Unchecked_test(n);}}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){assert(!PData);}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{assert(this==&activeReceive->PCurrentItem);event('I');return activeItem;}
CTCHeroReceiveItems::CTCHeroReceiveItems(CThing&h):CTCBase(h),PersistCurrentItem(false),ReceivingItems(false),DisplayingItems(false),ForceClear(false),SendMessageW(false){}
CTCHeroReceiveItems::~CTCHeroReceiveItems(){}
namespace NGraphicAppearance {CTCGraphicAppearance::CTCGraphicAppearance(CThing&h):CTCBase(h){}CTCGraphicAppearance::~CTCGraphicAppearance(){}
void CTCGraphicAppearance::GetAllMeshes(std::vector<CMeshInfo>&v)const{assert(this==activeAppearance&&v.empty());event('M');++meshCalls;v._M_start=v._M_end_of_storage.allocate(2);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+2;for(unsigned i=0;i<2;++i){std::_Construct(v._M_finish,CMeshInfo());v._M_finish->Index=301+i;v._M_finish->SizeX=4.0f+i;v._M_finish->PrimaryMesh=i==0;++v._M_finish;}}
}
CDisplayManager::CDisplayManager(CSystemManager&s):SystemManager(s){}CDisplayManager::~CDisplayManager(){}
CBusyResourceManager::CBusyResourceManager(){}CBusyResourceManager::~CBusyResourceManager(){}
CDisplayManager* GPDisplayManager=0;
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions(){assert(this==GPDisplayManager);event('R');return C2DExtentsI(640,512);}
namespace NPlayerGui {
CDrawBase::CDrawBase(CPlayerGui&g,std::vector<CDrawBase*>&):PlayerGui(g),PlayerManager(*activePlayers),World(*activeWorld),Component(*activeComponent),Bank(*activeBank),Drawing(false){}
CDrawBase::~CDrawBase(){}void CDrawBase::ClearPrimitives(){abort();}void CDrawBase::ClearPrimitivesIfNecessary(){abort();}
CDrawReceivedItems::CDrawReceivedItems(CPlayerGui&g,std::vector<CDrawBase*>&v):CDrawBase(g,v){}CDrawReceivedItems::~CDrawReceivedItems(){}
}
const CPlayerGuiDef& FABLE_FASTCALL CPlayerGui::PeekGuiDef(){event('G');return*activeDef;}
namespace std {template<> void vector<CEnginePrimitiveHandle>::_M_fill_insert(iterator p,size_type n,const CEnginePrimitiveHandle&x){assert(p==end());if(!n)return;assert(empty()&&n==2);event('Z');_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;for(size_type i=0;i<n;++i){_Construct(_M_finish,x);++_M_finish;}}}
CSubDefClassBase::CSubDefClassBase(){}
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&){abort();return 0;}
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&s)const{return CDefClassBase::GetPSubDefInfo(s);}
CThingObjectDef::CThingObjectDef(){}CThingObjectDef::~CThingObjectDef(){}
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&){abort();return 0;}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&s)const{assert(this==activeObjectDef&&strcmp(chars(s),"CAppearanceDef")==0);event('U');static CSubDefInfo i;i.DefIndex=22;i.OriginalParentDefIndex=11;return &i;}
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)const{assert(this==activeDefinitions&&(index==11||index==22));++lookupCalls;event(index==11?'F':'A');CDefPointer<const CDefClassBase> out;out.Object=index==11?static_cast<CDefPointeeBase*>(activeObjectDef):static_cast<CDefPointeeBase*>(activeAppearanceDef);out.Object->AddRef();return out;}
const CGameDefinitionManager& CMainGameComponent::PeekDefinitionManager()const{assert(this==activeComponent);event('Q');return *static_cast<CGameDefinitionManager*>(activeDefinitions);}
NGraphicAppearance::CPrimitive::~CPrimitive(){}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index){assert(this==activeDefinitions&&index==22);++lookupCalls;event('A');CDefPointer<CDefClassBase> out;out.Object=activeAppearanceDef;out.Object->AddRef();return out;}
class FixtureReceive:public CTCHeroReceiveItems {public:FixtureReceive(CThing&t):CTCHeroReceiveItems(t){}ETCInterfaceType GetInterfaceType()const{abort();return TCI_HERO_RECEIVE_ITEMS;}CCharString GetClassTypeName()const{abort();return CCharString();}};
class FixtureAppearance:public NGraphicAppearance::CTCGraphicAppearance {public:FixtureAppearance(CThing&t):CTCGraphicAppearance(t){}ETCInterfaceType GetInterfaceType()const{abort();return TCI_HERO_RECEIVE_ITEMS;}CCharString GetClassTypeName()const{abort();return CCharString();}};
class TestAppearanceDef:public CAppearanceDef {public:void Transfer(CPersistContext&){abort();}void Copy(const CDefClassBase*){abort();}unsigned long GetSizeofClass(){abort();return 0;}};
C3DAnimationBlendState::C3DAnimationBlendState(){}
void C3DAnimationBlendState::SetBaseAnimation(const C3DAnimationState&s){assert(Data.IsNull()&&s.AnimationIndex==777&&s.AnimationTime==0.0f&&!allocatedState);event('T');allocatedState=new C3DAnimationState(s);assert(allocatedState->IVRefCount==1);activeShared->BaseAnimation.Reset(allocatedState);activeShared->AddRef();Data.Reset(activeShared);}
C3DAnimationStateBase::EType C3DAnimationState::GetType()const{return TYPE_ANIMATION_STATE;}
const C3DAnimationState& C3DAnimationState::FindFirstAnimation()const{return*this;}
const C3DAnimationState& C3DAnimationState::FindDominantAnimation()const{return*this;}
void C3DAnimationState::Validate(){abort();}
bool CAnimFilterNoGroup::operator()(const CAnimationEntry&e)const{assert(&e==activeAnimation);return e.GroupName.TablePos==-1;}
CAnimFilterBase* CAnimFilterNoGroup::Clone()const{abort();return 0;}
const CAnimationEntry& CAnimationSet::GetAnim(const CCharString&s,long i,const CAnimFilterBase&f)const{assert(this==&activeAppearanceDef->Animation&&strcmp(chars(s),"DEFAULT")==0&&i==0&&f(*activeAnimation));event('N');++animationCalls;return*activeAnimation;}
C3DOrientationInterpolator::C3DOrientationInterpolator(const CRightHandedSet&a,const CRightHandedSet&b):StartOrientation(a),EndOrientation(b){assert(&a==&activeReceive->OldItemRHSet&&&b==&activeReceive->CurrentItemRHSet);event('O');++orientationCalls;}
CEnginePrimitive2DStaticMesh::CEnginePrimitive2DStaticMesh(unsigned __int64 id,unsigned long mesh,const C2DBoxF&box,const C3DOrientationInterpolator&o,EEngineInterpolationSet interp,unsigned char alpha,CRGBColour tint):CEnginePrimitive(ENGINE_PRIMITIVE_2D_STATIC_MESH,id,interp),MeshIndex(mesh),ClipBox(box),Alpha(alpha),Orientation(o),TintColour(tint){event('C');}
static void checkBox(const C2DBoxF&b){assert(b.TLX==8.0f+16.0f*interpolation&&b.TLY==16.0f+16.0f*interpolation&&b.BRX==40.0f+32.0f*interpolation&&b.BRY==64.0f+32.0f*interpolation);}
static void checkOrientation(const C3DOrientationInterpolator&o){assert(o.StartOrientation.Up.X==1&&o.StartOrientation.Up.Y==2&&o.StartOrientation.Up.Z==4&&o.StartOrientation.Forward.X==8&&o.EndOrientation.Up.X==16&&o.EndOrientation.Forward.Z==128);}
static void checkTint(CRGBColour t){assert(t.R==255&&t.G==255&&t.B==255&&t.A==255);}
class TestEngine:public CIEngine {public:
 virtual void SetCamera(const CCamera*) { abort(); }
 virtual void SetWindow(const C2DBoxF*) { abort(); }
 virtual void Virtual08_00b27d90() { abort(); }
 virtual void Virtual11_00b23cf0() { abort(); }
 virtual void Virtual12_00b24400() { abort(); }
 virtual void Virtual13_00b242d0() { abort(); }
 virtual bool PreChangeResolution() { abort(); }
 virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long) { abort(); }
void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned long f){assert(f==0xc0&&submissions<2);assert(&h==&activeDrawer->PrimitiveHandles[submissions]);
 if(submissions==0){const CEnginePrimitive2DSprite&s=static_cast<const CEnginePrimitive2DSprite&>(p);assert(p.Type==ENGINE_PRIMITIVE_2D_SPRITE&&p.InterpolationSet==ENGINE_INTERPOLATION_SET_0&&s.Layer==ENGINE_2D_LAYER_7&&s.BankIndex==0&&s.Frame==0&&s.BlendMode==ENGINE_PRIMITIVE_BLEND_ALPHA&&s.StartAngle==0&&s.EndAngle==0&&s.Flags==0&&!s.Texture&&!s.ClipBoxIsPos);assert(s.ClipBox.TLX==0&&s.ClipBox.TLY==0&&s.ClipBox.BRX==640&&s.ClipBox.BRY==512&&s.UVOffsetsStart.X==0&&s.UVOffsetsStart.Y==0&&s.UVOffsetsEnd.X==0&&s.UVOffsetsEnd.Y==0);assert(s.Colour.R==activeDef->ContainerItemDisplayBackgroundColour.R&&s.Colour.G==activeDef->ContainerItemDisplayBackgroundColour.G&&s.Colour.B==activeDef->ContainerItemDisplayBackgroundColour.B&&s.Colour.A==activeDef->ContainerItemDisplayBackgroundColour.A);event('0');}
 else{assert(p.InterpolationSet==ENGINE_INTERPOLATION_SET_1);if(itemType==5){const CEnginePrimitive2DStaticMesh&m=static_cast<const CEnginePrimitive2DStaticMesh&>(p);assert(p.Type==ENGINE_PRIMITIVE_2D_STATIC_MESH&&m.MeshIndex==301&&m.Alpha==255);checkBox(m.ClipBox);checkOrientation(m.Orientation);checkTint(m.TintColour);assert(m.BoundingSphere.Radius==0);}
 else{const CEnginePrimitive2DAnimatingMesh&m=static_cast<const CEnginePrimitive2DAnimatingMesh&>(p);assert(itemType==4&&p.Type==ENGINE_PRIMITIVE_2D_ANIMATING_MESH&&m.MeshIndex==301&&m.Alpha==255);checkBox(m.ClipBox);checkOrientation(m.Orientation);checkTint(m.TintColour);assert(m.BoundingSphere.Center.X==0&&m.BoundingSphere.Center.Y==0&&m.BoundingSphere.Center.Z==0&&m.BoundingSphere.Radius==1);assert(m.Anim.StartState.Data.Object==activeShared&&m.Anim.EndState.Data.Object==activeShared&&activeShared->IVRefCount==4&&activeShared->BaseAnimation.Object==allocatedState&&allocatedState->IVRefCount==1&&allocatedState->AnimationIndex==777&&allocatedState->AnimationTime==0);}
 event('1');}++submissions;}
};
int main(){unsigned cases=0;
 {CGame game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);activeComponent=&component;CMainGameComponent::PMainGameComponent=&component;
 CGameDefinitionManager definitions;activeDefinitions=&definitions;
 CPlayerManager players(component,definitions);activePlayers=&players;CPlayer player;activePlayer=&player;
 CWorldInit wi;wi.Component=&component;wi.PlayerManager=&players;wi.DefinitionManager=&definitions;CWorld world(wi);activeWorld=&world;
 CSystemManager system;CDisplayManager display(system);GPDisplayManager=&display;CGraphicDataBank bank;activeBank=&bank;
 CPlayerGui gui;CPlayerGuiDef def;activeDef=&def;CThingPlayerCreature hero;FixtureReceive receive(hero);activeReceive=&receive;
 CThingObject item;FixtureAppearance appearance(item);activeAppearance=&appearance;item.PTCGraphicAppearance=&appearance;item.DefGlobalIndex=11;
 CThingObjectDef objectDef;TestAppearanceDef appearanceDef;activeObjectDef=&objectDef;activeAppearanceDef=&appearanceDef;objectDef.PDefManager=&definitions;appearanceDef.PDefManager=&definitions;
 C3DAnimationBlendState::CSharedData shared;activeShared=&shared;CAnimationEntry animation;animation.BankIndex=777;activeAnimation=&animation;
 typedef std::pair<ETCInterfaceType,CTCBase*> Entry;Entry e(TCI_HERO_RECEIVE_ITEMS,&receive);hero.Components._M_start=hero.Components._M_end_of_storage.allocate(1);std::_Construct(hero.Components._M_start,e);hero.Components._M_finish=hero.Components._M_start+1;hero.Components._M_end_of_storage._M_data=hero.Components._M_finish;
 TestEngine engine;CInterpolationInfo ii;CCamera camera;
 for(int test=0;test<38;++test){recording=false;activeHero=&hero;activeItem=&item;hero.Dead=item.Dead=false;receive.DisplayingItems=true;itemType=5;
 memset(hero.UsingComponents._M_w,0,sizeof hero.UsingComponents._M_w);hero.UsingComponents._M_w[TCI_HERO_RECEIVE_ITEMS/(8*sizeof(unsigned long))]|=1UL<<(TCI_HERO_RECEIVE_ITEMS%(8*sizeof(unsigned long)));
 if(test==0)activeHero=0;if(test==1)hero.Dead=true;if(test==2)memset(hero.UsingComponents._M_w,0,sizeof hero.UsingComponents._M_w);if(test==3)receive.DisplayingItems=false;if(test==4)activeItem=0;if(test==5)item.Dead=true;
 if(test>=6&&test<11)itemType=4;if(test>=11&&test<16)itemType=0;if(test>=16&&test<21)itemType=3;if(test>=21&&test<26)itemType=6;if(test>=26&&test<31)itemType=5;if(test>=31&&test<36)itemType=4;
 interpolation=(float)(test%5-1)*0.5f;ii.PausedCurrent.WFInterpolate=interpolation;ii.Current.WFInterpolate=8;ii.Last.WFInterpolate=16;ii.PausedLast.WFInterpolate=32;
 receive.OldAreaItemTaking=C2DBoxF(8,16,40,64);receive.CurrentAreaItemTaking=C2DBoxF(24,32,72,96);
 receive.OldItemRHSet.Up=C3DVector(1,2,4);receive.OldItemRHSet.Forward=C3DVector(8,4,2);receive.CurrentItemRHSet.Up=C3DVector(16,32,64);receive.CurrentItemRHSet.Forward=C3DVector(32,64,128);
 appearance.MainGraphic.Type=(unsigned char)itemType;def.ContainerItemDisplayBackgroundColour=CRGBColour((unsigned char)(test*4),(unsigned char)(test*2),(unsigned char)(255-test),(unsigned char)(test*6));
 std::vector<NPlayerGui::CDrawBase*> v;NPlayerGui::CDrawReceivedItems drawer(gui,v);activeDrawer=&drawer;drawer.Drawing=test==37;
 if(test==36)drawer.PrimitiveHandles.resize(2);
 bool shown=test>=4;bool itemLive=shown&&test!=4&&test!=5;bool rendered=itemLive&&(itemType==4||itemType==5);bool anim=rendered&&itemType==4;
 submissions=meshCalls=orientationCalls=lookupCalls=animationCalls=eventCount=0;events[0]=0;CEnginePrimitivePrepareInfo info(ii,engine,false,camera);recording=true;drawer.Draw(info);recording=false;
 assert(drawer.Drawing==(shown||test==37)&&drawer.PrimitiveHandles.size()==(shown?2:0));assert(submissions==(unsigned)(shown+rendered)&&meshCalls==(unsigned)itemLive&&orientationCalls==(unsigned)itemLive&&lookupCalls==(unsigned)(anim?2:0)&&animationCalls==(unsigned)anim);
 char expected[512];strcpy(expected,"PH");if(test!=0&&test!=1){strcat(expected,"B");if(shown){if(test!=36)strcat(expected,"Z");strcat(expected,"RG0I");if(itemLive){strcat(expected,"OM");if(itemType==5)strcat(expected,"C1");if(anim)strcat(expected,"QFDUdASNTs1");}}}
 if(strcmp(events,expected)!=0){fprintf(stderr,"trace %d: %s expected %s\n",test,events,expected);abort();}assert(stringsLive==0&&shared.IVRefCount==1&&objectDef.IVRefCount==1&&appearanceDef.IVRefCount==1);
 if(anim){assert(allocatedState&&allocatedState->IVRefCount==1);shared.BaseAnimation.Reset(0);assert(!allocatedState);}else assert(!allocatedState);
 ++cases;
 }
 CMainGameComponent::PMainGameComponent=0;GPDisplayManager=0;
 }
 assert(cases==38&&stringsLive==0&&!allocatedState);puts("0064e978_TEST PASS");return 0;
}