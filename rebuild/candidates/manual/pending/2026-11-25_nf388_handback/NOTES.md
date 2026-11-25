# NF388: string-literal content audit of the landed rows (Account A, 2026-11-24)

## The hole
A string literal is a relocation operand, so its address is masked when the grader compares bytes, and
`reloc_identity.check()` skips `??_C` / `$SG` symbols (SKIP_PREFIXES). So a row that pushes the wrong text grades
RELOCATION_MATCH. Example: 0x00B1AE00 pushed "Bounding Sphere" (0x00AA7100's literal) where retail pushes 0x0129F5E8 "CPSCLight".

## Method (two sources: the compiler's object bytes, the retail image)
- `patches/new/tools/decomp_pipeline/reloc_identity.py` gains `literal_check()`. It pairs the graded function's
  relocations with the retail span exactly as `check()` does: `function_relocs()` plus `retail_target()`, the target
  arithmetic factored out of `check()` with no behaviour change (316 objects compared, `patches/check_unchanged.log`).
  - Every dir32 relocation is resolved through its COFF symbol index. This disambiguates the `.rdata` / `.data` section
    symbols that objdump prints by name.
  - A relocation counts as a string the object defines when it targets a `??_C@_0` narrow literal, a `??_C@_1` wide
    literal, a `$SG` literal, or another data symbol of the object whose bytes at the relocation are a string. Its bytes,
    with the terminator, are compared with the retail file bytes at the address the retail instruction names.
  - A retail read is bounded by SizeOfRawData, so a target in a section's zero-fill tail is reported UNMAPPED.
- `tools/audit.py` runs that same function over compiled objects and re-grades each object first.
  - Only MATCH / RELOCATION_MATCH objects are audited.
  - DIFFER objects are re-run through the patched vland, whose flag / pragma sweep applies (differ2/).
  - EH funclet slices are skipped, as the grader skips their reloc checks.
- Objects: the whole landed catalog was turned into one recheck.py payload (`all/payload.json`) and batch-compiled by
  `tools/bcompile.py`, one cl.exe per chunk of 25-50 files.
  - It uses the grader's own vc71() text, BASE_FLAGS + catalog flags, `/Fo` and `/Fd` into the objdir and a 600 s timeout.
  - Validation:
    - 20 rows: the batch grade equals real vland.
    - Every COMDAT of the batch object equals the vland object. The anonymous-namespace hash and the `.debug$S` object
      path are build-path dependent (`xval/`).
    - 0x00B1AE00 flags; 0x00AA7100 is clean.
- Compile order:
  1. Rows whose source spells a literal and whose retail bytes point at a string.
  2. The other rows with a source literal.
  3. The other retail string operands.
  4. Retail empty-string slots.
  5. Rows that only pull header literals.
  6. Everything else, as time allowed.
- The batch compile used Wine at 2-6 rows/s under the shared load.

## Numbers (final run, evidence/coverage_summary.txt)
- Landed catalog: 57,155 rows. All 56,703 non-library rows were compiled; batch compile ~2 h 40 min, every chunk rc 0.
- COVERAGE.tsv:
  - audited_clean 55,164;
  - audited_mismatch 124;
  - not_audited 1,867:
    - 452 toolchain_library stubs (no compiled body);
    - 1,318 EH funclet slices (the grader skips their reloc checks);
    - 83 rows that do not re-grade at HEAD even through the patched vland sweep (q8);
    - 14 RELOC_UNCHECKED (no single object block of the retail length). None of the 14 has a source literal or a retail
      string operand.
- String relocations checked: 33,496 in 5,750 rows (32,276 narrow `??_C@_0`, 1,194 wide `??_C@_1`, 26 other data
  symbols holding a string). **150 differ, in 124 rows** (LITERAL_AUDIT.tsv: one line per relocation, with object and
  retail text, hex, and the disposition).
- Prefilter check: every mismatch is in a row whose source spells a literal. The 9.6k rows that only pull header literals,
  and the 26k rows with neither a source literal nor a retail string operand, added no mismatch. A 468-row probe of rows
  without a source literal found 2 header-literal relocations, both equal.
- Dispositions (STATUS.tsv, by owner row):
  - 70 ACCEPT: b1 1, b3 65, b4 4.
  - 50 HOLD, all with the fix ready and proven statically:
    - 42 have no asserting test (self-contained tests);
    - 6 are stand-in leaves;
    - 2 change another function's literal too: 009944a0 and 00c0e0f0 (+ its 2 funclet includers).
  - 4 QUEUE: 0047eee4, 00cadd00, 00cb09a0, 004804de (Account B).
  - 5 thin includers follow their owners: 00c0e0f0 HOLD, 00c48860 HOLD.
- Mismatch families (LITERAL_AUDIT.tsv):
  - Copied literals:
    - the "DoCreatureAction" / "ACTION_DO_CREATURE_ACTION" GetClassTypeName family;
    - "Bounding Sphere".
  - Made-up identifiers where retail has the def name:
    - 27 CCombatSequence GetName rows return the class-ish name; retail has COMBAT_SEQUENCE_*;
    - chunk descriptions: "XTextureIndex" for "Texture Index", "SubMesh" for "Sub-Mesh", ...;
    - renderer console names: "SkyRenderer" for "EnableSky", ...;
    - shader names: VSHADER_*_DECAL_3POINTLIGHTS for ..._DIRLIGHT_3POINTLIGHTS.
  - Truncated texts: ":" for ": ", ")" for ")\n\n", "game" for "game.bin", "%x" for "%X", ...
  - Swapped pairs: 0048f939 CONFIRM/CANCEL; 00df2380 SummonerMinion/CREATURE_MINION_WARDOG.
  - Five rows pass a non-empty text where retail passes the pooled "" (0x0122D70E, the target of 3,919 other "" relocations).
- Validation of the batch compile: see `evidence/xval/`.
- Snapshot:
  - The tree payload was built at 20:56. The coordinator landed work after that, so at hand-back 146 files were newer:
    sources, 16 engine headers, the catalog and reloc_bindings.
  - The 305 rows whose source or included header changed were recompiled and re-audited at the current HEAD (all/re/):
    305 grade MATCH / RELOCATION_MATCH, with no new mismatch.
  - No new catalog rows. No batch row's orig/ differs from the repo.
- Recall check: 201 audited rows spell a literal in source but had no string relocation in the graded function. None of
  their graded COFF sections relocates a `??_C` / `$SG` symbol: the literal belongs to another function of the unit.

## Job 2: fixes
`tools/fixlit.py` respells a mismatching literal token in the row's owner file (naming_status.owner_source) with the retail
bytes.
- It applies only when the object literal is spelt by exactly one token that is not on a preprocessor line.
- Several tokens are replaced together only when every relocation of the row to that literal mismatches with one retail
  text and the counts agree.
- `tools/fixcomments.py` respells whole-token mentions of the old text in `//` comments, all mappings at once (so the
  0x0048F939 CONFIRM/CANCEL swap stays a swap).

Gates per row (`tools/runbatch.sh` -> `tools/gates.py`), each with a private LANDVERIFY_WORK:
1. vland HEAD and new (patched grader): MATCH / RELOCATION_MATCH and test PASS; LITERAL_DIFF count HEAD > 0 -> new 0.
2. COMDAT-by-COMDAT comparison (`tools/comdat_cmp.py`, keyed by COMDAT symbol, anonymous-namespace hash normalised; with
   SELF_SYMBOLS old->new for 00b1ae00). It must be literal-only:
   - `??_C` sections present on one side only;
   - code / data sections with equal bytes whose relocation changes are one consistent old->new `??_C` swap. This covers
     a data pointer `static const char* kName`, or another function of the unit that uses the same literal, which is
     disclosed in RESULTS;
   - compiler-local `$L` / `$T` renumbering in EH tables;
   - a `char name[] = "..."` data symbol the fix edited;
   - `.debug$S`, which holds the object path.
3. Both pick-any checkers HEAD v new (pk_all, pickany_vs_retail): no new NEQ, 0 reach flips (`$S/tools/reachflip.py`).
4. /FORCE link of one object per TU, HEAD set v new set: duplicates new <= HEAD and no new unresolved externals.
5. **The test must assert the fixed string.** The batch's new test is run against the HEAD row text (the wrong literal;
   `logs/vland_mut.log`) and must not PASS.
   - Most landed tests are self-contained copies of the row body. They PASS whatever literal the row holds, so those rows
     are HOLD with the fix proven statically.
   - For rows shaped like 0x00B1AE00 (the literal handed straight to an extern CCharString constructor),
     `tools/gen_linked_test.py` writes a test that links the row object, captures the constructor argument and asserts
     text / length -1 / one call / return slot. A few tests were written by hand: 008b4f10 and 00e403d0.
   - Existing linked tests that asserted the OLD text (007e2490, 008b4820) were respelt by `tools/fixtests.py`.
   - Self-contained tests were respelt by `tools/fixtests.py` where they still PASS, matched by text in either width.
     Where they would FAIL (length asserts and the like), the landed test is kept unchanged.
