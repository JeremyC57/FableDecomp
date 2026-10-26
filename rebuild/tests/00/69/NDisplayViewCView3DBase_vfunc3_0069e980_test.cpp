#include <stdio.h>
#include <string.h>
// BEGIN exact source snapshot SHA256 aa1ec96bf9ec7747b03de64e63679dbb27dd710ae98f0667919f023f93926f44
// Shared donor layouts remain visible alongside the retail call views below.
#define CWorld CWorldPdb
#define CRenderManagerCore CRenderManagerCorePdb
#define CTCInGameMenu CTCInGameMenuPdb
#define CPlayer CPlayerPdb
#define CPlayerManager CPlayerManagerPdb
#define CDisplayEngine CDisplayEnginePdb
#define CTCTavernGame CTCTavernGamePdb
#define CSubtitleRenderer CSubtitleRendererPdb
#define CPlayerGui CPlayerGuiPdb
#include "engine/CWorld.h"
#include "engine/CRenderManagerCore.h"
#include "engine/CTCInGameMenu.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CDisplayEngine.h"
#include "engine/CTCTavernGame.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CPlayerGui.h"
#undef CWorld
#undef CRenderManagerCore
#undef CTCInGameMenu
#undef CPlayer
#undef CPlayerManager
#undef CDisplayEngine
#undef CTCTavernGame
#undef CSubtitleRenderer
#undef CPlayerGui

