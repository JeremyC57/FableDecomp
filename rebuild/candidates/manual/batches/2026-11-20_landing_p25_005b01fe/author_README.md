# 005b01fe (p25P, Account B, 2026-11-21): 0x005B01FE CTCInventoryTrade::SellItemPC -- READY

- Grade: RELOCATION_MATCH (g.py 0 diff lines; strict vland `RELOCATION_MATCH PASS`, LANDVERIFY_STRICT_HEADERS=1, empty overlay).
  Leaf ?SellItemPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z, 2222 bytes, `ret 8`. Genuine plain C++, no flags, no asm.
- Header changes: none (hdr/ is empty, hdr.diff is empty). Existing opt-ins only. Newly flagged rows: none.
- Inferred / Bracket lines: none. No inline control (no Inline-budget line needed).

## What closed it (from p25O's 2222v2222, 6 lines: the Buy-total sum order at 0x005B0992)
p25O measured the mechanism: the sum's IND leaves are keyed by (address-temp number & 3). Retail's W,I,C order needs the Items
zeroing temp at 0 or 1 mod 4; ours was at 2. The fix is Ego_d's own statement list (egod_lines/005b01fe.txt):
- Ego_d 7381 / 7383 / 7389 / 7618 are all 22 B, the size of `p_text->SetValue(new_title, true, true)`. The lines before them,
  7382 (93 B), 7388 (84 B) and 7617 (78 B), reassign the function-scope `CText* p_text` (Ego_d type 0x2DE27):
  `p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild(new_subtitles));`, then `...sold_pentry->FindChild("PC_TRADING_SELL_TEXT")`,
  then `...buy_pparent->FindChild("PC_TRADING_BUY_TEXT")`. The drafts chained `static_cast<CText*>(...)->SetValue(...)` directly.
- Measured (tz.sh = zeroing temp numbers; g.py):
  - subtitle reuse only: 2222v2222, temps #94..97 = [2,0,2,0];
  - Sell-text reuse only: 2222v2222, [2,0,2,0];
  - both: RELOCATION_MATCH, temps #95..98 = [1,3,1,3];
  - adding the Buy-text reuse: still RELOCATION_MATCH (byte-neutral).
  All three are kept, as Ego_d has them.
- Ego_d 7410 (6 B, inside `if (NumSellableThings[sold_pparent].second <= 0)`) is still unidentified. No statement is placed for it.

## Other corrections in this batch
- `ptc_shop->BuyItem(item_def_index)`: 0x00774D20 is CTCShop::BuyItem in both PDBs (FABLE_RETAIL_CTCSHOP_BUYITEM). The drafts called
  SellItem, whose PDB address is 0x00774730 (landed 0x005A3581); that gave a vland RELOC_CONFLICT, now gone. See NAME_FIXES.tsv.
- Subtitle names: `anonymous namespace'::WEAPONS_SUBTITLE / CLOTHES_SUBTITLE / ITEMS_SUBTITLE. These are the PDB names; the landed
  staticinit rows 0x0121A9A9.. and every landed CTCInventoryTrade TU-mate declare them the same way. The drafts had g_CCharString_013b8b7x.
- The controlled creature now comes from the shared engine/CThingPlayerCreature.h, as landed 0x005AECC0 does. The drafts used a local
  `class CThingPlayerCreature : public CThing {}` model. Byte-neutral.
- `// Fidelity: same-TU strong definition of 0x0059C0C6 (retail TU-mate)`:
  - GetMenuBranchFromDefIndexPC is the body of batches/0059c0c6_report (the re-port of landed 0x0059C0C6 onto
    engine/CTCInventoryTrade.h), verbatim. Land that re-port first.
  - Compiled in this TU it also grades RELOCATION_MATCH against retail 0x0059C0C6.
  - It is needed: with a declaration only, the row is DIFFER(2222v2222), 82 lines.
- No inline control: the drafts' `#pragma auto_inline(off)` around the copy is dropped. Without it the row and the copy still grade
  RELOCATION_MATCH, because the copy is not inlined under this TU's /Os.

## Test (test/005b01fe.cpp): source-level, prints `005b01fe_TEST PASS`
- Why source-level: the row's anonymous-namespace externs mangle as ?WEAPONS_SUBTITLE@?A0x<hash>@@..., and the hash changes with the
  object path and per compile. A test cannot bind the real object, which is the same reason the landed TU-mate tests (0x005AECC0,
  0x005A6769) are source-level.
- How it works: the exact production body (generated from src/ by mutants/tgen/mk.py, verbatim) runs against controlled stand-ins
  with the row's interfaces. Results are compared with an independent model, over:
  - 4096 forced fixture combinations: branch type 0..3 (3 = no switch arm), category text present or null, quantity 1..4, entry
    left over or exhausted, branch rest;
  - 20000 seeded random draws: per-def prices, types, inventories, space and special flags, missing inventory components, buy lists
    with or without the sold def, gold readouts with a null entry and a decoy.
- What it checks: the counts, captions and subtitle mirrors, both deletion arms, the shop / payment / inventory calls, the buy-back
  recount (short-circuit query counts included), the insert under the buy map's size, the Buy caption, the gold readouts and the
  closing calls.

## Mutants (mutants/mutants.tsv, mutants.log; mutants/run.py applies each to the body and rebuilds the test): 39 / 39 KILLED

## NAME_FIXES.tsv
0x00774D20: the label CTCShop_SellHighlightedItem (labels_forge_autoname2) is ?BuyItem@CTCShop@@QAEXJ@Z.
