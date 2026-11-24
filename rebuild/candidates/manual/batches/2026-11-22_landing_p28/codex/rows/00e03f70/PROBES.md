# Native VC7.1 experiment log

Baseline: 1986 bytes, retail 1906, 543/593 normalized instructions.

| Probe | Source shape | Native result | Instructions aligned |
|---|---|---:|---:|
| `v2_shared_layout` | Replace all stand-ins with original script and handle headers and use the FableWin failure holder. | 1747 | 425 |
| `v2_plain_cleanup` | Allow the compiler to choose inlining at all destructor sites. | 1747 | 425 |
| `v2_angle_accessor` | Use the original named virtual accessor at every angle site with its authentic null fallback. | 1739 | 430 |
| `v2_frame_for` | Put frame advance in each loop header while retaining its exact initial ordering. | 1747 | 425 |
| `v2_declared_destructor` | Use the shared out-of-line ScriptThing destructor at every returned handle lifetime. | 1675 | 123 |
| `v4_member_angle` | Use original member calls consistently and expanded GetPos/IsAlive positive branches. | 1700 | 237 |
| `v4_scoped_respawn` | Use original FableWin local scopes and expansion of the assignment operator. | 1859 | 469 |
| `v4_block_budget` | Sweep a larger compiler inline growth budget on the ordinary two-loop form. | 2052 | 392 |
| `v4_late_respawn_scope` | Cool each second-phase respawn assignment independently to match retail temporary cleanup. | 1741 | 420 |
| `followup_authentic_get_pos` | Use the native GetPos implementation pointer so the out-of-line copy tail-forwards correctly. | 1885 | 519 |
| `last_alive_owner` | Retain the CScriptThing implementation pointer once, as the original IsAlive owner does. | 1885 | 519 |

First handoff selected `last_alive_owner`. 1885 versus 1906 bytes. All six bound copies match. Temporary handle destruction and angle scheduling differ at respawn sites. Shared headers now supply the original quest flag and virtual interfaces.

The instruction count measures alignment independently of byte equality. See RESULT.json for the byte grade.

## Main-chat retry

The FableWin caller has four direct calls to CScriptThing::IsDead and eight calls to both GetAngleXY and GetPos. Ego_r and FableWin agree on the method spellings. In retail the four IsDead evaluations each call slot 75 (IsAlive) on the outer handle. The previous candidate called IsAlive on its implementation. Restoring the IsDead wrapper reproduced all four calls. The out-of-line GetAngleXY at 004AAA40 returns 0.0f on a null implementation and uses a virtual call followed by ret on the other path. Removing parentheses around that returned expression produces a tail jump in VC7.1. The reconstructed getter reproduces the complete 20-byte owner copy and its zero literal at 0122DEDC.

The donor puts RockTroll in the function-level local list. Removing the extra lexical scope fixed the last ten slot displacements in the second respawn loop and the shutdown effects. The final comparison covers 593 instructions, 76 named bindings and 18 literals. The native fixture exercises live and missing barrier locations with exact expected angles, both effects alive or dead, phase transitions, effect removal and early termination.

| Native probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| solo_angle_declared_alive | 1927 | 519 | 5 / 1 |
| solo_angle_owner | 1948 | 519 | 6 / 1 |
| solo_angle_late_calls | 1867 | 537 | 5 / 1 |
| solo_alive_ref | 1880 | 545 | 5 / 1 |
| solo_alive_cast | 1867 | 537 | 5 / 1 |
| solo_alive_late_scope | 1867 | 545 | 5 / 1 |
| solo_alive_dispatch | 1877 | 527 | 5 / 0 |
| solo_dead_fallthrough | 1940 | 535 | 5 / 1 |
| solo_dead_virtual | 1940 | 527 | 5 / 1 |
| solo_dead_isnull | 1940 | 527 | 5 / 1 |
| solo_dead_isnull_tail1 | 1940 | 527 | 5 / 1 |
| solo_dead_parens | 1906 | 583 | 6 / 0 |
| solo_dead_parens_tail1 | 1906 | 583 | 6 / 0 |
| solo_dead_round | 1940 | 527 | 5 / 1 |
| solo_dead_round_tail1 | 1940 | 527 | 5 / 1 |
| solo_dead_conditional | 1940 | 527 | 5 / 1 |
| solo_dead_conditional_tail1 | 1940 | 527 | 5 / 1 |
| solo_dead_result | 1865 | 557 | 5 / 1 |
| solo_dead_result_tail1 | 1865 | 557 | 5 / 1 |
| solo_flat_locals | 1906 | 593 | 6 / 0 |
| solo_flat_respawn | 1906 | 593 | 6 / 0 |
| solo_late_loop_scope | 1906 | 593 | 6 / 0 |
| solo_first_loop_scope | 1906 | 593 | 6 / 0 |
| solo_both_loop_scopes | 1906 | 593 | 6 / 0 |
| solo_decl_before_check | 1906 | 593 | 6 / 0 |

Final selection: `solo_flat_locals`. All 1,906 bytes, 94 relocated operands and six mapped inline copies verified.
