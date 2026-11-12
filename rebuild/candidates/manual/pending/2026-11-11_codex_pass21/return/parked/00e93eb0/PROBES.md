# Pass21 probes

| Attempt | Result |
|---|---|
| sol_b00_current_best | DIFFER 5171/5321; strict 0.738865 |
| sol_s01_donor_guarded_candle_loops | DIFFER 5171/5321; strict 0.738865 |
| sol_s02_native_final_return_called | DIFFER 5151/5321; strict 0.807716 |
| sol_s03_only_final_return_member_called | DIFFER 5171/5321; strict 0.738865 |
| sol_s04_genuine_assignment_body | DIFFER 5171/5321; strict 0.738865 |
| sol_s05_original_virtual_alive_calls | DIFFER 5171/5321; strict 0.741058 |
| sol_s06_genuine_ordinary_empty_return | DIFFER 5171/5321; strict 0.741058 |
| sol_s07_return_definition_after_main | DIFFER 5171/5321; strict 0.741058 |
| sol_s08_return_after_main_scoped_call | DIFFER 5171/5321; strict 0.741058 |
| sol_s09_genuine_return_member_pointer | DIFFER 5197/5321; strict 0.810134 |
| sol_s10_guarded_house_wait_do_loop | DIFFER 5027/5321; strict 0.805961 |
| sol_s11_explicit_natural_return | DIFFER 5197/5321; strict 0.810134 |

Virtual IsAlive calls restored by named genuine const references; final typed Return member-pointer call folds direct and restores frameCC. Remaining duplicate vector cleanup layout/pad and slots. No arbitrary helper address.

All source attempts and private overlays retained in history. Current status IN_PROGRESS, not parked.

DONOR_MAIN03A1DB10/PDB_LOCALS prove DoneThisBefore, FXBarrier, Forcefield1/2, Candles, CellarDoor, LockableDoor and four scoped longi indices. All four candle loops have literal-false guards before their index initialization (03A1E06C,03A1EBE7,03A1EEBA,03A1F087); wrapping them is neutral.

Native E94489/E9449F dispatch IsAlive through virtual slot75 on the local forcefield object; the draft frontend-expanded the direct-object call. Named const CScriptThing& references preserve exactly those dispatches. The explicit genuine assignment body includes original implicit CBaseClass assignment then PImp assignment (donor02A75C10), but is byte-neutral vsoldimplicit switch.

Native final T calls the original empty TRetVoid::Return helperE95380 and needs frameCC. Scoped0 around the void-return expression outlinescleanup but still removes the empty member call; scoped0 around only a separated member call is neutral. Ordinary empty Return definitions before/after Main are neutral. Taking &TRetVoid::Return in a genuine typed member pointer only at finalT retains exactly a DIRECT call after folding and restores frameCC; no literal function address or substitute identity used. SourceS09 is current best5197/5321,81.0%strict. Guarded-do InHouse wait is5027,90.96%loose but poorlayout.

The temporary TRetVoid declaration overlay tested bys06..s08 is unused by bests09; removed from activehdr, preserved under all corresponding histories. No shared headers changed.


Checkpoint112: Haunted5197/5321: duplicatevector/Thingcleanupsharing andslotsremain. Genuinevisiblecalledspawnctor andbindingctorbothneutral; noflag. NativevirtualforcefieldIsAlive andfinaldirectReturncall restored bytypedreferences/member-pointer. No originalclaimfortheadditionaltypedlocalnames.

Complete current history (including duplicates/build repairs/flags, which do not count as source strategies):

| History | Bytes / strict |
|---|---|
| sol_b00_current_best | 5171/5321 0.738865 DIFFER |
| sol_s01_donor_guarded_candle_loops | 5171/5321 0.738865 DIFFER |
| sol_s02_native_final_return_called | 5151/5321 0.807716 DIFFER |
| sol_s03_only_final_return_member_called | 5171/5321 0.738865 DIFFER |
| sol_s04_genuine_assignment_body | 5171/5321 0.738865 DIFFER |
| sol_s05_original_virtual_alive_calls | 5171/5321 0.741058 DIFFER |
| sol_s06_genuine_ordinary_empty_return | 5171/5321 0.741058 DIFFER |
| sol_s07_return_definition_after_main | 5171/5321 0.741058 DIFFER |
| sol_s08_return_after_main_scoped_call | 5171/5321 0.741058 DIFFER |
| sol_s09_genuine_return_member_pointer | 5197/5321 0.810134 DIFFER |
| sol_s10_guarded_house_wait_do_loop | 5027/5321 0.805961 DIFFER |
| sol_s11_explicit_natural_return | 5197/5321 0.810134 DIFFER |
| sol_s12_visible_called_spawn_ctor | 5197/5321 0.810134 DIFFER |
| sol_s13_visible_called_binding_ctor | 5197/5321 0.810134 DIFFER |

