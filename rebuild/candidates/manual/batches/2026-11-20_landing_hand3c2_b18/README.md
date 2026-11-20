# Landing hand3c2 b18: 0x008185B0 NMaths::GetRootsNumerically<CVeryRandomFunction...> + four re-ports (Account B, 2026-11-20)

All five rows are RELOCATION_MATCH with their tests passing under strict vland `--land`. All five recheck again after the file rename below.
Audited by the rolling auditor ($S/stage/audit_roll).

| address | row | kind |
|---|---|---|
| 0x008185B0 | NMaths::GetRootsNumerically<`anonymous namespace'::CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint> (/Gr) | new (hand4d hand-off) |
| 0x0087AA10 | NMaths::GetRootsNumerically<CTanMethod> (/Gr) | re-port onto the new template text |
| 0x0087AFF0 | NMaths::GetRootsNumerically<CCotMethod> (/Gr) | re-port onto the new template text |
| 0x0078B7C0 | NMaths::GetRootsNumerically<CAnalyseRootsOfBowPullBackFunction> (/Gr) | re-port onto the new template text |
| 0x0078A690 | CTCHeroRangedAttacks::FrameUpdate | re-port: its same-TU instance copy takes the new text; the emitted instance == retail 0x0078B7C0 1390/1390 |

## The template
The template is rewritten statement for statement from Ego_d's line table, with three `// Inferred:` choices, each needed. The author measured
them in c2's factoring routine: the factor chosen at each of the five inlined GetValue sites depends on the relative symbol numbers of cos_x and
sin_x there.
1. The do-loop's first value is `func.GetValue(r_start)`. Without it, 41 instructions differ.
2. The existing opt-in FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE, STLport's real deallocate layer (0x008185B0 only; the three instances match
   without it). Without it, 38 differ.
3. GetValue returns through `(float)(...)`, as the landed CTanMethod / CCotMethod already did. Without it, 5 differ.

There is no header change.

## Test
- The row is a template instance over an anonymous-namespace class, so its decorated name carries a hash of the TU's path, and a plain link
  drops the unreferenced COMDAT. The strict vland PASS therefore runs the test's model-only mode, as the landed sibling tests do.
- Real-object mode: `LINK_EXTRA=/OPT:NOREF python3 tools/trun.py 008185b0 test/008185b0.cpp` (author workspace). It passes 4,041 cases, with
  115 knife-edge cases skipped, and kills 84 of 87 mutants (3 equivalent).

## Paths
vland named three files from the decorated name (`global_function_008185b0`, `NMaths_CTanMethod_0087aa10`, `NMaths_CCotMethod_0087aff0`). They
are renamed to the sibling pattern NMaths_GetRootsNumericallyC..._<addr>. All five rows recheck RELOCATION_MATCH PASS on the renamed paths.

## Name fix
NAME_FIXES.tsv: 0x008185B0's catalog label is the empty `::`.
