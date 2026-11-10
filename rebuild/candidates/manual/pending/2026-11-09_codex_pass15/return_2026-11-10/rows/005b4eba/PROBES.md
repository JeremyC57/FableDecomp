# 005b4eba attempts

Frozen:007_final_portable_headers. Exact3372 bytes/1105 instructions after audited relocations;184 relocations and both switches independently PASS.

| Trial | Strategy | Result | Bytes | Strict / loose share |
|---|---|---|---|---|
| 001_complete_first_source | Complete first source/shared headers | DIFFER | 3413/3372 | 0.6216 / 0.8849 |
| 002_visible_actual_definition_helpers | Actual GetDef/GetSubDef bodies remove output capture | DIFFER | 3352/3372 | 0.6540 / 0.9637 |
| 003_iterator_lifetimes_and_list_end | Cached loop ends, native append order, current-modifier reference | DIFFER | 3377/3372 | 0.7712 / 0.9801 |
| 004_direct_response_and_proven_bases | Direct current-map access; RTTI-proven subdef bases | DIFFER | 3373/3372 | 0.7719 / 0.9819 |
| 005_visible_actual_thing_alive | Actual plain IS_THING_ALIVE body removes final pointer capture; match | MASKED_MATCH_PENDING_AUDIT | 3372/3372 | 1.0000 / 1.0000 |
| 006_canonical_const_alive_pointer | Canonical const pointer symbol; identity-only adjustment | MASKED_MATCH_PENDING_AUDIT | 3372/3372 | 1.0000 / 1.0000 |
| 007_final_portable_headers | Guard/include audit regrade, not a source strategy | MASKED_MATCH_PENDING_AUDIT | 3372/3372 | 1.0000 / 1.0000 |
| 008_main_independent_regrade | Main independent validation regrade | MASKED_MATCH_PENDING_AUDIT | 3372/3372 | 1.0000 / 1.0000 |

Five source strategies001–005.006 is canonical identity correction;007 and main regrades are validation, not additional source-shape attempts. No park decision:005 matched and006/007 retain it.

Native IS_THING_ALIVE00436380 reads the passed pointer and tests Dead+91 without retaining the reference. A declaration-only call exposed the local, causing one extra frame word(94 versus90), a post-call reload and widespread slot permutations. Defining the actual ordinary specialization made it uncaptured while RTM naturally retained the call; all3372 bytes matched at once. No inline control, padding, extra operation or compiler/object patch was used.

Important body fixes: append current modifiers at list end; cache the true loop ends; keep the current modifier through a const reference; preserve direct map access to infer non-null usage. The two switch chains and all literal pairings are independently audited. The standalone test body is compared exactly with selected production and passes native64 g++03 and real VC7.1/Od/W3.

## Main complete-enum revalidation

FINAL_002_complete_original_enums restores the complete original enum definitions from both raw donor records. Fixed-value source spellings now use original enumerators where applicable. Target bytes, full Leaf and all relocation identities remain unchanged. Fresh VC7.1 /GR- and g++03 /O2 -fstrict-enums ASan/UBSan tests pass. This corrects type fidelity and portable-test validity; it is not a new matching attempt. Main verification and fresh logs are retained in FINAL_002 history.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