// NDisplayView::CView3DBase::Render(const CInterpolationInfo&,bool)
// PDB egor 13552 / debug 0x006c3850. Global spellings not recovered are explicitly descriptive.
class C3DVector { public: float X,Y,Z; C3DVector(){} C3DVector(const C3DVector&); };
class CRightHandedSet { public: C3DVector Up,Forward; CRightHandedSet(){} CRightHandedSet(const CRightHandedSet&); };
class C2DBoxF { public: float Left,Top,Right,Bottom; };
struct CInterpolationInfoSet { float GTPredictedTimeSinceLastRenderFrame,WFInterpolate; };
class CInterpolationInfo { public:
    CInterpolationInfoSet Current,Last,PausedCurrent,PausedLast,BulletTimeCurrent,BulletTimeLast;
    long WFServerCurrentTime;
};
class CCamera { public:
    C3DVector WorldPosition; CRightHandedSet RHSet; bool HeightLocked,ViewVecZLocked;
    unsigned char _pad26[2]; unsigned long FOVFlags; float HorizontalFOV,VerticalFOV,Zoom;
    CCamera(){} CCamera(const CCamera&);
};
struct CCameraMovementInfo { long NoUpdates; bool MovementVectorValid; CCamera CurrentCam,LastCam; C3DVector MovementVector; };
class CEnginePrimitivePrepareInfo { public:
    CInterpolationInfo Interpolate; class CIEngine* Engine; bool Edit2DEngine; unsigned char _pad39[3]; CCamera Camera;
    CEnginePrimitivePrepareInfo(const CInterpolationInfo& info,CIEngine* engine,bool edit,const CCamera& camera)
      : Interpolate(info),Engine(engine),Edit2DEngine(edit),Camera(camera) {}
};
struct CEngineInterpolationInfoSet {
    float GTPredictedRenderAt,GTPredictedTimeSinceLastRenderFrame,WFInterpolate;
    CEngineInterpolationInfoSet():GTPredictedRenderAt(0),GTPredictedTimeSinceLastRenderFrame(0),WFInterpolate(0){}
    CEngineInterpolationInfoSet& operator=(const CInterpolationInfoSet& s) { GTPredictedTimeSinceLastRenderFrame=s.GTPredictedTimeSinceLastRenderFrame;WFInterpolate=s.WFInterpolate;return *this; }
};
class CEngineInterpolationInfo { public:
    CEngineInterpolationInfoSet Current,Last,PausedCurrent,PausedLast,BulletTimeCurrent,BulletTimeLast;
    long WFServerCurrentTime;
    CEngineInterpolationInfoSet InterpolationSetList[3];
    CEngineInterpolationInfo(const CInterpolationInfo&); // landed 0069ede0, zero sets then assign
};
class C2DBoxI { public: long Left,Top,Right,Bottom; };
// Layout: original CDisplayEngine, not the CIEngine pointed to by its Engine3D member.
// Native640020/640030 return MainComponent.PDisplayEngine directly.
// Shared donor EnginePreloadFlags+4 and PreparePrimitives+b0 agree with retail.
class CDisplayEngine : public CDisplayEnginePdb { public:};
// Layout: CWorld.h describes a 0x1b8-byte donor object. This call view exposes only
// the primary vptr; Render reads no CWorld data and dispatches slot11 (+0x2c).
class CWorld { public: virtual void v00(){}; virtual void v01(){}; virtual void v02(){}; virtual void v03(){}; virtual void v04(){}; virtual void v05(){}; virtual void v06(){}; virtual void v07(){}; virtual void v08(){}; virtual void v09(){}; virtual void v10(){}; virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&); };
#define CIEngine CIEnginePdb
#include "engine/CIEngine.h"
#undef CIEngine
// RTTI012a0d24: CIEngine -> CInitBaseClass -> CBaseClass, all at offset0.
// PDB36001: CInitBaseClass.Valid+4; CIEngine.Active+8.
class CBaseClass { public: virtual ~CBaseClass() {} };
class CInitBaseClass : public CBaseClass { public: bool Valid; unsigned char ValidPadding[3]; };
class CIEngine : public CInitBaseClass { public:
    bool Active; unsigned char ActivePadding[3];
    virtual void v01(){};virtual void v02(){};virtual void v03(){};
    virtual void SetCamera(const CCamera*); virtual void SetWindow(const C2DBoxF*);
    virtual void v06(){};virtual void v07(){};virtual void Render(const CEngineInterpolationInfo&,unsigned long);
};
class CEngineManager { public: void* VTable; void* Engines; void* ChangedAreas[3]; void OnRender(); }; // retail 20B; STLport list/vector
template<class T> struct CTBaseSingleton { static T& __fastcall Get(); };
// Layout: CRenderManagerCore.h donor size0x3d20; this method-only call view reads
// no fields. SetVWindow receives an existing manager, never an allocated view.
struct CRenderManagerCore { void SetVWindow(const C2DBoxF&); };
// Layout: CTCInGameMenu.h donor size0x118; only its primary vptr slot11
// DrawPreparePrimitivesForRendering is accessed by this leaf.
class CTCInGameMenu { public: virtual void v00(){};virtual void v01(){};virtual void v02(){};virtual void v03(){};virtual void v04(){};virtual void v05(){};virtual void v06(){};virtual void v07(){};virtual void v08(){};virtual void v09(){};virtual void v10(){};virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&); };
struct CTCMap {
    struct Entry { long Type; void* Component; };
    Entry *First,*Last;
    Entry* LowerBound(const long*); // 0x0040f020
    const Entry* LowerBound(const long&) const; // 0x004365b0
    template<class T> __forceinline T* Get(long interfaceType) {
        long type=interfaceType;
        Entry* it=LowerBound(&type);
        return (T*)(it==Last || interfaceType<it->Type ? Last : it)->Component;
    }
    template<class T> __forceinline const T* Get(long interfaceType) const {
        long type=interfaceType;
        const Entry* it=LowerBound(type);
        return (const T*)(it==Last || interfaceType<it->Type ? Last : it)->Component;
    }
};
class CThingPlayerCreature { public:
    unsigned char _pad00[0x20]; unsigned long UsingComponents[9]; CTCMap Interfaces;
    unsigned char _pad4c[0x91-0x4c]; unsigned char Killed:1; unsigned char _padKilled:7;
    unsigned long HasTC(long type) const { return UsingComponents[type>>5] & (1UL<<(type&31)); }
};
// Layout: CPlayer.h ControlledCreature+0x2c and CPlayerManager.h MainPlayer+0x20
// belong to the external getters. These method-only views do not flatten or read them.
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; const CThingPlayerCreature* PeekPControlledCreature() const; };
class CPlayerManager { public: CPlayer& GetMainPlayer(); const CPlayer& GetMainPlayer() const; };
class CMainGameComponent { public: unsigned char _pad00[0x1c]; CPlayerManager* PPlayerManager; unsigned char _pad20[4]; CWorld* PWorld; CDisplayEngine* PDisplayEngine; };

