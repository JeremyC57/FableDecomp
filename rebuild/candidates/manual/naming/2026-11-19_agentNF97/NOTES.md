# NF97 notes (round 8, 2026-11-19): ports of NF95's 29 held rows, then NF95's leftover ports -- PAUSED at a long-slice stop

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log):
- Entries (16): 8 done (q11276, q11292, q11320, q11325, q11331, q11335, q11376, q11465 = my binder 0x0062ABA0 only), 0 done-already,
  0 rejected, 2 blocked: port (q11273 rows left 0x009D1320; q11436 rows left 0x0055FC90), 0 deferred, 6 open (q11400, q11444, q11447,
  q11448, q11236, q11237: not reached for context).
- Rows (39 in rows.txt): 28 ported, 1 not-a-binder (0x005698A0, q11436's self row, Leaf already the entry symbol), 2 blocked
  (0x009D1320, 0x0055FC90, both 0 grades), 8 open (entry not reached).
- recheck_final.log: one vland --recheck-landed run over all 28 ported rows (rows/ as handed back): 2 MATCH + 26 RELOCATION_MATCH,
  every test PASS. selfcheck.log: `selfcheck: 0 problem(s)`.
- Grades: 42 in STATUS.tsv (the CGameEventDataPackage family used 32 of its 40; max on one row 4, 0x008645E0).
- No rows claimed outside rows.txt; no GameCODA rows touched; HELD.txt was empty.

## What the ports keep
NF95's names and bindings are unchanged in every port (RENAMES.tsv = NF95's section 1 / section 2 lines for these 28 rows, from
naming/2026-11-19_agentNF95_part2/RENAMES_all.tsv, evidence suffixed with the port; 9 section-1 self renames, 50 section-2 respellings).
Only the class models moved onto the shared headers, in the row and in its test. No new local model of a shared-header class anywhere.

## CGameEventDataPackage family (q11320; 22 rows)
- Transformer work/gedp_port.py (proven on 0x0049EAF0 / 0x0062C0E0 / 0x00629390 by q.py): delete the local `class CGameEventDataPackage`,
  define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE before every include, include engine/CGameEventDataPackage.h where the model was. 16 rows
  RELOCATION_MATCH in one payload (work/b1.log).
- The made-up local reader helpers (ReadBool / ReadChar / ReadUShort / ReadFloat / ReadLong) could not stay (they would be invented
  members of a header class). The three forced-inline ReadBool rows (0x006292D0, 0x00629330, 0x00629390) match with a direct
  `bool x; buffer.GetData(1, &x);` (work/gedp_port2.py). The other six DIFFERed that way (b1) and match with the FableWin.pdb typed
  readers as plain header inlines (new opt-in _TYPED_READERS, `// Inferred:` bodies; work/gedp_port3.py, b2.log): GetSLONG for ReadLong
  (0x00629DE0, 0x0062ABA0, 0x0062CE60), GetUWORD + GetFloat (0x00629C40), `GetUBYTE() != 0` (0x00629B10), `GetUBYTE() == true` (0x00629420,
  whose old helper returned `value == true`).
- 0x004165E8 also moved its new local CGameEvent / CGameEventPackage onto engine/CGameEvent.h (_DATAPACKAGE) and
  engine/CGameEventPackage.h (+ _GETEVENTS); 0x006294A0 its CGameEvent likewise. The other rows keep their PRE-EXISTING local CGameEvent /
  CWorld / CPlayer ... models (landed text; vland WARN SHARED_HEADER_AVAILABLE as before).
- 0x00628E80's test now includes the header (+ _GETUHUGE), defines GetData and links the row's GetUHUGE.

## The other held rows
- 0x00437870 (q11376): test rewritten on engine/CPlayerGui.h, links the row over raw storage (PDrawMiniMap +0x160 -> Drawing +0x18);
  it prints the catalog PassPattern unchanged (no comment quotes it).
