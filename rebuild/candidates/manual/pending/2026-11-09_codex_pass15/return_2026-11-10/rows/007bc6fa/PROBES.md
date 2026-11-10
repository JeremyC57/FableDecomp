# 007bc6fa attempts

Selected: 015_final_complete_headers. RELOCATION_MATCH;514/514 bytes,179/179 instructions, all22 relocation operands audited, behavior PASS.

| Trial | Result | Bytes | Strict hunks |
|---|---|---|---|
| 001_shared_hierarchy | DIFFER | 516/514 | 7 |
| 002_named_target_distance | DIFFER | 514/514 | 2 |
| 003_squared_unparenthesised | DIFFER | 514/514 | 2 |
| 004_magnitude_direct_sum | DIFFER | 514/514 | 2 |
| 005_magnitude_implicit_return_cast | DIFFER | 514/514 | 2 |
| 006_scalar_distance_components | DIFFER | 514/514 | 2 |
| 007_scalar_lead_components | DIFFER | 486/514 | 18 |
| 008_named_scaled_vector | DIFFER | 514/514 | 6 |
| 009_multiply_named_result | DIFFER | 528/514 | 9 |
| 010_multiply_grouped_components | DIFFER | 518/514 | 3 |
| 011_multiply_grouped_x | MASKED_MATCH_PENDING_AUDIT | 514/514 | 0 |
| 012_multiply_grouped_y | MASKED_MATCH_PENDING_AUDIT | 514/514 | 0 |
| 013_multiply_grouped_xy | DIFFER | 514/514 | 2 |
| 014_final_optin | MASKED_MATCH_PENDING_AUDIT | 514/514 | 0 |
| 015_final_complete_headers | MASKED_MATCH_PENDING_AUDIT | 514/514 | 0 |

The shared-header rebase replaces the old local component, pointer, vector and physics models. Naming the target item before the distance expression reproduces the retail target/shooter call order and removes the two-byte spill difference.

Measured scheduling: baseline DAG2 has83 nodes (81 interior tuples), ending in two x87 pops. The remaining pop and physics reload are in DAG3, where the reload is scheduled between the second and third pops. Grouping only the X product inside the real C3DVector::operator* adds one no-code0x269 tuple at each of the two lead multiplications. DAG2 then ends at the Y target store, leaving all three pops in DAG3; the reload schedules before them exactly as retail. X-only and Y-only grouping both match; grouping X/Y moves the reload too early. Grouping all three also changes the Z multiplication. The retained X grouping is a narrow opt-in with all default header bodies unchanged.

003–006 magnitude/distance spellings preserve the original residue. 007 scalar lead components reduces stores and changes size;008 named scaled vector and009 named multiply result alter the expected homes. These are discarded.

Diagnostic traces002 and014 were built through the trace backend and checked byte-identical to the normal unmodified RTM compiler. No diagnostic backend or object is part of the deliverable.

Standalone C++03 behavior fixture copies the selected function exactly plus real vector/pointer accessor bodies. It passes VC7.1 /Od /W3 and64-bit g++ -std=c++03 without task include paths. Six cases cover dead-target/shooter short circuiting, zero/positive/negative/truncated accuracy, target lead, normalized frame, speed/FPS acceleration, call order and disabling gravity.

Final audit: history/007bc6fa/015_final_complete_headers/RELOCATION_AUDIT.json and HEADER_DEFAULT_CHECK.txt. Test logs: TEST_VC_BUILD.txt, TEST_VC_RUN.txt, TEST_GXX_BUILD.txt, TEST_GXX_RUN.txt.

## Main complete-enum revalidation

FINAL_002_complete_original_enums restores the complete original enum definitions from both raw donor records. Fixed-value source spellings now use original enumerators where applicable. Target bytes, full Leaf and all relocation identities remain unchanged. Fresh VC7.1 /GR- and g++03 /O2 -fstrict-enums ASan/UBSan tests pass. This corrects type fidelity and portable-test validity; it is not a new matching attempt. Main verification and fresh logs are retained in FINAL_002 history.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
