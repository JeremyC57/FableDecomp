# Native VC7.1 experiment log

Baseline: 2424 bytes, retail 2084, 610/635 normalized instructions.

| Probe | Source shape | Native result | Instructions aligned |
|---|---|---:|---:|
| `v3_retvoid` | Use the proven TRetVoid and CScriptDef layout with straightforward loop control. | 2420 | 612 |
| `v3_frame_for` | Use a conditional for loop and native frame advance in its update clause. | 2388 | 610 |
| `v3_flee_scope` | Keep flee and anger processing in scoped statements independently from the toll handling. | 2676 | 610 |
| `v3_warning_guard` | Use early next-frame flow when the toll conditions are unsatisfied. | 2460 | 557 |
| `v3_inline_threshold` | Reduce the inliner growth budget to check flee and anger cleanup against toll processing. | 1932 | 445 |
| `v4_priorities` | Use a proven local move type and rely on inline timer and accessor defaults. | 2376 | 564 |
| `v4_called_timer` | Make the implicit timer destructor visible only as a declaration and retain the conditional frame loop. | 2300 | 607 |
| `v4_flee_budget` | Use moderate VC7.1 budget on the ordinary frame loop. | 1984 | 462 |
| `v4_escape_body` | Select escape coordinates with an early-return implementation and keep the flee body scoped. | 2388 | 610 |
| `priority_flee_cold` | Keep inline depth scoped to three wait-loop returns and the final flee/anger returns. | 2204 | 617 |
| `priority_flee_colder` | Cool every termination return in the flee and anger handling. | 2072 | 620 |
| `priority_flee_wait_exit` | Use ordinary scope to move only wait-loop returns across the flee and anger cleanup blocks. | 2212 | 603 |
| `priority_flee_shared_return` | Make the framework’s shared termination return explicit for the two scripted-control waits. | 2088 | 519 |
| `priority_flee_end_dtor` | Use an out-of-line scripted thing handle destructor and leave timer inlining loadable. | 2208 | 600 |
| `priority_flee_body_budget` | Set inline size before per-statement control; check the score against the mixed timer lifetime. | 2400 | 611 |
| `refine_toll_getpos` | Retain the implementation pointer across the null check as the original 18-byte GetPos body does. | 2072 | 620 |
| `refine_toll_macro` | Use generated-script termination macros in the paid toll branch after cooling flee and anger exits. | 2072 | 620 |
| `refine_toll_first_macro` | Use the do-while termination macro at the shared timer-service site only. | 2072 | 620 |
| `refine_toll_equal_timer` | Use the CTimer comparison operator rather than raw time query in the paid toll test. | 2072 | 620 |
| `refine_toll_bare` | Use bare returns at paid toll guards for the native timer-service scheduling comparison. | 2072 | 620 |
| `refine_toll_guard` | Move to the next frame early when the toll remains unpaid, then handle the countdown and threat. | 2068 | 558 |
| `refine_toll_timer_first` | Use a named timer result and keep the termination check immediately before the timer lookup. | 2072 | 620 |
| `refine_toll_loop_check` | Put frame advancement and the goto-free failure guard at the end, with an initial guard before entering. | 1952 | 552 |
| `followup_toll_vtable_ref` | Use the original entity script Game reference rather than pointer syntax. | 2072 | 620 |
| `followup_toll_timer_else` | Express the paid toll timer condition as the else branch of the terminating return. | 2072 | 620 |
| `followup_toll_timer_pointer` | Take the timer service pointer in the caller and use the original timer index accessor for the guarded time read. | 2084 | 615 |
| `followup_toll_timer_scope` | Place only the paid guard’s cleanup in its own do-while scope and keep its decision as an ordinary timer call. | 2072 | 620 |
| `followup_toll_timer_index_first` | Preserve the timer index before selecting the GetTimer or automatic destructor site. | 2084 | 571 |
| `retvoid` | Use the proven TRetVoid and CScriptDef layout with straightforward loop control. | COMPILE_FAIL | – |
| `native_retvoid` | Use the proven TRetVoid and CScriptDef layout with straightforward loop control. | COMPILE_FAIL | – |
| `v2_retvoid` | Use the proven TRetVoid and CScriptDef layout with straightforward loop control. | COMPILE_FAIL | – |
| `native_frame_for` | Use a conditional for loop and native frame advance in its update clause. | COMPILE_FAIL | – |
| `frame_for` | Use a conditional for loop and native frame advance in its update clause. | COMPILE_FAIL | – |
| `v2_frame_for` | Use a conditional for loop and native frame advance in its update clause. | COMPILE_FAIL | – |
| `native_flee_scope` | Keep flee and anger processing in scoped statements independently from the toll handling. | COMPILE_FAIL | – |
| `v2_flee_scope` | Keep flee and anger processing in scoped statements independently from the toll handling. | COMPILE_FAIL | – |
| `flee_scope` | Keep flee and anger processing in scoped statements independently from the toll handling. | COMPILE_FAIL | – |
| `v2_warning_guard` | Use early next-frame flow when the toll conditions are unsatisfied. | COMPILE_FAIL | – |
| `warning_guard` | Use early next-frame flow when the toll conditions are unsatisfied. | COMPILE_FAIL | – |
| `native_warning_guard` | Use early next-frame flow when the toll conditions are unsatisfied. | COMPILE_FAIL | – |
| `v2_inline_threshold` | Reduce the inliner growth budget to check flee and anger cleanup against toll processing. | COMPILE_FAIL | – |
| `inline_threshold` | Reduce the inliner growth budget to check flee and anger cleanup against toll processing. | COMPILE_FAIL | – |
| `native_inline_threshold` | Reduce the inliner growth budget to check flee and anger cleanup against toll processing. | COMPILE_FAIL | – |
| `exits_toll_outer_break` | Use the in-body explicit termination break while keeping the normal frame advance expression. | 2072 | 629 |
| `exits_toll_break_receiver` | Combine an explicit outer break with a local timer-service pointer at the paid toll exit. | 2084 | 621 |
| `exits_toll_break_index` | Keep the timer index and reference from the original local check and use an explicit outer break. | 2084 | 564 |
| `exits_toll_paid_fail_label` | Route only the paid toll-state termination to a common function return after scope unwinding. | 2072 | 629 |
| `exits_toll_paid_outer_scope` | Use an explicit failure break only at the paid-toll entry, carrying timer teardown through the main loop scope. | 2072 | 624 |
| `exits_toll_paid_continue` | Skip the paid branch on a live warning, keeping the top loop termination and frame order. | 2084 | 580 |
| `exits_toll_fade_function` | Use the package fade helper declaration, and keep the natural named movement policy from FableWin. | 2072 | 629 |
| `toll12_toll_targeted_two` | Allow ordinary destructor inlining at the two retail inline-timer exits, the movement wait and anger entry. | 2252 | 626 |
| `toll12_toll_targeted_two_wrapped` | Restore script termination statement scoping at the two retail inline-timer exits. | 2252 | 626 |
| `toll12_toll_targeted_movement` | Restore ordinary inlining only at the movement wait exit to isolate its cleanup choice. | 2240 | 625 |
| `toll12_toll_targeted_anger` | Restore ordinary inlining only at the anger-entry exit to isolate its cleanup choice. | 2072 | 629 |
| `toll12_toll_targeted_move_policy` | Use the donor movement-policy local alongside the two original hardware scope transitions. | 2252 | 626 |
| `toll12_toll_targeted_ctor_split` | Use original reference-form Game member while restoring the two confirmed inline-timer sites. | 2252 | 626 |
| `tollscope_toll_timer_only_unwind` | Keep the timer as the only object in the selected goto unwind, with earlier handles destroyed at a shared cold return. | 2108 | 630 |
| `tollscope_toll_timer_shared_unwind` | Send all three retail inline-timer guards through a single timer-scope unwind and cold handle cleanup. | 2084 | 630 |
| `tollscope_toll_timer_plain_tail` | Use ordinary cleanup expansion after separate timer-scope unwinds for all three selected guards. | 2236 | 629 |
| `tollscope_toll_timer_wrapped_scope` | Use wrapped generated termination statements for the two flee/anger timer-scope exits. | 2108 | 630 |
| `tollfin_toll_all_timer_edges` | Route the exact five flee/anger branches and the paid toll entry through the timer-only cleanup. | 2084 | 630 |
| `tollfin_toll_exact_timer_edges` | Use the exact inline-timer branch targets and keep all destructors cold at the post-time check. | 2084 | 630 |
| `tollfin_toll_all_timer_else` | Use the exact timer teardown edges and express the active countdown as an early next-frame edge. | 2112 | 573 |
| `tollabi_toll_typed_global` | Use the original global CDefPointer<const CScriptDef> and established name. | 2084 | 630 |
| `tollabi_toll_typed_deref` | Use the proven original direct dereference on the constant script-def handle. | 2084 | 630 |
| `tollabi_toll_typed_donor_locals` | Use the exact typed script-def global and the donor local movement policy. | 2084 | 630 |

Selected `tollabi_toll_typed_donor_locals`. The outer explicit break yields the original common timer-service receiver and argument setup. A separate WarningTimer scope drives the original divided cleanup, including three early flee terminations and the movement-wait, anger-entry and paid-entry edges. The body and decoded local jump-table targets match retail.
