// NF242 (q12678 step 1): the CIEngine slot 8 / 11 / 12 / 13 overrides respelled from the retired VirtualNN_ placeholders to the PDB
// declarations engine/CIEngine.h now makes the default (Render, ConvertScreenPosToWorldPos, ...AtViewDistance, ConvertWorldPosToScreenPos).
// Self-contained real-header fixture. All external methods are controlled test doubles.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <float.h>
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
enum EEnginePrimitiveType {FixturePrimitive2DText=0x27};
enum EEngineInterpolationSet {FixtureInterpolationDefault=0};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_DUAL_PATH_TEST
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CWIDESTRING
#define private public
#define protected public
// 0054EF00 NUISystem::CText::Draw -- render text and its bottom-right shadow, or clear off-screen handles.
// Inline-budget: original Clear sites score87/79(default74), measuredT88 admits all4.
// Retail:0x0054EF00,1127bytes,thiscall ret20.
// Leaf: ?Draw@CText@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
// Flags: /d2inlT88
// Retail-asm: 0x0054F0B0 `fistp dword ptr [esp+28h]` (and 0x0054F0C0, the y rounding) -- the engine's inline-asm float-to-long
//   helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h); VC7.1 emits no bare dword fistp from C++ here ((long)x -> __ftol2,
//   /QIfist -> fistp qword. ROUNDING_LOWERING_CATALOG.json records 30 C++ and 10 intrinsic forms at both sites.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CTEXT_LAYOUT
#define FABLE_RETAIL_CTEXT_DRAW_PRIMITIVE_BODIES
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CText.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitive2DText.h"
#include "engine/GFFloatToLongNear.h"
// Original complete descriptor bodies; native114-byte ctor and91-byte assignment are audited separately.
CEnginePrimitive2DText::CEnginePrimitive2DText(unsigned __int64 id, const C2DVector& position,
    EEngine2DLayer layer, float scale, const CWideString& text, const CFontBank& font,
    const CRGBColour& colour, EEngineInterpolationSet interpolation_set)
    : CEnginePrimitive((EEnginePrimitiveType)0x27, id, interpolation_set),
      Pos(position.X, position.Y, 0.0f), Layer(layer), Scale(scale), Colour(colour), Text(text), PFont(&font)
{
}
CEnginePrimitive2DText& CEnginePrimitive2DText::operator=(const CEnginePrimitive2DText& other)
{
    CEnginePrimitive::operator=(other);
    Pos = other.Pos;
    Layer = other.Layer;
    Scale = other.Scale;
    Colour = other.Colour;
    Text = other.Text;
    PFont = other.PFont;
    return *this;
}

namespace NUISystem {
void CText::Draw(CIEngine& engine, CEnginePrimitiveHandle* p_handle, long layer, long* pprimitive_index, CComponent* pparent)
{
    if (RenderColour.A <= 0 || RenderZoom.X <= 0.0f) {
        if (NoRenderNextFrame == 1) {
            if (p_handle) {
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
                engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
            } else {
                Handle.Clear();
                HandleDropShadowBottomRight.Clear();
            }
            return;
        }
        NoRenderNextFrame = 1;
    } else
        NoRenderNextFrame = 0;
    CRGBColour colour = RenderColour;
    C2DVector render_position = RelativeRenderPosition;
    float render_zoom_x = ScaleText ? RelativeRenderZoom.X : RenderZoom.X;
    if (render_position.X >= CUIScreenUnitConverter::GetBackBufferWidth() || render_position.Y >= CUIScreenUnitConverter::GetBackBufferHeight()) {
        if (p_handle) {
            engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
            engine.ClearChildPrimitive(*p_handle, (*pprimitive_index)++);
        } else {
            Handle.Clear();
            HandleDropShadowBottomRight.Clear();
        }
        return;
    }
    bool is_independent = IsIndependent();
    bool is_layerindependent = IsLayerIndependent();
    long render_layer;
    if (!is_independent && !is_layerindependent)
        render_layer = Layer + layer;
    else
        render_layer = Layer;
    CRGBColour black(0, 0, 0, colour.A);
    CEnginePrimitive2DText primitive;
    CEnginePrimitive2DText primitive_bottom_right;
    switch (GetAlignment()) {
    case CENTER:
        render_position.X -= render_zoom_x * Size.X * 0.5f;
        break;
    case RIGHT:
        render_position.X -= render_zoom_x * Size.X;
        break;
    }
    render_position.X = (float)GFFloatToLongNear(render_position.X);
    render_position.Y = (float)GFFloatToLongNear(render_position.Y);
    if (Unavailable) {
        primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom_x, Value, *PFont, black, (EEngineInterpolationSet)0);
        C2DVector position(render_position.X + 2.0f, render_position.Y + 2.0f);
        primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom_x, Value, *PFont, colour, (EEngineInterpolationSet)0);
    } else {
        primitive = CEnginePrimitive2DText(0, render_position, CManager::GetInstance()->GetLayer(render_layer), render_zoom_x, Value, *PFont, colour, (EEngineInterpolationSet)0);
        C2DVector position(render_position.X + 2.0f, render_position.Y + 2.0f);
        primitive_bottom_right = CEnginePrimitive2DText(0, position, CManager::GetInstance()->GetLayer(render_layer + 1), render_zoom_x, Value, *PFont, black, (EEngineInterpolationSet)0);
    }
    if (p_handle && pprimitive_index) {
        engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive, 0x40);
        engine.SetChildPrimitive(*p_handle, (*pprimitive_index)++, primitive_bottom_right, 0x40);
    } else {
        engine.SetPersistentPrimitive(Handle, primitive, 0x40);
        engine.SetPersistentPrimitive(HandleDropShadowBottomRight, primitive_bottom_right, 0x40);
    }
    return;
}
}

