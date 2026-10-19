// CMainGameComponent::~CMainGameComponent() -- conditionally shut down, then destroy owned game resources.
// Native00418b7a length193. RTTI: CGameComponent->CBaseClassNonCopyable->CBaseClass, secondary CDeviceResetCallback at4.
// Actual fable_game.h included as donor; full native vtable signatures below, no fabricated slots.
#pragma optimize("s", on)
#define CMainGameComponent CMainGameComponentDonor
#define CGameComponent CGameComponentDonor
#define CMainGameComponentInit CMainGameComponentInitDonor
#define CGame CGameDonor
#include "fable_game.h"
#undef CGame
#undef CMainGameComponentInit
#undef CGameComponent
#undef CMainGameComponent
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld
#define CPlayerManager CPlayerManagerDonor
#include "engine/CPlayerManager.h"
#undef CPlayerManager
#define CGamePlayerInterface CGamePlayerInterfaceDonor
#include "engine/CGamePlayerInterface.h"
#undef CGamePlayerInterface
#define CDisplayEngine CDisplayEngineDonor
#include "engine/CDisplayEngine.h"
#undef CDisplayEngine
#define CNetworkClient CNetworkClientDonor
#include "engine/CNetworkClient.h"
#undef CNetworkClient
#define CDiskFileWin32 CDiskFileWin32Donor
#include "engine/CDiskFileWin32.h"
#undef CDiskFileWin32
#define CFontBank CFontBankDonor
#include "engine/CFontBank.h"
#undef CFontBank
struct CGame;struct CFontBank;struct CLUA;struct CGraphicDataBank;struct CCPPointerInfo;
namespace NGameText {struct CDataBank;}
struct CBaseClass { CBaseClass(); virtual ~CBaseClass(); }; //0099a300 destructor
struct CBaseClassNonCopyable : CBaseClass { CBaseClassNonCopyable(); private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable&operator=(const CBaseClassNonCopyable&); };
struct CDeviceResetCallback {virtual void OnPreDeviceReset()=0;virtual bool OnPostDeviceReset()=0;};
struct CGameComponent : CBaseClassNonCopyable,CDeviceResetCallback {
 bool Quit,Running; CGame* Game;
 virtual ~CGameComponent(){}
 virtual void Init();
 virtual bool Run(CGameComponent**)=0;
 virtual void ChangeTextureColourDepth(long)=0;
 virtual void SetQuit();
};
struct CWorld { virtual ~CWorld(); };
struct CPlayerManager { virtual ~CPlayerManager(); };
struct CGamePlayerInterface { virtual ~CGamePlayerInterface(); };
struct CDisplayEngine : CBaseClassNonCopyable { virtual ~CDisplayEngine(); virtual bool InitEngine(); virtual void UninitEngine(); };
template<class T>struct scoped_ptr {
 T* ptr;
 scoped_ptr():ptr(0){}
 ~scoped_ptr(){delete ptr;}
};
template<class T>struct CCountedPointer {T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}~CCountedPointer();};
struct CNetworkClient : CNetworkClientDonor {~CNetworkClient();}; //00419352
struct CDiskFileWin32 : CDiskFileWin32Donor {~CDiskFileWin32();}; //00401630
struct CFrameRateSmoother {unsigned char SamplesAndTiming[0x38];~CFrameRateSmoother();}; //00419764
struct CMainGameComponentInit {CWideString InitialWorldName,InitialWorldHolySiteName;CCharString InitialQuestName;CWideString SaveGameName;~CMainGameComponentInit();}; //004131d0
struct CMainGameComponent : CGameComponent {
 void* PSampleBank;
 CCountedPointer<NGameText::CDataBank> PTextBank;
 scoped_ptr<CPlayerManager> PPlayerManager;
 scoped_ptr<CGamePlayerInterface> PPlayerInterface;
 scoped_ptr<CWorld> PWorld;
 scoped_ptr<CDisplayEngine> PDisplayEngine;
 CCountedPointer<CLUA> PLUA;
 unsigned char TimingAndPackageSet[0x13a84];
 CNetworkClient Client;
 unsigned long NoRenderFramesSinceLastGameUpdate,WorldSeed,LocalSeed;
 CCountedPointer<CFontBank> PDebugFont,PCutSceneMainFont;
 CDiskFileWin32 EventPackageFile;
 bool LoadingEventPackages,SavingEventPackages;
 unsigned long EventPackageFileHeader;
 CFrameRateSmoother FrameRateSmoother;
 double LastRenderFrameStartTime;
 float TimePassedSinceLastUpdate,LastUpdateTime;
 bool WorldUpdateTurn;
 CGameComponent* NextComponentToRun;
 CCountedPointer<CGraphicDataBank> PMainGraphicBank;
 CMainGameComponentInit InitStructure;
 bool Initialised,AllowRender,Rendered;
 long DebugNoFramesUnableToRender;
 CMainGameComponent(CGame&,const CMainGameComponentInit&);
 virtual ~CMainGameComponent();
 virtual void Init(); //slot1 004184bd
 virtual bool Run(CGameComponent**); //slot2 004189c2
 virtual void ChangeTextureColourDepth(long); //slot3 0041799b
 virtual void SetQuit(); //slot4 004197a0
 virtual void Update(); //slot5 00418289
 virtual void GetInputs(); //slot6 00416e78
 virtual void Render(); //slot7 00417001
 virtual void PostInit(); //slot8 00416953
 virtual bool IsEditorActive()const; //slot9 004197b0
 virtual void OnPreDeviceReset(); //secondary0 00415e83
 virtual bool OnPostDeviceReset(); //secondary1 004175da
 void Shutdown(); //004175e5
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CGameComponent)==16);
FABLE_STATIC_ASSERT(offsetof(CGameComponent,Quit)==8);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,Client)==0x13ab8);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PDebugFont)==0x16144);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,EventPackageFile)==0x16154);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,FrameRateSmoother)==0x16178);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PMainGraphicBank)==0x161c8);
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,InitStructure)==0x161d0);
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent)==0x161e8);
#endif
CMainGameComponent::~CMainGameComponent(){if(!Quit)Shutdown();}
