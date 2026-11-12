# Pass21 probes

| Attempt | Result |
|---|---|
| sol_b00_current_best | COMPILE_FAIL None/None; strict 0 |
| sol_b01_rebased_owner | DIFFER 6244/6164; strict 0.953189 |
| sol_s01_predicate_depth | DIFFER 6196/6164; strict 0.945315 |
| sol_f01_movie_budget100 | DIFFER 6624/6164; strict 0.791932 |
| sol_f02_budget75 | DIFFER 6212/6164; strict 0.946183 |
| sol_f02_budget76 | DIFFER 6212/6164; strict 0.946183 |
| sol_f02_budget78 | DIFFER 6212/6164; strict 0.946183 |
| sol_f02_budget80 | DIFFER 6212/6164; strict 0.946183 |
| sol_f02_budget85 | DIFFER 6632/6164; strict 0.809019 |
| sol_s02_loop_frequency | DIFFER 6196/6164; strict 0.945315 |
| sol_f03_neighbour_budget15 | DIFFER 6096/6164; strict 0.856985 |
| sol_f04_movie_budget20 | DIFFER 6096/6164; strict 0.856985 |
| sol_s03_implicit_movie_plain_predicate | COMPILE_FAIL None/None; strict 0 |
| sol_s03b_implicit_movie_plain_predicate | DIFFER 6196/6164; strict 0.945315 |
| sol_s04_original_idle_loop_postguards | DIFFER 6180/6164; strict 0.958196 |
| sol_f04_original_idle_budget15 | DIFFER 6088/6164; strict 0.865523 |
| sol_s05_final_brace_cleanup_depth | DIFFER 6132/6164; strict 0.698975 |
| sol_s06_idle_return_cleanup_depth | DIFFER 6132/6164; strict 0.698975 |
| sol_s06b_idle_return_cleanup_depth | DIFFER 6116/6164; strict 0.868676 |
| sol_s07_called_handle_dtor_implicit_movie | DIFFER 6088/6164; strict 0.865523 |
| sol_f05_original_idle_budget10 | DIFFER 6104/6164; strict 0.851381 |
| sol_s08_native_cold_timer_returns_depth | DIFFER 6104/6164; strict 0.872093 |
| sol_f06_cold_returns_budget14 | DIFFER 6132/6164; strict 0.89619 |
| sol_s09_late_movie_return_depth_one | DIFFER 6104/6164; strict 0.872093 |
| sol_s10_original_timer_comparison | DIFFER 6104/6164; strict 0.872093 |
| sol_s11_late_movie_return_depth_zero | DIFFER 6132/6164; strict 0.951091 |
| sol_s12_visible_called_movie_pointer_dtor | DIFFER 6104/6164; strict 0.872093 |
| sol_s13_late_loop_true_guard | DIFFER 6104/6164; strict 0.872093 |
| sol_s14_outofclass_genuine_base_dtor | DIFFER 6132/6164; strict 0.89619 |
| sol_s15_ordinary_timer_dtor | DIFFER 6104/6164; strict 0.872093 |
| sol_s16_timer_dtor_definition_after_main | DIFFER 6104/6164; strict 0.872093 |

Sixteen source strategies including prior two. s03 genuine implicit movie ctor and plain predicate dtor depth0 (samebytes), original local names/TRetVoid restored. First s03 compile failed because local identifier renaming missed some occurrences; s03b is the build repair. s04 restores original donor while(true){NewScriptFrame();T;} idle loop, missing post-late-loopT and dead finalT:6196→6180, strict95.82%. s05 scoped final function brace depth0 withS15 worsens all allocation; s06b scopes only final idle T, restoring called Timer/frame but not Movie cleanup partition. The first s06 grade accidentally reused s05 after a script typo; it is not a source attempt. s07 declaration-only STB dtor neutral. s08 scopes only native cold Timer returns OUTSIDE movie scopes (postearlyloop, latetop/attack/chat guards, postlate/idle/deadlast guards) atdepth0 underS15. This gives6104/6164 with ONLYone real loose hunk: missing60byte duplicate cold Movie cleanup CF65E4..661F; the other hunk disassembles switch data. All ordinary body instruction forms agree; slot offsets differ because the missing pad allows reuse. Native two hot Timer expansions + one called cold Timer; allfourMovie cleanups expanded.

