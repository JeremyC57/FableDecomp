# NF159 overlay headers (round 10)

Base copies in `../orig/engine/` were copied from the repo working tree at start and are byte-identical to it.
Every change is an add-only `#ifdef` opt-in with a member-specific macro. No sibling agent (NF150..NF158) overlays these headers.

| header | opt-in macro | member added | evidence |
|---|---|---|---|
| engine/CTCTavernGameCardBase.h | `FABLE_RETAIL_CTCTAVERNGAMECARDBASE_QUITMIDGAME` | `virtual void QuitMidGame(); // 34 0x008F09A0` (after the ENDROUND block) | ?QuitMidGame@CTCTavernGameCardBase@@UAEXXZ in Ego_r.pdb and FableWin.pdb; vtable slot 34 = 0x008F09A0 |
| engine/CTCTavernGameCardBlackjack.h | `FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_TURNPLAYERCARDS` | `void TurnPlayerCards(); // 0x007DA290` (after NewRound) | both PDBs, QAE |
| engine/CTCTavernGameCardBlackjack.h | `FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_REORDERCARDS` | `void ReorderCards(); // 0x007DA810` (after NewRound) | both PDBs, QAE |
| engine/CTCTavernGameCardPairs.h | `FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_CARDSMAINGAME` | `virtual void CardsMainGame(); // 56 0x007D62D0` | Ego_r.pdb 0x007D38F0 / FableWin.pdb 0x0274ACE0 records, UAE; vtable slot 56 |

- `pin_asserts.py` pinned 0 asserts in each of the three headers (no `FABLE_STATIC_ASSERT` moved).
- Default path is token-identical: preprocessed output with the macros off has the same md5 for orig/ and hdr/
  (CardBase 6b1edf02..., Blackjack 790d395e..., CardPairs 7d3b17e6...).
- Landed includers (`grep -rlE 'engine/<header>.h' rebuild/src/compiled`): CardBase 12, Blackjack 10, CardPairs 4. None of them
  defines the new macros, so the opt-ins change none of them.
- Rows that turn the opt-ins on: 0x008F09A0 (QUITMIDGAME), 0x007DA290 (TURNPLAYERCARDS), 0x007DA810 (REORDERCARDS),
  0x007D62D0 (CARDSMAINGAME).
