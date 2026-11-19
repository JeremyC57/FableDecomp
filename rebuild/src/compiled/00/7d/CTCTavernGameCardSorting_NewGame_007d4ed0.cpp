// CTCTavernGameCardSorting::NewGame() -- base NewGame, round-limit game type, the round count = the def's card-order count, counter reset
// Leaf: ?NewGame@CTCTavernGameCardSorting@@UAEXXZ
// Retail: Fable.exe @ 0x007D4ED0, 108 bytes; CTCTavernGameCardSorting vtable 0x0126F5B4 slot 31 (NewGame; the catalog's
//         "vfunc_31").
// Callees: CTCTavernGameCardBase::NewGame() 0x008F0930 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(CDefPointer*)
//          0x008F0B20; the def's virtual slot 1 when its count reaches 0.
// Layout (retail): CTCTavernGame::NumRounds +0x24 (PDB +0x2c); CTCTavernGameCardBase::Counter +0x154, GameType +0x300;
//         CTavernGameCardBaseDef::CardOrders (vector<CCardPositionsDef>, 72-byte entries, retail 12-byte vector) +0x6c.
// Shape: `size()` divides the byte span by 72 (`imul 0x38E38E39 ; sar 4`); null-tested CDefPointer dtor with the def's inline
//        ReleaseTemporary().
// Shared headers (NF96 q11339): engine/card_sorting_types.h (the Ego_r.pdb CTCTavernGameCardBase / CTCTavernGameCardSorting, recipe of the
//   landed CardsMainGame 0x007D5070) with CDefPointer<const CTavernGameCardBaseDef>, so GetCardDef 0x008F0B20 binds its PDB spelling
//   (was a local CDefPointer struct and local CTCTavernGameCardBase / CTavernGameCardBaseDef / vector models). The +0x300 store is
//   GameType (Ego_r.pdb member order; the old model called it NumCards).
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
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardPositionsDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
void CTCTavernGameCardSorting::NewGame()
{
    CTCTavernGameCardBase::NewGame();
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    GameType = ECGT_ROUND_LIMIT;
    NumRounds = def->CardOrders.size();
    Counter = 0.0f;
}
