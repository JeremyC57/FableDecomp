# agent1002 header overlay (chunk 1002, 2026-11-10)
Grade with `FABLE_HDR_OVERLAY=$A/hdr`.

## engine/CTCInventoryWeapons.h
New opt-in `FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS`: declarations only (no layout change, default path byte-identical) of the
remaining Ego_r.pdb members of CTCInventoryWeapons, in their exact PDB spellings (egor_pdb_names.tsv / fablewin_pdb_names.tsv):
the virtual overrides OnInitialActivate (CTCBase slot 6) and RemoveItemFromInventory (slot 79), the public non-virtuals and the
private FinalConstructWeaponItemEntries(PC) / SetupActiveWeaponsOnCarrier (AAE).

## engine/NInventory_CTCInventoryBase.h
New opt-in `FABLE_RETAIL_CTCINVENTORYBASE_CREATEOBJECT`: the protected Ego_r.pdb member
`?CreateInventoryObject@CTCInventoryBase@NInventory@@IAEPAVCThing@@J@Z` (0x005BE8F3), declaration only.

## engine/CTCObjectAugmentations.h (new)
Retail CTCObjectAugmentations : CTCBase with StatGetInterfaceType (TCI_OBJECT_AUGMENTATIONS) and the two members the rows call
(GetFreeSlot const 0x00765D50, AttachAugmentationToSlot(long,long) 0x00768120), PDB spellings. No layout.

## engine/CTCCarrying.h
New opt-in `FABLE_RETAIL_CTCCARRYING_REMOVETHING`: Ego_r.pdb `?IsCarryingThing@CTCCarrying@@QBE_NABVCThing@@@Z` (0x006A1060) and
`?RemoveThing@CTCCarrying@@QAEXAAVCThing@@@Z` (0x006A19B0), declarations only.

## engine/CTCInventoryItem.h
New nested opt-in `FABLE_RETAIL_CTCINVENTORYITEM_DETAILS`: the static `__fastcall` `?GetDetails@CTCInventoryItem@@SI?AVCWideString@@J@Z`
(0x005D9630), declaration only.
(engine/CTCObjectAugmentations.h also declares the static `?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z` 0x00765F90.)

## engine/CObjectAugmentationsDef.h (new)
Retail CObjectAugmentationsDef : CSubDefClassBase, Ego_r member order and spelling up to InitialAugmentations at the retail offsets
(NumberOfSlots +0x28, InitialAugmentations +0x40: the STLport map is 12 bytes). StatGetTypeName literal 'CObjectAugmentationsDef'.
New nested opt-in `FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR`: the inline CPerItemData() (Ego_r ??0CPerItemData@NInventory@@QAE@XZ) as
retail expands it (zero vector + counts, QuestPersistent true, QuestPersistentRefCount -1, BestFishWeight unset).

## engine/CTCWeapon.h
New nested opt-in `FABLE_RETAIL_CTCWEAPON_MEMBERS` (retail branch): the Ego_r.pdb members after CTCBase (WeaponDef +0x0c .. Wielded +0x24,
sizeof 0x28, the generated default branch's offsets).

## engine/CTCInventoryItem.h
New nested opt-in `FABLE_RETAIL_CTCINVENTORYITEM_ENGINEGRAPHIC`: `?GetEngineGraphic@CTCInventoryItem@@QBEABVCEngineGraphic@@XZ` (0x005D92D0).

## engine/CTCCarrying.h (`FABLE_RETAIL_CTCCARRYING_REMOVETHING`, cont.)
Also `?IsCarryingThing@CTCCarrying@@QBE_NJ@Z` (0x006A10A0) and `?GetCarriedThing@CTCCarrying@@QBEPAVCThing@@J@Z` (0x006A1120).

## engine/CEngineGraphic.h (new)
Ego_r.pdb CEngineGraphic, 14 bytes (pack 1): BankIndex, AnimStep, RenderSizeX, AdditiveAlpha, Type.

## engine/CTCCarryable.h (new)
Retail CTCCarryable : CTCBase, Ego_r members (ForceFragility +0x0c, PCarryingTC +0x10, OffsetCoord.. +0x14, OffsetAmount +0x20; 0x24),
StatGetInterfaceType TCI_CARRYABLE, GetPCarriedByThing() const 0x00705170, GetCarrySlotActive() const 0x00705CC0.

## engine/NInventory_CTCInventoryBase.h (cont.)
New opt-in `FABLE_RETAIL_CTCINVENTORYBASE_ONINITIALACTIVATE`: `?OnInitialActivate@CTCInventoryBase@NInventory@@UAEXXZ` (0x005BD1EB).