#include "engine/CFontBank.h"
#include "engine/CCamera.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CRGBFloatColour.h"
#include "engine/NUISystem_CDeletion.h"


static int FixtureStringLive=0,FontLive=0,PrimitiveLive=0,Notifications=0,Deletions=0;
static const CFontBank* ExpectedFont;
static const unsigned short FixtureValueText[2]={65,0};
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CInitBaseClass::CInitBaseClass(){} CInitBaseClass::~CInitBaseClass(){}
CIEngine::~CIEngine(){}
void CIVCountedPointeeBase::Delete()const{delete this;}
CEnginePersistentPrimitiveBase::CEnginePersistentPrimitiveBase(long type):Type(type){}
CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase(){}
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&){}
bool CEnginePersistentPrimitiveBase::WasRendered()const{return true;}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char* p,long):PStringData(reinterpret_cast<CCharStringData*>(const_cast<char*>(p))){}
CCharString::~CCharString(){}
CWideString::CWideString():PStringData(0){++FixtureStringLive;}
CWideString::CWideString(const CWideString& x):PStringData(x.PStringData){++FixtureStringLive;}
CWideString::~CWideString(){--FixtureStringLive;}
const CWideString& CWideString::operator=(const CWideString& x){PStringData=x.PStringData;return *this;}
namespace NUISystem {
CUIInterface::~CUIInterface(){} CObserver::CObserver(){} CObserver::~CObserver(){}
CComponent::CComponent(long){} CComponent::~CComponent(){}
CChangingStateComponent::CChangingStateComponent(long x):CComponent(x){} CChangingStateComponent::~CChangingStateComponent(){}
CText::CText(long x):CChangingStateComponent(x){} CText::~CText(){}

CManager::CManager(){} CManager::~CManager(){}
}
class FixturePersistent:public CEnginePersistentPrimitiveBase {public:FixturePersistent():CEnginePersistentPrimitiveBase(39){++PrimitiveLive;}~FixturePersistent(){--PrimitiveLive;}virtual void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const{}virtual void NotifyReleased(const CEnginePrimitiveHandle&){++Notifications;}virtual void Delete()const{++Deletions;delete this;}};
static unsigned Bits(float v){unsigned x;assert(sizeof x==sizeof v);memcpy(&x,&v,4);return x;}
struct Call {char Kind;unsigned Index,Flags;float X,Y,Z,Scale;unsigned Colour;long Layer;unsigned Text;int Font;};
static Call Calls[2];static int CallCount=0,LayerCalls=0;static long Layers[2];static CEnginePrimitiveHandle*ExpectedParent;static CEnginePrimitiveHandle*ExpectedOwn[2];
static unsigned ColourBits(const CRGBColour&c){return unsigned(c.B)|(unsigned(c.G)<<8)|(unsigned(c.R)<<16)|(unsigned(c.A)<<24);}
class FixtureEngine : public CIEngine { public:
void SetCamera(const CCamera*)  { abort(); }
void SetWindow(const C2DBoxF*)  { abort(); }
void Render(const CEngineInterpolationInfo&, unsigned long)  { abort(); }
bool ConvertScreenPosToWorldPos(const C2DVector&, C3DVector&) const  { abort(); }
bool ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const  { abort(); }
bool ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const  { abort(); }
bool PreChangeResolution()  { abort(); }
bool PostChangeResolution(unsigned long, unsigned long, unsigned long)  { abort(); }
void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
};
static void Capture(char kind,unsigned index,const CEnginePrimitive&p,unsigned long flags){assert(CallCount<2);const CEnginePrimitive2DText&t=static_cast<const CEnginePrimitive2DText&>(p);assert((int)t.Type==0x27&&(int)t.InterpolationSet==0);assert(t.ThemeChannelDesc.ChannelIndex0==0&&t.ThemeChannelDesc.ChannelIndex1==0&&t.ThemeChannelDesc.ChannelBlendFactor==0);assert(t.PFont==ExpectedFont);Call c;memset(&c,0,sizeof c);c.Kind=kind;c.Index=index;c.Flags=(unsigned)flags;c.X=t.Pos.X;c.Y=t.Pos.Y;c.Z=t.Pos.Z;c.Scale=t.Scale;c.Colour=ColourBits(t.Colour);c.Layer=(long)t.Layer;c.Text=reinterpret_cast<const unsigned short*>(t.Text.PStringData)[0];c.Font=73;Calls[CallCount++]=c;}

