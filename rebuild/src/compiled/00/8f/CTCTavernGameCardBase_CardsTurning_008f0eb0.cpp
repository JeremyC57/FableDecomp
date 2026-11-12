// CTCTavernGameCardBase::CardsTurning() -- once no card is still turning, advance the game state; in state 25 gather every card onto the averaged position, stacked (53 - i) separations high, and go to state 26
// Leaf: ?CardsTurning@CTCTavernGameCardBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F0EB0, 506 bytes, thiscall, plain `ret`, no SEH frame. Flags: none.
// Identity: Ego_r.pdb / FableWin.pdb ?CardsTurning@CTCTavernGameCardBase@@UAEXXZ; vtable slot 54 of CTCTavernGameCardBase 0x01288294
//   (inherited by CTCTavernGameCardSorting 0x0126F5B4 and the other card games).
// Callees: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20 (kept a call);
//   CCharString::CCharString(const char*, long) 0x0099EBF0; CTCTavernGame::PlaySoundW(const CCharString&) 0x005EA950;
//   CCharString::~CCharString 0x0099EAE0; CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20; the def's virtual slot 1 on the
//   last release. CCard::Move 0x008F0660 is inlined (same TU, defined above with its real body).
// Shape: settle loop `Turning == true` returns (its own release + epilogue); state 22 -> 23, anything but 25 -> 0 (the two
//   SetState calls cross-jump); state 25 sums CurrentPos.X/Y in ONE loop, divides each by (float)Cards.size() (computed twice),
//   then for each card plays the deal sound (named CCharString in its own block: destroyed before the Move) and moves the card to
//   (avgX, avgY, (53 - i) * CardSeparation + TableSeparation), then 26 and an explicit `return` (gives retail's separate release
//   through ebp). As in ShuffleCards 0x008F10B0, the visible GetCardDef / GetSubDef bodies let VC7.1 keep def in ebp and reuse its
//   frame slot for the strength-reduced `53 - i` counter.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE          // count-store schedule of the expanded GetPackPosition (0x008F116B)
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_TYPENAME
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCARD_POSITIONS
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#include "stlport/fable_containers.h"                
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
#include "engine/CDefClassBase_GetSubDef.h"

// 0x008F0B20 (landed row; real body, same TU in retail)
void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* pdef)
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    ParentThing->PeekBaseDef().GetSubDef(&def);
    *pdef = def;
}

// 0x008F0660 (landed row; real body, same TU in retail)
void CCard::Move(const C3DVector& position, bool immediately)
{
    if (immediately) {
        LocalPos = position;
        NewLocalPos = position;
        return;
    }
    if (Moving) {
        LocalPos = CurrentPos;
    } else {
        CurrentPos = LocalPos;
        Moving = true;
    }
    NewLocalPos = position;
    MoveFraction = 0.0f;
}

void CTCTavernGameCardBase::CardsTurning()
{
    CDefPointer<const CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    for (std::vector<CCard*>::iterator it = Cards.begin(); it != Cards.end(); ++it)
        if ((*it)->Turning == true)
            return;
    if (TavernGameState == 0x16)
        SetState((ETavernGameStates)0x17);
    else if (TavernGameState == 0x19) {
        float x = 0.0f;
        float y = 0.0f;
        for (std::vector<CCard*>::iterator it = Cards.begin(); it != Cards.end(); ++it) {
            x += (*it)->CurrentPos.X;
            y += (*it)->CurrentPos.Y;
        }
        x /= (float)Cards.size();
        y /= (float)Cards.size();
        int i = 0;
        for (std::vector<CCard*>::iterator it = Cards.begin(); it != Cards.end(); ++it, ++i) {
            {
                CCharString sound("TAVERN_GAME_CARD_DEAL", -1);
                PlaySoundW(sound);
            }
            (*it)->Move(C3DVector(x, y, (float)(53 - i) * def->CardSeparation + def->TableSeparation), false);
        }
        SetState((ETavernGameStates)0x1a);
        return;
    } else
        SetState((ETavernGameStates)0);
}