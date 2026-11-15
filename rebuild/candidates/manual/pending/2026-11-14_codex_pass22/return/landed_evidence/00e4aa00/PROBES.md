# 00e4aa00 current-pass probes

Baseline:1356/1356, only Info reload and push at E4AE44 use eax instead of ecx. All4 supplied drafts and handbacks read; prior lexical-loop/delete-cast attempts excluded. Baseline builds do not count as strategies.

| Attempt | Distinct source strategy | Result |
|---|---|---|
| main_01_count_value | Live named count after decrement; preserve required Info alias reload before test | DIFFER1356/1356; unchanged single mov/push pair |
| main_02_split_callback | Separate the data and deleter reads into named values before callback; reload Info only after it | DIFFER1356/1356; unchanged single mov/push pair |
| main_03_release_return_chain | Complete destructor early-return chain for null and surviving count, with zero stores at each exit | DIFFER1356/1356; unchanged single mov/push pair |
| main_02b_split_delete | Strategy2 subvariant: name the count-block reload after callback instead of callback inputs; no additional strategy credit | DIFFER1356/1356; unchanged single mov/push pair |
| main_04_loop_receiver | Name the final follow-loop interface receiver as a reference; alter scratch candidate provenance without new calls | DIFFER1356/1356; unchanged single mov/push pair |
| main_05_conversation_chain | Express all four complete conversation arms as if/else chain to change decision-tree temporary provenance | DIFFER1356/1356; 10 hunks |
| main_06_named_faction | Name and tightly scope the CCharString faction temporary, preserving call and destruction order | DIFFER1356/1356; unchanged single mov/push pair |
| main_07_named_predicate | Construct predicate as a named object in an explicit scope ending before NewScriptFrame | DIFFER1356/1356; unchanged single mov/push pair |
| main_08_shared_conversation | One genuine conversation local shared by the four arms instead of independent arm-local values | DIFFER1356/1356; unchanged single mov/push pair |
| main_09_named_attack_condition | Store the short-circuit anger/alive/awareness result as a named bool before branch | DIFFER1356/1356; unchanged single mov/push pair |
| main_10_visible_predicate | Actual SetPredicate member body visible, forwarding to original parent virtual and original SetPredicate; retains native call | DIFFER1356/1356; unchanged single mov/push pair; real SetPredicate body visible did not move residue |
| main_11_split_follow_target | Split the actual GetHero return into a const reference before FollowThing, targeting the last live temporary chain before cleanup | DIFFER1356/1356;3hunks: argument-push placement changed, original cleanup pair remains; trace confirms same final rotation |
| main_12_member_reference | Bind the Info member itself by const pointer reference through its entire release; preserve callback-driven alias reload | DIFFER1356/1356; unchanged single mov/push pair; validated RTM trace unchanged at final temp |

Compiler diagnostics: existing c2rotation_veh proxy exited245 before producing data; superseded by c2rotation4. Read-only traces for strategies10,11,12 finish successfully and assert plain RTM target bytes unchanged. Last real source loop temp uses edx and the final cleanup reload rotates to eax. These diagnostic compiles are not strategy attempts. Original PDB locals/scope inspection in progress; row remains IN_PROGRESS, not parked.
| main_13_original_fail_return | Original raw PDB proves TRetVoid _Fail_Val and seh_me; restore real generated-script return expression at each exit | DIFFER1356/1356; unchanged single mov/push pair; original return idiom retained for fidelity |
| main_14_original_for_frame | Donor proves endless for loop with NewScriptFrame in increment, explicit top termination Return() and cleanup at that return, rather than while-condition scope exit | MATCH1356/1356, all433 instructions. Pending full closure/test audit |
| main_15_original_timer_and_local | Fidelity: donor calls CTimer::operator==, and raw PDB has one HLStyleConversationIndex; restore both without changing native behavior | MATCH1356/1356, all433 instructions; original names and timer operator retained |

Final main target match first occurred at strategy14. Strategy15 restores the donor-proven timer comparison and single original conversation identifier while retaining exact bytes. Full10,240-case standalone behavior fixture passed VC71 /Od /W3 /GR- without /EHsc and GCC64 C++03. It sweeps termination through every early-exit/cleanup stage, false/true awareness/alive/distance branches, all four speeches, randomized boo/no-boo choices, scripting failures, pending tasks and ownership cleanup. All model inputs use exact floats.

Original-evidence breakthrough: FableWin module23 has seh_me, TRetVoid _Fail_Val and HLStyleConversationIndex. Donor0397BC98..0397BCC5 and0397C548 establish the generated endless for-loop with frame advance in its increment and an explicit terminating return at the top. Replacing the reconstructed while-condition exit with that actual return changes just the final cleanup temporary from eax to ecx. Validated proxy trace compiles are diagnostic only; final grades always use pristine RTM.
