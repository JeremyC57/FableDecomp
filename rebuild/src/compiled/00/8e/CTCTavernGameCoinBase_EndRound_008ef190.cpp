// CTCTavernGameCoinBase::EndRound() -- a coin game's round ends as any tavern game's (tail jump to CTCTavernGame::EndRound)
// Leaf: ?EndRound@CTCTavernGameCoinBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EF190, 5 bytes: `jmp 0x005E9AA0` (thiscall, tail jump, no ret).
// Identity (NF179, PRIORITY q11990): Ego_r.pdb 0x00914AD0 + FableWin.pdb 0x02AEC620 ?EndRound@CTCTavernGameCoinBase@@UAEXXZ;
//   engine/CTCTavernGameCoinBase.h slot 33 = 0x008EF190; both landed vtable-side binders (0x007CF4C0, 0x007D2920) bind this spelling.
//   Was the extern "C" stand-in `CTCTavernGameCoinBase_EndRound` forwarding to `ForwardTarget_005e9aa0`. Same shape as the landed
//   siblings NewGame 0x008EF170 / NewRound 0x008EF180, here on the shared class header.
// Callee: CTCTavernGame::EndRound 0x005E9AA0, the qualified call of the virtual: Ego_r.pdb 0x006474D0 + FableWin.pdb 0x022D48C0
//   ?EndRound@CTCTavernGame@@UAEXXZ (engine/CTCTavernGame.h slot 33); was the stand-in _ForwardTarget_005e9aa0 (NOTES: the other two
//   landed referencers 0x007D7BA0 / 0x008F1720 bind the non-virtual QAE form, an existing split).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGameCoinBase.h"

void CTCTavernGameCoinBase::EndRound()
{
    CTCTavernGame::EndRound();
}
