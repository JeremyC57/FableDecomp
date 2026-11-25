# NF418 hand-back (2026-11-25): data-symbol baked strings and float-pool constants (T3 `nf415 table baked`, `nf415 0x0122DEDC baked`)

## Result
- **31 rows ACCEPT** in two batches, locks kept as NF418; **9 HOLD** (f1, test only), locks released; 1 table-only batch.
  - **s1** (strings) 17/17 ACCEPT: 00744110 00b1b470 00bdb2f0 008b3980 008b3b80 008b3ea0 008b3d80 008b34d0 008b4200 00842c00 00842c50
    00abfcd0 009979e0 00438570 00d732a0 00697860 00699090. 19 table deletes. Units: 008b34d0 + 008b3d80 + 008b4200 (`_kName`), others single.
  - **f1** (floats) 14 ACCEPT: 004df3b4 004df3bd 004df3d4 004df46e (one unit: g_terrainHeight) 004df3dd 005edff7 00697710 006a4e40 00c0b180
    00c0b420 00890370 008903a0 008903d0 00890400. 14 table deletes. HOLD 00437500 0054ecb0 006448a4 006a98f0 006e7340 00753ba0 008909f0
    00891720 00895a10: the row edit grades RELOCATION_MATCH with literal-only sections (rows/ kept), but the HEAD tests are self-contained copies
    (mutant PASS); a linked test is the only thing missing. Their symbols' table lines are kept.
  - **t0** (table only) 4 stale deletes: ?g_str_0122f3b4@@3QBDB, _CCharString_ConstructCCoopSpiritDef_0066fa10_Name (the witnesses already spell
    the literal), ?g_PrimitiveDrawDistanceFactor@@3MA, _C2DVector_DivideByScalar_0066f990_Unit (no user; witness objects checked: evidence/stale_objects.txt).
- No header declarations to remove: no fixed symbol is declared in rebuild/include (census418.py; mktable418.py re-greps src / tests / include /
  integration before each delete: other_users "-" for all, `_kName` aside -- see below).
- No self / callee / name changes (SELF_SYMBOLS.tsv header only). NAME_FIXES.tsv: `_TWO_PI` is a second spelling of 0x0124F2B8 (?TWO_PI@@3NB).

## Task 1: the candidate universe, re-derived (CLASSIFICATION.tsv; tools/classify418.py, strrefs418.py, floatpool418.py, mkclass418.py)
- Universe = every reloc_bindings line binding a non-literal symbol into retail .rdata (524 lines, evidence/universe_raw.tsv), decoded narrow and
  wide WITHOUT NF415's predecessor rule, with the witness row's retail instructions (capstone) that use the VA.
