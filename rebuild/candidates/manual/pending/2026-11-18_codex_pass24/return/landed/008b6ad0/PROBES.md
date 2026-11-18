Ego_d local port. Each tag gives the perpendicular construction, its position relative to normalization, and the dot receiver.

- ctor_early_norm_facing: DIFFER, 344 bytes, 12 instruction diff hunks.
- ctor_early_norm_target: DIFFER, 344 bytes, 12 instruction diff hunks.
- ctor_early_perp_facing: DIFFER, 344 bytes, 7 instruction diff hunks.
- ctor_early_perp_target: DIFFER, 344 bytes, 7 instruction diff hunks.
- ctor_late_perp_facing: DIFFER, 342 bytes, 1 instruction diff hunks.
- ctor_late_perp_target: DIFFER, 342 bytes, 1 instruction diff hunks.
- fields_early_norm_facing: DIFFER, 344 bytes, 12 instruction diff hunks.
- fields_early_norm_target: DIFFER, 344 bytes, 12 instruction diff hunks.
- fields_early_perp_facing: DIFFER, 342 bytes, 6 instruction diff hunks.
- fields_early_perp_target: DIFFER, 342 bytes, 6 instruction diff hunks.
- fields_late_perp_facing: MASKED_MATCH_PENDING_AUDIT, 344 bytes, 0 instruction diff hunks.
- fields_late_perp_target: MASKED_MATCH_PENDING_AUDIT, 344 bytes, 0 instruction diff hunks.
- fields_y_early_norm_facing: DIFFER, 344 bytes, 12 instruction diff hunks.
- fields_y_early_norm_target: DIFFER, 344 bytes, 12 instruction diff hunks.
- fields_y_early_perp_facing: DIFFER, 342 bytes, 6 instruction diff hunks.
- fields_y_early_perp_target: DIFFER, 342 bytes, 6 instruction diff hunks.
- fields_y_late_perp_facing: MASKED_MATCH_PENDING_AUDIT, 344 bytes, 0 instruction diff hunks.
- fields_y_late_perp_target: MASKED_MATCH_PENDING_AUDIT, 344 bytes, 0 instruction diff hunks.
- package_baseline: DIFFER, 342 bytes, 1 instruction diff hunks.
Selected fields_late_perp_target. Setting X/Y/Z individually after Normalise restores the missing x87 exchange. Both behavior builds pass.

The selected result is RELOCATION_MATCH (344 object bytes, 344 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
