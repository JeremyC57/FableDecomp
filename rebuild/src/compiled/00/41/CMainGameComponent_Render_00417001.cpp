// CMainGameComponent::Render() -- build this frame's interpolation info (live, or held while the world is paused),
//   hand it to the world and, when the network client allows it, render a frame and time it; a pending forced-render
//   count renders catch-up frames instead
// Leaf: ?Render@CMainGameComponent@@MAEXXZ
// Retail: Fable.exe @ 0x00417001, 620 bytes, frame pointer, no EH.
// Layout: the landed CMainGameComponent::ForceRender 0x004164E0 model (retail offsets); CWorld PBulletTimeManager +0x68 and
//   Paused +0xa4, CDisplayEngine +0x68 (copied to the global 0x013B7D6C) from the retail loads.
// Callees: SetCurrentThingManager 0x004C74F0, GFGetSystemManager 0x009A4EC0, CSystemManager::IsApplicationActive 0x009A57B0,
//   CWorld::GetWorldFrame 0x0049D870, ForceRender 0x004164E0, GetGameTimeOfNextPresentCompletion 0x004166E2,
//   GFLimitReturn<double> 0x0041919C, 0x00416258 (game time since the last render-frame start), IsBulletTimeActive 0x00419680,
//   ConvertInterpolateToBulletTimeInterpolate 0x004C6010, CWorld::RenderUpdate 0x0049E080, CNetworkClient 0x004AEA70
//   (name unknown), GetCurrentGameTime 0x00416231, CDisplayEngine::DoRender 0x00435F70.
// Shape: the clamp constants are scoped per arm; the interpolate goes through an inline helper (retail keeps the game
//   time on the x87 stack); the second bullet-time call uses an inline getter; the render duration is assigned BEFORE
//   the counter updates (VC7.1 sinks the fsub past them, as in ForceRender).
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CASoundBank;
struct CPlayerManager;
struct CGamePlayerInterface;
struct CMainGameComponentParent;
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
template <class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) { }
    T* operator->() const { return ptr; }
};
namespace NGameText { struct CDataBank; }
struct CLUA;
struct CFontBank;
struct CGraphicDataBank;
struct CInterpolationInfoSet {
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};
struct CInterpolationInfo {                        // 0x34 bytes
    CInterpolationInfoSet Current;                 // +0x00
    CInterpolationInfoSet Last;                    // +0x08
    CInterpolationInfoSet PausedCurrent;           // +0x10
    CInterpolationInfoSet PausedLast;              // +0x18
    CInterpolationInfoSet BulletTimeCurrent;       // +0x20
    CInterpolationInfoSet BulletTimeLast;          // +0x28
    long                  WFServerCurrentTime;     // +0x30
    CInterpolationInfo();                          // 0x00415A60
};
struct CThingManager;
void __fastcall SetCurrentThingManager(CThingManager* manager);   // 0x004C74F0
struct CSystemManager { bool IsApplicationActive(); };            // 0x009A57B0
CSystemManager* __fastcall GFGetSystemManager();                    // 0x009A4EC0
struct CBulletTimeManager {
    bool IsBulletTimeActive() const;                                // 0x00419680
    float ConvertInterpolateToBulletTimeInterpolate(float interpolate);   // 0x004C6010
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();                       // slot 3 (+0x0c)
    unsigned char       _pad04[0x68 - 4];
    CBulletTimeManager* PBulletTimeManager;                         // +0x68
    CBulletTimeManager* GetBulletTimeManager() const { return PBulletTimeManager; }
    unsigned char       _pad6c[0xa4 - 0x6c];
    bool                Paused;                                     // +0xa4
    void RenderUpdate(CInterpolationInfo* info);                    // 0x0049E080
    static long GetWorldFrame();                                    // 0x0049D870
};
template <class T> const T& __fastcall GFLimitReturn(const T& value, const T& lower, const T& upper);   // 0x0041919C
extern long g_framesPerSecond;                     // 0x01375550
extern long g_ForceRenderFrames;                   // 0x013B8630
extern bool g_b13B8688;                            // 0x013B8688
extern long g_l13B7D6C;                            // 0x013B7D6C
struct CDisplayEngine {
    unsigned char _pad00[0x68];
    long          f68;                             // +0x68
    void DoRender(CInterpolationInfo* info, bool worldUpdateTurn, bool b);   // 0x00435F70
};
struct CGameEventPackageSet { long _body[0x13a14 / 4]; CGameEventPackageSet(); };   // 0x009F1A80
struct CNetworkClient { long _body[0x2680 / 4]; CNetworkClient(); bool IsReadyToRender(); };   // 0x00419323 / 0x004AEA70
struct CDiskFileWin32 { long _body[0x1c / 4]; CDiskFileWin32(); };                  // 0x00415C30
struct CEventPackageFileHeader { long Version; };
struct CFrameRateSmoother { long _body[0x30 / 4]; CFrameRateSmoother(); };          // 0x00419747
struct CRoughFrameCounter { long _body[0x8 / 4]; CRoughFrameCounter(); };           // 0x009F8B10
struct CMainGameComponentInit {
    long _body[0x10 / 4];
    CMainGameComponentInit(const CMainGameComponentInit& other);                          // 0x00415E17
};
struct CBaseClass { virtual ~CBaseClass(); };
struct CComponentListEntry { virtual void v00(); };                                      // the +4 interface
struct CGameComponent : CBaseClass, CComponentListEntry {
    bool  m_b08;                                   // +0x08
    bool  m_b09;                                   // +0x09
    CMainGameComponentParent* m_pParent;           // +0x0c
    CGameComponent(CMainGameComponentParent* parent);   // 0x0051D8E0
};
struct CComponentList {
    void AddComponent(CComponentListEntry* component);  // 0x009C0190
};
extern CComponentList* g_pComponentList;           // 0x013B8390
class CMainGameComponent : public CGameComponent {
public:
    CASoundBank*                         PSampleBank;               // +0x10
    CCountedPointer<NGameText::CDataBank> PTextBank;                // +0x14
    scoped_ptr<CPlayerManager>           PPlayerManager;            // +0x1c
    scoped_ptr<CGamePlayerInterface>     PPlayerInterface;          // +0x20
    scoped_ptr<CWorld>                   PWorld;                    // +0x24
    scoped_ptr<CDisplayEngine>           PDisplayEngine;            // +0x28
    CCountedPointer<CLUA>                PLUA;                      // +0x2c
    bool                                 ForceUpdateTick;           // +0x34
    float                                ForceUpdateTickSpeedMultipler;          // +0x38
    float                                ForceUpdateTickSpeedDesiredFramerate;   // +0x3c
    long                                 ForceUpdateNoFailedUpdates;             // +0x40
    bool                                 FirstWorldFrameUpdate;     // +0x44
    long                                 CurrentServerFrame;        // +0x48
    long                                 InputServerFrame;          // +0x4c
    long                                 LastGameTurnForceRendered; // +0x50
    double                               CurrentFrameStartGameTime; // +0x58
    double                               GameStartTime;             // +0x60
    double                               LastFrameRenderDuration;   // +0x68
    CInterpolationInfo                   LastInterpolationInfo;     // +0x70
    CGameEventPackageSet                 EventPackageSet;           // +0xa4
    CNetworkClient                       Client;                    // +0x13ab8
    unsigned long                        NoRenderFramesSinceLastGameUpdate;   // +0x16138
    unsigned long                        WorldSeed;                 // +0x1613c
    unsigned long                        LocalSeed;                 // +0x16140
    CCountedPointer<CFontBank>           PDebugFont;                // +0x16144
    CCountedPointer<CFontBank>           PCutSceneMainFont;         // +0x1614c
    CDiskFileWin32                       EventPackageFile;          // +0x16154
    bool                                 LoadingEventPackages;      // +0x16170
    bool                                 SavingEventPackages;       // +0x16171
    CEventPackageFileHeader              EventPackageFileHeader;    // +0x16174
    CFrameRateSmoother                   FrameRateSmoother;         // +0x16178
    double                               LastRenderFrameStartTime;  // +0x161a8
    float                                TimePassedSinceLastUpdate; // +0x161b0
    float                                LastUpdateTime;            // +0x161b4
    bool                                 WorldUpdateTurn;           // +0x161b8
    CRoughFrameCounter                   RoughFPSCounter;           // +0x161bc
    CGameComponent*                      NextComponentToRun;        // +0x161c4
    CCountedPointer<CGraphicDataBank>    PMainGraphicBank;          // +0x161c8
    CMainGameComponentInit               InitStructure;             // +0x161d0
    bool                                 Initialised;               // +0x161e0
    bool                                 AllowRender;               // +0x161e1
    bool                                 Rendered;                  // +0x161e2
    long                                 DebugNoFramesUnableToRender;   // +0x161e4
    double GetCurrentGameTime();                  // 0x00416231
    double GetTimeSinceLastRenderFrameStart();    // 0x00416258
    double GetGameTimeOfNextPresentCompletion();  // 0x004166E2
    void ForceRender(long frames);                // 0x004164E0
    void Render();
    __forceinline double GetServerFrameInterpolate(double gameTime) { return gameTime / (1.0f / g_framesPerSecond) - CurrentServerFrame; }
    virtual ~CMainGameComponent();
    virtual void v00();
};

