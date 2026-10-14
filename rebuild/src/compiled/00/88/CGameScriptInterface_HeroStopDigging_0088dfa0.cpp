// CGameScriptInterface::HeroStopDigging() -- removes the digging player mode (0x1e) from the main player when it is the current mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088DFA0, length 48 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (`GetPlayer(MainPlayer)`), CPlayer::GetCurrentMode @ 0x00633BE0,
// CPlayer::RemoveMode(EPlayerMode) @ 0x00634190.
// Layout from engine/CGameScriptInterface.h: PlayerManager +0x14.
// Shape: the mode is a named local (`push eax` re-uses the compared register instead of an immediate 1Eh); the player lives
// in esi across the two calls.
#include "engine/CGameScriptInterface.h"

struct CPlayer {
    long GetCurrentMode();                          // 0x00633BE0 EPlayerMode
    void RemoveMode(long mode);                     // 0x00634190
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                       // 0x00449970
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void HeroStopDigging();
};

void CGameScriptInterface_Methods::HeroStopDigging()
{
    CPlayer* player = PlayerManager->GetMainPlayer();
    long mode = player->GetCurrentMode();
    if (mode == 0x1e)
        player->RemoveMode(mode);
}