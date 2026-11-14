// CTCTavernGameCardBlackjack::NewRound() -- shuffle, give the player one empty hand and deal two cards each to the dealer and the player from the top of the pack, then reset the round state and start dealing the dealer's cards
// Leaf: ?NewRound@CTCTavernGameCardBlackjack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DB2E0, 723 bytes, one `ret` (0x007DB5B2).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_NEWROUND
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardBlackjack.h"

void CTCTavernGameCardBlackjack::NewRound()
{
    CTCTavernGameCardBase::NewRound();
    ShuffleCards();
    PlayersCards.clear();
    PlayersCards.push_back(std::vector<CCard*>());
    CurrentPackCardIndex = 0;
    Cards.push_back(Pack[CurrentPackCardIndex]);
    DealersCards.push_back(Pack[CurrentPackCardIndex++]);
    Cards.push_back(Pack[CurrentPackCardIndex]);
    DealersCards.push_back(Pack[CurrentPackCardIndex++]);
    Cards.push_back(Pack[CurrentPackCardIndex]);
    PlayersCards[0].push_back(Pack[CurrentPackCardIndex++]);
    Cards.push_back(Pack[CurrentPackCardIndex]);
    PlayersCards[0].push_back(Pack[CurrentPackCardIndex++]);
    CurrentMovingCard = -1;
    CurrentDealingCards = &DealersCards;
    CurrentPlayerCards = &PlayersCards[0];
    CurrentPlayerCardsIndex = 0;
    CurrentDealingCardsIndex = 0;
    NewlyDealtCard = 0;
    HasPlayerSplit = false;
    HasPlayerFinishedRound = false;
    HasDealerStartedTakingCards = false;
    Doubled = false;
    SetState(ETGS_DEAL_DEALERS_CARDS);
}
