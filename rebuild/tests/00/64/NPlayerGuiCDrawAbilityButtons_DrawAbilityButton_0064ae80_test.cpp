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

#include "engine/EPlayerMode.h"
#define private public
#define protected public
// 0064ae80 NPlayerGui::CDrawAbilityButtons::DrawAbilityButton
// Leaf: ?DrawAbilityButton@CDrawAbilityButtons@NPlayerGui@@IAEXABVCEnginePrimitivePrepareInfo@@AAV?$vector@V?$CDefPointer@$$CBVCHeroAbilityDef@@@@V?$allocator@V?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@@std@@JJJM@Z
// Flags: none
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_ABILITY_CONTEXT_PASS23
#define FABLE_RETAIL_CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CURSOR_ORIGINAL_DECLARATIONS_PASS22
#define FABLE_RETAIL_NPLAYERGUI_CDRAWABILITYBUTTONS_PASS23
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
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
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LIFETIMES_PASS23
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_USE
#define FABLE_RETAIL_CTCSPECIALABILITIES_GUI_DRAW_PASS23
#include "engine/EEnginePrimitiveType.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/NPlayerGui_CDrawAbilityButtons.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/ThingAlive.h"
#include "engine/NPlayerGui_Graphics.h"

#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;
// Genuine same-TU FableWin020d9289 body; native uses float640/480 literals.
inline C2DVector FABLE_FASTCALL NPlayerGui::GFAnchorToBottomRight(const C2DVector& pos)
{return C2DVector((float)GPDisplayManager->GetRenderTargetDimensions().X+pos.X-640.0f,(float)GPDisplayManager->GetRenderTargetDimensions().Y+pos.Y-480.0f);}
// Original FableWin020d93ce forwarding body, ordinary out-of-line context.
unsigned char NPlayerGui::CDrawBase::ScaleAlpha(unsigned char alpha)const{return PlayerGui.ScaleAlpha(alpha);}
// Genuine native00645099 / original both-PDB seven-argument DrawButton body.
// Ordinary visibility fixes caller0064af89 schedule; all324 helper bytes independently match.
// Existing GetAlpha/IsShowSpecialAbilityButtons and ScaleAlpha retain their original owners.
void NPlayerGui::CDrawAbilityButtons::DrawButton(const CEnginePrimitivePrepareInfo& info,const C2DVector& position,float alpha,long primitive_index,long graphic_index,bool special,EEngine2DLayer layer)
{
 const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
 std::vector<CEnginePrimitiveHandle>& handles=special?SpecialPrimitiveHandles:PrimitiveHandles;
 bool available=true;
 if(alpha<0.0f){available=false;alpha=-alpha;}
 unsigned char actual_alpha=PlayerGui.GetAlpha();
 if(PlayerGui.IsShowSpecialAbilityButtons()&&actual_alpha<255)actual_alpha=255;
 if(graphic_index>0&&alpha>0.0f){
  float icon_alpha=alpha*actual_alpha;
  if(!available)icon_alpha=160.0f;
  else if(PlayerGui.IsShowSpecialAbilityButtons()&&actual_alpha<255)icon_alpha=special?255.0f:0.0f;
  CEnginePrimitive2DSprite abxy_icon_sprite(0,GFToScreen(position),layer,graphic_index,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,ScaleAlpha((unsigned char)icon_alpha),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
  info.Engine.SetPersistentPrimitive(handles[primitive_index],abxy_icon_sprite,0xc0);
 }
}
void NPlayerGui::CDrawAbilityButtons::DrawAbilityButton(const CEnginePrimitivePrepareInfo& info,std::vector<CDefPointer<const CHeroAbilityDef> >& ability_defs,long button_index,long ability_index,long level_offset,float alpha)
{
 const CThingPlayerCreature* phero=GetPlayer().PeekPControlledCreature();
 if(IS_THING_ALIVE(phero)){
  std::vector<long> position_mapping;
  GetAbilityPositionMapping(position_mapping);
  const CTCInventoryAbilities* ptc_inventory_abilities=0;
  phero->PeekTC(&ptc_inventory_abilities);
  if(ptc_inventory_abilities){
   long num_abilities_to_draw=((long)ability_defs.size()+2)/Columns*Columns;
   if(ability_index<num_abilities_to_draw&&(unsigned long)ability_index<ability_defs.size()){
    const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
    long handle_index=gui_def.AbilityButtons.size()+ability_index+7;
    C2DVector button_pos=GFAnchorToBottomRight(gui_def.ABXYRingsPos);
    // Fidelity: cache the genuine STLport iterator after anchoring; no substitute container model.
    std::vector<CAbilityButtonGuiDef>::const_iterator button_types=gui_def.SpecialAbilityButtons.begin();
    button_pos.X+=button_types[position_mapping[button_index]].Offset.X;
    // Fidelity: named running sum preserves original addition sequence.
    float button_y=button_pos.Y;
    button_y+=button_types[position_mapping[0]].Offset.Y;
    button_y+=level_offset*gui_def.ABXYAbilityLevelOffset.Y;
    button_y+=gui_def.ABXYAbilityLevelInitialOffset.Y;
    button_pos.Y=button_y;
    CDefPointer<const CHeroAbilityDef> pdef;
    if((unsigned long)ability_index<ability_defs.size())pdef=ability_defs[ability_index];
    float actual_alpha=alpha;
    long graphic=gui_def.ABXYAbilityUnavailableGraphic;
    if(pdef.Get()){
     graphic=pdef->IconGraphicIndex;
     bool can_use_ability=true;
     if(!ptc_inventory_abilities->IsAbleToUseAbility(pdef->Ability,true)){
      if(pdef->Ability!=HERO_ABILITY_PHYSICAL_SHIELD_SPELL||!phero->HasTC(TCI_PHYSICAL_SHIELD_SPELL))can_use_ability=false;
     }else{
      const CTCSpecialAbilities* ptc_special_abilites;
      if(phero->PeekTC(&ptc_special_abilites)&&!ptc_special_abilites->IsAbilityAvailable(pdef->Ability))can_use_ability=false;
     }
     if(!can_use_ability){
      long unusable_graphic=gui_def.ABXYAbilityUncastableGraphic;
      long unusable_handle=handle_index+20;
      if((unsigned long)unusable_handle<PrimitiveHandles.size()){
       EEngine2DLayer layer=ENGINE_2D_LAYER_0;
       if(position_mapping[button_index]==1)layer=ENGINE_2D_LAYER_1;
       CEnginePrimitive2DSprite unusable_sprite(0,GFToScreen(button_pos),layer,unusable_graphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,ScaleAlpha(128),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
       info.Engine.SetPersistentPrimitive(PrimitiveHandles[unusable_handle],unusable_sprite,0x8c0);
      }
     }
     if(pdef->Ability==HERO_ABILITY_PHYSICAL_SHIELD_SPELL&&phero->HasTC(TCI_PHYSICAL_SHIELD_SPELL))graphic=gui_def.ABXYAbilityShieldOffGraphic;
    }else actual_alpha/=3.0f;
    EEngine2DLayer layer=ENGINE_2D_LAYER_1;
    if(position_mapping[button_index]==1)layer=ENGINE_2D_LAYER_2;
    DrawButton(info,button_pos,actual_alpha,handle_index,graphic,false,layer);
   }
  }
 }
}

#include "engine/CDefString.h"
#include "engine/CGame.h"
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CWorldInit.h"
#include "engine/CWorld.h"
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
CWideString NHeroInformationScreens::CBase::GetScreenName() const { abort(); }
EPlayerMode NHeroInformationScreens::CBase::GetPlayerMode() const { abort(); }
NHeroInformationScreens::EScreenType NHeroInformationScreens::CBase::GetScreenType() const { abort(); }
bool NHeroInformationScreens::CBase::IsInventoryScreen() const { abort(); }
void CAbilityButtonGuiDef::Transfer(CPersistContext&) { abort(); }
void CAbilityButtonGuiDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CAbilityButtonGuiDef::GetSizeofClass() { abort(); }
void NPlayerGui::CDrawAbilityButtons::Draw(const CEnginePrimitivePrepareInfo&) { abort(); }
void NPlayerGui::CDrawAbilityButtons::ClearPrimitivesIfNecessary() { abort(); }
long NPlayerGui::CDrawAbilityButtons::GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef&)const { abort(); }
void NPlayerGui::CDrawAbilityButtons::DrawAbilityList(const CEnginePrimitivePrepareInfo&,float) { abort(); }
void NPlayerGui::CDrawAbilityButtons::GetAbilityButtonMapping(std::vector<long>&) { abort(); }
void NPlayerGui::CDrawAbilityButtons::DrawExpressionList(const CEnginePrimitivePrepareInfo&,float) { abort(); }
void NPlayerGui::CDrawAbilityButtons::DrawExpressionButton(const CEnginePrimitivePrepareInfo&,std::vector<CDefPointer<const CExpressionDef> >&,long,long,long,float) { abort(); }

CCharString::CCharString():PStringData(0){}CCharString::~CCharString(){assert(!PStringData);}
static char events[512];static unsigned eventCount;static bool recording;
static void event(char c){if(recording){assert(eventCount<511);events[eventCount++]=c;events[eventCount]=0;}}
static CPlayerManager* activePlayers;static CPlayer* activePlayer;static CThingPlayerCreature* activeHero;
static CWorld* activeWorld;static CMainGameComponent* activeComponent;static CGraphicDataBank* activeBank;
static CPlayerGuiDef* activeDef;static NPlayerGui::CDrawAbilityButtons* activeDrawer;
static CTCInventoryAbilities* activeInventory;static CTCSpecialAbilities* activeSpecial;
static CHeroAbilityDef* activeAbility;static CPlayerGui* activeGui;
static unsigned mappingCalls,canUseCalls,availableCalls,guiCalls,submissions,drawSubmissions,dimensionCalls;
static bool expectedSpecial;static unsigned char expectedByteAlpha;
static bool canUse,available;static long buttonIndex,abilityIndex,levelOffset,expectedHandle,expectedGraphic;
static float inputAlpha,expectedAlpha;static EEngine2DLayer expectedLayer;static float expectedX,expectedY;
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
CTCInventoryAbilities::CTCInventoryAbilities(CThing&h):NInventory::CTCInventoryBase(h){}CTCInventoryAbilities::~CTCInventoryAbilities(){}
CTCSpecialAbilities::CTCSpecialAbilities(CThing&t):CTCBase(t),AbilityMutexLock(false),UsingBed(false),CurrentAbility(HERO_ABILITY_NULL),Creature(static_cast<CThingCreatureBase&>(t)){}CTCSpecialAbilities::~CTCSpecialAbilities(){}
class FixtureInventory:public CTCInventoryAbilities {public:void OnItemSelected(long,bool&){abort();}FixtureInventory(CThing&t):CTCInventoryAbilities(t){}ETCInterfaceType GetInterfaceType()const{abort();return TCI_HERO_ABILITIES;}CCharString GetClassTypeName()const{abort();return CCharString();}};
class FixtureSpecial:public CTCSpecialAbilities {public:FixtureSpecial(CThing&t):CTCSpecialAbilities(t){}ETCInterfaceType GetInterfaceType()const{abort();return TCI_SPECIAL_ABILITIES;}CCharString GetClassTypeName()const{abort();return CCharString();}};
class FixtureAbility:public CHeroAbilityDef {public:void Transfer(CPersistContext&){abort();}void Copy(const CDefClassBase*){abort();}unsigned long GetSizeofClass(){abort();return 0;}};
namespace std {template<size_t N> bool bitset<N>::test(size_t n)const{assert(n==TCI_HERO_ABILITIES||n==TCI_SPECIAL_ABILITIES||n==TCI_PHYSICAL_SHIELD_SPELL);event(n==TCI_HERO_ABILITIES?'B':n==TCI_SPECIAL_ABILITIES?'S':'H');return _Unchecked_test(n);}}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}CBaseIntelligentPointer::~CBaseIntelligentPointer(){assert(!PData);}
CDisplayManager::CDisplayManager(CSystemManager&s):SystemManager(s){}CDisplayManager::~CDisplayManager(){}
CBusyResourceManager::CBusyResourceManager(){}CBusyResourceManager::~CBusyResourceManager(){}
CDisplayManager* GPDisplayManager=0;
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions(){assert(this==GPDisplayManager);event('R');++dimensionCalls;return C2DExtentsI(800,600);}
bool CTCInventoryAbilities::IsAbleToUseAbility(EHeroAbility a,bool cost)const{assert(this==activeInventory&&activeAbility&&a==activeAbility->Ability&&cost&&activeAbility->IVRefCount==3);event('U');++canUseCalls;return canUse;}
bool CTCSpecialAbilities::IsAbilityAvailable(EHeroAbility a)const{assert(this==activeSpecial&&activeAbility&&a==activeAbility->Ability&&activeAbility->IVRefCount==3);event('A');++availableCalls;return available;}
const CPlayerGuiDef& FABLE_FASTCALL CPlayerGui::PeekGuiDef(){event('G');++guiCalls;return*activeDef;}
namespace NPlayerGui {
CDrawBase::CDrawBase(CPlayerGui&g,std::vector<CDrawBase*>&):PlayerGui(g),PlayerManager(*activePlayers),World(*activeWorld),Component(*activeComponent),Bank(*activeBank),Drawing(false){}
void CDrawBase::ClearPrimitives(){abort();}void CDrawBase::ClearPrimitivesIfNecessary(){abort();}
CDrawAbilityButtons::CDrawAbilityButtons(CPlayerGui&g,std::vector<CDrawBase*>&v):CDrawBase(g,v),Columns(3){}CDrawAbilityButtons::~CDrawAbilityButtons(){}
void CDrawAbilityButtons::GetAbilityPositionMapping(std::vector<long>&v){assert(this==activeDrawer&&v.empty());event('V');++mappingCalls;v._M_start=v._M_end_of_storage.allocate(3);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+3;for(unsigned i=0;i<3;++i){std::_Construct(v._M_finish,2-(long)i);++v._M_finish;}}
}
namespace std {template<> void vector<CEnginePrimitiveHandle>::_M_fill_insert(iterator p,size_type n,const CEnginePrimitiveHandle&x){assert(p==end()&&empty());if(!n)return;_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;for(size_type i=0;i<n;++i){_Construct(_M_finish,x);++_M_finish;}}}
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
if(f==0xc0){
 assert(activeDrawer&&drawSubmissions==0);std::vector<CEnginePrimitiveHandle>& handles=expectedSpecial?activeDrawer->SpecialPrimitiveHandles:activeDrawer->PrimitiveHandles;
 assert(expectedHandle>=0&&(unsigned long)expectedHandle<handles.size()&&&h==&handles[expectedHandle]);
 const CEnginePrimitive2DSprite&s=static_cast<const CEnginePrimitive2DSprite&>(p);
 assert(p.Type==ENGINE_PRIMITIVE_2D_SPRITE&&p.InterpolationSet==ENGINE_INTERPOLATION_SET_0&&s.Layer==expectedLayer&&s.BankIndex==(unsigned long)expectedGraphic&&s.Frame==0&&s.BlendMode==ENGINE_PRIMITIVE_BLEND_ALPHA&&s.StartAngle==0&&s.EndAngle==0&&s.Flags==0&&!s.Texture&&s.ClipBoxIsPos);
 assert(s.ClipBox.TLX==expectedX&&s.ClipBox.BRX==expectedX&&s.ClipBox.TLY==expectedY&&s.ClipBox.BRY==expectedY&&s.Colour.R==255&&s.Colour.G==255&&s.Colour.B==255&&s.Colour.A==expectedByteAlpha&&s.UVOffsetsStart.X==0&&s.UVOffsetsStart.Y==0&&s.UVOffsetsEnd.X==0&&s.UVOffsetsEnd.Y==0);
 assert(!activeAbility||activeAbility->IVRefCount==3);event('D');++drawSubmissions;return;
}
assert(f==0x8c0&&submissions==0&&activeDrawer->PrimitiveHandles.size()>(unsigned long)(expectedHandle+20)&&&h==&activeDrawer->PrimitiveHandles[expectedHandle+20]);const CEnginePrimitive2DSprite&s=static_cast<const CEnginePrimitive2DSprite&>(p);assert(p.Type==ENGINE_PRIMITIVE_2D_SPRITE&&p.InterpolationSet==ENGINE_INTERPOLATION_SET_0&&s.Layer==(expectedLayer==ENGINE_2D_LAYER_2?ENGINE_2D_LAYER_1:ENGINE_2D_LAYER_0)&&s.BankIndex==501&&s.Frame==0&&s.BlendMode==ENGINE_PRIMITIVE_BLEND_ALPHA&&s.StartAngle==0&&s.EndAngle==0&&s.Flags==0&&!s.Texture&&s.ClipBoxIsPos);assert(s.ClipBox.TLX==expectedX&&s.ClipBox.BRX==expectedX&&s.ClipBox.TLY==expectedY&&s.ClipBox.BRY==expectedY&&s.Colour.R==255&&s.Colour.G==255&&s.Colour.B==255&&s.Colour.A==64&&s.UVOffsetsStart.X==0&&s.UVOffsetsStart.Y==0&&s.UVOffsetsEnd.X==0&&s.UVOffsetsEnd.Y==0);event('E');++submissions;}
};
static void setBit(CThing&h,unsigned n){h.UsingComponents._M_w[n/(8*sizeof(unsigned long))]|=1UL<<(n%(8*sizeof(unsigned long)));}
int main(){unsigned cases=0;
 {CGame game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);activeComponent=&component;CGameDefinitionManager definitions;CPlayerManager players(component,definitions);activePlayers=&players;CPlayer player;activePlayer=&player;
 CWorldInit wi;wi.Component=&component;wi.PlayerManager=&players;wi.DefinitionManager=&definitions;CWorld world(wi);activeWorld=&world;CSystemManager system;CDisplayManager display(system);GPDisplayManager=&display;CGraphicDataBank bank;activeBank=&bank;
 CPlayerGui gui;activeGui=&gui;gui.Alpha=0.5f;gui.ShowSpecialAbilityButtons=false;CPlayerGuiDef def;activeDef=&def;def.ABXYRingsPos=C2DVector(32,48);def.ABXYAbilityLevelInitialOffset=C2DVector(128,12);def.ABXYAbilityLevelOffset=C2DVector(256,8);def.ABXYAbilityUnavailableGraphic=500;def.ABXYAbilityUncastableGraphic=501;def.ABXYAbilityShieldOffGraphic=502;
 def.AbilityButtons._M_start=def.AbilityButtons._M_end_of_storage.allocate(2);def.AbilityButtons._M_finish=def.AbilityButtons._M_start;def.AbilityButtons._M_end_of_storage._M_data=def.AbilityButtons._M_start+2;for(unsigned i=0;i<2;++i){new(def.AbilityButtons._M_finish)CAbilityButtonGuiDef;++def.AbilityButtons._M_finish;}
 def.SpecialAbilityButtons._M_start=def.SpecialAbilityButtons._M_end_of_storage.allocate(3);def.SpecialAbilityButtons._M_finish=def.SpecialAbilityButtons._M_start;def.SpecialAbilityButtons._M_end_of_storage._M_data=def.SpecialAbilityButtons._M_start+3;for(unsigned i=0;i<3;++i){new(def.SpecialAbilityButtons._M_finish)CAbilityButtonGuiDef;def.SpecialAbilityButtons._M_finish->Offset=C2DVector(8.0f+16.0f*i,i==2?64.0f:16.0f+16.0f*i);++def.SpecialAbilityButtons._M_finish;}
 CThingPlayerCreature hero;FixtureInventory inventory(hero);FixtureSpecial special(hero);activeInventory=&inventory;activeSpecial=&special;FixtureAbility ability;ability.IconGraphicIndex=503;
 typedef std::pair<ETCInterfaceType,CTCBase*> Entry;Entry e(TCI_HERO_ABILITIES,&inventory);hero.Components._M_start=hero.Components._M_end_of_storage.allocate(2);std::_Construct(hero.Components._M_start,e);std::_Construct(hero.Components._M_start+1,Entry(TCI_SPECIAL_ABILITIES,&special));hero.Components._M_finish=hero.Components._M_start+2;hero.Components._M_end_of_storage._M_data=hero.Components._M_finish;
 TestEngine engine;CInterpolationInfo ii;CCamera camera;CEnginePrimitivePrepareInfo info(ii,engine,false,camera);
 for(int test=0;test<160;++test){recording=false;activeHero=&hero;hero.Dead=false;memset(hero.UsingComponents._M_w,0,sizeof hero.UsingComponents._M_w);setBit(hero,TCI_HERO_ABILITIES);hero.Components._M_start->second=&inventory;
 bool hasSpecial=((test/6)%2)!=0,hasShield=((test/12)%2)!=0;canUse=((test/24)%2)!=0;available=((test/3)%2)!=0;if(hasSpecial)setBit(hero,TCI_SPECIAL_ABILITIES);if(hasShield)setBit(hero,TCI_PHYSICAL_SHIELD_SPELL);
 buttonIndex=test%3;abilityIndex=(test/3)%4;levelOffset=test%5-2;inputAlpha=(float)(test%3)*0.75f;bool nullDef=test%3==0;ability.Ability=test%3==1?HERO_ABILITY_PHYSICAL_SHIELD_SPELL:HERO_ABILITY_FORCE_PUSH;activeAbility=nullDef?0:&ability;
 if(test==0)activeHero=0;if(test==1)hero.Dead=true;if(test==2)memset(hero.UsingComponents._M_w,0,sizeof hero.UsingComponents._M_w);if(test==3)hero.Components._M_start->second=0;if(test==4)abilityIndex=-1;if(test==5)abilityIndex=6;if(test==6)abilityIndex=4;
 std::vector<CDefPointer<const CHeroAbilityDef> > abilities;unsigned count=test==7?0:test==8?1:4;abilities._M_start=abilities._M_end_of_storage.allocate(count);abilities._M_finish=abilities._M_start;abilities._M_end_of_storage._M_data=abilities._M_start+count;for(unsigned i=0;i<count;++i){std::_Construct(abilities._M_finish,CDefPointer<const CHeroAbilityDef>());++abilities._M_finish;}
 if(activeAbility&&abilityIndex>=0&&(unsigned long)abilityIndex<count){abilities[abilityIndex].Object=&ability;ability.AddRef();}
 std::vector<NPlayerGui::CDrawBase*> v;NPlayerGui::CDrawAbilityButtons drawer(gui,v);activeDrawer=&drawer;if(test%11==0)drawer.Columns=2;if(test==8)drawer.Columns=4;unsigned handleCount=test%7==0?13:test%7==1?29:test%7==2?30:40;drawer.PrimitiveHandles.resize(handleCount);
 bool live=test!=0&&test!=1;bool inventoryOK=live&&test!=2&&test!=3;long rounded=((long)count+2)/drawer.Columns*drawer.Columns;bool drawn=inventoryOK&&abilityIndex<rounded&&(unsigned long)abilityIndex<count;bool pdef=drawn&&activeAbility;
 expectedHandle=2+abilityIndex+7;bool shield=pdef&&ability.Ability==HERO_ABILITY_PHYSICAL_SHIELD_SPELL;bool allowed=canUse?(!hasSpecial||available):(shield&&hasShield);bool overlay=pdef&&!allowed&&(unsigned long)(expectedHandle+20)<handleCount;
 expectedGraphic=!pdef?500:shield&&hasShield?502:503;expectedAlpha=pdef?inputAlpha:inputAlpha/3.0f;expectedLayer=buttonIndex==1?ENGINE_2D_LAYER_2:ENGINE_2D_LAYER_1;expectedX=192.0f+8.0f+16.0f*(2-buttonIndex);expectedY=168.0f+64.0f+levelOffset*8.0f+12.0f;
 expectedSpecial=false;expectedByteAlpha=(unsigned char)((unsigned char)(expectedAlpha*127.0f)*0.5f);
 mappingCalls=canUseCalls=availableCalls=guiCalls=submissions=drawSubmissions=dimensionCalls=eventCount=0;events[0]=0;recording=true;drawer.DrawAbilityButton(info,abilities,buttonIndex,abilityIndex,levelOffset,inputAlpha);recording=false;
 assert(mappingCalls==(unsigned)live&&guiCalls==(unsigned)(drawn?2:0)&&drawSubmissions==(unsigned)(drawn&&expectedAlpha>0)&&submissions==(unsigned)overlay&&dimensionCalls==(unsigned)(drawn?2:0)&&canUseCalls==(unsigned)pdef&&availableCalls==(unsigned)(pdef&&canUse&&hasSpecial));
 char expected[512];strcpy(expected,"PH");if(live){strcat(expected,"VB");if(drawn){strcat(expected,"GRR");if(pdef){strcat(expected,"U");if(!canUse){if(shield)strcat(expected,"H");}else{strcat(expected,"S");if(hasSpecial)strcat(expected,"A");}if(overlay)strcat(expected,"E");if(shield)strcat(expected,"H");}strcat(expected,"G");if(expectedAlpha>0)strcat(expected,"D");}}
 if(strcmp(events,expected)){fprintf(stderr,"trace %d: %s expected %s\n",test,events,expected);abort();}
 assert(ability.IVRefCount==(unsigned long)(activeAbility&&abilityIndex>=0&&(unsigned long)abilityIndex<count?2:1));++cases;
 }

 // Execute the genuine324B helper separately across table selection, signed-alpha and graphic gates.
 activeAbility=0;activeHero=&hero;
 {std::vector<NPlayerGui::CDrawBase*> v;NPlayerGui::CDrawAbilityButtons drawer(gui,v);activeDrawer=&drawer;drawer.PrimitiveHandles.resize(3);drawer.SpecialPrimitiveHandles.resize(3);
 const float alphas[5]={0.0f,0.25f,0.5f,1.0f,-0.25f};const long graphics[3]={-1,0,503};
 for(unsigned specialFlag=0;specialFlag<2;++specialFlag)for(unsigned show=0;show<2;++show)for(unsigned guiFull=0;guiFull<2;++guiFull)for(unsigned ai=0;ai<5;++ai)for(unsigned gi=0;gi<3;++gi){
  gui.ShowSpecialAbilityButtons=show!=0;gui.Alpha=guiFull?1.0f:0.5f;float a=alphas[ai];unsigned guiByte=guiFull||show?255:127;
  unsigned unscaled=a<0?160:(unsigned)(a*guiByte);expectedByteAlpha=(unsigned char)(unscaled*gui.Alpha);
  expectedSpecial=specialFlag!=0;expectedHandle=1;expectedGraphic=graphics[gi];expectedLayer=specialFlag?ENGINE_2D_LAYER_2:ENGINE_2D_LAYER_0;expectedX=16;expectedY=32;
  guiCalls=submissions=drawSubmissions=dimensionCalls=eventCount=0;events[0]=0;recording=true;
  drawer.DrawButton(info,C2DVector(expectedX,expectedY),a,expectedHandle,expectedGraphic,expectedSpecial,expectedLayer);recording=false;
  bool submitted=expectedGraphic>0&&a!=0;assert(guiCalls==1&&submissions==0&&drawSubmissions==(unsigned)submitted&&dimensionCalls==0&&!strcmp(events,submitted?"GD":"G"));++cases;
 }
 }
 assert(ability.IVRefCount==1);GPDisplayManager=0;
 }
 assert(cases==280);puts("0064ae80_TEST PASS");return 0;
}