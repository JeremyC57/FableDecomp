// CWorld::EACloseLiveGUI(const CGameEvent* event) -- game-event action: unpause the world, take the event player out of the live-GUI mode (0x2B) and close the hero information screens
// Leaf: ?EACloseLiveGUI@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x00629DA0, 57 bytes, thiscall `ret 4`.
// Identity (proven by source order): registered by the static initialiser 0x0121B9B0 as the handler of game event 66 in the
//   dispatch table 0x013B9288 (no direct caller). Retail keeps the Ego_r.pdb donor order of the CWorld EA handlers:
//   EAControlledCreatureUseAbility (0x00629C40), EACloseInGameMenu, EACloseLiveGUI, EAAnswerQuestion (0x00629DE0) -- the
//   two between are 0x00629D60 and this 0x00629DA0. The body fits: Ego_r.pdb CPlayer has Init/UninitModeLiveGUI, and the
//   hero information screens are the live GUI.
// Callees: CWorld::SetAsPaused(bool) 0x0049D8F0 (Ego_r.pdb ?SetAsPaused@CWorld@@IAEX_N@Z), CWorld::GetPlayer(unsigned long)
//   0x0049D850, CPlayer::RemoveMode(EPlayerMode) 0x00634190, CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0,
//   NHeroInformationScreens::CManager::Close() 0x0066E73D; global CPlayerGui pointer 0x013B8790.
// Shape: `this` kept in esi across the calls; the player fetched into a named local so the mode push follows the call.
// Layout: CWorld through engine/CWorld.h (Donor include); CPlayerGui / CManager are declaration-only partial views.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld

enum EPlayerMode { PLAYER_MODE_NULL = 0, PLAYER_MODE_LIVE_GUI = 0x2B };   // enumerator names are ours

class CThing {
public:
    void Kill(bool b);                                     // 0x004C9B80 (Ego_r.pdb ?Kill@CThing@@QAEX_N@Z)
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject { };
class CThingPlayerCreature : public CThingCreatureBase { };

class CGameEvent {                                         // 0x28 bytes (Ego_r.pdb = retail)
public:
    long          Type;                                    // +0x00
    signed char   Player;                                  // +0x04
    unsigned char Data[32];                                // +0x05
    unsigned char EndPos;                                  // +0x25
    bool          Valid;                                   // +0x26
    bool          Replacement;                             // +0x27
};

class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;  // 0x00487DC0
    void RemoveMode(EPlayerMode mode);                     // 0x00634190
};

namespace NHeroInformationScreens {
class CManager {
public:
    void Close();                                          // 0x0066E73D
};
}
class CPlayerGui {
public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                           // 0x013B8790

class CWorld : public CWorldDonor {
protected:
    CPlayer* GetPlayer(unsigned long number);              // 0x0049D850
    void SetAsPaused(bool paused);                         // 0x0049D8F0
public:
    void EACloseLiveGUI(const CGameEvent* event);
};

void CWorld::EACloseLiveGUI(const CGameEvent* event)
{
    SetAsPaused(false);
    CPlayer* player = GetPlayer(event->Player);
    player->RemoveMode(PLAYER_MODE_LIVE_GUI);
    g_pPlayerGui->GetPHeroInfoScreensManager()->Close();
}