- 0x00643BE2 (q11292 / q11331): test on engine/NPlayerGui_Graphics.h (GFTOSCREEN_CALLED) + the shared C2DVector, links the row.
- 0x0065F0FA (q11276): test on the row's headers; it defines the out-of-line callees the row binds (CIntelligentPointer<CThing> copy,
  ~CBaseIntelligentPointer, base ctor, GetPItem / SetPItem) and checks the member copy lands on the object (+0) before the argument is
  destroyed. It prints the catalog ONCREATE_BUYHOUSE_OK (NF95's test printed `0065f0fa_TEST PASS`, which the catalog pattern would FAIL).
- 0x006A7610 (q11325): test drops its CThing / CThingCreatureBase / CThingAICreature models (opaque addresses), defines
  NCreatureAI::IsAwareOfThing (from engine/NCreatureAI.h) and links the row. The filter class is the row's own (no shared header).
- 0x008645E0 (q11325): row and test on engine/CTCPerceiveThing.h. `_CANSEEORSMELL` sits inside `_AWARENESS`, so both are defined (the first
  form failed C2039). The test stays a self-contained model (the row's callee is in an anonymous namespace, so the test cannot link it).
- 0x007554C0 (q11335): the local base / vector / tree models replaced by the landed dtor 0x00755520's header set
  (CEnginePrimitiveMorphedStaticMesh.h EYEDRAW_PASS24 over CEnginePrimitiveStaticMesh.h) + engine/EEnginePrimitiveType.h; the body is
  `Type = ENGINE_PRIMITIVE_MORPHED_STATIC_MESH;`. RELOCATION_MATCH first grade. Its landed test is unchanged (self-contained stand-in model
  of no shared-header class).
- 0x009D1320 (q11273): blocked: port, 0 grades (inventory of existing and missing opt-ins in STATUS). The landed callee row 0x009D1260
  itself still models CBankFile locally.

## Self renames and funclets
9 section-1 rows (NF95's renames). 8 move their file slugs at landing (0x00628E80, 0x006294A0, 0x00437870, 0x00643BE2, 0x0065F0FA,
0x006A7610, 0x007554C0, 0x008645E0); 0x004165E8 is a leaf-only rename (catalog name unchanged). No EH funclet row includes any of the 28 changed rows' files (grep of rebuild/src/compiled
for each file name: 0 includers).

## Tests changed (6; this reverses NF95's notes that the 0x00437870 / 0x0065F0FA PassPatterns must change)
rows/00628e80_test.cpp, 00437870_test.cpp, 00643be2_test.cpp, 0065f0fa_test.cpp, 006a7610_test.cpp, 008645e0_test.cpp: every one
prints its catalog PassPattern (build_candidates.ps1), so no PassPattern changes at landing.

## Descriptive names
No new made-up names. The typed readers are FableWin.pdb names. CThingFilter_IsAICreatureAwareOf (0x006A7610) is NF95's name, unchanged.

## NAME_FIXES.tsv (3)
0x009F18F0 GetData (catalog-only callee, catalog label CircularBuffer_Read), 0x009F16E0 GetNoEvents, 0x009F16D0 GetEvent (catalog labels
propagated; both PDB name sets checked).

## Header debt / notes for the coordinator
- 0x0049EAF0's header comment says the PDB spelling is `IAEXPBVCGameEvent`; both name sets (Ego_r 0x004A8150, FableWin 0x01B479A0) say
  `?HandleMoveHeroToRegionGameEvent@CWorld@@QAEXABVCGameEvent@@@Z`, which is the row's Leaf and reloc_bindings. The comment is wrong,
  the name is right: no fix needed.
- Overlays: hdr/README.md. orig/ is byte-equal to the repo at hand-back.

## For the resume
- q11400 (0x0063E950): see APPLIED (header-port inventory; NF95's draft only declares the template, the self row's header set makes the
  body visible).
- q11444 / q11447 / q11448 (0x005BF414 header port, 103v110 register choice): next form is an include-order permutation.
- q11236 (0x005BDEC1) / q11237 (0x005C150B): NF95's prerequisites unchanged.
- q11436 (0x0055FC90): blocked under the dozen-models rule (STATUS lists the classes to move).

## Rule slips
- One foreground `sleep 1` before reading the b1 log (the brief says no foreground sleep).
- Two rows blocked with 0 grades (0x009D1320, 0x0055FC90), on the dozen-local-models rule and the inventories in STATUS, not on attempts.
- Queue order: q11436 was recorded blocked without an attempt and the later ports (q11400, q11444 / q11447 / q11448, q11236 / q11237) were
  not reached; they are `open`.
- STATUS / APPLIED for the 22-row family were written after the batch grade, not after each row (one payload); the hand-back files were
  started only after the first payload.
