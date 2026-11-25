# NF411 hand-back (2026-11-25): retail literals spelled in the StatGetClassTypeName family

## Summary
- **Worklist: 271 rows**, from `crawl/purity.py` `baked_string_addresses()` run over every landed row whose `// Leaf:` is a StatGetClassTypeName (tools/family.py, work/family_all.tsv).
  - 281 family rows are landed; 271 bake an address, 10 already spell their literal (0x004D2A74, 0x004D31C0, 0x004D39CF, 0x004D3CDE, 0x004D4232, 0x004D4249, 0x004D429B, 0x004D430F, 0x004D5CE6, 0x004D5FDB).
  - The 271 are the 270 nf408 rows plus 0x004D4A86 (NF407).
  - Each row has exactly one baked address, in one uniform expression: `CCharString((const char*)0x0123xxxx, -1)`.
- **Locks.** None of the 271 was locked by anyone else (NF410 holds no family row). All 271 are locked NF411 and kept, since every row is ACCEPT.
- **Result: 271 ACCEPT, 0 HOLD.** No RULINGS_NEEDED.

| batch | rows | range |
|---|---|---|
| b1 | 50 | 0x004D2D26..0x004D38CD |
| b2 | 50 | 0x004D38E0..0x004D4617 |
| b3 | 50 | 0x004D465D..0x004D4F3E (holds 0x004D4A86) |
| b4 | 50 | 0x004D4F6E..0x004D58E3 |
| b5 | 50 | 0x004D5913..0x004D627F |
| b6 | 21 | 0x004D630D..0x004D674D |

- Batch layout is NF408's: RESULTS.tsv, SELF_SYMBOLS.tsv and TABLE_CHANGES.tsv (both header-only), UNITS.tsv (one unit per row), rows.txt, rows/, tests/, orig/ (HEAD copies), head/payload.json + oracle.tsv, logs/ (vland HEAD/new, reserved_check), evidence/.
- Any batch can land alone. No includers: no landed row `#include`s a family source.
- `head/oracle.tsv` holds retail bytes, as NF408's did. It is lane input only and is not for the repo.

## Literal check (step 1; tools/lit411.py, work/lit_all.tsv, LITERALS.tsv)
For every row, read from Fable.exe ($FABLE_EXE), all 271 pass all of the following:
- The cast VA lies in `.rdata` (0x0122D000..0x0137384C).
- The byte before the VA is NUL, so the VA is a string start. None of the 271 VAs is the section start.
- The bytes up to the first NUL are printable ASCII. The strings are narrow, with nothing to escape.
- The text equals the row's class name exactly, the unqualified class of the leaf symbol, case-sensitive. No row needed a HOLD.
- The retail function at the row address begins `56 6A FF 68 <imm32>`, and that imm32 equals the cast VA.
- The row's old trailing comment `// "<text>"` agrees.

LITERALS.tsv lists address, VA and the exact spelling for all 271.

## Change (step 2/3; tools/gen411.py)
Each change is an exact replacement that must hit once, followed by a diff gate. The row changes in exactly one line plus a two-line added comment; the test changes in one line plus one added include.
- **Row.** `CCharString((const char*)0x<VA>, -1); }   // "<text>"` becomes `CCharString("<text>", -1); }`.
  - This is the nf406_b2 0x004D31C0 / NF402 form. The redundant trailing comment is dropped.
  - A two-line comment is added before `// Form (`: `// Literal (NF411, 2026-11-25): HEAD cast the retail address (const char*)0x<VA> ...; it is now the string literal, spelled from retail's bytes at 0x<VA> ...`.
  - Name, form, includes, leaf, helper and every other line are unchanged.
- **Test.** The test already links the row object. `if (g_seenText != (const char*)0x<VA>)` becomes `if (g_seenText == 0 || strcmp(g_seenText, "<text>") != 0)`, and `#include <string.h>` is added.
  - The ctor-count, hidden-slot and length -1 assertions are unchanged.