// Layout: CTCTavernGame.h donor size0x108. This view exposes the primary virtual
// slot11 and IsBetting call only; none of its donor data is accessed.
class CTCTavernGame { public: bool IsBetting() const; virtual void v00(){};virtual void v01(){};virtual void v02(){};virtual void v03(){};virtual void v04(){};virtual void v05(){};virtual void v06(){};virtual void v07(){};virtual void v08(){};virtual void v09(){};virtual void v10(){};virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&); };
// Layout: PDB struct88222: Game+0xc and GameTable+0x10, size0x18, agree with retail.
// GameTable is an unused CIntelligentPointer<CThing> storage region, not a raw pointer.
class CTCGamePlayer { public: void* VTable; unsigned char ComponentState[8]; CTCTavernGame* Game; unsigned char GameTable[8]; /* CIntelligentPointer<CThing>, original +0x10, opaque to this caller */ CTCTavernGame* GetGame() const; };
// Layout: CSubtitleRenderer.h gives 0x128 bytes of donor storage. Only the
// receiver address is used here; SubtitleState reserves bounded fixture storage,
// without claiming that every retail subtitle member has its debug offset.
class CSubtitleRenderer { public: unsigned char SubtitleState[0x120]; void DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&); };
// Layout: CPlayerGui.h SubtitleRenderer+0x18 agrees with native0069e980.
// ForceAllowSubtitleToRender is donor+0x2cd but RETAIL+0x2bd (native byte load).
// The unnamed interval is deliberately opaque: debug STL sizes cannot identify
// its retail subfields. Both accessed regions are separately offset-checked below.
class CPlayerGui { public:
    unsigned char _pad00[0x18]; CSubtitleRenderer SubtitleRenderer; unsigned char BeforeForceSubtitle[0x2bd-0x138]; bool ForceAllowSubtitleToRender;
    bool IsOpen() const; void SetVWindowForDeprecatedDrawInterface(const C2DBoxF&);
    void DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&);
    void ClearGui(); void DrawReceivedItems(const CEnginePrimitivePrepareInfo&); void DrawInventory(const CEnginePrimitivePrepareInfo&);
    void DrawSaveScreen(const CEnginePrimitivePrepareInfo&); void DrawScreenFade(const CEnginePrimitivePrepareInfo&);
    void DrawBettingHUD(const CEnginePrimitivePrepareInfo&); void Draw2DText(const CEnginePrimitivePrepareInfo&); void DrawDebugSafeAreas(const CEnginePrimitivePrepareInfo&);
};
namespace NDisplayView {
class CViewBase { public:
    virtual void v00(){}; virtual void v01(){}; virtual void v02(){}; virtual void v03(){};
protected:
    CWorld& GetWorld(); const CDisplayEngine& PeekDisplayEngine() const; CDisplayEngine& GetDisplayEngine();
};
struct CView3DBase : CViewBase {
    virtual CCamera GetCamera(const CInterpolationInfo&); // vslot 4
    CIEngine* PEngine; C2DBoxF Window; CCameraMovementInfo CameraMovementInfo;
    void UpdateCameraMovementInfo(const CCamera&);
    void Render(const CInterpolationInfo&,bool);
};
struct CView3DThirdPerson : CView3DBase { CCamera GetCamera(const CInterpolationInfo&); };
}

// Descriptive views of global state whose original symbols remain unverified.
extern unsigned char g_RenderThrottleEnabled_013b860c;
extern unsigned char g_RenderThrottlePaused_013b8629;
extern unsigned long g_RenderFrame_013b7d70;
extern CMainGameComponent* g_MainGameComponent_013b86a0;
// Original CInputManager.Loading is debug+44, retail+38: three preceding
// vector headers each lose4 bytes in the retail STLport layout. The global
// input-manager role is independently established by native input consumers.
class CInputManager { public: unsigned char StateBeforeLoading[0x38]; bool Loading; };
extern CInputManager* g_pInputManager;
extern CRenderManagerCore* g_RenderManagerCore_013b8384;
extern CPlayerGui* g_PlayerGui_013b8790;
extern double g_DeprecatedScreenWidth_013961e8;
extern double g_DeprecatedScreenHeight_013961f0;
extern unsigned char g_BettingHudEnabled_013bae1d;

