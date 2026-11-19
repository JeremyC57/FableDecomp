// CTCTavernGameCardPairs::NewRound() -- card-base NewRound, then deals the round (as many pack cards as the card-order layout has positions onto the table list), sets the timed game type, the round count from the def's layouts, and clears the counter and the alternate selected card
// Leaf: ?NewRound@CTCTavernGameCardPairs@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D70E0, 244 bytes (plain `ret`); CTCTavernGameCardPairs vtable 0x0126F6FC
// slot 32 (Ego_r.pdb `?NewRound@CTCTavernGameCardPairs@@UAEXXZ`). The CardSorting override 0x007D5A00 (landed) is the
// same deal without the trailing stores and with a ShuffleCards call.
// Callees: CTCTavernGameCardBase::NewRound() 0x008F0960 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(
//          CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20; GetCardOrder (slot 57, `call [edx+0E4h]`); the table
//          list's vector<CCard*>::_M_insert_overflow(CCard**, CCard* const&, const __true_type&, size_t, bool) 0x007D5D60
//          (this TU's instantiation); the def's virtual slot 1 when its count reaches zero.
// Layout (retail, as the landed CardSorting NewRound): Cards +0x128, Pack +0x134, NumCards +0x2fc; CTCTavernGame::NumRounds
//          +0x24 (PDB +0x2c); CTCTavernGameCardBase::Counter +0x154 (PDB +0x178), GameType +0x300 (PDB +0x324);
//          CTCTavernGameCardPairs::AlternateSelectedCard +0x318 (PDB +0x338); CTavernGameCardBaseDef CardOrders +0x6c (0x48-byte
//          CCardPositionsDef records, `imul 38E38E39h ; sar 4`).
// Shape: as the CardSorting body; the trailing stores follow the deal in source order (GameType first, the round count
//        from CardOrders.size(), then the two zero stores from the re-zeroed loop register); the def is released through
//        the def's inline `DecRef(); if (GetRefCount() == 0) Destroy();` with its null test.
// Shared headers (NF96 q11339): engine/CTCTavernGameCardBase.h / CTCTavernGameCardPairs.h (the 0x007D6870 recipe; overlay opt-ins
//   FABLE_RETAIL_CTCTAVERNGAMECARDBASE_NEWROUND (existing) / _ECARDGAMETYPE_VALUES and FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_ROUND_OVERRIDES),
//   CTavernGameCardBaseDef.h (CardOrders), CCardPositionsDef.h (Offset) and the STLport vector<CCard*> (push_back inline,
//   _M_insert_overflow 0x007D5D60 declared only), so GetCardDef 0x008F0B20 binds its PDB spelling (was a local CDefPointer struct and local
//   CTCTavernGameCardBase / def / CRetailVector models).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_ROUND_OVERRIDES
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_NEWROUND
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ECARDGAMETYPE_VALUES
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardPositionsDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardPairs.h"
void CTCTavernGameCardPairs::NewRound()
{
    CTCTavernGameCardBase::NewRound();
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    long order = GetCardOrder();
    NumCards = def->CardOrders[order].Offset.size();
    for (long i = 0; i < NumCards; ++i) {
        CCard* card = Pack[i];
        Cards.push_back(card);
    }
    GameType = ECGT_ROUND_LIMIT;
    NumRounds = def->CardOrders.size();
    Counter = 0;
    AlternateSelectedCard = 0;
}
