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
    virtual bool SelectCard();                                           // 59 0x007D5FB0
    CCard*              AlternateSelectedCard;                           // +0x318
    long                LastFramePointerMoved;                           // +0x31c
    long                NumFramesRepeated;                               // +0x320
    bool                RepeatMovement;                                  // +0x324
    long                PreviousDirection;                               // +0x328
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardPairs, AlternateSelectedCard) == 0x318);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardPairs, PreviousDirection) == 0x328);
FABLE_STATIC_ASSERT(sizeof(CTCTavernGameCardPairs) == 0x330);
#endif
