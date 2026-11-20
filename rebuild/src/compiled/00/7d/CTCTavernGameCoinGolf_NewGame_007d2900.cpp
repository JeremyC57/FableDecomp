// CTCTavernGameCoinGolf::NewGame() -- the coin-base NewGame, then SetState(ETGS_NEW_ROUND)
// Leaf: ?NewGame@CTCTavernGameCoinGolf@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D2900: push esi; mov esi,ecx; call 0x008EF170; push 7; mov ecx,esi; call 0x005E9B20; pop esi; ret
//   (vtable 0x0126F49C slot 31, rtti-vtables.tsv).
// Naming (NF159, 2026-11-20, q4271): ported onto engine/CTCTavernGameCoinGolf.h (retail branch) from the free-form stand-in
//   FableEndCut_007d2900 (h1 / h2): the base call is CTCTavernGameCoinBase::NewGame 0x008EF170 (Ego_r.pdb 0x00914AB0 / FableWin.pdb 0x02AEC5E0
//   ?NewGame@CTCTavernGameCoinBase@@UAEXXZ, non-virtual qualified call; the spelling the landed 0x007CF4A0 binds), the second
//   CTCTavernGame::SetState 0x005E9B20 (?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z, Ego_r.pdb 0x00647550 / FableWin.pdb 0x022D5AD0);
//   7 is ETGS_NEW_ROUND. Own leaf: the virtual override (Ego_r.pdb 0x007CFB50 / FableWin.pdb 0x02743120
//   ?NewGame@CTCTavernGameCoinGolf@@UAEXXZ); was ?NewGame@FableEndCut_007d2900@@QAEXXZ.
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CTCTavernGameCoinGolf.h"

void CTCTavernGameCoinGolf::NewGame()
{
    CTCTavernGameCoinBase::NewGame();
    SetState(ETGS_NEW_ROUND);
}
