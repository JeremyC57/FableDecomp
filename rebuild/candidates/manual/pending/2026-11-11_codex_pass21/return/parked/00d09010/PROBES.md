# Pass 21 probes

| Attempt | Strategy | Result |
|---|---|---|
| sol_b00_current_best | current_best | COMPILE_FAIL  v 5864; strict 0 |
| sol_b01_rebased_owner | rebased_owner | DIFFER 5880 v 5864; strict 0.94854 |


Checkpoint112: CampHostage5672/5864strict95.44 afteractualcalledTimerdtor+implicitMovieDtor/S15+forcedoriginalMoviector. MissingnativehotTimercleanupcopies andsomeCSE/slots. Donorconstantframefor/deadidleT guardsneutral. Braceonly/deadreturn+brace0worse; ordinaryTimerbodieswithoutoffsameallinline; explicitrescue-breakloopworse. Donorcounter++andTurningfalse-beforeMarkerAt assignmentsrestoredins10,byte-neutral; retainbaselinecorrectcase5literal6infuturemerge.

Complete current history (including duplicates/build repairs/flags, which do not count as source strategies):

| History | Bytes / strict |
|---|---|
| sol_b00_current_best | None/None 0 COMPILE_FAIL |
| sol_b01_rebased_owner | 5880/5864 0.948543 DIFFER |
| sol_s01_genuine_implicit_movie | 5880/5864 0.759852 DIFFER |
| sol_f01_movie_budget15 | 6016/5864 0.838887 DIFFER |
| sol_s02_implicit_ctor_pair | 5964/5864 0.834969 DIFFER |
| sol_f02_movie_budget14 | 5916/5864 0.738037 DIFFER |
| sol_s03_original_generated_loops | 5964/5864 0.834969 DIFFER |
| sol_s04_visible_called_timer_dtor | 5640/5864 0.851466 DIFFER |
| sol_s05_called_timer_force_original_movie_ctor | 5672/5864 0.954388 DIFFER |
| sol_s06_final_brace_called_timer_depth | 5980/5864 0.838868 DIFFER |
| sol_s07_dead_postloop_return_and_brace_depth | 5980/5864 0.838868 DIFFER |
| sol_s08_ordinary_timer_lifetimes_assignment_ctor | 6016/5864 0.838887 DIFFER |
| sol_s09_explicit_rescue_break_loop | 5736/5864 0.818779 DIFFER |
| sol_s10_donor_counter_increment_and_turn_order | 5672/5864 0.954388 DIFFER |

Full unchanged Main-body fixture: 27 scenarios PASS both actual VC71 /Od /W3 /GR- without /EHsc and 64-bit g++03. Resource, timer, movie and pause destruction checked. Log: runtime/sol_tests/00d09010/main_run_001.

Donor correction s10b retained: increment GoadLevel only original cases0..4; earlycase5 remains6, earlycase6 resets5. Turning=false precedes MarkerAt assignment at both turns. These source corrections preserve selected5672bytes/95.4388%strict. Regenerated unchanged-body fixture and reran all27 scenarios on both required compilers: runtime/sol_tests/00d09010/main_run_003 PASS.

Genuine handle-access rebase: both original PDBs prove private CScriptThing::PImp and private CTimer::TimerIndex (where present). Private row Thing overlay plus existing Timer opt-in enables exact access, no input/header-cache mutation. Full records PDB_HANDLE_TYPES.json. sol_model_a01_original_handle_access checks bytes against selected version; model correction not counted as source strategy. Main body/fixture unchanged.

Private CBaseObject::IntelligentPointer opt-in restored from BOTH original full class records (runtime/sol_base_object_type_evidence.json). Row-only full header overlay, no shared/input mutation. sol_model_a03_original_base_access is RAW-leaf byte-identical to selected previous version, recorded BASE_ACCESS_AUDIT.json. Actual Main/fixture unchanged. Model correction not counted as source strategy.

Original donor source-chain restoration: MovieBase02468220 and Thing018F0A90 counted-pointer destructors only call ReleaseData; donor02468530 release clears Data thenInfo; ShareData027C8B40 calls ReleaseData and increments stored Info. Actual disassemblies runtime/sol_donor_countedptr_*.disasm. Private complete header corrects the earlier retail-inferred source claim, without new helpers or padding. sol_model_a04_donor_counted_pointer_source is RAW-leaf byte-identical to selected prior version, COUNTED_POINTER_SOURCE_AUDIT.json. Actual Main/fixture unchanged. Direct retail/donor version-source uncertainty retained in private header; no shared mutation.

Both original resource classes have private PImp, confirmed BOTH full PDB records runtime/sol_resource_counted_pointer_pdb_types.json. Row-only full headers restore private access; Movie ctor inline controls use FABLE_FORCEINLINE alias. sol_model_a08_original_resource_access is RAW-entire-leaf identical, RESOURCE_ACCESS_AUDIT.json. Main/fixture unchanged, no input/shared mutation. Access/model regrade not counted as source strategy.

