// CTCTavernGameCoinGolf::QuitMidGame() -- quitting mid-game: the game is finished and not won, then SetState(ETGS_END_ROUND)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D29F0, oracle length 16 bytes.
// Leaf: ?QuitMidGame@CTCTavernGameCoinGolf@@UAEXXZ
// Retail bytes:  push 6 ; mov byte ptr [ecx + 0x28], 1 ; mov byte ptr [ecx + 0x21], 0 ; call 0x5e9b20 ; ret
// Naming (NF159, 2026-11-20, q4271): ported onto engine/CTCTavernGameCoinGolf.h (retail branch) from a local stand-in
//   `CTCTavernGameCoinGolf::Begin(long)` that bound CTCTavernGame::SetState 0x005E9B20 as ?Begin@CTCTavernGameCoinGolf@@QAEXJ@Z; now the PDB
//   ?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z (Ego_r.pdb 0x00647550 / FableWin.pdb 0x022D5AD0). The two flag bytes are
//   CTCTavernGame::GameFinished (+0x28) and GameWon (+0x21); 6 is ETGS_END_ROUND. Own leaf: the virtual override (vtable slot 34; Ego_r.pdb
//   0x007CFB70 / FableWin.pdb 0x02743EF0 ?QuitMidGame@CTCTavernGameCoinGolf@@UAEXXZ; was QAE).
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CTCTavernGameCoinGolf.h"

void CTCTavernGameCoinGolf::QuitMidGame()
{
    GameFinished = true;
    GameWon = false;
    SetState(ETGS_END_ROUND);
}