void NDisplayView::CView3DBase::Render(const CInterpolationInfo& interpolation,bool finalPass)
{
    if(g_RenderThrottleEnabled_013b860c && !g_RenderThrottlePaused_013b8629 &&
       g_pInputManager->Loading && g_RenderFrame_013b7d70%15!=0)
        return;

    PEngine->SetWindow(&Window);
    CCamera camera=GetCamera(interpolation);
    PEngine->SetCamera(&camera);
    CTBaseSingleton<CEngineManager>::Get().OnRender();
    g_RenderManagerCore_013b8384->SetVWindow(Window);
    UpdateCameraMovementInfo(camera);
    PEngine->SetWindow(&Window);
    PEngine->SetCamera(&camera);

    if(PeekDisplayEngine().PreparePrimitives) {
        CWorld& world=GetWorld();
        CEnginePrimitivePrepareInfo prepare(interpolation,PEngine,false,camera);
        world.PreparePrimitivesForEngine(prepare);
    }
    if(PeekDisplayEngine().EnginePreloadFlags!=1) {
        PeekDisplayEngine();
        GetDisplayEngine().EnginePreloadFlags=1;
    }

    CEnginePrimitivePrepareInfo guiPrepare(interpolation,PEngine,false,camera);
    if(g_PlayerGui_013b8790->IsOpen()) {
        C2DBoxF fullWindow;
        fullWindow.Left=0.0f;fullWindow.Top=0.0f;
        fullWindow.Right=(float)g_DeprecatedScreenWidth_013961e8;
        fullWindow.Bottom=(float)g_DeprecatedScreenHeight_013961f0;
        g_PlayerGui_013b8790->SetVWindowForDeprecatedDrawInterface(fullWindow);
        g_PlayerGui_013b8790->DrawPreparePrimitivesForEngine(guiPrepare);
        CPlayer& player=g_MainGameComponent_013b86a0->PPlayerManager->GetMainPlayer();
        CThingPlayerCreature* creature=player.GetPControlledCreature();
        if(creature && !creature->Killed && creature->HasTC(0x102)) {
            CTCInGameMenu* menu=creature->Interfaces.Get<CTCInGameMenu>(0x102);
            menu->DrawPreparePrimitivesForRendering(guiPrepare);
            if(g_BettingHudEnabled_013bae1d) g_PlayerGui_013b8790->DrawBettingHUD(guiPrepare);
        }
    } else {
        g_PlayerGui_013b8790->ClearGui();
        g_PlayerGui_013b8790->DrawReceivedItems(guiPrepare);
        g_PlayerGui_013b8790->DrawInventory(guiPrepare);
        g_PlayerGui_013b8790->DrawSaveScreen(guiPrepare);
        g_PlayerGui_013b8790->DrawScreenFade(guiPrepare);
        if(g_PlayerGui_013b8790->ForceAllowSubtitleToRender) {
            g_PlayerGui_013b8790->SubtitleRenderer.DrawPreparePrimitivesForEngine(guiPrepare);
            const CPlayer& player=((const CPlayerManager*)g_MainGameComponent_013b86a0->PPlayerManager)->GetMainPlayer();
            const CThingPlayerCreature* creature=player.PeekPControlledCreature();
            if(creature && !creature->Killed && creature->HasTC(0xd0)) {
                const CTCGamePlayer* gamePlayer=creature->Interfaces.Get<CTCGamePlayer>(0xd0);
                CTCTavernGame* game=gamePlayer->GetGame();
                if(game) {
                    if(game->IsBetting()) g_PlayerGui_013b8790->DrawBettingHUD(guiPrepare);
                    game->DrawPreparePrimitivesForRendering(guiPrepare);
                }
            }
        }
        g_PlayerGui_013b8790->Draw2DText(guiPrepare);
    }
    g_PlayerGui_013b8790->DrawDebugSafeAreas(guiPrepare);
    {
    CEngineInterpolationInfo engineInterpolation(interpolation);
    PEngine->Render(engineInterpolation,finalPass?1UL:0UL);
    }
}

// Original-layout guards: bounded storage for every field read by this leaf.
#include <stddef.h>
typedef char CameraSizeCheck[sizeof(CCamera)==56?1:-1];
typedef char PrepareSizeCheck[sizeof(CEnginePrimitivePrepareInfo)==116?1:-1];
typedef char InterpolationSizeCheck[sizeof(CInterpolationInfo)==52?1:-1];
typedef char EngineInterpolationSizeCheck[sizeof(CEngineInterpolationInfo)==112?1:-1];
typedef char ViewSizeCheck[sizeof(NDisplayView::CView3DBase)==156?1:-1];
typedef char EngineSizeCheck[sizeof(CIEngine)==sizeof(CIEnginePdb)?1:-1];
typedef char KilledOffsetCheck[sizeof(CThingPlayerCreature)>=0x92?1:-1];
typedef char GuiSubtitleOffset[offsetof(CPlayerGui,SubtitleRenderer)==0x18?1:-1];
typedef char GuiFlagOffset[offsetof(CPlayerGui,ForceAllowSubtitleToRender)==0x2bd?1:-1];

