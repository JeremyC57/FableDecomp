// CMainGameComponent::CMainGameComponent(CMainGameComponentParent*, const CMainGameComponentInit&) -- main game component ctor: game-component base, empty banks/scoped pointers, force-update tuning (3.0x at 30 fps), the event/network/file/frame-rate members, a copy of the init structure, then registration in the component list
// Leaf: ??0CMainGameComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00418DCA, 294 bytes (`ret 8`). Vtables 0x0122F180 (+0) / 0x0122F174 (+4).
// Callees: CGameComponent::CGameComponent 0x0051D8E0 (labelled CAIStateGroup_GazeAtHome), CInterpolationInfo 0x00415A60,
//   CGameEventPackageSet 0x009F1A80, CNetworkClient 0x00419323 (labelled CAIStateGroup_GoToBed), CDiskFileWin32 0x00415C30,
//   CFrameRateSmoother 0x00419747 (labelled CAIStateGroup_WhisperPotionCombat), CRoughFrameCounter 0x009F8B10 (labelled
//   CBaseIntelligentPointer), CMainGameComponentInit copy ctor 0x00415E17, the component list (0x013B8390) AddComponent
//   0x009C0190 with the +4 interface subobject.
// Layout = Ego_r.pdb CMainGameComponent (90608 bytes; retail offsets equal the PDB's for every member stored here).
// Shape: size-optimised TU (`#pragma optimize("s", on)`, no frame pointer): the big-offset counted pointers are built
//   through `lea eax` + [eax]/[eax+4], `fldz` for the 0.0 double, `push [esp+N]` for the forwarded arguments; the opaque
//   member bodies are long arrays so the 1-byte bools before RoughFPSCounter keep its 4-byte alignment. The stores after
//   InitStructure (LastGameTurnForceRendered, DebugNoFramesUnableToRender) are body statements around the registration.
#pragma optimize("s", on)
struct CASoundBank;
struct CPlayerManager;
struct CGamePlayerInterface;
struct CWorld;
struct CDisplayEngine;
struct CMainGameComponentParent;
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
template <class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) { }
};
namespace NGameText { struct CDataBank; }
struct CLUA;
struct CFontBank;
struct CGraphicDataBank;
struct CInterpolationInfo { long _body[0x34 / 4]; CInterpolationInfo(); };          // 0x00415A60
struct CGameEventPackageSet { long _body[0x13a14 / 4]; CGameEventPackageSet(); };   // 0x009F1A80
struct CNetworkClient { long _body[0x2680 / 4]; CNetworkClient(); };                // 0x00419323
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
    CMainGameComponent(CMainGameComponentParent* parent, const CMainGameComponentInit& init);
    virtual ~CMainGameComponent();
    virtual void v00();
};

CMainGameComponent::CMainGameComponent(CMainGameComponentParent* parent, const CMainGameComponentInit& init)
    : CGameComponent(parent),
      ForceUpdateTick(false), ForceUpdateTickSpeedMultipler(3.0f), ForceUpdateTickSpeedDesiredFramerate(30.0f),
      ForceUpdateNoFailedUpdates(0), FirstWorldFrameUpdate(true), CurrentServerFrame(0), InputServerFrame(0),
      NoRenderFramesSinceLastGameUpdate(0), WorldSeed(0), LocalSeed(0),
      LoadingEventPackages(false), SavingEventPackages(false),
      LastRenderFrameStartTime(0.0), WorldUpdateTurn(false), NextComponentToRun(0),
      InitStructure(init), Initialised(false), AllowRender(true), Rendered(false)
{
    LastGameTurnForceRendered = 0;
    g_pComponentList->AddComponent(this);
    DebugNoFramesUnableToRender = 0;
}
