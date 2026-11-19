// CTCTavernGameCardBlackjack::HasPlayerWonHand() -- the current hand beats the dealer: dealer bust, higher total, or an unsplit two-card 21 against a dealer 21 of more cards
// Leaf: ?HasPlayerWonHand@CTCTavernGameCardBlackjack@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA900, length 168 bytes (plain `ret`). Ego_r.pdb
// `?HasPlayerWonHand@CTCTavernGameCardBlackjack@@QAE_NXZ`.
// Callees: CTCTavernGameCardBlackjack::GetHandValue(vector<CCard*>*, bool) @ 0x007DA230 (second call only).
// Layout: engine/CTCTavernGameCardBlackjack.h (retail = PDB - 0x20): DealersCards +0x318, CurrentPlayerCards +0x33c, HasPlayerSplit +0x368;
// CCard::Value +0x08 (an ace is 11), Turning +0x24, FaceDown +0x25 (engine/CCard.h).
// Shape: GetHandValue is inline. VC7.1 inlines the FIRST call (the dealer's hand; `includeFaceDown` = true folds the visibility test away,
// and the `dealer > 21` test merges into the bust loop's exit) and emits the second call out of line. The vector size() tests compile to
// `(last - first) & ~3 == 8`.
// Name fix (agentNF94, q11252): moved off the row-local CTCTavernGameCardBlackjack / CCard / CRetailVector<CCard*> models onto the shared
//   headers (CARDSMAINGAME declares HasPlayerWonHand and GetHandValue; the overlay opt-in GETHANDVALUE_INLINE gives the inline body the old
//   local model carried in class), so the out-of-line call binds Ego_r.pdb + FableWin.pdb
//   ?GetHandValue@CTCTavernGameCardBlackjack@@QAEJPAV?$vector@PAVCCard@@V?$allocator@PAVCCard@@@std@@@std@@_N@Z (was ..PAU?$CRetailVector@..).
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
#define FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_CARDSMAINGAME
#define FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_GETHANDVALUE_INLINE
#include <stddef.h>
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardBlackjack.h"
bool CTCTavernGameCardBlackjack::HasPlayerWonHand()
{
    long dealer = GetHandValue(&DealersCards, true);
    if (dealer > 21)
        return true;
    long player = GetHandValue(CurrentPlayerCards, true);
    if (dealer < player)
        return true;
    if (!HasPlayerSplit && dealer == 21 && player == dealer && DealersCards.size() != 2 && CurrentPlayerCards->size() == 2)
        return true;
    return false;
}
