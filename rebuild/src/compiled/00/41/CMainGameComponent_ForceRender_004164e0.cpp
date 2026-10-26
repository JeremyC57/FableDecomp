// CMainGameComponent::ForceRender(long) -- render interpolated frames until the server frame is caught up
#pragma optimize("s", on)
class CASoundBank;
class CPlayerManager;
class CGamePlayerInterface;
struct CMainGameComponentParent;
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
template <class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) { }
    T* operator->() const { return ptr; }
};
namespace NGameText { class CDataBank; }
class CLUA;
class CFontBank;
class CGraphicDataBank;
struct CInterpolationInfoSet {
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};
class CInterpolationInfo { public:                        // 0x34 bytes
    CInterpolationInfoSet Current;                 // +0x00
    CInterpolationInfoSet Last;                    // +0x08
    CInterpolationInfoSet PausedCurrent;           // +0x10
    CInterpolationInfoSet PausedLast;              // +0x18
    CInterpolationInfoSet BulletTimeCurrent;       // +0x20
    CInterpolationInfoSet BulletTimeLast;          // +0x28
    long                  WFServerCurrentTime;     // +0x30
    CInterpolationInfo();                          // 0x00415A60
};
class CWorld { public: void RenderUpdate(CInterpolationInfo* info); };                          // 0x0049E080
class CDisplayEngine { public: void DoRender(CInterpolationInfo* info, bool worldUpdateTurn, bool b); };   // 0x00435F70
class CGameEventPackageSet { public: long _body[0x13a14 / 4]; CGameEventPackageSet(); };   // 0x009F1A80
struct CNetworkClient { long _body[0x2680 / 4]; CNetworkClient(); };                // 0x00419323
class CDiskFileWin32 { public: long _body[0x1c / 4]; CDiskFileWin32(); };                  // 0x00415C30
struct CEventPackageFileHeader { long Version; };
struct CFrameRateSmoother { long _body[0x30 / 4]; CFrameRateSmoother(); };          // 0x00419747
struct CRoughFrameCounter { long _body[0x8 / 4]; CRoughFrameCounter(); };           // 0x009F8B10
class CMainGameComponentInit { public:
    long _body[0x10 / 4];
    CMainGameComponentInit(const CMainGameComponentInit& other);                          // 0x00415E17
};
class CBaseClass { public: virtual ~CBaseClass(); };
struct CComponentListEntry { virtual void v00(); };                                      // the +4 interface
class CGameComponent : public CBaseClass, public CComponentListEntry { public:
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
    void ForceRender(long frames);
    virtual ~CMainGameComponent();
    virtual void v00();
};

void CMainGameComponent::ForceRender(long frames)
{
    long steps = (long)(frames * (1.0 / 15.0));
    float step = 1.0f / frames;
    if (LastGameTurnForceRendered < CurrentServerFrame) {
        for (long i = 0; i < steps; ++i) {
            CInterpolationInfo info;
            info.Current.WFInterpolate = (float)i / (float)steps;
            info.Last = LastInterpolationInfo.Current;
            info.PausedLast = LastInterpolationInfo.PausedCurrent;
            info.WFServerCurrentTime = CurrentServerFrame;
            info.Current.GTPredictedTimeSinceLastRenderFrame = (float)(i + 1) * step;
            info.PausedCurrent = info.Current;
            LastInterpolationInfo = info;
            PWorld->RenderUpdate(&info);
            double start = GetCurrentGameTime();
            LastRenderFrameStartTime = GetCurrentGameTime();
            PDisplayEngine->DoRender(&info, WorldUpdateTurn, true);
            double end = GetCurrentGameTime();
            LastFrameRenderDuration = end - start;
            ++NoRenderFramesSinceLastGameUpdate;
            LastGameTurnForceRendered = CurrentServerFrame;
        }
    }
}