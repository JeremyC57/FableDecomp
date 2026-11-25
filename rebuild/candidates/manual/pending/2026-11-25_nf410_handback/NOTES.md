# NF410: baked string addresses -> retail literals (2026-11-25)

## Result
- Worklist: `purity.baked_string_addresses` over every landed source = 571 rows (evidence/worklist_all.tsv).
  - Skipped 278 (evidence/worklist_skip.tsv): 274 in NF407's family_census.tsv (StatGetClassTypeName family), 4 named by a
    T3 line (T3.txt). No todo row was locked by NF408 / NF409 / anyone else; the 28 remaining `GetClassTypeName` rows (instance
    member, not `StatGetClassTypeName`, none in the census) were done, batched last (b6 / b7).
  - Done: 293 rows, 295 baked VAs. **293 ACCEPT, 0 HOLD**, in b1 (the pilot, 10) .. b7 (50 / 50 / 50 / 50 / 50 / 33).
  - All 293 locks are held as NF410 (no HOLD to release).
- No path-like string among the 293 (no `\`, `/` or file extension): 276 identifiers / symbol names (SG_*, COMBAT_SEQUENCE_*,
  class-type names, hook names), 10 display texts with spaces ("UV Tangent Vectors", "<Empty Slot Name>", ...), 9 uses of the pooled
  empty string. The only path-like target in the whole worklist is the T3-skipped 0x00406F70 (`U:\`, spell `"U:\\"`; T3.txt).

## Method
1. Targets (tools/classify.py -> evidence/targets.tsv), from the retail image:
   - all 295 VAs are in `.rdata`; none has a binding in reloc_bindings.tsv except 0x0122D70E (below); none has a non-printable byte;
   - width from the bytes (UTF-16 pattern) cross-checked with the cast type: 293 narrow, 2 wide (`unsigned short*`, both ASCII);
   - start of literal: 292 are NUL-preceded. 3 are preceded by a non-NUL byte (0x01275988, 0x012780DC, 0x01288C48): each is 4-aligned
     and preceded by a float constant (40 40 00 00 = 3.0f, 3E 4C CC CD = 0.2f, 3F 84 7A E1 = 1.035f), so they are literal starts,
     not tails;
   - 0x0122D70E is retail's one pooled `""` (NF388: 3,919 landed `""` relocations target it): spelled `""`. reloc_bindings.tsv
     binds six data symbols to that address (`?g_shadowTag@@3HA`, `?kEmptyDisplayNameTag@@3QBDB`, `?GHO_ItemName@@3QBDB`,
     `?g_profileTag@@3QBDB`, `?g_FableEmptyText_0122D70E@@3QBDB`, `?g_emptyString_0122d70e@@3QBDB`): other rows model `""` as
     extern data. Not reused here; flagged as naming debt for the coordinator.
2. Spelling (tools/fix.py): bytes up to the terminator, escaped (`\\`, `\"`, `\n` `\t` `\r`, `\xNN` split before a hex digit,
   trigraph guard), wide as `L"..."`; each spelling round-tripped through NF388's independent decoder (cliteral.literals) against the
   retail bytes, terminator included. Wide literals keep the row's cast, as landed rows do: `(const unsigned short*)L"AutoSave"`.
   Only the cast-address expression changes; comments are untouched (every trailing `// "..."` comment already equals the retail
   text). LITERALS.tsv lists every VA with its C spelling.
3. **Named literal pointers (11 rows; RULINGS_NEEDED r1).** `static const char* const kName = (const char*)0x...;` folded to an
   immediate at HEAD. Spelled as `kName = "..."`, VC7.1 keeps a non-COMDAT 4-byte `.rdata` object (`_kName`: pointer + DIR32 to the
   literal) that nothing reads (pilot: `_LIT_AB`, `_kName`, `_kActionNameLiteral`: COMDAT gate failed). Retail has no such object: no
   dword equal to any of the 12 literal VAs of these rows exists in any non-code section of Fable.exe (.rdata .data .idata _PDATA
   .rsrc scanned; evidence/r1_datapointer_scan.txt). So in these rows the literal is spelled at
   the use site and the declaration goes (code bytes identical; tools/hand.py INLINE): 00406c20 007a6db0 007bad60 (b1),
   007bb610 007d2220 (b7), 008b49a0 (b2), 00aa7350 00aa75d0 00ab0b30 00abea90 (b6), 00ac1270 (b7).