void FixtureEngine::SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned long flags){assert(CallCount<2&&&h==ExpectedOwn[CallCount]);Capture('P',0,p,flags);}
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&h,unsigned long index,const CEnginePrimitive&p,unsigned long flags){assert(&h==ExpectedParent);Capture('D',(unsigned)index,p,flags);}
void CIEngine::ClearChildPrimitive(CEnginePrimitiveHandle&h,unsigned long index){assert(&h==ExpectedParent&&CallCount<2);Call c;memset(&c,0,sizeof c);c.Kind='C';c.Index=(unsigned)index;Calls[CallCount++]=c;}
namespace NUISystem {
static CManager Manager;
CManager* FABLE_FASTCALL CManager::GetInstance(){return &Manager;}
EEngine2DLayer CManager::GetLayer(long layer){assert(LayerCalls<2);::Layers[LayerCalls++]=layer;return(EEngine2DLayer)(1000+layer);}
bool CUIScreenUnitConverter::IsUsingRelativeCoords=false;
float CUIScreenUnitConverter::BackBufferWidth=128.0f;
float CUIScreenUnitConverter::BackBufferHeight=96.0f;
float CUIScreenUnitConverter::DefaultBackBufferWidth=640.0f;
float CUIScreenUnitConverter::DefaultBackBufferHeight=480.0f;
class FixtureText : public CText { public:
 bool Independent,LayerIndependent; ETextAlignement Align;
 FixtureText():CText(0){PFont.Reset(new CFontBank);++FontLive;Value.PStringData=reinterpret_cast<CWideStringData*>(const_cast<unsigned short*>(FixtureValueText));}
 ~FixtureText(){--FontLive;}
 bool IsIndependent()const{return Independent;} bool IsLayerIndependent()const{return LayerIndependent;}
 ETextAlignement GetAlignment()const{return Align;}
};
}

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
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) { abort(); }
void CIEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&) { abort(); }
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
namespace NUISystem { void CObservable::SetEvent(NUISystem::EEvent) { abort(); } }
namespace NUISystem { void CObservable::SetOwnedEvent(NUISystem::CObserver *,NUISystem::EEvent) { abort(); } }
namespace NUISystem { void CObservable::AddObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::AddExclusiveObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::RemoveExclusiveObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::RemoveObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::ClearObservers() { abort(); } }
namespace NUISystem { NUISystem::CObserver * CObservable::GetExclusiveObserver() { abort(); } }
namespace NUISystem { bool CObservable::IsExclusivelyObserved() { abort(); } }
namespace NUISystem { void CObservable::AddConcurrentExclusiveObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::RemoveConcurrentExclusiveObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CUIInterface::Update(float) { abort(); } }
namespace NUISystem { void CUIInterface::Draw(CIEngine &, CEnginePrimitiveHandle *, long, long *, NUISystem::CComponent *) { abort(); } }
namespace NUISystem { bool CObserver::IsObservingEvent(EEvent) { abort(); } }
namespace NUISystem { void CObserver::ObserveEvent(EEvent) { abort(); } }
namespace NUISystem { void CObserver::IgnoreEvent(EEvent) { abort(); } }
namespace NUISystem { void CObserver::IgnoreAllEvents() { abort(); } }
namespace NUISystem { void CObserver::ObserveAllEvents() { abort(); } }
namespace NUISystem { void CObserver::SetPreventObserving(bool) { abort(); } }
namespace NUISystem { void CComponent::Update(float) { abort(); } }
namespace NUISystem { void CComponent::Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindChild(CCharString) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindChildForward(CCharString) { abort(); } }
namespace NUISystem { void CComponent::FindAllChildren(CCharString, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { abort(); } }
namespace NUISystem { void CComponent::FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { abort(); } }
namespace NUISystem { void CComponent::SetPAcceptButton(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindChildNonRecursive(CCharString) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindFirstChildOfType(NUISystem::EType) { abort(); } }
namespace NUISystem { bool CComponent::HasChild(NUISystem::CComponent *) const { abort(); } }
namespace NUISystem { void CComponent::SetZoom(const C2DVector &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetZoom() const { abort(); } }
namespace NUISystem { void CComponent::SetColour(const CRGBColour &) { abort(); } }
namespace NUISystem { CRGBColour CComponent::GetColour() const { abort(); } }
namespace NUISystem { void CComponent::SetPosition(const C2DVector &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetPosition() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetParentPosition() const { abort(); } }
namespace NUISystem { void CComponent::SetParentPosition(C2DVector &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetParentZoom() const { abort(); } }
namespace NUISystem { void CComponent::SetParentZoom(C2DVector &) { abort(); } }
namespace NUISystem { CRGBColour CComponent::GetParentColour() const { abort(); } }
namespace NUISystem { void CComponent::SetParentColour(CRGBColour &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetSize() const { abort(); } }
namespace NUISystem { C2DBoxF CComponent::GetBoundingBox() const { abort(); } }
namespace NUISystem { const C2DVector & CComponent::GetRenderPosition() const { abort(); } }
namespace NUISystem { const C2DVector & CComponent::GetRenderZoom() const { abort(); } }
namespace NUISystem { const CRGBColour & CComponent::GetRenderColour() const { abort(); } }
namespace NUISystem { void CComponent::SetRenderPosition(C2DVector &) { abort(); } }
namespace NUISystem { void CComponent::SetRenderZoom(C2DVector &) { abort(); } }
namespace NUISystem { void CComponent::SetRenderColour(CRGBColour &) { abort(); } }
namespace NUISystem { void CComponent::ChangePosition(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::ChangePositionDelta(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::ChangeTargetPositionDelta(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::UpdatePosition(float) { abort(); } }
namespace NUISystem { void CComponent::ChangeZoom(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::ChangeZoomDelta(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::UpdateZoom(float) { abort(); } }
namespace NUISystem { void CComponent::ChangeColour(const CRGBColour &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::ChangeColourDelta(const CRGBColour &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::UpdateColour(float) { abort(); } }
namespace NUISystem { void CComponent::ResetState() { abort(); } }
namespace NUISystem { void CComponent::ForceState(unsigned long, bool) { abort(); } }
namespace NUISystem { void CComponent::Initialise() { abort(); } }
namespace NUISystem { bool CComponent::HasState(unsigned long) { abort(); } }
namespace NUISystem { bool CComponent::HasStateInChildren(unsigned long) { abort(); } }
namespace NUISystem { void CComponent::ChangeState(unsigned long) { abort(); } }
namespace NUISystem { void CComponent::ChangeState(unsigned long, float) { abort(); } }
namespace NUISystem { void CComponent::ChangeState(NUISystem::CUIState &) { abort(); } }
namespace NUISystem { bool CComponent::ChangedState() { abort(); } }
namespace NUISystem { NUISystem::CUIState * CComponent::GetCurrentState() { abort(); } }
namespace NUISystem { void CComponent::SetParent(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::GetParent() const { abort(); } }
namespace NUISystem { std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > & CComponent::GetChildren() { abort(); } }
namespace NUISystem { float CComponent::GetUpdateTime() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetPosition() const { abort(); } }
namespace NUISystem { CRGBColour CComponent::GetTargetColour() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetZoom() const { abort(); } }
namespace NUISystem { void CComponent::ProcessChangeState() { abort(); } }
namespace NUISystem { void CComponent::AddChild(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CComponent::AddChildFront(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CComponent::SetDeletion(NUISystem::CDeletion) { abort(); } }
namespace NUISystem { NUISystem::CDeletion & CComponent::GetDeletion() { abort(); } }
namespace NUISystem { void CComponent::RemoveChild(unsigned long) { abort(); } }
namespace NUISystem { NUISystem::EType CComponent::GetType() const { abort(); } }
namespace NUISystem { void CComponent::AddAction(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnSelected(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnBack(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddPreAction(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnInactive(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDropped(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnHovered(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::Die() { abort(); } }
namespace NUISystem { std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > > & CComponent::GetStates() { abort(); } }
namespace NUISystem { void CComponent::SetName(const CCharString &) { abort(); } }
namespace NUISystem { CCharString CComponent::GetName() { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindComponent(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { unsigned long CComponent::GetCurrentStateIndex() { abort(); } }
namespace NUISystem { unsigned long CComponent::GetTargetStateIndex() const { abort(); } }
namespace NUISystem { float CComponent::CanScroll(bool, C2DBoxF &) { abort(); } }
namespace NUISystem { void CComponent::FakeChildDying(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CComponent::SetParentViewport(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CComponent::ClearHandle() { abort(); } }
namespace NUISystem { void CComponent::MakeUnavailable() { abort(); } }
namespace NUISystem { void CComponent::MakeUnavailableDynamically() { abort(); } }
namespace NUISystem { void CComponent::MakeAvailable() { abort(); } }
namespace NUISystem { void CComponent::MakeAvailableDynamically() { abort(); } }
namespace NUISystem { void CComponent::AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindFirstChildOfTypeNonRecursive(NUISystem::EType) { abort(); } }
namespace NUISystem { bool CComponent::IsAllowedToScroll() const { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::GetParentViewport() { abort(); } }
namespace NUISystem { bool CComponent::IsBastardChild() const { abort(); } }
namespace NUISystem { bool CComponent::IsIndependent() const { abort(); } }
namespace NUISystem { bool CComponent::IsPositionIndependent() const { abort(); } }
namespace NUISystem { bool CComponent::IsZoomIndependent() const { abort(); } }
namespace NUISystem { bool CComponent::IsLayerIndependent() const { abort(); } }
namespace NUISystem { bool CComponent::IsDrawFromViewport() const { abort(); } }
namespace NUISystem { bool CComponent::IsPositionIsCenter() const { abort(); } }
namespace NUISystem { long CComponent::GetDefIndex() const { abort(); } }
namespace NUISystem { CDefPointer<NUISystem::CUIDef const > CComponent::LoadAndReturnDef() const { abort(); } }
namespace NUISystem { void CComponent::OnObserverRemove() { abort(); } }
namespace NUISystem { C2DVector CComponent::CanMove(const C2DVector &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetDynamicSize() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetRenderPosition() const { abort(); } }
namespace NUISystem { bool CComponent::IsPCStyle() const { abort(); } }
namespace NUISystem { void CComponent::SetRelativeParentPosition(C2DVector &) { abort(); } }
namespace NUISystem { void CComponent::SetRelativeParentZoom(C2DVector &) { abort(); } }
namespace NUISystem { bool CComponent::IsUsingRelativeZoom() const { abort(); } }
namespace NUISystem { bool CComponent::IsUsingRelativePosition() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetRelativeRenderPosition() { abort(); } }
namespace NUISystem { C2DVector CComponent::GetRelativeRenderZoom() { abort(); } }
namespace NUISystem { void CComponent::SetRelativeRenderPosition(C2DVector) { abort(); } }
namespace NUISystem { C2DVector CComponent::SetRelativeRenderZoom(C2DVector) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetRelativeRenderPosition() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetRelativeRenderZoom() const { abort(); } }
namespace NUISystem { void CComponent::AddPositionChild(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CComponent::SetPositionParent(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::GetPositionParent() { abort(); } }
namespace NUISystem { void CComponent::PositionChildDying(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CComponent::PositionParentDying(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { long CComponent::GetRenderLayer() const { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *) { abort(); } }
namespace NUISystem { bool CComponent::IsClickable() const { abort(); } }
namespace NUISystem { void CComponent::RemoveFromHoveredList() { abort(); } }
namespace NUISystem { void CComponent::ProcessEvent(EEvent) { abort(); } }
namespace NUISystem { void CChangingStateComponent::Initialise() { abort(); } }
namespace NUISystem { void CChangingStateComponent::Update(float) { abort(); } }
namespace NUISystem { void CChangingStateComponent::ForceState(unsigned long, bool) { abort(); } }
namespace NUISystem { void CChangingStateComponent::ChangeState(unsigned long, float) { abort(); } }
namespace NUISystem { void CChangingStateComponent::ChangeState(unsigned long) { abort(); } }
namespace NUISystem { NUISystem::CUIState * CChangingStateComponent::GetCurrentState() { abort(); } }
namespace NUISystem { bool CChangingStateComponent::HasState(unsigned long) { abort(); } }
namespace NUISystem { bool CChangingStateComponent::ChangedState() { abort(); } }
namespace NUISystem { unsigned long CChangingStateComponent::GetCurrentStateIndex() { abort(); } }
namespace NUISystem { unsigned long CChangingStateComponent::GetTargetStateIndex() const { abort(); } }
namespace NUISystem { float CChangingStateComponent::GetUpdateTime() const { abort(); } }
namespace NUISystem { void CChangingStateComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { bool CChangingStateComponent::IsPositionIndependent() const { abort(); } }
namespace NUISystem { bool CChangingStateComponent::IsZoomIndependent() const { abort(); } }
namespace NUISystem { void CChangingStateComponent::ProcessChangeState() { abort(); } }
namespace NUISystem { NUISystem::CUIState * CChangingStateComponent::GetState(unsigned long) { abort(); } }
namespace NUISystem { const NUISystem::CUIState * CChangingStateComponent::GetState(unsigned long) const { abort(); } }
namespace NUISystem { void CChangingStateComponent::UpdateStateChange() { abort(); } }
namespace NUISystem { bool CChangingStateComponent::ChangedStateLastUpdate() { abort(); } }
namespace NUISystem { bool CChangingStateComponent::InternalChanged() { abort(); } }
namespace NUISystem { bool CChangingStateComponent::AllChildrenChangedState() { abort(); } }
namespace NUISystem { void CChangingStateComponent::OnActivate() { abort(); } }
namespace NUISystem { void CChangingStateComponent::OnDeactivate() { abort(); } }
namespace NUISystem { void CText::Update(float) { abort(); } }
namespace NUISystem { const CWideString& CText::GetValue() { abort(); } }
namespace NUISystem { void CText::SetValue(const CCharString&, bool, bool) { abort(); } }
namespace NUISystem { void CText::SetValue(const CWideString&, bool, bool) { abort(); } }
namespace NUISystem { void CText::SetValue(unsigned long, const CWideString&, bool) { abort(); } }
namespace NUISystem { CCountedPointer<CFontBank>& CText::GetFont() { abort(); } }
namespace NUISystem { void CText::SetTextWindow(C2DBoxF&) { abort(); } }
namespace NUISystem { C2DBoxF& CText::GetTextWindow() { abort(); } }
namespace NUISystem { void CText::ForceWidth(float) { abort(); } }
namespace NUISystem { ETextAlignement CText::GetAlignment() const { abort(); } }
namespace NUISystem { bool CManager::GetMouseDisabled() { abort(); } }
namespace NUISystem { void CManager::SetKeyPressed(unsigned short) { abort(); } }
namespace NUISystem { unsigned short CManager::GetKeyPressed() { abort(); } }
namespace NUISystem { void CManager::Action(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CManager::SetWeaponsInventory(CTCInventoryWeapons *) { abort(); } }
namespace NUISystem { void CManager::SetMapInventory(CTCInventoryMap *) { abort(); } }
namespace NUISystem { void CManager::SetQuestInventory(CTCInventoryQuests *) { abort(); } }
namespace NUISystem { void CManager::SetTCHeroExperienceInventory(CTCHeroExperience *) { abort(); } }
namespace NUISystem { void CManager::SetAbilitiesInventory(CTCInventoryAbilities *) { abort(); } }
namespace NUISystem { void CManager::SetExperienceInventory(CTCInventoryExperience *) { abort(); } }
namespace NUISystem { void CManager::SetInventory(CTCInventory *) { abort(); } }
namespace NUISystem { void CManager::SetInventoryClothing(CTCInventoryClothing *) { abort(); } }
namespace NUISystem { void CManager::SetInventoryTrade(CTCInventoryTrade *) { abort(); } }
namespace NUISystem { void CManager::AddThingActionParam(long) { abort(); } }
namespace NUISystem { void CManager::RemoveThingActionParam(long) { abort(); } }
namespace NUISystem { void CManager::SetPlayerThing(CThing *) { abort(); } }
namespace NUISystem { void CManager::AddUIActionParam(NUISystem::CActionParam *) { abort(); } }
namespace NUISystem { void CManager::RemoveUIActionParam(NUISystem::CActionParam *) { abort(); } }
namespace NUISystem { void CManager::ClearActionParams() { abort(); } }
namespace NUISystem { void CManager::SetNumberAugmentations(unsigned long) { abort(); } }
namespace NUISystem { unsigned long CManager::GetNumberAugmentations() const { abort(); } }
namespace NUISystem { void CManager::SetChange(NUISystem::CManager::EChange) { abort(); } }
namespace NUISystem { NUISystem::CManager::EChange CManager::GetChange() { abort(); } }
namespace NUISystem { void CManager::SetMetaLayer(long) { abort(); } }
namespace NUISystem { long CManager::GetMetaLayer() { abort(); } }
namespace NUISystem { void CManager::ClearBastardChild() { abort(); } }
namespace NUISystem { void CManager::SetRightStickForce(C2DVector &) { abort(); } }
namespace NUISystem { C2DVector & CManager::GetRightStickForce() { abort(); } }
namespace NUISystem { void CManager::PlaySoundW(CCharString &,long) { abort(); } }
namespace NUISystem { unsigned long CManager::PlaySample(CCharString &,long) { abort(); } }
namespace NUISystem { CDefPointer<NUISystem::CUIMiscThingsDef> CManager::GetMiscThingsDef() { abort(); } }
namespace NUISystem { CDefPointer<NUISystem::CUIIconsDef> CManager::GetIconsDef() { abort(); } }
namespace NUISystem { void CManager::AddActionsListenIgnoreOnList(NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::EEvent) { abort(); } }
static long OracleRound(float value){unsigned bits=Bits(value),sign=bits>>31;int exponent=int((bits>>23)&255)-127;if(exponent< -1)return 0;if(exponent>31)return -2147483647L-1;unsigned mantissa=(bits&0x7fffffU)|0x800000U,magnitude;if(exponent>=23)magnitude=mantissa<<(exponent-23);else{unsigned shift=23-exponent,remainder=mantissa&((1U<<shift)-1),half=1U<<(shift-1);magnitude=mantissa>>shift;if(remainder>half||(remainder==half&&(magnitude&1)))++magnitude;}if(magnitude>=0x80000000U)return -2147483647L-1;return sign?-(long)magnitude:(long)magnitude;}
static unsigned RoundCases=0,TargetCases=0;static FILE*Outputs;
static void CheckRound(unsigned bits){float value;memcpy(&value,&bits,4);long expected=OracleRound(value),native=GFFloatToLongNear(value),portable=fable_portable::GFFloatToLongNear(value);assert(native==expected&&portable==expected);if(Outputs)fprintf(Outputs,"R %u %08x %ld\n",RoundCases,bits,native);++RoundCases;}
static void TestRounding(){unsigned raw[]={0,0x80000000U,1,0x80000001U,0x007fffffU,0x807fffffU,0x00800000U,0x80800000U,0x7f800000U,0xff800000U,0x7fc12345U,0xffc54321U,0x7f800001U,0xff800001U,0x4effffffU,0x4f000000U,0xcf000000U,0xcf000001U};for(unsigned i=0;i<sizeof(raw)/sizeof(raw[0]);++i)CheckRound(raw[i]);for(int n=-2048;n<=2048;++n)for(int k=-2;k<=3;++k)CheckRound(Bits(float(n)+float(k)*0.25f));unsigned random=0x534ef00U;for(unsigned i=0;i<65536;++i){random=random*1664525U+1013904223U;CheckRound(random);}}
static bool TestCase(unsigned code){unsigned q=code;int alpha=q%2;q/=2;int zoomIndex=q%4;q/=4;unsigned char noRender=(unsigned char)(q%3);q/=3;int posIndex=q%6;q/=6;bool scaleText=q%2;q/=2;int alignment=q%4;q/=4;bool unavailable=q%2;q/=2;int independent=q%4;q/=4;int parentMode=q%3;q/=3;int handleState=q%3;q/=3;int layerCase=q%2;q/=2;int relativeCase=q%2;q/=2;assert(q==0);
 const float zooms[4]={-1.0f,0.0f,0.5f,2.0f};const float xs[6]={-3.5f,12.25f,120.75f,128.0f,24.5f,24.5f};const float ys[6]={4.5f,20.75f,32.25f,16.0f,96.0f,110.5f};float zoom=zooms[zoomIndex];bool invisible=alpha==0||zoom<=0.0f;NUISystem::CUIScreenUnitConverter::IsUsingRelativeCoords=relativeCase!=0;float width=relativeCase?128.0f:640.0f,height=relativeCase?96.0f:480.0f;bool release=(invisible&&noRender==1)||xs[posIndex]>=width||ys[posIndex]>=height;if(release&&parentMode==2)return false;
 assert(FontLive==0&&PrimitiveLive==0&&FixtureStringLive==0);CallCount=LayerCalls=Notifications=Deletions=0;memset(Calls,0,sizeof Calls);FixtureEngine engine;CEnginePrimitiveHandle parent;ExpectedParent=&parent;FixturePersistent*owners[2]={0,0};long primitiveIndex=5;
 {
  NUISystem::FixtureText text;ExpectedFont=text.PFont.Data;text.RenderColour.IntValue=0;text.RenderColour.B=11;text.RenderColour.G=22;text.RenderColour.R=33;text.RenderColour.A=(unsigned char)(alpha?161:0);text.RenderZoom=C2DVector(zoom,1.0f);text.RelativeRenderPosition=C2DVector(xs[posIndex],ys[posIndex]);text.RelativeRenderZoom=C2DVector(relativeCase?1.5f:0.75f,1.0f);text.Size=C2DVector(16.0f,8.0f);text.Independent=(independent&1)!=0;text.LayerIndependent=(independent&2)!=0;text.Align=(NUISystem::ETextAlignement)alignment;text.Layer=(signed char)(layerCase?4:-2);text.ScaleText=scaleText;text.Unavailable=unavailable;text.NoRenderNextFrame=noRender;
  ExpectedOwn[0]=&text.Handle;ExpectedOwn[1]=&text.HandleDropShadowBottomRight;
  for(int i=0;i<2;++i)if(handleState){owners[i]=new FixturePersistent;if(handleState==2)owners[i]->AddRef();ExpectedOwn[i]->Data.Object=owners[i];}
  CEnginePrimitiveHandle*p_handle=parentMode?&parent:0;long*index=parentMode==2?0:&primitiveIndex;long inputLayer=layerCase?8:-3;text.Draw(engine,p_handle,inputLayer,index,0);
  unsigned char expectedNoRender=invisible?1:0;assert(text.NoRenderNextFrame==expectedNoRender&&CallCount==(release?(parentMode?2:0):2)&&FixtureStringLive==1&&FontLive==1);assert(text.PFont.Info&&text.PFont.Info->RefCount==1);bool child=parentMode==1;int expectedNotifications=release&&parentMode==0&&handleState==1?2:0;int expectedDeletions=expectedNotifications;assert(Notifications==expectedNotifications&&Deletions==expectedDeletions);assert(text.Handle.Data.IsNull()==(handleState==0||(release&&parentMode==0)));assert(text.HandleDropShadowBottomRight.Data.IsNull()==text.Handle.Data.IsNull());
  if(release){assert(LayerCalls==0);for(int i=0;i<2;++i){if(parentMode==1){assert(Calls[i].Kind=='C'&&Calls[i].Index==(unsigned)(5+i));}else assert(Calls[i].Kind==0);}if(parentMode==0)assert(CallCount==0);}
  else{
   assert(LayerCalls==2);long expectedLayer=(layerCase?4:-2)+((independent==0)?inputLayer:0);assert(Layers[0]==expectedLayer&&Layers[1]==expectedLayer+1);float scale=scaleText?(relativeCase?1.5f:0.75f):zoom;long x16=(long)(xs[posIndex]*16.0f),scale16=(long)(scale*16.0f);if(alignment==1)x16-=scale16*8;else if(alignment==2)x16-=scale16*16;float expectedX=(float)OracleRound((float)x16/16.0f),expectedY=(float)OracleRound(ys[posIndex]);unsigned foreground=unsigned(text.RenderColour.B)|(unsigned(text.RenderColour.G)<<8)|(unsigned(text.RenderColour.R)<<16)|(unsigned(text.RenderColour.A)<<24),black=unsigned(text.RenderColour.A)<<24;
   for(int i=0;i<2;++i){assert(Calls[i].Kind==(child?'D':'P'));assert(Calls[i].Flags==0x40&&Calls[i].Text==65&&Calls[i].Font==73);assert(Calls[i].X==expectedX+float(i*2)&&Calls[i].Y==expectedY+float(i*2)&&Calls[i].Z==0.0f&&Calls[i].Scale==scale);assert(Calls[i].Layer==1000+expectedLayer+i);unsigned colour=(i==0)?(unavailable?black:foreground):(unavailable?foreground:black);assert(Calls[i].Colour==colour);if(child)assert(Calls[i].Index==(unsigned)(5+i));}
  }
  assert(primitiveIndex==(child?7:5));if(Outputs){fprintf(Outputs,"T %u %u %ld %d %d %d",code,(unsigned)text.NoRenderNextFrame,primitiveIndex,CallCount,Notifications,Deletions);for(int i=0;i<CallCount;++i)fprintf(Outputs," %c,%u,%u,%08x,%08x,%08x,%08x,%08x,%ld,%u,%d",Calls[i].Kind,Calls[i].Index,Calls[i].Flags,Bits(Calls[i].X),Bits(Calls[i].Y),Bits(Calls[i].Z),Bits(Calls[i].Scale),Calls[i].Colour,Calls[i].Layer,Calls[i].Text,Calls[i].Font);fprintf(Outputs,"\n");}
  text.Handle.Clear();text.HandleDropShadowBottomRight.Clear();if(handleState==2)for(int i=0;i<2;++i){assert(owners[i]->GetRefCount()==1);if(owners[i]->Release()==0)owners[i]->Delete();}
 }
 assert(FontLive==0&&PrimitiveLive==0&&FixtureStringLive==0);++TargetCases;return true;
}
int main(int argc,char**argv){
#ifdef _MSC_VER
 unsigned control=_controlfp(0,0);assert((control&_MCW_RC)==_RC_NEAR&&(control&_EM_INVALID)!=0);
#endif
 Outputs=argc>1?fopen(argv[1],"w"):0;assert(argc<=1||Outputs);TestRounding();for(unsigned i=0;i<331776;++i)TestCase(i);assert(RoundCases==90136);assert(TargetCases==283392);if(Outputs)fclose(Outputs);printf("0054ef00_TEST PASS\n");return 0;}