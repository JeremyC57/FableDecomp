// Self-contained behavior fixture: actual shared models, normal lifetimes, recording callee doubles.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
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
#define FABLE_RETAIL_CSYSTEMMANAGER
#include "engine/EPlayerMode.h"
#define private public
#define protected public
// 0064a639 NPlayerGui::CDrawGuildSeal::Draw
// Leaf: ?Draw@CDrawGuildSeal@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail:0x0064A639,911bytes,thiscall.
// Flags: none
#define FABLE_RETAIL_STLP_QUEUE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ADD_COPY_RHS_X_PASS23
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWGUILDSEAL_PASS23
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISCHILD
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD_CTOR_PASS23
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_DEFINITION_REFERENCE_PASS23
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#include "engine/EEnginePrimitiveType.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/NPlayerGui_CDrawGuildSeal.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventory.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/ThingAlive.h"
#include "engine/NPlayerGui_Graphics.h"

#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;
// Original same-TU inline helper, FableWin020d922d; native00643cda omits frame pointer.
inline C2DVector FABLE_FASTCALL NPlayerGui::GFAnchorToBottomLeft(const C2DVector& pos)
{
 return C2DVector(pos.X,(float)GPDisplayManager->GetRenderTargetDimensions().Y+pos.Y-480.0f);
}

