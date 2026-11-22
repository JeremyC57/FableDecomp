// CMainGameComponent::GetInputs() -- fold the current game camera's render positions (t = 0 and t = 1) into an input checksum, record it with the world checksum and world seed in the input manager and update the input manager; while the application is active (and the world frame is past 1), update the player interface and poll its game events, queueing them as game events -- or, while loading event packages, only processing system events
// Leaf: ?GetInputs@CMainGameComponent@@MAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00416E78, length 393 bytes; size-optimised frame-pointer TU
//   (`#pragma optimize("s", on)` + `optimize("y", off)`), /GS cookie 0x0139C8A8 / __security_check_cookie @ 0x00BFE9F9.
// Name: Ego_r.pdb 0x00416188 / FableWin.pdb 0x018A6CB0 ?GetInputs@CMainGameComponent@@MAEXXZ (protected virtual); retail vtable 0x0122F180
//   slot 6 (both vtable emitters 0x00418DCA / 0x00418B7A bind this spelling).
// Callees (Ego_r.pdb / FableWin.pdb spellings): PWorld (+0x24, boost::scoped_ptr<CWorld>) -> CWorld::GetGameCamera (inline,
//   PCurrentGameCamera +0x34) -> CAIGameCameraBase::GetRenderCamera(float) const (slot 1, CCamera by hidden pointer), __ftol2 @ 0x00BFEA70,
//   PeekWorldChecksum @ 0x00416392, PeekWorldSeed (inline, WorldSeed +0x1613C), CInputManager::UpdateChecksum(long, const CChecksum&)
//   @ 0x009F4A90 and CInputManager::Update (slot 2) on GPInputManager @ 0x013B8388, GFGetSystemManager @ 0x009A4EC0 /
//   CSystemManager::IsApplicationActive @ 0x009A57B0, CWorld::GetFrame @ 0x0049D870, PPlayerInterface (+0x20) ->
//   CGamePlayerInterface::Update @ 0x004457F0 and GetGameEvent (slot 1), AddGameEvent @ 0x0041649C, EventIsSystemEvent @ 0x00415FF2,
//   ProcessEvent @ 0x00415FE0 (both protected).
// Layout: engine/CMainGameComponent.h (CurrentServerFrame +0x48, WorldSeed +0x1613C, LoadingEventPackages +0x16170); CGameEvent's inline
//   default constructor (Player -1, Valid false; FableWin 0x018A13F0); CChecksum's inline three-long constructor (FableWin ??0CChecksum@@QAE@JJJ@Z).
// Shape: six `sum = (long)(sum + camera position component)` statements, each fetching the camera again; the 56-byte CCamera temporary
//   is retail's 0x38-byte frame block at [ebp-78h].
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTISSYSTEMEVENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PROCESSEVENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PPLAYERINTERFACE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTPACKAGES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CNETWORKCLIENT_UPDATE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_APPACTIVE
#define FABLE_RETAIL_CINPUTMANAGER_UPDATECHECKSUM
#define FABLE_RETAIL_CCHECKSUM_CTOR_THREE
#define FABLE_RETAIL_CGAMEEVENT_DEFAULT_CONSTRUCTOR_PASS26
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDISKFILEWIN32
#include "engine/CChecksum.h"
#include "engine/CInputManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameEvent.h"
#include "engine/CCamera.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CWorld.h"
#include "engine/CSystemManager.h"
#include "engine/CGamePlayerInterface.h"
#pragma optimize("s", on)
#pragma optimize("y", off)

extern CInputManager* GPInputManager;                   // 0x013B8388 ?GPInputManager@@3PAVCInputManager@@A (Ego_r.pdb / FableWin.pdb)

void CMainGameComponent::GetInputs()
{
    CGameEvent event;
    long inputChecksum = (long)PWorld->GetGameCamera()->GetRenderCamera(0.0f).PeekWorldPos().X;
    inputChecksum = (long)(inputChecksum + PWorld->GetGameCamera()->GetRenderCamera(0.0f).PeekWorldPos().Y);
    inputChecksum = (long)(inputChecksum + PWorld->GetGameCamera()->GetRenderCamera(0.0f).PeekWorldPos().Z);
    inputChecksum = (long)(inputChecksum + PWorld->GetGameCamera()->GetRenderCamera(1.0f).PeekWorldPos().X);
    inputChecksum = (long)(inputChecksum + PWorld->GetGameCamera()->GetRenderCamera(1.0f).PeekWorldPos().Y);
    inputChecksum = (long)(inputChecksum + PWorld->GetGameCamera()->GetRenderCamera(1.0f).PeekWorldPos().Z);
    CChecksum checksum(PeekWorldChecksum(), PeekWorldSeed(), inputChecksum);
    GPInputManager->UpdateChecksum(CurrentServerFrame, checksum);
    GPInputManager->Update();
    if (GFGetSystemManager()->IsApplicationActive() && CWorld::GetFrame() > 1) {
        PPlayerInterface->Update();
        while (PPlayerInterface->GetGameEvent(event)) {
            if (!LoadingEventPackages)
                AddGameEvent(event);
            else if (EventIsSystemEvent(&event))
                ProcessEvent(&event);
        }
    }
}
