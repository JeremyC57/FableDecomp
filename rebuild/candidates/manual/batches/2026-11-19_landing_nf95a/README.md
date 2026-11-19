# 2026-11-19 landing nf95a: naming round 8, NF95 part 1 (18 rows, q11225..q11442 slice 3)

Round 8 (owner: "start the 3 naming agents on the naming queue"); see `batches/2026-11-19_landing_nf93a/README.md` for the round. NF95 stopped
for context after 14 of its 75 entries; this lands that part. A fresh NF95 continues from q11253 with its overlays reset to the merged
headers. Records: `naming/2026-11-19_agentNF95_part1/`.

## Entries (14 of 75 terminal)
- **done (10):** q11225, q11227, q11228, q11231, q11233, q11234, q11235, q11240, q11241, q11245 (inventory CTCInventoryBase / CCategory members
  off the `_Retail` stand-ins, `list<long>::push_back`, the CTCInventoryItem quest-persist accessor, the GlobalUpdateTCs container binder).
- **blocked (2, no grade spent; the continuation may retry):** q11236 (0x005BDEC1 needs binder2nd / const_mem_fun1_ref_t in
  stlport/fable_algorithm.h first), q11237 (0x005C150B needs the nf86 / nf88 one-definition scaffolding on 0x005BC450 first).
- **deferred (2):** q11229 0x00774D20 and q11414 0x00C77800, Account B's rows (hand4d).

## Rows (18): 9 self renames (RENAMES_applied.tsv, 12 file moves) + 9 caller respellings
Recheck from the repo after landing (`logs/recheck.log`): 16 RELOCATION_MATCH + 2 MATCH, every test PASS. Five rows were respelled inside
the local models they already had (0x005BD33C, 0x005BDB94, 0x005BDC08, 0x005BFB96, 0x005BF414; header attempts in NF95's work/, one C-error,
one 103 v 110 bytes); no new local model. No test or `// Flags:` line changed.

## Headers (6 opt-ins; merged three-way onto NF93's CCategory.h with `git merge-file`, no conflict)
CCategory.h `_HIGHLIGHTPREVIOUSITEM`; CTCBase.h `_GLOBALUPDATETCS` (protected static, Ego_d PUB32 spelling); CTCInventoryClothing.h and
CTCInventoryWeapons.h `_PROCESSBUTTONARELEASED` (15 asserts pinned with FABLE_STATIC_ASSERT_AT, one by hand); NInventory_CTCInventoryBase.h
`_GETPDEF_INLINE`; NPlayerGui_CDrawQuestInfo.h `_ORACLESYMBOLPUSHBACK`. Default path token-identical (hdrcheck2);
`lane_merge/assert_names_check.py` against the pre-round tree: 0 mismatches. Direct-includer recheck (1,191 rows, plus the 197 skipped ones in a second pass: all good): 1,189 good; 0x007E2A60 (known, on hold) and 0x0072A680 (pre-existing: the same with the 11-18 headers) DIFFER. `logs/includer_recheck.txt`.

## Checks
`naming/rebind.py`: removed 20 binding lines, added 11; `bindings_multi_check.py` at baseline (`NullCoord`); funclets 1,355 checked, 0 broken.

## Queued (`land1119nf95`, 5)
0x005BD0F7 HighlightPreviousItem and 0x005BCFE2 GetPreviousOccupiedSlot (self rows on the `CCategory_Retail` stand-in), 0x005BF414's own name,
0x005BF414's FindCategory binding of 0x005C08D3 (`find_if`), and 0x013BA8AC `CTCBase::GlobalUpdateTCs` (binders 0x0121E010, 0x00686DC0).

## Rule slips (NF95 NOTES)
Two Bash calls exited non-zero; q11236 / q11237 blocked without a grade; grades_used corrected on three rows; some section-2 old spellings
read from source, not a baseline compile.
