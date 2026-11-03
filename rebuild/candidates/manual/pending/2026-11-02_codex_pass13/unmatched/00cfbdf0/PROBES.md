# 00cfbdf0 final handoff

Final status: **IN_PROGRESS**. Selected grade: 31. Worker-selected nonmatch; final independent integration/review not completed. Do not land as a match.

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00cfbdf0/PROBES.md

# 00cfbdf0 probes

1. Original donor for loop and scope, exact original PDB locals, full owner STLport layout, original inline helpers: DIFFER(2306v2317).
2. Reverse original vector-size comparison to match native operand order: DIFFER(2306v2317).
3. Original thing destructor body after Main so predicate cleanup sees declaration first: DIFFER(2306v2317).
4. Original thing destructor body after predicate definitions before Main: DIFFER(2306v2317).
5. Same-TU non-inline original predicate destructor: DIFFER(2306v2317).
6. Same-TU non-inline original thing destructor: DIFFER(2306v2317).
7. Original thing destructor external declaration controls cleanup nesting: DIFFER(2126v2317).
8. Top-tested outer loop with common terminal cleanup: DIFFER(2332v2317).
9. Single original timer declaration with ordered construction: DIFFER(2306v2317).
10. Original direct timer accessor avoids equality wrapper nesting: DIFFER(2306v2317).
11. Same-TU non-inline timer assignment body: DIFFER(2306v2317).
12. Direct void early exits remove empty result lifetime: DIFFER(2306v2317).
13. Original index declared in enclosing branch before scan loop: DIFFER(2306v2317).
14. Same-TU non-inline original predicate lifetime bodies: DIFFER(2306v2317).
15. Outer for with implicit condition rather than constant false negation: DIFFER(2246v2317).
16. Inner digging loop expressed as condition-only for: DIFFER(2306v2317).
17. Inner digging loop with explicit availability break: DIFFER(2274v2317).
18. Nested original conversation guards preserving evaluation order: DIFFER(2306v2317).
19. Original timer equality with scalar-first operands: DIFFER(2306v2317).
20. Original timer index assigned in constructor body: DIFFER(2306v2317).
21. Same-TU non-inline original thing assignment and data-string wrapper: DIFFER(2306v2317).
22. Native scan vector-size-first comparison with original loop index: DIFFER(2306v2317).
23. Discarded postfix increments for original grave-count updates: DIFFER(2306v2317).
24. Original string equality inversion for final claim guard: SRC_FAIL.
25. Flags last: disable global optimization after source families: DIFFER(4188v2317).
26. Flags last: inline expansion limited to inline original definitions: DIFFER(2306v2317).
27. Freeze original donor shape with size-first allocation guard and private predicate member opt-in: DIFFER(2306v2317).

Source families include original scope/for shape, vector comparison order, thing/predicate destructor context and external declaration, top-tested outer loop, timer declaration scope, direct timer accessor, original assignment context, direct void exits, enclosing scan-index scope, implicit outer condition, explicit digging-loop break, nested conversation guards, constructor initialization placement and original wrapper context. Repeated body-order probes3/4 are one family,24 is failed compile,25/26 are flags and27 freezes2; none counted as additional source families. No stronger inline annotation was added.

## Current eligibility restoration (main)

Selected main31: DIFFER(2306v2317), sourceSHA0c946e3649c9d641c7b8c6afa0bb28583bb0620fb2a649e826469bea7b2ae247. Any older authored generated member source is INELIGIBLE and preserved in history/review backups. Current generated members are implicit or original external declarations. Exact-body GNU64 and VC7.1 tests PASS; root semantic/ten-valid-family parked acceptance remains pending. The older status prose and grade headings above are historical.