void NPlayerGui::CDrawGuildSeal::Draw(const CEnginePrimitivePrepareInfo& info)
{
 if(ReadyToLevelUp&&!World.IsPaused()){
  const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
  ReminderTime+=info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame;
  if(ReminderTime>gui_def.GuildSealReminderTimeSeconds){
   LevelUpGraphic=gui_def.DPadGuildSealLevelUpGraphic;
   SetEnabled(true);
  }
 }
 if(FlashCycles>0){
  if(Enabled){
   const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
   if(!World.IsPaused()){
    FadeInOut.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
    if(FadeInOut.IsOpen())FadeInOut.Close();
    else if(!FadeInOut.IsActive()){
     if(--FlashCycles>0)FadeInOut.Open(gui_def.GuildSealFlashCycleSeconds*0.5f,gui_def.GuildSealFlashCycleSeconds*0.5f,CRGBColour(0,0,0,255));
    }
   }
   FlashAlpha=(unsigned char)(FadeInOut.GetFadeInOutAlpha()*255.0f);
   if(ReadyToLevelUp&&FlashAlpha>0){
    const CThingPlayerCreature* phero=GetPlayer().PeekPControlledCreature();
    const CTCInventory* ptc_inventory=0;
    if(IS_THING_ALIVE(phero)&&!phero->IsChild()&&phero->PeekTC(&ptc_inventory)){
     long def_index=World.PeekDefinitionManager().GetDefGlobalIndexFromName("OBJECT_GUILD_SEAL_1");
     if(def_index>0&&ptc_inventory->GetNumberOfItemsOfTypeInInventory(def_index)>0){
      Drawing=true;
      long graphic=gui_def.GuildSealLevelUpGraphic;
      std::map<CCharString,long>::const_iterator it=gui_def.ScriptSprites.find("HUD_ORB_QUEST_CORE");
      if(it!=gui_def.ScriptSprites.end()&&it->second==LevelUpGraphic)graphic=gui_def.GuildSealQuestGraphic;
      CEnginePrimitive2DSprite level_up_guild_seal_sprite(0,GFToScreen(GFAnchorToBottomLeft(gui_def.DPadRingsPos+gui_def.DPadRingsTopOffset)),ENGINE_2D_LAYER_1,graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,ScaleAlpha(FlashAlpha),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
      info.Engine.SetPersistentPrimitive(PrimitiveHandles[0],level_up_guild_seal_sprite,0xc0);
     }
    }
    if(!LevelUpPositions.empty()){
     Drawing=true;
     if(LevelUpGraphic==0)LevelUpGraphic=gui_def.DPadGuildSealLevelUpGraphic;
     for(long i=0;(unsigned long)i<LevelUpPositions.size();++i){
      CEnginePrimitive2DSprite level_up_sprite(0,GFToScreen(LevelUpPositions[i]),ENGINE_2D_LAYER_1,LevelUpGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,ScaleAlpha(FlashAlpha),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
      info.Engine.SetPersistentPrimitive(PrimitiveHandles[i+1],level_up_sprite,0xc0);
     }
    }
   }
  }
 }
 if(FlashCycles==0)SetEnabled(false);
}

#include "engine/CDefString.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CWorldInit.h"
#include "engine/CSystemManager.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CDateAndTime.h"
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
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
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
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
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
void NInventory::CTCInventoryBase::OnCreate() { abort(); }
void NInventory::CTCInventoryBase::InitialActivate() { abort(); }
void NInventory::CTCInventoryBase::OnInitialActivate() { abort(); }
void NInventory::CTCInventoryBase::GlobalUpdate(bool) { abort(); }
void NInventory::CTCInventoryBase::OnKill() { abort(); }
void NInventory::CTCInventoryBase::Copy(const CTCBase&) { abort(); }
bool NInventory::CTCInventoryBase::IsComponentSerialisable() const { abort(); }
void NInventory::CTCInventoryBase::UpdateMenuScrollButtonsCategory() { abort(); }
void NInventory::CTCInventoryBase::UpdateMenuScrollButtonsItem() { abort(); }
void NInventory::CTCInventoryBase::OpenInventory() { abort(); }
void NInventory::CTCInventoryBase::CloseInventory() { abort(); }
void NInventory::CTCInventoryBase::AddCloseInventoryEvent() { abort(); }
void NInventory::CTCInventoryBase::OnItemSelectedInHand(CThing&,bool&) { abort(); }
void NInventory::CTCInventoryBase::OnItemDeselected(long,bool&) { abort(); }
void NInventory::CTCInventoryBase::OnItemHighlighted(long) { abort(); }
void NInventory::CTCInventoryBase::OnChangeCategory() { abort(); }
void NInventory::CTCInventoryBase::ProcessInventoryInputUp() { abort(); }
void NInventory::CTCInventoryBase::ProcessInventoryInputDown() { abort(); }
void NInventory::CTCInventoryBase::ProcessInventoryInputLeft() { abort(); }
void NInventory::CTCInventoryBase::ProcessInventoryInputRight() { abort(); }
float NInventory::CTCInventoryBase::GetInitialCategoryRepeatDelay() const { abort(); }
float NInventory::CTCInventoryBase::GetContinuedCategoryRepeatDelay() const { abort(); }
float NInventory::CTCInventoryBase::GetInitialItemRepeatDelay() const { abort(); }
float NInventory::CTCInventoryBase::GetContinuedItemRepeatDelay() const { abort(); }
void NInventory::CTCInventoryBase::SetInventorySelectionState(NInventory::CTCInventoryBase::EInventorySelectionState) { abort(); }
CThing * NInventory::CTCInventoryBase::GetItemOfTypeInInventory(long) { abort(); }
bool NInventory::CTCInventoryBase::GetInventoryShoppingValidity() { abort(); }
bool NInventory::CTCInventoryBase::GetInventoryGuildValidity() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonAPressed() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonBPressed() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonXPressed() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonYPressed() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonAHeld() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonBHeld() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonXHeld() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonYHeld() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonAReleased() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonBReleased() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonXReleased() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonYReleased() { abort(); }
void NInventory::CTCInventoryBase::ProcessDPadUpReleased() { abort(); }
void NInventory::CTCInventoryBase::ProcessDPadDownReleased() { abort(); }
void NInventory::CTCInventoryBase::ProcessDPadLeftReleased() { abort(); }
void NInventory::CTCInventoryBase::ProcessDPadRightReleased() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonWhitePressed() { abort(); }
void NInventory::CTCInventoryBase::ProcessButtonWhiteReleased() { abort(); }
void NInventory::CTCInventoryBase::ProcessTradeAction() { abort(); }
void NInventory::CTCInventoryBase::ProcessAugmentAction() { abort(); }
bool NInventory::CTCInventoryBase::ProcessQuestActivateAction() { abort(); }
void NInventory::CTCInventoryBase::IncrementQuestCardItemSelection() { abort(); }
void NInventory::CTCInventoryBase::DecrementQuestCardItemSelection() { abort(); }
void NInventory::CTCInventoryBase::ReturnAllConfiscatedItems() { abort(); }
void NInventory::CTCInventoryBase::RemoveAllConfiscatedItems() { abort(); }
bool NInventory::CTCInventoryBase::GetIsEnabled() const { abort(); }
CWideString NInventory::CTCInventoryBase::GetCategoryNameFromAssociation(CThing *) const { abort(); }
NUISystem::CComponent * NInventory::CTCInventoryBase::ConstructScreen(NUISystem::CComponent *) { abort(); }
void NInventory::CTCInventoryBase::GetItemDescription(CWideString&,long,bool) const { abort(); }
void NInventory::CTCInventoryBase::RemoveItemFromInventory(long,long) { abort(); }
bool NInventory::CTCInventoryBase::AddItemToInventory(CThing&,bool,bool,long,bool) { abort(); }
bool NInventory::CTCInventoryBase::AddItemToInventory(long,long,bool,bool,long,bool) { abort(); }
std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >& NInventory::CTCInventoryBase::GetComponents() { abort(); }
void NInventory::CTCInventoryBase::OnSerialise(CPersistContext&,bool) { abort(); }
void NInventory::CTCInventoryBase::OnItemReturnedToInventory(CThing&) { abort(); }
void NInventory::CTCInventoryBase::OnItemAdded(long,bool,bool) { abort(); }
void NInventory::CTCInventoryBase::ConfiscateItems(const std::vector<NInventory::CItem,std::allocator<NInventory::CItem> >&) { abort(); }
ETCInterfaceType CTCInventory::GetInterfaceType() const { abort(); }
CCharString CTCInventory::GetClassTypeName() const { abort(); }
void CTCInventory::OnKill() { abort(); }
void CTCInventory::Copy(const CTCBase & p0) { abort(); }
void CTCInventory::OnCreate() { abort(); }
void CTCInventory::InitialActivate() { abort(); }
void CTCInventory::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0) { abort(); }
void CTCInventory::OnItemSelected(long p0, bool & p1) { abort(); }
void CTCInventory::OnItemSelectedInHand(CThing & p0, bool & p1) { abort(); }
void CTCInventory::OnItemHighlighted(long p0) { abort(); }
void CTCInventory::OpenInventory() { abort(); }
void CTCInventory::CloseInventory() { abort(); }
void CTCInventory::GlobalUpdate(bool p0) { abort(); }
bool CTCInventory::IsComponentSerialisable() const { abort(); }
void CTCInventory::OnSerialise(CPersistContext & p0, bool p1) { abort(); }
void CTCInventory::OnChangeCategory() { abort(); }
void CTCInventory::GetItemDescription(CWideString & p0, long p1, bool p2) const { abort(); }
const std::vector<long,std::allocator<long> > & CTCInventory::GetInitialItems() const { abort(); }
CWideString CTCInventory::GetScreenName() const { abort(); }
EPlayerMode CTCInventory::GetPlayerMode() const { abort(); }
NHeroInformationScreens::EScreenType CTCInventory::GetScreenType() const { abort(); }
void CTCInventory::ProcessButtonAReleased() { abort(); }
void CTCInventory::ProcessButtonBReleased() { abort(); }
void CTCInventory::ProcessButtonXReleased() { abort(); }
void CTCInventory::ProcessButtonYReleased() { abort(); }
void CTCInventory::ProcessDPadUpReleased() { abort(); }
void CTCInventory::ProcessDPadDownReleased() { abort(); }
void CTCInventory::ProcessDPadLeftReleased() { abort(); }
void CTCInventory::ProcessDPadRightReleased() { abort(); }
void CTCInventory::ProcessInventoryInputLeft() { abort(); }
void CTCInventory::ProcessInventoryInputRight() { abort(); }
void CTCInventory::AddCloseInventoryEvent() { abort(); }
NUISystem::CComponent * CTCInventory::ConstructScreen(NUISystem::CComponent * p0) { abort(); }
NUISystem::CComponent * CTCInventory::ConstructScreenPC(NUISystem::CComponent * p0) { abort(); }
void CTCInventory::RemoveItemFromInventory(long p0, long p1) { abort(); }
void CTCInventory::OnItemAdded(long p0, bool p1, bool p2) { abort(); }
CWideString NHeroInformationScreens::CBase::GetScreenName() const { abort(); }
EPlayerMode NHeroInformationScreens::CBase::GetPlayerMode() const { abort(); }
NHeroInformationScreens::EScreenType NHeroInformationScreens::CBase::GetScreenType() const { abort(); }
bool NHeroInformationScreens::CBase::IsInventoryScreen() const { abort(); }
static char events[512];static unsigned eventCount;static bool recording;
static void event(char c){if(!recording)return;assert(eventCount<511);events[eventCount++]=c;events[eventCount]=0;}
static unsigned stringsLive,submissions,openCalls,updateCalls,enableCalls,scaleCalls,compareCalls;
static bool child,mapPresent;static long defIndex,itemCount;static float fadeAlpha;static unsigned char expectedAlpha;
static NPlayerGui::CDrawGuildSeal* activeDrawer;static CPlayerGuiDef* activeDef;
static CPlayerManager* activePlayers;static CPlayer* activePlayer;static CThingPlayerCreature* activeHero;
static CWorld* activeWorld;static CMainGameComponent* activeComponent;static CGraphicDataBank* activeBank;
static const char* chars(const CCharString&s){return s.PStringData?reinterpret_cast<const char*>(s.PStringData):"";}
static CCharStringData* copyText(const char*x,long n=-1){if(!*x)return 0;if(n<0)n=(long)strlen(x);char*p=(char*)malloc(n+1);assert(p);memcpy(p,x,n);p[n]=0;++stringsLive;return reinterpret_cast<CCharStringData*>(p);}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*x,long n):PStringData(copyText(x,n)){event(strcmp(x,"OBJECT_GUILD_SEAL_1")==0?'S':'K');}
CCharString::CCharString(const CCharString&x):PStringData(copyText(chars(x))){}
CCharString::~CCharString(){if(PStringData){event(strcmp(chars(*this),"OBJECT_GUILD_SEAL_1")==0?'s':'k');free(PStringData);assert(stringsLive);--stringsLive;}}
bool CCharString::operator<(const CCharString&x)const{++compareCalls;event('F');return strcmp(chars(*this),chars(x))<0;}
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
void CIVCountedPointeeBase::Delete()const{abort();}CFailedAllocationHandler::~CFailedAllocationHandler(){}
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
NInventory::CTCInventoryBase::CTCInventoryBase(CThing&h):CTCBase(h){}NInventory::CTCInventoryBase::~CTCInventoryBase(){}
CTCInventory::CTCInventory(CThing&h):NInventory::CTCInventoryBase(h){}CTCInventory::~CTCInventory(){}
bool CThingCreatureBase::IsChild()const{assert(this==activeHero);event('C');return child;}
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&s)const{assert(strcmp(chars(s),"OBJECT_GUILD_SEAL_1")==0);event('I');return defIndex;}
long NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long i)const{assert(i==defIndex);event('N');return itemCount;}
namespace std {template<size_t N> bool bitset<N>::test(size_t n)const{assert(n==TCI_INVENTORY);event('B');return _Unchecked_test(n);}}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CDisplayManager::CDisplayManager(CSystemManager&s):SystemManager(s){}CDisplayManager::~CDisplayManager(){}

