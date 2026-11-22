# Native VC7.1 experiment log

Baseline: 1714 bytes, retail 1830, 481/542 normalized instructions.

| Probe | Source shape | Native result | Instructions aligned |
|---|---|---:|---:|
| `v3_shared_layout` | Replace stand-ins with shared Oakvale script layout, timer calls and the FableWin failure holder. | 1714 | 481 |
| `v3_plain_cleanup` | Let VC7.1 choose cleanup sites using only normal scope and FableWin locals. | 1714 | 456 |
| `v3_countdown_local` | Read the first timer result into the FableWin Fred local while keeping the second read in the bar expression. | 1714 | 456 |
| `v3_common_open` | Merge the open-gates arms explicitly after the region logic to test whether branching causes the tail differences. | 1714 | 481 |
| `v3_while_frame` | Move script frame advancement to the end of a pretest outer while loop. | 1693 | 341 |
| `v4_cleanup_wrap` | Use same-TU termination macro blocks around only the post-countdown exits. | 1714 | 456 |
| `v4_bar_scope` | Nest the gate timer lifetime beneath the closed-gate wait. | 1714 | 456 |
| `v4_noinline_timer` | Use out-of-line timer destruction to compare whether remaining shared exits are cleanup copy mismatches. | 1721 | 352 |
| `v4_native_timeout` | Use the original initially unshouted local and Fred clock-read local with shared timers. | 1715 | 454 |

First handoff selected `v3_shared_layout`. 1714 versus 1830 bytes. Three separated inline two-handle cleanup sequences in retail are merged here. Some timer-service lookups stay on different sides of termination checks.

The instruction count measures alignment independently of byte equality. See RESULT.json for the byte grade.

## Main-chat retry

FableWin shows a loop with a frame-advance increment and an explicit termination return inside the body. Implementing it restores the separate timer and handle cleanup sequences. The named Fred timer read and Shouted local come from the same debug procedure. One four-byte store still differed. Moving the BanditsKilled reset after the two repopulation branches, while keeping it inside the LeftRegion branch, retains the retail OpenGates write at 00DC0DA5. The two assignments have independent gameplay roles even when the compiler merges neighboring blocks.

The final comparison covers 542 instructions, 72 named bindings and 9 literals. The native fixture exercises the wait for a closed gate, countdown reset, shout timers, region exit and re-entry, bandit repopulation and termination during active timers.

| Native probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| solo_oak_return_plain | 1826 | 513 | 3 / 0 |
| solo_oak_return | 1826 | 541 | 3 / 0 |
| solo_oak_break | 1825 | 528 | 3 / 0 |
| solo_oak_break_plain | 1825 | 499 | 3 / 0 |
| solo_oak_post | 1826 | 541 | 3 / 0 |
| solo_oak_post_plain | 1826 | 513 | 3 / 0 |
| solo_oak_scope_1 | 1720 | 486 | 3 / 0 |
| solo_oak_scope_2 | 1715 | 486 | 3 / 0 |
| solo_oak_scope_3 | 1715 | 486 | 3 / 0 |
| solo_oak_scope_12 | 1616 | 453 | 3 / 0 |
| solo_oak_scope_23 | 1611 | 453 | 3 / 0 |
| solo_oak_scope_34 | 1715 | 505 | 3 / 0 |
| solo_oak_scope_123 | 1482 | 432 | 3 / 0 |
| solo_oak_scope_456 | 1820 | 519 | 3 / 0 |
| solo_oak_killed_first | 1826 | 536 | 3 / 0 |
| solo_oak_shared_reset | 1830 | 542 | 3 / 0 |
| solo_oak_invert_join | 1830 | 536 | 3 / 0 |
| solo_oak_joined_continue | 1773 | 427 | 3 / 0 |
| solo_oak_joined_return_else | 1830 | 542 | 3 / 0 |
| solo_oak_joined_reverse_stores | 1830 | 542 | 3 / 0 |
| solo_oak_joined_nested_arms | 1830 | 542 | 3 / 0 |
| solo_oak_arms_continue | 1773 | 427 | 3 / 0 |
| solo_oak_arms_return_else | 1826 | 541 | 3 / 0 |
| solo_oak_arms_reverse_stores | 1826 | 541 | 3 / 0 |
| solo_oak_arms_nested_arms | 1826 | 541 | 3 / 0 |

Final selection: `solo_oak_shared_reset`. All 1,830 bytes, 81 relocated operands and three mapped inline copies verified.
