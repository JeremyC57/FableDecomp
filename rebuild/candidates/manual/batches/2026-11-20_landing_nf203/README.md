# Landing nf203 (Account A, 2026-11-20): STLport CCharString map members and the CDataInputStream base (5 rows)

Agent NF203 (round 10) handed back early (context) after q12018 with 43 entries: 2 done, 6 blocked and 35 open. Its files are in
`naming/2026-11-20_agentNF203/`. The 35 open entries go back to the queue unclaimed. The six blocked ones were returned before (nf198), so
they are now noted for family passes: q11902, q11920 / q11923, q11936, and q11942 with q6315 / q6319. q11724 keeps its next form as a
hint (define FABLE_RETAIL_CIVCOUNTEDPOINTER).

- 5 rows land, all RELOCATION_MATCH, every test PASS.
- q12017: 0x00696340's self leaf is respelled in place to the member-template `_M_find<CCharString>` form its only binder 0x006946C0
  already binds; the address goes from two spellings to one.
- q12018: 0x01229280 was already right. Three binders move onto stlport/fable_map.h with FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY, each a
  self rename: 0x00696AF0 `_Rb_tree::clear` (was the July CTCCarryable_OnKill stand-in; test rewritten on the headers), 0x00696D10
  `~_Rb_tree`, 0x00696E20 `~map`. The tree-versus-map split follows the COMDAT run order. These are descriptive-by-library STLport
  spellings, not PDB names. Each address ends on at most one table spelling.
- q11902 (partial): 0x00993B20 moves onto engine/CDataInputStream.h and binds `??0CBaseClassNonCopyable@@IAE@XZ`; its test is
  rewritten on the shared headers.
  - Advisor decision (owner delegated): its HEAD object already defined `??_GCDataStream@@UAEPAXI@Z` / `??_GCDataInputStream@@UAEPAXI@Z`,
    which already differed from the owners 0x00996EA0 / 0x00996ED0 (q.py of the HEAD source: DIFFER 34 v 36). The port changes them to
    28 v 36. They are not new copies (symdiff lists only GetPosition as new), so the row lands and the header fix is queued as debt. The
    rule is recorded in EXTRA_RULES (pick-any section, nf203 line).
  - The one newly defined copy, `?GetPosition@CDataInputStream@@UBEKXZ`, grades MATCH at its owner 0x00405FA0 (q.py).
- Self-rename check: none of the four section-1 old leaves remains in the binding table or in any row.
- Link (`logs/link_landing.log`): 1 LNK2005 against 2 at HEAD (`logs/link_control_head.log`). `??1CRetailTree_erase00696900@@QAE@XZ` is
  gone, and no new one appears.
- Recheck (`logs/recheck.log`): split gate 0 created; gate and referencing rows 7 good, 0 bad. tablecheck: 0x00696900 2 -> 1.
- Queued: 0x005DCF70 is `CAnimationSet::Remove` (both PDBs; NAME_FIXES), port at DIFFER 299 v 273 after 3 forms.
