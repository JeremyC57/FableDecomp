# 0095bcf0 probes

Historical local-model attempts from hand278/Q12/Q13 were read before new authoring. Baseline records are diagnostics, not accepted sources. No old local models or fake bodies remain.

| Attempt | Shape / grade |
|---|---|
| sol_baseline_localmodel | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_final01 | MASKED_MATCH_PENDING_AUDIT 948v948; 0 strict/0 loose hunks |
| sol_final02_host32 | MASKED_MATCH_PENDING_AUDIT 948v948; 0 strict/0 loose hunks |
| sol_final03_aggregate_rebase | MASKED_MATCH_PENDING_AUDIT 948v948; 0 strict/0 loose hunks |
| sol_mathshape_01 | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_mathshape_02 | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_mathshape_03 | DIFFER 948v948; 5 strict/5 loose hunks |
| sol_mathshape_04 | DIFFER 948v948; 7 strict/7 loose hunks |
| sol_mathshape_05 | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_mathshape_06 | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_newtonshape_01 | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_newtonshape_02 | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_newtonshape_03 | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_newtonshape_04 | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_newtonshape_05 | DIFFER 948v948; 2 strict/2 loose hunks |
| sol_operatorshape_01 | DIFFER 948v948; 17 strict/4 loose hunks |
| sol_operatorshape_02 | DIFFER 948v948; 17 strict/4 loose hunks |
| sol_operatorshape_03 | MASKED_MATCH_PENDING_AUDIT 948v948; 0 strict/0 loose hunks |
| sol_port01 | COMPILE_FAIL v948;  strict/ loose hunks |
| sol_port02_typed_pointer | DIFFER 986v948; 39 strict/11 loose hunks |
| sol_port03_real_subdef_scope | COMPILE_FAIL v948;  strict/ loose hunks |
| sol_port04_real_subdef_scope | DIFFER 948v948; 2 strict/2 loose hunks |

Key strategy families: genuine shared class migration; correctly typed Get()/converting constructor; real visible GetSubDef body and lifetime scope; normalise square/local/helper forms; original union/Newton expression forms; named-result/Initialise/grouped vector operators. Named-result and Initialise subtraction changed the frame/tails; grouped X/Y subtraction matched. Existing FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP moves the no-code scheduler cut before the next GetPItem setup.

Final sol_final03_aggregate_rebase:948bytes291instructions,0hunks. All52relocations across the row and its genuine182-byteGetSubDef COMDAT audited. The helper already landed at0095c5c0; it is not an extra reserved match. Native/Od/W3/noEHsc and GCC64C++03 pass.

Behavior:14scenarios covering initial/runnable gates, missing/dead target, missing/dead first circle marker, stopping, denied target, losing attacker state during retry, three movement iterations, failed movement return, pre-action/post-action gates, timed pause loop, pause interruption, and zero pause. A complete event string asserts call ordering through both loops. Final runnable-gate assertions prove the def handle has released before that gate.