4. Tests (tools/fix.py + tools/hand.py, all edits recorded):
   - every pointer comparison against a baked VA (`(const char*)`, `(char*)`, `(void*)`, `reinterpret_cast`, bare integer) became a
     null-safe content compare, `RetailTextEquals(p, "text")` / `RetailWideTextEquals(p, "ascii")` (hand-written loops, no
     `<cstring>`), added after the includes;
   - body copies in self-contained tests get the same literal as the row;
   - self-contained tests whose copy used an invented stand-in text ("XUVTangent", "BoneMask", "DoCreatureAction",
     "AUTOSAVE_FMT", ...) now spell retail's text in body and assertion (TOK list in hand.py; 008b5090's length check 27 -> 29);
   - tests that observed nothing gained an assertion (004568bc / 00456903 capture the marker argument, 008fdd10 / 008b49a0 /
     00b619f0 check the text); 4 printf texts lost a `ptr=0x...` (pass patterns unchanged);
   - three tests that only printed PASS (00aae0f0, 00aaacd0, 00ac62b0) are now linked tests: they supply `CCharString(const char*,
     long)` (the objects' only external, objdump -t) and assert retail's text through vland's fallback link;
   - 00406c20 / 00406a80: their models discarded the text-bank symbol; the test now captures it and asserts
     "TEXT_GUI_SAVE_AUTO_SAVE_NAME" / "TEXT_GUI_SAVE_EMPTY_SLOT" (so every fixed literal of the 91 self-contained tests sits inside a
     RetailTextEquals / strcmp check);
   - 00750a40: the model stores the pointer in an `int d`; its check is `r.d == (int)g_last_s` (no integer cast to a pointer).
5. Gates (tools/runbatch.sh + tools/gates.py, per batch, vland at ONE LANDVERIFY_WORK path `lv/batches_<b>_one`, renamed to
   `_h` / `_n` / `_m` after each run; logs/workpath.txt):
   - g1 vland --recheck-landed HEAD and new: 293 x RELOCATION_MATCH/PASS -> RELOCATION_MATCH/PASS (HEAD rows were already
     RELOCATION_MATCH through their call relocations);
   - g2 new logs: 0 WARN LITERAL_DIFF / LITERAL_UNCHECKED / LITERAL_CHECK_ERROR; and a positive proof: `reloc_identity.literal_check`
     run on each new object returns every fixed VA as equal (the vland warning is silent when it cannot pair relocations);
   - g3 COMDAT gate (offset-exact, evidence/comdat_<a>.txt with the raw `$S/comdat_cmp.py` output appended; raw comdat_cmp reports
     DIFF on each leaf because the baked immediate became a relocated field): the only new sections are `??_C@` literals, every other
     section is byte-equal once the new `??_C` relocation fields are masked, HEAD held exactly one of the row's baked VAs in each
     such field, the addend is 0, the `??_C` bytes equal the retail bytes at that VA, and HEAD's relocations are unchanged. 293
     literal-only;
   - g4 `purity.baked_string_addresses(new row) == []` and no baked VA left as a hex constant in the comment-stripped row or test: 293;
   - g5 mutant run (row literal's first character changed, NEW test): 202 tests FAIL on the mutant (they observe the text) and
     91 PASS (self-contained copies, ruling q1; accepted only when the test itself spells every fixed literal). Every mutant
     object raised WARN LITERAL_DIFF, so the literal check sees each fixed relocation.
   - No row has an includer (naming_status.includers_of, and the brief's `grep -rl '_<addr>.cpp"' rebuild/src/compiled`: 0):
     no funclet / thin row to regrade.
   - Before hand-back: every orig/rows and orig/tests copy is byte-equal to the repo file (no HEAD drift);
     `purity.baked_string_addresses` on all 293 new tests: []; RESULTS old symbol == new symbol on all 293 (none `?`, none
     StatGetClassTypeName).
   - Not run (no code change to pair or link): pick-any checkers and the /FORCE link census; the new sections are pick-any
     `??_C` COMDATs only.

## Pilot (b1, 10 rows, measured before the other batches)
| row | shape | literal | test |
|---|---|---|---|
| 00406c20 | two VAs, wide + narrow, named constants (inlined) | `L"AutoSave"`, `"TEXT_GUI_SAVE_AUTO_SAVE_NAME"` | self-contained, invented texts replaced |
| 00406a80 | two VAs, wide + narrow | `L"<Empty Slot Name>"`, `"TEXT_GUI_SAVE_EMPTY_SLOT"` | self-contained (hand: char model of the wide string) |
| 006926f0 | pooled empty string | `""` | self-contained |
| 004568bc | `""` in a `#pragma optimize` row | `""` | self-contained, marker text now asserted |
| 008fe381 | float-preceded target | `"SG_DEED_REACTION_ROOT"` | linked, asserts |
| 007a6db0 | `(const char* const)` named constant (inlined) | `"CTCGraphicAppearanceReplaceableMesh"` | self-contained |
| 007bad60 | `reinterpret_cast` in an anonymous-namespace constant (inlined) | `"CTCTrespassable"` | self-contained |
| 00aa49d0 | `result->ctor(...)` | `"Compressed Animation Sequence"` | linked, asserts |
| 006922a0 | plain `CCharString(lit, -1)` | `"CARRYTHINGADD"` | linked, asserts |
| 006921d0 | short string | `"SOUND"` | self-contained |

First pilot run: 7 ACCEPT, 3 HOLD (the named-constant `.rdata` pointer, method 3); after inlining, 10 ACCEPT. All 10 HEAD and new
RELOCATION_MATCH/PASS, 0 literal warnings, mutant LITERAL_DIFF on every fixed literal. b1 was re-run in full (head, new, mut,
gates) at one path after the last test edits below: 10/10.

## HOLDs
None. (First b7 run held 00750a40: its test compared `int d` through the text helper, TCC_FAIL; fixed in hand.py, b7 re-run: 33/33.)

## Findings for the coordinator
- Six data symbols are bound to the pooled `""` 0x0122D70E in reloc_bindings.tsv (one an `int`, `?g_shadowTag@@3HA`): rows that model
  retail's `""` as an extern global. They are the same baked-literal problem in data-symbol form; candidates for a follow-up wave.
- Several self-contained tests modelled the wrong function or text (008fe019's test models `CActionDoCreatureAction`, a copied test);
  only the literal was corrected, the test's own class names were left as they were.
- 00aaacd0's header comment names retail 0x00aaea90; the row is 0x00AAACD0 (comment only, not changed).

## Layout
batches/b1..b7, each: rows.txt, RESULTS.tsv, CHECKS.tsv, UNITS.tsv, LITERALS.tsv, SELF_SYMBOLS.tsv (header of the landed
hand-backs, e.g. pending/2026-11-24_nf385_handback) / TABLE_CHANGES.tsv (headers only: no self or table change), rows/, tests/, orig/rows, orig/tests (HEAD byte copies), head/ new/ mut/ (payload.json +
oracle.tsv), logs/ (recheck, vland_head / _new / _mut, gates, workpath), evidence/comdat_<addr>.txt.
Tools: tools/ (worklist, filter, classify, fix, hand, mkbatch, mkpayload, runbatch, gates). evidence/: worklists, targets.tsv.
