# Pass21 probes

| Attempt | Result |
|---|---|
| sol_b00_current_best | DIFFER 5255/5373; strict 0.753146 |
| sol_s01_original_talked_flag_loop | DIFFER 5304/5373; strict 0.911604 |
| sol_f01_original_loop_S15 | DIFFER 5304/5373; strict 0.911604 |
| sol_f02_actual_S15 | DIFFER 5331/5373; strict 0.877845 |
| sol_s02_donor_absent_pass_wave | DIFFER 5304/5373; strict 0.912247 |
| sol_s03_final_idle_called_cleanup | DIFFER 5398/5373; strict 0.873168 |
| sol_s04_donor_dead_final_check | DIFFER 5304/5373; strict 0.912247 |
| sol_s05_original_timer_comparison | DIFFER 5304/5373; strict 0.912247 |
| sol_s06_generated_true_loop_guards | DIFFER 5304/5373; strict 0.912247 |
| sol_s07_visible_called_movie_pointer_dtor | DIFFER 5304/5373; strict 0.912247 |
| sol_s08_ordinary_timer_lifetime_S15 | DIFFER 5331/5373; strict 0.878487 |
| sol_s09_visible_called_timer_lifetime | DIFFER 5287/5373; strict 0.764953 |
| sol_s10_called_timer_dtor_movie_budget | DIFFER 5293/5373; strict 0.992573 |
| sol_s11_genuine_timer_index_accessor | DIFFER 5331/5373; strict 0.878487 |
| sol_s12_only_final_brace_called_cleanup | DIFFER 5276/5373; strict 0.857512 |
| sol_s13_post_boolean_loop_called_cleanup | DIFFER 5356/5373; strict 0.869926 |

13 distinct source strategies (s01..s13), excluding budget flags/regrades. All histories retained. Initial f01 accidentally used an unsupported flags file and is explicitly a duplicate noflags grade; f02 is the actual S15 budget check. No flags file remains.

New source evidence: original TalkedTo=false before HasPass=true/PsstAmount=false/NotAttacked=true; donor036A6955 tests !TalkedTo&&NotAttacked, and036A81DE setsTalkedTo=true after the first conversation before scope cleanup. The old draft substituted whileNotAttacked+break and lost the native loop/static frequencies. Restoring the original variable/loop improves strict75.3% to91.16%.

A real source error was found and fixed: wave condition must use !IsObjectInThingsPossession(pass,hero). NativeD0C75D/764 branches on false possession to the true condition; donor036A6CA3/CA8 confirms. The old source tested positive possession, whereas the old test model already expected absence.

Original CTimer::operator<= calls GetTime (donor036A9FC0), restored as a genuine inline body. PDB_LOCALS records original names. Genuine EHeroAbility header replaces copied enum. Visible ordinary Movie PImp destructor body is the genuine CCountedPointer Data-first destructor; it retains native calls under guarded auto_inline(off) and is byte-neutral vs declaration-only.

Best s10: S15 plus ordinary CTimer destructor under guarded auto_inline(off), ctorinline. Native frame0xa4 and every body/register choice now agree. Only two normalized hunks remain: nativeD0D3A1..D3EE hot pause/movie/timer/STB cleanup pad missing (78bytes), and2-byte JMP before an aligned loop. Other timer cleanups correctly call the common native destructor.

Failed followups: index accessor factoring does not change the S15 expanded-timer result; scoped final-brace-only0 and scoped both post-boolean-loop cancellation returns0 on ordinary-inlineTimer+S15 lose native frame/cleanup layout. Scope0 around final idle return plusbrace also fails. s06 generated&&true guards are neutral. Work remainsIN_PROGRESS, not parked.


Whole unchanged Main fixture now PASS both required compilers,27 scenarios (runtime/sol_tests/00d0c410/main_run_002). Covers early success/insufficientgold/refusal,late success/failure/repeateddecline,topupbetweenoffers,gate/pass earlyandlate branches,dynamic pass-removal andgateopen,threealternatingwaves andnegativepossessionguard,normal/special/heal/enemy triggers,simultaneoushit+talkactualcontinuedbodyflow,acquisition/predicate/question/successspeech/wave interruptions,zerohealth andpersistedGivenPass. Checksexactpurchaseeventorder, currency transfer, inventory, experience, resourcecleanup andpost-saleidle. First fixture accidentally reapplied pass-removal eachframe; one-shot eventstimulus corrected, source unchanged. First sourcecommentcorrected toactualnegativepasscondition andofferendsafterasale; regrade5293/5373 strict99.257 unchanged. Source remains byte-unmatched IN_PROGRESS, not a byte-match claim.

