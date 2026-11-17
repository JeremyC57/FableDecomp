// CMainGameComponent::AddGameEvent(const CGameEvent& event) -- send world/system events to the network client, then let the world and the display engine see it
// Leaf: ?AddGameEvent@CMainGameComponent@@QAEXABVCGameEvent@@@Z
// Retail: Fable.exe @ 0x0041649C, 68 bytes, `ret 4`; many callers (0x00416FD7, 0x00418336, 0x004183A0, ...); Ego_r.pdb
// ?AddGameEvent@CMainGameComponent@@QAEXABVCGameEvent@@@Z.
// Body: if (PWorld->EventIsWorldEvent(&event) (0x0049D8C0) || EventIsSystemEvent(&event) (0x00415FF2))
// Client.SendGameEvent(&event) (0x004AE9A0); PWorld->ReceiveLocalEvent(&event) (0x0049E1D0);
// PDisplayEngine->ReceiveFeedbackGameEvent(&event) (0x00434A30) -- Ego_r.pdb names; the catalog calls the last two
// CWorld::ProcessEvent.
// Layout: CMainGameComponent PWorld +0x24, PDisplayEngine +0x28 (scoped_ptr data), Client +0x13ab8 (as 0x004166A8);
// CNetworkClient from engine/CNetworkClient.h; CWorld / CDisplayEngine through engine/ headers with method shims.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
// NF56 (round 7, queues q10443 / q10444): moved onto the shared headers. CMainGameComponent.h (EVENTPACKAGES: scoped_ptr<CWorld> PWorld +0x24,
//   scoped_ptr<CDisplayEngine> PDisplayEngine +0x28, CNetworkClient Client +0x13AB8; ADDGAMEEVENT; overlay opt-in EVENTISSYSTEMEVENT), CWorld.h
//   (EventIsWorldEvent; overlay opt-in RECEIVELOCALEVENT), CDisplayEngine.h (overlay opt-in RECEIVEFEEDBACKGAMEEVENT) and CNetworkClient.h
//   (overlay opt-in SENDGAMEEVENT): the calls bind the Ego_r.pdb / FableWin.pdb spellings ?ReceiveLocalEvent@CWorld@@QAEXPBVCGameEvent@@@Z,
//   ?ReceiveFeedbackGameEvent@CDisplayEngine@@QAEXPBVCGameEvent@@@Z, ?EventIsWorldEvent@CWorld@@QBE_NPBVCGameEvent@@@Z and
//   ?SendGameEvent@CNetworkClient@@QAEXPBVCGameEvent@@@Z. Were the stand-ins CWorld_Methods / CDisplayEngine_Methods / CNetworkClient_Methods
//   and a local CMainGameComponent model.
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTPACKAGES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTISSYSTEMEVENT
#define FABLE_RETAIL_CNETWORKCLIENT_UPDATE
#define FABLE_RETAIL_CNETWORKCLIENT_SENDGAMEEVENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_RECEIVELOCALEVENT
#define FABLE_RETAIL_CDISPLAYENGINE_RECEIVEFEEDBACKGAMEEVENT
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CDisplayEngine.h"

void CMainGameComponent::AddGameEvent(const CGameEvent& event)
{
    if (PWorld->EventIsWorldEvent(&event) || EventIsSystemEvent(&event))
        Client.SendGameEvent(&event);
    PWorld->ReceiveLocalEvent(&event);
    PDisplayEngine->ReceiveFeedbackGameEvent(&event);
}