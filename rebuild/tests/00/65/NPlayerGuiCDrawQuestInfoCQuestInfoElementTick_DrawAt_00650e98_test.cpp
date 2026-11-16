// Self-contained behavior fixture: actual shared classes and pasted production function.
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
enum EEnginePrimitiveType {FableTestSpriteType=34};
#include <strings.h>
#define _stricmp strcasecmp
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define private public
#define protected public
// 0x00650E98 NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick::DrawAt -- draw a quest caption or graphic and its optional completion tick.
// Leaf: ?DrawAt@CQuestInfoElementTick@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 0x00650E98, 987 bytes, thiscall.
// Flags: none
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_CDRAWQUESTINFO_CTOR_PASS23
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CFONTBANK_HEIGHT
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CSUBTITLERENDERER_QUESTTICK_PASS23
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CFontBank.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CMainGameComponent.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CPlayerGuiDef.h"

void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&,C2DVector&);
namespace NPlayerGui {
inline C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF& box){return box;}
float CDrawQuestInfo::CQuestInfoElementTick::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
 Clear();
 float max_width;
 float max_height;
 long tick_primitive_index;
 if(Graphic>0){
  PrimitiveHandles.resize(2);
  const CGraphicDataBank& graphic_bank=CPlayerGui::Get().GetQuestInfo().Bank;
  tick_primitive_index=1;
  float graphic_width=(float)graphic_bank.GetGraphicFrameWidth(Graphic,0);
  float graphic_height=(float)graphic_bank.GetGraphicFrameHeight(Graphic,0);
  max_width=graphic_width;
  max_height=graphic_height;
  C2DBoxF sprite_box(pos,C2DExtentsF(graphic_width*Scale,graphic_height*Scale));
  CEnginePrimitive2DSprite graphic_sprite(0,GFToScreen(sprite_box),ENGINE_2D_LAYER_6,Graphic,0,
   ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,CPlayerGui::Get().GetAlpha(),255,255,255,2,
   C2DVector(0.0f,0.0f),C2DVector(0.0f,0.0f));
  info.Engine.SetPersistentPrimitive(PrimitiveHandles[0],graphic_sprite,0xc0);
  CWideString text_string=CPlayerGui::Get().GetSubtitleRenderer().GetTextFromEmbeddedTagGraphic(Graphic);
  if(text_string!=L""){
   const C2DVector& text_offset=CPlayerGui::Get().GetSubtitleRenderer().GetTextOffsetFromEmbeddedTagGraphic(Graphic);
   C2DVector vtext_pos;
   C2DVector text_pos=pos+text_offset-C2DVector(4.0f,4.0f);
   GFACoordToVCoord(text_pos,vtext_pos);
   const CFontBank& font_bank=CMainGameComponent::Get()->PeekMainFont();
   CPlayerGui::Get().Draw2DText(vtext_pos,text_string,font_bank,CRGBColour(255,255,255),0.75f,ENGINE_2D_LAYER_0,true);
  }
 }else if(Text!=L""){
  PrimitiveHandles.resize(1);
  tick_primitive_index=0;
  C2DVector vtext_pos;
  GFACoordToVCoord(pos,vtext_pos);
  const CFontBank& font_bank=CMainGameComponent::Get()->PeekMainFont();
  CPlayerGui::Get().Draw2DText(vtext_pos,Text,font_bank,CRGBColour(255,255,255),0.75f,ENGINE_2D_LAYER_0,true);
  max_height=font_bank.GetFontMaxHeight();
  max_width=font_bank.GetStringWidth(Text);
 }else return 0.0f;
 const CPlayerGuiDef& player_gui_def=CPlayerGui::PeekGuiDef();
 long tick_graphic=player_gui_def.QuestInfoTickGraphic;
 if(tick_graphic>0){
  C2DVector tick_pos(pos.X,pos.Y);
  tick_pos.X=tick_pos.X+max_width*Scale+3.0f;
  const CGraphicDataBank& graphic_bank=CPlayerGui::Get().GetQuestInfo().Bank;
  float graphic_width=(float)graphic_bank.GetGraphicFrameWidth(tick_graphic,0);
  float graphic_height=(float)graphic_bank.GetGraphicFrameHeight(tick_graphic,0);
  if(graphic_height>max_height)max_height=graphic_height;
  if(Ticked){
   C2DBoxF tick_box(tick_pos,C2DExtentsF(graphic_width*Scale,graphic_height*Scale));
   CEnginePrimitive2DSprite tick_sprite(0,GFToScreen(tick_box),ENGINE_2D_LAYER_6,tick_graphic,0,
    ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_0,0.0f,0.0f,CPlayerGui::Get().GetAlpha(),255,255,255,2,
    C2DVector(0.0f,0.0f),C2DVector(0.0f,0.0f));
   info.Engine.SetPersistentPrimitive(PrimitiveHandles[tick_primitive_index],tick_sprite,0xc0);
  }
 }
 return max_height*Scale+2.0f;
}
}