Measured inline checkpoint: Measured Timer14/Movie19 and S15 floor20: both unconditionally inline. Late genuine GetIndex and visible actual BaseObject destructor leave cost unchanged. Forced genuine outer Movie destructor adds five nonnative vptr resets. Call-site auto_inline rejected by compiler, no such device retained. All additional histories and diagnostic leaf-identical validations retained. 15 genuine source strategies, excluding flags/devices/build failures.

Measured owner-inline probe p18: forcing the genuine GROBase destructor while leaving Movie implicit is byte-neutral5304 to the ordinary Timer baseline. Fresh leaf-identical trace confirms Movie cost19 / Timer14 unchanged, so the forced intermediate destructor does not reduce Movie cost. Private probe header removed from active overlay; retained in history. Device is not counted toward source strategies.

Genuine handle-access rebase: both original PDBs prove private CScriptThing::PImp and private CTimer::TimerIndex (where present). Private row Thing overlay plus existing Timer opt-in enables exact access, no input/header-cache mutation. Full records PDB_HANDLE_TYPES.json. sol_model_a01_original_handle_access checks bytes against selected version; model correction not counted as source strategy. Main body/fixture unchanged.

Private CBaseObject::IntelligentPointer opt-in restored from BOTH original full class records (runtime/sol_base_object_type_evidence.json). Row-only full header overlay, no shared/input mutation. sol_model_a03_original_base_access is RAW-leaf byte-identical to selected previous version, recorded BASE_ACCESS_AUDIT.json. Actual Main/fixture unchanged. Model correction not counted as source strategy.

Original donor source-chain restoration: MovieBase02468220 and Thing018F0A90 counted-pointer destructors only call ReleaseData; donor02468530 release clears Data thenInfo; ShareData027C8B40 calls ReleaseData and increments stored Info. Actual disassemblies runtime/sol_donor_countedptr_*.disasm. Private complete header corrects the earlier retail-inferred source claim, without new helpers or padding. sol_model_a04_donor_counted_pointer_source is RAW-leaf byte-identical to selected prior version, COUNTED_POINTER_SOURCE_AUDIT.json. Actual Main/fixture unchanged. Direct retail/donor version-source uncertainty retained in private header; no shared mutation.

Both original resource classes have private PImp, confirmed BOTH full PDB records runtime/sol_resource_counted_pointer_pdb_types.json. Row-only full headers restore private access; Movie ctor inline controls use FABLE_FORCEINLINE alias. sol_model_a08_original_resource_access is RAW-entire-leaf identical, RESOURCE_ACCESS_AUDIT.json. Main/fixture unchanged, no input/shared mutation. Access/model regrade not counted as source strategy.

Fresh inline controls p20 explicit-inline Timer under auto_inline(off) are byte-neutral5293, so explicit hint does not bypass the callee disable. p21 declaration-site depth0 on ordinary Timer outlines the native-expanded constructor and worsens5363/frame; discarded. Neither counts as source strategy. Selected a08 source restored; the measured cost/frequency conflict remains19Movie versus14Timer with equal per-exit frequencies andS15floor20.

Compatibility-only modela10 restores inactive legacy counted-pointer dtor and gates original ShareData stored-Info reload under existing RELEASEDATA. Normal RTM entire compiled leaf is RAW-identical to selected prior history, and every private header default token stream matches baseline with helper/access opt-ins off. Source/test body unchanged; no source-strategy count or new semantic-test claim. SOL_DEFAULT_COMPATIBILITY_AUDIT.json retains checks.

Fresh call-site controls p22 outline17 outer-Timer returns plus finalbrace while leaving Movie scopes eligible, worsening5287/frame; p23 isolates only the return statement after the empty Return() call and is identical5287; p24 removes finalbrace control, worsening5358. No control retained and no source-strategy count. Exact native scan corrects earlier generalized wording: Forger has ONE hot inlined DeregisterTimer site D0D3D5 and one called commonTimer pad D0D8CC; Gate2 has two, not this row. Both complete CTimer types have no base and sole private long TimerIndex, size4. Original donor destructor0360A160 reads TimerIndex directly and calls interface slot160, with no GetIndex wrapper.

