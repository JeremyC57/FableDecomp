// CTCTavernGameCardBlackjack::DealCard() -- deal one card from the pack to the hand being dealt (state 0x25), turn it face up once it lands (state 0x16), or send a stray landed card back to its slot
// Grade: DIFFER(904v904) -- Q21 PORTED onto engine/card_sorting_types.h (shared CTCTavernGameCardBase / CCard; overlay opt-in CCARD_POSITIONS types LocalPos/NewLocalPos/CurrentPos as the packed C3DVector) + CTCTavernGame / CDefPointer / CTavernGameCardBaseDef retail headers (4 Q21 grades). Same residue as the local draft: in the flip branch (Moving test, PlaySound arg, Turn) and the GetCardDef arg the scratch registers rotate (retail vector ecx / start edx / card eax / flag cl, ours edx / ecx / edx / al; retail lea ecx / edx for the temp addresses, ours eax). Tried `== false` and a nested if in the else block (worse: string temp slot moves).
// Leaf: ?DealCard@CTCTavernGameCardBlackjack@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DAEF0, 904 bytes, plain `ret`.
// Callees: CTCTavernGame::GetDef(CDefPointer<const CTavernGameDef>*) 0x005EA000 (out of line in this TU, as the landed 0x007DA3A0);
//   vector<CCard*>::_M_insert_overflow 0x007D5D60 (STLport push_back slow path); CTCTavernGame::SetState 0x005E9B20 /
//   PlaySoundW 0x005EA950; CTCTavernGameCardBlackjack::GetPosition(long, long) 0x007DA760; CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0; CCard::Move 0x008F0660 / Turn 0x008F0620; CTCTavernGameCardBase::GetCardDef 0x008F0B20 /
//   GetPackPosition 0x008F0B80. Constants: 2^32 0x0122DCB4 (unsigned size to float), 1.0f 0x0122DED8, 1/3 0x0125A4DC, the card layer
//   height 0x01383C98, DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Layout: shared CTCTavernGameCardBase (engine/card_sorting_types.h: MinX..MaxY +0x118, Cards +0x128, Pack +0x134) and CCard (Moving +0x26,
//   LocalPos +0x27, NewLocalPos +0x33, CurrentPos +0x3f: overlay opt-in FABLE_RETAIL_CCARD_POSITIONS types them as the PDB's byte-aligned
//   C3DVector); CTavernGameCardBaseDef CardWidth +0x2c, CardHeight +0x30, CardSeparation +0x5c, TableSeparation +0x60. Blackjack's own
//   members (Ego_r.pdb order, retail offsets as the landed 0x007DA120): CurrentDealingCards +0x340, NewlyDealtCard +0x344,
//   CurrentDealingCardsIndex +0x358, CurrentPackCardIndex +0x35c; DealIndex (+0x310) is the Ego_r.pdb CTCTavernGameCardBase tail member
//   modelled here as the class's own (no header has it yet). No shared CTCTavernGameCardBlackjack header: local class on the shared base.
// Shape: the dealt card is pushed onto Cards, remembered and pushed onto the dealing hand (Pack[CurrentPackCardIndex++] read three times);
//   the bounds update is four plain compares; the stray-card test builds two C2DVectors (pack position, card position) and compares fabs
//   differences against the card size.
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
#include "engine/card_sorting_types.h"
#include "engine/C2DVector.h"
#include <math.h>

extern float g_CardLayerHeight;                             // 0x01383C98

class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    enum EBlackjackWinStates { };                           // W4EBlackjackWinStates (enumerators not needed here)
    C3DVector GetPosition(long hand, long card);            // 0x007DA760
    void DealCard();
private:
    std::vector<CCard*> DealersCards;                       // +0x318
    std::vector<std::vector<CCard*> > PlayersCards;         // +0x324
    std::vector<long> Bets;                                 // +0x330
    std::vector<CCard*>* CurrentPlayerCards;                // +0x33c
    std::vector<CCard*>* CurrentDealingCards;               // +0x340
    CCard* NewlyDealtCard;                                  // +0x344
    float MostMoneyWon;                                     // +0x348
    float MoneyWonThisSitting;                              // +0x34c
    long Answer;                                            // +0x350
    long CurrentPlayerCardsIndex;                           // +0x354
    long CurrentDealingCardsIndex;                          // +0x358
    long CurrentPackCardIndex;                              // +0x35c
    float CurrentBet;                                       // +0x360
    EBlackjackWinStates WinState;                           // +0x364
    bool HasPlayerSplit;                                    // +0x368
};

void CTCTavernGameCardBlackjack::DealCard()
{
    CDefPointer<const CTavernGameDef> pdef(NULL_DEF);
    GetDef(&pdef);
    if (CurrentMovingCard == -1) {
        CurrentMovingCard = CurrentDealingCards->size();
        Cards.push_back(Pack[CurrentPackCardIndex]);
        NewlyDealtCard = Pack[CurrentPackCardIndex];
        CurrentDealingCards->push_back(Pack[CurrentPackCardIndex++]);
        SetState(ETGS_BLACKJACK_NEW_CARD_DEALT);
        C3DVector position = GetPosition(CurrentDealingCardsIndex, CurrentMovingCard);
        position.Z = (*CurrentDealingCards)[CurrentMovingCard]->LocalPos.Z;
        if (position.X < MinX)
            MinX = position.X;
        if (position.X > MaxX)
            MaxX = position.X;
        if (position.Y < MinY)
            MinY = position.Y;
        if (position.Y > MaxY)
            MaxY = position.Y;
        PlaySoundW("TAVERN_GAME_CARD_DEAL");
        (*CurrentDealingCards)[CurrentMovingCard]->Move(position, false);
    } else if (!(*CurrentDealingCards)[CurrentMovingCard]->Moving) {
        PlaySoundW("TAVERN_GAME_CARD_FLIP");
        (*CurrentDealingCards)[CurrentMovingCard]->Turn();
        SetState(ETGS_CARDS_TURNING_UP);
        return;
    } else {
        CDefPointer<const CTavernGameCardBaseDef> pcarddef(NULL_DEF);
        GetCardDef(&pcarddef);
        CCard* card = (*CurrentDealingCards)[CurrentMovingCard];
        float z = (((float)CurrentDealingCards->size() - 1.0f) * 0.33333334f + g_CardLayerHeight) * pcarddef->CardSeparation +
                  pcarddef->TableSeparation;
        if (card->NewLocalPos.Z - z > DELTA_NOTIONAL_ZERO) {
            C2DVector pack(GetPackPosition());
            C2DVector current(card->CurrentPos);
            if (fabs(pack.X - current.X) > pcarddef->CardWidth || fabs(pack.Y - current.Y) > pcarddef->CardHeight) {
                C3DVector to(card->NewLocalPos.X, card->NewLocalPos.Y, z);
                PlaySoundW("TAVERN_GAME_CARD_DEAL");
                card->Move(to, false);
            }
        }
    }
}
