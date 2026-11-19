// CTCTavernGameCardBlackjack::GetPosition(long hand, long card) -- table position of a card slot: the layout's card offset plus the hand offset of the layout picked by the number of hands, scaled by the card size, at the table height
// Leaf: ?GetPosition@CTCTavernGameCardBlackjack@@QAE?AVC3DVector@@JJ@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retry 10b (B-12 sweep): the offset sum is bound to a const REFERENCE (`const C2DVector& offset = hand + card;`, the operator+ temporary): retail loads the card offset X right after addressing it, before the hand offset; a value local loads it at the fadd.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA760, length 164 bytes (`ret 0Ch`: hidden C3DVector result + 2).
// Ego_r.pdb `?GetPosition@CTCTavernGameCardBlackjack@@QAE?AVC3DVector@@JJ@Z`.
// Callees: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) @ 0x008F0B20; the def's vtable
// slot 1 on the last release.
// Layout: CTavernGameCardBaseDef (PDB offsets hold) CardWidth +0x2c, CardHeight +0x30, TableSeparation +0x60, CardOrders
// (vector<CCardPositionsDef>) +0x6c; CCardPositionsDef is 0x48 bytes in retail (0x3c-byte def base, 12-byte vector) with
// Offset (vector<C2DVector>) at +0x3c. Blackjack PlayersCards +0x324 (12-byte elements, hence the divide by 12).
// Shape: CardOrders[0] holds the per-card offsets, CardOrders[PlayersCards.size()] the per-hand offsets; their sum goes
// through the inline C2DVector operator+ into an 8-byte frame temporary, written hand + card (the argument, the card
// offset, is addressed and loaded first); the C3DVector constructor's arguments evaluate
// right to left (TableSeparation first); the CDefPointer destructor's null test is dropped (the def was dereferenced) and
// the release is the def's inline `DecRef(); if (GetRefCount() == 0) Destroy();`.
// Shared headers (NF96 q11339): engine/CTCTavernGameCardBlackjack.h (GetPosition declared by the existing FABLE_RETAIL_BLACKJACK_DEAL_PASS23,
//   the recipe of the landed DealCard 0x007DAEF0), CTavernGameCardBaseDef.h (CardWidth / CardHeight / TableSeparation / CardOrders),
//   CCardPositionsDef.h (Offset), C2DVector.h and CDefPointer<const CTavernGameCardBaseDef>, so GetCardDef 0x008F0B20 binds its PDB spelling
//   on CTCTavernGameCardBase (was ?GetCardDef@CTCTavernGameCardBlackjack@@QAEXPAUCDefPointer@@@Z through local CDefPointer / def / vector /
//   C2DVector / CCardPositionsDef / CTCTavernGameCardBlackjack models).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#include "fable_game.h"
#include "engine/C2DVector.h"
#include "engine/CCardPositionsDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#define FABLE_RETAIL_BLACKJACK_DEAL_PASS23
#include "engine/CTCTavernGameCardBlackjack.h"
C3DVector CTCTavernGameCardBlackjack::GetPosition(long hand, long card)
{
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    const C2DVector& offset = def->CardOrders[PlayersCards.size()].Offset[hand] + def->CardOrders[0].Offset[card];
    return C3DVector(def->CardWidth * offset.X, def->CardHeight * offset.Y, def->TableSeparation);
}
