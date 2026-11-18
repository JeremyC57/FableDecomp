// CTCTavernGameCardBlackjack::GetCurrentScore() -- the def's "current score" text followed by the value of the player's current hand (or "0" while no hand is being played)
// Leaf: ?GetCurrentScore@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA400, 280 bytes, thiscall, `ret 4` (CWideString by hidden pointer); vtable 0x0126F8F4 slot 44.
// Names: Ego_r.pdb / FableWin.pdb / Ego_d.pdb (tc_tavern_game_card_blackjack.obj, in that TU order: GetBestScore, GetCurrentScore,
//   GetRequiredScore, GetAdditionalInfo, GetBet ... GetHandValue); locals as Ego_d (pdef). GetDef 0x005EA000 (out of line for this TU,
//   FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE, as the landed GetWinPhrase 0x007DA3A0); the def release is the count-through-reference
//   form (`mov edi,[eax+4] ; add eax,4 ; dec edi ; mov edx,edi ; test edx,edx ; mov [eax],edi`: FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF).
// Callees: CTCTavernGame::GetDef 0x005EA000, CTCTavernGameCardBlackjack::GetHandValue 0x007DA230 (same TU, body below),
//   GFFloatToCharString(double n, long precision) 0x009A08B0 (Ego_r / FableWin ?GFFloatToCharString@@YI?AVCCharString@@NJ@Z; landed
//   rows bind it as CCharString::FormatDouble / FormatRealText_009a08b0 -> NAME_FIXES.tsv), CCharString::ToWideString 0x0099E480,
//   CTCTavernGame::GetText 0x005E9E70, operator+(const CWideString&, const CWideString&) 0x0099BE70 / (const CWideString&,
//   const unsigned short*) 0x0099BF30 (L"0" 0x01231640), ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, the def's Delete (slot 1).
// Shape: GetHandValue's body is visible in the TU (defined after this row, as in Ego_d's object order): without it VC7.1 gives the
//   long -> double conversion word its own frame slot (frame 0x14); with it the word shares the GetText result slot (retail 0x10).
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
CWideString CTCTavernGameCardBlackjack::GetCurrentScore()
{
    CDefPointer<const CTavernGameDef> pdef;
    GetDef(&pdef);
    if (CurrentPlayerCards)
        return GetText(pdef->CurrentScore) + GFFloatToCharString(GetHandValue(CurrentPlayerCards, false), 0).ToWideString();
    return GetText(pdef->CurrentScore) + L"0";
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