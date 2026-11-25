# NF413: the last seven baked-string rows, and retail's pooled "" modelled as data symbols (2026-11-25)

## Result
- `purity.baked_string_addresses` over rebuild/src/compiled: 7 rows, as expected: 0x00406F70, 0x004D4369, 0x004D5FF2, 0x004D66C3,
  0x008FD4BC, 0x00AA7230, 0x00ABEC70. All 7 are **ACCEPT** (batches/b1). After b1 the purity scan of the tree is empty.
- Second item: the six data symbols bound to 0x0122D70E. 4 live witness rows **ACCEPT** (batches/b2) + 6 table deletes; the
  last mention of one symbol, in a shared header, goes in batches/b3 (ACCEPT).
- Locks: the 11 changed rows are kept as NF413 (b1: 7, b2: 4). The b3 includers (004022b0 0049b460 009e9fd0) and the two
  stale-line witnesses (00431020 004e59e1) were only compiled, not changed, and were not locked. No HOLD, so nothing is released.
- At hand-back every orig/ copy is byte-equal to the repo (no HEAD drift), fable_boot.h included.

## b1: the seven rows (one changed line per literal; header-only SELF_SYMBOLS / TABLE_CHANGES)
| row | self (unchanged) | HEAD | new | test |
|---|---|---|---|---|
| 00406f70 | ?GetAutoSavePathName@CUserProfileManager@@QAE?AVCWideString@@XZ | 3 named constants `LIT_C/LIT_A/LIT_B` = casts of 0x0122E05C / 0x0122DE9C / 0x0122D70C | `"U:\\"`, `(const wchar16*)L"AutoSave"`, `(const wchar16*)L""` at the use sites, constants removed (NF410 r1) | already linked; now asserts all three texts and length -1 |
| 004d4369 | ?FableStatGetClassTypeName_5@@YIPAVCCharString@@PAV1@@Z | `(const char*)0x123bb40` | `"CTCDExperienceOrb"` | self-contained copy -> linked, asserts text |
| 004d66c3 | @StatGetClassTypeName_9@4 | `(const char*)0x123e6ec` | `"CTCDExplosiveTrail"` | copy -> linked |
| 004d5ff2 | ?StatGetClassTypeName_8@CCharString@@QAEPAV1@XZ | `(const char*)0x123dd2c` | `"CTCDRumble"` | copy -> linked |
| 008fd4bc | ?GetName@CAIStateGroup_MinionReactToCorpse@@SI?AVCCharString@@XZ | `(const char*)0x01288FF0` | `"SG_MINION_REACT_TO_CORPSE"` | old free stand-in copy -> linked nf406_b2 form (closes T3 nf406 0x008FD4BC test) |
| 00aa7230 | ?GetChunkDescription@C3DMeshFileXVertexDataChunk_Methods@@QBE?AVCCharString@@XZ | `(const char*)0x129e368` | `"Vertex Buffer Data"` | copy with text 0x40 -> linked (row's own ctor body: Data - 1 is the text) |
| 00abec70 | ?GetChunkDescription@C3DMeshFileVerticesChunk_Methods@@QBE?AVCCharString@@XZ | `(const char*)0x129ed24` | `"Vertex List"` | as 00aa7230 |

- Each row gains a two-line `// Literal (NF413, 2026-11-25)` comment naming the HEAD cast and the retail VA (NF411 form).
- 00406f70: purity flagged only 0x0122E05C. The two wide constants are cast through the row's `wchar16` typedef, which
  purity's `_BAKED_CAST` does not recognise; they are the same problem, so all three are fixed (LITERALS.tsv). The `U:\` path is
  spelled `"U:\\"` (retail 55 3A 5C 00: one backslash). L"AutoSave" is the same retail literal NF410 spelled in 00406c20.
- T3: the literal fix is independent of every open line (they touch the self's const-ness, the CreateSaveGame stand-in, the
  _Methods alias and the same-TU CCharString stand-in, never the literal). Land b1 first; T3.txt gives the per-line verdict
  (1 CLOSE, 7 KEEP, 2 AMEND) and the note for the three CTCD rows, which no T3 line names (they lack a `// Leaf:` line, so
  NF411's family filter missed them; their forwarder callers are on NF412's in-progress worklist).

## b2: the pooled "" as a data symbol
reloc_bindings.tsv binds six data symbols to 0x0122D70E (retail 00, NUL-preceded, .rdata; NF388: retail's one pooled ""):

| symbol | witness | state at HEAD | NF413 |
|---|---|---|---|
| ?g_shadowTag@@3HA (an `int`) | 004522f4 | `EnterShadowScope((void*)&g_shadowTag)` | `EnterShadowScope((void*)"")`; delete the line |
| ?g_FableEmptyText_0122D70E@@3QBDB | 00499a70 | `TextTag = g_FableEmptyText_0122D70E;` | `TextTag = "";`; delete the line; b3 removes the unused declaration from fable_boot.h |
| ?kEmptyDisplayNameTag@@3QBDB | 006bbf20 | `*pB != kEmptyDisplayNameTag` | `*pB != ""`; delete the line |
| ?GHO_ItemName@@3QBDB | 008902e0 | `ctor(GHO_ItemName, -1)` | `ctor("", -1)`; delete the line |
| ?g_profileTag@@3QBDB | 00431020 | stale: the row already spells `CheckCRC("")` | delete only (witness row unchanged) |
| ?g_emptyString_0122d70e@@3QBDB | 004e59e1 | stale: the row already spells `CheckCRC("")` | delete only (witness row unchanged) |

- Stale lines: both witness rows rebuilt at HEAD (batches/witness: RELOCATION_MATCH/PASS); `objdump -t / -r` of their objects
  shows only `??_C@_00CNPNBAHC@?$AA@` and no reference to either symbol.
- TABLE_CHANGES.tsv (b2): 6 deletes; `apply_table.py --dry`: `{'delete': 6} misses 0`, multi-target 1 -> 1, new [].
- After b2 + b3 a fixed-string grep of rebuild/src, rebuild/oracles and rebuild/include finds none of the six symbols (each
  checked by mangled and plain name). Tests: six tests (008cfe50 008cfeb0 00cb8690 004b6380 00521ae0 004ca230) keep a
  test-local `static const char g_emptyString_0122d70e[] = "";` (internal linkage, not the symbol); left alone.
- Tests: 004522f4 / 006bbf20 / 008902e0 were self-contained copies (006bbf20's copy still called the old `Compare` name,
  008902e0's used an invented "SILVER_KEY"); all three are now linked tests that define the row's callees under the row's
  spellings and assert the text is "" (mutant `"x"` FAILs them). 00499a70's test already linked; it now checks the text, not the
  address of its own `g_FableEmptyText_0122D70E` definition (removed).
- `_s_Empty` (0x0122D70C, wide L"", witness 004fdab0, also 004a05c0) is the same form but outside the six: reported, not touched.

## b3: fable_boot.h (an extra unit beyond the brief, so that the symbol's last mention goes too)
- One line deleted: `extern const char g_FableEmptyText_0122D70E[];` (line 113). The new header is batches/b3/include/fable_boot.h
  (HEAD copy in orig/include). batches/b3/includers.txt lists the three unchanged includer rows it was graded with (not locked).
- **Order: land b3 with or after b2, never before.** rebuild/integration/stage2_engine_boundary.cpp line 59 DEFINES
  `const char g_FableEmptyText_0122D70E[] = "";` and gets external linkage only from this fable_boot.h declaration; the bootstrap
  (build_bootstrap.ps1) links that boundary object with the 00499a70 row object (progress-display-text-mode.obj). Without the
  declaration the definition is an internal `const` (C++), so HEAD 00499a70's reference would go unresolved in the bootstrap link.
  After b2 nothing references it: delete line 59 in the same landing (prepared: batches/b3/integration/stage2_engine_boundary.cpp,
  HEAD copy batches/b3/orig/). Not build-verified: the boundary does not compile stand-alone in the lane at HEAD with the
  bootstrap's base options (C2373 `CWideString::operator=` redefinition at line 455, unchanged by this edit; batches/b3/boundary,
  logs in this note only); the coordinator's bootstrap build is the check. rebuild/backlog/staged_debakes/round4/
  UpdateShadowScene_65_00431020.cpp (a staged backlog file, not built) also still names g_profileTag; left alone.
- Includers: rows 004022b0, 0049b460, 009e9fd0 (direct), tests 004022b0, 009e9fd0, integration GFInitialise_ProgressPhase_test,
  and VisualBootCheckpoint_test via fable_visual_boot.h. None names the symbol.
- Gate: vland --recheck-landed of the three rows, HEAD and with `FABLE_HDR_OVERLAY=batches/b3/overlay`, at one LANDVERIFY_WORK
  path: same grades (RELOCATION_MATCH / MATCH / RELOCATION_MATCH, tests PASS), WARN sets equal, objects equal (comdat_cmp all EQ,
  `objdump -d -r` identical). A probe overlay that defines a symbol shows it in 0049b460.obj, so the overlay is really used
  (logs/vland_probe.log). The integration tests were not compiled (grep only).

## Gates (both batches; tools/runbatch.sh + tools/gates.py, NF410's, repointed)
- g1 vland --recheck-landed HEAD and new at one path (lv/batches_<b>_one, renamed _h/_n/_m): 11 x RELOCATION_MATCH/PASS ->
  RELOCATION_MATCH/PASS; WARN sets HEAD v new identical.
- g2 new logs: 0 LITERAL_DIFF / LITERAL_UNCHECKED / LITERAL_CHECK_ERROR, and `reloc_identity.literal_check` on each new object
  returns every LITERALS VA as equal (positive proof).
- g3 COMDAT gate: the only new sections are `??_C@` literals, every other section byte-equal once the literal relocation fields are
  masked; the `??_C` bytes equal retail at the VA, terminator included; addend 0. For b2 NF413 extended the gate with a SWAP case:
  a HEAD relocation to a data symbol that reloc_bindings.tsv binds to the row's LITERALS VA, replaced at the same offset by a
  `??_C@` relocation (addend 0 on both sides). Evidence: batches/<b>/evidence/comdat_<a>.txt.
- g4 purity `baked_string_addresses` [] on every new row and test, no VA left as a hex constant in code; no `__asm` / `naked` /
  `_emit` / `volatile`.
- g5 mutant (row literal's first character changed, new test): all 11 tests FAIL, every mutant object raises WARN LITERAL_DIFF
  (00406f70: 3). So every test observes the text (no q1 self-contained copies left in these rows).
- RESULTS old symbol == new symbol for all 11; recheck `--with-includers`: no includer of any changed row.
- coordinator/reserved_check.py (RESERVED_REFS="", so the second account's branch was not read: rerun with the default refs at
  landing): b1 reserved 0, mentions 0; b2 reserved 0, one mention (00499a70 declares callee 0x00497B60, codex_pass27; unchanged
  from HEAD). Not run: pick-any checkers and the /FORCE link census (no code change to pair; the new sections are `??_C` COMDATs).

## Findings
- **purity gap (RULINGS_NEEDED q1):** `_BAKED_CAST` misses casts through `void*`, `const void*` and typedefs. A scan
  (evidence/typedef_cast_scan*.tsv) finds 27 such casts to NUL-preceded printable .rdata strings: 00406f70's two (fixed here),
  0x00452B4D `(const void*)0x0122D70E`, and 24 StatGetClassTypeName rows 0x004D2A91 / 0x004D2B04..0x004D2D13 (`(void*)0x0123xxxx`,
  NF407's "unparsed shape"), which are still baked strings that the grade does not count. Sampled: 0x004D2C55 `result->m((void*)0x123a180, -1)` (the
  CCharString constructor argument, "CTCActionUseAcceptBoast") and 0x00452B4D `SomeGlobalFunc((const void*)0x122d70e)` (callee
  0x00404500, also bound as CPersistContext::CheckCRC(const char*)). The scan is a lower bound: it sees parenthesised pointer casts
  only, not `reinterpret_cast` or bare integer pushes.
- 0x004522F4 is probably mis-modelled (T3.txt): its callee 0x00404500 is also bound as CPersistContext::CheckCRC(const char*).
- The three CTCD rows have no `// Leaf:` line (vland grades them by the catalog leaf `StatGetClassTypeName`, a substring of the
  stand-in symbols).

## Layout
batches/b1, b2: rows.txt, RESULTS.tsv, CHECKS.tsv, UNITS.tsv, LITERALS.tsv, SELF_SYMBOLS.tsv (header only), TABLE_CHANGES.tsv
(b1 header only; b2 six deletes), rows/, tests/, orig/rows, orig/tests, head/ new/ mut/ (payload.json + oracle.tsv: lane input with
retail bytes, not for the repo), logs/, evidence/comdat_<a>.txt. batches/b3: include/fable_boot.h (new), orig/include (HEAD),
overlay/ (the same new header, as run), probe_overlay/ + probe/ (overlay proof), RESULTS.tsv, UNITS.tsv, logs/, evidence/.
batches/witness: HEAD build of 00431020 / 004e59e1 (stale-line proof). tools/: runbatch.sh, mkpayload.py, gates.py (NF410's;
gates.py adds the SWAP case). evidence/: typedef_cast_scan.tsv, typedef_cast_scan_classified.tsv.