- **Spelling source.** It comes from work/lit_all.tsv (retail's bytes), not from the leaf or the comment.

## Gates (tools/gates411.py, strict; anything else HOLDs)
- **vland** (runpair411.sh: HEAD and new at one LANDVERIFY_WORK path per batch, work/wd_bN moved to wd_bNh / wd_bNn):
  - 271/271 RELOCATION_MATCH PASS -> RELOCATION_MATCH PASS, so tests PASS on the new side;
  - the WARN set is identical on both sides, with only the pre-existing SHARED_HEADER_AVAILABLE;
  - the new side has 0 LITERAL_DIFF and 0 LITERAL_UNCHECKED, with FABLE_EXE set (evidence/vland_cmp.txt).
- **objsec356 + own COFF read, HEAD v new by COMDAT symbol** (evidence/objsec.txt). For every row:
  - The leaf is 19 B on both sides, with selection and characteristics equal.
  - Its bytes are equal except at +4..+8. HEAD holds the LITERALS VA there; new holds 0 under exactly one added DIR32 (type 6) relocation to a `??_C@` symbol.
  - The only ADDED section is that `??_C@` .rdata COMDAT, and its data is exactly `<text>\0`.
  - Nothing is DROPPED or CHANGED, and there is no .drectve delta. SYM+ is that literal only; there is no SYM-.
  - The scaffolding helper is byte- and reloc-equal.
- **$S/comdat_cmp.py HEAD v new** (evidence/comdat_cmp.txt). Every .text section is EQ except the leaf, which is DIFF 19 19: its raw bytes differ only in the 4-byte field at +4..+8 (HEAD's VA v 0 under the one added DIR32), so every other code byte is equal. This is how the brief's "code bytes are equal" is read here. Nothing is NEW or GONE. The literal is the .rdata ADDED above.
- **purity** (evidence/purity.txt): `baked_string_addresses` = [] on every new row and every new test.
- **r10cmp against the HEAD-built binder 0x004EE23F** (rebuilt fresh from the repo into work/wd_binder; evidence/r10.txt):
  - 271/271 go from HEAD `LITERAL_FORM_ONLY` to new `EQUAL`: the row's pick-any copy is byte- and reloc-identical to the binder's same-named copy.
  - This is R1 tier 1, and it closes the R10 debt (T3 nf408 family literal, nf407 0x004D4A86 literal).
  - No row failed it.
- **/FORCE link of binder + batch** (link377.py with a response file; batches/bN/evidence/link_bN.txt):
  - Every batch gives duplicates 0 -> 0, NEW_UNRESOLVED 0 and RESOLVED_TO_GONE 0.
  - Binder + all 271 (272 objects) gives the same: 0 -> 0, 0 new unresolved (evidence/link_all271.txt). The raw logs are non-empty, about 190 KB each.
- **reserved_check** (RESERVED_REFS="", no git): 0 reserved rows in each batch. Rerun it with the default refs at landing.
- **HEAD drift.** At hand-back, every orig/ file is byte-equal to the repo's current row and test.

## Pilot (10 rows, run first: work/batch_pilot, evidence/pilot/)
- **Rows:** 0x004D4A86 (NF407's shape); 0x004D4415 (NGraphicAppearance::) and 0x004D53A8 (NThingShot::), the namespaced shape; 0x004D2D26, 0x004D35B9, 0x004D3E60, 0x004D4819, 0x004D4EC9, 0x004D582A and 0x004D5EB7 (plain).
- **Results:**
  - vland: 10/10 RELOCATION_MATCH PASS on both sides, with identical WARN sets and no literal warns.
  - Gates: 10/10 PASS.
  - r10cmp: 10/10 LITERAL_FORM_ONLY -> EQUAL. The namespaced rows stringize identically to the binder.
  - Link (binder + 10): 0 -> 0 duplicates, nothing newly unresolved.
- **Mutant proof that the literal gates are not vacuous** (evidence/mutant/). Row 0x004D2D26 was given a one-character case change ("CTCCarriedActionUseFooD"), keeping the correct test. The mutant is caught three ways:
  - vland prints `WARN LITERAL_DIFF +4 ??_C@...FooD?$AA@: object 'CTCCarriedActionUseFooD', retail 0x0123A2A0 'CTCCarriedActionUseFood'`;
  - the linked test FAILs (`RELOCATION_MATCH FAIL`);
  - r10cmp reports DIFF.
- The pilot rows were then regated inside their address-order batches with the same results.

## Tools (agentNF411/tools; NF408's copied and repointed, never run in place)
- **family.py:** the worklist.
- **lit411.py:** the retail literal check.
- **gen411.py:** the generator and diff gate.
- **runpair411.sh / binder411.sh:** vland pairs and the HEAD binder build.
- **gates411.py:** the gates above.
- **mkbatch411.py:** writes RESULTS, UNITS and the other batch files.
- Copies of NF408's link377.py, r10cmp.py, objsec356.py, cmpcomdat.py, nf.py and nf387.py.

## Debt and notes
See T3.txt.
- Still open: self_symbols lines for the family; NF407's four self-contained tests (rows that already spelled their literal); the 0x004D5853 two-names table line; the 113 stand-in callers.
- After landing, the dashboard's `baked_string` count drops by 271. Regenerate it, do not hand-edit it.
