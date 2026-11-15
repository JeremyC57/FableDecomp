# 00D2988B whole-parent recovery probes

The authorized slice is not a function head. The unlanded parent is 00D29880..00D29DA7 (1319 bytes); grade_parent.py redirects only this assigned-key oracle. BOUNDS_FIX.tsv and review/sol2_parent_envelopes contain independent complete disassembly and landed/unlanded proof. Target source contains the entire parent, never the interior slice.

Original identity is independently present in Ego_r and FableWin PUBLIC and class records. Original module 23 register_scripts.obj procedure at 036E0200 identifies the locals, actual nested scopes, TRetVoid return macro and CTimer declaration after the two introduction messages. ORIGINAL_ANNOTATED.disasm records original calls with thunk resolution; retail defines release offsets and virtual slot targets. No neighboring stand-in declarations were copied.

## Source strategies

1. `sol2_01_original_scopes_plain_members`: original full control flow and shared declarations, ordinary inline timer constructor/assignment/GetTime, original reference member opt-in. PREPROCESS_FAIL because complete CScriptDef also requires CDEFSTRINGTABLE; dependency fix `sol2_01b_complete_def_dependencies` compiled to 1237/1319, 362/398 instructions. Dependency diagnostics are not separate strategies. Native expands early timer destruction while source only declared that member, and native map default constructor has no arguments.
2. `sol2_02_timer_lifetime_and_original_map`: restore actual ordinary inline CTimer destructor and existing STLport original map interface. 1258/1319, 375/398 instructions; initial code and stack frame now agree up to the timer register choice. Remaining real call differences: native expands ScriptedThingBase assignment, MovieBase constructor and destructor, and pause guard constructor; source calls them. Per-site timer destructor split also differs. No forced expansion has been added.

## Inline-budget diagnostics (not distinct source strategies)

- `sol2_02b_inline_budget200_diagnostic`: /d2inlT200,1262 bytes,376 instructions. Does not reproduce the target expansion split.
- `sol2_02c_inline_budget400_diagnostic`: /d2inlT400,1278 bytes,378 instructions; expands the unwanted ScriptedThingBase constructor, not the later Movie/pause sites.
- `sol2_02d_inline_size1000_diagnostic`: /d2inlS1000,2026 bytes,613 instructions; expands the needed sites together with large unwanted map/member bodies.
- `sol2_02e_scoped_depth255_diagnostic`: scoped maximum inline depth255,1258 bytes,375 instructions, identical output to plain strategy2. Depth is not the limiting mechanism.

3. `sol2_03_original_expanded_owners_called_counted_dtor`: after flag/scoped ladder, reuse existing original-member force opt-ins for the three native expanded call sites; preserve implicit Movie destructor and declare the counted-pointer destructor as an external member, matching native direct call to its COMDAT. This is original class/helper visibility recovery, with no local class/substitute helper and no control intended to select a register. Result1338 bytes402 instructions; counted assignment expanded too far and Movie destruction remained a call.

All histories retain sources, objects, symbols, full disassembly and relocation lists. No match or test success is claimed yet.


## Continued structural and helper-visibility recovery

4. `sol2_04_original_single_cutscene_scope`: PDB BLOCK32 proves seh_hero, speech_movie, pause guard and mymap are all in one shared lexical block. Remove the unnecessary inner movie block from plain strategy2. Output stays1258; original scope restored even though release code is identical.
5. `sol2_05_counter_loop_for_frequency`: express the counter loop as for with its natural condition and empty increment. C2 canonicalizes it identically1258; this syntactic structural strategy does not establish a frequency distinction.
6. `sol2_06_original_endless_macro_guard`: donor proves the final loop is `while(true)` followed by the macro's unreachable termination guard. Restore both, rather than a bare `for(;;)` with no postguard.1271 bytes386 instructions, strict0.890/loose0.954; timer allocation and three cleanup copies now agree. This proves dead-path macro structure affects early decisions even though it produces no final instructions.
7. `sol2_07_original_macro_and_expanded_members`: retain original macro, expand three actual owner members using existing opt-ins, ordinary counted implementation visible.1364 bytes415 instructions, strict0.942; counted assignment expands too far, Movie destructor stays a call. Scoped depth1 diagnostic `sol2_07b_called_nested_counted_members_depth1` is byte-identical: nested body optimization is already captured before the outer call expression.
8. `sol2_08_original_called_counted_members`: preserve actual counted assignment as external declaration via new opt-in, preserve existing default header behavior and existing destructor-declaration opt-in. Both exact member names independently exist in both PDBs and native call sites. Original implicit outer assignment still stays a call,1287 bytes390 instructions. Expansion correction `sol2_08b_expanded_owner_called_assignment` uses existing outer assignment opt-in:1302 bytes395 instructions; ALL instructions agree except native implicit Movie destructor expansion (three instructions,17bytes). No allocator control is used.

Further budget diagnostics on corrected visibility are separate from structural attempts: S50 expands the needed implicit destructor but also unwanted ScriptedThing ctor/map ctor/timer cleanup (1402bytes). S1 pending. Every source, compiler outcome and exact call list stays in history. No exact match or behavior test success is claimed yet.


## Single-decision compiler observation and further structural work

Read-only original RTM proxy: `trace_inline_sol2.py` adapts the stable Pass21 observer. `sol2_08b.../SOL_INLINE_VALIDATION.json` proves diagnostic target bytes exactly equal the ordinary unmodified RTM object. Trace reports Movie destructor score595,T74,freq468750; ScriptedThing destructor at the same final scope also score595/freq468750. Uniform threshold cannot split those two. Map ctor1504 and dtor1880 remain safely called at T596.