s09 depth1 on late Movie returns neutral; s10 actual CTImer comparison wrapper neutral but donor explicitly calls GetTime (0361BB58), so wrapper removed. s11 depth0 on late Movie returns outlines Movie/Pause incorrectly. s12 visible ordinary called genuine Movie counted-pointer destructor body neutral. s13 late condition&&true neutral. s14 genuine GROBase destructor as out-of-class inline through private opt-in neutral vsS14 flag baseline; unused overlay removed from activehdr, history retained. s15 ordinary Timer dtor vsinline neutral; s16 define Timer dtor afterMain neutral. S10/S14 floors leave Movie calls, S15 expandsallMovies but also cold Timer, preventing correct late duplicate pad.

Current source is s08 structural candidate; s04 retained as best strict candidate. Currentstatus IN_PROGRESS, no parking. Full native/donor/table/source histories retained. Actual-body fixture pending eventual byte match.


Full unchanged Main fixture now PASS in both required compilers (runtime/sol_tests/00cf4ec0/main_run_001).17 meaningful scenarios cover all14 early switch cases with speaker identities/cadence and repeated insult tail; all7 late cases/chainmail grant/arena-over; first and late hit movies; concurrent hit conversation with original actor mapping; dead fan/ignored start failure/partial acquisition; both interrupted hit phases with actual persistent InHit flag; far/approaching hero; preexisting leave/arena-over; predicate exit and asynchronous leave. All resources balance. Source remains byte-unmatched IN_PROGRESS; passing semantics fixture is not a byte-match claim.

Latest checkpoint: retained all new probes; value-initialization variants were byte-neutral (Fan/Haunted), split separate proximity blocks worsened Sister to 89.02% strict despite exact length. Restored selected candidate; no strategy parked.

Genuine called rootdtor visibility s18 changes6104to6100 but worsens12loosehunks; discarded, selected s08 restored. 18 genuine source strategies retained.

Genuine handle-access rebase: both original PDBs prove private CScriptThing::PImp and private CTimer::TimerIndex (where present). Private row Thing overlay plus existing Timer opt-in enables exact access, no input/header-cache mutation. Full records PDB_HANDLE_TYPES.json. sol_model_a01_original_handle_access checks bytes against selected version; model correction not counted as source strategy. Main body/fixture unchanged.

Private CBaseObject::IntelligentPointer opt-in restored from BOTH original full class records (runtime/sol_base_object_type_evidence.json). Row-only full header overlay, no shared/input mutation. sol_model_a03_original_base_access is RAW-leaf byte-identical to selected previous version, recorded BASE_ACCESS_AUDIT.json. Actual Main/fixture unchanged. Model correction not counted as source strategy.

Original donor source-chain restoration: MovieBase02468220 and Thing018F0A90 counted-pointer destructors only call ReleaseData; donor02468530 release clears Data thenInfo; ShareData027C8B40 calls ReleaseData and increments stored Info. Actual disassemblies runtime/sol_donor_countedptr_*.disasm. Private complete header corrects the earlier retail-inferred source claim, without new helpers or padding. sol_model_a04_donor_counted_pointer_source is RAW-leaf byte-identical to selected prior version, COUNTED_POINTER_SOURCE_AUDIT.json. Actual Main/fixture unchanged. Direct retail/donor version-source uncertainty retained in private header; no shared mutation.

Both original resource classes have private PImp, confirmed BOTH full PDB records runtime/sol_resource_counted_pointer_pdb_types.json. Row-only full headers restore private access; Movie ctor inline controls use FABLE_FORCEINLINE alias. sol_model_a08_original_resource_access is RAW-entire-leaf identical, RESOURCE_ACCESS_AUDIT.json. Main/fixture unchanged, no input/shared mutation. Access/model regrade not counted as source strategy.

Source20 exposes the original CBaseObject constructor under guarded auto_inline(off), with the genuine visible root constructor. Main structure stays6104/6164 and strict slightly worsens87.04%; discarded. Source19 checkpoint remains selected before compatibility-only header correction.20 genuine source strategies retained.

Compatibility-only modela10 restores inactive legacy counted-pointer dtor and gates original ShareData stored-Info reload under existing RELEASEDATA. Normal RTM entire compiled leaf is RAW-identical to selected prior history, and every private header default token stream matches baseline with helper/access opt-ins off. Source/test body unchanged; no source-strategy count or new semantic-test claim. SOL_DEFAULT_COMPATIBILITY_AUDIT.json retains checks.