typedef char SharedWorldExtent[sizeof(CWorldPdb)==0x198?1:-1];
typedef char SharedRenderManagerExtent[sizeof(CRenderManagerCorePdb)==0x3d20?1:-1];
typedef char SharedMenuExtent[sizeof(CTCInGameMenuPdb)==0xf8?1:-1];
typedef char SharedPlayerControlledOffset[offsetof(CPlayerPdb,ControlledCreature)==0x2c?1:-1];
typedef char SharedPlayerManagerIndexOffset[offsetof(CPlayerManagerPdb,MainPlayer)==0x1c?1:-1];
typedef char SharedDisplayEngineOffset[offsetof(CDisplayEnginePdb,Engine3D)==0x2c?1:-1];
typedef char RetailDisplayEngineOffset[offsetof(CDisplayEngine,Engine3D)==offsetof(CDisplayEnginePdb,Engine3D)?1:-1];
typedef char SharedTavernExtent[sizeof(CTCTavernGamePdb)==0xf0?1:-1];
typedef char SharedSubtitleExtent[sizeof(CSubtitleRenderer)==sizeof(CSubtitleRendererPdb)?1:-1];
typedef char SharedGuiSubtitleOffset[offsetof(CPlayerGui,SubtitleRenderer)==offsetof(CPlayerGuiPdb,SubtitleRenderer)?1:-1];
typedef char GuiFlagRetailDrift[offsetof(CPlayerGui,ForceAllowSubtitleToRender)==offsetof(CPlayerGuiPdb,ForceAllowSubtitleToRender)?1:-1];
typedef char GuiFlagInBounds[offsetof(CPlayerGui,ForceAllowSubtitleToRender)+sizeof(bool)<=sizeof(CPlayerGui)?1:-1];
typedef char RetailGamePointerOffset[offsetof(CTCGamePlayer,Game)==0xc?1:-1];
typedef char RetailGamePlayerExtent[sizeof(CTCGamePlayer)==0x18?1:-1];

typedef char DisplayPrepareOffset[offsetof(CDisplayEngine,PreparePrimitives)==0xb0?1:-1];
typedef char DisplayFlagsOffset[offsetof(CDisplayEngine,EnginePreloadFlags)==4?1:-1];
typedef char InputLoadingOffset[offsetof(CInputManager,Loading)==0x38?1:-1];