- `sol2_08d_movie_inline_minimum_size1_diagnostic`: S1 is not a fix,1302bytes393instructions, different timer/frame choices.
- `sol2_08e_proved_movie_budget_and_called_exits`: T596 expands actual implicit Movie destructor. Existing ScriptedThing constructor/destructor external declarations preserve actual native calls, scoped depth0 preserves called timer cleanup.1332bytes401instructions, remaining4hunks are only early cleanup block placement plus alignment/jumps. No missing operation remains.
- `sol2_08f_scope_only_called_cleanup_expression`: narrow depth0 from guard+return to return alone, same1332. Diagnostic scope correction, not another structural strategy.
- `sol2_08g_scope_scripted_object_lifetime_calls`: restore visible ScriptedThing members and scope depth0 at object declaration. Constructor remains called, but this does not suppress its final implicit destructor (1348bytes405instructions). Captured destructor decision belongs to its cleanup expression, not declaration.

9. `sol2_09_start_wait_explicit_success_break`: explicit success break in endless StartScriptingEntity wait rather than condition-form wait, starting from1302 best.1310 bytes395instructions; Movie stays called and branch layout differs. No improvement; prior source preserved.
10. `sol2_10_distinct_cleanup_scopes_original_members`: normal Movie inner cleanup scope then depth0 only at outer map/ScriptedThing scope close, plus scoped ctor/exits. This preserves actual visible ScriptedThing bodies and all native call/expand decisions;1332bytes401instructions, same four layout hunks. More faithful helper visibility than declarations alone. Current full-operation base; no need to invent a user-written Movie destructor or extra vptr store.
11. `sol2_11_post_counter_guard_explicit_continuation_arm`: explicit else around all continuation code after post-counter termination return; no byte effect,1332.
12. `sol2_12_post_counter_termination_return_loop`: equivalent single-return while guard at post-counter test to distinguish branch-frequency/layout metadata; pending.

The minimum strategy count is not a stopping point; a small layout residue with every actual operation restored remains active. All ordinary sources/objects and failed diagnostics are preserved.


## Final recovery and independent audit

13. `sol2_13_post_counter_macro_statement_scope`: conventional do/while(false) guard wrapper; still1332. This is a variant in the post-counter guard family.
14. `sol2_14_folded_macro_alternative_guard_frequency`: folded alternative guard and its predicted larger Movie budget;1340 and worse branch placement. Retained as a failed variant in that same guard family.
15. `sol2_15_post_counter_explicit_scope_exit`: express the post-counter guard as an exit from the timer lifetime scope, returning the original empty TRetVoid after cleanup. Exactly1319/1319,398instructions; it restores the native cleanup absorption order. No added machine operations or user Movie destructor.
16. `sol2_16_original_external_instantiation_boundaries`: independent helper audit found three shared header bodies were wrong for this TU (ScriptedThing dtor25 versus native67; map index263; map dtor144). Use the actual original external member declarations, with exact names and native calls proven from both original PDBs plus actual call operands. Target remains exact; the remaining three emitted actually called helpers are26/20/54 and each independently matches its full extent. Earlier failing helper evidence is preserved.

`sol2_17_uniform_original_member_budget_no_force` is a budget/annotation cleanup, not a new source strategy: /d2inlT1000 now expands all required ordinary and implicit owner members with no forced-inline opt-ins. The final source retains scoped depth0 only at actual called construction/cleanup sites. Exact1319/398. `sol2_18_final_pinned_headers_and_metadata` is the final metadata/format/pin rerun, not another strategy; same exact code.

For conservative counting, the16 authored source forms above are grouped into11 genuinely distinct whole-function strategy families: original body; timer/map visibility; outer/member expansion visibility(3/7); cutscene lifetime scopes(4/10); loop macro structure(5/6); original counted member API(8); success-break wait(9); explicit continuation arm(11); post-counter guard family(12/13/14); timer lifetime exit(15); original external instantiation boundary(16). Compiler flags, pragma-level diagnostics, forced/ordinary annotation cleanup, metadata and dependency fixes are not independent strategies.

Read-only inliner observation is preserved and verifies unchanged ordinary RTM bytes. A user-written forced Movie destructor was tested only as a diagnostic (`sol2_08i`):1324 with an extra Movie vptr store, so it was rejected. S50 with called sites preserved (`sol2_08h`) also failed. All residues and failed probes remain in history.

Final independent audit: target1319 plus real helpers26+20+54=1419 bytes,74 code/data relocation operands (70 target), three native RTTI/vtable ABI records with34 original callback entries, no actual-called-helper debts. All41 other shared text COMDATs are explicitly listed without assigned native addresses or independent landing claims.

Standalone full production Main and timer members are copied verbatim.110592 scenarios exhaust48 termination positions for each selected input combination; checked actions form exact prefixes across consecutive termination cuts. Fixtures independently check all arguments, initial cached-condition behavior, native frame/decrement order, GUI updates, map ownership, token cleanup and final sequence. An additional4096 cases exercise the actual shared STLport no-argument map constructor and sentinel layout directly, namespace-isolated with a fixture-only ABI adapter; no constructor logic is altered. VC71 /Od /W3 /GR- without /EHsc and g++64 -std=c++03 both print exact00d2988b_TEST PASS. The first fixture's overstrong completion assumption and a missing host __int64 spelling adapter are recorded as fixture diagnostics, not source strategies. No test changes were made to production logic.
