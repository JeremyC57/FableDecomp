# Landing nf246 (Account A, 2026-11-22): three wrong-`~list` families onto `~_List_base` (17 rows)

Agent NF246, entry 1 of its brief (the wrong DTOR_OUTOFLINE families of nf243 NOTES section 2); entry 2 handed on. Hand-back:
`pending/2026-11-22_nf246_handback/` (NOTES = per-family census and recipes; owners.tsv = the pk.py owner map for the `~list` thunks).
Rule used throughout (nf241 / nf243): a body with a 5-byte `jmp` thunk to it is `~_List_base`; the thunk is `~list`.
- **0x005127A0 list<C2DLineF>** (5 -> 4 spellings): self `~list` -> `~_List_base<C2DLineF>` (shared headers, new test); binders 0x00A82BE0 /
  0x00A83570 / 0x00A84470 NF241's swap (0x00A84470's leaf calls no list dtor; its pick-any `__pop_heap` copy now EQ 0x00A83570),
  0x00A81D60 / 0x00A820E0 local models -> shared headers, 0x00A84690 / 0x00A85240 / 0x00A852B0 in-place respells of their local list
  models. HEAD's 62-byte strong `~list` (clashing with 8 rows' 5-byte pick-any copies) is gone. Thunk 0x00514280 = `~list` (catalog-only,
  queued); 0x00A81C40 (a second jmp) undecided. 0x00517650 / 0x00A81030 / 0x00A83CC0 / 0x00A843F0 left on unidentified stand-ins (queued).
- **0x004AB4F0 list<CThingAICreature*>** (7 -> 5): self off the SortTreeRecursively stand-in onto `~_List_base<CThingAICreature*>`;
  0x005EB590 swap; 0x0051A2F0 ported onto fable_map.h (TREE_ERASE + _BODY already give erase(iterator)); 0x004AC060 made-up
  `ListNode_AllocateSelfLinked_RegionLoad` -> `list(const allocator&)` (2 -> 1). 0x0051BF90 blocked (one-parameter local list / map
  models; the table witness of the kept `??1?$list@PAVCThingAICreature@@@std@@` line); 0x00516F10 / 0x00519560 / 0x006C5E90 left on
  generated stand-ins.
- **0x00685BC0 list<const CThingAICreature*>** (3 -> 1): self onto `~_List_base`; 0x00682810 swap; 0x00986510's global-namespace list model
  -> std::list and its local CTCHeroStats::GetWivesInMarriageState respelled in place to the PDB signature; 0x006860C0 made-up
  `DoubleLinkedList_InitializeEmpty` -> `list(const allocator&)` (3 -> 1). 0x0057AE53's two binders now share one spelling (self row
  still a stand-in: queued).
- Files / Modules respelled for the five self rows (renamer); PassPatterns unchanged. reloc_bindings: 6 deletions (2 stale SList8 lines)
  + 1 replacement, per TABLE_CHANGES.tsv. pk.py: every new list copy EQ its retail address (with owners.tsv); remaining NEQ copies NEQ
  at HEAD. Link of the 17 new objects: 0 duplicates.
- **Recheck** (`logs/recheck.log`, landed tree, carrying nf244's CIEngine.h / CEngine.h): the 17 changed rows 17 RELOCATION_MATCH PASS; the 13 unchanged family
  referencers (incl. blocked 0x0051BF90) + 0x00463871 14 RELOCATION_MATCH PASS; 0 RELOC_CONFLICT.

## Coordinator rulings on NF246's questions
1. Land entry 1 as three units: yes (no address gains a spelling).
2. 0x00A85240's in-place respell defines a row-local `~_List_base<C2DLineF>` EQ to 0x005127A0 and links clean: accepted under the standing
   ruling (in-place local-model edits are accepted when the new COMDATs equal retail).
3. 0x00A820E0's test rewritten on the shared headers (tests the template body in its own TU, as the landed 0x00A82BE0 test does): accepted.
4. 0x004AC060 / 0x006860C0 as `list(const allocator&)`: accepted, on the caller binding plus the COMDAT-run position (nf245 ruling 2).
5. Catalog-only queue: the `~list` thunks 0x00514280 / 0x004AC080 / 0x006860E0, placeholder 0x00514260, 0x00A81C40 (undecided).
6. File / Module renames: done.  7. Locks: the 17 released with this landing.
Rule slip (one foreground `sleep 45`): noted; the next brief repeats the rule.
