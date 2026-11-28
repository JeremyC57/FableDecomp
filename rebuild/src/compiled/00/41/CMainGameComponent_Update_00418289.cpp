// CMainGameComponent::Update() -- per-frame game update: force double precision / round-to-nearest, optional debug sleep, pause the game (and show the error) when the controller is disconnected, refresh the update timing, then advance the world from the saved or the client's event packages
// Leaf: ?Update@CMainGameComponent@@MAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00418289, 564 bytes, thiscall, frame pointer (`leave ; ret`), /GS cookie 0x0139C8A8 checked by
//   __security_check_cookie 0x00BFE9F9 (the 0x28-byte CGameEvent sits under the cookie). Size TU: `optimize("s", on)` + `("y", off)`
//   applies to Update. The file default /O2 /Oy gives the separately emitted 004159B0 and 004159E0 their retail bodies.
// Identity: Ego_r.pdb (0x00416DA7) / FableWin.pdb (0x018A7182) ?Update@CMainGameComponent@@MAEXXZ (protected virtual): slot 5 of
//   vtable 0x0122F180 (engine/CMainGameComponent.h EDITOR comment); the generated catalog label carried the mangled name only.
// Retail-asm: 0x0041829C `fnstcw word ptr [ebp-34h]` ... 0x004182AF `fldcw word ptr [ebp-34h]` (GFFPUSetDoublePrecision, new shared
//   helper engine/GFFPUSetDoublePrecision.h) and 0x004182B2 `fnstcw word ptr [ebp-34h]` + 0x004182B5 `and byte ptr [ebp-33h],0F3h` +
//   0x004182B9 `fldcw word ptr [ebp-34h]` (engine/GFFPUSetRoundingNearest.h, which cites these addresses).
// Callees (PDB spellings): CMainGameComponent::IsControllerDisconnected 0x00416296, SignalFrameUpdate 0x00416047 (Ego_r order + size),
//   IsTimeForServerUpdate 0x0041674A, GetEventPackageSetFromSave 0x00416148, AddEventPackageSetToSave 0x004161A7,
//   UpdateFromEventPackageSet 0x0041726D, GetFrameDifferenceFromCurrent 0x004167A7, AddGameEvent 0x0041649C, GetInputs (slot 6, +0x18);
//   CSubtitleRenderer::IsInInteractiveState 0x00490A22 on CPlayerGui::Get().GetSubtitleRenderer() (singleton 0x013B8790 + 0x18);
//   NUISystem::CManager::GetInstance 0x0041E5F2 / HasErrorMessage (inline +0x9C) / ClearErrorMessage 0x0041CE6A; the global
//   ShowErrorMessage(ERRORMESSAGE_TYPE) 0x0041E61B (Ego_r.pdb ?ShowErrorMessage@@YIXW4ERRORMESSAGE_TYPE@@@Z 0x00438C49);
//   CWorld::IsPaused (inline +0xA4) / UpdateInputRelatedInterpolation 0x0049D9E0 / GetFrame 0x0049D870; CNetworkClient::IsLocal (inline
//   LocalGame +0x2662) / GetGameEventPackageSet 0x004AEBA0; CInputManager::IsLoadingEvents / IsSavingEvents (inline +0x38 / +0x39);
//   CGameEvent(long) / AddUBYTE (inline) -> AddData 0x009F1760; GFGetTime 0x009E1BC0; Sleep (IAT 0x0143FE1C).
// Globals: GPInputManager 0x013B8388; 0x013B8610 (debug sleep per frame), 0x013B86AC (set when this function paused the game),
//   0x013B8630 and 0x013B7D70 (landed spellings GForceRenderFramesPerSec / GWorldFrame): typed FableWin globals, confirmed by the release stores and references.
// Shape: the subtitle renderer reference is taken before IsControllerDisconnected (retail loads the singleton into edi first,
//   0x004182CE); the event type 9 is the pause-game event (enumerator name not in the extracts); the update-time stores use the
//   implicit double -> float conversions: explicit (float) casts keep `fstp [edi]` ahead of the LoadingEventPackages test, retail
//   schedules the test first (0x004183C3).
// Headers (overlay hdr/, new opt-ins documented in hdr/README.md): CMainGameComponent.h _UPDATE, CNetworkClient.h _GAMEEVENTPACKAGESET,
//   CWorld.h _UPDATEINPUT, CInputManager.h _EVENTFLAGS, CSubtitleRenderer.h _INTERACTIVE, NUISystem_CManager.h _ERRORMESSAGE,
//   GFFPUSetDoublePrecision.h (new shared retail-asm helper with a FABLE_PORTABLE twin).
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTPACKAGES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_UPDATE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GAMETIME
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CNETWORKCLIENT_UPDATE
#define FABLE_RETAIL_CNETWORKCLIENT_GAMEEVENTPACKAGESET
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CINPUTMANAGER_EVENTFLAGS
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER_INTERACTIVE
#define FABLE_RETAIL_CMANAGER_ERRORMESSAGE
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CGameEvent.h"
#include "engine/CInputManager.h"
#include "engine/CPlayerGui.h"
#include "engine/NUISystem_CManager.h"
#include "engine/GFFPUSetDoublePrecision.h"
#include "engine/GFFPUSetRoundingNearest.h"
#include "engine/GFGetTime.h"
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);   // IAT 0x0143FE1C
extern CInputManager* GPInputManager;                   // 0x013B8388 ?GPInputManager@@3PAVCInputManager@@A
extern long GDelayEachFrameMS; // FableWin S_GDATA32 + S_PUB32; the same named symbol is public in Ego_r.
extern long GForceRenderFramesPerSec;                     // 0x013B8630 (landed spelling, CMainGameComponent::Render 0x00417001)
extern unsigned long GWorldFrame;         // 0x013B7D70 (landed spelling)
#pragma optimize("s", on)
#pragma optimize("y", off)

