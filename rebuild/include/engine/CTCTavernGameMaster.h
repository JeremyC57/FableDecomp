#ifndef FABLE_ENGINE_CTCTAVERNGAMEMASTER_H
#define FABLE_ENGINE_CTCTAVERNGAMEMASTER_H
// CTCTavernGameMaster (new, chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the component of a
// villager who runs a tavern game (TCI_TAVERN_GAME_MASTER 210). Ego_r.pdb (struct_layouts_egor.tsv, 16 bytes, no STL member):
// TavernGame CTCTavernGame* +0x0C (the row tests it: a game master with a game is a thing of interest).
// Needs FABLE_RETAIL_CTCBASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCTavernGame;
#pragma pack(push, 1)
class CTCTavernGameMaster : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TAVERN_GAME_MASTER; }
    CTCTavernGame* GetTavernGame() const { return TavernGame; }         // PLACEHOLDER accessor name (inline member read in the row)
private:
    CTCTavernGame* TavernGame;                                           // +0x0c
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CTCTavernGameMaster) == 0x10);
#endif
