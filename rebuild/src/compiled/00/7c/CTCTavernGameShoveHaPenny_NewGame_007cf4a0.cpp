// CTCTavernGameShoveHaPenny::NewGame() -- coin-base NewGame, then clears the score and the rounds played and enters ETGS_NEW_ROUND
// Leaf: ?NewGame@CTCTavernGameShoveHaPenny@@UAEXXZ
// Retail: Fable.exe @ 0x007CF4A0, 30 bytes (vtable 0x0126F2F4 slot 31 = NewGame).
// Callees: CTCTavernGameCoinBase::NewGame 0x008EF170 (base slot 31, non-virtual call),
//          CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20.
// Shape: the float `0.0f` and the integer zero share one `xor eax,eax` register (VC7.1 stores a float zero through an integer register
//        when another zero store needs it).
// Naming (NF159, 2026-11-20, q4271): ported onto engine/CTCTavernGameShoveHaPenny.h (retail branch) from the local CTCTavernGameCoinBase /
//   ShoveHaPenny stand-ins: SetState is CTCTavernGame's (?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z, Ego_r.pdb 0x00647550 /
//   FableWin.pdb 0x022D5AD0; was ?SetState@CTCTavernGameCoinBase@@QAEXJ@Z); +0x2C is CTCTavernGame::Score, +0x148 RoundsPlayed (was
//   m_numShoves); 7 is ETGS_NEW_ROUND.
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CTCTavernGameShoveHaPenny.h"

void CTCTavernGameShoveHaPenny::NewGame()
{
    CTCTavernGameCoinBase::NewGame();
    Score = 0.0f;
    RoundsPlayed = 0;
    SetState(ETGS_NEW_ROUND);
}
