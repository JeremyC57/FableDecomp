// CMainGameComponent::CMainGameComponent(CGame&, const CMainGameComponentInit&) -- main game component ctor: game-component base, empty banks/scoped pointers, force-update tuning (3.0x at 30 fps), the event/network/file/frame-rate members, a copy of the init structure, then registration as a device-reset callback
// Leaf: ??0CMainGameComponent@@QAE@AAVCGame@@ABVCMainGameComponentInit@@@Z
// Names (NF233, 2026-11-22, q12638, in place): the self signature is the PDB one, ??0CMainGameComponent@@QAE@AAVCGame@@ABVCMainGameComponentInit@@@Z
//   (Ego_r.pdb 0x004172C9 + FableWin.pdb), which its binders 0x00412F90 / 0x0042EC7C / 0x00496CF0 already bind. The base ctor 0x0051D8E0 is
//   ??0CGameComponent@@QAE@AAVCGame@@@Z.
// Header port (NF275, 2026-11-22, q12659): the row-local CMainGameComponent / CGameComponent / CComponentListEntry / CComponentList / member-class
//   models are replaced by the shared headers. The class is engine/CMainGameComponent.h on engine/CGameComponent.h (CBaseClassNonCopyable +0,
//   CDeviceResetCallback +4), so the two vtables are the PDB pair ??_7CMainGameComponent@@6BCBaseClassNonCopyable@@@ (0x0122F180) /
//   ??_7CMainGameComponent@@6BCDeviceResetCallback@@@ (0x0122F174) (Ego_r / FableWin / Ego_d publics; was 6BCBaseClass@@@ /
//   6BCComponentListEntry@@@), with Init / SetQuit overrides at slots 1 / 4 (FABLE_RETAIL_CMAINGAMECOMPONENT_INIT_SETQUIT) and the class's
//   own slots 5..9 (FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR). The registration call 0x009C0190 is
//   ?AddDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z (Ego_r.pdb + FableWin.pdb; engine/CDisplayManager.h; the +4
//   CDeviceResetCallback subobject is the argument, `lea edi,[esi+4] ; push edi`), was the stand-in ?AddComponent@CComponentList@@...
//   The typed members come from FABLE_RETAIL_CMAINGAMECOMPONENT_CTOR plus the existing member opt-ins; the member ctors are declared
//   out of line as retail calls them.
// Plan phase 5 (2026-11-05), CCountedPointer wave: engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4).
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00418DCA, 294 bytes (`ret 8`). Vtables 0x0122F180 (+0) / 0x0122F174 (+4).
// Callees: CGameComponent::CGameComponent 0x0051D8E0, CInterpolationInfo 0x00415A60, CGameEventPackageSet 0x009F1A80, CNetworkClient
//   0x00419323, CDiskFileWin32 0x00415C30, CFrameRateSmoother 0x00419747, CRoughFrameCounter 0x009F8B10, CMainGameComponentInit copy ctor
//   0x00415E17, CDisplayManager::AddDeviceResetCallback 0x009C0190 (GPDisplayManager 0x013B8390).
// Shape: size-optimised TU (`#pragma optimize("s", on)`, no frame pointer): the big-offset counted pointers are built through `lea eax` +
//   [eax]/[eax+4], `fldz` for the 0.0 double, `push [esp+N]` for the forwarded arguments. The stores after InitStructure
//   (LastGameTurnForceRendered, DebugNoFramesUnableToRender) are body statements around the registration.
#pragma optimize("s", on)
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR_OUTOFLINE
#define FABLE_RETAIL_CDISKFILEWIN32_DTOR_DECL_ONLY
#define FABLE_RETAIL_CNETWORKCLIENT_UPDATE
#define FABLE_RETAIL_CNETWORKCLIENT_CTOR
#define FABLE_RETAIL_CGAMEEVENTPACKAGESET_CTOR
#define FABLE_RETAIL_CROUGHFRAMECOUNTER_CTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PPLAYERINTERFACE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTPACKAGES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_FORCEDTICK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_UPDATE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS
#define FABLE_RETAIL_CMAINGAMECOMPONENT_INITSTRUCTURE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_INIT_SETQUIT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_CTOR
#define FABLE_RETAIL_CDISPLAYMANAGER
#include "engine/CRoughFrameCounter.h"
#include "engine/CGameEventPackageSet.h"
#include "engine/CNetworkClient.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFrameRateSmoother.h"
#include "engine/CMainGameComponent.h"
#include "engine/Direct3D9Types.h"
#include "engine/Win32Types.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;           // 0x013B8390

CMainGameComponent::CMainGameComponent(CGame& game, const CMainGameComponentInit& init)
    : CGameComponent(game),
      ForceUpdateTick(false), ForceUpdateTickSpeedMultipler(3.0f), ForceUpdateTickSpeedDesiredFramerate(30.0f),
      ForceUpdateNoFailedUpdates(0), FirstWorldFrameUpdate(true), CurrentServerFrame(0), InputServerFrame(0),
      NoRenderFramesSinceLastGameUpdate(0), WorldSeed(0), LocalSeed(0),
      LoadingEventPackages(false), SavingEventPackages(false),
      LastRenderFrameStartTime(0.0), WorldUpdateTurn(false), NextComponentToRun(0),
      InitStructure(init), Initialised(false), AllowRender(true), Rendered(false)
{
    LastGameTurnForceRendered = 0;
    GPDisplayManager->AddDeviceResetCallback(this);
    DebugNoFramesUnableToRender = 0;
}
