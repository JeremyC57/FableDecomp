# 2026-11-10 landing s -- chunk 1003, the CTCInventoryClothing TU (5 of 7 rows)

Source: `pending/2026-11-10_linux_hand1003/`. vland --land: 5 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x005B3AEF OnInitialActivate (slot 6), 0x005B48A2 OnItemSelected (slot 33), 0x005B690B RefreshDescriptions,
  0x005B6A66 ConstructClothingListPC (an old 1511v1507 park), 0x005B76D1 ConstructScreen (slot 77, the console twin of the
  ConstructScreenPC landed in batch q; manifest / catalog name corrected from "vfunc_77"). Levers per row in the pending NOTES.
- Headers (opt-in, default paths unchanged): NUISystem_CMenuEntry.h _UNSELECTED_VISIBLE; CTCInventoryClothing.h _LISTS / _ACTIVATE /
  _REFRESH / _SELECT; NInventory_CTCInventoryBase.h _SELECTION; CCreatureDef.h _INITIALAPPEARANCEMODIFIERS; CThingObject.h _CREATE;
  CTCInventoryItem.h _ADDTOINVENTORY; CThing.h _CREATEDBYOWNER.

Parked: 0x005B3E4E GlobalUpdate DIFFER(538v543) (frame 0x54 v 0x58; needs shared animation headers); 0x005B9641
DrawPreparePrimitivesForRendering (manifest name corrected from "vfunc_11") not compiled: its body ends at 0x005B9BF0 (0x5AF bytes, not
1,867), the rest of the row is small STLport helpers (SPLITS.tsv; split them before authoring). Queued `land1011t`: 4 name fixes
(0x005B70B8 ConstructClothingList access / vector, 0x005BDFB1 SelectItemWithDefIndex and 0x005BCCF1 DeselectAll protected
NInventory::CTCInventoryBase, 0x005B406D SetThingAsWearingSelectedClothes takes CThing&).
