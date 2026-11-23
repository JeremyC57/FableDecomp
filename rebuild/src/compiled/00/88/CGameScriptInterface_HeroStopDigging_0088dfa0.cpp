// Leaf: ?HeroStopDigging@CGameScriptInterface@@UBEXXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h (class-model switch FABLE_RETAIL_NSCRIPT; was ?HeroStopDigging@CGameScriptInterface_Methods@@QAEXXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// CGameScriptInterface::HeroStopDigging() const -- removes the digging player mode (0x1e) from the main player when it is the current mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088DFA0, length 48 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (`GetPlayer(MainPlayer)`), CPlayer::GetCurrentMode @ 0x00633BE0,
// CPlayer::RemoveMode(EPlayerMode) @ 0x00634190.
// Layout from engine/CGameScriptInterface.h: PlayerManager +0x14.
// Shape: the mode is a named local (`push eax` re-uses the compared register instead of an immediate 1Eh); the player lives
// in esi across the two calls.
// Name fix (agentNF5 2026-11-10): CPlayer::RemoveMode 0x00634190 takes EPlayerMode (?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z, Ego_r.pdb / FableWin.pdb), was long.
enum EPlayerMode;                                  // MS forward enum (CPlayer::RemoveMode 0x00634190 takes it)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CPlayer { public:
    EPlayerMode GetCurrentMode() const;   /* ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ */                          // 0x00633BE0 EPlayerMode
    void RemoveMode(EPlayerMode mode);                     // 0x00634190
};

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

void CGameScriptInterface::HeroStopDigging() const
{
    CPlayer* player = &PlayerManager->GetMainPlayer();
    long mode = player->GetCurrentMode();
    if (mode == 0x1e)
        player->RemoveMode((EPlayerMode)(mode));
}