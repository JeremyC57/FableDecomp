# 2026-11-17 landing nf89: name-fix round 7, NF89 (29 rows: the nf87 follow-ups, q11163..q11176)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes".

NF89 took the 14 entries queued at the nf87 landing. Brief, list and records: `naming/2026-11-17_agentNF89/`.

## Rows (STATUS.tsv, 60 = 51 locked + 9 claimed through the lock): 29 ported, 29 not-a-binder, 2 blocked
- **NInventory accessors (q11165..q11171):** 0x005BC4B2 GetSelectedCategoryIndex (was `_global::GetMember_dword0x70_005bc4b2`), 0x005BD1AE
  CCategory::GetCategoryThingAssociation (was `_global::TailJmp_GetPItem_005bd1ae`), GetUniqueItemCountInCategory, GetAllItems,
  GetNumberOfSlotsForCategory, 0x005BD054 CCategory::HighlightNextItem, and 0x0061A040 `CTCInventoryQuests::GetSelectedCategoryAssociation`
  (was catalogued `CTCInventoryBase::GetSelectedCategoryThing`), plus the CTCInventoryTrade / CTCInventoryExperience callers that bound them.
- **CTCAIScratchpad gossip maps (q11172 / q11173):** the static maps' dynamic initializers and atexit destructors at 0x0121F0E0 / 0x0121F120 /
  0x01229BE0 / 0x01229C40 (were `_static_init::g_tree_...` and `_unattributed::Database_Array_BuildDeleter*` stand-ins), spelled like the
  sibling CTCAIScratchpad `Gossip` initializer row.
- **`__introsort_loop` families (q11174..q11176):** 0x00A35490 / 0x00A73910 / 0x00A73A00 with their `__unguarded_partition` and `__median` helpers,
  on the stlport/fable_containers.h opt-ins.
- **Blocked:** 0x005BF77D GetResurrectionItemInInventory (DIFFER 324 v 334, register choice only; three forms; two candidate levers queued) and
  0x005A30A1 InitialiseBuyableItems (DIFFER 917 v 917: two `stock[key]=1` stores tail-merged, temporaries one slot off).

## Entries (APPLIED.tsv, 14): 12 done, 1 done-already (q11169), 1 blocked (q11171)

## Rows and checks
- **Section 1: 27 self renames (38 file moves).** Modules match their siblings' catalog spellings.
- **Headers (17 opt-ins in 6 files):** engine/CCategory.h, engine/CTCInventoryQuests.h, engine/CTCInventoryTrade.h, engine/_quarantine/CInventoryCategoryDef.h
  (11 asserts pinned), engine/CTCInventoryExperience.h, engine/NInventory_CTCInventoryBase.h. NF89 copied three blocks verbatim from NF90's
  running overlay (`FABLE_RETAIL_CCATEGORY_GETPDEF`, `FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF`, and an identical `CCATEGORY_ADDITEM`), so NF90's
  landing keeps one copy of each. Token-identical without the opt-ins for 46 includers; `lane_merge/assert_names_check.py`: 0 mismatches
  against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 29 rows graded from the repo after landing: 27 RELOCATION_MATCH and 2 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 56 binding lines and added 29; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 16)
NF89's NAME_FIXES.tsv (14; from a retail call / tail-jump scan of every renamed address and a binding sweep of landed callers,
`sweep_result.tsv`: 40 caller pairs, 32 right), plus two lines: a one-definition case (0x0059C6B0 keeps a same-TU auto_inline(off) strong copy
of GetCategoryIndexForTradeType, landed separately at 0x0059B6F4) and the 0x005BF77D port with its two untested levers.

## Rule slips
- One st.py note had backticks inside double quotes (bash tried to run them; no state change); rewritten from a quoted heredoc.