// END exact source snapshot
static int events[64],eventCount,prepareResult,preloadFlag,guiOpen,forceSubtitle,hasControlled,controlledKilled,controlledHasTC,inGameCalls,betHudFlag,bettingResult,gamePresent;
static CInterpolationInfo expectedInfo;static CCamera expectedCamera;static int badPayload,worldPrepareCalls,guiPrepareCalls,renderCalls;
static unsigned long renderFlags;static bool throttleOn,throttlePaused,throttleActive;static unsigned long frameNumber;
static CDisplayEngine engine3d;static CWorld worldObject;static CIEngine engineObject;static CPlayerGui guiObject;static CPlayerManager playerManager;static CPlayer player;static CThingPlayerCreature creature;static CTCInGameMenu ingame;static CTCGamePlayer gamePlayer;static CTCTavernGame tavern;
unsigned char g_RenderThrottleEnabled_013b860c=0;unsigned char g_RenderThrottlePaused_013b8629=0;unsigned long g_RenderFrame_013b7d70=0;CInputManager* g_pInputManager=0;
static CMainGameComponent mainGame;CMainGameComponent* g_MainGameComponent_013b86a0=&mainGame;CRenderManagerCore* g_RenderManagerCore_013b8384=0;CPlayerGui* g_PlayerGui_013b8790=&guiObject;
double g_DeprecatedScreenWidth_013961e8=65536.0,g_DeprecatedScreenHeight_013961f0=65536.0;unsigned char g_BettingHudEnabled_013bae1d=0;
static void event(int e){events[eventCount++]=e;}
C3DVector::C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}
CRightHandedSet::CRightHandedSet(const CRightHandedSet& o):Up(o.Up),Forward(o.Forward){}
CCamera::CCamera(const CCamera& o):WorldPosition(o.WorldPosition),RHSet(o.RHSet),HeightLocked(o.HeightLocked),ViewVecZLocked(o.ViewVecZLocked),FOVFlags(o.FOVFlags),HorizontalFOV(o.HorizontalFOV),VerticalFOV(o.VerticalFOV),Zoom(o.Zoom){}
CEngineInterpolationInfo::CEngineInterpolationInfo(const CInterpolationInfo& i){
    Current=i.Current;Last=i.Last;PausedCurrent=i.PausedCurrent;PausedLast=i.PausedLast;
    BulletTimeCurrent=i.BulletTimeCurrent;BulletTimeLast=i.BulletTimeLast;WFServerCurrentTime=i.WFServerCurrentTime;
    InterpolationSetList[0]=i.Current;InterpolationSetList[1]=i.PausedCurrent;InterpolationSetList[2]=i.BulletTimeCurrent;event(20);
}
static bool sameCamera(const CCamera& c) { return c.WorldPosition.X==expectedCamera.WorldPosition.X && c.WorldPosition.Y==expectedCamera.WorldPosition.Y && c.WorldPosition.Z==expectedCamera.WorldPosition.Z && c.RHSet.Up.X==expectedCamera.RHSet.Up.X && c.RHSet.Up.Y==expectedCamera.RHSet.Up.Y && c.RHSet.Up.Z==expectedCamera.RHSet.Up.Z && c.RHSet.Forward.X==expectedCamera.RHSet.Forward.X && c.RHSet.Forward.Y==expectedCamera.RHSet.Forward.Y && c.RHSet.Forward.Z==expectedCamera.RHSet.Forward.Z && c.HeightLocked==expectedCamera.HeightLocked && c.ViewVecZLocked==expectedCamera.ViewVecZLocked && c.FOVFlags==expectedCamera.FOVFlags && c.HorizontalFOV==expectedCamera.HorizontalFOV && c.VerticalFOV==expectedCamera.VerticalFOV && c.Zoom==expectedCamera.Zoom; }
static void checkprepare(const CEnginePrimitivePrepareInfo& p) { if(p.Engine!=&engineObject || p.Edit2DEngine || memcmp(&p.Interpolate,&expectedInfo,sizeof(expectedInfo)) || !sameCamera(p.Camera))++badPayload; }
static bool sameSet(const CEngineInterpolationInfoSet& a,const CInterpolationInfoSet& b) {return a.GTPredictedRenderAt==0 && a.GTPredictedTimeSinceLastRenderFrame==b.GTPredictedTimeSinceLastRenderFrame && a.WFInterpolate==b.WFInterpolate;}

void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& p){event(2);++worldPrepareCalls;checkprepare(p);}
void CIEngine::SetCamera(const CCamera* c){event(4);if(!sameCamera(*c))++badPayload;}void CIEngine::SetWindow(const C2DBoxF*){event(5);}void CIEngine::Render(const CEngineInterpolationInfo& i,unsigned long b){event(6);++renderCalls;
 if(b!=renderFlags || i.WFServerCurrentTime!=expectedInfo.WFServerCurrentTime || !sameSet(i.Current,expectedInfo.Current) || !sameSet(i.Last,expectedInfo.Last) || !sameSet(i.PausedCurrent,expectedInfo.PausedCurrent) || !sameSet(i.PausedLast,expectedInfo.PausedLast) || !sameSet(i.BulletTimeCurrent,expectedInfo.BulletTimeCurrent) || !sameSet(i.BulletTimeLast,expectedInfo.BulletTimeLast) || !sameSet(i.InterpolationSetList[0],expectedInfo.Current) || !sameSet(i.InterpolationSetList[1],expectedInfo.PausedCurrent) || !sameSet(i.InterpolationSetList[2],expectedInfo.BulletTimeCurrent))++badPayload; }
