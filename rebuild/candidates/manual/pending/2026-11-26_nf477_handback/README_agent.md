# NF477 hand-back (2026-11-26): r467d, the CInputProcess family vftables (r459a)

No repo writes, no git, no `wineserver -k`. Base: the working tree as read (HEAD incl. the `_USE_ANSI_CPP` row define).
Both sides of every vland pair were compiled after that change (verify_and_land.row_defines).

## What was found
- 0x00488D10 CPlayer::InitInterfaces emits 68 `??_7` (66 family tables + 2 `CIntelligentPointer`), all 4 bytes at HEAD.
- Retail counted slots (RTTI locator at vftable - 4, then consecutive dwords at .text function starts; NF461 `retail_size` not used):
  9 for 54 tables, 10 for CInputProcessInventoryBase and its 10 screens (slot 9 GetPInventory, `_purecall` in the base),
  11 for CInputProcessPhotojournalCapture (HasFrameUpdate, FrameUpdate). evidence: `work/audit.tsv` (every slot, parent target, PDB spelling, landed Leaf).
- The base list is TU-wide: CAInputProcess's slots 1..8 (with a pure ProcessInput) change all 66 tables at once, so there is no
  one-class landable state of 0x00488D10. Pilot = a scratch TU (`exp/pilot.cpp`, CInputProcessBButtonExitMode only): 36 bytes,
  9/9 slot symbols = the PDB spellings of retail's targets, its `??_G` still 30 bytes and equal to 0x0048B6A0.
- Slot order: retail's vftables; names: the PDB virtual lists (Ego_r / FableWin; CAInputProcess's own Cleanup / PreparePrimitivesForEngine /
  Update / ProcessWhilePaused / IsInputEventToBeSentToThisInputProcess are FableWin-only, and the FableWin donor address order
  0x01AC7D70.. matches slots 2, 3, 5, 6, 7).

## Land order
1. `batches/b0_hdr` (69 headers, add-only). One family opt-in `FABLE_RETAIL_INPUTPROCESS_VTABLE`, declaration only:
   - engine/CAInputProcess.h: the base list after the existing AddToInterface / Init opt-ins (`#error` unless both are defined,
     and in the generated branch) + forward declarations (CEnginePrimitivePrepareInfo, CInputEvent, CProcessedInput, NInventory::CTCInventoryBase;
     all `V` in pdb_class_keys).
   - each of the 66 class headers + CInputProcessGameBase + CInputProcessControlSpirit (constructed by InitInterfaces, so it must not be abstract):
     one block at the end of the retail class declaring every slot whose retail target differs from the base table, with the PDB access
     letter; declarations an existing opt-in already makes are guarded `#if !(<that opt-in>)` (14 cases); `#error` in the 39 generated branches.
   - 173 moved FABLE_STATIC_ASSERT lines pinned `_AT(<old line>)` (evidence/pin_asserts.txt, 0 warnings).
   - g++ -E -P identical for all 201 HEAD includers (rows + tests, headers-only overlay, macros unset): evidence/hdr_equiv_direct.txt.
     No other header includes a family header (closure empty).
2. `batches/bD` (3 rows, ACCEPT, independent of b0_hdr): thin `??_G` rows of 0x00488D10 (nf424 pattern, like the 60 landed siblings) for
   0x0048B8D0 DebugControls (40 bytes), 0x0048B4E0 InventoryBase, 0x0048BC20 PhotojournalCapture: the owner TU's copies are byte- and
   relocation-equal to retail already at HEAD. CATALOG_CHANGES (3 underscore relabels: the STANDIN_LEAF WARNs go with them).
3. `batches/bR` (34 rows, ACCEPT, needs b0_hdr): respells of slot-target selves from free stand-ins (`?CAInputProcess_Init@@YIXXZ`,
   `?CInputProcessMain_ProcessWhilePaused@@YI_NXZ`, ...) to the PDB members on the shared headers; bodies unchanged
   (`ret` / `ret 4` / `ret 8` / `mov al,1 ; ret` / `xor al,al ; ret`; 0x00689C20 `jmp 0x006894C0` now names
   `?ProcessInput@CInputProcessInventoryBase@@UAE...` instead of the unbound `_ForwardTarget_006894c0`). Tests rewritten to call the member
   through the header (link against the row object); 0x00689C20's test checks the forward (this / event / input passed through).
   These land before the tables (nobody binds the new names until bT); r467b says "with, or after": please confirm the order (R-NF477-2).
4. `batches/bT` (HOLD r467b): 0x00488D10 + its 60 thin rows, built and slot-proofed, not landable yet (below). bD's 3 thin rows were regraded
   with it (RELOCATION_MATCH PASS against the new owner text) but are listed only in bD.

