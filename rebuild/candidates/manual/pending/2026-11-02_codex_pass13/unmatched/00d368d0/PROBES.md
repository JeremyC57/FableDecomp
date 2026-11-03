# 00d368d0 final handoff

Final status: **IN_PROGRESS**. Selected grade: 16. Worker-selected nonmatch; final independent integration/review not completed. Do not land as a match.

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00d368d0/PROBES.md

# 00d368d0 probes

1. Original PDB owner, nested layout and local scopes; native branch reconstruction: DIFFER(2020v2147).
2. Same-TU non-inline original timer method definitions: DIFFER(2020v2147).
3. Final termination call with direct fallthrough cleanup: DIFFER(2020v2147).
4. Movement position locals live through remaining dialogue scope: DIFFER(2026v2147).
5. Original timer GetTime expression at branch use: DIFFER(2019v2147).
6. Direct void guard returns without empty macro result local: DIFFER(2020v2147).
7. Prefix line increment inside wrap condition: DIFFER(2020v2147).
8. Explicit random float multiplication and integer timer conversion: DIFFER(2026v2147).
9. Original resource assignment body explicitly available inline: DIFFER(2020v2147).
10. Same-TU original MovieBase constructor before script body: DIFFER(2020v2147).
11. Guarded do loop with native bottom mission test: DIFFER(2016v2147).
12. Time optimization after source scope and same-TU methods exhausted: DIFFER(2020v2147).
13. Explicit random multiply shape with Ob1 automatic expansion control: DIFFER(2026v2147).
14. Explicit random multiply shape without global optimizer: DIFFER(3801v2147).
15. Select original scopes and native two-multiply random interval expression: DIFFER(2026v2147).
16. Original base Game and MasterScriptData reference correction, preserved native layout: DIFFER(2026v2147).

Attempts1-11 test source shape;12-14 are optimizer controls;15 verifies selection. Source8 restores native two-multiply sequence instead of folded random scale.

Reference correction: selected attempt16 adopts original CEntityScriptBase Game reference and CLevelScriptBase Game/MasterScriptData references, with original access. Extracted Leaf bytes equal attempt15 exactly; GNU64 and VC7.1 fixtures were rerun and pass. See current SELECTION.json.
