// CTCTavernGameShoveHaPenny::QuitMidGame() -- quitting mid-game: the game is finished and not won, then SetState(ETGS_END_ROUND)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CF520, oracle length 16 bytes.
// Leaf: ?QuitMidGame@CTCTavernGameShoveHaPenny@@UAEXXZ
// Retail bytes:  push 6 ; mov byte ptr [ecx + 0x28], 1 ; mov byte ptr [ecx + 0x21], 0 ; call 0x5e9b20 ; ret
// Naming (NF159, 2026-11-20, q4271): ported onto engine/CTCTavernGameShoveHaPenny.h (retail branch) from a local stand-in
//   `CTCTavernGameShoveHaPenny::Begin(long)` that bound CTCTavernGame::SetState 0x005E9B20 as ?Begin@CTCTavernGameShoveHaPenny@@QAEXJ@Z;
//   now the PDB ?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z (Ego_r.pdb 0x00647550 / FableWin.pdb 0x022D5AD0). The two flag
//   bytes are CTCTavernGame::GameFinished (+0x28) and GameWon (+0x21); 6 is ETGS_END_ROUND. Own leaf: the virtual override (vtable
//   0x0126F2F4 slot 34; Ego_r.pdb 0x007CDA80 / FableWin.pdb 0x0273B600 ?QuitMidGame@CTCTavernGameShoveHaPenny@@UAEXXZ; was QAE).
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CTCTavernGameShoveHaPenny.h"

void CTCTavernGameShoveHaPenny::QuitMidGame()
{
    GameFinished = true;
    GameWon = false;
    SetState(ETGS_END_ROUND);
}