void CEngineManager::OnRender(){event(3);}template<> CEngineManager& __fastcall CTBaseSingleton<CEngineManager>::Get(){static CEngineManager m;return m;}
void CRenderManagerCore::SetVWindow(const C2DBoxF&){event(7);}
CTCMap::Entry* CTCMap::LowerBound(const long* key) { ++inGameCalls; Entry* p=First;while(p!=Last && p->Type<*key)++p;return p; }
const CTCMap::Entry* CTCMap::LowerBound(const long& key) const { const Entry* p=First;while(p!=Last && p->Type<key)++p;return p; }
CPlayer& CPlayerManager::GetMainPlayer(){event(8);return player;}const CPlayer& CPlayerManager::GetMainPlayer()const{event(8);return player;}
CThingPlayerCreature* CPlayer::GetPControlledCreature()const{event(9);return hasControlled?&creature:0;}const CThingPlayerCreature* CPlayer::PeekPControlledCreature()const{event(9);return hasControlled?&creature:0;}
void CTCInGameMenu::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& p){event(10);checkprepare(p);}
bool CTCTavernGame::IsBetting()const{event(11);return bettingResult!=0;}void CTCTavernGame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& p){event(12);checkprepare(p);}
CTCTavernGame* CTCGamePlayer::GetGame()const{event(13);return Game;}
void CSubtitleRenderer::DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& p){checkprepare(p);event(14);}
bool CPlayerGui::IsOpen()const{return guiOpen!=0;}void CPlayerGui::SetVWindowForDeprecatedDrawInterface(const C2DBoxF& b){event(15);if(b.Left!=0||b.Top!=0||b.Right!=65536.0f||b.Bottom!=65536.0f)event(97);}
void CPlayerGui::DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& p){checkprepare(p);event(16);}void CPlayerGui::ClearGui(){event(17);}void CPlayerGui::DrawReceivedItems(const CEnginePrimitivePrepareInfo& p){checkprepare(p);event(18);}void CPlayerGui::DrawInventory(const CEnginePrimitivePrepareInfo& p){checkprepare(p);event(19);}void CPlayerGui::DrawSaveScreen(const CEnginePrimitivePrepareInfo& p){checkprepare(p);event(21);}void CPlayerGui::DrawScreenFade(const CEnginePrimitivePrepareInfo& p){checkprepare(p);event(22);}void CPlayerGui::DrawBettingHUD(const CEnginePrimitivePrepareInfo& p){checkprepare(p);event(23);}void CPlayerGui::Draw2DText(const CEnginePrimitivePrepareInfo& p){checkprepare(p);event(24);}void CPlayerGui::DrawDebugSafeAreas(const CEnginePrimitivePrepareInfo& p){checkprepare(p);event(25);}
CCamera NDisplayView::CView3DBase::GetCamera(const CInterpolationInfo&){event(26);return expectedCamera;}
CCamera NDisplayView::CView3DThirdPerson::GetCamera(const CInterpolationInfo& i){return CView3DBase::GetCamera(i);}
CWorld& NDisplayView::CViewBase::GetWorld(){return worldObject;}const CDisplayEngine& NDisplayView::CViewBase::PeekDisplayEngine()const{return engine3d;}CDisplayEngine& NDisplayView::CViewBase::GetDisplayEngine(){return engine3d;}
void NDisplayView::CView3DBase::UpdateCameraMovementInfo(const CCamera&){event(27);}

