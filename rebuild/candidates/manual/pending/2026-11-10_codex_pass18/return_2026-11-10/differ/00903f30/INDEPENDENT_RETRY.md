# Independent route retry

Selected version remains the main original GetTC(id)/Find/Compare-chain source:254/254,89/89 instructions,10 strict hunks, sole EBX/EBP role interchange. No main row was mutated by this worker. All independent candidates/headers are in runtime/route_sol and graded histories are history/00903f30/sol_*.

Distinct strategy families: conversion CFG (ternary/default initializer/direct local switch), conversion evaluation lifetime (direct constructor argument/earlier pure conversion/early default with later overrides), parameter snapshot, explicit argument alias lifetime, nested/inverted guards. Const/declaration-assignment forms are syntax-only checks and are not counted as additional source strategies; flags are CPU diagnostics only. Original main probes already satisfy the required minimum independently.

Attempt | Result | Size | Strict/loose hunks
---|---|---|---
sol_s01_ternary_conversion | DIFFER | 253/254 | 8/8
sol_s02_default_initialized_conversion | DIFFER | 252/254 | 9/9
sol_s03_nested_else_conversion | DIFFER | 253/254 | 8/8
sol_s04_conversion_direct_argument | DIFFER | 280/254 | 13/8
sol_s05_conversion_declared_outer | DIFFER | 254/254 | 10/10
sol_s06_route_deferred_assignment | DIFFER | 254/254 | 10/10
sol_s07_local_switch_direct | DIFFER | 254/254 | 10/10
sol_s08_const_route_and_movement | DIFFER | 254/254 | 10/10
sol_s09_condition_inverted_else | DIFFER | 254/254 | 10/10
sol_s10_nested_alive_conditions | COMPILE_FAIL | ?/? | ?/?
sol_s11_nested_alive_corrected | DIFFER | 254/254 | 10/10
sol_s12_shared_sneak_default_label | DIFFER | 254/254 | 10/10
sol_s13_move_conversion_early | DIFFER | 250/254 | 5/4
sol_s14_argument_reference_alias | DIFFER | 254/254 | 10/10
sol_s15_argument_pointer_alias | DIFFER | 254/254 | 10/10
sol_s16_argument_reference_last | DIFFER | 254/254 | 10/10
sol_s17_move_parameter_snapshot | DIFFER | 254/254 | 10/10
sol_s18_default_early_delayed_overrides | DIFFER | 252/254 | 9/6
sol_cpu_G5 | DIFFER | 254/254 | 10/10
sol_cpu_G7 | DIFFER | 254/254 | 12/12

Strongest causal experiment: sol_s13_move_conversion_early computes ai_move_type before route.GetPThing(). It achieves correct EBX(route)/EBP(movement) allocation but moves the switch before the virtual GetPThing call and moves the EBP pop after Wait,250/254. Thus an extra call crossed by movement decreases its priority enough to reverse allocation, as predicted by the original diagnostic ai_move_type0/proute-3. Same-order variants preserve both the code and the wrong priority ordering. Early-default/later-overrides and ternary conversion change emitted instructions, so they are not retained.

Current remaining cause is a measured allocation-priority residue, with no credible further source lever from this retry. Preserve the authentic254-byte source and mark PARKED if no additional genuine helper evidence closes it. No new inline or layout devices, no selected flags.
