// CTCTavernGameCardPairs::CardsFinishedMoving() -- once the cards have moved out, waits for OK on the click-to-continue text, then goes to the main game (ETGS_CARDS_MAIN_GAME)
// Leaf: ?CardsFinishedMoving@CTCTavernGameCardPairs@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D6260, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 53 of CTCTavernGameCardPairs's vtable 0x0126F6FC (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardPairs); the Xbox 360 Test PDB virtual order (third source) aligned on 40 TLC-named slots of that vtable puts CardsFinishedMoving(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CardsFinishedMoving@CTCTavernGameCardPairs@@UAEXXZ; `ret` matches the signature.
// Was: CTCTavernGameCardPairs::vfunc_53 (rebuild/src/compiled/00/7d/CTCTavernGameCardPairs_vfunc53_007d6260.cpp).
// No shared header for CTCTavernGameCardPairs: minimal local definition on its RTTI base CTCTavernGameCardBase.
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
#include "fable_game.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
class CTCTavernGameCardPairs : public CTCTavernGameCardBase {
public:
    virtual void CardsFinishedMoving();                      // slot 53
};
void CTCTavernGameCardPairs::CardsFinishedMoving()
{
    CDefPointer<const CTavernGameDef> pdef(NULL_DEF);
    GetDef(&pdef);
    SetStateAfterOK(GetText(pdef->ClickToContinue), GetText(pdef->Continue), ETGS_CARDS_MAIN_GAME);
}