## Full Main fixture, 2026-11-11
Entire active Main body copied verbatim except fixture-owner signature. Twenty-four scenarios cover paired forcefield regeneration/removal, single surviving field AND guard, region unload/reload, quest pre/post completion, four typed entity bindings and spawned CheckRegionLeaving ownership, candle counts 0..4, all light phases, immediate/preexisting state, and termination during each wait/vector phase including final typed Return. All local/wrapper resources balance; spawned resources survive Main until script destruction. Both actual VC7.1 /Od /W3 /GR- without /EHsc and 64-bit g++03 passed: `runtime/sol_tests/00e93eb0/main_run_001`.

Measured inline checkpoint: Direct const-ref forcefield casts keep5197bytes but worsen slots; selected named references retained. Verified inline trace shows matching expanded vector/Thing cascades but different sharing/layout; no synthetic helper bodies. All additional histories and diagnostic leaf-identical validations retained. 14 genuine source strategies, excluding flags/devices/build failures.

Latest checkpoint: retained all new probes; value-initialization variants were byte-neutral (Fan/Haunted), split separate proximity blocks worsened Sister to 89.02% strict despite exact length. Restored selected candidate; no strategy parked.

Additional checked source probes: final typed void-return expression, genuine forcefield pointer aliases, and original false-guard else spelling are byte-neutral5197. Per-expression scoped inline_depth(0) removes the extra references but changes wrapper inlining and worsens allocation (5197,76.10%); discarded device, not counted as source strategy. Original PDB lexical scopes saved in PDB_SCOPES.txt: all three persistent Things and Candles are root locals; both door locals share one nested initialization block, four longi indices each have their loop block. Current lifetimes agree with donor; no invented scopes. 19 distinct source strategies, not parked.

Root lifetime recovery: visible genuine called CBaseClass::~CBaseClass() empty body alone yields5180/5321,84.66%strict, and restores native FXBarrier/Forcefield1 slots plus the full prelude instruction stream through E94690 (raw branches still differ to cleanup destinations). Native7-byte helper exact after its one original-vtable binding; ROOT_DTOR_HELPER_AUDIT.json and both originalPDB root type/public records saved. Ctor-only neutral5197, bothlifetimes4750worse. Fresh diagnostic trace is leaf-identical; Thingcost remains60. Complete called spawn/binding ctor bodies combined with root dtor are neutral5180. Real string lifetime chain neutral5180; CCharString dtor12-byte helper exact, while UnassignString compiles a different scalar-deleting helper path, so no helper-match claim there. Private string overlay removed; genuine original type evidence retained in s24 history. 24 source strategies retained; selected minimal root-dtor-only candidate.

Minimal selected rootdtor source comment regrade sol_best_a02_root_dtor_visibility confirms5180/5321 at84.66%. Actual unchanged Main fixture freshly passes24 scenarios bothcompilers: runtime/sol_tests/00e93eb0/main_run_002. Ordinary Thingdtor is neutral(s25). Genuine visible called CScriptBase termination body(s26) measured separately; selected source retains original declaration visibility. 26 source strategies retained, not parked.

Genuine handle-access rebase: both original PDBs prove private CScriptThing::PImp and private CTimer::TimerIndex (where present). Private row Thing overlay plus existing Timer opt-in enables exact access, no input/header-cache mutation. Full records PDB_HANDLE_TYPES.json. sol_model_a01_original_handle_access checks bytes against selected version; model correction not counted as source strategy. Main body/fixture unchanged.

Final one-iteration termination while-loop form(s27) is byte-neutral5180, no score/layout improvement. Restored selected if form.27source strategies retained.

Donor source-graph recovery s30/s31: actual MovieBase02468220 and Thing018F0A90 counted-pointer dtors call ReleaseData; ReleaseData02468530 clears Data thenInfo; ShareData027C8B40 forwardsReleaseData and increments storedInfo. Full actual bodies runtime/sol_donor_countedptr_lifetimes.disasm and sol_donor_countedptr_constructors.disasm. Real callgraph changes Main5180/84.66% to4933/86.62%; no extra ReleaseData calls appear in the compiled leaf. Private full genuine header selected after fresh unmodified sol_model_a05_donor_counted_pointer_best regrade. Both actual24-scenario Main tests freshly PASS main_run003, bodyunchanged. Old5180 structural candidate is preserved as structural_history, not erased.

