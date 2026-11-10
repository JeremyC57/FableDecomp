# GFGetHermiteSplinePosition attempts

Historical input: hand529 and hand895 drafts preserved under input/. New measured probes:

| Attempt | Source shape | Result |
|---|---|---|
| 000 | Supplied shared-header baseline, unchanged | DIFFER 295/295; four instruction-order hunks at final copy; exact Leaf confirmed |
| 001 | Name s2+s2 separately | DIFFER 295/295, identical to baseline |
| 002 | Use 2.0f*s2 instead of parenthesised s2+s2 | MASKED MATCH 295/295; all92 instructions identical |
| 003 | Use existing portable FABLE_FASTCALL macro; final source regrade | RELOCATION_MATCH; both float constants checked; VC7.1 /Od and g++ C++03 behaviour PASS (68 checks including all four output/input aliases) |

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
