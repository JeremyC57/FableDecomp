# Native VC7.1 experiment log

Baseline: 2456 bytes, retail 2088, 585/643 normalized instructions.

| Probe | Source shape | Native result | Instructions aligned |
|---|---|---:|---:|
| `native_retvoid` | Use RAII shared quest layout and the debug macro return holder. | 2456 | 585 |
| `native_short_scope` | End the trader lifetime on a successful attack through an explicit shared break label. | 2456 | 585 |
| `native_signed_count` | Take the loop count from the vector once per outer pass using FableWin long loop variable. | 2440 | 510 |
| `native_trader_lifetime` | Leave the trader destructor out of line while preserving the inline assignment and new return holder. | 2248 | 497 |
| `native_guarded_attack` | Reject the non-attacking path early, keeping an attack scope that owns both dialog choices. | 2424 | 545 |
| `v4_copy_ctor_declared` | Use the original out-of-line vector copy constructor and keep the vector owner shared. | 2456 | 585 |
| `v4_one_attack_line` | Choose the known line before a single conversation path while preserving type-specific target choice. | 2264 | 470 |
| `v4_wrapped_terminate` | Restore statement macro wrapping exclusively at the four trader-selection termination sites. | 2424 | 545 |
| `v4_short_budget` | Reduce inlining growth to see whether the trader destructor copies move to the shared cleanup. | 1588 | 384 |
| `followup_owned_assignment` | Use the owned counted-pointer assignment with normal CScriptThing scope, allowing the retail assignment body to expand. | 2280 | 515 |
| `last_alive_owner` | Retain the CScriptThing implementation pointer once, as the original IsAlive owner does. | 2280 | 515 |

First handoff selected `last_alive_owner`. 2280 versus 2088 bytes. All eleven bound copies match. Trader destructor expansion differs at the successful-attack break, and the termination cleanup groups and hero targeting arm retain different schedules.

The instruction count measures alignment independently of byte equality. See RESULT.json for the byte grade.

## Main-chat retry

FableWin records signed long i, CreaturesAttacking initialized to zero, TotalCreaturesToAttack initialized from AllCreatures.size(), and the increment before the successful attack break. It calls CTimer::operator==(long) const in both conversation arms. The submitted source uses those locals and the original operator. Ordinary inline on the timer and script-handle assignments improved the leaf from 2,280 to 2,232 bytes and from 515 to 527 aligned instructions. All eleven bound copies still match.

Giving CScriptThing's destructor a visible body raised instruction alignment but expanded the leaf to 2,504 bytes. Limiting it only at the successful break did not recover the mix of inline and out-of-line cleanup. Continue from the per-call cleanup choices and the hero-target path.

| Native probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| solo_trader_locals | 2280 | 515 | 11 / 0 |
| solo_trader_timer | 2280 | 515 | 11 / 0 |
| solo_trader_per_site | 2280 | 515 | 11 / 0 |
| solo_trader_dtor | 2504 | 589 | 12 / 0 |
| solo_trader_dtor_break | 2440 | 502 | 12 / 0 |
| solo_trader_plain | 2232 | 527 | 11 / 0 |
| solo_trader_plain_dtor | 2456 | 580 | 12 / 0 |
| solo_trader_v_cleanup1 | 2280 | 515 | 11 / 0 |
| solo_trader_v_cleanup2 | 2280 | 515 | 11 / 0 |
| solo_trader_v_cleanup3 | 2280 | 515 | 11 / 0 |
| solo_trader_b_cleanup1 | 2504 | 589 | 12 / 0 |
| solo_trader_b_cleanup2 | 2504 | 589 | 12 / 0 |
| solo_trader_b_cleanup3 | 2504 | 589 | 12 / 0 |

Final selection: `solo_trader_plain`. 2,232 versus 2,088 bytes, with 527 of 643 instructions aligned. Trader cleanup at the attack break and control flow into the hero-target path still differ. All eleven mapped copies match.

## Continued main-chat grades

