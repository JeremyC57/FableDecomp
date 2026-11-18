// CTCTavernGameCardBlackjack::GetBet() -- the def's "bet" text followed by the bet on the player's current hand (when there are bets and the hand index is valid), else the game's base bet
// Leaf: ?GetBet@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA640, 274 bytes, thiscall, `ret 4` (CWideString by hidden pointer); vtable 0x0126F8F4 slot 47.
// Names: Ego_r.pdb / FableWin.pdb / Ego_d.pdb (tc_tavern_game_card_blackjack.obj, in that TU order: GetBestScore, GetCurrentScore,
//   GetRequiredScore, GetAdditionalInfo, GetBet ... GetHandValue); locals as Ego_d (pdef). GetDef 0x005EA000 (out of line for this TU,
//   FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE, as the landed GetWinPhrase 0x007DA3A0); the def release is the count-through-reference
//   form (`mov edi,[eax+4] ; add eax,4 ; dec edi ; mov edx,edi ; test edx,edx ; mov [eax],edi`: FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF).
//   Ego_d locals pdef, ret (CWideString).
// Callees: GetDef 0x005EA000, GetText 0x005E9E70, GFIntToWideString(long) 0x00405F00 (engine/GFIntToWideString.h: called in the first
//   arm, expanded in the second -- CWideString() 0x0099AED0, Sprintf(L"%d") 0x0099BA70, CWideString(const CWideString&) 0x0099B720),
//   CWideString::operator+=(const CWideString&) 0x0099B8D0, ~CWideString 0x0099B510, the def's Delete (slot 1).
// Layout: CTCTavernGame::BaseBet +0x34; PlayersCards +0x324, Bets +0x330, CurrentPlayerCardsIndex +0x354 (engine/CTCTavernGameCardBlackjack.h);
//   CTavernGameDef::Bet +0xec.
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
#include <stddef.h>
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardBlackjack.h"
#include "engine/CTavernGameDef.h"
#include "engine/GFIntToWideString.h"
CWideString CTCTavernGameCardBlackjack::GetBet()
{
    CDefPointer<const CTavernGameDef> pdef;
    GetDef(&pdef);
    CWideString ret = GetText(pdef->Bet);
    if (Bets.size() != 0 && CurrentPlayerCardsIndex >= 0 && (unsigned long)CurrentPlayerCardsIndex < PlayersCards.size())
        ret += GFIntToWideString(Bets[CurrentPlayerCardsIndex]);
    else
        ret += GFIntToWideString(BaseBet);
    return ret;
}