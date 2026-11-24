# VC7.1 source probes

| Build | Bytes | Matched instructions | Mapped pick-any EQ/NEQ | Source change |
|---|---:|---:|---:|---|
| baseline | 1190 v 1203 | 294 / 367 | 5 / 0 | Recompile the parked source as supplied. |
| tail_explicit_accumulator_assignment | 1190 v 1203 | 294 / 367 | 5 / 0 | Spell the accumulation as an explicit read/compute/store. |
| tail_explicit_scripting_handle_guard | 1204 v 1203 | 291 / 367 | 5 / 0 | Place the entity-unpause call on each edge of the scripting-handle release guard. |
| tail_guard_nested_in_fallthrough | 1190 v 1203 | 294 / 367 | 5 / 0 | Keep the success body as a positive guard and put the fallback in its explicit alternate arm. |
| tail_scoped_while_traversal | COMPILE_FAIL | — | — | Use an explicit loop body and post-test iterator advance. |
| tail_swap_conditional_arms | 1190 v 1203 | 289 / 367 | 5 / 0 | Swap conditional arms. |
| tail_oracle_donor | COMPILE_FAIL | — | — | Oracle donor. |
| tail_oracle_donor_first_copy_scope | COMPILE_FAIL | — | — | Oracle donor first copy scope. |
| tail_oracle_donor_implicit_copy | COMPILE_FAIL | — | — | Oracle donor implicit copy. |
| tail_oracle_donor_named_predicate | COMPILE_FAIL | — | — | Oracle donor named predicate. |
| tail_oracle_donor_noinline_returns | COMPILE_FAIL | — | — | Oracle donor noinline returns. |
| tail_oracle_l2_first_bare | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 first bare. |
| tail_oracle_l2_first_braced | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 first braced. |
| tail_oracle_l2_first_guard_collected_reference | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 first guard collected reference. |
| tail_oracle_l2_first_guard_false_block | 1203 v 1203 | 362 / 367 | 11 / 0 | Oracle l2 first guard false block. |
| tail_oracle_l2_first_guard_inline_control | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 first guard inline control. |
| tail_oracle_l2_first_guard_leave_else | 1203 v 1203 | 362 / 367 | 11 / 0 | Oracle l2 first guard leave else. |
| tail_oracle_l2_first_named_fail | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 first named fail. |
| tail_oracle_l2_first_void_expression | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 first void expression. |
| tail_oracle_l2_line_increase_bare | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 line increase bare. |
| tail_oracle_l2_line_increase_braces | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 line increase braces. |
| tail_oracle_l2_line_increase_depth | 1227 v 1203 | 292 / 367 | 11 / 0 | Oracle l2 line increase depth. |
| tail_oracle_l2_line_increase_local_condition | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 line increase local condition. |
| tail_oracle_l2_line_increase_void | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 line increase void. |
| tail_oracle_l2_pause_access_ref | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 pause access ref. |
| tail_oracle_l2_pause_bind_value | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 pause bind value. |
| tail_oracle_l2_pause_early_receiver_member | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 pause early receiver member. |
| tail_oracle_l2_pause_literal_enum | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 pause literal enum. |
| tail_oracle_l2_pause_ptr | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 pause ptr. |
| tail_oracle_l2_task_after_bare | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 task after bare. |
| tail_oracle_l2_task_after_braces | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 task after braces. |
| tail_oracle_l2_task_after_depth | 1227 v 1203 | 289 / 367 | 11 / 0 | Oracle l2 task after depth. |
| tail_oracle_l2_task_after_local_condition | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 task after local condition. |
| tail_oracle_l2_task_after_void | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l2 task after void. |
| tail_oracle_l3_conditional_line_order | 1203 v 1203 | 361 / 367 | 11 / 0 | Oracle l3 conditional line order. |
| tail_oracle_l3_first_return_expr_depth | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 first return expr depth. |
| tail_oracle_l3_flag_Oa | 1258 v 1203 | 242 / 367 | 6 / 5 | Oracle l3 flag oa. |
| tail_oracle_l3_flag_Ob2 | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 flag ob2. |
| tail_oracle_l3_flag_Og | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 flag og. |
| tail_oracle_l3_flag_OgOff | 1852 v 1203 | 104 / 367 | 0 / 3 | Oracle l3 flag ogoff. |
| tail_oracle_l3_flag_Oi | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 flag oi. |
| tail_oracle_l3_flag_Op | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 flag op. |
| tail_oracle_l3_flag_Ot | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 flag ot. |
| tail_oracle_l3_flag_d2inlT56 | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 flag d2inlt56. |
| tail_oracle_l3_flag_d2inlT63 | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 flag d2inlt63. |
| tail_oracle_l3_flag_d2inlT66 | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 flag d2inlt66. |
| tail_oracle_l3_initial_goto | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 initial goto. |
| tail_oracle_l3_initial_goto_empty | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 initial goto empty. |
| tail_oracle_l3_line_guard_local_scope | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 line guard local scope. |
| tail_oracle_l3_loop_for_condition_status | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 loop for condition status. |
| tail_oracle_l3_original_guard_only_if_true | 1206 v 1203 | 280 / 367 | 11 / 0 | Oracle l3 original guard only if true. |
| tail_oracle_l3_separate_initial_lifetime | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 separate initial lifetime. |
| tail_oracle_l3_try_initial_guard_late | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle l3 try initial guard late. |
| tail_oracle_last_default_stores | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last default stores. |
| tail_oracle_last_do_for_combine | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle last do for combine. |
| tail_oracle_last_explicit_loop_early_break | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle last explicit loop early break. |
| tail_oracle_last_first_return_expression | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle last first return expression. |
| tail_oracle_last_first_return_ordinary | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle last first return ordinary. |
| tail_oracle_last_first_return_ordinary_block | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle last first return ordinary block. |
| tail_oracle_last_inline_budget_54 | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle last inline budget 54. |
| tail_oracle_last_inline_budget_59 | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle last inline budget 59. |
| tail_oracle_last_inline_budget_60 | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle last inline budget 60. |
| tail_oracle_last_inline_budget_62 | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle last inline budget 62. |
| tail_oracle_last_inline_budget_65 | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle last inline budget 65. |
| tail_oracle_last_inline_budget_67 | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last inline budget 67. |
| tail_oracle_last_inline_budget_68 | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last inline budget 68. |
| tail_oracle_last_inline_budget_70 | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last inline budget 70. |
| tail_oracle_last_inline_budget_71 | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last inline budget 71. |
| tail_oracle_last_inline_budget_72 | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last inline budget 72. |
| tail_oracle_last_inline_budget_73 | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last inline budget 73. |
| tail_oracle_last_inline_budget_74 | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last inline budget 74. |
| tail_oracle_last_inline_budget_76 | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last inline budget 76. |
| tail_oracle_last_inline_budget_78 | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last inline budget 78. |
| tail_oracle_last_inline_budget_80 | 1265 v 1203 | 359 / 367 | 11 / 0 | Oracle last inline budget 80. |
| tail_oracle_last_original_complete_members_pass22 | 1203 v 1203 | 363 / 367 | 12 / 0 | Oracle last original complete members pass22. |
| tail_oracle_last_releasedata | 1203 v 1203 | 363 / 367 | 12 / 0 | Oracle last releasedata. |
| tail_oracle_last_return_values_with_comma | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last return values with comma. |
| tail_oracle_last_return_values_without_helper | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle last return values without helper. |
| tail_oracle_shape_predicate | 1287 v 1203 | 318 / 367 | 11 / 0 | Oracle shape predicate. |
| tail_oracle_shape_typed_budget_105 | 1419 v 1203 | 321 / 367 | 11 / 0 | Oracle shape typed budget 105. |
| tail_oracle_shape_typed_budget_50 | 1320 v 1203 | 314 / 367 | 9 / 2 | Oracle shape typed budget 50. |
| tail_oracle_shape_typed_budget_59 | 1220 v 1203 | 367 / 367 | 11 / 0 | Oracle shape typed budget 59. |
| tail_oracle_shape_typed_budget_86 | 1419 v 1203 | 321 / 367 | 11 / 0 | Oracle shape typed budget 86. |
| tail_oracle_shape_typed_dowhile | 1134 v 1203 | 276 / 367 | 11 / 0 | Oracle shape typed dowhile. |
| tail_oracle_shape_typed_initializer_loop | 1203 v 1203 | 363 / 367 | 11 / 0 | Oracle shape typed initializer loop. |
| tail_oracle_shape_typed_loop_condition | 1277 v 1203 | 323 / 367 | 11 / 0 | Oracle shape typed loop condition. |
| tail_oracle_shape_typed_loop_condition_forced_pause_dtor | 1277 v 1203 | 323 / 367 | 11 / 0 | Oracle shape typed loop condition forced pause dtor. |
| tail_oracle_shape_typed_loop_condition_ordinary_returns | 1277 v 1203 | 323 / 367 | 11 / 0 | Oracle shape typed loop condition ordinary returns. |
| tail_oracle_shape_typed_loop_condition_partial_dtor_calls | 1200 v 1203 | 273 / 367 | 11 / 0 | Oracle shape typed loop condition partial dtor calls. |
| tail_oracle_shape_typed_loop_condition_release_chain | 1277 v 1203 | 323 / 367 | 12 / 0 | Oracle shape typed loop condition release chain. |
| tail_oracle_typed_donor | 1306 v 1203 | 347 / 367 | 11 / 0 | Oracle typed donor. |
| tail_oracle_typed_donor_first_copy_scope | 1303 v 1203 | 318 / 367 | 11 / 0 | Oracle typed donor first copy scope. |
| tail_oracle_typed_donor_implicit_copy | 1306 v 1203 | 347 / 367 | 10 / 0 | Oracle typed donor implicit copy. |
| tail_oracle_typed_donor_named_predicate | 1306 v 1203 | 347 / 367 | 11 / 0 | Oracle typed donor named predicate. |
| tail_oracle_typed_donor_noinline_returns | 1224 v 1203 | 262 / 367 | 11 / 0 | Oracle typed donor noinline returns. |
| tail_oracle_scheduling_bracket_AfterAddValid | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket afteraddvalid. |
| tail_oracle_scheduling_bracket_FirstExternalReturn | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket firstexternalreturn. |
| tail_oracle_scheduling_bracket_FirstTerminatingCall | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket firstterminatingcall. |
| tail_oracle_scheduling_bracket_Health | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket health. |
| tail_oracle_scheduling_bracket_Movie | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket movie. |
| tail_oracle_scheduling_bracket_NextLineAdd | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket nextlineadd. |
| tail_oracle_scheduling_bracket_ReleaseValid | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket releasevalid. |
| tail_oracle_scheduling_bracket_ResetPredicate | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket resetpredicate. |
| tail_oracle_scheduling_bracket_ScriptingEntityPriority | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket scriptingentitypriority. |
| tail_oracle_scheduling_bracket_SpeechStringFinalArgument | 1203 v 1203 | 360 / 367 | 11 / 0 | Oracle scheduling bracket speechstringfinalargument. |
| tail_oracle_scheduling_bracket_TalkFirstArgument | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket talkfirstargument. |
| tail_oracle_scheduling_bracket_TalkMessage | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket talkmessage. |
| tail_oracle_scheduling_bracket_TalkObject | COMPILE_FAIL | — | — | Oracle scheduling bracket talkobject. |
| tail_oracle_scheduling_bracket_TaskCheck | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling bracket taskcheck. |
| tail_oracle_scheduling_include_first_C3DVector | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first c3dvector. |
| tail_oracle_scheduling_include_first_C3DVector.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first c3dvector.h. |
| tail_oracle_scheduling_include_first_CCountedPointer | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first ccountedpointer. |
| tail_oracle_scheduling_include_first_CCountedPointer.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first ccountedpointer.h. |
| tail_oracle_scheduling_include_first_CGameScriptInterfaceBase.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first cgamescriptinterfacebase.h. |
| tail_oracle_scheduling_include_first_CIsThingAlive.h | COMPILE_FAIL | — | — | Oracle scheduling include first cisthingalive.h. |
| tail_oracle_scheduling_include_first_COracleMinigameTestScript.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first coracleminigametestscript.h. |
| tail_oracle_scheduling_include_first_CScriptGameResourceObjectMovieBase.h | COMPILE_FAIL | — | — | Oracle scheduling include first cscriptgameresourceobjectmoviebase.h. |
| tail_oracle_scheduling_include_first_CScriptGameResourceObjectScriptedThingBase.h | COMPILE_FAIL | — | — | Oracle scheduling include first cscriptgameresourceobjectscriptedthingbase.h. |
| tail_oracle_scheduling_include_first_CWideScreenMagicPauseEntities.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first cwidescreenmagicpauseentities.h. |
| tail_oracle_scheduling_include_first_EScriptAIPriority.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first escriptaipriority.h. |
| tail_oracle_scheduling_include_first_EScriptingStateGroups.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first escriptingstategroups.h. |
| tail_oracle_scheduling_include_first_ETextGroupSelectionMethod.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first etextgroupselectionmethod.h. |
| tail_oracle_scheduling_include_first_TRetVoid.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first tretvoid.h. |
| tail_oracle_scheduling_include_first_fable_string.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include first fable string.h. |
| tail_oracle_scheduling_include_last_C3DVector | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last c3dvector. |
| tail_oracle_scheduling_include_last_C3DVector.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last c3dvector.h. |
| tail_oracle_scheduling_include_last_CCountedPointer | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last ccountedpointer. |
| tail_oracle_scheduling_include_last_CCountedPointer.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last ccountedpointer.h. |
| tail_oracle_scheduling_include_last_CGameScriptInterfaceBase.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last cgamescriptinterfacebase.h. |
| tail_oracle_scheduling_include_last_CIsThingAlive.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last cisthingalive.h. |
| tail_oracle_scheduling_include_last_COracleMinigameTestScript.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last coracleminigametestscript.h. |
| tail_oracle_scheduling_include_last_CScriptGameResourceObjectMovieBase.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last cscriptgameresourceobjectmoviebase.h. |
| tail_oracle_scheduling_include_last_CScriptGameResourceObjectScriptedThingBase.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last cscriptgameresourceobjectscriptedthingbase.h. |
| tail_oracle_scheduling_include_last_CWideScreenMagicPauseEntities.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last cwidescreenmagicpauseentities.h. |
| tail_oracle_scheduling_include_last_EScriptAIPriority.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last escriptaipriority.h. |
| tail_oracle_scheduling_include_last_EScriptingStateGroups.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last escriptingstategroups.h. |
| tail_oracle_scheduling_include_last_ETextGroupSelectionMethod.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last etextgroupselectionmethod.h. |
| tail_oracle_scheduling_include_last_fable_string.h | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling include last fable string.h. |
| tail_oracle_scheduling_source_move_base_constructor | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling source move base constructor. |
| tail_oracle_scheduling_source_move_constructor | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle scheduling source move constructor. |
| tail_oracle_edges_after_compare_not | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges after compare not. |
| tail_oracle_edges_after_do_while | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges after do while. |
| tail_oracle_edges_after_for_test | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges after for test. |
| tail_oracle_edges_after_if_then_else | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges after if then else. |
| tail_oracle_edges_after_while_test | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges after while test. |
| tail_oracle_edges_all_compare_not | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges all compare not. |
| tail_oracle_edges_all_conditional_return | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges all conditional return. |
| tail_oracle_edges_all_do_while | 1377 v 1203 | 358 / 367 | 11 / 0 | Oracle edges all do while. |
| tail_oracle_edges_all_for_test | 1127 v 1203 | 274 / 367 | 11 / 0 | Oracle edges all for test. |
| tail_oracle_edges_all_if_then_else | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges all if then else. |
| tail_oracle_edges_all_while_test | 1127 v 1203 | 274 / 367 | 11 / 0 | Oracle edges all while test. |
| tail_oracle_edges_first_compare_not | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges first compare not. |
| tail_oracle_edges_first_conditional_return | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges first conditional return. |
| tail_oracle_edges_first_do_while | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges first do while. |
| tail_oracle_edges_first_for_test | 1127 v 1203 | 274 / 367 | 11 / 0 | Oracle edges first for test. |
| tail_oracle_edges_first_if_then_else | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges first if then else. |
| tail_oracle_edges_first_while_test | 1127 v 1203 | 274 / 367 | 11 / 0 | Oracle edges first while test. |
| tail_oracle_edges_after_boolean_context | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges after boolean context. |
| tail_oracle_edges_after_conditional_return | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges after conditional return. |
| tail_oracle_edges_after_dereferenced_failval | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges after dereferenced failval. |
| tail_oracle_edges_after_negated_and_more | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges after negated and more. |
| tail_oracle_edges_all_boolean_context | 1203 v 1203 | 364 / 367 | 11 / 0 | Oracle edges all boolean context. |
| tail_oracle_edges_all_dereferenced_failval | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges all dereferenced failval. |
| tail_oracle_edges_all_negated_and_more | 1190 v 1203 | 295 / 367 | 11 / 0 | Oracle edges all negated and more. |
| tail_oracle_edges_first_boolean_context | 1203 v 1203 | 364 / 367 | 11 / 0 | Oracle edges first boolean context. |
| tail_oracle_edges_first_dereferenced_failval | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges first dereferenced failval. |
| tail_oracle_edges_first_explicit_bool_local | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges first explicit bool local. |
| tail_oracle_edges_first_negated_and_more | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges first negated and more. |
| tail_oracle_edges_head_scope_initial | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges head scope initial. |
| tail_oracle_edges_head_scope_predicate_frame | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges head scope predicate frame. |
| tail_oracle_edges_increment_first_line_split | 1197 v 1203 | 286 / 367 | 11 / 0 | Oracle edges increment first line split. |
| tail_oracle_edges_increment_select_wrap | 1195 v 1203 | 285 / 367 | 11 / 0 | Oracle edges increment select wrap. |
| tail_oracle_edges_main_continue_untalked | 1216 v 1203 | 296 / 367 | 11 / 0 | Oracle edges main continue untalked. |
| tail_oracle_edges_main_loop_manual_test | 1203 v 1203 | 367 / 367 | 11 / 0 | Read the termination condition in the body before the talked-to check. The primary and eleven mapped copies match. |
| tail_oracle_edges_ordinary_for_explicit_else | 1277 v 1203 | 323 / 367 | 11 / 0 | Oracle edges ordinary for explicit else. |
| tail_oracle_edges_pause_ctor_assignment | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges pause ctor assignment. |
| tail_oracle_edges_pause_ctor_assignment_after | 1203 v 1203 | 366 / 367 | 10 / 1 | Oracle edges pause ctor assignment after. |
| tail_oracle_edges_pause_ptr_dtor | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges pause ptr dtor. |
| tail_oracle_edges_pause_readonly_dtor | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges pause readonly dtor. |
| tail_oracle_edges_pause_ref_ctor | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges pause ref ctor. |
| tail_oracle_edges_pause_resumed_flag_last | 1203 v 1203 | 366 / 367 | 11 / 0 | Oracle edges pause resumed flag last. |
| tail_oracle_ready_exact_bare | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle ready exact bare. |
| tail_oracle_ready_exact_no_guard_depth | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact no guard depth. |
| tail_oracle_ready_exact_original_budget_50 | 1237 v 1203 | 366 / 367 | 9 / 2 | Lower inline budget keeps two linked helper copies different and increases the leaf to 1237 bytes. |
| tail_oracle_ready_exact_original_budget_55 | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle ready exact original budget 55. |
| tail_oracle_ready_exact_original_budget_59 | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle ready exact original budget 59. |
| tail_oracle_ready_exact_original_budget_60 | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle ready exact original budget 60. |
| tail_oracle_ready_exact_original_budget_63 | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle ready exact original budget 63. |
| tail_oracle_ready_exact_original_budget_65 | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle ready exact original budget 65. |
| tail_oracle_ready_exact_original_budget_67 | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact original budget 67. |
| tail_oracle_ready_exact_original_budget_69 | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact original budget 69. |
| tail_oracle_ready_exact_original_budget_71 | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact original budget 71. |
| tail_oracle_ready_exact_original_budget_73 | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact original budget 73. |
| tail_oracle_ready_exact_original_budget_74 | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact original budget 74. |
| tail_oracle_ready_exact_original_budget_75 | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact original budget 75. |
| tail_oracle_ready_exact_original_budget_76 | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact original budget 76. |
| tail_oracle_ready_exact_original_budget_77 | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact original budget 77. |
| tail_oracle_ready_exact_original_budget_78 | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle ready exact original budget 78. |
| tail_oracle_ready_exact_original_budget_80 | 1265 v 1203 | 366 / 367 | 11 / 0 | Oracle ready exact original budget 80. |
| tail_oracle_ready_exact_original_budget_85 | 1265 v 1203 | 366 / 367 | 11 / 0 | Oracle ready exact original budget 85. |
| tail_oracle_ready_exact_original_return | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle ready exact original return. |
| tail_oracle_scale_51_depthFalse | 1219 v 1203 | 359 / 367 | 11 / 0 | Oracle scale 51 depthfalse. |
| tail_oracle_scale_51_depthTrue | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle scale 51 depthtrue. |
| tail_oracle_scale_52_depthFalse | 1219 v 1203 | 359 / 367 | 11 / 0 | Oracle scale 52 depthfalse. |
| tail_oracle_scale_52_depthTrue | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle scale 52 depthtrue. |
| tail_oracle_scale_53_depthFalse | 1219 v 1203 | 359 / 367 | 11 / 0 | Oracle scale 53 depthfalse. |
| tail_oracle_scale_53_depthTrue | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle scale 53 depthtrue. |
| tail_oracle_scale_54_depthFalse | 1219 v 1203 | 359 / 367 | 11 / 0 | Oracle scale 54 depthfalse. |
| tail_oracle_scale_54_depthTrue | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle scale 54 depthtrue. |
| tail_oracle_scale_55_depthFalse | 1219 v 1203 | 359 / 367 | 11 / 0 | Oracle scale 55 depthfalse. |
| tail_oracle_scale_55_depthTrue | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle scale 55 depthtrue. |
| tail_oracle_scale_59_depthFalse | 1219 v 1203 | 359 / 367 | 11 / 0 | Oracle scale 59 depthfalse. |
| tail_oracle_scale_59_depthTrue | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle scale 59 depthtrue. |
| tail_oracle_scale_65_depthFalse | 1219 v 1203 | 359 / 367 | 11 / 0 | Oracle scale 65 depthfalse. |
| tail_oracle_scale_65_depthTrue | 1203 v 1203 | 367 / 367 | 11 / 0 | Oracle scale 65 depthtrue. |
| tail_oracle_scale_74_depthFalse | 1250 v 1203 | 354 / 367 | 11 / 0 | Oracle scale 74 depthfalse. |
| tail_oracle_scale_74_depthTrue | 1234 v 1203 | 362 / 367 | 11 / 0 | Oracle scale 74 depthtrue. |
| tail_oracle_verified_selected | 1203 v 1203 | 367 / 367 | 11 / 0 | The explicit termination check in the frame loop matches both cleanup exits. /d2inlT51 keeps the original TRetVoid returns and their necessary inline split. |
| merged_ff415bcb32cd_76ffc746 | 1203 v 1203 | 367 / 367 | 11 / 0 | Merged ff415bcb32cd 76ffc746. |
| merged_bb8d29680465_76ffc746 | 1203 v 1203 | 367 / 367 | 11 / 0 | Merged bb8d29680465 76ffc746. |
| merged_ff70691f9dc0_76ffc746 | 1203 v 1203 | 367 / 367 | 11 / 0 | Merged ff70691f9dc0 76ffc746. |
| merged_3fa25c46799c_76ffc746 | 1203 v 1203 | 367 / 367 | 11 / 0 | Merged 3fa25c46799c 76ffc746. |
| merged_49e49c338a76_76ffc746 | 1203 v 1203 | 367 / 367 | 11 / 0 | Merged 49e49c338a76 76ffc746. |
| merged_94780123ef45_76ffc746 | 1203 v 1203 | 367 / 367 | 11 / 0 | Merged 94780123ef45 76ffc746. |
| merged_b832b33efc4a_76ffc746 | 1203 v 1203 | 367 / 367 | 11 / 0 | Merged b832b33efc4a 76ffc746. |
| merged_98d5fd458706_76ffc746 | 1203 v 1203 | 367 / 367 | 11 / 0 | Merged 98d5fd458706 76ffc746. |
| merged_3c58f9b8349e_76ffc746 | 1203 v 1203 | 367 / 367 | 11 / 0 | Merged 3c58f9b8349e 76ffc746. |
