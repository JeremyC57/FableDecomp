# Landing nf204 (Account A, 2026-11-20): vector<CCharString>::_M_insert_overflow (1 row); returned families set aside for family passes

Agent NF204 (round 10) handed back early (context) after q6342 with 49 entries: 1 done-already (q12292), 10 blocked, 2 deferred and 38
open. It spent most of its context reading the census of entries that earlier agents had just returned. Its files are in
`naming/2026-11-20_agentNF204/`, with the HEAD probe of the 39 referencing rows of 0x00433530 (`work/probe.tsv`).

- 1 row lands: 0x00433530, RELOCATION_MATCH, test PASS. It is a leaf-only self rename onto the STLport spelling
  `?_M_insert_overflow@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@IAEXPAVCCharString@@ABV3@ABU__false_type@2@I_N@Z`.
  The row's local vector / CCharString / `__false_type` model is gone; it now uses fable_string.h and stlport/fable_containers.h with the
  existing NF41 opt-in FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE (plain inline gave DIFFER 280 v 316). Its test is rewritten on the
  shared headers, and its file moves from `stdvector_Reallocate_00433530` to `stdvector_Minsertoverflow_00433530` (no funclet includes it).
- Self-rename rule (advisor decision, owner delegated, as 0x00CB1890 at nf200): none of the 39 referencing rows binds the old self leaf.
  27 bind the new spelling and 12 still bind stand-ins, so the address narrows 8 -> 7 spellings and the 12 are a split to port.
- Table: the line naming 0x006FC8E0 as a witness of the protected (IAE) spelling was wrong. That row binds the public `QAE` spelling
  through a same-TU stand-in body (its source, and the agent's probe), so the line now records that spelling.
- Pick-any (`logs/pickany_check.txt`): the new COMDATs are shared-header inlines (`vector<CCharString>` `_M_clear` / `_M_set` / `size`,
  `_Construct`, `_Destroy`, and the cdecl `__uninitialized_copy` / `__uninitialized_fill_n` instances). None has a table line or a
  landed Leaf; landed 0x0044B120 / 0x0044B150 own the `__fastcall` instances, which are different symbols. Placement new was already in
  the HEAD object.
- Queue (advisor decision, owner delegated): entries that come back a second time with only ports left are now noted as blocked for a
  named family pass, not returned, so queue-order slices stop reopening them. FP-COMBATMAP (q4389, plus 0x006E9310 IsAttackerOn from
  NF204's NAME_FIXES) and FP-VECTORCCHARSTRING (q6332 / q6341 / q6333 / q6342) are started next. q4394 goes with q12191, q6315 / q6319
  with q11942, q5089 to a family pass, and q4456's constructor bodies to the decision batch. The 38 open entries are returned.
- Link (`logs/link_landing.log`): 0 LNK2005, as at HEAD. Recheck (`logs/recheck.log`): split gate 0 created; 40 good, 0 bad.