#include "engine/CGame.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CWorld.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CDateAndTime.h"
// The fixture keeps both incomplete-element containers empty.
namespace std {
template<> _Vector_base<CGraphicInfo,allocator<CGraphicInfo> >::~_Vector_base(){assert(!_M_start);}
template<> vector<CGraphicInfo>::~vector(){assert(!_M_start&&!_M_finish);}
template<> _Vector_base<CGraphicInfoFrame,allocator<CGraphicInfoFrame> >::~_Vector_base(){assert(!_M_start);}
template<> vector<CGraphicInfoFrame>::~vector(){assert(!_M_start&&!_M_finish);}
}
#undef private
#undef protected

// Unrelated virtual callees fail if reached.
bool CGameComponent::Run(CGameComponent**) { abort(); }
void CGameComponent::ChangeTextureColourDepth(long) { abort(); }
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
unsigned long CBankFile::GetDataFormatVersion() { abort(); }
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
bool CFontBank::Preload(const CWideString& text, bool) const { abort(); }
static char events[128];static unsigned eventCount;
static void event(char c){assert(eventCount<127);events[eventCount++]=c;events[eventCount]=0;}
static unsigned stringsLive,submissions,textDraws;
static bool caption;
static NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick* activeElement;
static const wchar_t* expectedText;
static C2DVector offsetResult(8.0f,12.0f),textDrawPos;
static const CFontBank* activeFont;
static const CGraphicDataBank* activeBank;
static CMainGameComponent* activeComponent;
static CPlayerManager players;
static CWorld world;
static CPlayerGuiDef guiDef;
static C2DBoxF boxes[2];static unsigned long spriteIDs[2];static unsigned char spriteAlpha[2];
static const wchar_t* chars(const CWideString& x){return x.PStringData?reinterpret_cast<const wchar_t*>(x.PStringData):L"";}
static CWideStringData* copyText(const wchar_t* x){if(!*x)return 0;size_t n=(wcslen(x)+1)*sizeof(wchar_t);wchar_t*p=(wchar_t*)malloc(n);assert(p);memcpy(p,x,n);++stringsLive;return reinterpret_cast<CWideStringData*>(p);}
CWideString::CWideString():PStringData(0){}
CWideString::CWideString(const wchar_t*x):PStringData(copyText(x)){}
CWideString::CWideString(const CWideString&x):PStringData(copyText(chars(x))){}
CWideString::~CWideString(){if(PStringData){free(PStringData);assert(stringsLive);--stringsLive;}}
bool CWideString::operator!=(const wchar_t*x)const{return wcscmp(chars(*this),x)!=0;}
const CWideString& CWideString::operator=(const wchar_t*x){if(PStringData){free(PStringData);--stringsLive;}PStringData=copyText(x);return*this;}
CCharString::CCharString():PStringData(0){}
CCharString::~CCharString(){assert(!PStringData);}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){} CInitBaseClass::~CInitBaseClass(){}
CGame::CGame():CurrentGameComponent(0),Quit(false){memset(ParameterBuffer,0,sizeof ParameterBuffer);}CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){}CGameComponent::~CGameComponent(){}
void CGameComponent::Init(){}void CGameComponent::SetQuit(){Quit=true;}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){}
CMainGameComponent::~CMainGameComponent(){}
CMainGameComponent* CMainGameComponent::PMainGameComponent=0;
CMainGameComponentInit::CMainGameComponentInit(){}CMainGameComponentInit::~CMainGameComponentInit(){}
template<> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::~CTBaseSingletonNoDefaultAlloc(){}
template<> CCountedPointer<CPlayerGui> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass=CCountedPointer<CPlayerGui>();
CInterpolationInfo::CInterpolationInfo(){Current.GTPredictedTimeSinceLastRenderFrame=Current.WFInterpolate=Last.GTPredictedTimeSinceLastRenderFrame=Last.WFInterpolate=PausedCurrent.GTPredictedTimeSinceLastRenderFrame=PausedCurrent.WFInterpolate=PausedLast.GTPredictedTimeSinceLastRenderFrame=PausedLast.WFInterpolate=BulletTimeCurrent.GTPredictedTimeSinceLastRenderFrame=BulletTimeCurrent.WFInterpolate=BulletTimeLast.GTPredictedTimeSinceLastRenderFrame=BulletTimeLast.WFInterpolate=0;WFServerCurrentTime=0;}
CEnginePrimitivePrepareInfo::CEnginePrimitivePrepareInfo(const CInterpolationInfo&i,CIEngine&e,bool edit,const CCamera&c):Interpolate(i),Engine(e),Edit2DEngine(edit),Camera(c){}
CIEngine::~CIEngine(){}
CBankFile::~CBankFile(){} CBankFileAsync::~CBankFileAsync(){}
CResourceBank::CResourceBank(){}CResourceBank::~CResourceBank(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CFailedAllocationHandler::~CFailedAllocationHandler(){}
CResource::~CResource(){} CResourceList::~CResourceList(){}
CGraphicDataBank::~CGraphicDataBank(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CFontBank::~CFontBank(){}
CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id,const C2DBoxF&b,EEngine2DLayer layer,unsigned long bank,unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interp,float sa,float ea,unsigned char a,unsigned char r,unsigned char g,unsigned char blue,unsigned long flags,const C2DVector&u,const C2DVector&v)
 :CEnginePrimitive((EEnginePrimitiveType)34,interp),ClipBox(b),Layer(layer),BankIndex(bank),Frame(frame),StartAngle(sa),EndAngle(ea),BlendMode(blend),Colour(r,g,blue,a),ClipBoxIsPos(false),Flags(flags),Texture(0),UVOffsetsStart(u),UVOffsetsEnd(v){assert(id==0);}
namespace NPlayerGui {
CDrawBase::CDrawBase(CPlayerGui&g,std::vector<CDrawBase*>&):PlayerGui(g),PlayerManager(players),World(world),Component(*activeComponent),Bank(*activeBank),Drawing(false){}
CDrawBase::~CDrawBase(){}
void CDrawBase::ClearPrimitives(){abort();}void CDrawBase::ClearPrimitivesIfNecessary(){abort();}
CDrawQuestInfo::CDrawQuestInfo(CPlayerGui&g,std::vector<CDrawBase*>&v):CDrawBase(g,v){}
CDrawQuestInfo::CQuestInfoElement::CQuestInfoElement(float scale):ID(0),ScriptID(0),Scale(scale){}
CDrawQuestInfo::CQuestInfoElementTick::CQuestInfoElementTick(long graphic,bool ticked,float scale):CQuestInfoElement(scale),Graphic(graphic),Ticked(ticked){}
CDrawQuestInfo::CQuestInfoElementTick::CQuestInfoElementTick(const CWideString&text,bool ticked,float scale):CQuestInfoElement(scale),Graphic(0),Text(text),Ticked(ticked){}
bool CDrawQuestInfo::CQuestInfoElement::Update(){abort();}
void CDrawQuestInfo::CQuestInfoElement::Clear(){event('C');assert(PrimitiveHandles.empty());}
CDrawQuestInfo::EElementType CDrawQuestInfo::CQuestInfoElementTick::GetType()const{return QUEST_INFO_ELEMENT_COUNTER_TICK;}
}
namespace std {
template<> void vector<CEnginePrimitiveHandle>::_M_fill_insert(iterator pos,size_type n,const CEnginePrimitiveHandle&x){assert(pos==end()&&empty()&&n<=2);event('R');_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;for(size_type i=0;i<n;++i){_Construct(_M_finish,x);++_M_finish;}}
}
unsigned long CGraphicDataBank::GetGraphicFrameWidth(unsigned long g,unsigned long f)const{assert(this==activeBank&&f==0);event('W');return g==7?16:8;}
unsigned long CGraphicDataBank::GetGraphicFrameHeight(unsigned long g,unsigned long f)const{assert(this==activeBank&&f==0);event('H');return g==7?12:20;}
CWideString CSubtitleRenderer::GetTextFromEmbeddedTagGraphic(long g)const{assert(g==7);event('S');return CWideString(caption?L"caption":L"");}
const C2DVector& CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic(long g)const{assert(g==7);event('O');return offsetResult;}
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&p,C2DVector&v){event('V');v=C2DVector(p.X*2.0f,p.Y*4.0f);}
void CPlayerGui::Draw2DText(const C2DVector&p,const CWideString&s,const CFontBank&f,const CRGBColour&c,float scale,EEngine2DLayer layer,bool snap){assert(&f==activeFont&&wcscmp(chars(s),expectedText)==0&&scale==0.75f&&layer==ENGINE_2D_LAYER_0&&snap);assert(c.R==255&&c.G==255&&c.B==255);event('D');++textDraws;textDrawPos=p;}
float CFontBank::GetStringWidth(const CWideString&s)const{assert(this==activeFont&&s!=L"");event('L');return 24.0f;}
const CPlayerGuiDef& FABLE_FASTCALL CPlayerGui::PeekGuiDef(){event('G');return guiDef;}
class TestFont:public CFontBank{
public:
 C2DExtentsF GetStringExtents(const wchar_t*,bool)const{abort();return C2DExtentsF();}
 float GetFontMaxHeight()const{event('F');return 10.0f;}
 void PrepareToRender(bool)const{abort();}
 void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{abort();}
};
class TestEngine:public CIEngine{
public:
 virtual void SetCamera(const CCamera*) { abort(); }
 virtual void SetWindow(const C2DBoxF*) { abort(); }
 virtual void Render(const CEngineInterpolationInfo&, unsigned long) { abort(); }
 virtual void Virtual11_00b23cf0() { abort(); }
 virtual bool ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const { abort(); }
 virtual bool ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const { abort(); }
 virtual bool PreChangeResolution() { abort(); }
 virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long) { abort(); }
 void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned long flags){
  assert(submissions<2&&flags==0xc0);assert(h.Data.IsNull());const CEnginePrimitive2DSprite&s=static_cast<const CEnginePrimitive2DSprite&>(p);
  assert(activeElement);unsigned index=s.BankIndex==7?0:(activeElement->Graphic>0?1:0);assert(&h==&activeElement->PrimitiveHandles[index]);
  assert(s.Layer==ENGINE_2D_LAYER_6&&s.Frame==0&&s.BlendMode==ENGINE_PRIMITIVE_BLEND_ALPHA&&s.InterpolationSet==ENGINE_INTERPOLATION_SET_0&&s.StartAngle==0&&s.EndAngle==0&&s.Flags==2&&!s.ClipBoxIsPos&&!s.Texture);
  assert(s.UVOffsetsStart.X==0&&s.UVOffsetsStart.Y==0&&s.UVOffsetsEnd.X==0&&s.UVOffsetsEnd.Y==0);assert(s.Colour.R==255&&s.Colour.G==255&&s.Colour.B==255);
  event('P');boxes[submissions]=s.ClipBox;spriteIDs[submissions]=s.BankIndex;spriteAlpha[submissions]=s.Colour.A;++submissions;
 }
};
static void boxEq(const C2DBoxF&b,float x,float y,float w,float h){assert(b.TLX==x&&b.TLY==y&&b.BRX==x+w&&b.BRY==y+h);}
int main(){
 unsigned cases=0;
 {CGame game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);CMainGameComponent::PMainGameComponent=&component;activeComponent=&component;
 TestFont font;activeFont=&font;component.PCutSceneMainFont.Data=&font;component.PCutSceneMainFont.Info=0;
 CGraphicDataBank bank;activeBank=&bank;
 CPlayerGui gui;CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Data=&gui;CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Info=0;
 std::vector<NPlayerGui::CDrawBase*> illustrators;NPlayerGui::CDrawQuestInfo quest(gui,illustrators);gui.PDrawQuestInfo.Data=&quest;gui.PDrawQuestInfo.Info=0;
 TestEngine engine;CInterpolationInfo ii;CCamera camera;CEnginePrimitivePrepareInfo prepare(ii,engine,false,camera);
 for(int mode=0;mode<4;++mode)for(int tick=0;tick<2;++tick)for(int available=0;available<2;++available){
  NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick element(mode<2?7:0,tick!=0,0.5f);element.Text=mode==2?L"text":mode==1?L"ignored":L"";gui.Alpha=0.5f;caption=mode==1;guiDef.QuestInfoTickGraphic=available?9:0;
  eventCount=0;events[0]=0;submissions=textDraws=0;textDrawPos=C2DVector(-1,-1);
  activeElement=&element;expectedText=mode==1?L"caption":L"text";
  C2DVector pos(32.0f,48.0f);float out=element.DrawAt(pos,prepare);
  if(mode==3){assert(out==0&&submissions==0&&textDraws==0&&element.PrimitiveHandles.empty());assert(strcmp(events,"C")==0);}
  else{
   float height=available?20.0f:(mode==2?10.0f:12.0f);assert(out==height*0.5f+2.0f);assert(element.PrimitiveHandles.size()==(mode==2?1:2));
   unsigned graphicCount=mode<2?1:0;assert(submissions==graphicCount+(unsigned)(available&&tick));assert(textDraws==(unsigned)(mode==1||mode==2));
   if(mode<2){boxEq(boxes[0],32,48,8,6);assert(spriteIDs[0]==7&&spriteAlpha[0]==127);}
   if(available&&tick){unsigned index=mode<2?1:0;boxEq(boxes[index],mode<2?43.0f:47.0f,48,4,10);assert(spriteIDs[index]==9&&spriteAlpha[index]==127);}
   if(mode==1)assert(textDrawPos.X==72&&textDrawPos.Y==224);
   if(mode==2)assert(textDrawPos.X==64&&textDrawPos.Y==192);
   char expected[128];strcpy(expected,mode==0?"CRWHPS":mode==1?"CRWHPSOVD":"CRVDFL");strcat(expected,"G");if(available)strcat(expected,tick?"WHP":"WH");assert(strcmp(events,expected)==0);
  }
  ++cases;
 }
 gui.PDrawQuestInfo.Data=0;component.PCutSceneMainFont.Data=0;CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Data=0;CMainGameComponent::PMainGameComponent=0;
 }
 assert(stringsLive==0);assert(cases==16);puts("00650e98_TEST PASS");return 0;
}