6. A graded symbol flagged STANDIN_LEAF holds the row: the literal fix is ready, but the respell is naming work.
7. A fix that also respells the literal of another code COMDAT of the unit holds the row, because that function's retail literal
   was not compared (009944a0, 00c0e0f0). A data pointer (`kName`) is allowed. An empty reach-flip output also holds.

## Decisions
- 0x00B1AE00 (batches/b1) uses NF385's held text (leaf respelt to `?GetClassTypeName@CPSCLight@NParticleEngine@@...`
  plus the literal; SELF_SYMBOLS.tsv) with a new linked asserting test.
  - The HEAD row also defined 0x00AA7100's symbol: /FORCE link of {00b1ae00, 00aa7100} goes from 1 duplicate to 0
    (evidence/link_b1_with_00aa7100.txt).
  - Its lock still reads NF385. It was not claimed (the brief allows claiming only a missing or `released` lock); the
    coordinator transfers it.
- Rows on the stay-off list are reported only, never changed: 004804de is Account B pending.
- 0047eee4: two L":" tokens and one wrong relocation, so which token is wrong is not known. It is queued, and its L"."
  fix is held with it.
- Not literal-only and not fixed here: 00cadd00 (retail operand is binary data) and 00cb09a0 (literal comes from a
  TEXT() macro and the retail operand differs from the row's comment). Both are queued.

## Found on the way (out of scope, queued)
- 569 landed rows bake a retail string ADDRESS as an integer, `(const char*)0x0129E564`, instead of spelling the literal.
  They grade MATCH by construction and carry no relocation, so the literal check cannot see them
  (evidence/baked_string_addresses.tsv, with the retail string at each address).
- 14 landed rows do not re-grade at HEAD even with the vland sweep (differ2/vland.log).

## Limits of the check (stated, not hidden)
- Empty literals compare one byte, so `""` passes against any zero byte.
  - The 4,406 `""` relocations target 5 retail addresses (evidence/empty_literal_targets.txt):
    - 0x0122D70E (3,919) and 0x0122D70C (285) are the pooled narrow / wide "";
    - 0x0129AAF4 (200), 0x0129C5F0 (1) and 0x0129A8E0 (1) are 4-aligned zero slots that follow a NUL, i.e. standalone
      empty strings.
  - No `""` relocation lands mid-string.
- Strings reached through an object-defined pointer (leaf -> data symbol -> `??_C`) are not followed. Only 3 compiled units
  have a data section that relocates to a literal at all (evidence/string_pointer_tables.tsv: 008b3630 / 008b42e0 /
  00bdbf00, whose `kName` / `s_name`). In all 3 the function also relocates the literal directly, so it was checked.
- Literals baked as integer addresses carry no relocation (q10; 569 rows).
- Float-pool constants (`__real@`) are the same hole and are not covered (q9).

## Process notes
- One read-only `git apply --check` was run against the repo to confirm the grader diff applies (no write). No other git;
  nothing was written into the repo.
- Locks: NF388 was written for every owner / includer of b2-b4; all were missing or `released` before.
  - b2's new text was compiled about 10 minutes before its locks were written (an ordering slip; nobody held them).
  - b3 / b4 were claimed before compiling.
  - 00b1ae00 still reads NF385 (q11).
- Comment respell, first pass (b3 / b4 COMMENT_FIXES_first_pass.tsv): the whole-token respell also changed prose and class
  names in 5 lines. 008b4350 and 00bdbf00 line 1 (a class name equal to the old literal), 0098d6f0 line 1 ("PeekName"),
  006f9be0 line 3 (an `<EAnimType name>` placeholder) and 00df2380 line 7 (an address list that was already right) were
  restored from orig. COMMENT_FIXES.tsv marks each line kept / REVERTED, and the batches were re-graded after.
- Pick-any checkers: HEAD and new outputs are identical for all 125 batch rows, both checkers, once literal names and `$L`
  label numbers are normalised (evidence/pk_HN_compare.tsv). No Traceback.
- The gate now also holds a row on an empty reach-flip output.
- Disk: all/payload.json (373 MB, regenerable with recheck.py) was deleted. obj/ keeps the 56,703 HEAD objects (*.obj only;
  the *.cpp copies were deleted) for a quick re-audit with tools/audit.py. They go stale with any landing; delete freely.
