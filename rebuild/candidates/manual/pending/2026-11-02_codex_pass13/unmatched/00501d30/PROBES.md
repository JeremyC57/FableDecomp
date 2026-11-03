# 00501d30 final handoff

Final status: **EXTENT_WRONG**. Selected grade: 6. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00501d30/PROBES.md

# 00501d30 probe ledger

Status: diagnostic corrected-extent candidate, independent main bounds review pending. Selected source is attempt04, confirmed with current private headers and1087-byte diagnostic extent in attempt05. Attempt02 repairs an include dependency and attempt05 is an extent confirmation; neither counts as a new source strategy. Three source strategies were needed to reach the exact candidate.

- Attempt01: 01: original file/parser locals and structured token dispatch — SRC_FAIL.
- Attempt02: 01 repair: include the original STLport map declaration for full CWorldMap fields — DIFFER(1099v1086).
- Attempt03: 02: original basic-string comparison body visible; restore STLport char fill overloads — DIFFER(1136v1086).
- Attempt04: 03: explicit boolean returns for text equality and early non-string token guard — DIFFER(1087v1086).
- Attempt05: diagnostic corrected extent includes compiler terminal INT3; independent main bounds review pending — RELOCATION_MATCH.

Behavior:158 exact-body scenarios pass GNU C++03 and VC7.1 /GR-. Reproduce with bash workers/engine/00501d30_test.sh; its explicit CL=/EHsc enables ISO exception unwind behavior for the invalid-quote mock tests. See SEMANTIC_COMMAND.json and TEST_COVERAGE.json.

Main independent confirmation 6: EXTENT_WRONG, GNU C++03 and native VC7.1 /GR- tests PASS; 83 relocation operands verified. Assigned1086 omits terminal compiler INT3 at0050216e; complete1087-byte function matches, then one alignment INT3 before00502170. Diagnostic corrected extent only; excluded from assigned matches.
