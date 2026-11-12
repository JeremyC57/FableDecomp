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
#ifdef FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
class CPersistContext;
#endif
class CThingObject;
class CTCHeroReceiveItems : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_HERO_RECEIVE_ITEMS; }
    void GiveItem(CThingObject& item, bool flag);                        // 0x007CAB30
#ifdef FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS
    // Retail opt-in (chunk 1023, row 0x007CB8B0 CTCHeroReceiveItems::OnSerialise): the first two members (Ego_r.pdb order and names,
    // retail STLport sizes: list 4 at +0x0C, vector<long> 12 at +0x10, as the landed SetItemAsPickedUp 0x007CB850 and OnSerialise read
    // them) and vtable slot 1, ?OnSerialise@CTCHeroReceiveItems@@MAEXAAVCPersistContext@@_N@Z (vtable 0x0126EBFC slot 1 = 0x007CB8B0).
    // The members after PickedUpItems are not modelled yet (no sizeof).
protected:
    virtual void OnSerialise(CPersistContext& context, bool in_game);   // 0x007CB8B0
public:
    std::list<std::pair<CIntelligentPointer<CThingObject>, bool> > Items;   // +0x0c
    std::vector<long> PickedUpItems;                                     // +0x10
#ifdef FABLE_RETAIL_CTCHERORECEIVEITEMS_DISPLAYINGITEMS
    // Nested opt-in (wf13, row 0x00492973 CSubtitleRenderer::SetText): the next members in Ego_r.pdb order and names (Ego_r +0x28..+0x33,
    // retail -0xC as for Items / PickedUpItems): SetText sets DisplayingItems (`mov byte ptr [eax+26h],1`), Clear 0x00492BAB sets
    // ForceClear +0x27. Layout only.
    CIntelligentPointer<CThingObject> PCurrentItem;                      // +0x1c
    bool PersistCurrentItem;                                             // +0x24
    bool ReceivingItems;                                                 // +0x25
    bool DisplayingItems;                                                // +0x26
    bool ForceClear;                                                     // +0x27
#endif
#endif
#ifdef FABLE_RETAIL_CTCHERORECEIVEITEMS_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x007CBD70
public:
#endif
};
#endif
