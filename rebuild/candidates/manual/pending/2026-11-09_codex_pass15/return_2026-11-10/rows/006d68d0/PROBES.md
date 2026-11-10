# 006d68d0 attempts

Selected: 006_complete_original_trainable_enum;495/495 relocation match, tests PASS.

| Trial | Result | Bytes | Hunks |
|---|---|---|---|
| 001_shared_headers_direct_fractions | DIFFER | 495/495 | 2 |
| 002_distribute_skill_weight | MASKED_MATCH_PENDING_AUDIT | 495/495 | 0 |

The baseline shared-header rebase preserves the old one-instruction scheduling residue. Named sum is inert; const-reference sum changes additional arithmetic; distributing the skill weights matches. The two row bodies differ only in the proven threshold members and function identity. No speculative helper or compiler output patch is used.


## Complete original trainable-stat enum revalidation

The old empty EHeroTrainableStatType was an invalid model for nonzero stat values and could be optimized incorrectly under strict enum assumptions. Both original raw donor records now prove all11 constants (ten stats plus the count sentinel), underlying int: Ego_r0xd084 and FableWin0xce6b. The source uses the exact recovered HERO_STAT names for all six accessed indices. A new shared EHeroTrainableStatType.h is included only through FABLE_RETAIL_EHEROTRAINABLESTATTYPE; default and previous retail-header branches preprocess unchanged. The standalone test pastes the complete enum and allocates stat fixtures using NUMBER_OF_TRAINABLE_HERO_STATS.

Fresh normal RTM grade 006_complete_original_trainable_enum retains all495 bytes/153 instructions and all four literal relocation offsets/symbols from the audited selected predecessor. All1028 behavior cases pass VC7.1/Od/W3 and optimized g++ C++03 with-fstrict-enums, AddressSanitizer and UndefinedBehaviorSanitizer. Original tests and header were preserved under runtime/hero_enum_before_006d68d0_sol before editing; all prior history remains unchanged. ENUM_REVALIDATION.json records fingerprints and logs. This is a type/correctness repair, not a new instruction-matching source strategy.

## Main complete-enum revalidation

FINAL_002_complete_original_enums restores the complete original enum definitions from both raw donor records. Fixed-value source spellings now use original enumerators where applicable. Target bytes, full Leaf and all relocation identities remain unchanged. Fresh VC7.1 /GR- and g++03 /O2 -fstrict-enums ASan/UBSan tests pass. This corrects type fidelity and portable-test validity; it is not a new matching attempt. Main verification and fresh logs are retained in FINAL_002 history.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
