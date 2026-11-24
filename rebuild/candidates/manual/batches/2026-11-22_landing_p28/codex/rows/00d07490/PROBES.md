# Native VC7.1 experiment log

Baseline: 2089 bytes, retail 2182, 586/664 normalized instructions.

| Probe | Source shape | Native result | Instructions aligned |
|---|---|---:|---:|
| `native_retvoid` | Use FableWin TRetVoid failure holder and plain terminate returns. | 2178 | 575 |
| `native_ifelse` | Retain wrapper exits but make gate logic a mutually exclusive else arm. | 2089 | 586 |
| `native_frame_for` | Put per-frame advance in the update clause and keep a conditional head. | 2220 | 371 |
| `native_shared_chat` | Share the movie block between first and repeat conversation text, preserving the health and wait order. | 1685 | 408 |
| `native_inline_threshold` | Increase the native inline growth budget with the proven script failure holder. | 2225 | 572 |
| `v4_timer_compare` | Use a direct timer comparison with the existing timer accessors at zero. | 2178 | 575 |
| `v4_initial_movie_scope` | Use plain scopes for the RAII movies with the FableWin failure holder and guard-specific branch. | 2062 | 568 |
| `v4_timer_plain` | Ablate the prior forceinline controls on the two small timer accessors. | 2178 | 575 |
| `v4_exit_budget` | Set the compiler inline size threshold and compare emitted destructor split with the original. | 2451 | 541 |
| `exits_gate_explicit_break` | Use the outer frame-loop form that matched Maze, retaining the gate’s nested movie returns. | 2162 | 395 |
| `exits_gate_movie_unwind` | Use a single return outside the loop for each movie-scope termination, retaining all earlier guards and loop advancement. | 2164 | 392 |
| `exits_gate_movie_wait_unwind` | Route only movie task wait checks to an explicit scope-unwinding return, leaving each control acquisition guard unchanged. | 2162 | 395 |
| `exits_gate_literal_zero` | Use the compiler’s original float constant pool reference in the two health comparisons. | 2162 | 395 |
| `exits_gate_movie_owner` | Use the movie handle’s original refcount teardown body, making all owner definitions visible to VC7.1. | 2162 | 395 |
| `gateclose_gate_typed_cleanup` | Keep authentic out-of-line handle destruction and separate native timer-only and movie cleanup sites. | 2107 | 576 |
| `gateclose_gate_movie_unwind_selective` | Use a shared cold return after each movie scope unwinds, preserving the hot hit and closed-gate countdown cleanup. | 2107 | 576 |
| `gateclose_gate_movie_unwind_ordinary` | Test the four movie scope unwinds before suppressing any other resource cleanup. | 2146 | 576 |
| `gateclose_gate_cold_after_timer` | Route the known deeper timing returns to normal C++ scopes before adding any movie unwind edges. | 2139 | 569 |
| `gateclose_gate_dowhile_idle` | Use a bottom tested idle loop after a hit while preserving its first frame and terminating scope. | 2107 | 576 |
| `gateclose_gate_for_motion` | Put the frame advance in a for increment and use the native outer loop break. | 2162 | 395 |
| `gateclose_gate_bare_movie_scope` | Use braces for movie lifetimes and an explicit shared cold return after local unwinding. | 2026 | 568 |
| `gateclose_gate_fixed_receiver` | Use original protected Game reference and declared-only scripted entity cleanup across every exit. | 2107 | 576 |
| `gateedges_gate_timer_edges` | Use one timer-only scope exit for the first hit and closed gate conditions, and retain movie-only exits. | 2107 | 576 |
| `gateedges_gate_timer_edges_no_cold` | Use two timer-only exits and bounded movie scopes while allowing deep checks to compile normally. | 2146 | 587 |
| `gateedges_gate_timer_hit_only` | Route the hit guard through its own timer-only destruction without changing the gate guard. | 2107 | 576 |
| `gateedges_gate_timer_gate_only` | Route the closed-gate timer precheck through a timer-only scope unwind. | 2107 | 576 |
| `gateedges_gate_timer_idle_join` | Share the timer-only cleanup target with the terminating idle frame. | 2107 | 576 |
| `gateowners_gate_movie_native_owner` | Keep the authentic normal MovieBase counted-pointer owner body visible while retaining the successful frame condition. | 2108 | 567 |
| `gateowners_gate_all_native_owners` | Use original CScriptThing destructor and MovieBase pointer destructor bodies with the normal frame condition. | 2307 | 449 |
| `gateowners_gate_banded_owners` | Use original destructor bodies with proven timer-only and movie-only unwind edges. | 2225 | 449 |
| `gateowners_gate_timer_frame_owner` | Use original destructors while keeping a conditional timer-only exit and its shared cleanup. | 2062 | 579 |
| `gateowners_gate_full_motion_owner` | Use the actual owners and an explicit timer scope together with the original idle-frame loop. | 2170 | 441 |
| `gateowners_gate_release_names` | Use the original counted-pointer ReleaseData source from the shared header with the natural destructor structure. | 2108 | 567 |
| `gatefinal_gate_timer_profile35` | Use the measured timer-only edge shape with per-row inlining threshold 35. | 2080 | 569 |
| `gatefinal_gate_timer_profile40` | Use the measured timer-only edge shape with per-row inlining threshold 40. | 2123 | 569 |
| `gatefinal_gate_timer_profile45` | Use the measured timer-only edge shape with per-row inlining threshold 45. | 2178 | 573 |
| `gatefinal_gate_timer_profile55` | Use the measured timer-only edge shape with per-row inlining threshold 55. | 2146 | 587 |
| `gatefinal_gate_timer_profile65` | Use the measured timer-only edge shape with per-row inlining threshold 65. | 2146 | 587 |
| `gatefinal_gate_cached_index` | Keep the proven long timer index across the guarded conversation countdown and timer assignments. | 2178 | 575 |
| `gatefinal_gate_timer_receiver` | Use an explicit receiver reference and the original index value in the inlined timer query. | 2178 | 575 |
| `gatefinal_gate_terminate_macro` | Preserve script’s statement-macro scope in the first hit and post-frame exits. | 2178 | 575 |

