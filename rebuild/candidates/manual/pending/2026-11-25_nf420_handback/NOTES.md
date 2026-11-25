# NF420 hand-back (2026-11-25): the baked_symbols census rows (pooled string literals) + NF418's 9 held float-pool rows

## Result
- **46 rows ACCEPT** in two batches, locks kept as NF420. **No HOLD rows** (no HOLD batch folder).
  - **s2** (strings) 37/37 ACCEPT: the HEAD census (39 at f12740f5fd, 36 after NF417 landed 006285f0 / 00771100 / 007df2c0, dropped from this
    batch) plus 0048f881 (its file-static s_TextGuiSkipScene = L"TEXT_GUI_SKIP_SCENE" 0x01238198 has no table line, so the census cannot see it).
    50 table deletes (46 `?..@@3QB[DG]B` pool stand-ins + `_s_Default`, `_s_newLine`, `_s_backslash`, `_s_jpgMask`). Units (land whole):
    0040f7e0 + 00686240 (g_msgUnknownSymbol_0122dfe8), 0057f78f + 006c78f0 + 006d3880 + 00755a30 ("(" "," ")"), 00497a60 + 009bc900
    (s_newLine), 00d1dff0 + 00d1eae0 (g_knotholeGlade); the rest single.
  - **f2** (floats, NF418 f1's HOLD) 9/9 ACCEPT: 00437500 0054ecb0 006448a4 006a98f0 006e7340 00753ba0 008909f0 00891720 00895a10. Row edits
    are NF418's (rows unchanged from $S/w22/agentNF418/batches/f1/rows); what was missing was a linked test, now written for all nine. 8 deletes
    + 1 rewitness (`?g_zero@@3MB` -> 00732d00, which still reads it; 006cfec0 only declares it).
- Census: see "Census" below. apply_table --dry (current table, after NF417): s2 {'delete': 50}, f2 {'delete': 8, 'rewitness': 1}, both
  together {'delete': 58, 'rewitness': 1}: misses 0, multi-target 1 -> 1, new []. reserved_check (RESERVED_REFS=""): 0 for both.
- orig/rows and orig/tests are re-checked byte-equal to the working tree after NF417's landing (none of my rows was touched by NF417);
  head/payload.json is the vland HEAD run on those copies.
- Tests: every other landed test that mentions these names defines its own `static const float g_fNoOpinion_0122dedc` /
  `g_noFishWeight` (checked: no test only declares them), so the f2 deletes break none.
- No header declarations to remove (no fixed symbol is declared in rebuild/include). SELF_SYMBOLS.tsv header only (no self symbol changes).
  NAME_FIXES.tsv: 1 line (00d1eae0's made-up slot name SetThingFlag238 = EntitySetAsMirroringHeroEnemyRelationsWhileFollowing 0x00895650).
  RULINGS_NEEDED.tsv: none.

## The fix (rows)
- Every use site spells retail's exact bytes (text from Fable.exe: lit418.py; width from the parameter type), e.g. `L"\\"` / `L"*.jpg"`
  (00581b33), `";\r\n"` (00522600), `"\""` (004ca230), `L"%1"`, `L"%"` / `L"%d"`. LITERALS.tsv lists each (address, VA, width, C spelling);
  batches/s2/mkrows.py is the spec. The extern / static declaration is dropped and a `// Literal (NF420, 2026-11-25): ...` block in its place
  names the old identifier, its table line and VA, and the literal (tools/respell420.py: code-only replacement, comments and strings untouched).
- Evidence the strings are /GF-pooled literals, not named arrays: NF418 CLASSIFICATION.tsv (kind=string, verdict fix) and strrefs (VAs
  referenced from several 64K regions). None of the 50 deleted symbols (nor f2's 9) appears in ghidra_out egod_carved_names /
  egod_pdb_names / egor_publics_noproc / fablewin_publics_noproc, so none is a PDB data symbol, including the single-region ones
  (CHUNKED_FILE_ID ">>>>", the quest names). "1.2.1" 0x0129CFE0 is shared by the 00a3 and 00c0 regions (zlib's
  compress2 / zlibVersion) -- pooled; the copy at 0x012AE2D8 has no .text reference.
- 009bd910: the test's fixture `g_emitter` became `gEmitter` (EXTRA_RULES: no identifier containing `_emit`).

## Tests (linked, mutant-failing)
- HEAD tests were self-contained copies, so a mutated literal passed them. Each new test DECLARES the row function (does not define it) and
  links the row object: callees are defined under the row's mangled spellings, or as thiscall-compatible `__fastcall` fixtures bound by
  `/alternatename`; a non-public row is called through an `extern "C" __fastcall NF420_Row` shim aliased to the row symbol; every other
  external is aliased to a numbered trap (prints FAIL, exits 3) or to a zeroed data buffer. Script rows share one fixture
  (CGameScriptInterface vtable of 918 slots, CScriptThing {vptr, PImp.Data, PImp.Info} with a shared count block).
- tools/mktest.py builds tests/<a>_test.cpp from tcore/<a>.cpp, links it with the new object (link /nologo, LINK=bufferoverflowu.lib for the
  /GS cookie, test /Od /W3) and traps whatever is still unresolved; the catalog pass pattern from head/payload.json must print.
- Mutation control (mkpayload mut: first character of each literal changed; 0.0f -> 0.5f): all 46 tests FAIL on the mutant, PASS on new.
  The failure is the test's own assertion message for every row (evidence/mut_stdout.txt: each mutant exe rerun, last stdout line), never a
  trap or link error. 00aa79c0's mutant makes the row throw (retail rejects a bad file id); its test now catches that and prints
  `FAIL threw=1` instead of aborting. In rows with several literals every literal appears in an asserted expected text (e.g. 0057f78f
  asserts both "(..,TRUE)" and "(..,FALSE)", 00cb8690 all four markers, 006448a4 alpha 127 = 255.0f * 0.5).
- One gotcha (GOTCHAS candidate): VC7.1 link `/alternatename` silently does nothing for a very long symbol (the 319-character std::sort
  mangled name in 00cb8690): define the function as a C++ template specialization instead.

## Gates (both batches)
- vland HEAD and new at one LANDVERIFY_WORK path: RELOCATION_MATCH/PASS -> RELOCATION_MATCH/PASS for all 46; 0 LITERAL_DIFF / LITERAL_UNCHECKED.
- Section compare (cmpcomdat, evidence/comdat_<a>.txt for s2): only new `??_C@` / `__real@` sections; bytes equal with relocation fields masked.
  00686240: its "Unable to find symbol " literal sits in a same-TU non-leaf section (the row keeps a TU-mate copy of 0x0040F7E0), so
  literal_check reports it MISSING from the leaf; gates415.py accepts that only when the COMDAT SWAP equals retail (it does).
- purity [] / widened [] / stray [] for every row; baked_symbols source pass [] for every new row (batches/*/evidence/sourcepass_new.txt); object pass: s2 37 hits at HEAD -> 0 new,
  f2 0 / 0 (floats are not in the string key) -- batches/*/evidence/objpass_*.txt.
- /FORCE link with the definers (evidence/link_s2.txt, link_f2.txt): duplicates 0 / 0, NEW_UNRESOLVED none. s2: 46 GONE_UNRESOLVED, RESOLVED_TO_GONE
  00000260H; f2: 6 GONE_UNRESOLVED, RESOLVED_TO_GONE 00000017H (the map's .data length artefact, as NF418's 00000074H).

## Model debt seen (T3.txt, not fixed here)
- 00522600: local CRetailVector / GrowVectorStorage_00436ac0 stand-in (its InsertGrow keeps only the pushed element; the test uses one thing).
- 00686240: same-TU copy of CPersistTraits<float>::TransferIn 0x0040F7E0.
- 00d1eae0: slot 238 named SetThingFlag238 (NAME_FIXES.tsv).
- 00891720 (pre-existing RELOC_NAME warnings): 0x0129BA3C labelled g_NormaliseEpsilon (DELTA_NOTIONAL_ZERO is the PDB name); 0x008ED650 bound as
  CCreatureAction_StabbedToDeath's ctor but labelled CCreatureAction_Die's.
- BIGB 0x0129B004 / 0x0129B07C: not on the census; re-checked and left (T3 KEEP).

## Census
- `baked_symbols.py census` at current HEAD (after NF417): **36** rows (evidence/census_head.txt) = my 37 s2 rows minus 0048f881 (no table line).
- With both batches (`FABLE_BAKED_DROP=evidence/TABLE_CHANGES_s2_f2_combined.tsv` + `--replace batches/s2/rows batches/f2/rows`): **0** (evidence/census_after.txt).
  This number cannot fail by itself, since dropping the table lines removes the key, so the independent proof is the source pass with the
  UNCHANGED key (no DROP): batches/s2/evidence/sourcepass_new.txt -- new rows [] for all 37, HEAD rows hit for 36 (0048f881 [] at both, it has
  no table line; its respelling is proven by the object pass and the mutant test); f2 likewise (batches/f2/evidence/sourcepass_new.txt).
  The object pass (key unchanged) goes 37 -> 0 for s2 (the 'local' hit catches 0048f881's file-static array).
- Expected census after landing: 36 -> 0.