Inline ladder minimization p21 removes forced Pause constructor underS15, RAW-neutral6104. p22 makes both Timer GetTime and assignment plain, worsening6088/allocation. Isolated p23 removes only forced GetTime, RAW-neutral6104; this minimized source is selected. Assignment force remains required. No source-strategy count. Known-good observer on a10 validates identical normal leaf and records52nodes/33slots/frameE4. Built mergedPause slot weight21 (8+8+HL5),density5250 is belowBower5500/Oak5416; native actualstackorder PauseC/Bower10/Oak1C vsbuilt BowerC/Oak18/Pause24. Missing two-Pause-call pad may explain density ordering, but original IR weights are not observable and that remains a hypothesis. No fabricated locals or backend output retained as source.

Portable-source checkpoint a12 updates only stale source comments/line1, adds current Inline-budget/Retail notes, and replaces inherited bare calling conventions with existing FABLE_CDECL/FABLE_FASTCALL aliases where present. Entire normal RTM leaf is RAW-identical to the prior selected candidate; full Main body remains byte-for-byte text in the already passed fixture. No new semantic-test or source-strategy claim. SOL_PORTABILITY_CHECKPOINT.json retains hashes/validation.

Model a14 restores original private CIsThingAlive::Thing with a narrow opt-in. Both full PDB records prove access/type/offset; normal RTM entire leaf RAW-identical to sol_model_a12_portable_source_checkpoint. All 7 private header defaults tokenize identically to baseline. No source-strategy increment; existing actual-Main fixtures unchanged.

Model a16 restores BOTH original PDB declarations: protected Game reference at4 and protected Me at8, with narrow FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS. Constructor/member-access syntax follows original reference. Entire normal RTM leaf RAW-identical to sol_model_a14_original_predicate_access. All private header defaults tokenize identically to baseline. Actual full-Main fixture updated to reference member and rerun separately. Initial a15 missing-reference-argument compile fix retained, not counted source strategy.

Model a17 completes CPredicateBase with original public STLport unary_function<void,bool> base and protected static const Game pointer PGame. Both original full types agree; no body/control added. Narrow FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS guards inheritance, genuine shared STLport include and static declaration. Entire leaf RAW-identical to a16, default tokens identical. Full Main unchanged from the just-passed reference-member fixtures.

Model a19 enables the original public reference Game/MasterScriptData members in the genuine owning parent CLevelScriptBase (both original full PDB records). Main uses its own CEntityScriptBase Game and is unchanged. Entire normal RTM leaf RAW-identical to sol_model_a17_original_predicate_base. All private header default tokens identical; existing freshly passed full-body fixture is unchanged and reused. No source-strategy or new test-run claim.

## Final conservative completion audit

The selected source and every private .h file are exactly restored to `sol_model_a19_original_parent_level_references`. ATTEMPT_AUDIT.json/MD records 12 genuinely distinct compiled source families; it supersedes earlier informal cumulative totals and excludes flag-only/inline-only/name/default/model/regrade/build-fix/refinement counts. Final grounded callee-closure probes did not improve the selected residue. All individual history RESULT.json/candidate.cpp/header snapshots are retained. TEST_BODY.json verifies the complete selected Main compound body verbatim in the passing 17-scenario fixture (`runtime/sol_tests/00cf4ec0/main_run_002`); Ransommer uses the corrected genuine by-value GetScriptThing stub where applicable. PARK_READY.md and COMPLETION_AUDIT.json record the exact hashes, residual and meaningful historical alternate. Status is park-ready DIFFER, never MATCH.

## Final source-closure history ledger

| History | Result |
|---|---|
| `sol_source_s20_original_visible_object_constructor` | DIFFER; 6104/6164; 154 strict hunks |
| `sol_source_s21_genuine_called_task_query` | DIFFER; 6104/6164; 154 strict hunks |
| `sol_source_s22_genuine_called_validity_query` | DIFFER; 6084/6164; 158 strict hunks |
| `sol_source_s23_complete_called_termination_chain` | DIFFER; 6104/6164; 154 strict hunks |
| `sol_source_s24_genuine_called_get_script_thing` | DIFFER; 6104/6164; 154 strict hunks |

These entries are preserved diagnostics, not additional automatic strategy counts. ATTEMPT_AUDIT.json groups refinements and records the conservative qualifying subset. The selected checkpoint and private header set remain unchanged.