First handoff selected `v4_timer_plain` for its original source types and smallest extent gap. The best shared-scope follow-up aligned 587 of 664 instructions at 2146 bytes, but spread the mismatch over more regions. The selected version’s remaining differences include the timer-index home and the inline destructor choices for the hit and idle returns, and the branch order in both movie cleanup tails.

## Main-chat retry

FableWin constructs the predicate as an unnamed temporary passed directly to SetPredicate. The debug generator retains a termination check after the hit-path idle loop. The shared CScriptThing destructor and ordinary MovieBase counted-pointer owner restore both formerly unequal mapped copies. All 12 bound copies match on the selected source. The main function remains 2,190 versus 2,182 bytes with 572/664 aligned instructions.

The prior selection was 2,178 bytes with two unequal mapped copies. The replacement is selected for verified owners and direct debug evidence. The predicate destructor expands differently at entry, the timer index has a different register home, and the conditional string cleanup sites still differ. The nearer earlier length was recorded in the experiment history.

| Native probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| solo_guard_donor_base | 2162 | 395 | 8 / 2 |
| solo_guard_donor_base_post | 2162 | 395 | 8 / 2 |
| solo_guard_donor_base_movie | 2162 | 395 | 8 / 2 |
| solo_guard_donor_base_cold_movies | 2162 | 395 | 8 / 2 |
| solo_guard_donor_base_deep_returns | 2116 | 393 | 8 / 2 |
| solo_guard_donor_base_timed_returns | 2116 | 392 | 8 / 2 |
| solo_guard_donor_base_cold_6 | 2104 | 340 | 8 / 2 |
| solo_guard_donor_base_cold_7 | 2137 | 391 | 8 / 2 |
| solo_guard_donor_base_cold_8 | 2137 | 395 | 8 / 2 |
| solo_guard_donor_owners | 2140 | 361 | 12 / 0 |
| solo_guard_donor_owners_post | 2140 | 361 | 12 / 0 |
| solo_guard_donor_owners_movie | 2140 | 361 | 12 / 0 |
| solo_guard_donor_owners_cold_movies | 2140 | 361 | 12 / 0 |
| solo_guard_donor_owners_deep_returns | 2094 | 355 | 12 / 0 |
| solo_guard_donor_owners_timed_returns | 2094 | 354 | 12 / 0 |
| solo_guard_donor_owners_cold_6 | 2085 | 394 | 12 / 0 |
| solo_guard_donor_owners_cold_7 | 2115 | 357 | 12 / 0 |
| solo_guard_donor_owners_cold_8 | 2115 | 360 | 12 / 0 |
| solo_guard_temp_b_local | 2178 | 575 | 8 / 2 |
| solo_guard_temp_b_local_post | 2170 | 586 | 8 / 2 |
| solo_guard_temp_b_local_zero | 2178 | 575 | 8 / 2 |
| solo_guard_temp_b_increment | 2162 | 395 | 8 / 2 |
| solo_guard_temp_b_increment_post | 2162 | 395 | 8 / 2 |
| solo_guard_temp_b_increment_zero | 2162 | 395 | 8 / 2 |
| solo_guard_temp_b_headreturn | 2178 | 575 | 8 / 2 |
| solo_guard_temp_b_headreturn_post | 2170 | 586 | 8 / 2 |
| solo_guard_temp_b_headreturn_zero | 2178 | 575 | 8 / 2 |
| solo_guard_temp_h_local | 2307 | 449 | 12 / 0 |
| solo_guard_temp_h_local_post | 2190 | 572 | 12 / 0 |
| solo_guard_temp_h_local_zero | 2307 | 449 | 12 / 0 |
| solo_guard_temp_h_increment | 2140 | 361 | 12 / 0 |
| solo_guard_temp_h_increment_post | 2140 | 361 | 12 / 0 |
| solo_guard_temp_h_increment_zero | 2140 | 361 | 12 / 0 |
| solo_guard_temp_h_headreturn | 2307 | 449 | 12 / 0 |
| solo_guard_temp_h_headreturn_post | 2190 | 572 | 12 / 0 |
| solo_guard_temp_h_headreturn_zero | 2307 | 449 | 12 / 0 |
| solo_guard_depth_scope0 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_first0 | 2190 | 538 | 12 / 0 |
| solo_guard_depth_movies0 | 2187 | 414 | 12 / 0 |
| solo_guard_depth_all0 | 2096 | 445 | 12 / 0 |
| solo_guard_depth_first1 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_scope1 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_movies1 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_all1 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_first2 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_scope2 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_movies2 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_all2 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_first3 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_scope3 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_movies3 | 2190 | 572 | 12 / 0 |
| solo_guard_depth_all3 | 2190 | 572 | 12 / 0 |
| solo_guard_T30 | 2164 | 549 | 8 / 4 |
| solo_guard_T20 | 1985 | 227 | 8 / 4 |
| solo_guard_T50 | 2256 | 579 | 8 / 4 |
| solo_guard_T80 | 2437 | 445 | 12 / 0 |
| solo_guard_T100 | 2641 | 579 | 12 / 0 |
| solo_guard_T160 | 2836 | 569 | 12 / 0 |
| solo_guard_predicate_expand | 2167 | 526 | 12 / 0 |
| solo_guard_predicate_stage1 | 2167 | 526 | 12 / 0 |
| solo_guard_predicate_stage2 | 2167 | 526 | 12 / 0 |
| solo_guard_predicate_stage3 | 2167 | 526 | 12 / 0 |

