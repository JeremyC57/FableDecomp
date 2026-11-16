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
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define private public
#define protected public
// 0x00646F67 NPlayerGui::CDrawExperience::Draw -- draw gained experience with an icon, text and sized bar.
// Inline-budget: /d2inlS20 expands the original colour copy at006470aa; S15 calls it.
// Leaf: ?Draw@CDrawExperience@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 0x00646F67,843bytes,thiscall.
// Flags: /d2inlS20
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CSTATBAR_CHANGE_TEXT_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CMULTISTATBAR_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CDRAWEXPERIENCE_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GUIMODE
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_EXPERIENCE_PASS23
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CFontBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CMultiStatBar.h"
#include "engine/NPlayerGui_CDrawExperience.h"
#include "engine/NPlayerGui_Graphics.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);               // 0x00405F00
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector& in, C2DVector& out);  // 0x009E21D0

void NPlayerGui::CDrawExperience::Draw(const CEnginePrimitivePrepareInfo& info)
{
    EStatUpdateType type = PlayerGui.PStatUpdates->GetCurrentType();
    if (type < 0 || type > 3)
        return;
    SetAsDrawing(true);
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    unsigned char alpha = PlayerGui.PStatUpdates->GetFadeAlpha();
    C2DVector exp_pos = gui_def.ExperiencePos;
    long exp_graphic = gui_def.ExperienceGraphic;
    switch (type) {
    case 0: exp_graphic = gui_def.ExperienceStrengthGraphic; break;
    case 1: exp_graphic = gui_def.ExperienceSkillGraphic; break;
    case 2: exp_graphic = gui_def.ExperienceWillGraphic; break;
    case 3: exp_graphic = gui_def.ExperienceGeneralGraphic; break;
    }
    CEnginePrimitive2DSprite exp_sprite(0, GFToScreen(exp_pos), ENGINE_2D_LAYER_1, exp_graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0,
                                  0.0f, 0.0f, ScaleAlpha(alpha), 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], exp_sprite, 0xc0);
    const CFontBank& font = Component.PeekMainFont();
    CWideString exp_text = GFIntToWideString(PlayerGui.PStatUpdates->GetCurrentChange());
    CRGBColour colour_with_alpha = gui_def.ExperienceTextColour;
    colour_with_alpha.A = (unsigned char)(colour_with_alpha.A * PlayerGui.Alpha);
    C2DVector vtext_pos;
    GFACoordToVCoord(exp_pos + gui_def.ExperienceTextOffset, vtext_pos);
    CPlayerGui::Get().Draw2DText(vtext_pos, exp_text, font, colour_with_alpha, 0.85f, ENGINE_2D_LAYER_2, false);
    float box_height = (float)Bank.GetGraphicFrameHeight(gui_def.ExperienceMiddleGraphic, 0);
    float box_width = font.GetStringWidth(exp_text) + gui_def.ExperienceMiddleMinWidth;
    C2DVector bar_pos = exp_pos + gui_def.ExperienceMiddleOffset;
    C2DBoxF bar_box(bar_pos, C2DExtentsF(box_width, box_height));
    CEnginePrimitive2DSprite bar_sprite(0, GFToScreen(bar_box), ENGINE_2D_LAYER_4, gui_def.ExperienceMiddleGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                                    ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], bar_sprite, 0xc0);
    // Fidelity: original donor020e386b updates this position; PAREN settles native0064721b scheduling.
    bar_pos.X += (bar_box.GetWidth());
    CEnginePrimitive2DSprite bar_end_sprite(0, GFToScreen(bar_pos), ENGINE_2D_LAYER_4, gui_def.ExperienceEndGraphic, 0,
                                 ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 255, 255, 255, 2,
                                 C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[2], bar_end_sprite, 0xc0);
}

#include "engine/CDefString.h"
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
static char events[128];static unsigned eventCount;
static void event(char c){assert(eventCount<127);events[eventCount++]=c;events[eventCount]=0;}
static unsigned stringsLive,submissions,textDraws,scaleCalls;
static NPlayerGui::CDrawExperience* activeDrawer;
static const CFontBank* activeFont;
static const CGraphicDataBank* activeBank;
static CMainGameComponent* activeComponent;
static CPlayerGui* activeGui;
static CPlayerManager players;static CWorld world;
static EStatUpdateType currentType;
static long currentGain;
static unsigned char expectedAlpha;
static unsigned long expectedGraphic;
static float positions[3][4];static unsigned long graphicIDs[3];static unsigned char alphas[3];
static C2DVector drawnTextPosition;static CRGBColour drawnTextColour;
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
 :CEnginePrimitive((EEnginePrimitiveType)34,id,interp),ClipBox(b),Layer(layer),BankIndex(bank),Frame(frame),StartAngle(sa),EndAngle(ea),BlendMode(blend),Colour(r,g,blue,a),ClipBoxIsPos(false),Flags(flags),Texture(0),UVOffsetsStart(u),UVOffsetsEnd(v){assert(id==0);}

