# 00f0a960 final handoff

Final status: **IN_PROGRESS**. Selected grade: 28. Worker-selected nonmatch; final independent integration/review not completed. Do not land as a match.

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00f0a960/PROBES.md

# Probes

At least14 distinct source families: original natural form; explicit copy member stores; Movie counted-dtor declaration; ScriptedThing dtor declaration; owned/unowned copy branches; guarded while; named predicate; original failure adapter; shared RAII exit; explicit acquisition state; switch dispatch; explicit genuine resource ctor; separate-unit copy; explicit destructor release. Constructor linkage/order and repairs are not counted as new families.

Attempts1–3 are build repairs. Attempts4–12 initially selected the wrong appearance-seed operation; native slot483 takes a reference output. Attempt13 repairs this, and20–25 repeat the earlier distinct families with the corrected operation. No failed or wrong-semantic probe is selected.

1. Natural contact phases with genuine map specializations and inline script copy ownership — SRC_FAIL.
2. Header dependency repair only; same natural contact phase family — SRC_FAIL.
3. Select existing original text comparison overload; repair of first source family — SRC_FAIL.
4. Include prerequisite C strcmp declaration; first source family unchanged — DIFFER(2152v1985).
5. Predicate constructor ordinary same-TU definition before script copy definition — DIFFER(2152v1985).
6. Genuine script copy definition after Main to test original translation-unit availability — DIFFER(2152v1985).
7. Script copy ownership expressed by original member stores after default member construction — DIFFER(2172v1985).
8. Proven native Movie counted member destructor declared out of line as genuine specialization — DIFFER(2176v1985).
9. Use proven ScriptedThing destructor calls and Movie counted specialization with constructor initializer copy — DIFFER(2059v1985).
10. Genuine counted script copy with separate owned and unowned branches — DIFFER(2122v1985).
11. Main loop head guard replaces preguard plus do-while with identical checkpoint order — DIFFER(2047v1985).
12. Named predicate with explicit narrow lifetime before initial frame — DIFFER(2059v1985).
13. Repair native slot483 reference-output appearance seed; same head-guard family — DIFFER(2047v1985).
14. Original PDB failure adapter TRetVoid Return at each generated early exit — DIFFER(2047v1985).
15. One RAII exit path outside resource scope using structured goto finish — DIFFER(2047v1985).
16. Acquisition retry expressed with explicit bool state and for-loop update — DIFFER(2099v1985).
17. Short-circuit event selection followed by switch over mutually exclusive contact phases — DIFFER(2119v1985).
18. Explicit genuine ScriptedThing constructor body allows separate inlining budget for late hero resource — DIFFER(2047v1985).
19. Original ScriptThing copy imported from its separate source unit, destructor still available inline — DIFFER(1935v1985).
20. Recheck source family from attempt 4 with corrected native slot483; not a new strategy — DIFFER(2152v1985).
21. Recheck source family from attempt 7 with corrected native slot483; not a new strategy — DIFFER(2172v1985).
22. Recheck source family from attempt 8 with corrected native slot483; not a new strategy — DIFFER(2176v1985).
23. Recheck source family from attempt 9 with corrected native slot483; not a new strategy — DIFFER(2059v1985).
24. Recheck source family from attempt 10 with corrected native slot483; not a new strategy — DIFFER(2122v1985).
25. Recheck source family from attempt 12 with corrected native slot483; not a new strategy — DIFFER(2059v1985).
26. Combine separate-unit copy with original guarded do-while script loop — DIFFER(1944v1985).
27. ScriptThing destructor explicitly releases the original counted handle before member cleanup — DIFFER(1823v1985).
28. Freeze selected natural family26 with original private CDefPointer Object access; metadata repair only — DIFFER(1944v1985).

Selected28 freezes26 with private CDefPointer Object access. Native and object frames both0x98. The selected source correctly imports predicate copy but also calls the copy for four native expanded by-value arguments. Native first predicate/movie/map stack slots and early versus late resource destruction differ; native1985 versus selected1944. Keeping copy inline gave2047/2059 and expanded the first predicate incorrectly. Cleanup/inlining remains unresolved.

GNU64 C++03 and native VC /GR- tests pass. This is not a byte match.