Inline p25 forced genuine Timer destructor while its definition remains under auto_inline(off) is RAW-neutral5293; auto_inline(off) blocks it even with the force hint. Force discarded, selected a10 source restored. This is an inline-device probe, not a source strategy.

Portable-source checkpoint a12 updates only stale source comments/line1, adds current Inline-budget/Retail notes, and replaces inherited bare calling conventions with existing FABLE_CDECL/FABLE_FASTCALL aliases where present. Entire normal RTM leaf is RAW-identical to the prior selected candidate; full Main body remains byte-for-byte text in the already passed fixture. No new semantic-test or source-strategy claim. SOL_PORTABILITY_CHECKPOINT.json retains hashes/validation.

Model a14 restores original private CIsThingAlive::Thing with a narrow opt-in. Both full PDB records prove access/type/offset; normal RTM entire leaf RAW-identical to sol_model_a12_portable_source_checkpoint. All 6 private header defaults tokenize identically to baseline. No source-strategy increment; existing actual-Main fixtures unchanged.

Model a16 restores BOTH original PDB declarations: protected Game reference at4 and protected Me at8, with narrow FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS. Constructor/member-access syntax follows original reference. Entire normal RTM leaf RAW-identical to sol_model_a14_original_predicate_access. All private header defaults tokenize identically to baseline. Actual full-Main fixture updated to reference member and rerun separately. Initial a15 missing-reference-argument compile fix retained, not counted source strategy.

Model a17 completes CPredicateBase with original public STLport unary_function<void,bool> base and protected static const Game pointer PGame. Both original full types agree; no body/control added. Narrow FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS guards inheritance, genuine shared STLport include and static declaration. Entire leaf RAW-identical to a16, default tokens identical. Full Main unchanged from the just-passed reference-member fixtures.

Source17 removes inherited forced explicit Movie constructor in favor of original implicit constructor, worsening5386/74strict hunks; source18 additionally restores plain Timer destructor and gives5315/134hunks. Both genuine visibility probes discarded, selected a17 restored.18 distinct source strategies retained.

Model a19 enables the original public reference Game/MasterScriptData members in the genuine owning parent CLevelScriptBase (both original full PDB records). Main uses its own CEntityScriptBase Game and is unchanged. Entire normal RTM leaf RAW-identical to sol_model_a17_original_predicate_base. All private header default tokens identical; existing freshly passed full-body fixture is unchanged and reused. No source-strategy or new test-run claim.

## Final conservative completion audit

The selected source and every private .h file are exactly restored to `sol_model_a19_original_parent_level_references`. ATTEMPT_AUDIT.json/MD records 10 genuinely distinct compiled source families; it supersedes earlier informal cumulative totals and excludes flag-only/inline-only/name/default/model/regrade/build-fix/refinement counts. Final grounded callee-closure probes did not improve the selected residue. All individual history RESULT.json/candidate.cpp/header snapshots are retained. TEST_BODY.json verifies the complete selected Main compound body verbatim in the passing 27-scenario fixture (`runtime/sol_tests/00d0c410/main_run_003`); Ransommer uses the corrected genuine by-value GetScriptThing stub where applicable. PARK_READY.md and COMPLETION_AUDIT.json record the exact hashes, residual and meaningful historical alternate. Status is park-ready DIFFER, never MATCH.

## Final source-closure history ledger

| History | Result |
|---|---|
| `sol_source_s17_original_implicit_movie_constructor` | DIFFER; 5386/5373; 74 strict hunks |
| `sol_source_s18_implicit_movie_plain_timer` | DIFFER; 5315/5373; 134 strict hunks |
| `sol_source_s20_genuine_called_get_script_thing` | DIFFER; 5293/5373; 2 strict hunks |

These entries are preserved diagnostics, not additional automatic strategy counts. ATTEMPT_AUDIT.json groups refinements and records the conservative qualifying subset. The selected checkpoint and private header set remain unchanged.
