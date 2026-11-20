// CTCTavernGameCardBase::QuitMidGame() -- quitting mid-game: the game is finished and not won, then SetState(ETGS_TURN_CARDS_BACK)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F09A0, oracle length 16 bytes.
// Leaf: ?QuitMidGame@CTCTavernGameCardBase@@UAEXXZ
// Retail bytes:  push 0x18 ; mov byte ptr [ecx + 0x28], 1 ; mov byte ptr [ecx + 0x21], 0 ; call 0x5e9b20 ; ret
// Naming (NF159, 2026-11-20, q4271): ported onto engine/CTCTavernGameCardBase.h (retail branch; NF159 opt-in
//   FABLE_RETAIL_CTCTAVERNGAMECARDBASE_QUITMIDGAME) from a local stand-in `CTCTavernGameCardBase::Begin(long)` that bound CTCTavernGame::SetState
//   0x005E9B20 as ?Begin@CTCTavernGameCardBase@@QAEXJ@Z; now the PDB ?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z (Ego_r.pdb 0x00647550 /
//   FableWin.pdb 0x022D5AD0). The two flag bytes are CTCTavernGame::GameFinished (+0x28) and GameWon (+0x21); 0x18 is ETGS_TURN_CARDS_BACK.
//   Own leaf: the virtual override (slot 34; Ego_r.pdb 0x00915E40 / FableWin.pdb 0x02AF0D80 ?QuitMidGame@CTCTavernGameCardBase@@UAEXXZ; was QAE).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_QUITMIDGAME
#include "engine/CTCTavernGameCardBase.h"

void CTCTavernGameCardBase::QuitMidGame()
{
    GameFinished = true;
    GameWon = false;
    SetState(ETGS_TURN_CARDS_BACK);
}