CBusyResourceManager::CBusyResourceManager(){}CBusyResourceManager::~CBusyResourceManager(){}
CDisplayManager* GPDisplayManager=0;
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions(){assert(this==GPDisplayManager);event('R');return C2DExtentsI(640,512);}
void CFadeInFadeOutBase::Update(float t){assert(t==0.25f);event('U');++updateCalls;}
bool CFadeInFadeOutBase::IsOpen()const{event('O');return Active&&!Opening&&!Closing;}
float CFadeInFadeOutBase::GetFadeInOutAlpha()const{event('L');return fadeAlpha;}
void CFadeInFadeOutBase::Open(float i,float o,CRGBColour c){assert(i==1.0f&&o==1.0f&&c.R==0&&c.G==0&&c.B==0&&c.A==255);event('E');++openCalls;Active=true;Opening=true;Closing=false;FadeInTime=i;FadeOutTime=o;}
namespace NPlayerGui {
CDrawBase::CDrawBase(CPlayerGui&g,std::vector<CDrawBase*>&):PlayerGui(g),PlayerManager(*activePlayers),World(*activeWorld),Component(*activeComponent),Bank(*activeBank),Drawing(false){}
CDrawBase::~CDrawBase(){}void CDrawBase::ClearPrimitives(){abort();}void CDrawBase::ClearPrimitivesIfNecessary(){abort();}
CDrawGuildSeal::CDrawGuildSeal(CPlayerGui&g,std::vector<CDrawBase*>&v):CDrawBase(g,v),Enabled(false),ReadyToLevelUp(false),FlashAlpha(0),FlashCycles(0),ReminderTime(0),LevelUpGraphic(0),CurrentMessageEndFrame(0),NextMessageDelayEndFrame(0){}
CDrawGuildSeal::~CDrawGuildSeal(){}
void CDrawGuildSeal::SetEnabled(bool x){event(x?'e':'d');++enableCalls;Enabled=x;}
unsigned char CDrawBase::ScaleAlpha(unsigned char a)const{assert(a==expectedAlpha);event('A');++scaleCalls;return (unsigned char)(a*0.5f);}
}
const CPlayerGuiDef& FABLE_FASTCALL CPlayerGui::PeekGuiDef(){event('G');return*activeDef;}
namespace std {
template<> void vector<CEnginePrimitiveHandle>::_M_fill_insert(iterator p,size_type n,const CEnginePrimitiveHandle&x){assert(p==end()&&empty()&&n==3);_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;for(size_type i=0;i<n;++i){_Construct(_M_finish,x);++_M_finish;}}
template<> void vector<C2DVector>::_M_fill_insert(iterator p,size_type n,const C2DVector&x){assert(p==end()&&empty()&&n==2);_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;for(size_type i=0;i<n;++i){_Construct(_M_finish,x);++_M_finish;}}
}
class TestEngine:public CIEngine {public:
 virtual void SetCamera(const CCamera*) { abort(); }
 virtual void SetWindow(const C2DBoxF*) { abort(); }
 virtual void Virtual08_00b27d90() { abort(); }
 virtual void Virtual11_00b23cf0() { abort(); }
 virtual void Virtual12_00b24400() { abort(); }
 virtual void Virtual13_00b242d0() { abort(); }
 virtual bool PreChangeResolution() { abort(); }
 virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long) { abort(); }
 void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned long f){
  assert(f==0xc0&&submissions<3);const CEnginePrimitive2DSprite&s=static_cast<const CEnginePrimitive2DSprite&>(p);
  assert(s.Type==ENGINE_PRIMITIVE_2D_SPRITE&&s.Frame==0&&s.Layer==ENGINE_2D_LAYER_1&&s.BlendMode==ENGINE_PRIMITIVE_BLEND_ALPHA&&s.InterpolationSet==ENGINE_INTERPOLATION_SET_0&&s.StartAngle==0&&s.EndAngle==0&&s.Flags==0&&!s.Texture&&s.ClipBoxIsPos);
  assert(s.UVOffsetsStart.X==0&&s.UVOffsetsStart.Y==0&&s.UVOffsetsEnd.X==0&&s.UVOffsetsEnd.Y==0&&s.Colour.R==255&&s.Colour.G==255&&s.Colour.B==255&&s.Colour.A==(unsigned char)(expectedAlpha*0.5f));
  long index=(long)(&h-&activeDrawer->PrimitiveHandles[0]);assert(index>=0&&index<3);assert(index==0?submissions==0:true);
  if(index==0){assert(s.ClipBox.TLX==40&&s.ClipBox.TLY==92);assert(s.BankIndex==(activeDrawer->LevelUpGraphic==777&&mapPresent?303:302));}
  else{assert(s.ClipBox.TLX==16*index&&s.ClipBox.TLY==32*index&&s.BankIndex==(unsigned long)activeDrawer->LevelUpGraphic);}
  assert(s.ClipBox.BRX==s.ClipBox.TLX&&s.ClipBox.BRY==s.ClipBox.TLY);event(index==0?'0':index==1?'1':'2');++submissions;
 }
};
int main(){unsigned cases=0;
 {CGame game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);activeComponent=&component;
 CGameDefinitionManager definitions;CPlayerManager players(component,definitions);activePlayers=&players;CPlayer player;activePlayer=&player;
 CWorldInit wi;wi.Component=&component;wi.PlayerManager=&players;wi.DefinitionManager=&definitions;CWorld world(wi);activeWorld=&world;
 CSystemManager system;CDisplayManager display(system);GPDisplayManager=&display;
 CGraphicDataBank bank;activeBank=&bank;CPlayerGui gui;CPlayerGuiDef def;activeDef=&def;
 def.GuildSealReminderTimeSeconds=1.0f;def.GuildSealFlashCycleSeconds=2.0f;def.DPadGuildSealLevelUpGraphic=301;def.GuildSealLevelUpGraphic=302;def.GuildSealQuestGraphic=303;def.DPadRingsPos=C2DVector(32,48);def.DPadRingsTopOffset=C2DVector(8,12);
 typedef std::map<CCharString,long>::value_type ScriptValue;std::_Rb_tree_node<ScriptValue>*node=def.ScriptSprites._M_t._M_create_node(ScriptValue(CCharString("HUD_ORB_QUEST_CORE"),777));
 node->_M_parent=def.ScriptSprites._M_t._M_header._M_data;node->_M_left=node->_M_right=0;node->_M_color=true;def.ScriptSprites._M_t._M_header._M_data->_M_parent=node;def.ScriptSprites._M_t._M_header._M_data->_M_left=node;def.ScriptSprites._M_t._M_header._M_data->_M_right=node;def.ScriptSprites._M_t._M_node_count=1;
 CThingPlayerCreature hero;CTCInventory inventory(hero);activeHero=&hero;
 typedef std::pair<ETCInterfaceType,CTCBase*> Entry;Entry entry(TCI_INVENTORY,&inventory);hero.Components._M_start=hero.Components._M_end_of_storage.allocate(1);std::_Construct(hero.Components._M_start,entry);hero.Components._M_finish=hero.Components._M_start+1;hero.Components._M_end_of_storage._M_data=hero.Components._M_finish;
 std::vector<NPlayerGui::CDrawBase*> illustrators;NPlayerGui::CDrawGuildSeal drawer(gui,illustrators);activeDrawer=&drawer;
 drawer.PrimitiveHandles.resize(3);drawer.LevelUpPositions.resize(2);drawer.LevelUpPositions[0]=C2DVector(16,32);drawer.LevelUpPositions[1]=C2DVector(32,64);
 TestEngine engine;CInterpolationInfo ii;ii.Current.GTPredictedTimeSinceLastRenderFrame=0.25f;CCamera camera;CEnginePrimitivePrepareInfo info(ii,engine,false,camera);
 for(int test=0;test<34;++test){
  recording=false;mapPresent=true;def.ScriptSprites._M_t._M_header._M_data->_M_parent=node;def.ScriptSprites._M_t._M_header._M_data->_M_left=node;def.ScriptSprites._M_t._M_header._M_data->_M_right=node;def.ScriptSprites._M_t._M_node_count=1;drawer.Enabled=true;drawer.ReadyToLevelUp=true;drawer.FlashCycles=2;drawer.FlashAlpha=0;drawer.ReminderTime=0;drawer.LevelUpGraphic=777;drawer.Drawing=false;drawer.FadeInOut.Active=true;drawer.FadeInOut.Opening=true;drawer.FadeInOut.Closing=false;world.Paused=false;activeHero=&hero;hero.Dead=false;child=false;defIndex=9;itemCount=1;fadeAlpha=1.0f;
  memset(hero.UsingComponents._M_w,0,sizeof hero.UsingComponents._M_w);hero.UsingComponents._M_w[TCI_INVENTORY/(8*sizeof(unsigned long))]|=1UL<<(TCI_INVENTORY%(8*sizeof(unsigned long)));
  drawer.LevelUpPositions._M_finish=drawer.LevelUpPositions._M_start+2;
  if(test==0)drawer.ReadyToLevelUp=false;if(test==1)world.Paused=true;if(test==2)drawer.Enabled=false;if(test==3)drawer.FlashCycles=-1;if(test==4)drawer.FlashCycles=0;
  if(test==5)fadeAlpha=0;if(test==6)activeHero=0;if(test==7)hero.Dead=true;if(test==8)child=true;if(test==9)memset(hero.UsingComponents._M_w,0,sizeof hero.UsingComponents._M_w);
  if(test==10)defIndex=0;if(test==11)defIndex=-1;if(test==12)itemCount=0;if(test==13)itemCount=-1;
  if(test==14)drawer.LevelUpGraphic=0;if(test==15)drawer.LevelUpPositions._M_finish=drawer.LevelUpPositions._M_start;if(test==16){drawer.LevelUpPositions._M_finish=drawer.LevelUpPositions._M_start;activeHero=0;}
  if(test==17)drawer.ReminderTime=0.75f;if(test==18)drawer.ReminderTime=1.0f;if(test==19){drawer.ReminderTime=1.0f;world.Paused=true;}
  if(test==20){drawer.FadeInOut.Opening=false;}if(test==21){drawer.FadeInOut.Active=false;}if(test==22){drawer.FadeInOut.Active=false;drawer.FlashCycles=1;}
  if(test==23)fadeAlpha=0.5f;if(test==24)drawer.LevelUpGraphic=888;if(test==25){drawer.LevelUpGraphic=0;activeHero=0;}
  if(test==26){drawer.Enabled=false;drawer.ReminderTime=1.0f;}if(test==27){drawer.Enabled=false;drawer.ReminderTime=1.0f;world.Paused=true;}
  if(test==28){drawer.FadeInOut.Closing=true;}if(test==29){drawer.FadeInOut.Active=false;world.Paused=true;}if(test==30){drawer.FlashCycles=0;drawer.ReminderTime=1.0f;}if(test==31){drawer.ReadyToLevelUp=false;drawer.FlashCycles=0;}
  if(test==32){mapPresent=false;def.ScriptSprites._M_t._M_header._M_data->_M_parent=0;def.ScriptSprites._M_t._M_header._M_data->_M_left=def.ScriptSprites._M_t._M_header._M_data;def.ScriptSprites._M_t._M_header._M_data->_M_right=def.ScriptSprites._M_t._M_header._M_data;def.ScriptSprites._M_t._M_node_count=0;}if(test==33)drawer.LevelUpPositions._M_finish=drawer.LevelUpPositions._M_start+1;
  unsigned queued=(unsigned)drawer.LevelUpPositions.size();
  bool reminder=drawer.ReadyToLevelUp&&!world.Paused;bool reminderEnable=reminder&&drawer.ReminderTime+0.25f>1.0f;bool run=drawer.FlashCycles>0&&(drawer.Enabled||reminderEnable);bool shown=run&&drawer.ReadyToLevelUp&&fadeAlpha>0;
  bool seal=shown&&activeHero&&!hero.Dead&&!child&&test!=9&&defIndex>0&&itemCount>0;bool positions=shown&&test!=15&&test!=16;long originalCycles=drawer.FlashCycles;bool inactive=!drawer.FadeInOut.Active;bool open=drawer.FadeInOut.Active&&!drawer.FadeInOut.Opening&&!drawer.FadeInOut.Closing;float expectedReminder=drawer.ReminderTime+(reminder?0.25f:0);long expectedGraphic=reminderEnable?301:drawer.LevelUpGraphic;if(positions&&expectedGraphic==0)expectedGraphic=301;
  expectedAlpha=(unsigned char)(fadeAlpha*255.0f);submissions=openCalls=updateCalls=enableCalls=scaleCalls=compareCalls=eventCount=0;events[0]=0;recording=true;drawer.Draw(info);recording=false;
  assert(drawer.ReminderTime==expectedReminder&&drawer.LevelUpGraphic==expectedGraphic);assert(submissions==(unsigned)(seal+queued*positions)&&scaleCalls==submissions&&drawer.Drawing==(seal||positions));
  assert(updateCalls==(unsigned)(run&&!world.Paused));assert(openCalls==(unsigned)(run&&!world.Paused&&inactive&&originalCycles>1));
  assert(drawer.FlashCycles==originalCycles-(run&&!world.Paused&&inactive?1:0));assert(enableCalls==(unsigned)(reminderEnable+(drawer.FlashCycles==0)));
  if(run){assert(drawer.FlashAlpha==expectedAlpha);if(!world.Paused&&open)assert(drawer.FadeInOut.Closing&&!drawer.FadeInOut.Opening);}
  assert((strchr(events,'I')!=0)==(shown&&activeHero&&!hero.Dead&&!child&&test!=9));assert((strchr(events,'N')!=0)==(shown&&activeHero&&!hero.Dead&&!child&&test!=9&&defIndex>0));assert((strchr(events,'R')!=0)==seal);
  if(seal){const char* k=strchr(events,'K');const char* r=strchr(events,'R');const char* a=strchr(events,'A');const char* zero=strchr(events,'0');assert(k&&r&&a&&zero&&k<r&&k<a&&r<zero&&a<zero&&compareCalls==(unsigned)(mapPresent?2:0));}
  if(positions){const char*one=strchr(events,'1');const char*two=strchr(events,'2');assert(one);if(queued==2)assert(two&&one<two);else assert(!two);if(seal)assert(strchr(events,'0')<one);}
  assert((strchr(events,'P')!=0)==shown&&(strchr(events,'H')!=0)==shown);
  assert((strchr(events,'C')!=0)==(shown&&activeHero&&!hero.Dead));assert((strchr(events,'B')!=0)==(shown&&activeHero&&!hero.Dead&&!child));
  char expected[512],alternate[512];expected[0]=0;
  if(reminder)strcat(expected,"G");if(reminderEnable)strcat(expected,"e");
  if(run){strcat(expected,"G");if(!world.Paused){strcat(expected,"UO");if(inactive&&originalCycles>1)strcat(expected,"E");}strcat(expected,"L");
   if(shown){strcat(expected,"PH");if(activeHero&&!hero.Dead){strcat(expected,"C");if(!child){strcat(expected,"B");if(test!=9){strcat(expected,"SIs");if(defIndex>0){strcat(expected,"N");if(itemCount>0)strcat(expected,mapPresent?"KFFkAR0":"KkAR0");}}}}
    if(positions){strcat(expected,"A1");if(queued==2)strcat(expected,"A2");}
   }
  }
  if(drawer.FlashCycles==0)strcat(expected,"d");strcpy(alternate,expected);char* ar=strstr(alternate,"AR0");if(ar){ar[0]='R';ar[1]='A';}
  // Argument evaluation order may place alpha before or after anchoring; all sequenced calls are exact.
  assert(strcmp(events,expected)==0||strcmp(events,alternate)==0);
  assert(stringsLive==1);++cases;
 }
 def.ScriptSprites._M_t._M_header._M_data->_M_parent=node;def.ScriptSprites._M_t._M_header._M_data->_M_left=node;def.ScriptSprites._M_t._M_header._M_data->_M_right=node;def.ScriptSprites._M_t._M_node_count=1;drawer.LevelUpPositions._M_finish=drawer.LevelUpPositions._M_start+2;GPDisplayManager=0;
 }
 assert(stringsLive==0&&cases==34);puts("0064a639_TEST PASS");return 0;
}