New leaf-identical trace(s30) shows genuine callgraph changes late Thing costs60→93..96 and vector44→69..71; old constant-cost assumptions no longer apply. FinalPMFremoval(s32)4905/84.07% and isolated guarded-do(s33)4868/78.54% worsen; this isolated loop probe avoids old s10 conflating bothchanges. SourceData-first versusretail-inferredInfo-first(s34) has same4933 length but higher strict86.62 vs84.80; selected donorData-first. All attempts retained; duplicate control forms are not counted again as distinctsource strategies. Current32source strategies counted; no parking or match claim.

## Genuine helper and root-lifetime checkpoint

Original donor Forcefield1.IsDead() && Forcefield2.IsDead() replaces inherited const-reference aliases/manual IsAlive negation. Real IsAlive is !PImp.IsNull() && PImp->IsAlive(). Both calls are byte-neutral in Main; emitted IsAlive28 and IsDead14 bytes individually match native, with raw virtual slots75/76 retained. Original root constructor alone visible and ordinarily called improves to5221/5321, strict90.32258%,8 loose hunks; both root lifetimes visible worsens4757. Exact9-byte constructor audit is ROOT_CTOR_HELPER_AUDIT.json. Final unmodified RTM grade sol_model_a09_original_root_ctor_best and all24 actual-body scenarios pass both compilers. Total35 distinct source strategies; visibility rechecks/flags excluded. Remains IN_PROGRESS.

Compatibility-only modela10 restores inactive legacy counted-pointer dtor and gates original ShareData stored-Info reload under existing RELEASEDATA. Normal RTM entire compiled leaf is RAW-identical to selected prior history, and every private header default token stream matches baseline with helper/access opt-ins off. Source/test body unchanged; no source-strategy count or new semantic-test claim. SOL_DEFAULT_COMPATIBILITY_AUDIT.json retains checks.

Inline p40/p41 isolate nativeForcefield-return vector/Thing cleanup using void/bare return forms; both byte-neutral5221. They are discarded. Relocation inspection confirms builtE94A2A already calls the correct vector destructor; nativeE94A24→E950D9 versus built inverted branch/fallthrough is block placement, not a missing vector-call choice. Do not interpret the loose alignment hunk as the wrong callee. Genuine residue remains duplicated expandedThing cleanup sharing/slots and block placement.

## Measured22-return cleanup correspondence

Fresh normal-leaf-identical v2 trace at a10 finds lateThing score74 rejected atT74; repeated expansion mutates cost60→93..96. LowestT75(p42) gives5373/5321,14strict hunks versus62, with all observed stack-slot uses aligned. It is retained as a structural alternative, not selected over the higher strict5221 candidate and not a match. runtime/sol_haunted_sites.py traverses all22 native true-return cleanup paths and records exact callee-choice correspondence. AtT75 sites1–19 agree. Site20postCellarDoor wants calledvector+3calledThings; builtvectorinline. Site21lastcandleT wants inlinevector+3calledThings; built2Thingsinline. Site22finalT wants inlinevector+2Thingsinline+1called; built3inline. No raw-match claim.

p43 depth0at20 reproduces its correct calls but shrinks5173 and lowers latercost so21/22both expand3Things. p44 depth1at21 is neutral. p45 force of genuine ReleaseData expansion and p46 depth0at alreadycalledForcefield exit are neutral againstp42; no unnecessary controls retained. s40 in-class genuine emptyThingdtor placement is neutral; s41 explicit pointer-null comparison is neutral; s42 early-null-return release form worsens4893 and is discarded. s43 guarded-do CellarDoor wait worsens5277/ordinaryloop choices, discarded. These four source forms bring39 distinct strategies; controls/revisited visibility combinations excluded. v43 complete genuine called termination predicate atcurrentT75 worsens6358 because visible clobber/capture information changes; discarded. Assertions were not introduced: original debug macro provenance is absent. Selected a10 source/headers restored; all failed candidates/histories retained.

Portable-source checkpoint a12 updates only stale source comments/line1, adds current Inline-budget/Retail notes, and replaces inherited bare calling conventions with existing FABLE_CDECL/FABLE_FASTCALL aliases where present. Entire normal RTM leaf is RAW-identical to the prior selected candidate; full Main body remains byte-for-byte text in the already passed fixture. No new semantic-test or source-strategy claim. SOL_PORTABILITY_CHECKPOINT.json retains hashes/validation.

Model a18 restores original CLevelScriptBase public Game and MasterScriptData references, proven BOTH full original types. GetMasterData still returns T*. Narrow flag FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS guards member and matching constructor initialization. Entire normal RTM leaf RAW-identical to sol_model_a12_portable_source_checkpoint. All private header default tokens identical. Actual complete Main fixture has reference Game member and freshly passes both compilers; no source-strategy increment.

