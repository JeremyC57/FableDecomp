# 2026-11-17 landing nf91: name-fix round 7, NF91 (31 rows: the nf89 follow-ups, q11190..q11205; one row held)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes"; since then "just dont launch anymore naming agents. you can queue more just save the queue for later"
(NF91 was already running).

NF91 took the 16 entries queued at the nf89 landing. Brief, list and records: `naming/2026-11-17_agentNF91/`.

## Rows (STATUS.tsv, 50 = 41 locked + 9 claimed through the lock): 32 ported (31 land, 1 held), 16 not-a-binder, 2 blocked
- **C3DMeshVolume sort families:** `sort` for the `unsigned long`, `long` and `CCharString` keyed vectors, their `__insertion_sort` /
  `__unguarded_insertion_sort_aux` / `__copy_backward` helpers (eight claimed extras in one scripted pass), the CVectorMap `Sort` / `Update` rows
  (were stand-ins; coordinator respelled four modules to their catalog siblings) and `~pair<CCharString, C3DMeshVolume>` 0x00A6F390.
- **Inventory and hero rows:** CCategory::GetItemInSlotWithIndex 0x005BC9B0, GetNumberOfSlots 0x005BD1CF, HighlightNextItem binders,
  CTCInventoryTrade::ApplyHairCard 0x0059D9C2, InitialiseBuyableItems 0x005A30A1, GetAllItems 0x005BF482 binders, and others.
- **Held: 0x0059B6F4 GetCategoryIndexForTradeType** (RELOCATION_MATCH + PASS at hand-back; `held/`). Landing it under the PDB name would leave
  0x0059C747 calling the old `CTCInventoryTrade_Retail` spelling that no row defines, while 0x0059C6B0 / 0x0059C6DD / 0x0059C710 still define
  the PDB symbol strongly in their own TUs (a whole-image link already clashes there; `naming/2026-11-17_agentNF91/link/`). It lands with the
  q11204 one-definition fix and the 0x0059C747 respell (queued).
- **Blocked:** 0x005BF77D GetResurrectionItemInInventory (retail spills the element pointer, this build the offset `i*8`; neither suggested lever
  helped: `auto_inline(off)` gave 343 v 334, the visible GetAllItems body 324) and 0x0059C6B0 (q11204: DIFFER 47 v 45 on the shared header body
  in three forms; only the landed same-TU strong copy keeps `this` in edx across the call).

## Entries (APPLIED.tsv, 16): 12 done (q11201 via overrides.tsv), 4 blocked (q11200, q11202, q11205 on 0x005BF77D; q11204 on 0x0059C6B0)
REJECTED.tsv: q11190's note that 0x00A2F2D0 binds a sort-family stand-in (it binds no slice address).

## Rows and checks
- **Section 1: 29 self renames applied (52 file moves); 0x0059B6F4's line skipped (held).**
- **Headers (5 opt-ins):** CTCHeroAttachableAppearanceModifiers.h (`_ADDWITHREPLACE`), CHeroTitleDef.h (`_TYPENAME`; NF92's overlay carries the same
  block, keep one copy), CTCInventoryTrade.h (`_TRADECATEGORY_BODY`, used by the held row), NInventory_CTCInventoryBase.h (`_RESURRECTIONITEM`),
  CThingPlayerCreature.h (`_PREONKILL`). Token-identical without the opt-ins for 42 includers (`tokid.log`); no assert moved;
  `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 31 landed rows graded from the repo after landing: 29 RELOCATION_MATCH and 2 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 53 binding lines and added 31; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued for later (`land1117fu`, 6)
NF91's NAME_FIXES.tsv (5: 0x0042B687, 0x0059C747, 0x005BD33C access QBE to ABE, 0x005BDCDC, the 0x00774D20 BuyItem label) and the held 0x0059B6F4.

## Rule slips
- One Bash call ran a shell script through python3 (exit 1, no change); 0x005BF77D went through 8 failed forms (within the 15-grade budget).
- One generated Leaf line had an extra `@` (0x00A35670 / 0x00A35690), fixed.
