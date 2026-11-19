// CTCTavernGameCardSorting::EnterGame() -- base EnterGame, round-limit game type, create the pointer piece from the card def's model and place it at the game position raised by the pointer height
// Leaf: ?EnterGame@CTCTavernGameCardSorting@@UAEXXZ
// Retail: Fable.exe @ 0x007D4E30, 153 bytes (CTCTavernGameCardSorting vtable 0x0126F5B4).
// Callees: CTCTavernGameCardBase::EnterGame() 0x008F1E30; CTCTavernGameCardBase::GetCardDef(CDefPointer*) 0x008F0B20;
//          CTCTavernGame::CreateGamePiece(long model) 0x005EA9B0 (returns CThingObject*);
//          CTCTavernGame::PositionMesh(CThing*, const C3DVector& pos, const CRightHandedSet& rh, const C3DVector& offset, long, long)
//          0x005EABC0 (__fastcall global); the def's virtual slot 1 when its count reaches 0.
// Layout (retail CTCTavernGameCardBase, 0x18..0x24 below Ego_r.pdb): GamePositionRHSet +0xf0, GamePositionPos +0x108,
//          GameType +0x300, Pointer +0x140. Def (engine/CTavernGameCardBaseDef.h offsets hold in retail): PointerModel
//          +0x3c, PointerHeightOffset +0x48.
// Shape: GameType = ECGT_ROUND_LIMIT (1) precedes the def temporary (its store sits before the zeroing of the out pointer); the offset vector
//        is a 3-float temporary; the CDefPointer dtor is null-tested and calls the def's inline ReleaseTemporary().
// Shared headers (NF96 q11339): engine/card_sorting_types.h (the Ego_r.pdb CTCTavernGameCardBase / CTCTavernGameCardSorting of the landed
//   CardsMainGame 0x007D5070), CTCTavern(GameCardBase)Def.h, CDefPointer<const CTavernGameCardBaseDef>, so GetCardDef 0x008F0B20 binds its
//   PDB spelling ?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z (was a local CDefPointer
//   struct and local CTCTavernGameCardBase / CTavernGameCardBaseDef / CTCTavernGame models). The +0x300 store is GameType (Ego_r.pdb member
//   order; the old model called it NumCards).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
void CTCTavernGameCardSorting::EnterGame()
{
    CTCTavernGameCardBase::EnterGame();
    GameType = ECGT_ROUND_LIMIT;
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    Pointer = CreateGamePiece(def->PointerModel);
    CTCTavernGame::PositionMesh(Pointer, GamePositionPos, GamePositionRHSet, C3DVector(0.0f, 0.0f, def->PointerHeightOffset), 0.0f, 0.0f);
}
