# 00e19560 final handoff

Final status: **IN_PROGRESS**. Selected grade: 15. Worker-selected nonmatch; final independent integration/review not completed. Do not land as a match.

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00e19560/PROBES.md

# Probes

At least12 distinct source families: natural do loop; while round-bound loop; external ScriptThing destruction; external Movie construction/destruction; explicit counted-pointer release; declared counted-pointer destruction; implicit scripted-resource destruction; external resource construction; early-continue health gate; explicit index while loop; TRetVoid failure adapter; shared RAII exit; explicit Movie constructor; pause guard using its original reference parameter. Repairs and local-order-only followups are not counted as new families.

1. PDB locals and native three-round health loop; scoped animation/cutscene lifetimes — DIFFER(2194v2078).
2. original local declaration order and while round-bound loop — DIFFER(2164v2078).
3. external ScriptThing destruction to test ownership cleanup expansion budget — DIFFER(1944v2078).
4. external Movie constructor/destructor declarations for later cutscene sites — DIFFER(2079v2078).
5. explicit ScriptThing ownership release before implicit member/base cleanup — DIFFER(1944v2078).
6. genuine counted-pointer ScriptThing dtor declaration — DIFFER(2037v2078).
7. implicit scripted resource destructor and original ownership body — DIFFER(2225v2078).
8. external scripted resource constructor at both acquisition sites — DIFFER(2118v2078).
9. early-continue health gate preserving unordered-float behavior — DIFFER(2164v2078).
10. explicit spawn index lifetime and while increment — DIFFER(2377v2078).
11. original TRetVoid failure adapter throughout — DIFFER(2169v2078).
12. shared RAII exit label for every termination checkpoint — DIFFER(2164v2078).
13. genuine explicit plain-inline Movie constructor — SRC_FAIL.
14. repair counted-pointer specialization before Movie constructor body — DIFFER(2164v2078).
15. while loop with both native boolean initializations before health setup — DIFFER(2164v2078).
16. genuine pause guard constructor calls its reference parameter directly — DIFFER(2164v2078).

Selected15. Selected2164 versus native2078, selected frame0xec versus native0xe4. Quarter-health arithmetic and all script actions/scopes agree semantically. Native late scripted-resource construction and pause guards expand differently; selected emits two genuine pause-guard constructor calls where native inlines. Stack slots and mixed ScriptThing destruction differ. Movie declaration probe2079 still changes first inline constructor/destructor, so size proximity alone was rejected. No byte match claimed.

Dual-compiler exact-body tests PASS. DIFFER remains distinct from behavior PASS.
