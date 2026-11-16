# SubmitFrame parent probes

Historical hand601 source retained:415/415 with frame28vs2c, bad local models and mislabeled CODA calls. The new source reuses real headers first.

- main_submit01_shared: DIFFER, 415/415, first differing byte 23.
- main_submit02_desc_function_scope: DIFFER, 415/415, first differing byte 23.
- main_submit03_real_break_body: DIFFER, 415/415, first differing byte 23.
- main_submit04_desc_dependency_order: DIFFER, 415/415, first differing byte 23.
- main_submit05_aggregate_descriptor: DIFFER, 415/415, first differing byte 23.
- main_submit06_named_break_lifetime: DIFFER, 415/415, first differing byte 23.
- main_submit07_real_guard_ctor: DIFFER, 415/415, first differing byte 23.
- main_submit08_early_mailbox_return: DIFFER, 415/415, first differing byte 216.
- main_submit09_size_before_rate: MASKED_MATCH_PENDING_AUDIT, 415/415, first differing byte None.
- main_submit10_final_physical_model: MASKED_MATCH_PENDING_AUDIT, 415/415, first differing byte None.
- main_submit11_channel_type_proof: MASKED_MATCH_PENDING_AUDIT, 415/415, first differing byte None.

Strategies: genuine shared rebase; function-scope descriptor only (inert); genuine complete DebugBreak body (worse); dependency-order descriptor assignments (closer); aggregate descriptor initialization (inert); named break lifetime (worse); genuine complete guard constructor (inert); early invalid-return with guard moved to function scope (exact frame); size-before-rate assignment (exact parent). Final model/prototype/native-offset rechecks are not additional independent source strategies.
All source/header snapshots and compiler results are preserved. The wrapper syntax error before the first build was corrected without a compiler attempt.
