# Landing nf229 (Account A, 2026-11-22): the nf228 follow-ups (second-layer tree self rows, three 0x013B871C users)

Agent NF229. Hand-back: `pending/2026-11-22_nf229_handback/`. Entries q12608-q12618.

- **Recheck** (`logs/recheck.log`): 12 rows (11 changed + 0x0042BE50 test only), 1 MATCH (0x0042AE33) + 11 RELOCATION_MATCH, all PASS,
  no RELOC_CONFLICT. HEAD control the same (`logs/recheck_head_control.log`). Link (`logs/link_check.txt`): LNK4006 1 -> 0, LNK2005 0.
- **Entry 1, done:** 0x0042B8FC, 0x0042A264, 0x0042A237 (`_M_erase`), 0x0042AC9F, 0x0042ACEC, 0x0042AD10 (`_Rb_tree_base` ctors),
  0x0042AE33 (`_M_lower_bound`, with `FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22` as 0x004B9D00) and 0x0042AC0A (`_List_base<CObserver*>`
  ctor) become explicit instantiations of the header members under their binder rows' defines / pragma, each on its first form. Every
  address 2 -> 1 spelling; no absolute references to any of the eight. New self-contained tests on the shared headers. Catalog names
  are the STLport demangled names (files not renamed, as nf227 / nf228). Owned pick-any copies EQ (`logs/pickany_vs_retail.tsv`). 0x00599D5C's two NEQ copies (GetProfileNames 36 v 558, ~vector<CWideString> 41 v 49) are byte-identical NEQ at HEAD: existing debt, not a hold (nf203 ruling).
- **Entry 2, partial (0x013B871C):** a compile census of all 28 landed rows that reference the address. 0x00446720 and 0x00558170 read
  `CNewFrontendGameComponent::Get()->GetTextBank()` (the 0x0041CFB2 pattern), 0x00599D5C reads `Get()->GetLastActiveJoystickDeviceNumber()`
  (new opt-in). The `?g_` stand-in drops from 6 users to 3 (0x0041D0A8 and 0x0042EBB6 need header claims; 0x0059A238 not reached).
- **Entry 3, partial:** 0x0042BE50 has a new test on engine/NUISystem_CAction.h.
- **Header opt-in** (add-only, default branch only, no layout change): engine/CNewFrontendGameComponent.h
  `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_GETLASTACTIVEJOYSTICKDEVICENUMBER` splits the opaque pad around `LastActiveJoystickDeviceNumber`
  (+0x140). Name from FableWin.pdb only (Ego_r has the member, not the accessor); the retail read at 0x00599E1B is the second source.
  Proof: pin_asserts 0, assert_names_check 0 mismatches; equivalence over 57,022 landed rows (sources as at HEAD): 0 differing; the one user, 0x00599D5C, is rechecked above.
- **Coordinator rulings on NF229's questions:**
  1. The accessor opt-in: accepted on FableWin + the retail body (the nf228 IsRunning rule).
  2. 0x00446720 / 0x00599D5C land with their unchanged HEAD host-model tests; the test rewrite is queued (as nf228).
  3. 0x0042A8E0 as the pair's `??_G` (moving 0x00429E16's `~pair` off `@CAssoc_dtor@4`): queued for the next naming agent, not applied here.
  4. 0x0041D0A8 / 0x0042EBB6: header claims, queued; the `?g_` table line stays.
- **reloc_bindings** (`logs/TABLE_CHANGES.tsv`): 3 stand-in lines deleted, 3 added: the nf228-held `_M_erase` at 0x0042B8FC, `~pair` at
  0x00429E16 (replaces the deleted `Cleanup_00429e16` line; `@CAssoc_dtor@4` stays until 0x0042A8E0 moves), and the `?g_` line re-witnessed
  from 0x00599D5C to 0x0041D0A8 (`logs/reloc_bindings_readded.tsv`). No target gains a spelling.
- **Queued** (tag land1122nf229): 0x0042A8E0 `??_G`; the `?System_013b871c` (0x00541F20, 0x00542140) and public-static (0x00597BF2) stand-ins
  for 0x013B871C; 0x0059A238; the two header claims; 14 rows still on HEAD tests.