void CMainGameComponent::Render()
{
    CInterpolationInfo info;
    SetCurrentThingManager(PWorld->GetThingManager());
    if (GFGetSystemManager()->IsApplicationActive() && AllowRender && CWorld::GetWorldFrame() > 1) {
        if (g_ForceRenderFrames > 0) {
            if (g_ForceRenderFrames < g_framesPerSecond)
                g_ForceRenderFrames = g_framesPerSecond;
            ForceRender(g_ForceRenderFrames);
            Rendered = true;
        } else {
            if (!PWorld->Paused) {
                double one = 1.0;
                double zero = 0.0;
                info.Last = LastInterpolationInfo.Current;
                info.PausedLast = LastInterpolationInfo.PausedCurrent;
                info.WFServerCurrentTime = CurrentServerFrame;
                double interpolate = GetServerFrameInterpolate(GetGameTimeOfNextPresentCompletion());
                info.Current.WFInterpolate = (float)GFLimitReturn(interpolate, zero, one);
                info.Current.GTPredictedTimeSinceLastRenderFrame = (float)GetTimeSinceLastRenderFrameStart();
                info.PausedCurrent = info.Current;
                info.BulletTimeCurrent = info.Current;
                CBulletTimeManager* bulletTime = PWorld->PBulletTimeManager;
                if (bulletTime->IsBulletTimeActive()) {
                    info.BulletTimeCurrent.WFInterpolate = bulletTime->ConvertInterpolateToBulletTimeInterpolate(info.Current.WFInterpolate);
                    info.BulletTimeLast.WFInterpolate = PWorld->GetBulletTimeManager()->ConvertInterpolateToBulletTimeInterpolate(info.Last.WFInterpolate);
                }
            } else {
                double one = 1.0;
                double zero = 0.0;
                info.Last = LastInterpolationInfo.Last;
                info.Current = LastInterpolationInfo.Current;
                info.PausedLast = LastInterpolationInfo.PausedCurrent;
                info.BulletTimeCurrent = LastInterpolationInfo.BulletTimeCurrent;
                info.WFServerCurrentTime = CurrentServerFrame;
                double interpolate = GetServerFrameInterpolate(GetGameTimeOfNextPresentCompletion());
                info.PausedCurrent.WFInterpolate = (float)GFLimitReturn(interpolate, zero, one);
                info.PausedCurrent.GTPredictedTimeSinceLastRenderFrame = (float)GetTimeSinceLastRenderFrameStart();
            }
            LastInterpolationInfo = info;
            PWorld->RenderUpdate(&info);
            if (!Client.IsReadyToRender())
                ++DebugNoFramesUnableToRender;
            if (g_b13B8688 || Client.IsReadyToRender()) {
                double start = GetCurrentGameTime();
                LastRenderFrameStartTime = GetCurrentGameTime();
                PDisplayEngine->DoRender(&info, WorldUpdateTurn, true);
                double end = GetCurrentGameTime();
                LastFrameRenderDuration = end - start;
                ++NoRenderFramesSinceLastGameUpdate;
                DebugNoFramesUnableToRender = 0;
                Rendered = true;
            }
        }
    }
    g_l13B7D6C = PDisplayEngine->f68;
}