- Strings (90 lines; rows: fix 54 = 17 done in s1 + 37 open, held 3 NF417 rows): 69 fix, 3 fix-held (kActionStr / kActionNameLiteral / g_actionName: NF417's GetClassTypeName rows), 2 stale, 7 keep,
  1 unsure, 8 vtable (left: q1 pending). NF415 missed 4 file-static arrays because the string follows a float (`_kName` 0x012786AC) or is wide
  without a preceding NUL pair (`_s_newLine`, `_s_Default` L"TEXT_GUI_NEXT", `_s_jpgMask` L"*.jpg").
- Keep evidence: `_MAX_APOLOGY_SECONDS` is `const long = 10` bound to a const reference (the "\n" reading is 0x0A); g_metaLayerBase /
  g_refreshRateOptions are read indexed (`[eax*4 + VA]`); szOle32Aut / szControl / szRegTypeLib / szTypeLib are DirectShow BaseClasses arrays
  (Vendor-proof lines; vendor tree not on disk, so from the rows' proof, not re-read). g_funcInfo (NF415: "false positive") is a pushed
  "PLAYER_GUI_PC" and was fixed. Unsure: g_retailBankMagic -- "BIGB" also at 0x0129B07C in the same region; /GF merges equal literals, so one of
  the two is a named array.
- Pool evidence for strings: evidence/strrefs.tsv -- most fix VAs are referenced from several 64K regions (TUs), which a file-static array cannot be.
- Floats: VC7.1 RTM emits a float literal as COMDAT `__real@<bits>` (LINK_ONCE_DISCARD, select-any: merged exe-wide) and gives every named const
  (`static const float k`, header `const float`, `extern const`) its own storage -- it never folds a named const into __real@ (probe/p1.cpp).
  So for each value at most one VA is the pool entry. floatpool418.py counts x87 memory references per VA exe-wide: 0x0122DEDC 0.0f 3667 refs
  from 169 regions, 0x0122DED8 1.0f 2912 / 154. Verdicts (119 float lines; 0x0122DEDC: 21 symbols in 25 rows = 14 ACCEPT + 9 HOLD + 00732D00 / 006CFEC0 not batched): 79 fix (dominant / sole VA referenced from several TUs),
  25 keep (NOT-POOL: the value's pool entry is another VA, e.g. CameraSetupVerticalTolerance 0.25 at 0x012654E8 v the pool 0x0123078C; or
  TOP-close twin copies), 14 unsure (sole VA used from one region: pool or a TU const), 1 vtable, and DELTA_NOTIONAL_ZERO (Ego_d PUB32) /
  TWO_PI (established 2026-11-01) kept -- RULINGS_NEEDED r1 on the TWO_PI premise.

## Task 2/3: the fixes
- Each use site spells retail's exact bytes (tools/lit418.py from Fable.exe; widths from the parameter type: L"\\" for 009979e0's
  operator+(const CWideString&, const unsigned short*)); the extern / static declaration is dropped; a `// Literal (NF418 ...)` comment names
  the old symbol, table line and VA. 00438570 keeps the InitGuard stand-in by `(int*)"PLAYER_GUI_PC"` (NF415 b2 precedent; T3 port line).
- Floats: `return 0.0f;`, compares and the libpk clamps compile to the same instruction with the relocation on `__real@00000000`,
  `__real@3f800000`, `__real@437f0000`, `__real@40490fdb`, `__real@40c90fdb`, `__real@c0490fdb` (bytes == the name == retail at the VA).
- Not fixed: 00732D00 (`g_zero + 0.5f`: a spelled 0.0f folds; retail loads the pool entry -> shape question, T3) and 006CFEC0 (dead
  `extern const float g_zero;`, object references nothing; T3 hygiene); both locks released untouched.
- `_kName` table line: its other "users" are pointer-form `static const char* const kName = "..."` rows (00bdbf00, 008b3630) whose code already
  references `??_C@` (objects checked: the unreferenced `_kName` pointer is not in any relocation) and tests with their own statics: safe to delete.

## Gates (tools/gates415.py + runbatch.sh, NF415 model, all at one LANDVERIFY_WORK path per batch lv/batches_<b>_one -> _h/_n/_m)
- vland HEAD / new: RELOCATION_MATCH + PASS both sides for every ACCEPT (all were RELOCATION_MATCH at HEAD: no MATCH->RELOCATION_MATCH change);
  new logs 0 LITERAL_DIFF / LITERAL_UNCHECKED; literal_check on each new object returns every string VA equal.
- Section compare (evidence/comdat_<a>.txt): only new sections `??_C@` / `__real@`; code byte-equal with the relocation fields masked; HEAD
  relocations to the data symbol swap at the same offsets (addend 0) to the literal; dropped HEAD data (file-static arrays, static float
  consts) accepted only as GONE-DATA == retail == the new literal / __real@.
- Patched gates415.py (NF418): object-local static strings matched by content (no table line), data-symbol -> `__real@` swaps, dropped float
  consts, `real_ok` picks the VA matching the COMDAT's bytes, stray check also over float VAs, and **a mutant PASS is a HOLD** (brief).
  mkpayload.mutate handles signed / unsuffixed floats.
- Tests: every ACCEPT test is LINKED (row declared, callees defined under the row's spellings) and asserts the text / value; the mutant
  (first character / 0.5f) FAILs for all 31. Pass patterns unchanged.
- purity: [] and genuine for every new row and test; no __asm / naked / _emit / volatile.
- /FORCE link (evidence/link_s1.txt, link_f1.txt): duplicates 0 -> 0, NEW_UNRESOLVED 0 (only GONE_UNRESOLVED: the dropped data symbols).
- apply_table --dry: s1 {'delete': 19}, f1 {'delete': 14, 'rewitness': 3}, t0 {'delete': 4}, misses 0, multi 1 -> 1. reserved_check (RESERVED_REFS=""): 0.

## Task 4: detector (purity_baked_symbol_patch.py, separate, not applied; key STRING_VAS_ADD.tsv, 68 verified VAs to merge into string_vas.tsv)
- Source pass baked_symbol_names(text): a table symbol bound to a key string VA (not ??_C@) whose identifier the row declares / defines as data
  and uses. Object pass baked_symbols_in_object(obj): relocations to such symbols, plus object-local static string arrays ('local').
- Validation: object pass on s1 -- all 17 HEAD objects hit, all 17 new objects clean (evidence/detector_objects_s1.tsv).
- Census over the landed catalog (genuine-class sources): **HEAD 56 rows** (17 s1 + 36 open + 3 NF417-held); **after s1 + its / t0's table
  deletes: 39** (evidence/detector_HEAD.tsv, detector_after.tsv; `FABLE_BAKED_DROP` applies a hand-back's TABLE_CHANGES before landing).
  Reconciled with CLASSIFICATION.tsv: no unexpected row; the only open row it misses is 0048f881 (file-static s_TextGuiSkipScene with no
  table line: the object pass sees it). The source pass needs the declaration in the row: a symbol declared only in a shared header is
  invisible to it (none today). The pointer form `const char* const kName = "..."` is excluded (it IS the literal; 008b3630 / 00bdbf00).
- Floats are not keyed (task 4 is baked_string); the same pass keyed on CLASSIFICATION.tsv float `fix` VAs would grade the float form.
- STRING_VAS_ADD.tsv: every VA verified at a NUL-terminated string start (4 after a float: start_evidence says so); the width of the one-
  character / empty readings ("(", ",", ")", ";", "\"", " ", L"\\", L"%") comes from the stand-in's declared type (QBD v QBG), checked
  against the callee's parameter only for the s1 rows.

## Retail bytes
The batches' head/ new/ mut/ payload.json + oracle.tsv and lv/ objects are lane inputs carrying retail bytes: NOT for the repo.

## Restart
The container restarted after t0 was written; only the t0 user grep was cut (re-run: 0 users). After the advisor review: f1 got 3 `rewitness` lines
(_kPi / _kTwoPi / _kMinusPi -> 0x00C0B4C0, probe object relocates to all three), the f1 user check was re-run over ACCEPT rows only
(no HOLD row uses a deleted symbol), and s1 / f1 gates + all three apply_table --dry were re-run on the final tools (s1 17 ACCEPT;
f1 14 ACCEPT / 9 HOLD; deletes 19 / 14+3 rewitness / 4, misses 0).
