// 007dac10 CTCTavernGameCardBlackjack::DealCards
// Flags: none
// Leaf: ?DealCards@CTCTavernGameCardBlackjack@@UAEXXZ
#define FABLE_RETAIL_BLACKJACK_DEALCARDS_PASS26
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCARD_POSITIONS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#include "fable_game.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#define FABLE_RETAIL_BLACKJACK_DEAL_PASS23
#include "engine/CTCTavernGameCardBlackjack.h"
#include "engine/C2DVector.h"
#include <math.h>

// The FableWin module data record names the file-scope float. DealCard has a different, block-scope float.
static float mult_to_stay_off_table = 2.0f;

void CTCTavernGameCardBlackjack::DealCards()
{
    CDefPointer<const CTavernGameCardBaseDef> pdef(NULL_DEF);
    GetCardDef(&pdef);
    long card_order = GetGameState() == ETGS_DEAL_DEALERS_CARDS ? PlayersCards.size() : 0;
    bool moving = true;
    if (CurrentMovingCard == -1 || !(*CurrentDealingCards)[CurrentMovingCard]->Moving) {
        ++CurrentMovingCard;
        if (CurrentMovingCard == 2) {
            if (GetGameState() == ETGS_DEAL_DEALERS_CARDS) {
                CurrentMovingCard = -1;
                CurrentDealingCards = &PlayersCards[0];
                Bets.push_back(BaseBet);
                SetState(ETGS_DEAL_CARDS);
                return;
            } else {
                SetState(ETGS_CARDS_FINISHED_MOVING);
                return;
            }
        }
        moving = false;
    }

    if (!moving) {
        C3DVector offset;
        offset = GetPosition(card_order, CurrentMovingCard);
        offset.Z = (*CurrentDealingCards)[CurrentMovingCard]->LocalPos.Z;
        if (offset.X < MinX) MinX = offset.X;
        if (offset.X > MaxX) MaxX = offset.X;
        if (offset.Y < MinY) MinY = offset.Y;
        if (offset.Y > MaxY) MaxY = offset.Y;
        PlaySoundW("TAVERN_GAME_CARD_DEAL");
        (*CurrentDealingCards)[CurrentMovingCard]->Move(offset, false);
    } else if ((float)fabs((*CurrentDealingCards)[CurrentMovingCard]->NewLocalPos.Z -
                (pdef->CardSeparation * mult_to_stay_off_table + pdef->TableSeparation)) > DELTA_NOTIONAL_ZERO) {
        C2DVector base_point(GetPackPosition());
        C2DVector current_position((*CurrentDealingCards)[CurrentMovingCard]->CurrentPos);
        float x_dist = (float)fabs(base_point.X - current_position.X);
        float y_dist = (float)fabs(base_point.Y - current_position.Y);
        if (x_dist > pdef->CardWidth || y_dist > pdef->CardHeight) {
            C3DVector new_pos((*CurrentDealingCards)[CurrentMovingCard]->NewLocalPos);
            new_pos.Z = pdef->CardSeparation * mult_to_stay_off_table + pdef->TableSeparation;
            PlaySoundW("TAVERN_GAME_CARD_DEAL");
            (*CurrentDealingCards)[CurrentMovingCard]->Move(new_pos, false);
        }
    }
}