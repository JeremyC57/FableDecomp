# Pass8 current handoff

Read this file first. This is the current selected work for authorized pass8 N001–N358 only: 979 functions. No repository files were edited, no commit or push was performed by this work, and no row was landed here.

Current results: **917/979 verified functions (93.67%)**, including 912 ordinary C++ and 5 genuine original-instruction exceptions. Matched function bytes: 95068/104121 (91.31%). This percentage is for this authorized pass, not the whole game or the prior Claude baseline.

Remaining: 43 PARKED, 9 REVIEW_HELD (excluded even if byte/test checks pass), 10 BLOCKED, zero UNSTARTED. Four blocked custom-ABI rows have evidence only and no fabricated source/test. Every other row has its selected source and standalone fixture in SELECTED. See ROWS.tsv, ROW_DETAILS.md, per-row ROW.json and NAME_RECOVERY.md.

## Verify and land on your own lane

1. Verify SHA256SUMS and run `python3 verify_package.py` from this directory. Retain all lifetime counts: **four native source grades per row**, including failed/duplicate experiments. Root imports of Sol grades are the same attempts, never extra budget. An unchanged dry-verifier rerun or fixture-only correction is not a new source strategy.
2. Read INPUT_PASS/BRIEF.md and the original INPUT_RULES, especially owner overrides. Use your retail executable with SHA1 39fd416ba23a91a89713ec80c789f57ddb22e2c8 and your unchanged verification tools, compiler13.10.3077, normal flags/O2 /Oy /W3 plus per-row extras. Recut your oracle locally; no oracle or retail binary bytes are included here.
3. Regrade payload.json and payload_retail_asm.json separately using your unchanged `tools/decomp_pipeline/linux_lane/lane_tools/vland.py <payload> <oracle>` in dry mode. Re-run the standalone fixtures. Match source/test text against SELECTED and the payload; older receipt payloads and generators are historical, not a combined landing queue.
4. Review every relocation target and the cross-row binding audit. Name-vs-propagated-label warnings are documented. Compiler-local _$E2 callbacks are translation-unit locals; their different retail addresses do not constitute a shared external symbol. Only the coordinator should decide whether and how to land after its own checks.
5. Keep all parked/review-held/blocked rows out of the accepted payload. A cap is not reset by this archive. Any future extra source attempts require new authorization. Follow the recorded best-selection metadata rather than assuming the latest experiment is the closest.

## What is included

- payload.json and payload_retail_asm.json contain only the current locally verified selections. src/, test/, retail_asm/ mirror them. SELECTED also includes every unresolved row; differ/ is a convenience copy with an added status comment and is not the authoritative native source hash.
- history/main and history/sol retain available source variants, recorded grades, diff-offset metadata and compiler/test transcripts. Imported duplicate records remain visible. First30 rows N001–N010 have original attempt logs but some overwritten early unsuccessful source texts were not preserved; see history/legacy_N001_N010_ATTEMPTS.json. Never claim those missing versions exist.
- receipts/ contains preserved original review snapshots and chronology. Some original snapshot hash manifests reference deliberately excluded binaries/logs or scratch files; use the root SHA256SUMS for this delivery. The latest SELECTED/current payload wins over older receipts.
- verification/ contains actual behavior runs and dry-verifier receipts. Root vland.log is an explicitly labeled consolidation of 917 current-selection PASS receipts, not a claim of one combined invocation; CURRENT_VERIFIER_RECEIPTS.json links each exact selected source/test payload to its original log. Selected unmatched fixtures also received a strict C++03 host check; four evidence-only ABI blockers have no test. Historical FAIL records remain intact. A passing behavior fixture never converts a DIFFER or model-review hold into a match.
- gen/ retains generators and amendment scripts. Many contain original absolute workspace paths and older model versions; do not blindly regenerate. Port paths deliberately and apply the chronological amendments, or use the authoritative selected files directly.
- support/include holds unchanged project headers referenced by selections with hashes. It contains no Microsoft compiler/header distribution. The coordinator's matching repository headers are preferred after hash comparison.

## Important review debt and corrections

Review-held rows are listed separately in STATUS/ROW_DETAILS: complete model/header/template ancestry or signature debt remains despite some byte/test passes. They are not accepted. Capped0090f770 has a documented ungraded successor strategy proven on its school/home siblings; do not silently exceed its cap. Similar capped sibling notes are preserved.

N035 fixtures originally used a non-C++03 host unsignedlonglong alias. Main pedantic review caught it; the fixture-only LP64 correction and both rerun histories are preserved. Native grade4 sources were unchanged; their host-only source branch remains a documented portability limitation. N241 constructor evidence prose accidentally spelled the class CPPPointerInfo; the exact original PDB symbol and sources correctly use CCPPointerInfo. This spelling correction is recorded in NOTES. N1340090e3d0 grade4 was an accidental duplicate after setup failed, not the /G7 strategy named in its original record; GRADE_04_CORRECTION.json explains this, and the attempt still counts.

Names and layouts are evidence-based with uncertainty retained. Local variable names were not recovered. Some inactive storage or unknown library types remain explicitly descriptive projections; consult per-row evidence before extending those models. Neither guessed manifest labels nor donor-build addresses are retail identity proof.

This archive excludes Wine, compilers, executable/object files, Git history, oracle data, raw retail disassembly and previous delivery ZIPs. It is the work and review evidence only.
