# Native VC7.1 source experiments

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `baseline` | Regrade the bundled legacy source. | DIFFER 1758/1784 | 284 |
| `shared_geometry` | Restore shared geometry while retaining the previous local owner model. | DIFFER 1699/1784 | 322 |
| `engine_vector` | Trial port to shared C3DVector with overlapping declarations. | COMPILE_FAIL |  |
| `briar_complete_centre` | Restore the complete three-component donor centre arithmetic. | DIFFER 1743/1784 | 309 |
| `r_briar_shared_initial` | Port the owner and dependencies to shared classes. | COMPILE_FAIL |  |
| `r_briar_shared_container` | Select the STLport vector reserve and push-back owners. | COMPILE_FAIL |  |
| `r_briar_shared_emitter` | Use the complete CTCDriverBase-derived particle emitter and retain the donor locals. | DIFFER 1777/1784 | 341 |
| `r_briar_shared_scope_timer` | Compare a narrower timer temporary lifetime. | DIFFER 1777/1784 | 341 |
| `r_briar_shared_counted_release` | Route the definition temporary through the pointee release helper. | DIFFER 1777/1784 | 341 |
| `r_briar_shared_direct_ptr_assign` | Use the pointer assignment expansion without a named intermediate. | DIFFER 1777/1784 | 341 |
| `r_briar_shared_snap_position` | Evaluate the marker coordinate before looking up the physics component. | DIFFER 1805/1784 | 319 |
| `r_briar_shared_pointer_constructor` | Force inline the original intelligent-pointer constructor. | DIFFER 1777/1784 | 341 |
| `r_briar_shared_free_brace` | Compare a narrower effect-definition scope. | DIFFER 1777/1784 | 400 |
| `r_briar_shared_rng_local` | Use the existing GFRandom result-local body. | DIFFER 1757/1784 | 346 |
| `r_briar_shared_effect_first_branch` | Try an early return when the effects definition is missing. | DIFFER 1821/1784 | 319 |
| `r_briar_shared_no_extra_flag` | Regrade the donor-preserving shared port with the unused switch removed. | DIFFER 1777/1784 | 341 |
| `r_briar_shared_timer_statement_split` | Compare a compound if body for the timer. | DIFFER 1777/1784 | 341 |
| `r_briar_shared_body_const` | Use the donor type literal directly. | DIFFER 1777/1784 | 341 |
| `r_briar_shared_cpp03_casts` | Compare explicit conversions for timer and marker count. | DIFFER 1777/1784 | 341 |
| `r_briar_shared_dir_components` | Construct each direction local directly from the subtraction result. | DIFFER 1777/1784 | 341 |

Selected `r_briar_shared_no_extra_flag` with the donor effect-definition scope. The narrower-scope trial aligned more instructions, but the original effect-def pointer is a function-scope local.

## Continued main-chat grades

Verified all 1,784 bytes and all 503 instructions. The 92 leaf operands resolved at their exact retail sites. All 25 mapped inline copies passed. The marker-search copy at 008E8AC0 independently matched its full 153 bytes and all three callee targets. The 96-case VC7.1 fixture passed. Implicit CCharString arguments in the marker, spawn, hit and particle-effect calls restored the stack slots. The authentic GetSubDef, effect lookup and marker search bodies preserved the original marker pointer across calls. The effects definition remains in the scope shown by Ego_d.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_briarown_act | COMPILE_FAIL |  |  /  |
| round2_briarown_char | 1777 | 341 | 12 / 1 |
| round2_briarown_def | 1778 | 403 | 14 / 0 |
| round2_briarown_joint | COMPILE_FAIL |  |  /  |
| round2_briarown_post | COMPILE_FAIL |  |  /  |
| round2_briarscan_debug | 1777 | 400 | 15 / 1 |
| round2_briarscan_mask | 1777 | 400 | 15 / 1 |
| round2_briarscope_action | 1770 | 402 | 15 / 0 |
| round2_briarscope_all | 1756 | 418 | 27 / 2 |
| round2_briarscope_fx | 1778 | 403 | 18 / 0 |
| round2_briarscope_reserve | 1778 | 403 | 16 / 2 |
| round2_briarscope_scanno | 1769 | 364 | 19 / 0 |
| round2_briarscope_scanpush | 1764 | 419 | 21 / 0 |
| round2_briarscope_withreserve | 1764 | 419 | 22 / 2 |
| round2_briarstr_all | 1784 | 502 | 21 / 0 |
| round2_briarstr_effects | 1776 | 439 | 21 / 0 |
| round2_briarstr_fxall | 1784 | 503 | 25 / 0 |
| round2_briarstr_marker | 1764 | 394 | 21 / 0 |
| round2_briarstr_searchspawn | 1772 | 406 | 21 / 0 |
