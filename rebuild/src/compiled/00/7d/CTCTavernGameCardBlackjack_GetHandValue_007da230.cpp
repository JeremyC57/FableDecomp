// CTCTavernGameCardBlackjack::GetHandValue(vector<CCard*>* cards, bool includeFaceDown) -- blackjack hand value: sums the visible cards, then counts aces (11) down to 1 while the hand is bust
// Leaf: ?GetHandValue@CTCTavernGameCardBlackjack@@QAEJPAV?$vector@PAVCCard@@V?$allocator@PAVCCard@@@std@@@std@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA230, length 84 bytes (ret 8). No callees; `this` is not used.
// CCard (Ego_r.pdb, engine/CCard.h): Value +0x08 (an ace is valued 11), Turning +0x24, FaceDown +0x25. A card counts when face-down cards
// are included, when it is turning over, or when it is face up.
// Name fix (agentNF94, q11252): Ego_r.pdb 0x007D72F0 + FableWin.pdb 0x0275B500
//   ?GetHandValue@CTCTavernGameCardBlackjack@@QAEJPAV?$vector@PAVCCard@@V?$allocator@PAVCCard@@@std@@@std@@_N@Z (the landed 0x007DB6A0 binds it so);
//   the old row defined it on a local CRetailVector<CCard*> model (?..@@QAEJPAU?$CRetailVector@PAVCCard@@@@_N@Z). The body is now the inline
//   one in engine/CTCTavernGameCardBlackjack.h (overlay opt-in FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_GETHANDVALUE_INLINE), which retail's
//   HasPlayerWonHand 0x007DA900 expands once; this row only takes its address, so VC7.1 emits it here as a pick-any COMDAT (nf86 / nf88).
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
#define FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_SCORETEXT
#define FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_GETHANDVALUE_INLINE
#include <stddef.h>
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardBlackjack.h"
namespace {
// Fidelity: scaffolding, not retail -- takes the address of the header's inline GetHandValue so VC7.1 emits its COMDAT here.
long (CTCTavernGameCardBlackjack::*EmitGetHandValue())(std::vector<CCard*>*, bool)
{
    return &CTCTavernGameCardBlackjack::GetHandValue;
}
}
