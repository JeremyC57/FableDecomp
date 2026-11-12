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
#ifdef FABLE_RETAIL_CTCHERORECEIVEITEMS_GLOBALUPDATE
#include <stddef.h>
#include "engine/C2DBoxF.h"
#include "engine/CRightHandedSet.h"
#include "engine/CEnginePrimitiveHandle.h"
class CThing;
#endif
class CThingObject;
class CTCHeroReceiveItems : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_HERO_RECEIVE_ITEMS; }
    void GiveItem(CThingObject& item, bool flag);                        // 0x007CAB30
#ifdef FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUP
    // Retail opt-in (chunk 2039, row 0x006ACA30 CThingPlayerCreature::OnCollisionWithObject): Ego_r.pdb / FableWin.pdb
    // ?HasPickedUpItem@CTCHeroReceiveItems@@QBE_NJ@Z and ?SetItemAsPickedUp@CTCHeroReceiveItems@@QAEXJ@Z, out of line in retail at
    // 0x007CA800 / 0x007CB850 (both landed under these spellings). Declarations only: no layout change.
    bool HasPickedUpItem(long item) const;                               // 0x007CA800
    void SetItemAsPickedUp(long item);                                   // 0x007CB850
#endif
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
#ifdef FABLE_RETAIL_CTCHERORECEIVEITEMS_GLOBALUPDATE
    // Nested opt-in (w22 agent2057 scout, row 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate; needs _PICKEDUPITEMS,
    // FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_CRIGHTHANDEDSET, FABLE_RETAIL_CENGINEPRIMITIVEHANDLE + FABLE_RETAIL_CIVCOUNTEDPOINTER):
    // the rest of the Ego_r.pdb member list (struct_layouts_egor.tsv CTCHeroReceiveItems, names and order) at retail offsets (Ego_r less
    // 12: the 4-byte STLport list and 12-byte vector), as the landed ctor 0x007CA830 / dtor 0x007CA8E0 / OnKill 0x007CA630 /
    // PrepareToShowItems 0x007CA670 rows read them (PCurrentItem +0x1C, flags +0x24..+0x28, PContainer +0x29 byte-packed,
    // AmountThroughForAreaResize +0x34 .. BackgroundPrimitiveHandle +0xA4; sizeof 0xAC = the retail malloc size of the Alloc row
    // 0x004D57C7 UNVERIFIED). Members:
    //   ?GlobalUpdate@CTCHeroReceiveItems@@UAEX_N@Z (Ego_r 0x007C9230 / FableWin 0x02726CD0; vtable 0x0126EBFC slot 26 = 0x007CACB0),
    //   ?AddItemToInventory@CTCHeroReceiveItems@@AAEXAAVCThing@@_N@Z (private; Ego_r 0x007C8F40 / FableWin 0x02726C20; landed 0x007CABD0).
    // SendMessageW is the PDB spelling (windows.h maps SendMessage to SendMessageW in Lionhead's TU).
    virtual void GlobalUpdate(bool paused);                             // 0x007CACB0
    // FableWin.pdb inline accessors (no Ego_r.pdb / Fable.exe copy; a member-byte read in retail): ?IsReceivingItems@CTCHeroReceiveItems@@QBE_NXZ
    // (0x027290B0, 17 bytes / 9 instructions: its first COMDAT copy directly follows GlobalUpdate 0x02726CD0, so GlobalUpdate is its first
    // user -- retail 0x007CACB6 `mov al,[esi+25h] ; test al,al`) and ?IsDisplayingItems@CTCHeroReceiveItems@@QBE_NXZ (0x020E152B, first
    // copy in another TU, so whether GlobalUpdate uses it is UNVERIFIED; retail 0x007CB6BF `mov al,[esi+26h]`).
    bool IsReceivingItems() const { return ReceivingItems; }
    bool IsDisplayingItems() const { return DisplayingItems; }
private:
    void AddItemToInventory(CThing& item, bool persist);                 // 0x007CABD0
public:
    CIntelligentPointer<CThingObject> PCurrentItem;                      // +0x1c
    bool PersistCurrentItem;                                             // +0x24
    bool ReceivingItems;                                                 // +0x25
    bool DisplayingItems;                                                // +0x26
    bool ForceClear;                                                     // +0x27
    bool SendMessageW;                                                   // +0x28
    CIntelligentPointer<CThing> PContainer;                              // +0x29 (byte-packed pointer class)
    float AmountThroughForAreaResize;                                    // +0x34
    long FullSizeTimer;                                                  // +0x38
    C2DBoxF InitialAreaItemTook;                                         // +0x3c
    C2DBoxF OldAreaItemTaking;                                           // +0x4c
    C2DBoxF CurrentAreaItemTaking;                                       // +0x5c
    CRightHandedSet OldItemRHSet;                                        // +0x6c
    CRightHandedSet CurrentItemRHSet;                                    // +0x84
    CEnginePrimitiveHandle ItemPrimitiveHandle;                          // +0x9c
    CEnginePrimitiveHandle BackgroundPrimitiveHandle;                    // +0xa4
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
#ifdef FABLE_RETAIL_CTCHERORECEIVEITEMS_GLOBALUPDATE
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, Items) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, PickedUpItems) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, PCurrentItem) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, ReceivingItems) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, SendMessageW) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, PContainer) == 0x29);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, AmountThroughForAreaResize) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, InitialAreaItemTook) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, OldItemRHSet) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, ItemPrimitiveHandle) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroReceiveItems, BackgroundPrimitiveHandle) == 0xa4);
FABLE_STATIC_ASSERT(sizeof(CTCHeroReceiveItems) == 0xac);
#endif
#endif
