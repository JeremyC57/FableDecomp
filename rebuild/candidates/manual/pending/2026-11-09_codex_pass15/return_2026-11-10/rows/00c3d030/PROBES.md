# 00c3d030 current attempts

MATCHED_PENDING_INDEPENDENT_VERIFICATION: selected034 freezes source031. Each history directory contains the exact source/header snapshot and compiler result. Older package attempts remain untouched.

| Trial | Result | Bytes (ours/retail) | Strict hunks |
|---|---|---|---|
| 001_shared_vendor_header | PREPROCESS_FAIL | -/- | - |
| 001b_include_path_fix | DIFFER | 98/94 | 4 |
| 002_hidden_result | DIFFER | 94/94 | 8 |
| 003_output_reference | DIFFER | 98/94 | 4 |
| 004_first_store_before_branch | DIFFER | 96/94 | 4 |
| 005_explicit_tail_returns | DIFFER | 98/94 | 4 |
| 006_separate_counts | DIFFER | 98/94 | 4 |
| 007_first_selection_before_arm | DIFFER | 96/94 | 6 |
| 008_named_platform_condition | DIFFER | 98/94 | 4 |
| 009_branch_local_first_count | DIFFER | 98/94 | 4 |
| 010_negated_outer_branch | DIFFER | 92/94 | 6 |
| 011_size_optimisation | DIFFER | 92/94 | 5 |

Original selected baseline: 001b_include_path_fix; replaced by frozen034 after source031 matched.

001 is preprocessing failure, not a source-shape attempt.002 tests a hidden-result ABI hypothesis and is rejected.011 is a flag experiment, not another source shape. Exact vendor method spelling/constness remain unproven.

## Follow-up source strategies

012–017 test pointer/count parentheses, output aliases, first-field reference and explicit platform snapshot. All canonicalize to the same98-byte baseline; these are recorded experiments, not six distinct optimizer mechanisms.018–019 move first selection/store outside the later branch while snapshotting platform state once (unlike007, this preserves aliasing semantics if output overlaps context); both100 bytes.020 explicit goto and021 boolean switch test CFG construction independently. No improvement selected.

Actual C2 scheduler trace for001b confirms the entry DAG has only platform load, maximum load, test and branch; the output-pointer load is already absent before scheduling. Thus this residue cannot be repaired solely by scheduling those existing nodes. Diagnostic object matches the normal RTM object; schedule_trace.txt, schedule_dags.json and validation are preserved in001b history.

Standalone exact-body test now covers8,750 combinations: absent/present platform; four independently negative/zero/positive maxima; seven patterns of live counts including negative/zero. It checks returned pointer, all four outputs, guard words and unchanged maxima. VC7.1/Od and g++03 PASS. Test includes complete context/list field declarations from the selected header, with private visibility opened only for fixtures; root allocation operators are irrelevant and omitted from the test fixture.

## CPU scheduling flag checks

022 /G5 and 023 /G6 retain the98-byte baseline.024 /G7 emits97 bytes and a different stream; it does not match94-byte retail. These are three compiler-flag checks, not three genuinely distinct source shapes. Selected001b remains unchanged.


## Fresh source/ABI review025–034

025 non-const member and026 reference result with pointer argument are ABI-compatible signature diagnostics. Both reproduce the98-byte baseline and do not establish a different original declaration.027 conditional-value stores on the first three fields also canonicalize to baseline.028 using that form for the last field changes the native duplicated tail and emits100 bytes.029 /Og- emits219 bytes; this is a compiler-flag check, not another source mechanism.030 an explicit return from the active outer arm reproduces baseline.

031 replaces each of the first three `if (count>0) count=list.GetNumFree(); output->field=count;` pairs with explicit two-arm output stores: `if (count>0) output->field=list.GetNumFree(); else output->field=count;`. The final field already had that form. This preserves the native read/store sequence, including storage overlap effects, but retains a different compiler CFG before optimization. It matches94/94 bytes and35/35 instructions exactly.032 negates each two-arm condition and also matches; it is the same mechanism, not an independent win.033 adds the required address/flags heading;034 corrects vendor provenance comments only. Neither is counted as a new source-shape attempt. Frozen selection:034_final_vendor_provenance_comment.

The actual033 C2 entry DAG contains the output-pointer load missing from001b. Its measured issue order is platform load, first maximum load, test, output-pointer load, branch, exactly native. The diagnostic object matches the normal RTM leaf. This solves the pre-scheduler sinking residue through genuine control/data-flow structure, without padding, opaque calls or compiler instrumentation in production.

Standalone exact selected-body tests PASS under VC7.1/Od and g++03. The original8750 ordinary defined-object cases remain. An additional78750 implementation-overlap diagnostics test both platform conditions, every maximum combination and seven count patterns, placing output windows across future maxima or later free counters so early stores affect later reads. Expected results use byte-addressed four-byte integer loads/stores in native order and compare the entire initialized context representation. These overlap windows reinterpret class storage as CRESOURCEDESC and therefore do not claim strict C++ object-lifetime/aliasing guarantees; g++ uses-O0 -fno-strict-aliasing for these diagnostics. All87500 total cases pass.

AUDIT.json records source/test fingerprints and zero selected-leaf relocations; OBJDUMP_LEAF_AUDIT.txt retains complete object disassembly. VENDOR_COMPILER_EVIDENCE.txt corrects the donor debug/release layout distinction and records SDK RTM compiler provenance. No shared package input changed.

## Main verification035

Fresh exact RTM regrade matches94/94bytes and35instructions, no relocations. Test output corrected to the required exact address_TEST PASS line. Added optional FABLE_TEST_DEFINED_ONLY to omit representation-overlap diagnostics for an optimized ASan/UBSan run of8750 ordinary cases; default still runs78750 additional implementation-level overlap checks. All fresh VC71 /GR-, g++03/O0 no-strict-alias and defined-only sanitized runs pass. Prior worker test is frozen in035 history. Full Leaf remains descriptive under explicit vendor exception, not a PDB-proven function spelling.

## Final handoff authority

Current status: VERIFIED_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
