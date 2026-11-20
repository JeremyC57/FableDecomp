// CTCTavernGameShoveHaPenny::EndRound() -- end-of-game override: coin-base hook, then if the game was won and the hero has no best score yet (0) or beat it, flags a new best score; enters state 8
// Retail: Fable.exe @ 0x007CF4C0, 96 bytes (vtable 0x0126F2F4 slot 33).
// Callees: CTCTavernGameCoinBase::EndRound 0x008EF190 (non-virtual base call),
//          CTCTavernGame::GetHeroStats(CTCHeroStats**) 0x005EB4B0 (out parameter),
//          CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20.
// Members (retail = PDB - 8 / - 0x1C, see CTCTavernGame.h): GameWon +0x21, Score +0x2C,
// NewBestScore +0xA0; the hero stats record holds the best shove-ha'penny score at +0x18C.
// Shape: the stats pointer is a null-initialised out-parameter local (`mov [esp+8],0` after the
//        push); `best == 0.0f` is `fld [0.0] ; fld best ; fucompp ; test ah,44h ; jnp` (the literal
//        is a pool constant -- 0x0122DEDC), `Score > best` is `fld ; fcomp ; test ah,41h ; jne`.
// Leaf: ?EndRound@CTCTavernGameShoveHaPenny@@UAEXXZ
// Naming (NF159, 2026-11-20, q4271): ported onto engine/CTCTavernGameShoveHaPenny.h / CTCHeroStats.h (retail branches) from the local
//   CTCTavernGameCoinBase / CHeroStats stand-ins: SetState is CTCTavernGame's (?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z, Ego_r.pdb
//   0x00647550 / FableWin.pdb 0x022D5AD0; was ?SetState@CTCTavernGameCoinBase@@QAEXJ@Z), GetHeroStats is CTCTavernGame's
//   (?GetHeroStats@CTCTavernGame@@QAEXPAPAVCTCHeroStats@@@Z, the landed 0x007CF5B0 spelling; was the CoinBase / CHeroStats stand-in), the base
//   call is the virtual CTCTavernGameCoinBase::EndRound (Ego_r.pdb 0x00914AD0 / FableWin.pdb 0x02AEC620 ?EndRound@CTCTavernGameCoinBase@@UAEXXZ,
//   called non-virtually; was QAE) and the best score is CTCHeroStats::BestScoreShoveHaPenny (+0x18C). 8 is ETGS_REACTION_AND_PAYOUT.
//   Own leaf: the virtual override (vtable 0x0126F2F4 slot 33; Ego_r.pdb 0x007CDA20 / FableWin.pdb 0x0273B170
//   ?EndRound@CTCTavernGameShoveHaPenny@@UAEXXZ; was QAE).
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#include "engine/CTCTavernGameShoveHaPenny.h"
#include "engine/CTCHeroStats.h"

void CTCTavernGameShoveHaPenny::EndRound()
{
    CTCTavernGameCoinBase::EndRound();
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    if (GameWon) {
        if (stats->BestScoreShoveHaPenny == 0.0f || Score > stats->BestScoreShoveHaPenny)
            NewBestScore = true;
    }
    SetState(ETGS_REACTION_AND_PAYOUT);
}
