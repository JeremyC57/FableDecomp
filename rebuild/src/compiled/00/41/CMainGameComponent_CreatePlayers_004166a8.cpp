// CMainGameComponent::CreatePlayers() -- create the players, then bring the network client up as a local game (uninitialise it again if that fails half way)
// Leaf: ?CreatePlayers@CMainGameComponent@@IAEXXZ
// Retail: Fable.exe @ 0x004166A8, 58 bytes, `ret`; one caller (0x00418834); Ego_r.pdb ?CreatePlayers@CMainGameComponent@@IAEXXZ.
// Body: CGameDefinitionManager::Get() (0x0044C6B0, static __fastcall, result unused); PPlayerManager->CreatePlayers()
// (0x0044A530); Client.InitialiseAsLocal(this) (0x004AE940); on failure, if Client.IsValid() (CInitBaseClass, 0x0099A330:
// `mov al,[ecx+4]`) tail-jump to Client.Uninitialise() (0x004AE990).
// Layout: PDB CMainGameComponent: scoped_ptr<CPlayerManager> PPlayerManager +0x1c, CNetworkClient Client +0x13ab8 (80568);
// no engine/CMainGameComponent.h exists. CNetworkClient from engine/CNetworkClient.h (CInitBaseClass base: vptr +0, valid flag +4).
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
#include "engine/CNetworkClient.h"
#include "engine/CPlayerManager.h"
class CGameDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ
struct CPlayerManager_Methods : CPlayerManager {
    void CreatePlayers();                                // 0x0044A530
};
struct CNetworkClient_Methods : CNetworkClient {
    bool InitialiseAsLocal(CMainGameComponent* component);  // 0x004AE940
    bool IsValid() const;                                // 0x0099A330 (CInitBaseClass)
    void Uninitialise();                                 // 0x004AE990
};
class CMainGameComponent { public:
    unsigned char            _pad00[0x1c];
    CPlayerManager_Methods*  PPlayerManager;             // +0x1c scoped_ptr<CPlayerManager>
    unsigned char            _pad20[0x13ab8 - 0x20];
    CNetworkClient_Methods   Client;                     // +0x13ab8
protected:
    void CreatePlayers();
    friend struct CMainGameComponent_Test;               // behaviour-test hook
};
void CMainGameComponent::CreatePlayers()
{
    const CGameDefinitionManager& definitions = CGameDefinitionManager::Get();
    PPlayerManager->CreatePlayers();
    if (!Client.InitialiseAsLocal(this) && Client.IsValid())
        Client.Uninitialise();
}