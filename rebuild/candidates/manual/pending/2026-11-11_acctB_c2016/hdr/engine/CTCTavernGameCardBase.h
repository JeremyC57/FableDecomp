#ifndef FABLE_ENGINE_CTCTAVERNGAMECARDBASE_H
#define FABLE_ENGINE_CTCTAVERNGAMECARDBASE_H
// CTCTavernGameCardBase (chunk 2016, row 0x007D6870 CTCTavernGameCardPairs::ProcessInput): the base of the tavern card games (RTTI
// vtable 0x01288294; the landed rows 0x008F0930..0x008F2580 still use local models). Needs FABLE_RETAIL_CTCTAVERNGAME (the retail
// CTCTavernGame, 0xF0 bytes).
// Members: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv, 824 bytes there) in order and spelling at the retail offsets = PDB - 0x18 (the
// six STLport vectors of CTCTavernGame) - 4 per earlier vector of this class (Cards, Pack, ParticleEffects). Retail evidence: 0x007D6BFC
// GamePositionRHSet +0xF0, 0x007D6F26 GamePositionPos +0x108, 0x007D6D1C..0x007D6D84 MinX / MaxX / MinY / MaxY +0x118..+0x124,
// 0x007D6D8E Cards +0x128, 0x007D6BA6 Pointer +0x140, GameType +0x300 / SelectedCard +0x308 (the landed CardsMainGame 0x007D62D0),
// 0x007D6BFF HighlightedCard +0x30C. Retail sizeof is 0x318 (the landed CTCTavernGameCardBlackjack ctor 0x007DB280 starts its own
// members at +0x318; CTCTavernGameCardPairs::AlternateSelectedCard is at +0x318): the members end at +0x314 and the class is 8-aligned
// (CTCTavernGame::LastRenderTime is a double), so the last word is tail padding.
// Virtuals: slots 51..61 in FableWin.pdb / Ego_r.pdb spelling, ordered from the vtables (CTCTavernGameCardSorting 0x0126F5B4 inherits
// slots 51 0x008F0BD0, 52 0x008F2280 CardsMoving (landed), 53 0x008F09E0, 54 0x008F0EB0, 55 0x008F17D0 TurnCardsBack (landed),
// 58 0x008F09B0, 60 0x008F10B0, 61 0x008F18C0; CTCTavernGameCardPairs 0x0126F6FC overrides 53 / 56 / 57 / 58 / 59 / 60, exactly its
// Ego_r.pdb overrides CardsFinishedMoving / CardsMainGame / GetCardOrder / GetRandomCard / SelectCard / ShuffleCards; 0x007D6909 calls
// slot 59 (+0xEC) SelectCard). CardsMainGame has no CTCTavernGameCardBase body in either PDB (declared here without `= 0`, which does
// not change the vtable layout).
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CTCTavernGame.h"
#include "engine/CCard.h"
class CThingObject;
class CTavernGameCardBaseDef;
enum ECardGameType;                                                      // MS forward enum (Ego_r.pdb member type; values not needed here)
class CTCTavernGameCardBase : public CTCTavernGame {
public:
    virtual void ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*);   // 28 0x008F12D0
    virtual void DealCards();                                            // 51 0x008F0BD0
    virtual void CardsMoving();                                          // 52 0x008F2280
    virtual void CardsFinishedMoving();                                  // 53 0x008F09E0
    virtual void CardsTurning();                                         // 54 0x008F0EB0
    virtual void TurnCardsBack();                                        // 55 0x008F17D0
    virtual void CardsMainGame();                                        // 56
    virtual long GetCardOrder();                                         // 57
    virtual void GetRandomCard(long&, long&, long, long);                // 58 0x008F09B0
    virtual bool SelectCard();                                           // 59
    virtual void ShuffleCards();                                         // 60 0x008F10B0
    virtual void UpdateHighlightedCard();                                // 61 0x008F18C0
    // Ego_r.pdb / FableWin.pdb ?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z, out of line in
    // retail at 0x008F0B20 (landed, the bound spelling). Declaration only.
    void GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* def);     // 0x008F0B20
    CRightHandedSet     GamePositionRHSet;                               // +0xf0
    C3DVector           GamePositionPos;                                 // +0x108
    long                GamePositionActionPointIndex;                    // +0x114
    float               MinX;                                            // +0x118
    float               MaxX;                                            // +0x11c
    float               MinY;                                            // +0x120
    float               MaxY;                                            // +0x124
    std::vector<CCard*> Cards;                                           // +0x128
    std::vector<CCard*> Pack;                                            // +0x134
    CThingObject*       Pointer;                                         // +0x140
    float               PointerScale;                                    // +0x144
    std::vector<CIntelligentPointer<CThing> > ParticleEffects;           // +0x148
    float               Counter;                                         // +0x154
    bool                RoundFinished;                                   // +0x158
    float               Times[52];                                       // +0x15c
    float               BestTimes[52];                                   // +0x22c
    long                NumCards;                                        // +0x2fc
    ECardGameType       GameType;                                        // +0x300
    long                CardOrder;                                       // +0x304
    CCard*              SelectedCard;                                    // +0x308
    CCard*              HighlightedCard;                                 // +0x30c
    long                CurrentMovingCard;                               // +0x310
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, GamePositionRHSet) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, GamePositionPos) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, MinX) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, Cards) == 0x128);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, Pointer) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, Counter) == 0x154);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, GameType) == 0x300);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, HighlightedCard) == 0x30c);
FABLE_STATIC_ASSERT(sizeof(CTCTavernGameCardBase) == 0x318);
#endif