## Gates
- bR: vland HEAD v new 34/34 (33 MATCH + 1 RELOCATION_MATCH) PASS both sides; WARN 5 -> 0. objproof: each object only swaps its self
  symbol (old removed, PDB spelling added), .drectve unchanged in 34/34, no other symbol; 0x00689C20 drops `_ForwardTarget_006894c0` for
  the 0x006894C0 self. Link (34 HEAD v 34 new + 0x006894C0's HEAD object): duplicates 0 -> 0, unresolved 9 -> 8.
- bD: vland 3/3 RELOCATION_MATCH PASS both; WARN 5 -> 3 (STANDIN_LEAF, gone with the catalog relabel). The thin object's symbol table is the owner's.
  Link (3 stand-in objects + owner v owner): duplicates 0 -> 0, unresolved 12 -> 12 (same set).
- Census (evidence/fullcensus.tsv): 37 rows, 0 BAD. New-name collisions: none of the 37 old or new spellings is bound, self-listed or
  a Leaf at another address (reloc_bindings / self_symbols / every row Leaf); no other source defines `Class::Method(` for them; the
  HEAD objects of the other family emitters (0x00A0D290, 0x00A0D3D0, 0x00A0D410, 0x00687A30, 0x006A9140, 0x0048D390, 0x00486C20,
  0x00691AD0, 0x006894C0, 0x00687A70) define none of the new names (work/emitters_newnames.txt). Every retail reference to the 37 addresses is a vftable slot (322 `.rdata` dwords, all
  inside family tables; one `.text` hit at 0x0056D4BA is the rel32 of `call malloc`, not a reference). No landed source or test names any
  old stand-in except the row itself.
- Sandbox (shadow, overlay ov/inc, bD + bR): apply_table --dry 0 misses (the 0x00689C20 binding line is already in the table under
  0x0068BC70's witness, so TABLE_CHANGES is empty); bindings_multi_check 1 -> 1 (no new); descriptive_check 2030, 0 FAIL; s_unit_check 205, 0 failed;
  repo table unchanged; tracker dry run (--out in the sandbox): 33 rtti_identified -> agree_pdb, 4 agree_unverified -> agree_pdb, none worse.
- T3: 31 nf324 lines ("header does not declare X (add-only opt-in)") on bR rows are resolved by b0_hdr + bR (REL tags in RESULTS).
  No COORDINATION_CLAIMS hit.

## bT (HOLD) -- what it proves and what blocks it
- New 0x00488D10 = HEAD + the three defines + a header comment. objdiff HEAD v new: InitInterfaces 4142 bytes, bytes and relocations
  equal; the ONLY changed COMDATs are the 66 tables (4 -> 36 / 40 / 44 bytes); no symbol added or removed; .drectve unchanged;
  145 new undefined slot externals. slotproof: 66/66 tables at retail's counted slots, every slot symbol = the PDB spelling of
  retail's target (evidence/slotproof_00488d10.tsv, 0 BAD).
- The 145 new externals (evidence/bT_unresolved_classified.tsv): 71 resolve to HEAD rows, 34 to bR, 2 row-less (0x00691FD0
  DebugControls::ProcessInput, 0x0068DD00 UseEnvironment::ProcessInput), 1 `__purecall`; **37 name slot rows landed under misspelt selves**
  (body / local-model ports: 19 `QAE` ProcessInput rows on local class models, 10 GetPInventory (needs the NInventory screen types),
  0x00687DB0, 0x0068CB70 / 0x00690D20 `_Methods` stand-ins, 0x0068D830/40/90, 0x0068EB10, 0x00690AC0, 0x00689130, 0x006A4700 ...).
  r467b: they land with or before bT; listed in NAME_FIXES.tsv. bT's link gate was NOT run (only this classification of its 145
  externals); run it when bT is re-gated.
- Other emitters of the same names: 0x0048D390 (JumpingAndRolling `??_G`, local model) emits a 4-byte `??_7CInputProcessJumpingAndRolling`;
  and the owner TU's `??_GCInputProcessJumpingAndRolling` is 30 bytes v retail 48 (engine/CInputProcessJumpingAndRolling.h lacks the two
  list members' destructors), as is `??_GCInputProcessCombat` (30 v 40) -- both NEQ copies already at HEAD (debt), but they block turning
  0x0048D390 / 0x0048D680 into thin rows. Out of the 66 (listed, not fixed): ??_7CAInputProcess (0x00A0D290 8, 0x00A0D3D0 / 0x00A0D410 16;
  retail 36), ??_7CInputProcessGameBase (0x00687A30, 0x0048D390: 4; retail 36), ??_7CInputProcessControlSpirit (0x006A9140: 4; retail 36).
- bT vland HEAD v new (overlay ov/inc + ov/src/compiled, so the thin rows include the new owner text): 64/64 (61 bT rows + bD's 3) RELOCATION_MATCH PASS both
  (owner + 60 thin + bD's 3); WARN 11 -> 3 (the 3 bD STANDIN_LEAF, gone with bD's relabel). All 63 new thin objects carry the 66 full
  tables (no 4-byte family table left; slotproof of a thin object 0 BAD). T3 / claims: no line.
- To release bT: port the 37 rows in NAME_FIXES.tsv (or hold their tables), make 0x0048D390 a thin row once the JumpingAndRolling header
  models its two list members (its `??_G` must become 48 bytes in the owner TU), then re-gate (link: only row-less slot names unresolved).

## Batches
| batch | rows | verdict |
|---|---:|---|
| b0_hdr | 69 headers | land (add-only, inert for every HEAD includer) |
| bD | 3 | ACCEPT |
| bR | 34 | ACCEPT (needs b0_hdr) |
| bT | 61 (owner + 60 thin) | HOLD r467b |

## Files
NAME_FIXES.tsv (42 addresses: the 37 bT blockers + 0x0048CEA0 / 0x0048D680 / 0x00687DF0 / 0x006A91D0 / 0x006A9170 dtor and ControlSpirit
homes), VFTABLES_FIXED.tsv (66 tables built in bT), RULINGS_NEEDED.tsv, LOCKS.txt, work/audit.tsv (slot audit), tools/ (audit.py, genhdr.py,
geng1.py, slotproof.py, objdiff.py, xrefs.py, hdrequiv.sh, runpair.sh, sandbox.sh).

## Locks
101 taken as NF477 (none were owned), 98 held: 0x00488D10, its 60 thin rows + 0x00486C20 / 0x00691AD0 (header includers, untouched), the 3 bD rows,
the 34 bR rows; 0x0048CEA0, 0x00486C20 and 0x00691AD0 written back to released. Keep 0x00488D10 + the 60 thin rows for bT; release the rest when bD / bR land.
