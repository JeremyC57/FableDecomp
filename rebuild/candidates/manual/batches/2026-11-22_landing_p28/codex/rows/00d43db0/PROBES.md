# Native VC7.1 experiment log

Baseline: 1665 bytes, retail 1799, 509/560 normalized instructions.

| Probe | Source shape | Native result | Instructions aligned |
|---|---|---:|---:|
| `v3_shared_layout` | Use shared script classes and the debug RAII pause owner, retaining existing inline-depth controls. | 1603 | 376 |
| `v3_plain_cleanup` | Let normal scope choose destructor expansion with the RAII pause owner. | 1934 | 469 |
| `v3_direct_health` | Apply the health check as the if expression instead of storing its boolean result. | 1934 | 469 |
| `v3_script_frame_for` | Put frame advancement in the for loop update clause and keep scope-based destruction. | 1821 | 539 |
| `v3_declared_handles` | Keep script handle destruction and movie construction out of line, preserving the shared scope guards. | 1515 | 438 |
| `v4_early_null` | Match the zero-coordinate fall-through with the original early-return spelling. | 1821 | 542 |
| `v4_mid_budget` | Probe normal RAII branches with a moderate inline budget for the movie constructor. | 1632 | 316 |
| `v4_movie_inference` | Use the declaration-only movie constructor at both sites and keep the pause operator naturally scoped. | 1783 | 538 |
| `v4_hit_movie_cold` | Scope inline depth to the hit-movie declaration only; preserve native RAII cleanup. | 1799 | 543 |
| `priority_pause_goto` | Let both movie paths leave through a common scope-exit label so RAII runs on each terminating edge. | 1801 | 492 |
| `priority_pause_wrapped` | Use generated-script do-while checks in both movie waits and their subsequent checks. | 1799 | 543 |
| `priority_pause_continue` | Use a positive continuation edge in the movie waits; leave cleanup to scope exit on termination. | 1945 | 505 |
| `priority_pause_outer_do` | Make the loop’s exit a bottom test after the first guard rather than a for increment. | 1900 | 525 |
| `priority_pause_body_scope` | Scope each movie, pause owner and wait inside a separate statement block. | 1911 | 517 |
| `priority_pause_getpos` | Use the out-of-line GetPos owner’s behavior with a local implementation pointer and reference result. | 1799 | 543 |
| `priority_pause_scope_returns` | Use direct failure macros at both post-wait sites while guarding the inline decisions in both waits. | 1796 | 465 |
| `priority_pause_tail_flags` | See whether the per-function inlining threshold restores source-observed cleanup block ordering. | 1983 | 511 |
| `refine_tail_bare` | Use a bare termination return only at move/speech termination sites. | 1799 | 543 |
| `refine_tail_wait_for` | Express the task wait as a for loop with frame advance in the body and identical termination checks. | 1799 | 543 |
| `refine_tail_tested` | Express successful wait and termination on complementary if/else arms after each movie wait. | 1799 | 543 |
| `refine_tail_alive_direct` | Use the direct positive health comparison in both movie scopes, as FableWin has no bool temporary. | 1799 | 543 |
| `refine_tail_includes` | Use the quest includes in neighbor order, to measure whether compiler symbol order shifts the cleanup placement. | 1799 | 543 |
| `refine_tail_while` | Keep hit movie constructor out of line, but use the plain pretest while loop with frame at the bottom. | 1916 | 528 |
| `followup_tail_talk_goto` | Only the talk movie leaves through the generated return label; the hit cleanup is still a direct return. | 1799 | 542 |
| `followup_tail_hit_goto` | Only the hit movie leaves through the generated return label; the talk cleanup is still a direct return. | 1799 | 542 |
| `followup_tail_both_goto` | Both movie cleanup paths leave through an explicit tail with a normal return before the label. | 1799 | 552 |
| `followup_tail_original_members` | Use the original protected Game reference in the entity-script base layout. | 1799 | 543 |
| `followup_tail_break_wait` | Use plain breaks to merge the task wait into the next termination gate without moving the second runtime check. | 1765 | 539 |
| `last_maze_scope_guards` | At the listed movie guards, leave the local movie via the shared script-return path: 1, 2, 4, 5. | 1799 | 554 |
| `last_maze_scope_loop` | At the listed movie guards, leave the local movie via the shared script-return path: 1, 4. | 1799 | 543 |
| `last_maze_scope_post` | At the listed movie guards, leave the local movie via the shared script-return path: 2, 5. | 1799 | 543 |
| `last_maze_scope_all_but_talk` | At the listed movie guards, leave the local movie via the shared script-return path: 0, 1, 3, 4, 5. | 1799 | 541 |
| `last_maze_scope_all_but_hit` | At the listed movie guards, leave the local movie via the shared script-return path: 0, 1, 2, 3, 4. | 1799 | 552 |
| `last_maze_scope_deep_1` | At the listed movie guards, leave the local movie via the shared script-return path: 1, 2, 5. | 1799 | 554 |
| `last_maze_scope_deep_2` | At the listed movie guards, leave the local movie via the shared script-return path: 2, 4, 5. | 1799 | 543 |
| `tie_maze_implicit_tail` | Use the failure holder on the ordinary final fall-through instead of the redundant void return. | 1801 | 492 |
| `tie_maze_bare_shared_tail` | Use an ordinary void return at the shared cleanup label, retaining the original failure holder elsewhere. | 1799 | 554 |
| `tie_maze_guard_values` | Use named conditions on the two return edges of the first pause scope. | 1799 | 554 |
| `tie_maze_guard_last_value` | Give the last guard its own condition value. | 1799 | 554 |
| `tie_maze_exact_reference` | Keep the original CEntityScriptBase Game reference through both movie cleanup branches. | 1799 | 554 |
| `tie_maze_exact_include_order` | Use declaration order from the sibling’s TU before the game interface and script math headers. | 1799 | 554 |
| `tie_maze_exact_helper_reorder` | Define the native position accessor before the inline ScriptThing destructor. | 1799 | 554 |
| `tie_maze_exact_helpers_last` | Move the inline helper definitions after the main member definition and test compiler function ordering. | 1799 | 554 |
| `tie_maze_exact_wait_then_guard` | Use for-loop task waits and explicit early polarity for their stop edges inside each local movie scope. | 2105 | 535 |
| `tie_maze_original_ctor` | Use the existing single-TU forced-inline pause constructor opt-in and preserve the exact original dtor body. | 1799 | 554 |
| `v2_shared_layout` | Use shared script classes and the debug RAII pause owner, retaining existing inline-depth controls. | COMPILE_FAIL | – |
| `v2_plain_cleanup` | Let normal scope choose destructor expansion with the RAII pause owner. | COMPILE_FAIL | – |
| `v2_direct_health` | Apply the health check as the if expression instead of storing its boolean result. | COMPILE_FAIL | – |
| `v2_script_frame_for` | Put frame advancement in the for loop update clause and keep scope-based destruction. | COMPILE_FAIL | – |
| `v2_declared_handles` | Keep script handle destruction and movie construction out of line, preserving the shared scope guards. | COMPILE_FAIL | – |
| `evidence_evidence_tail_labels` | Give talk and hit cleanup paths separate labels at the same exit statement. | 1799 | 554 |
| `evidence_evidence_distinct_returns` | Use a distinct void return for each movie scope, retaining the original failure holder. | 1799 | 543 |
| `evidence_evidence_shared_block` | Put the outer script loop and common cleanup jump target in one guarded scope. | 1799 | 554 |
| `evidence_evidence_scope_break` | Break the scripted frame loop on successful exit and use the explicit script-ended label. | 1799 | 560 |
| `evidence_evidence_split_postwait` | Give clean and interrupted path guards independent exit labels for each movie scope. | 1799 | 554 |
| `evidence_evidence_separate_exit_cast` | Keep the original return holder while making its void conversion explicit at the cleanup target. | 1799 | 554 |
| `evidence_evidence_value_return` | Return through an isolated failure-holder lifetime on the composite exit edge. | 1799 | 554 |
| `evidence_evidence_wait_do` | Spell each wait as an initial conditional do-while, preserving its first check and the trailing guard. | 1799 | 554 |
| `evidence_evidence_exit_labels_inside` | Put each scope exit on an explicit return in the movie branch before falling to the shared tail. | 1799 | 554 |
| `evidence_evidence_guard_call_form` | Retain the guard boolean across the cleanup jump to see whether C2 has a live EAX on the resulting tail edge. | 1799 | 554 |
| `owner_pointer_local` | Use a local receiver pointer in the original pause destructor. | 1799 | 554 |
| `owner_reference_local` | Use a local receiver reference in the original pause destructor. | 1799 | 554 |
| `owner_const_pointer` | Make the destructor local receiver pointer const. | 1799 | 554 |
| `owner_bool_local` | Name the original false bool argument inside the pause destructor. | 1799 | 554 |
| `owner_reference_to_pointer` | Bind a reference to the original pointer field to test receiver aliasing. | 1799 | 554 |
| `owner_ctor_assign` | Initialize the pause owner in the constructor’s body before the same virtual true call. | 1799 | 554 |
| `owner_ctor_reference` | Call through the constructor’s reference formal after storing the equivalent receiver pointer. | 1799 | 554 |

Selected `evidence_evidence_scope_break`. The exact branch shape is a for loop with a frame-advance expression and an explicit break, plus the existing four scope-unwinding return jumps. The former for-loop predicate form differed only by eight register encodings in four cleanup-tail virtual calls.
