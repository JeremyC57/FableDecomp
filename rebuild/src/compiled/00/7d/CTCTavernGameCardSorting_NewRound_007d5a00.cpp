// CTCTavernGameCardSorting::NewRound() -- card-base NewRound, reshuffles, then deals the round: as many cards as the current card-order layout has positions are moved from the pack onto the table list
// Leaf: ?NewRound@CTCTavernGameCardSorting@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D5A00, 199 bytes (plain `ret`); CTCTavernGameCardSorting vtable 0x0126F5B4
// slot 32. Ego_r.pdb `?NewRound@CTCTavernGameCardSorting@@UAEXXZ`.
// Callees: CTCTavernGameCardBase::NewRound() 0x008F0960 (non-virtual base call; the landed body says NewRound, the manifest
//          says NewGame); ShuffleCards (slot 60, `call [eax+0F0h]`); CTCTavernGameCardBase::GetCardDef(CDefPointer<const
//          CTavernGameCardBaseDef>*) 0x008F0B20; GetCardOrder (slot 57, `call [edx+0E4h]`); the table list's
//          vector<CCard*>::_M_insert_overflow(CCard**, CCard* const&, const __true_type&, size_t, bool) 0x007D5D60.
// Layout (retail, as the landed constructor 0x008F1CD0): Cards +0x128, Pack +0x134, NumCards +0x2fc; CTavernGameCardBaseDef
//         CardOrders +0x6c (0x48-byte CCardPositionsDef records, Offset vector<C2DVector> at +0x3c).
// Shape: the card order is fetched into a local before the def is dereferenced (`def->CardOrders[GetCardOrder()]` loads
//        the def first); NumCards is stored from the layout's size() and re-read as the loop bound; the card is copied into a named local
//        (its frame word is the push_back argument) and pushed through the inline STLport push_back (null-tested placement
//        store, or _M_insert_overflow with the __true_type tag in the high byte of the frame word below); the def is
//        released through the def's inline `DecRef(); if (GetRefCount() == 0) Destroy();` with its null test.
// Shared headers (NF96 q11339): engine/card_sorting_types.h (the Ego_r.pdb CTCTavernGameCardBase / CTCTavernGameCardSorting, recipe of the
//   landed CardsMainGame 0x007D5070 / ShuffleCards 0x007D71E0), CTavernGameCardBaseDef.h (CardOrders), CCardPositionsDef.h (Offset) and the
//   STLport vector<CCard*> of stlport/fable_containers.h (push_back inline, _M_insert_overflow 0x007D5D60 declared only), so GetCardDef
//   0x008F0B20 binds its PDB spelling and the overflow binds the STLport spelling 0x007D71E0 binds (was a local CDefPointer struct, a
//   local CRetailVector and local CTCTavernGameCardBase / CTavernGameCardBaseDef / CCardPositionsDef models).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardPositionsDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
void CTCTavernGameCardSorting::NewRound()
{
    CTCTavernGameCardBase::NewRound();
    ShuffleCards();
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    long order = GetCardOrder();
    NumCards = def->CardOrders[order].Offset.size();
    for (long i = 0; i < NumCards; ++i) {
        CCard* card = Pack[i];
        Cards.push_back(card);
    }
}
