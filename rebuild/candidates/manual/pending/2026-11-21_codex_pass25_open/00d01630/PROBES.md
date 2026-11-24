- idle_exits — Restore normal timer destructor visibility and localise cleanup expansion on idle exits. DIFFER 5289/5178 bytes, 71 instruction hunks.
- no_movie_cold — Restore normal timer destructor visibility and localise cleanup expansion on no movie cold. DIFFER 5264/5178 bytes, 92 instruction hunks.
- cold_cleanup_depth1 — Restore normal timer destructor visibility and localise cleanup expansion on cold cleanup depth1. DIFFER 5247/5178 bytes, 73 instruction hunks.
- cold_cleanup_depth2 — Restore normal timer destructor visibility and localise cleanup expansion on cold cleanup depth2. DIFFER 5247/5178 bytes, 73 instruction hunks.
- speech_exits — Restore normal timer destructor visibility and localise cleanup expansion on speech exits. DIFFER 5142/5178 bytes, 163 instruction hunks.
- loop_wait_exits — Restore normal timer destructor visibility and localise cleanup expansion on loop wait exits. DIFFER 5261/5178 bytes, 145 instruction hunks.
- all_but_heads — Restore normal timer destructor visibility and localise cleanup expansion on all but heads. DIFFER 5008/5178 bytes, 169 instruction hunks.
- all_but_heads_level1 — Restore normal timer destructor visibility and localise cleanup expansion on all but heads level1. DIFFER 5247/5178 bytes, 73 instruction hunks.
- normal_linkage — Use a definition with ordinary linkage for the timer destructor. DIFFER 5247/5178 bytes, 73 instruction hunks.
- timer_body_after_main — Keep the inline timer destructor visible in the TU after the script body. DIFFER 5247/5178 bytes, 73 instruction hunks.
- explicit_inline_only — Restrict expansion to explicitly inline definitions and the generated movie destructor. DIFFER 5161/5178 bytes, 29 instruction hunks.
- loop_header_unconditional — Express the endless quest loop with an empty condition and preserve its increment. DIFFER 5248/5178 bytes, 17 instruction hunks.
- frame_at_loop_tail — Put the frame advance after the attacked-guard branch in the main loop. DIFFER 5161/5178 bytes, 29 instruction hunks.
- timer_constructor_init — Initialize the timer index in its constructor initializer like the same script-family implementation. DIFFER 5161/5178 bytes, 29 instruction hunks.
- loop_header_dtor_score — Enable timer inlining on the empty-condition outer loop. DIFFER 5334/5178 bytes, 72 instruction hunks.
- loop_header_default_budget — Use the natural timer and movie inline budget on the high-frequency outer-loop edge. DIFFER 5241/5178 bytes, 50 instruction hunks.
- for_scope — Debug donor at +1eb wraps outer loop in if(false){}else which precedes both header and increment; +2aff uses a true-test wait; compare for_scope. DIFFER 5161/5178 bytes, 29 instruction hunks.
- for_scope_waits — Debug donor at +1eb wraps outer loop in if(false){}else which precedes both header and increment; +2aff uses a true-test wait; compare for_scope_waits. DIFFER 5161/5178 bytes, 29 instruction hunks.
- for_scope_waits_natural — Debug donor at +1eb wraps outer loop in if(false){}else which precedes both header and increment; +2aff uses a true-test wait; compare for_scope_waits_natural. DIFFER 5247/5178 bytes, 73 instruction hunks.
- for_scope_score — Debug donor at +1eb wraps outer loop in if(false){}else which precedes both header and increment; +2aff uses a true-test wait; compare for_scope_score. DIFFER 5134/5178 bytes, 35 instruction hunks.
- selected_donor — Select source after verified debug donor, guarded portable compiler controls, shared headers and clean opt-ins. DIFFER 5161/5178 bytes, 29 instruction hunks.
- tu_legacy_for — Apply the debug-donor-confirmed legacy loop-scope workaround across the translation unit including all library headers. DIFFER 5161/5178 bytes, 29 instruction hunks.
- tu_legacy_for_natural — Evaluate the original loop workaround without the inherited nested destructor controls. DIFFER 5247/5178 bytes, 73 instruction hunks.
- tu_legacy_for_default — Evaluate origin-matched loop scoping with normal /O2 and natural destructor inlining. DIFFER 5134/5178 bytes, 35 instruction hunks.
- visible_counted_release — Provide the real ReleaseData destructor for Movie smart pointers while preserving retail-called release sites. DIFFER 5134/5178 bytes, 35 instruction hunks.
- visible_release_movie_force — Test whether inline visibility of the original counted release removes the extra vptr in an explicitly expanded movie destructor. DIFFER 5216/5178 bytes, 75 instruction hunks.
- visible_release_flag — Fix movie inlining using the existing S15 compiler threshold with the real member release visible. DIFFER 5247/5178 bytes, 73 instruction hunks.
- early_resource_none — Early-expand the evidenced resource base cleanup in the implicit Movie destructor with ordinary budget. DIFFER 5134/5178 bytes, 35 instruction hunks.
- early_resource_d2inlS11 — Early-expand the evidenced resource base cleanup in the implicit Movie destructor with below timer threshold. DIFFER 5138/5178 bytes, 52 instruction hunks.
- early_resource_d2inlS13 — Early-expand the evidenced resource base cleanup in the implicit Movie destructor with timer threshold. DIFFER 5138/5178 bytes, 52 instruction hunks.
- early_resource_d2inlS15 — Early-expand the evidenced resource base cleanup in the implicit Movie destructor with movie threshold. DIFFER 5247/5178 bytes, 73 instruction hunks.
- timer_cost_frame — Diagnostic of the out-of-line timer destructor size driving the forced-small cutoff, keeping its real interface call unchanged: frame. DIFFER 5247/5178 bytes, 73 instruction hunks.
- timer_cost_small — Diagnostic of the out-of-line timer destructor size driving the forced-small cutoff, keeping its real interface call unchanged: small. DIFFER 5247/5178 bytes, 73 instruction hunks.
- timer_cost_global — Diagnostic of the out-of-line timer destructor size driving the forced-small cutoff, keeping its real interface call unchanged: global. DIFFER 5161/5178 bytes, 39 instruction hunks.
- timer_cost_unoptimised — Diagnostic of the out-of-line timer destructor size driving the forced-small cutoff, keeping its real interface call unchanged: unoptimised. DIFFER 5161/5178 bytes, 39 instruction hunks.
- budget_path_natural — Measure compiler budget-path effects on implicit timer/movie cleanups with genuine member bodies. DIFFER 5408/5178 bytes, 134 instruction hunks.
- budget_path_all_for_scopes — Restore compiler-era for scope throughout original script headers on the budget-inliner path. DIFFER 5408/5178 bytes, 134 instruction hunks.
- budget_path_script_thing_external — Keep the scripted-thing destructor called throughout as observed in retail. DIFFER 5247/5178 bytes, 73 instruction hunks.
- budget_path_all_pointer_bodies — Expose the genuine Movie smart-pointer release to budget inlining in its actual class, maintaining the default 66-byte COMDAT. DIFFER 5408/5178 bytes, 134 instruction hunks.
- two_postwait_guards_selected — The original debug waits on lines 427 and 434 each emit the termination check again after the while block, at offsets +2b71 and +2cd8. Restore both genuine guards. DIFFER 5161/5178 bytes, 29 instruction hunks.
- two_postwait_guards_natural — The original debug waits on lines 427 and 434 each emit the termination check again after the while block, at offsets +2b71 and +2cd8. Restore both genuine guards. DIFFER 5134/5178 bytes, 35 instruction hunks.
- two_postwait_guards_force_small — The original debug waits on lines 427 and 434 each emit the termination check again after the while block, at offsets +2b71 and +2cd8. Restore both genuine guards. DIFFER 5247/5178 bytes, 73 instruction hunks.
- project_counted_dtor — Rebase timer decision and the genuine compiler-generated movie destructor on the PDB-complete counted-pointer class; project_counted_dtor. DIFFER 5161/5178 bytes, 29 instruction hunks.
- project_full_natural — Rebase timer decision and the genuine compiler-generated movie destructor on the PDB-complete counted-pointer class; project_full_natural. DIFFER 5134/5178 bytes, 35 instruction hunks.
- project_available_movie — Rebase timer decision and the genuine compiler-generated movie destructor on the PDB-complete counted-pointer class; project_available_movie. DIFFER 5134/5178 bytes, 35 instruction hunks.
- project_release_s15 — Rebase timer decision and the genuine compiler-generated movie destructor on the PDB-complete counted-pointer class; project_release_s15. DIFFER 5138/5178 bytes, 52 instruction hunks.
- retail_codeview_Z7 — Retail contains an RSDS reference to Fable.pdb. Check whether VC7.1 debug-symbol emission influences the remaining code decisions while optimizing: /Z7. DIFFER 5161/5178, 29 hunks.
- retail_codeview_Zi — Retail contains an RSDS reference to Fable.pdb. Check whether VC7.1 debug-symbol emission influences the remaining code decisions while optimizing: /Zi. DIFFER 5161/5178, 29 hunks.
- retail_codeview_Zd — Retail contains an RSDS reference to Fable.pdb. Check whether VC7.1 debug-symbol emission influences the remaining code decisions while optimizing: /Zd. DIFFER 5161/5178, 29 hunks.
- goal_favour_size — Use the compiler favour size optimization objective with the original timer body visible, preserving every donor branch. DIFFER 4324/5178 bytes, 301 instruction hunks.
- goal_size_switch_movie_threshold — Use the compiler size switch movie threshold optimization objective with the original timer body visible, preserving every donor branch. DIFFER 4467/5178 bytes, 307 instruction hunks.
- goal_stated_inline_only — Use the compiler stated inline only optimization objective with the original timer body visible, preserving every donor branch. DIFFER 5134/5178 bytes, 35 instruction hunks.
- goal_minimal_size — Use the compiler minimal size optimization objective with the original timer body visible, preserving every donor branch. DIFFER 4324/5178 bytes, 301 instruction hunks.
- goal_minsize_movie — Use the compiler minsize movie optimization objective with the original timer body visible, preserving every donor branch. DIFFER 4467/5178 bytes, 307 instruction hunks.
- captured_movie_normal — Measure real implicit movie destructor with an isolated inline-depth region around the movie declaration under normal. DIFFER 5177/5178 bytes, 49 instruction hunks.
- captured_movie_below_timer — Measure real implicit movie destructor with an isolated inline-depth region around the movie declaration under below_timer. DIFFER 5177/5178 bytes, 49 instruction hunks.
- captured_movie_forced_movie — Measure real implicit movie destructor with an isolated inline-depth region around the movie declaration under forced_movie. DIFFER 5199/5178 bytes, 51 instruction hunks.
- captured_resourcebase_normal — Measure real implicit movie destructor with an isolated inline-depth region around the resourcebase declaration under normal. DIFFER 5134/5178 bytes, 35 instruction hunks.
- captured_resourcebase_below_timer — Measure real implicit movie destructor with an isolated inline-depth region around the resourcebase declaration under below_timer. DIFFER 5134/5178 bytes, 35 instruction hunks.
- captured_resourcebase_forced_movie — Measure real implicit movie destructor with an isolated inline-depth region around the resourcebase declaration under forced_movie. DIFFER 5247/5178 bytes, 73 instruction hunks.
- captured_including_base_normal — Measure real implicit movie destructor with an isolated inline-depth region around the including_base declaration under normal. DIFFER 5177/5178 bytes, 49 instruction hunks.
- captured_including_base_below_timer — Measure real implicit movie destructor with an isolated inline-depth region around the including_base declaration under below_timer. DIFFER 5177/5178 bytes, 49 instruction hunks.
- captured_including_base_forced_movie — Measure real implicit movie destructor with an isolated inline-depth region around the including_base declaration under forced_movie. DIFFER 5199/5178 bytes, 51 instruction hunks.
- split_capture_open — Isolate capture of the real implicit destructor at open with ordinary resource-base expansion. DIFFER 5134/5178 bytes, 35 instruction hunks.
- split_capture_open_earlybase — Isolate capture of the real implicit destructor at open with early resource-base expansion. DIFFER 5134/5178 bytes, 35 instruction hunks.
- split_capture_close — Isolate capture of the real implicit destructor at close with ordinary resource-base expansion. DIFFER 5177/5178 bytes, 49 instruction hunks.
- split_capture_close_earlybase — Isolate capture of the real implicit destructor at close with early resource-base expansion. DIFFER 5177/5178 bytes, 49 instruction hunks.
- split_capture_ctor_exempt — Isolate capture of the real implicit destructor at ctor_exempt with ordinary resource-base expansion. DIFFER 5177/5178 bytes, 49 instruction hunks.
- split_capture_ctor_exempt_earlybase — Isolate capture of the real implicit destructor at ctor_exempt with early resource-base expansion. DIFFER 5177/5178 bytes, 49 instruction hunks.
- split_capture_depth1 — Isolate capture of the real implicit destructor at depth1 with ordinary resource-base expansion. DIFFER 5131/5178 bytes, 114 instruction hunks.
- split_capture_depth1_earlybase — Isolate capture of the real implicit destructor at depth1 with early resource-base expansion. DIFFER 5131/5178 bytes, 114 instruction hunks.
- movie_original_ctor_after_class — Define the genuine default Movie constructor outside the class, at normal depth, so only the generated destructor captures the closing-brace setting. DIFFER 5134/5178 bytes, 35 instruction hunks.
- movie_original_ctor_support_header — Define the genuine default Movie constructor outside the class, at normal depth, so only the generated destructor captures the closing-brace setting. DIFFER 5134/5178 bytes, 35 instruction hunks.
- movie_generated_local_size — Independently change the code-size objective of the naturally generated movie members while retaining normal caller and timer optimization: size. DIFFER 5134/5178 bytes, 35 instruction hunks.
- movie_generated_local_omitframe — Independently change the code-size objective of the naturally generated movie members while retaining normal caller and timer optimization: omitframe. DIFFER 5134/5178 bytes, 35 instruction hunks.
- movie_generated_local_global — Independently change the code-size objective of the naturally generated movie members while retaining normal caller and timer optimization: global. DIFFER 5134/5178 bytes, 35 instruction hunks.
- movie_generated_local_speed — Independently change the code-size objective of the naturally generated movie members while retaining normal caller and timer optimization: speed. DIFFER 5134/5178 bytes, 35 instruction hunks.
- qp_bandit_nested_ctor and qp_bandit_nested_ctor_and_base_dtor — Independently forced the real counted-pointer and resource-base default constructors on the 5,177-byte alternative. The second build also forced the original empty resource-base destructor. Both produce the same 5,177 bytes and 49 instruction hunks as captured_movie_normal.

## Subsequent retry

The selected leaf retains the source with both original post-wait termination checks. Retail and this source take the same observable cleanup sequence for all 46 cancellation checks after inline timer deregistration is identified with its real destructor call. Native and checked builds still differ in where the code for destruction is expanded. The local operand audit now accounts for all 242 emitted relocation operands.

A conventional single-statement SAY macro around eleven utterances produced 5,177 bytes and 36 differing groups. Its unoptimized code adds eleven conditionals. The checked donor has 123 conditional decisions. The original structure reproduces all 123 observable-successor signatures, while the SAY rewrite reproduces only 90 out of its own 134. The alternative is rejected. Changing the legacy loop spelling, using implicit movie and pause constructors, and changing the optimization of the actual movie member produced no closer admissible version.

The selected TU also emits a 25-byte movie destructor for a 66-byte separately owned native body. Removing the declaration-only movie counted-pointer destructor lets the compiler emit the original 66 bytes. Across the three measured shared-resource configurations the parent then builds to 5,116 bytes and 58 groups. The tradeoff is important when linking an assembled project. See RESOURCE_HELPERS.json for the selected and diagnostic helper bodies.

Individual compiler results are listed in RETRY_GRADES.tsv.
