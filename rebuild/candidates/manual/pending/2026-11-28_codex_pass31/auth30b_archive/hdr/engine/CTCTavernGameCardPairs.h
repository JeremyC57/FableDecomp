#ifndef FABLE_ENGINE_CTCTAVERNGAMECARDPAIRS_H
#define FABLE_ENGINE_CTCTAVERNGAMECARDPAIRS_H
// CTCTavernGameCardPairs (chunk 2016, row 0x007D6870 ProcessInput): the pairs card game (RTTI vtable 0x0126F6FC). Members: Ego_r.pdb
// (848 bytes there) in order and spelling at retail = PDB - 0x20 (the card base is 0x318 in retail): AlternateSelectedCard +0x318 (the
// landed CardsMainGame 0x007D62D0), LastFramePointerMoved +0x31C (0x007D6D22), NumFramesRepeated +0x320 (0x007D6B55 / 0x007D6D07),
// RepeatMovement +0x324, PreviousDirection +0x328 (0x007D6CFF / 0x007D6D0D). Overrides: Ego_r.pdb names (vtable slot 28 ProcessInput
// 0x007D6870, 59 SelectCard 0x007D5FB0). Declarations only.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCTavernGameCardBase.h"
class CTCTavernGameCardPairs : public CTCTavernGameCardBase {
public:
    virtual void ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*);   // 28 0x007D6870
#ifdef FABLE_RETAIL_TAVERN_PAIRS_RANDOM_PASS29
    virtual void GetRandomCard(long&, long&, long, long);
#endif
    virtual bool SelectCard();                                           // 59 0x007D5FB0
#ifdef FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_CARDSMAINGAME
    // NF159 opt-in (q4273, row 0x007D62D0): Ego_r.pdb 0x007D38F0 / FableWin.pdb 0x0274ACE0 ?CardsMainGame@CTCTavernGameCardPairs@@UAEXXZ, the
    // override of CTCTavernGameCardBase slot 56 (vtable 0x0126F6FC; no new slot, no layout change). Declaration only.
    virtual void CardsMainGame();                                        // 56 0x007D62D0
#endif
#ifdef FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_ROUND_OVERRIDES
    // NF96 opt-in (q11339, rows 0x007D60C0 / 0x007D6150 / 0x007D70E0): Ego_r.pdb 0x007D3410 / 0x007D34B0 / 0x007D42E0 and FableWin.pdb
    // 0x0274A260 / 0x0274A530 / 0x0274A3A0 ?EnterGame / ?EndRound / ?NewRound@CTCTavernGameCardPairs@@UAEXXZ, the overrides of CTCTavernGame
    // slots 29 / 33 / 32 (vtable 0x0126F6FC; no new slot, no layout change). Declarations only.
    virtual void EnterGame();                                            // 29 0x007D60C0
    virtual void NewRound();                                             // 32 0x007D70E0
    virtual void EndRound();                                             // 33 0x007D6150
#endif
    CCard*              AlternateSelectedCard;                           // +0x318
    long                LastFramePointerMoved;                           // +0x31c
    long                NumFramesRepeated;                               // +0x320
    bool                RepeatMovement;                                  // +0x324
    long                PreviousDirection;                               // +0x328
};
FABLE_STATIC_ASSERT_AT(21, offsetof(CTCTavernGameCardPairs, AlternateSelectedCard) == 0x318);
FABLE_STATIC_ASSERT_AT(22, offsetof(CTCTavernGameCardPairs, PreviousDirection) == 0x328);
FABLE_STATIC_ASSERT_AT(23, sizeof(CTCTavernGameCardPairs) == 0x330);
#endif