void CMainGameComponent::Update()
{
    GFFPUSetDoublePrecision();
    GFFPUSetRoundingNearest();
    if (GDelayEachFrameMS > 0)
        Sleep(GDelayEachFrameMS);
    CSubtitleRenderer& subtitle = CPlayerGui::Get().GetSubtitleRenderer();
    // Ego_d and FableWin both place BRequireUnpause in the function scope as S_LDATA32 bool.
    static bool BRequireUnpause;
    if (IsControllerDisconnected() && subtitle.IsInInteractiveState()) {
        if (!NUISystem::CManager::GetInstance()->HasErrorMessage()) {
            if (!PWorld->IsPaused()) {
                CGameEvent game_event(9);
                game_event.AddUBYTE(true);
                AddGameEvent(game_event);
                BRequireUnpause = true;
            } else
                BRequireUnpause = false;
            ShowErrorMessage((ERRORMESSAGE_TYPE)0);
        }
    } else if (NUISystem::CManager::GetInstance()->HasErrorMessage()) {
        NUISystem::CManager::GetInstance()->ClearErrorMessage();
        if (BRequireUnpause) {
            CGameEvent game_event(9);
            game_event.AddUBYTE(false);
            AddGameEvent(game_event);
            BRequireUnpause = false;
        }
    }
    TimePassedSinceLastUpdate = GFGetTime() - LastUpdateTime;
    LastUpdateTime = GFGetTime();
    WorldUpdateTurn = false;
    if (LoadingEventPackages) {
        if (IsTimeForServerUpdate(CurrentServerFrame)) {
            if (GetEventPackageSetFromSave(&EventPackageSet) == true) {
                UpdateFromEventPackageSet(&EventPackageSet);
                NoRenderFramesSinceLastGameUpdate = 0;
                WorldUpdateTurn = true;
            }
            SignalFrameUpdate();
        }
    } else {
        if (Client.IsLocal() && CurrentServerFrame > 1 && !GPInputManager->IsLoadingEvents() && !GPInputManager->IsSavingEvents()) {
            long frame_diff = GetFrameDifferenceFromCurrent(CurrentServerFrame);
            if (GForceRenderFramesPerSec == 0 && frame_diff > 15) {
                CurrentServerFrame += frame_diff;
                AllowRender = false;
            }
        }
        if (Client.GetGameEventPackageSet(&EventPackageSet)) {
            PWorld->UpdateInputRelatedInterpolation();
            GetInputs();
            SignalFrameUpdate();
            if (SavingEventPackages)
                AddEventPackageSetToSave(&EventPackageSet);
            UpdateFromEventPackageSet(&EventPackageSet);
            NoRenderFramesSinceLastGameUpdate = 0;
            WorldUpdateTurn = true;
        }
    }
    GWorldFrame = CWorld::GetFrame();
}