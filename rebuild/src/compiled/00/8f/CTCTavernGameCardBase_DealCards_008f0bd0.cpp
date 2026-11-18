// CTCTavernGameCardBase::DealCards() -- deal the cards one by one: with no dealt area yet start at card 0; when the current card has stopped, step to the next (all dealt -> state ETGS_CARDS_FINISHED_MOVING); place the card at its card-order slot (growing the dealt-area bounds, deal sound, CCard::Move); while it moves, send a card that drifted off the pack position back to its target at table height
// FITTED: the grouping parentheses `(mult_to_stay_off_table * pdef->CardSeparation) + pdef->TableSeparation` (no code; without them the new_pos Y store is scheduled before the Z fstp, 0x008F0E83 v 0x008F0E87); the implicit const char* PlaySoundW argument (retail re-reads the temporary's address, 0x008F0D1A).
// Leaf: ?DealCards@CTCTavernGameCardBase@@UAEXXZ
// Inline-budget: existing shared noinline body FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY -- retail calls GetSubDef<CTavernGameCardBaseDef> 0x008F27A0 out of line from GetCardDef 0x008F0B20 (graded RELOCATION_MATCH there). CCard::Move / GetCardDef are plain same-TU bodies: VC7.1 inlines Move at the placing site and calls it at 0x008F0E8B as retail, with no device. FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE (existing declaration-only opt-in, as landed 0x007DB2E0) is not an inline control here: the row never calls CTCTavernGame::GetDef; it only replaces the inline default body, which needs the complete CTavernGameDef (C2027 without it).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F0BD0, 722 bytes, `ret`; vtable slot 51.
// Ego_d.pdb (tc_tavern_game_card_base.obj): pdef, card_order, moving at function scope; { offset } ; { current_position, y_dist,
//   base_point, x_dist { new_pos } }; file static `mult_to_stay_off_table` (LDATA32 float; retail .data 0x0138927C = 2.0f, only read here).
// Same-TU bodies (Ego_d object order: CCard::Move before, GetCardDef after the row), each graded at its own address with this file:
//   CCard::Move 0x008F0660 MATCH (inlined at the placing site, called at 0x008F0E8B), GetCardDef 0x008F0B20 RELOCATION_MATCH (Ego_d
//   names pdef / new_pdef), the shared GetSubDef<CTavernGameCardBaseDef> body 0x008F27A0 RELOCATION_MATCH.
// Callees: GetCardOrder (slot 57), CTCTavernGame::PlaySoundW 0x005EA950 / SetState 0x005E9B20, GetPackPosition 0x008F0B80,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0; DELTA_NOTIONAL_ZERO 0x0129BA3C.
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
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_BLACKJACK_DEAL_PASS23
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGame.h"
#include "engine/CCardPositionsDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThing.h"
#include "engine/CTCTavernGameCardBase.h"
extern const float DELTA_NOTIONAL_ZERO;                                  // 0x0129BA3C
static float mult_to_stay_off_table = 2.0f;                              // 0x0138927C (Ego_d LDATA32 tc_tavern_game_card_base.obj)

void CCard::Move(const C3DVector& new_pos, bool immediate)
{
    if (immediate) {
        LocalPos = new_pos;
        NewLocalPos = new_pos;
        return;
    }
    if (Moving) {
        LocalPos = CurrentPos;
    } else {
        CurrentPos = LocalPos;
        Moving = true;
    }
    NewLocalPos = new_pos;
    MoveFraction = 0.0f;
}

void CTCTavernGameCardBase::DealCards()
{
    CDefPointer<const CTavernGameCardBaseDef> pdef;
    GetCardDef(&pdef);
    long card_order = GetCardOrder();
    bool moving = false;
    if (MinX == 0.0f && MaxX == 0.0f && MinY == 0.0f && MaxY == 0.0f)
        CurrentMovingCard = 0;
    else if (!Cards[CurrentMovingCard]->Moving) {
        ++CurrentMovingCard;
        if (CurrentMovingCard == Cards.size()) {
            SetState(ETGS_CARDS_FINISHED_MOVING);
            return;
        }
    }
    else
        moving = true;
    if (!moving) {
        C3DVector offset(pdef->CardWidth * pdef->CardOrders[card_order].Offset[CurrentMovingCard].X,
                         pdef->CardOrders[card_order].Offset[CurrentMovingCard].Y * pdef->CardHeight,
                         Cards[CurrentMovingCard]->LocalPos.Z);
        if (offset.X < MinX)
            MinX = offset.X;
        if (offset.X > MaxX)
            MaxX = offset.X;
        if (offset.Y < MinY)
            MinY = offset.Y;
        if (offset.Y > MaxY)
            MaxY = offset.Y;
        PlaySoundW("TAVERN_GAME_CARD_DEAL");
        Cards[CurrentMovingCard]->Move(offset, false);
    } else if (fabs(Cards[CurrentMovingCard]->NewLocalPos.Z - ((mult_to_stay_off_table * pdef->CardSeparation) + pdef->TableSeparation)) > DELTA_NOTIONAL_ZERO) {
        C2DVector base_point(GetPackPosition());
        C2DVector current_position(Cards[CurrentMovingCard]->CurrentPos);
        float x_dist = (float)fabs(base_point.X - current_position.X);
        float y_dist = (float)fabs(base_point.Y - current_position.Y);
        if (x_dist > pdef->CardWidth || y_dist > pdef->CardHeight) {
            C3DVector new_pos(Cards[CurrentMovingCard]->NewLocalPos.X, Cards[CurrentMovingCard]->NewLocalPos.Y,
                              (mult_to_stay_off_table * pdef->CardSeparation) + pdef->TableSeparation);
            Cards[CurrentMovingCard]->Move(new_pos, false);
        }
    }
}

void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* pdef)
{
    CDefPointer<const CTavernGameCardBaseDef> new_pdef;
    GetParentThing().PeekBaseDef().GetSubDef(&new_pdef);
    *pdef = new_pdef;
}