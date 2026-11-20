# fixed_009bf7e0 (audit_roll, 2026-11-20): hand3c2 b14 0x009BF7E0 CDisplayManager::CreateDevice + two owner re-ports
Rows: 009bf7e0 (new), 009be340 ReadDeviceRefreshRate (re-port of "CDisplayManager::GetRefreshRate"), 009be380 CalcPresentationIntervalTime
(re-port of "_global::FUN_009be380"). Strict vland --recheck-landed with FABLE_HDR_OVERLAY=hdr: RELOCATION_MATCH / RELOCATION_MATCH / MATCH, PASS x3
(vland_strict.log). Full audit in $AW/REPORT.md (section b14).
Files: src/, test/, payload.json, oracle.tsv, hdr/ (full files on today's tree) + hdr_*.diff, hdr_sha256.txt (today's headers at audit time),
NAME_FIXES.tsv, mutants*.tsv / mutants*.log, ppcmp_includers.log, includer_regrade_strict_on.txt, new_header_trips.txt, binding_splits.txt,
egod_lines.tsv, odlines_fixed.txt, probes.txt, audit_chain.log.
Changes against the author's b14: GetD3DFormat -> GetAsD3DPixelFormat (existing opt-in; CPixelFormat.h dropped from the overlay); the three
expanded .cpp functions are same-TU copies in the row (Fidelity lines), the header declares them only; HRESULT hr; Ego_d parameter names in the
header; `// Inferred:` on the message-less throw; comment corrections. The author's test is unchanged.
