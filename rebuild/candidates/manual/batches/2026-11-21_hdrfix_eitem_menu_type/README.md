# Header fix (Account A, 2026-11-21): CTCInventoryTrade::EItem_Menu_Type enumerators, one spelling repo-wide

Account B reported (COORDINATION_LOG 2026-11-20, p25O) that engine/CTCInventoryTrade.h spelled the enumerators two ways:
- `MENU_TYPE_*` under the FABLE_RETAIL_CTCINVENTORYTRADE_SELLITEM opt-in, used by landed 0x005AECC0 SellItem and B-reserved 0x005B01FE.
- `ITEM_MENU_TYPE_*` in the default path, used by the landed BuyItem family.

A TU holding both would not compile.

- Evidence: all four PDBs on hand (Ego_r.pdb, Ego_d.pdb, FableWin.pdb, the Anniversary 360 PDB) carry LF_ENUMERATE (0x1502) records
  `MENU_TYPE_CLOTHING` = 0, `MENU_TYPE_WEAPONS` = 1 and `MENU_TYPE_ITEMS` = 2. `ITEM_MENU_TYPE_*` is in none of them.
- Change:
  - The header now declares one enum with the PDB spelling; the `#ifdef` is gone.
  - Eight landed sources and four tests are respelled: 0x005A04F7, 0x005A3734, 0x005A4CDD, 0x005A5B31, 0x005A6769, 0x005A8175, 0x005AF64E,
    0x005B0AAC.
  - The test-local sentinel `ITEM_MENU_TYPE_NONE` in two tests is not the retail enum and stays.
- Enumerator names do not reach the object bytes. All 61 landed rows that include the header still match (`includer_recheck.txt`: 61 good, 0 bad).
