# Chunk 1009 notes: the CTCInventoryTrade TU (16 rows)

**Result: 8 of 16 rows match** (RELOCATION_MATCH). Four rows are parked in `differ/` and four are unworked.

| | Rows |
|---|---|
| Matched (src/ + test/) | 0x0059BB1B, 0x0059D270, 0x005A3734, 0x005A3E47, 0x005A79E6, 0x005A8175, 0x005A9A87, 0x005B0AAC |
| Parked (differ/) | 0x0059FC17, 0x0059FEE3, 0x005A6769, 0x005AD305 |
| Unworked | 0x005A29F4, 0x005A6CA1, 0x005A94BD, 0x005ACEDA |

**Grading setup.** Every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` exported. This covers `tools/gd.sh`, `gall.sh` and
c2prio. The overlay adds two declaration-only opt-ins; see `hdr/README.md`.

**Split row.** BuyItemPC 0x005B0AAC is 1518 bytes and ends at 0x005B109A; the catalog's 1793 is wrong. It is recorded in
SPLITS.tsv. `gall.sh` grades it through `tools/qs.py` (`QLEN`), and `finish.py` cuts its oracle at 1518 bytes.

## Identities proven (two sources each)

**Rows**

- **0x005A94BD = `OpenInventory`** (UAEXXZ). Evidence: CTCInventoryTrade vtable 0x01253034 slot 30. Slot 31 is CloseInventory 0x0059F304 and slot 86 is OpenInventoryPC 0x005AE86B.
- **0x005ACEDA = `ConstructActualInventoryPC`** (AAEXXZ). Evidence: the FableWin.pdb name, and its only caller is OpenInventoryPC 0x005AE86B. It builds the PC lists through ConstructSpecialShopListPC 0x005A8A5D and ConstructNormalShopListPC 0x005A6769. The catalog's "OpenInventory" is wrong, and so is the landed OpenInventoryPC's callee name "ConstructTradeScreenPC" (NAME_FIXES). ConstructActualBuyInventoryPC is 0x005AC10E.
- **0x005A3E47 = `ConstructBuyActionsList`** and **0x005A79E6 = `ConstructBuyActionsListPC`**. Evidence: the console vs PC strings, and twins of the landed sell rows 0x005A45F6 / 0x005A731F.
- **0x005A9A87 = `RefreshBuyPossibility`**. The catalog name "Inventory_PopulateBuyList" is a placeholder. Evidence: it is the console twin of the landed RefreshBuyPossibilityPC 0x005A887D, and the landed BuyItem calls it by this name.
- **0x005A3734 = `RefreshBuyPossibilityNumbers`** and **0x005A8175 = `RefreshBuyPossibilityNumbersPC`**. Evidence: each is the tail call of RefreshBuyPossibility / RefreshBuyPossibilityPC, plus the PDB names.
- **0x0059D270 = `AddWantedItemToListPC`**. Evidence: it ends `ret 10h` (four arguments), and the strings are PC_MENU_ENTRY_TEMPLATE / L"PC_MODEL".

**Callees**

- 0x005BF2ED is `IsSpaceInInventoryForItem` const.
- 0x005BF30D is `GetAmountOfItemsContainable` const.
- 0x005D99D0 is `GetInventoryType`. GetInventoryCategory is 0x005D97B0.
- 0x00772590 is `IsTitleShop`.
- 0x00487DD0 is `PeekPControlledCreature`.
- 0x0099BF30 is `operator+(const CWideString&, const unsigned short*)`.

**File-static CCharStrings.** The dynamic initialisers at 0x0121A93C.. give the strings at 0x013B8B60..7C:

| Address | String |
|---|---|
| 0x013B8B60 | Spec_Subtitle |
| 0x013B8B64 | Buy_Subtitle |
| 0x013B8B68 | Sell_Subtitle |
| 0x013B8B6C | Wanted_Subtitle |
| 0x013B8B70 | Weapons_Subtitle |
| 0x013B8B74 | Clothes_Subtitle |
| 0x013B8B78 | Items_Subtitle |
| 0x013B8B7C | Sell_Item_Desc |

The sources keep the landed `g_CCharString_013b8bXX` spellings.

**Not checked.** The landed `FinalRefreshSubtitles_0059d761` may really be FinalRefreshSubtitlesPC, which is in the PDB. It is not in
NAME_FIXES because I did not check it against a second source.

## Header debt

- The two overlay opt-ins in `hdr/engine/CTCInventoryTrade.h` need merging (see `hdr/README.md`).
- 0x005A3734 defines `GetMenuBranchFromDefIndex` 0x0059B79A in its own TU (noinline, `auto_inline(off)`), as landed BuyItem does. This needs the landed BuyItem's whole opt-in list: DEF_CLASS_BASE / CDEFINITIONMANAGER / CGAMEDEFINITIONMANAGER and others. With a trimmed list the build fails with C2118.

## Unmatched rows: measured residue and next step

### 0x0059FC17 AlreadyHaveSpecial: DIFFER(725v716)

- **Budget:** 4 grades plus 6 c2prio probes.
- **Residue:** VC7.1 makes `&def->BankIndex` a temp in edi (id 76) and spills `def` to `[ebp-10h]`. Retail keeps `def` in edi, the count in memory (`sar [ebp-0Ch],2`) and the index in esi (`xor esi,esi`).
- **Logic fixed over the prior:** the inner test is `t->BankIndex != -1 || def->BankIndex != -1`.
- **Next:** find a spelling that stops the address CSE, such as reading BankIndex through an inline const accessor or a by-value copy of the def's index.

### 0x0059FEE3 AddBuyItemToList: DIFFER(1552v1556)

- **Budget:** 1 grade plus 5 c2prio probes.
- **Residue (c2prio):** `this` (prio 17) gets ebx. The PeekTC inventory pointer (prio 13) is split between esi and memory, because its address goes to the inline PeekTC.
- **Retail:** spills `this` at entry, keeps the inventory in ebx (the shared `xor ebx,ebx` zero), puts the special flag at `[ebp-2]` and has a 0x64 frame.
- **Tried:** moving the declaration (3 places) and leaving it uninitialised. A hand-expanded PeekTC is blocked because `CThing::Components` is protected.
- **Next:** a CThing.h PeekTC variant that returns the pointer (not through `T**`), behind an opt-in. That needs a header change, which I did not make. 0x005A6CA1 AddBuyItemToListPC has the same structure (its prior is DIFFER 1681v1662) and should follow the same fix.

### 0x005A6769 ConstructNormalShopListPC: DIFFER(1336v1336)

- **Budget:** 6 grades.
- **Residue:** one operand order. Retail computes the wanted total as `mov edx,[esi+1F0h] ; add edx,[esi+1F4h] ; add edx,[esi+1ECh]`; ours loads +0x1F4 first.
- **Tried:** sum orders W+I+C, I+W+C and C+W+I; postfix `++` and `+= 1`; reset written as separate stores; a sequenced `t = W; t += I; t += C;`; reordered switch cases.
- **Next:** a GOTCHAS-style load-identity device. For example, read one counter through a member reference or an inline accessor so its load node is created later.

### 0x005AD305 ApplyTattooCard: DIFFER(620v620)

- **Budget:** 1 grade (the prior draft, flags `/O1 /Oy-`).
- **Residue:** a three-way allocation cycle in the appearance-morph block. Retail has morph in edi (spilled to `[ebp-10h]`), zero in esi and the cursor in ebx. Ours has morph in esi and zero in ebx.
- **Next:** c2prio on the morph / zero / cursor candidates.

## Unworked rows

- **0x005A29F4 ConstructSpecialShopList** (1709 bytes). There is no landed twin: the PC form 0x005A8A5D is only 521 bytes. It calls the shop GetName 0x00772610, the data-bank calls 0x009CC190 / 0x009C7D00 / 0x009C9070 / 0x009C7A40, the map clears 0x005B1C18 / 0x005B1C41, and AddBuyItemToList.
- **0x005A94BD OpenInventory** (1482 bytes) and **0x005ACEDA ConstructActualInventoryPC** (1067 bytes). They are near-twins. The prior for 0x005ACEDA is a local model at DIFFER(1113v1067) that does not use the shared headers. Start from the landed OpenInventoryPC 0x005AE86B / ConstructActualSpecialInventoryPC 0x005A8C66 opt-in sets.
- **0x005A6CA1 AddBuyItemToListPC.** It waits on the 0x0059FEE3 allocation fix.