static int countEvent(int code){int n=0;for(int i=0;i<eventCount;++i)if(events[i]==code)++n;return n;}
static bool ordered(const int* wanted,int n){int j=0;for(int i=0;i<eventCount && j<n;++i)if(events[i]==wanted[j])++j;return j==n;}
static void resetEvents(){eventCount=inGameCalls=worldPrepareCalls=guiPrepareCalls=renderCalls=badPayload=0;}
int main(){
    static NDisplayView::CView3DBase view;static CInputManager inputManager;static CRenderManagerCore core;
    g_pInputManager=&inputManager;g_RenderManagerCore_013b8384=&core;mainGame.PPlayerManager=&playerManager;mainGame.PWorld=&worldObject;
    CInterpolationInfoSet* sets[6]={&expectedInfo.Current,&expectedInfo.Last,&expectedInfo.PausedCurrent,&expectedInfo.PausedLast,&expectedInfo.BulletTimeCurrent,&expectedInfo.BulletTimeLast};
    for(int k=0;k<6;++k){sets[k]->GTPredictedTimeSinceLastRenderFrame=0.125f*(k+1);sets[k]->WFInterpolate=0.0625f*(k+2);}expectedInfo.WFServerCurrentTime=1234567;
    expectedCamera.WorldPosition.X=11;expectedCamera.WorldPosition.Y=12;expectedCamera.WorldPosition.Z=13;
    expectedCamera.RHSet.Up.X=1;expectedCamera.RHSet.Up.Y=2;expectedCamera.RHSet.Up.Z=3;expectedCamera.RHSet.Forward.X=4;expectedCamera.RHSet.Forward.Y=5;expectedCamera.RHSet.Forward.Z=6;
    expectedCamera.HeightLocked=true;expectedCamera.ViewVecZLocked=false;expectedCamera.FOVFlags=0x102;expectedCamera.HorizontalFOV=75;expectedCamera.VerticalFOV=45;expectedCamera.Zoom=1.25f;
    static CTCMap::Entry entries[3];entries[0].Type=0xd0;entries[0].Component=&gamePlayer;entries[1].Type=0x102;entries[1].Component=&ingame;entries[2].Type=0x7fffffff;entries[2].Component=0;
    creature.Interfaces.First=entries;creature.Interfaces.Last=entries+2;
    creature.UsingComponents[0x102>>5]=1UL<<(0x102&31);creature.UsingComponents[0xd0>>5]=1UL<<(0xd0&31);
    view.PEngine=&engineObject;view.Window.Left=1;view.Window.Top=2;view.Window.Right=3;view.Window.Bottom=4;
    guiOpen=1;hasControlled=1;creature.Killed=0;g_BettingHudEnabled_013bae1d=1;renderFlags=1;engine3d.PreparePrimitives=true;engine3d.EnginePreloadFlags=0;
    view.Render(expectedInfo,true);
    const int openOrder[]={5,26,4,3,7,27,5,4,2,15,16,8,9,10,23,25,20,6};
    if(badPayload||!ordered(openOrder,18)||inGameCalls!=1||worldPrepareCalls!=1||renderCalls!=1||engine3d.EnginePreloadFlags!=1)return 1;
    // Missing, dead, and component-absent creatures must not enter menu lookup.
    for(int state=0;state<3;++state){resetEvents();hasControlled=state!=0;creature.Killed=state==1;creature.UsingComponents[8]=state==2?0:4;view.Render(expectedInfo,true);if(badPayload||inGameCalls||countEvent(10)||countEvent(23)||renderCalls!=1)return 2;}
    hasControlled=1;creature.Killed=0;creature.UsingComponents[8]=4;
    resetEvents();g_BettingHudEnabled_013bae1d=0;view.Render(expectedInfo,true);if(badPayload||countEvent(10)!=1||countEvent(23))return 3;
    // Throttle only skips an active unpaused non-fifteenth frame.
    g_RenderThrottleEnabled_013b860c=1;inputManager.Loading=true;g_RenderFrame_013b7d70=1;g_RenderThrottlePaused_013b8629=0;
    resetEvents();view.Render(expectedInfo,true);if(eventCount)return 4;
    g_RenderFrame_013b7d70=15;resetEvents();view.Render(expectedInfo,true);if(badPayload||renderCalls!=1)return 5;
    g_RenderFrame_013b7d70=1;g_RenderThrottlePaused_013b8629=1;resetEvents();view.Render(expectedInfo,true);if(badPayload||renderCalls!=1)return 6;
    g_RenderThrottleEnabled_013b860c=0;guiOpen=0;renderFlags=0;guiObject.ForceAllowSubtitleToRender=true;gamePlayer.Game=&tavern;bettingResult=1;engine3d.PreparePrimitives=false;
    resetEvents();view.Render(expectedInfo,false);const int closedOrder[]={17,18,19,21,22,14,8,9,13,11,23,12,24,25,20,6};
    if(badPayload||!ordered(closedOrder,16)||worldPrepareCalls||countEvent(12)!=1||countEvent(23)!=1)return 7;
    bettingResult=0;resetEvents();view.Render(expectedInfo,false);if(badPayload||countEvent(12)!=1||countEvent(23))return 8;
    gamePlayer.Game=0;resetEvents();view.Render(expectedInfo,false);if(badPayload||countEvent(13)!=1||countEvent(11)||countEvent(12))return 9;
    gamePlayer.Game=&tavern;creature.UsingComponents[0xd0>>5]=0;resetEvents();view.Render(expectedInfo,false);if(badPayload||countEvent(13)||countEvent(12))return 10;
    creature.UsingComponents[0xd0>>5]=1UL<<(0xd0&31);guiObject.ForceAllowSubtitleToRender=false;resetEvents();view.Render(expectedInfo,false);if(badPayload||countEvent(14)||countEvent(8)||countEvent(24)!=1)return 11;
    puts("0069e980_TEST PASS");return 0;
}