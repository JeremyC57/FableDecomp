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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
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
// Layout: CPlayer.h ControlledCreature+0x2c and CPlayerManager.h MainPlayer+0x1c (retail STLport 4.6 layout)
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
// ForceAllowSubtitleToRender is RETAIL+0x2bd (native byte load), as engine/CPlayerGui.h (retail STLport 4.6 layout) has it;
// the Ego_r layout had +0x2cd.
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
typedef char GuiFlagRetail[offsetof(CPlayerGui,ForceAllowSubtitleToRender)==offsetof(CPlayerGuiPdb,ForceAllowSubtitleToRender)?1:-1];
typedef char GuiFlagInBounds[offsetof(CPlayerGui,ForceAllowSubtitleToRender)+sizeof(bool)<=sizeof(CPlayerGui)?1:-1];
typedef char RetailGamePointerOffset[offsetof(CTCGamePlayer,Game)==0xc?1:-1];
typedef char RetailGamePlayerExtent[sizeof(CTCGamePlayer)==0x18?1:-1];

typedef char DisplayPrepareOffset[offsetof(CDisplayEngine,PreparePrimitives)==0xb0?1:-1];
typedef char DisplayFlagsOffset[offsetof(CDisplayEngine,EnginePreloadFlags)==4?1:-1];
typedef char InputLoadingOffset[offsetof(CInputManager,Loading)==0x38?1:-1];