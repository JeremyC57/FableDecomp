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
#include "engine/CNetworkClient.h"
#include "engine/CWorld.h"
#include "engine/CDisplayEngine.h"
class CGameEvent;
struct CWorld_Methods : CWorld {
    bool EventIsWorldEvent(const CGameEvent* event) const;           // 0x0049D8C0
    void ReceiveLocalEvent(const CGameEvent* event);                 // 0x0049E1D0
};
struct CDisplayEngine_Methods : CDisplayEngine {
    void ReceiveFeedbackGameEvent(const CGameEvent* event);          // 0x00434A30
};
struct CNetworkClient_Methods : CNetworkClient {
    void SendGameEvent(const CGameEvent* event);                     // 0x004AE9A0
};
class CMainGameComponent { public:
    unsigned char             _pad00[0x24];
    CWorld_Methods*           PWorld;                    // +0x24 scoped_ptr<CWorld>
    CDisplayEngine_Methods*   PDisplayEngine;            // +0x28 scoped_ptr<CDisplayEngine>
    unsigned char             _pad2c[0x13ab8 - 0x2c];
    CNetworkClient_Methods    Client;                    // +0x13ab8
    void AddGameEvent(const CGameEvent& event);
protected:
    bool EventIsSystemEvent(const CGameEvent* event);    // 0x00415FF2
    friend struct CMainGameComponent_Test;               // behaviour-test hook
};
void CMainGameComponent::AddGameEvent(const CGameEvent& event)
{
    if (PWorld->EventIsWorldEvent(&event) || EventIsSystemEvent(&event))
        Client.SendGameEvent(&event);
    PWorld->ReceiveLocalEvent(&event);
    PDisplayEngine->ReceiveFeedbackGameEvent(&event);
}