VC7.1 RTM 13.10.3077 reproduces all 2,088 bytes and all 643 decoded instructions on the final headers with /O2 /Oy /W3 /d2inlT70. The precise audit checks 72 named operands, five literal addresses, all 16 termination destinations, all 11 mapped inline copies and the additional 87-byte CScriptThing scalar deleting destructor at 004AB170. The 12 secondary owners contain 15 relocations, each checked against its retail site. Baselines, cleanup experiments, single-byte branch diagnosis, header ablations and final rechecks are recorded below.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_traderbrace_aftercounter | 2280 | 507 | 11 / 0 |
| round2_traderbrace_afterend0 | 2575 | 458 | 12 / 0 |
| round2_traderbrace_afterend1 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_afterend2 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_afterend3 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_aftertraderscope0 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_aftertraderscope1 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_aftertraderscope2 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_aftertraderscope3 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_bodycounter | 2504 | 581 | 12 / 0 |
| round2_traderbrace_bodyend0 | 2575 | 458 | 12 / 0 |
| round2_traderbrace_bodyend1 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_bodyend2 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_bodyend3 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_bodytraderscope0 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_bodytraderscope1 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_bodytraderscope2 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_bodytraderscope3 | 2504 | 589 | 12 / 0 |
| round2_traderbrace_mixedcounter | 2488 | 577 | 12 / 0 |
| round2_traderbrace_mixedend0 | 2563 | 548 | 12 / 0 |
| round2_traderbrace_mixedend1 | 2488 | 585 | 12 / 0 |
| round2_traderbrace_mixedend2 | 2488 | 585 | 12 / 0 |
| round2_traderbrace_mixedend3 | 2488 | 585 | 12 / 0 |
| round2_traderbrace_mixedtraderscope0 | 2488 | 585 | 12 / 0 |
| round2_traderbrace_mixedtraderscope1 | 2488 | 585 | 12 / 0 |
| round2_traderbrace_mixedtraderscope2 | 2488 | 585 | 12 / 0 |
| round2_traderbrace_mixedtraderscope3 | 2488 | 585 | 12 / 0 |
| round2_traderclean_all | 2088 | 643 | 12 / 0 |
| round2_traderclean_basic | 2088 | 643 | 12 / 0 |
| round2_traderclean_basicT | 2088 | 643 | 12 / 0 |
| round2_traderclean_nomark1 | 2200 | 642 | 12 / 0 |
| round2_traderclean_only | 2088 | 643 | 12 / 0 |
| round2_traderclean_onlyT | 2088 | 643 | 12 / 0 |
| round2_traderclean_timeronly | 2056 | 625 | 12 / 0 |
| round2_tradercopy21_COPY_DECL_ONLY | 2088 | 643 | 11 / 0 |
| round2_tradercopy21_COPY_OUTOFLINE | 2088 | 643 | 12 / 0 |
| round2_traderedge_implicit60s9 | 1822 | 501 | 12 / 0 |
| round2_traderedge_implicit62s11 | 2031 | 508 | 12 / 0 |
| round2_traderedge_implicit64s10 | 2088 | 521 | 12 / 0 |
| round2_traderedge_implicit64s9 | 2024 | 550 | 12 / 0 |
| round2_traderedge_implicit67s7 | 2024 | 550 | 12 / 0 |
| round2_traderedge_implicit67s9 | 2024 | 550 | 12 / 0 |
| round2_traderedge_implicit70s9 | 2200 | 553 | 12 / 0 |
| round2_traderedge_implicit72s7 | 2264 | 565 | 12 / 0 |
| round2_traderedge_nested60s9 | 1867 | 500 | 12 / 0 |
| round2_traderedge_nested62s11 | 2063 | 525 | 12 / 0 |
| round2_traderedge_nested64s10 | 2120 | 542 | 12 / 0 |
| round2_traderedge_nested64s9 | 2056 | 582 | 12 / 0 |
| round2_traderedge_nested67s7 | 2056 | 582 | 12 / 0 |
| round2_traderedge_nested67s9 | 2056 | 582 | 12 / 0 |
| round2_traderedge_nested70s9 | 2232 | 570 | 12 / 0 |
| round2_traderedge_nested72s7 | 2296 | 582 | 12 / 0 |
| round2_traderedge_owner60s9 | 1822 | 482 | 10 / 2 |
| round2_traderedge_owner62s11 | 2063 | 525 | 12 / 0 |
| round2_traderedge_owner64s10 | 2120 | 542 | 12 / 0 |
| round2_traderedge_owner64s9 | 2056 | 582 | 12 / 0 |
| round2_traderedge_owner67s7 | 2056 | 582 | 12 / 0 |
| round2_traderedge_owner67s9 | 2056 | 582 | 12 / 0 |
| round2_traderedge_owner70s9 | 2232 | 570 | 12 / 0 |
| round2_traderedge_owner72s7 | 2296 | 582 | 12 / 0 |
| round2_traderedge_site60s9 | 1867 | 500 | 12 / 0 |
| round2_traderedge_site62s11 | 2063 | 525 | 12 / 0 |
| round2_traderedge_site64s10 | 2120 | 542 | 12 / 0 |
| round2_traderedge_site64s9 | 2056 | 582 | 12 / 0 |
| round2_traderedge_site67s7 | 2056 | 582 | 12 / 0 |
| round2_traderedge_site67s9 | 2056 | 582 | 12 / 0 |
| round2_traderedge_site70s9 | 2232 | 570 | 12 / 0 |
| round2_traderedge_site72s7 | 2296 | 582 | 12 / 0 |
| round2_tradergenerated_after64s9 | 2072 | 588 | 12 / 0 |
| round2_tradergenerated_after68s9 | 2232 | 574 | 12 / 0 |
| round2_tradergenerated_frame64s9 | 1800 | 488 | 12 / 0 |
| round2_tradergenerated_frame68s9 | 1896 | 534 | 12 / 0 |
| round2_tradergenerated_loops60s11 | 1688 | 451 | 12 / 0 |
| round2_tradergenerated_loops64s9 | 1800 | 488 | 12 / 0 |
| round2_tradergenerated_loops68s9 | 1896 | 534 | 12 / 0 |
| round2_tradergenerated_loops71s8 | 2024 | 564 | 12 / 0 |
| round2_tradergenerated_timer61s8 | 1899 | 494 | 12 / 0 |
| round2_tradergenerated_timer62s9 | 2011 | 559 | 12 / 0 |
| round2_tradergenerated_timer63s9 | 2072 | 588 | 12 / 0 |
| round2_tradergenerated_timer64s9 | 2072 | 588 | 12 / 0 |
| round2_tradergenerated_timer65s10 | 2136 | 549 | 12 / 0 |
| round2_tradergenerated_timer65s12 | 2136 | 549 | 12 / 0 |
| round2_tradergenerated_timer66s8 | 2072 | 588 | 12 / 0 |
| round2_tradergenerated_timer67s10 | 2136 | 549 | 12 / 0 |
| round2_tradergenerated_timer68s11 | 2248 | 585 | 12 / 0 |
| round2_tradergenerated_timer68s9 | 2232 | 574 | 12 / 0 |
| round2_tradergenerated_timer69s8 | 2232 | 574 | 12 / 0 |
| round2_tradergenerated_timer70s8 | 2248 | 577 | 12 / 0 |
| round2_tradergenerated_timer71s8 | 2312 | 589 | 12 / 0 |
| round2_tradergenerated_timer72s8 | 2312 | 589 | 12 / 0 |
| round2_tradergenerated_timer73s8 | 2376 | 614 | 12 / 0 |
| round2_traderjoin21_p0block | 2088 | 643 | 12 / 0 |
| round2_traderjoin21_p0line | 2088 | 643 | 12 / 0 |
| round2_traderjoin21_p1block | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_p1line | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_p2block | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_p2line | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_p3block | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_p3line | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_p4block | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_p4line | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_p8block | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_p8line | 2200 | 642 | 12 / 0 |
| round2_traderjoin21_target3from4 | 2216 | 617 | 12 / 0 |
| round2_traderjoin21_target3from457 | 2168 | 610 | 12 / 0 |
| round2_traderjoin21_target3from57 | 2280 | 618 | 12 / 0 |
| round2_traderjoin21_target4from3 | 2216 | 617 | 12 / 0 |
| round2_traderjoin21_target4from357 | 2168 | 610 | 12 / 0 |
| round2_traderjoin21_target4from57 | 2216 | 618 | 12 / 0 |
| round2_traderjoin21_target5from34 | 2216 | 618 | 12 / 0 |
| round2_traderjoin21_target5from347 | 2168 | 610 | 12 / 0 |
| round2_traderjoin21_target5from4 | 2216 | 618 | 12 / 0 |
| round2_traderjoin21_target7from34 | 2216 | 617 | 12 / 0 |
| round2_traderjoin21_target7from345 | 2168 | 610 | 12 / 0 |
| round2_traderjoin21_target7from4 | 2264 | 616 | 12 / 0 |
| round2_traderlast_flowafterloop | 2008 | 482 | 12 / 0 |
| round2_traderlast_flowbeforeloop | 2088 | 643 | 12 / 0 |
| round2_traderlast_flowdo | 2120 | 551 | 12 / 0 |
| round2_traderlast_flowfork | 2056 | 527 | 12 / 0 |
| round2_traderlast_flowifelse | 2200 | 642 | 12 / 0 |
| round2_traderlast_flowinvert | 2200 | 642 | 12 / 0 |
| round2_traderlast_flowinvertshort | 2200 | 642 | 12 / 0 |
| round2_traderlast_flowlast | 2088 | 643 | 12 / 0 |
| round2_traderlast_flowskip | 2200 | 642 | 12 / 0 |
| round2_traderlast_flowswitch | 2200 | 640 | 12 / 0 |
| round2_traderlast_flowswitchfalse | 2200 | 642 | 12 / 0 |
| round2_traderlast_flowwhile | 2056 | 527 | 12 / 0 |
| round2_traderlast_small10 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small11 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small12 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small13 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small14 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small15 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small16 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small17 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small20 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small24 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small5 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small6 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small7 | 2200 | 642 | 12 / 0 |
| round2_traderlast_small8 | 2200 | 642 | 12 / 0 |
| round2_traderlife_after | 2504 | 589 | 12 / 0 |
| round2_traderlife_after1 | 2504 | 589 | 12 / 0 |
| round2_traderlife_after2 | 2504 | 589 | 12 / 0 |
| round2_traderlife_after3 | 2504 | 589 | 12 / 0 |
| round2_traderlife_afterT100 | 2648 | 525 | 12 / 0 |
| round2_traderlife_afterT65 | 2056 | 554 | 11 / 1 |
| round2_traderlife_afterT80 | 2520 | 588 | 12 / 0 |
| round2_traderlife_force | 2744 | 477 | 12 / 0 |
| round2_traderlife_force1 | 2744 | 477 | 12 / 0 |
| round2_traderlife_force2 | 2744 | 477 | 12 / 0 |
| round2_traderlife_force3 | 2744 | 477 | 12 / 0 |
| round2_traderlife_forceT100 | 2744 | 443 | 12 / 0 |
| round2_traderlife_forceT65 | 2424 | 443 | 11 / 1 |
| round2_traderlife_forceT80 | 2856 | 469 | 12 / 0 |
| round2_traderlife_plain | 2504 | 589 | 12 / 0 |
| round2_traderlife_plain1 | 2504 | 589 | 12 / 0 |
| round2_traderlife_plain2 | 2504 | 589 | 12 / 0 |
| round2_traderlife_plain3 | 2504 | 589 | 12 / 0 |
| round2_traderlife_plainT100 | 2648 | 525 | 12 / 0 |
| round2_traderlife_plainT65 | 2056 | 554 | 11 / 1 |
| round2_traderlife_plainT80 | 2520 | 588 | 12 / 0 |
| round2_traderloops_braces | 2024 | 586 | 12 / 0 |
| round2_traderloops_braceshot | 2184 | 615 | 12 / 0 |
| round2_traderloops_demorgan | 2024 | 586 | 12 / 0 |
| round2_traderloops_demorganhot | 2184 | 615 | 12 / 0 |
| round2_traderloops_do | 1872 | 498 | 12 / 0 |
| round2_traderloops_dohot | 1974 | 490 | 12 / 0 |
| round2_traderloops_fused | 2024 | 586 | 12 / 0 |
| round2_traderloops_fusedhot | 2184 | 615 | 12 / 0 |
| round2_traderloops_heat66 | 2024 | 586 | 12 / 0 |
| round2_traderloops_heat67 | 2024 | 586 | 12 / 0 |
| round2_traderloops_heat68 | 2184 | 615 | 12 / 0 |
| round2_traderloops_heat70 | 2200 | 642 | 12 / 0 |
| round2_traderloops_heat72 | 2264 | 630 | 12 / 0 |
| round2_traderloops_heat74 | 2456 | 604 | 12 / 0 |
| round2_traderloops_heat76 | 2456 | 604 | 12 / 0 |
| round2_traderloops_heat80 | 2472 | 604 | 12 / 0 |
| round2_traderloops_heat85 | 2472 | 604 | 12 / 0 |
| round2_traderloops_heat90 | 2536 | 592 | 12 / 0 |
| round2_traderloops_split0_0 | 1901 | 477 | 12 / 0 |
| round2_traderloops_split0_0hot | 2002 | 511 | 12 / 0 |
| round2_traderloops_split0_1 | 1848 | 500 | 12 / 0 |
| round2_traderloops_split0_1hot | 1944 | 557 | 12 / 0 |
| round2_traderloops_split1_0 | 2024 | 586 | 12 / 0 |
| round2_traderloops_split1_0hot | 2184 | 615 | 12 / 0 |
| round2_traderloops_split1_1 | 2024 | 586 | 12 / 0 |
| round2_traderloops_split1_1hot | 2184 | 615 | 12 / 0 |
| round2_traderloops_split2_0 | 2024 | 586 | 12 / 0 |
| round2_traderloops_split2_0hot | 2184 | 615 | 12 / 0 |
| round2_traderloops_split2_1 | 2024 | 586 | 12 / 0 |
| round2_traderloops_split2_1hot | 2184 | 615 | 12 / 0 |
| round2_traderone_cold-1block | COMPILE_FAIL |  |  /  |
| round2_traderone_cold-1line | COMPILE_FAIL |  |  /  |
| round2_traderone_cold0block | 2072 | 588 | 12 / 0 |
| round2_traderone_cold0line | 2072 | 588 | 12 / 0 |
| round2_traderone_cold1block | 2072 | 588 | 12 / 0 |
| round2_traderone_cold1line | 2072 | 588 | 12 / 0 |
| round2_traderone_cold2block | 2072 | 588 | 12 / 0 |
| round2_traderone_cold2line | 2072 | 588 | 12 / 0 |
| round2_traderone_merge | 2024 | 570 | 12 / 0 |
| round2_traderone_merge63 | 2024 | 570 | 12 / 0 |
| round2_traderone_merge65 | 2024 | 570 | 12 / 0 |
| round2_traderone_merge67 | 2024 | 570 | 12 / 0 |
| round2_traderone_merge68 | 2184 | 549 | 12 / 0 |
| round2_traderone_merge69 | 2184 | 549 | 12 / 0 |
| round2_traderone_mergeback | 2024 | 570 | 12 / 0 |
| round2_traderone_mergeback63 | 2024 | 570 | 12 / 0 |
| round2_traderone_mergeback65 | 2024 | 570 | 12 / 0 |
| round2_traderone_mergeback67 | 2024 | 570 | 12 / 0 |
| round2_traderone_mergeback68 | 2184 | 549 | 12 / 0 |
| round2_traderone_mergeback69 | 2184 | 549 | 12 / 0 |
| round2_traderone_th62 | 1967 | 556 | 12 / 0 |
| round2_traderone_th63 | 2024 | 586 | 12 / 0 |
| round2_traderone_th64 | 2024 | 586 | 12 / 0 |
| round2_traderone_th65 | 2024 | 586 | 12 / 0 |
| round2_traderone_th66 | 2024 | 586 | 12 / 0 |
| round2_traderone_th67 | 2024 | 586 | 12 / 0 |
| round2_traderone_th68 | 2184 | 615 | 12 / 0 |
| round2_traderone_th69 | 2184 | 615 | 12 / 0 |
| round2_traderptr_aftergame | 2456 | 580 | 12 / 0 |
| round2_traderptr_aftergamer | 2456 | 580 | 12 / 0 |
| round2_traderptr_aftergamers | 2456 | 580 | 12 / 0 |
| round2_traderptr_aftergames | 2456 | 580 | 12 / 0 |
| round2_traderptr_dtorgame | 2456 | 580 | 12 / 0 |
| round2_traderptr_dtorgamer | 2456 | 580 | 12 / 0 |
| round2_traderptr_dtorgamers | 2456 | 580 | 12 / 0 |
| round2_traderptr_dtorgames | 2456 | 580 | 12 / 0 |
| round2_traderptr_mixedgame | 2504 | 589 | 12 / 0 |
| round2_traderptr_mixedgamer | 2504 | 589 | 12 / 0 |
| round2_traderptr_mixedgamers | 2504 | 589 | 12 / 0 |
| round2_traderptr_mixedgames | 2504 | 589 | 12 / 0 |
| round2_traderptr_selectedgame | 2232 | 527 | 11 / 0 |
| round2_traderptr_selectedgamer | 2232 | 527 | 11 / 0 |
| round2_traderptr_selectedgamers | 2232 | 527 | 11 / 0 |
| round2_traderptr_selectedgames | 2232 | 527 | 11 / 0 |
| round2_tradervector_call | 2024 | 586 | 12 / 0 |
| round2_tradervector_endfirst | 2024 | 592 | 12 / 0 |
| round2_tradervector_endouter | 2024 | 622 | 12 / 0 |
| round2_tradervector_first | 2024 | 586 | 12 / 0 |
| round2_tradervector_fork | 1656 | 464 | 12 / 0 |
| round2_tradervector_hotlevel0 | 2024 | 622 | 12 / 0 |
| round2_tradervector_hotlevel1 | 2024 | 586 | 12 / 0 |
| round2_tradervector_hotlevel16 | 2024 | 586 | 12 / 0 |
| round2_tradervector_hotlevel2 | 2024 | 586 | 12 / 0 |
| round2_tradervector_hotlevel4 | 2024 | 586 | 12 / 0 |
| round2_tradervector_hotlevel8 | 2024 | 586 | 12 / 0 |
| round2_tradervector_sense | 2024 | 614 | 12 / 0 |
| round2_tradervector_skip | 2024 | 610 | 12 / 0 |
| round2_tradervector_specALLOC_PROXY_REBIND1 | 2024 | 586 | 12 / 0 |
| round2_tradervector_specDESTROY_AUX_FASTCALL1 | 2024 | 586 | 12 / 0 |
| round2_tradervector_specDESTROY_DISPATCH_FORCEINLINE1 | 2024 | 586 | 12 / 0 |
| round2_tradervector_specDESTROY_DISPATCH_FORCEINLINE2 | 2024 | 586 | 12 / 0 |
| round2_tradervector_specVECTOR_DTOR_FORCEINLINE1 | 2504 | 607 | 12 / 0 |
| round2_tradervector_test | 2024 | 586 | 12 / 0 |
| round2_tradervector_vectorcold0 | 1828 | 437 | 12 / 0 |
| round2_tradervector_vectorcold1 | 2504 | 607 | 12 / 0 |
| round2_tradervector_vectorcold2 | 2504 | 607 | 12 / 0 |
