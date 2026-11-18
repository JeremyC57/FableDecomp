// CTCTavernGameCardBlackjack::GetRequiredScore() -- the def's "required score" text followed by the value of the dealer's hand (or "0" while no hand is being played)
// Leaf: ?GetRequiredScore@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA520, 286 bytes, thiscall, `ret 4` (CWideString by hidden pointer); vtable 0x0126F8F4 slot 45.
// Names: Ego_r.pdb / FableWin.pdb / Ego_d.pdb (tc_tavern_game_card_blackjack.obj, in that TU order: GetBestScore, GetCurrentScore,
//   GetRequiredScore, GetAdditionalInfo, GetBet ... GetHandValue); locals as Ego_d (pdef). GetDef 0x005EA000 (out of line for this TU,
//   FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE, as the landed GetWinPhrase 0x007DA3A0); the def release is the count-through-reference
//   form (`mov edi,[eax+4] ; add eax,4 ; dec edi ; mov edx,edi ; test edx,edx ; mov [eax],edi`: FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF).
// Callees: as GetCurrentScore 0x007DA400 (GetHandValue on DealersCards +0x318; CTavernGameDef::RequiredScore +0xbc).
// Shape: the twin of GetCurrentScore 0x007DA400; GetHandValue's same-TU body gives retail's shared conversion / GetText frame word.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_SCORETEXT
#define FABLE_RETAIL_GFFLOATTOCHARSTRING
#include <stddef.h>
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardBlackjack.h"
#include "engine/CTavernGameDef.h"
#include "engine/fable_script_text.h"
CWideString CTCTavernGameCardBlackjack::GetRequiredScore()
{
    CDefPointer<const CTavernGameDef> pdef;
    GetDef(&pdef);
    if (CurrentPlayerCards)
        return GetText(pdef->RequiredScore) + GFFloatToCharString(GetHandValue(&DealersCards, false), 0).ToWideString();
    return GetText(pdef->RequiredScore) + L"0";
}

// Same-TU body (Ego_d tc_tavern_game_card_blackjack.obj: GetHandValue follows the score getters; landed row 0x007DA230). With it
// visible VC7.1 lets the long -> double conversion word of the hand value share the GetText result's frame word (frame 0x10).
long CTCTavernGameCardBlackjack::GetHandValue(std::vector<CCard*>* hand, bool include_face_down_cards)   // 0x007DA230
{
    long total = 0;
    long num_aces = 0;
    for (std::vector<CCard*>::iterator card = hand->begin(); card != hand->end(); ++card) {
        if (include_face_down_cards || (*card)->Turning || !(*card)->FaceDown) {
            if ((*card)->Value == 11)
                ++num_aces;
            total += (*card)->Value;
        }
    }
    while (total > 21 && num_aces > 0) {
        total -= 10;
        --num_aces;
    }
    return total;
}