// 007daef0 CTCTavernGameCardBlackjack::DealCard
// Flags: none
// Leaf: ?DealCard@CTCTavernGameCardBlackjack@@QAEXXZ
// Identity: both original PDBs and native caller/state-machine operands.
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

// Original FableWin module332 local data at04a55ac4; native01383c98.
static float mult_to_stay_off_table=2.0f;

void CTCTavernGameCardBlackjack::DealCard()
{
    CDefPointer<const CTavernGameDef> pdef(NULL_DEF);
    GetDef(&pdef);
    bool moving=true;
    if (CurrentMovingCard == -1) {
        CurrentMovingCard = CurrentDealingCards->size();
        Cards.push_back(Pack[CurrentPackCardIndex]);
        NewlyDealtCard = Pack[CurrentPackCardIndex];
        CurrentDealingCards->push_back(Pack[CurrentPackCardIndex++]);
        moving=false;
        SetState(ETGS_BLACKJACK_NEW_CARD_DEALT);
    } else if (!(*CurrentDealingCards)[CurrentMovingCard]->Moving) {
        PlaySoundW("TAVERN_GAME_CARD_FLIP");
        (*CurrentDealingCards)[CurrentMovingCard]->Turn();
        SetState(ETGS_CARDS_TURNING_UP);
        return;
    }
    if(!moving) {
        C3DVector offset = GetPosition(CurrentDealingCardsIndex, CurrentMovingCard);
        offset.Z = (*CurrentDealingCards)[CurrentMovingCard]->LocalPos.Z;
        if (offset.X < MinX)
            MinX = offset.X;
        if (offset.X > MaxX)
            MaxX = offset.X;
        if (offset.Y < MinY)
            MinY = offset.Y;
        if (offset.Y > MaxY)
            MaxY = offset.Y;
        PlaySoundW("TAVERN_GAME_CARD_DEAL");
        (*CurrentDealingCards)[CurrentMovingCard]->Move(offset, false);
    } else {
        CDefPointer<const CTavernGameCardBaseDef> pcard_def(NULL_DEF);
        GetCardDef(&pcard_def);
        CCard* card = (*CurrentDealingCards)[CurrentMovingCard];
        float z = (((float)CurrentDealingCards->size() - 1.0f) * 0.33333334f + mult_to_stay_off_table) * pcard_def->CardSeparation +
                  pcard_def->TableSeparation;
        if (card->NewLocalPos.Z - z > DELTA_NOTIONAL_ZERO) {
            C2DVector base_point(GetPackPosition());
            C2DVector current_position(card->CurrentPos);
            if (fabs(base_point.X - current_position.X) > pcard_def->CardWidth || fabs(base_point.Y - current_position.Y) > pcard_def->CardHeight) {
                C3DVector new_pos(card->NewLocalPos.X, card->NewLocalPos.Y, z);
                PlaySoundW("TAVERN_GAME_CARD_DEAL");
                card->Move(new_pos, false);
            }
        }
    }
}