Final selection: `solo_guard_temp_h_local_post`. 2,190 versus 2,182 bytes. All 12 mapped copies match. Predicate destruction and the timer/temporary cleanup paths still use different instruction sequences.

## Continued main-chat grades

The final batch compared definition order and visibility of the real movie-pointer release and sibling predicate, and tested structured early exits against the call-frequency map. The retail timer/movie cleanup split remains open.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_gatebreak_memberall | 2215 | 460 | 13 / 0 |
| round2_gatebreak_memberboth | 2220 | 448 | 13 / 0 |
| round2_gatebreak_memberhit | 2103 | 578 | 13 / 0 |
| round2_gatebreak_memberidle | 2220 | 448 | 13 / 0 |
| round2_gatebreak_memberouter | 2103 | 578 | 13 / 0 |
| round2_gatebreak_memberouterhit | 2103 | 578 | 13 / 0 |
| round2_gatebreak_naturalall | 2215 | 460 | 12 / 0 |
| round2_gatebreak_naturalboth | 2220 | 448 | 12 / 0 |
| round2_gatebreak_naturalhit | 2103 | 578 | 12 / 0 |
| round2_gatebreak_naturalidle | 2220 | 448 | 12 / 0 |
| round2_gatebreak_naturalouter | 2103 | 578 | 12 / 0 |
| round2_gatebreak_naturalouterhit | 2103 | 578 | 12 / 0 |
| round2_gatebreak_predicateall | 2279 | 420 | 12 / 0 |
| round2_gatebreak_predicateboth | 2284 | 409 | 12 / 0 |
| round2_gatebreak_predicatehit | 2167 | 526 | 12 / 0 |
| round2_gatebreak_predicateidle | 2284 | 409 | 12 / 0 |
| round2_gatebreak_predicateouter | 2167 | 526 | 12 / 0 |
| round2_gatebreak_predicateouterhit | 2167 | 526 | 12 / 0 |
| round2_gatebreak_thinall | 2281 | 462 | 10 / 1 |
| round2_gatebreak_thinboth | 2290 | 451 | 10 / 1 |
| round2_gatebreak_thinhit | 2170 | 586 | 10 / 1 |
| round2_gatebreak_thinidle | 2290 | 451 | 10 / 1 |
| round2_gatebreak_thinouter | 2170 | 586 | 10 / 1 |
| round2_gatebreak_thinouterhit | 2170 | 586 | 10 / 1 |
| round2_gatecold_plainafter0 | 2103 | 569 | 12 / 0 |
| round2_gatecold_plainafterNone | 2098 | 560 | 12 / 0 |
| round2_gatecold_plainallcold0 | 2103 | 569 | 12 / 0 |
| round2_gatecold_plainallcoldNone | 2133 | 568 | 12 / 0 |
| round2_gatecold_plaindeep0 | 2060 | 561 | 12 / 0 |
| round2_gatecold_plaindeepNone | 2141 | 570 | 12 / 0 |
| round2_gatecold_plainmovies0 | 2060 | 579 | 12 / 0 |
| round2_gatecold_plainmoviesNone | 2062 | 565 | 12 / 0 |
| round2_gatecold_plainnonhit0 | 2060 | 561 | 12 / 0 |
| round2_gatecold_plainnonhitNone | 2141 | 561 | 12 / 0 |
| round2_gatecold_releaseafter0 | 2103 | 569 | 13 / 0 |
| round2_gatecold_releaseafterNone | 2098 | 560 | 13 / 0 |
| round2_gatecold_releaseallcold0 | 2103 | 569 | 13 / 0 |
| round2_gatecold_releaseallcoldNone | 2133 | 568 | 13 / 0 |
| round2_gatecold_releasedeep0 | 2060 | 561 | 13 / 0 |
| round2_gatecold_releasedeepNone | 2141 | 570 | 13 / 0 |
| round2_gatecold_releasemovies0 | 2060 | 579 | 13 / 0 |
| round2_gatecold_releasemoviesNone | 2062 | 565 | 13 / 0 |
| round2_gatecold_releasenonhit0 | 2060 | 561 | 13 / 0 |
| round2_gatecold_releasenonhitNone | 2141 | 561 | 13 / 0 |
| round2_gatecold_t50after0 | 2224 | 572 | 8 / 4 |
| round2_gatecold_t50afterNone | 2225 | 566 | 8 / 4 |
| round2_gatecold_t50allcold0 | 2224 | 572 | 8 / 4 |
| round2_gatecold_t50allcoldNone | 2224 | 566 | 8 / 4 |
| round2_gatecold_t50deep0 | 2180 | 564 | 8 / 4 |
| round2_gatecold_t50deepNone | 2181 | 542 | 8 / 4 |
| round2_gatecold_t50movies0 | 2180 | 581 | 8 / 4 |
| round2_gatecold_t50moviesNone | 2185 | 567 | 8 / 4 |
| round2_gatecold_t50nonhit0 | 2180 | 564 | 8 / 4 |
| round2_gatecold_t50nonhitNone | 2181 | 561 | 8 / 4 |
| round2_gatecold_thinafter0 | 2172 | 575 | 10 / 1 |
| round2_gatecold_thinafterNone | 2174 | 569 | 10 / 1 |
| round2_gatecold_thinallcold0 | 2172 | 575 | 10 / 1 |
| round2_gatecold_thinallcoldNone | 2201 | 573 | 10 / 1 |
| round2_gatecold_thindeep0 | 2143 | 567 | 10 / 1 |
| round2_gatecold_thindeepNone | 2237 | 577 | 10 / 1 |
| round2_gatecold_thinmovies0 | 2141 | 587 | 10 / 1 |
| round2_gatecold_thinmoviesNone | 2146 | 572 | 10 / 1 |
| round2_gatecold_thinnonhit0 | 2143 | 567 | 10 / 1 |
| round2_gatecold_thinnonhitNone | 2231 | 567 | 10 / 1 |
| round2_gateexit_after1 | 2254 | 587 | 10 / 1 |
| round2_gateexit_after2 | 2254 | 587 | 10 / 1 |
| round2_gateexit_early1 | 2254 | 587 | 10 / 1 |
| round2_gateexit_early2 | 2254 | 587 | 10 / 1 |
| round2_gateexit_end0 | 2328 | 598 | 8 / 3 |
| round2_gateexit_end1 | 2339 | 608 | 10 / 1 |
| round2_gateexit_end2 | 2254 | 587 | 10 / 1 |
| round2_gateexit_end3 | 2254 | 587 | 10 / 1 |
| round2_gateexit_finalspeech1 | 2254 | 587 | 10 / 1 |
| round2_gateexit_finalspeech2 | 2254 | 587 | 10 / 1 |
| round2_gateexit_firstspeech1 | 2254 | 587 | 10 / 1 |
| round2_gateexit_firstspeech2 | 2254 | 587 | 10 / 1 |
| round2_gateexit_gate1 | 2254 | 587 | 10 / 1 |
| round2_gateexit_gate2 | 2254 | 587 | 10 / 1 |
| round2_gateexit_hit1 | 2254 | 587 | 10 / 1 |
| round2_gateexit_hit2 | 2254 | 587 | 10 / 1 |
| round2_gateexit_movie_last1 | 2254 | 587 | 10 / 1 |
| round2_gateexit_movie_last2 | 2254 | 587 | 10 / 1 |
| round2_gateexit_movie_wait1 | 2254 | 587 | 10 / 1 |
| round2_gateexit_movie_wait2 | 2254 | 587 | 10 / 1 |
| round2_gateexit_movies1 | 2254 | 587 | 10 / 1 |
| round2_gateexit_movies2 | 2254 | 587 | 10 / 1 |
| round2_gatefirst_bodylate0 | 2190 | 538 | 12 / 0 |
| round2_gatefirst_bodylate1 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_bodylate2 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_bodylate3 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_bodylateT50 | 2256 | 579 | 8 / 4 |
| round2_gatefirst_bodylateT58 | 2103 | 578 | 11 / 1 |
| round2_gatefirst_bodylateT65 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_bodylateT72 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_bodylateT85 | 2437 | 445 | 12 / 0 |
| round2_gatefirst_both0 | 2190 | 538 | 12 / 0 |
| round2_gatefirst_both1 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_both2 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_both3 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_bothT50 | 2256 | 579 | 8 / 4 |
| round2_gatefirst_bothT58 | 2103 | 578 | 11 / 1 |
| round2_gatefirst_bothT65 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_bothT72 | 2167 | 526 | 12 / 0 |
| round2_gatefirst_bothT85 | 2437 | 445 | 12 / 0 |
| round2_gatefirst_plain0 | 2190 | 538 | 12 / 0 |
| round2_gatefirst_plain1 | 2190 | 572 | 12 / 0 |
| round2_gatefirst_plain2 | 2190 | 572 | 12 / 0 |
| round2_gatefirst_plain3 | 2190 | 572 | 12 / 0 |
| round2_gatefirst_plainT50 | 2256 | 579 | 8 / 4 |
| round2_gatefirst_plainT58 | 2103 | 578 | 11 / 1 |
| round2_gatefirst_plainT65 | 2190 | 572 | 12 / 0 |
| round2_gatefirst_plainT72 | 2190 | 572 | 12 / 0 |
| round2_gatefirst_plainT85 | 2437 | 445 | 12 / 0 |
| round2_gatelive_fallback | 2114 | 580 | 12 / 0 |
| round2_gatelive_fallbackbudget38_9 | 2187 | 581 | 8 / 4 |
| round2_gatelive_fallbackbudget49_15 | 2371 | 612 | 8 / 4 |
| round2_gatelive_fallbackbudget50_12 | 2417 | 612 | 8 / 4 |
| round2_gatelive_fallbackbudget55_8 | 2114 | 580 | 11 / 1 |
| round2_gatelive_fallbackbudget60_11 | 2200 | 610 | 11 / 1 |
| round2_gatelive_fallbackbudget62_15 | 2200 | 610 | 11 / 1 |
| round2_gatelive_fallbackbudget75_7 | 2114 | 580 | 12 / 0 |
| round2_gatelive_fallbackelse | 2106 | 576 | 12 / 0 |
| round2_gatelive_fallbackends | 2228 | 450 | 12 / 0 |
| round2_gatelive_fallbacksplit | 2114 | 580 | 12 / 0 |
| round2_gatelive_gen | 2162 | 395 | 12 / 0 |
| round2_gatelive_genbudget38_9 | 2070 | 350 | 8 / 4 |
| round2_gatelive_genbudget49_15 | 2266 | 582 | 8 / 4 |
| round2_gatelive_genbudget50_12 | 2122 | 470 | 8 / 4 |
| round2_gatelive_genbudget55_8 | 2070 | 337 | 11 / 1 |
| round2_gatelive_genbudget60_11 | 2055 | 455 | 11 / 1 |
| round2_gatelive_genbudget62_15 | 2145 | 535 | 11 / 1 |
| round2_gatelive_genbudget75_7 | 2162 | 395 | 12 / 0 |
| round2_gatelive_genelse | 2162 | 395 | 12 / 0 |
| round2_gatelive_genends | 2162 | 395 | 12 / 0 |
| round2_gatelive_gensplit | 2162 | 395 | 12 / 0 |
| round2_gatelive_while | 2103 | 578 | 12 / 0 |
| round2_gatelive_whilebudget38_9 | 2175 | 579 | 8 / 4 |
| round2_gatelive_whilebudget49_15 | 2360 | 609 | 8 / 4 |
| round2_gatelive_whilebudget50_12 | 2388 | 609 | 8 / 4 |
| round2_gatelive_whilebudget55_8 | 2103 | 578 | 11 / 1 |
| round2_gatelive_whilebudget60_11 | 2186 | 604 | 11 / 1 |
| round2_gatelive_whilebudget62_15 | 2186 | 604 | 11 / 1 |
| round2_gatelive_whilebudget75_7 | 2103 | 578 | 12 / 0 |
| round2_gatelive_whileelse | 2092 | 574 | 12 / 0 |
| round2_gatelive_whileends | 2220 | 448 | 12 / 0 |
| round2_gatelive_whilesplit | 2103 | 578 | 12 / 0 |
| round2_gatemov_default_bare | 2103 | 578 | 12 / 0 |
| round2_gatemov_default_gen | 2162 | 395 | 12 / 0 |
| round2_gatemov_default_moved | 2108 | 577 | 12 / 0 |
| round2_gatemov_default_post | 2114 | 580 | 12 / 0 |
| round2_gatemov_default_scope | 2039 | 346 | 12 / 0 |
| round2_gatemov_full_bare | 2507 | 563 | 11 / 1 |
| round2_gatemov_full_gen | 2324 | 324 | 11 / 1 |
| round2_gatemov_full_moved | 2511 | 448 | 11 / 1 |
| round2_gatemov_full_post | 2510 | 455 | 11 / 1 |
| round2_gatemov_full_scope | 2348 | 343 | 11 / 1 |
| round2_gatemov_member_bare | 2254 | 587 | 10 / 1 |
| round2_gatemov_member_gen | 2218 | 323 | 10 / 1 |
| round2_gatemov_member_moved | 2256 | 584 | 10 / 1 |
| round2_gatemov_member_post | 2259 | 588 | 10 / 1 |
| round2_gatemov_member_scope | 2154 | 346 | 10 / 1 |
| round2_gatemov_thin_bare | 2170 | 586 | 10 / 1 |
| round2_gatemov_thin_gen | 2162 | 395 | 10 / 1 |
| round2_gatemov_thin_moved | 2175 | 581 | 10 / 1 |
| round2_gatemov_thin_post | 2188 | 592 | 10 / 1 |
| round2_gatemov_thin_scope | 2039 | 346 | 10 / 1 |
| round2_gateowners2_genconjunction | 2162 | 395 | 12 / 0 |
| round2_gateowners2_genearly | 2162 | 395 | 12 / 0 |
| round2_gateowners2_genearlycall | 2162 | 395 | 13 / 0 |
| round2_gateowners2_genlate | 2162 | 395 | 12 / 0 |
| round2_gateowners2_genlatecall | 2162 | 395 | 13 / 0 |
| round2_gateowners2_gensibling | 2210 | 398 | 12 / 0 |
| round2_gateowners2_gensiblinglate | 2210 | 398 | 12 / 0 |
| round2_gateowners2_postconjunction | 2184 | 331 | 12 / 0 |
| round2_gateowners2_postearly | 2114 | 580 | 12 / 0 |
| round2_gateowners2_postearlycall | 2114 | 580 | 13 / 0 |
| round2_gateowners2_postlate | 2114 | 580 | 12 / 0 |
| round2_gateowners2_postlatecall | 2114 | 580 | 13 / 0 |
| round2_gateowners2_postsibling | 2178 | 533 | 12 / 0 |
| round2_gateowners2_postsiblinglate | 2178 | 533 | 12 / 0 |
| round2_gateowners2_whileconjunction | 2190 | 379 | 12 / 0 |
| round2_gateowners2_whileearly | 2103 | 578 | 12 / 0 |
| round2_gateowners2_whileearlycall | 2103 | 578 | 13 / 0 |
| round2_gateowners2_whilelate | 2103 | 578 | 12 / 0 |
| round2_gateowners2_whilelatecall | 2103 | 578 | 13 / 0 |
| round2_gateowners2_whilesibling | 2167 | 526 | 12 / 0 |
| round2_gateowners2_whilesiblinglate | 2167 | 526 | 12 / 0 |
| round2_gatepred_below0 | 2103 | 578 | 11 / 1 |
| round2_gatepred_below1 | 2103 | 578 | 12 / 0 |
| round2_gatepred_below2 | 2167 | 526 | 12 / 0 |
| round2_gatepred_belowNone | 2167 | 526 | 12 / 0 |
| round2_gatepred_below_loop | 2103 | 578 | 12 / 0 |
| round2_gatepred_hidden0 | 2103 | 578 | 10 / 1 |
| round2_gatepred_hidden1 | 2103 | 578 | 10 / 1 |
| round2_gatepred_hidden2 | 2103 | 578 | 10 / 1 |
| round2_gatepred_hiddenNone | 2103 | 578 | 10 / 1 |
| round2_gatepred_hidden_loop | 2103 | 578 | 10 / 1 |
| round2_gatepred_noinline0 | 2103 | 578 | 11 / 1 |
| round2_gatepred_noinline1 | 2103 | 578 | 11 / 1 |
| round2_gatepred_noinline2 | 2103 | 578 | 11 / 1 |
| round2_gatepred_noinlineNone | 2103 | 578 | 11 / 1 |
| round2_gatepred_noinline_loop | 2103 | 578 | 11 / 1 |
| round2_gatepred_visible0 | 2103 | 578 | 11 / 1 |
| round2_gatepred_visible1 | 2103 | 578 | 12 / 0 |
| round2_gatepred_visible2 | 2167 | 526 | 12 / 0 |
| round2_gatepred_visibleNone | 2167 | 526 | 12 / 0 |
