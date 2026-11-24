# NF368: port the CreateMapMarkers same-TU copies in 0x0060C900 / 0x0060EEB0 onto owner 0x005FF190 (Account A naming queue, 2026-11-24)

Result: both rows SKIPPED. Removing the strong same-TU body (so that the class declaration in engine/CTCInventoryMap.h line 239
is all the TU sees and the call binds the owner) moves each row's own leaf. No run dir is handed back (no ACCEPT row); no repo write,
no git; all locks released.

## Text tried (tools/mkvar368.py, var/N/, var/L/)
- Removed `void CTCInventoryMap::CreateMapMarkers(...) { ... }` (108 lines, identical in both rows) between the row's existing
  `#pragma auto_inline(off)` / `(on)` pair; the emptied pair is kept (HEAD pragma state) with one comment line. The file-head comment
  that said the body must stay was rewritten (one line, line count kept). Nothing else changed.
- The rows' behaviour tests define their own inline stub CreateMapMarkers in the fixture class, so they would not have needed a change.
- Control var/L: the same text with the removed body padded by comment lines so every later line keeps its HEAD number (rules out a
  `__LINE__` effect).

## Prechecks (evidence/pre*.log, pre_tests.tsv, pre_srcs_*.tsv)
- Landed tests, before the locks: clang fails only in VC7.1 headers (functional / ostream / istream), so D1: `cl /Zs` PASS for both.
- After the locks: clang PASS on both HEAD sources, owner 0x005FF190's HEAD source, both var/N and both var/L sources.

## Grades (pass.py, one path per row, `timeout 900` per launch; evidence/pass_p1..p5.log)
| row | HEAD | new (var/N) | control (var/L) |
|---|---|---|---|
| 0x0060C900 ConstructMapList | RELOCATION_MATCH | DIFFER(9633v9635) | DIFFER(9633v9635), leaf byte-equal to new |
| 0x0060EEB0 ConstructPCMapList | RELOCATION_MATCH | DIFFER(10236v10254) | DIFFER(10236v10254), leaf byte-equal to new |
| 0x005FF190 owner (read-only) | RELOCATION_MATCH | - | - |

## Where the leaf moves (tools/leafdiv368.py; evidence/leafdiv_*_HvN.txt, leafdiv_*_NvL.txt)
- The leaf did NOT inline CreateMapMarkers at HEAD: the call stays in both builds, and the external relocation target lists are equal in
  count and order (0x0060C900 362 = 362; 0x0060EEB0 381 = 381). Frame size is unchanged (`sub esp` 0x298 / 0x2B8).
- What changes is local frame-slot assignment, from the first frame store: 0x0060C900 at leaf +0x63 (an `lea ecx,[esp+N]` argument
  slot moves by 0x10), 0x0060EEB0 at leaf +0x29 (the first local stores move by 8 / 0x20); 1956 of 2725 and 2871 of 2913 aligned
  instructions differ (positional count of aligned objdump lines), and the leaves shrink by 2 / 18 bytes.
- Measured: only slot assignment moves. The cause is not measured; it is consistent with the visible-same-TU-callee effect (GOTCHAS
  chunk Q24 / w24B, hand3a 2026-11-19): with CreateMapMarkers' body in the TU, c2 can see that the callee does not leak the address of
  the caller's local passed by reference (`marker_regions`; `world_map.Get()` is a pointer value), and with a declaration only it must
  assume escape. Both rows' HEAD comments already recorded this (0x0060C900
  "without it DIFFER 9633v9635"; 0x0060EEB0 "without it the frame and colouring change"); the measurement after the nf365 GetDef swap is
  the same (0x0060C900 exactly the recorded 9633v9635).
- Symbols (evidence/cmm_symbols.txt): the new objects carry UND `?CreateMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAV?$vector@VCRegion@@V?$allocator@VCRegion@@@std@@@std@@@Z`,
  character-equal to the owner object's DEF, so the binding side of the port works; only the leaf gate fails.