CParentDefClassBase::CParentDefClassBase(){}
CParentDefClassBase::~CParentDefClassBase(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
static CPlayerGuiDef* activeDef;
CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id,const C2DVector&p,EEngine2DLayer layer,unsigned long bank,unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interp,float sa,float ea,unsigned char a,unsigned char r,unsigned char g,unsigned char blue,unsigned long flags,const C2DVector&u,const C2DVector&v)
 :CEnginePrimitive((EEnginePrimitiveType)34,id,interp),ClipBox(p,p),Layer(layer),BankIndex(bank),Frame(frame),StartAngle(sa),EndAngle(ea),BlendMode(blend),Colour(r,g,blue,a),ClipBoxIsPos(true),Flags(flags),Texture(0),UVOffsetsStart(u),UVOffsetsEnd(v){assert(id==0);}
namespace NPlayerGui {
CStatBase::CStatBase(const CPlayerGuiDef&d):GuiDef(d){}
CStatBase::~CStatBase(){}
CStatBar::CStatBar(const CPlayerGuiDef&d):CStatBase(d),FontBank(*activeFont){}
CStatBar::~CStatBar(){}
CMultiStatBar::CMultiStatBar(const CPlayerGuiDef&d):CStatBar(d){}
CMultiStatBar::~CMultiStatBar(){}
void CStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,const C2DVector&,const CRGBColour&,unsigned char,bool){abort();}
void CStatBar::ClearPrimitiveHandles(){abort();}
void CStatBar::DrawStatBox(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,long,long,long,const CWideString&,unsigned char,bool,bool){abort();}
void CMultiStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo&,const CGraphicDataBank&,const C2DVector&,const C2DVector&,const CRGBColour&,unsigned char,bool){abort();}
EStatUpdateType CMultiStatBar::GetCurrentType()const{event('T');return currentType;}
long CMultiStatBar::GetCurrentChange()const{event('C');return currentGain;}
CDrawBase::CDrawBase(CPlayerGui&g,std::vector<CDrawBase*>&):PlayerGui(g),PlayerManager(players),World(world),Component(*activeComponent),Bank(*activeBank),Drawing(false){}
CDrawBase::~CDrawBase(){}
void CDrawBase::ClearPrimitives(){abort();}void CDrawBase::ClearPrimitivesIfNecessary(){abort();}
unsigned char CDrawBase::ScaleAlpha(unsigned char a)const{assert(a==expectedAlpha);event('A');++scaleCalls;return(unsigned char)(a*activeGui->Alpha);}
CDrawExperience::CDrawExperience(CPlayerGui&g,std::vector<CDrawBase*>&v):CDrawBase(g,v){}
CDrawExperience::~CDrawExperience(){}
}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
namespace std {
template<> void vector<CEnginePrimitiveHandle>::_M_fill_insert(iterator pos,size_type n,const CEnginePrimitiveHandle&x){assert(pos==end()&&empty()&&n<=3);_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;for(size_type i=0;i<n;++i){_Construct(_M_finish,x);++_M_finish;}}
}
CWideString FABLE_FASTCALL GFIntToWideString(long gain){assert(gain==currentGain);event('I');return CWideString(gain<0?L"-8":L"16");}
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector&p,C2DVector&v){event('V');v=C2DVector(p.X*2.0f,p.Y*4.0f);}
void CPlayerGui::Draw2DText(const C2DVector&p,const CWideString&s,const CFontBank&f,const CRGBColour&c,float scale,EEngine2DLayer layer,bool snap){assert(&f==activeFont&&wcscmp(chars(s),currentGain<0?L"-8":L"16")==0&&scale==0.85f&&layer==ENGINE_2D_LAYER_2&&!snap);event('D');++textDraws;drawnTextPosition=p;drawnTextColour=c;}
float CFontBank::GetStringWidth(const CWideString&s)const{assert(this==activeFont&&s!=L"");event('L');return 24.0f;}
const CPlayerGuiDef& FABLE_FASTCALL CPlayerGui::PeekGuiDef(){event('G');return*activeDef;}
unsigned long CGraphicDataBank::GetGraphicFrameWidth(unsigned long,unsigned long)const{abort();return 0;}
unsigned long CGraphicDataBank::GetGraphicFrameHeight(unsigned long g,unsigned long f)const{assert(this==activeBank&&g==107&&f==0);event('H');return 12;}
class TestFont:public CFontBank{
public:
 C2DExtentsF GetStringExtents(const wchar_t*,bool)const{abort();return C2DExtentsF();}
 float GetFontMaxHeight()const{abort();return 0;}
 void PrepareToRender(bool)const{abort();}
 void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{abort();}
};
class TestEngine:public CIEngine{
public:
 virtual void SetCamera(const CCamera*) { abort(); }
 virtual void SetWindow(const C2DBoxF*) { abort(); }
 virtual void Virtual08_00b27d90() { abort(); }
 virtual void Virtual11_00b23cf0() { abort(); }
 virtual void Virtual12_00b24400() { abort(); }
 virtual void Virtual13_00b242d0() { abort(); }
 virtual bool PreChangeResolution() { abort(); }
 virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long) { abort(); }
 void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned long flags){
  assert(submissions<3&&flags==0xc0&&&h==&activeDrawer->PrimitiveHandles[submissions]);const CEnginePrimitive2DSprite&s=static_cast<const CEnginePrimitive2DSprite&>(p);
  assert(s.Frame==0&&s.BlendMode==ENGINE_PRIMITIVE_BLEND_ALPHA&&s.InterpolationSet==ENGINE_INTERPOLATION_SET_0&&s.StartAngle==0&&s.EndAngle==0&&!s.Texture);
  assert(s.UVOffsetsStart.X==0&&s.UVOffsetsStart.Y==0&&s.UVOffsetsEnd.X==0&&s.UVOffsetsEnd.Y==0&&s.Colour.R==255&&s.Colour.G==255&&s.Colour.B==255);
  assert(s.Layer==(submissions==0?ENGINE_2D_LAYER_1:ENGINE_2D_LAYER_4));assert(s.Flags==(submissions==0?0:2));assert(s.ClipBoxIsPos==(submissions!=1));
  event('P');positions[submissions][0]=s.ClipBox.TLX;positions[submissions][1]=s.ClipBox.TLY;positions[submissions][2]=s.ClipBox.BRX;positions[submissions][3]=s.ClipBox.BRY;graphicIDs[submissions]=s.BankIndex;alphas[submissions]=s.Colour.A;++submissions;
 }
};
int main(){unsigned cases=0;
 {CGame game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);activeComponent=&component;CMainGameComponent::PMainGameComponent=&component;
 TestFont font;activeFont=&font;CGraphicDataBank bank;activeBank=&bank;CPlayerGuiDef def;activeDef=&def;
 def.ExperiencePos=C2DVector(32,48);def.ExperienceGraphic=100;def.ExperienceStrengthGraphic=101;def.ExperienceSkillGraphic=102;def.ExperienceWillGraphic=103;def.ExperienceGeneralGraphic=104;def.ExperienceTextOffset=C2DVector(4,8);def.ExperienceTextColour=CRGBColour(40,80,120,200);def.ExperienceMiddleOffset=C2DVector(8,16);def.ExperienceMiddleMinWidth=8;def.ExperienceMiddleGraphic=107;def.ExperienceEndGraphic=108;
 CPlayerGui gui;activeGui=&gui;CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Data=&gui;CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Info=0;component.PCutSceneMainFont.Data=&font;component.PCutSceneMainFont.Info=0;
 NPlayerGui::CMultiStatBar bar(def);gui.PStatUpdates.Data=&bar;gui.PStatUpdates.Info=0;
 std::vector<NPlayerGui::CDrawBase*> illustrators;NPlayerGui::CDrawExperience drawer(gui,illustrators);drawer.PrimitiveHandles.resize(3);activeDrawer=&drawer;
 TestEngine engine;CInterpolationInfo ii;CCamera camera;CEnginePrimitivePrepareInfo info(ii,engine,false,camera);
 for(int type=0;type<7;++type)for(int fade=0;fade<2;++fade)for(int whole=0;whole<2;++whole){
  currentType=(EStatUpdateType)type;currentGain=fade?-8:16;bar.FadeInTimer=fade?0.5f:1.0f;gui.Alpha=whole?0.5f:1.0f;expectedAlpha=fade?127:255;expectedGraphic=101+type;
  drawer.Drawing=false;eventCount=0;events[0]=0;submissions=textDraws=scaleCalls=0;drawer.Draw(info);
  if(type>3){assert(!drawer.Drawing&&submissions==0&&textDraws==0&&scaleCalls==0&&strcmp(events,"T")==0);}
  else{assert(drawer.Drawing&&submissions==3&&textDraws==1&&scaleCalls==3);assert(strcmp(events,"TGAPCIVDHLAPAP")==0);assert(graphicIDs[0]==expectedGraphic&&graphicIDs[1]==107&&graphicIDs[2]==108);
   unsigned char a=whole?(fade?63:127):expectedAlpha;assert(alphas[0]==a&&alphas[1]==a&&alphas[2]==a);
   assert(positions[0][0]==32&&positions[0][1]==48&&positions[0][2]==32&&positions[0][3]==48);
   assert(positions[1][0]==40&&positions[1][1]==64&&positions[1][2]==72&&positions[1][3]==76);
   assert(positions[2][0]==72&&positions[2][1]==64&&positions[2][2]==72&&positions[2][3]==64);
   assert(drawnTextPosition.X==72&&drawnTextPosition.Y==224&&drawnTextColour.R==40&&drawnTextColour.G==80&&drawnTextColour.B==120&&drawnTextColour.A==(whole?100:200));
  }++cases;
 }
 gui.PStatUpdates.Data=0;component.PCutSceneMainFont.Data=0;CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Data=0;CMainGameComponent::PMainGameComponent=0;
 }
 assert(stringsLive==0&&cases==28);puts("00646f67_TEST PASS");return 0;
}