Compatibility-only modela10 restores inactive legacy counted-pointer dtor and gates original ShareData stored-Info reload under existing RELEASEDATA. Normal RTM entire compiled leaf is RAW-identical to selected prior history, and every private header default token stream matches baseline with helper/access opt-ins off. Source/test body unchanged; no source-strategy count or new semantic-test claim. SOL_DEFAULT_COMPATIBILITY_AUDIT.json retains checks.

Portable-source checkpoint a12 updates only stale source comments/line1, adds current Inline-budget/Retail notes, and replaces inherited bare calling conventions with existing FABLE_CDECL/FABLE_FASTCALL aliases where present. Entire normal RTM leaf is RAW-identical to the prior selected candidate; full Main body remains byte-for-byte text in the already passed fixture. No new semantic-test or source-strategy claim. SOL_PORTABILITY_CHECKPOINT.json retains hashes/validation.

Model a14 restores original private CIsThingAlive::Thing with a narrow opt-in. Both full PDB records prove access/type/offset; normal RTM entire leaf RAW-identical to sol_model_a12_portable_source_checkpoint. All 7 private header defaults tokenize identically to baseline. No source-strategy increment; existing actual-Main fixtures unchanged.

Model a16 restores BOTH original PDB declarations: protected Game reference at4 and protected Me at8, with narrow FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS. Constructor/member-access syntax follows original reference. Entire normal RTM leaf RAW-identical to sol_model_a14_original_predicate_access. All private header defaults tokenize identically to baseline. Actual full-Main fixture updated to reference member and rerun separately. Initial a15 missing-reference-argument compile fix retained, not counted source strategy.

Model a17 completes CPredicateBase with original public STLport unary_function<void,bool> base and protected static const Game pointer PGame. Both original full types agree; no body/control added. Narrow FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS guards inheritance, genuine shared STLport include and static declaration. Entire leaf RAW-identical to a16, default tokens identical. Full Main unchanged from the just-passed reference-member fixtures.

Model a19 enables the original public reference Game/MasterScriptData members in the genuine owning parent CLevelScriptBase (both original full PDB records). Main uses its own CEntityScriptBase Game and is unchanged. Entire normal RTM leaf RAW-identical to sol_model_a17_original_predicate_base. All private header default tokens identical; existing freshly passed full-body fixture is unchanged and reused. No source-strategy or new test-run claim.

Source12 restores genuine visible ordinarily called GetScriptThing with original null-PImp default Thing return and otherwise virtualslot12 forwarding (native007E7490, donor027A8850/027A89A9). Original default Thing ctor also visible. Helper exactly57native bytes after2original rootctor/vtable relocations; SOL_GET_SCRIPT_THING_HELPER_AUDIT.json retains proof. Main entire leaf RAW-neutral5672, so source restored minimally;12distinct strategies. No missing getter body/capture effect found.

## Final conservative completion audit

The selected source and every private .h file are exactly restored to `sol_model_a19_original_parent_level_references`. ATTEMPT_AUDIT.json/MD records 10 genuinely distinct compiled source families; it supersedes earlier informal cumulative totals and excludes flag-only/inline-only/name/default/model/regrade/build-fix/refinement counts. Final grounded callee-closure probes did not improve the selected residue. All individual history RESULT.json/candidate.cpp/header snapshots are retained. TEST_BODY.json verifies the complete selected Main compound body verbatim in the passing 27-scenario fixture (`runtime/sol_tests/00d09010/main_run_004`); Ransommer uses the corrected genuine by-value GetScriptThing stub where applicable. PARK_READY.md and COMPLETION_AUDIT.json record the exact hashes, residual and meaningful historical alternate. Status is park-ready DIFFER, never MATCH.

## Final source-closure history ledger

| History | Result |
|---|---|
| `sol_source_s12_genuine_called_get_script_thing` | DIFFER; 5672/5864; 16 strict hunks |
| `sol_source_s13_genuine_called_task_query` | DIFFER; 5672/5864; 16 strict hunks |
| `sol_source_s14_genuine_called_validity_query` | DIFFER; 5656/5864; 21 strict hunks |
| `sol_source_s15_complete_called_termination_chain` | DIFFER; 5672/5864; 16 strict hunks |
| `sol_source_s16_genuine_called_movie_pointer_dtor` | DIFFER; 5672/5864; 16 strict hunks |
| `sol_source_s17_original_called_base_object_ctor` | DIFFER; 5672/5864; 18 strict hunks |

These entries are preserved diagnostics, not additional automatic strategy counts. ATTEMPT_AUDIT.json groups refinements and records the conservative qualifying subset. The selected checkpoint and private header set remain unchanged.