Gates not run (the leaf gate failed first, so nothing lands): objsec, both pick-any checkers HEAD v new, reach flips, drop test, owner
link. Witness lines with these rows as witness (7, all witnessed by 0x0060C900, column 3 printed in full: ??0CUIState, ConstructRegionDescription, GetGuildSealRecallRegion,
GetRegionCount, GetTeleporterRegionName, _Rb_tree<long, pair<const long, CRegion>>::insert_unique, vector<CRegion>::push_back) need no
rewitness since nothing changes. The CreateMapMarkers table line's witness is 0x0061B610, not these rows.

No device was tried beyond the brief's text (NF364: these /O2 rows are closed for row devices; no pragma / noinline / reorder attempts).

## Rulings / next steps for the coordinator
- R1 (candidate, not built): the GOTCHAS landing-fz / nf96a form, "same-TU strong copy with the owner's body verbatim" (keep the body
  visible, but make it the owner 0x005FF190's landed text, `// Fidelity: same-TU strong definition of 0x005FF190 (retail TU-mate)`).
  It would keep the escape information the leaf needs and might make the copy EQ (4176). Caveats: (a) it is a strong duplicate of the
  owner (LNK2005 with identical bytes at best, a ruling item); (b) the owner's TU still uses its row-local GetDef template (nf365
  skipped 0x005FF190: the swap moves its leaf), which collides with these rows' GETDEF_GLOBAL_BODY (C2995 if both are present), so the
  owner text cannot be pasted unchanged (expected from NF361's C2995 measurement, not observed here); (c) the owner's spelling choices (loop guards, per-arm AddChild) change what c2 sees and could
  still move these rows' leaves. Outside this brief; queue it as a T3 note if wanted.
- Routing question: the object-order build mode (EXTRA_RULES 2026-11-24). The three functions share one retail object in both PDBs:
  `release\tc_inventory_map.obj` in ghidra_out/egor_obj_functions.tsv (CreateMapMarkers index 52, ConstructMapList 69,
  ConstructPCMapList 70) and `debug\tc_inventory_map.obj` in ghidra_out/fablewin_obj_functions.tsv. In one TU the owner's own body
  would be the visible one and no copy would be needed. But that mode was approved for residues proven to be compile order, and this
  residue is body visibility; and the owner's text differs from the copies', so whether the leaves then match is a prediction, not a
  measurement. Is that the route for these two rows, or a same-TU owner-text ruling (R1)?
- T3 nf365 0x005FF190 line: suggest appending "NF368: declaration-only port moves both leaves (0x0060C900 9633v9635, 0x0060EEB0
  10236v10254; same calls and frame size, only frame-slot assignment moves); routing (object-order mode or a same-TU owner-text ruling) open".

## Process
- Locks: 0060c900, 0060eeb0 (rows), 005ff190 (owner, read-only, released right after its HEAD build); all read `released`
  (evidence/locks_final.txt, evidence/locks.log).
- Stay-off check: no B claim / UNMATCHED / Codex reservation on the three addresses; the COORDINATION_CLAIMS line for NF368 is
  Account A's; the acctB package hits are 2026-11-11..12 header copies of long-landed rows. The T3 nf303 0x0060EEB0 line (column 2)
  is an unrelated binder census; my brief names the row explicitly (the prefix wins), and nothing changes it.
- No retail bytes in the hand-back (leafdiv disassembles rebuild objects only; the oracle stays in the lane).
- Tools (tools/): NF365 copies retargeted to NF368 (nf.py, pass.py, chain361.sh, leafrel.py, objsec352.py, pkrun352.py, pkcmp346.py,
  droptest.py, reachflip_coordinator.py, cmpcomdat.py, comdatsel.py, lvsize.py, leafcalls361.py, vland361.sh, mkbatch365.py,
  pre_srcs361.sh, pre_tests361.sh, zs_tests.sh); new: mkvar368.py (port text + line-preserving control), leafdiv368.py (first
  divergence of two builds of one leaf, relocation fields masked, call-target order).
