#ifndef FABLE_ENGINE_CTCHERORECEIVEITEMS_H
#define FABLE_ENGINE_CTCHERORECEIVEITEMS_H
// CTCHeroReceiveItems (chunk 1011, row 0x005EB590 CTCTavernGame::GivePrize): declarations only, no members modelled (header debt:
// do not take sizeof or member offsets from this header). RTTI / PDB: a CTCBase thing component, interface type
// TCI_HERO_RECEIVE_ITEMS 203 (FableWin.pdb ?StatGetInterfaceType@CTCHeroReceiveItems@@SI?AW4ETCInterfaceType@@XZ; retail
// GetInterfaceType 0x007CBD40, landed; GivePrize tests UsingComponents word +0x38 bit 0x800 and looks up key 0xCB).
// GiveItem: Ego_r.pdb ?GiveItem@CTCHeroReceiveItems@@QAEXAAVCThingObject@@_N@Z (public, takes the CThingObject GivePrize just created by
// reference; the private ?AddItemToInventory@CTCHeroReceiveItems@@AAEXAAVCThing@@_N@Z takes a CThing&), landed at retail 0x007CAB30
// under that name (retail neighbours GiveItems 0x007CA9E0 / AddItemToInventory 0x007CABD0, also landed).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThingObject;
class CTCHeroReceiveItems : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_HERO_RECEIVE_ITEMS; }
    void GiveItem(CThingObject& item, bool flag);                        // 0x007CAB30
};
#endif