Explicit instantiation of the already referenced genuine STLport vector<CScriptThing> destructor before Main atT75 is entire-leaf RAW-neutral to p42 (5373/14strict hunks). No fabricated helper body or new shared model; instantiation removed. This is an inline-visibility diagnostic, not a new distinct source strategy. Selected a18 restored; all history retained.

Fresh genuine vector destructor audit: all50native helper bytes at008AC970 match after only its singleactual free relocation, independently decoded to MSVCR71 free import. Raw loop branches and scalar-deleting virtualslot0 operands equal. VECTOR_DTOR_HELPER_AUDIT.json retains proof; no missing STLport vector body/helper chain found. Main still DIFFER.

New measured inline controls p47: keeping original defaultT74 while outlining only native-called post-cellar cancellation yields5173/5321,91.6157%strict versus prior90.3226%, and4loose hunks versus8. Full22-exit path map confirms first21 true cleanup choices match; finalT expands all3Things versus native2inline+1called. NativeFXBarrier28 andForcefield11C become built1C/28 because remaining cleanup weights differ; constructor order is original donor-proven, not arbitrarily reordered to chase slots. Fresh p42/p43 traces both normal-leaf RAW-identical, retained full relevant cost/frequency/acceptance records.

Original direct final-return and call-then-bare-return forms on correctedp47 context(s45/s46) both5145/85.72%strict, discarded. These repeat existing return source families, not counted again as new strategies. Functionclosingbrace depth0(p48)5504/worse;depth1(p49)RAW-neutral5173, both discarded. Existing guarded post-cellar0 is the only new retained inline control. No fake helper, extra local padding, new optimizer-scope device or compiler patch accepted.

Selected checkpointa20 only refreshes comments fromp47 and is whole-leaf RAW-identical. Full actualMain body including portable pragmas copied exactly into24-scenario fixture; freshly passes VC71/Od/W3/GR- withoutEHsc and64bitg++03. Header tree identical to a18, whose all3default checks pass; no header changes. Main remains DIFFER, no operand/branch-match claim. Both original higher-size5221 candidate and structurally matching-slotT75 alternative remain in histories.

Source47 adds an explicit bare return on the normal final fallthrough;48 places it in the final cancellation ELSE. These exercise duplicate-return AST formation without extra helpers/locals or changed runtime calls; both entire-leaf RAW-identical to a20 (5173/4loosehunks). One new return-path family counted (40distinct strategies), ELSE refinement not counted separately. Selected source/fixture unchanged.

## Final conservative completion audit

The selected source and every private .h file are exactly restored to `sol_checkpoint_a20_post_cellar_cleanup`. ATTEMPT_AUDIT.json/MD records 19 genuinely distinct compiled source families; it supersedes earlier informal cumulative totals and excludes flag-only/inline-only/name/default/model/regrade/build-fix/refinement counts. Final grounded callee-closure probes did not improve the selected residue. All individual history RESULT.json/candidate.cpp/header snapshots are retained. TEST_BODY.json verifies the complete selected Main compound body verbatim in the passing 24-scenario fixture (`runtime/sol_tests/00e93eb0/main_run_007`); Ransommer uses the corrected genuine by-value GetScriptThing stub where applicable. PARK_READY.md and COMPLETION_AUDIT.json record the exact hashes, residual and meaningful historical alternate. Status is park-ready DIFFER, never MATCH.

## Final source-closure history ledger

| History | Result |
|---|---|
| `sol_source_s40_original_inclass_thing_dtor` | DIFFER; 5173/5321; 62 strict hunks |
| `sol_source_s41_explicit_release_pointer_condition` | DIFFER; 5373/5321; 14 strict hunks |
| `sol_source_s42_release_early_null_return` | DIFFER; 4893/5321; 125 strict hunks |
| `sol_source_s43_guarded_cellar_wait` | DIFFER; 5277/5321; 53 strict hunks |
| `sol_source_s44_genuine_vector_dtor_explicit_instantiation_T75` | DIFFER; 5373/5321; 14 strict hunks |
| `sol_source_s45_original_final_return_post_cellar_control` | DIFFER; 5145/5321; 88 strict hunks |
| `sol_source_s46_original_final_call_then_return` | DIFFER; 5145/5321; 88 strict hunks |
| `sol_source_s47_explicit_normal_return_path` | DIFFER; 5173/5321; 59 strict hunks |
| `sol_source_s48_explicit_final_else_return` | DIFFER; 5173/5321; 59 strict hunks |

These entries are preserved diagnostics, not additional automatic strategy counts. ATTEMPT_AUDIT.json groups refinements and records the conservative qualifying subset. The selected checkpoint and private header set remain unchanged.
