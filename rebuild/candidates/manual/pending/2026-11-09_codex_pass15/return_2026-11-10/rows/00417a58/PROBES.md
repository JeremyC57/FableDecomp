# 00417a58 source attempts

Selected/frozen:007_final_bank_interface;2097/2097 masked match. All198 relocations independently checked; self-contained behavior tests PASS native64 g++03 and VC7.1/Od/W3.

| Trial | Source strategy | Result | Bytes | Strict/loose instruction share |
|---|---|---|---|---|
| 001_complete_first_source | First complete three-loop reconstruction, typed shared headers, explicit string conversions | DIFFER | 2094/2097 | 0.7175 / 0.9350 |
| 002_literal_and_implicit_conversions | Native wide literals and implicit CDefString→CCharString conversion at calls | DIFFER | 2106/2097 | 0.6205 / 0.9383 |
| 003_named_definition_references | Named const CSoundDef references (rejected: increased frame and extra instructions) | DIFFER | 2129/2097 | 0.5053 / 0.9053 |
| 004_visible_definition_lookup | Actual ordinary same-TU GetDef specialization makes output handles uncaptured; complete byte match | MASKED_MATCH_PENDING_AUDIT | 2097/2097 | 1.0000 / 1.0000 |
| 005_pdb_lookup_signature | Correct lookup to PDB const pointer-output specialization; removes wrong bound ref-output signature, bytes preserved | MASKED_MATCH_PENDING_AUDIT | 2097/2097 | 1.0000 / 1.0000 |
| 006_final_audited_headers | Audit/setup regrade only: final comment and full native bank storage assertion | MASKED_MATCH_PENDING_AUDIT | 2097/2097 | 1.0000 / 1.0000 |
| 007_final_bank_interface | Header audit regrade only: correct descriptive unused bank-slot signatures from actual overrides | MASKED_MATCH_PENDING_AUDIT | 2097/2097 | 1.0000 / 1.0000 |

There are four distinct body/source strategies001–004. Trial005 is the necessary naming correction;006/007 are validation/header-audit regrades, not additional source-shape attempts. No parking threshold is relevant because004 already matches;005/007 preserve it with corrected names and complete headers.

The first source already covers all658 native instructions semantically. Its uncaptured-output issue allowed a reused extension literal to occupy edi and left the localized handle in memory; named raw references made the problem worse. Giving the real GetDef body visibility let c2 keep definition handles in edi/esi and pack the frame from80 to7c, simultaneously removing all extra reloads and matching every offset. Plain ordinary C++ sufficed; no inline setting, padding, extra operation, or compiler/object mutation was used.

The final source/header snapshot is history/00417a58/007_final_bank_interface. It is frozen for main re-verification. Identity corrections and descriptive audio limitations are explicit in NAME_FIXES.tsv and NAMING_EVIDENCE.md.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_002_sound_default_gate. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
