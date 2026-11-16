# Pass23 Shove HaPenny attempts

Current: NOT MATCHED; selected main34 uses the original generic GetSubDef template (same root bytes as main23). Self-contained1920-case normal-object behaviour fixture passes native VC7.1 /Od /W3 noEHsc and GCC64 C++03.46root+8existinghelper operands audited; both182-byte existing helpers match, zero new credit. Current evidence below supersedes older chronological test-pending notes. Closest:1016/1016 bytes,276/276 instructions, frame28 versus retail2c. All original supplied histories and six neighbours were read. The supplied baseline was also compiled unchanged (late diagnostic, after rebase): its shortened Leaf is invalid and it emits nonvirtual QAEXXZ, not original virtual UAEXXZ. It is not an acceptable candidate.

## Identity and source recovery
Both PDB type records and native vtable0126F2F4 slot9 prove the root. FableWin02739BB0/3137B proves C2DVector::GetMagnitude/GetAngleXY, original GetGameState, both real CDefPointer specialisations, CTCTavernGame PlaySoundW/GetText, and the complete coin-base hierarchy. GetAngleXY calls GFArcTan(double,double) then GET_ROUNDED_ANGLE(float); the latter uses the float fmod overload. Original PDB local records place pdef at function scope, old_score/physics_frames/time_delta in the watch branch, n in its loop, base_def in the completed-game branch. Those original forms are restored in selected main23. Source no longer contains the supplied fake classes, guessed member names or new packing scopes.

## Build ledger (not all builds are distinct strategies)
01 shared rebase initially missed complete CThingObject base graph (compile failure).
02 added CThingObject header, still needed actual CThingGameObject/Physical profiles (compile failure).
03 complete genuine graph:983/1016. Rounding helper remains called.
04 both genuine GetSubDef bodies: explicit-specialisation order rejected.
05 moved both specialisations before first implicit use:961/1016; correct definition-release shape.
06/07/08/09 inline-budget diagnostics116/512/160/174.116..174 moves the unwanted call into GetAngleXY;512 expands both definition lookups incorrectly. These are ONE budget investigation.
10 scoped inline_depth16 does not fix the split; removed.
11 force only original GetAngleXY/rounding helpers after the ladder:1016/1016. No bare compiler-specific inline spelling in authored source.
12 original donor loop time_delta/physics_frames division at each iteration: unchanged.
13 original float fmod overload: fixes the swapped X-product operands, remaining differences stack-related.
14 donor free-helper calling convention /Gr: unchanged root.
15 donor case declaration order: unchanged.
16 genuine lookup bodies with auto_inline(off) plus natural angle budget512: same1016 result, removed as more intrusive.
17 named angle before constructing vector: unchanged.
18 explicit const-reference lifetime for aim vector: unchanged.
19 double-expression literal diagnostic: worse; rejected. Donor0.05 constant is promoted float0.05000000074505806, not exact double0.05.
20 angle-condition double promotions: worse; rejected.
21 named reference to definition: worse; rejected.
22 original C3DVector constructor body profile: unchanged; retained for original direct X/Y/Z body.
23 original function-scope pdef with early inactive return: unchanged; retained for PDB lexical scope.
24 genuine same-TU CCharString constructor/destructor bodies: unchanged; removed.
25 /GS frame diagnostic: unchanged; removed.
26 separate lexical scopes for the two C2DVector resets: unchanged; removed.

26 numbered production/diagnostic builds plus one unchanged-supplied baseline. Conservative12 distinct source-shape families; failed compilations and numeric budget variants do not inflate this count. No behaviour-test passes or relocation-audit passes are claimed yet.

## Remaining residue
Native frame2c versus28. Native aim vector starts at physical esp10, rounding spill20; ours vector14 and spill14. Native guide vectors24/30 versus20/2c; native old_score reuses10, ours14. CCharString temporaries1c/20 versus10/14. Reset zero stores are identical modulo these stack slots. No missing control-flow branch or extra native call remains. Follow original callee lifetime information and genuine aggregate scopes; never insert padding or artificial effects.


### main_27_component_constructor_body
Distinct hypothesis: real C2DVector component constructor body stores, rather than member-initializer syntax, alter the assignment temporary slot ordering. Original constructor takes x/y; no new model or layout. Result follows in history.

main_27: constructor body spelling inert (1016B, same28hunks); restored selected header.

### main_28_definition_precedes_positive_play_guard
PDB proves pdef function scope but does not constrain declaration relative to guard. Declare the genuine null-initialized handle before positive IsBeingPlayed block, letting dead null cleanup fold at inactive exit. Tests original lexical scope without extra references.

main_28:1016B but30hunks, worse; restored main23. main_29 only adds proven CoinBase CThing& constructor declaration for normal-object testing; production unchanged. Native /Od /W3 noEHsc and GCC64 C++03 self-contained1920-case fixture both PASS.

### main_30_complete_win_loss_sound_calls
Replace string-literal ternary with complete calls in each arm. Tests original CCharString temporary scopes and VC merge behaviour, a measured house-style lever; same behaviour.

main_30:1027B/279instructions, worse. Restored baseline.

### main_31_original_magnitude_squared_local
Use existing C2DVector original52-byte donor GetMagnitude profile with named squared local before sqrtf. Tests original helper temporary lifetime/reference ranking without invented references or new types.

main_31: original magnitude named-local profile inert; restored baseline.

### main_32_original_generic_subdefinition_template
The existing explicit-specialization helper itself byte-matches the full182B native body. Test original generic template definition instead of explicit specializations, preserving identical helper semantics, to expose genuine template call-graph/lifetime analysis.

main_32: generic original template is byte-inert, both182-byte GetSubDef helpers exact including8operands; prefer genuine generic template over two explicit specializations. Full46root operand audit PASS despite frame mismatch.

### main_33_per_case_lexical_blocks
Explicit sibling scopes per switch arm restrict address-taken C3DVector temporary lifetimes relative to angle and later event/string temporaries. No named locals, altered values or extra accesses.

main_33 per-case lexical blocks: byte-inert,1016B/276instructions; reverted to cleaner main32. Current33production/diagnostic builds plus supplied baseline;15conservative shape families. Test compile/debug work is not credited as source attempts. No byte match claimed.

### main_34_original_parent_accessor_chain
Donor calls02739C19 and later original GetParentThing() before PeekBaseDef/GetSubDef. Restore the exact real inline receiver chain at both sites instead of directly spelling ParentThing->; possible call/alias lifetime analysis difference, no extra effects.

main_34: original accessor chain is byte-inert; retained because it is directly evidenced by donor calls. Native/GCC1920cases and46root+8helper audit PASS on this exact source;34production/diagnostic builds, conservative15families (receiver spelling grouped with original API recovery). No match credit.

## Sol preserved-context continuation

All main34 selected source/test/header files remain unchanged. Fourteen new compiler invocations are retained: nine source/control probes, one current-root baseline, three final context regrades/corrections, and one actual RTM SDK diagnostic. Baseline/header verifications and equivalent spellings do not become source families. Four conservative new families cover first-component grouping, explicit genuine vector-copy lifetime, real definition-pointer callee graph, and the complete original string-cleanup graph. Prior34 production/diagnostic builds plus14 gives48, with the earlier supplied-baseline build preserved separately. Families15→19.

| Grade | Bytes | First difference |
|---|---:|---:|
| sol_retry_shove_baseline_01 | 1016 | 2 |
| sol_retry_shove_context_final_01 | 1016 | 2 |
| sol_retry_shove_context_final_02 | 1016 | 2 |
| sol_retry_shove_context_final_03 | 1016 | 2 |
| sol_retry_shove_explicit_aim_copy_01 | 1016 | 2 |
| sol_retry_shove_full_cleanup_called_01 | 1016 | 2 |
| sol_retry_shove_full_cleanup_native_calls_01 | 1016 | 2 |
| sol_retry_shove_full_cleanup_plain_01 | 1088 | 2 |
| sol_retry_shove_original_dynamiccast_01 | 1016 | 2 |
| sol_retry_shove_original_pointee_release_01 | 1016 | 2 |
| sol_retry_shove_original_release_chain_01 | 1016 | 2 |
| sol_retry_shove_paren_aim_first_component_01 | 1016 | 2 |
| sol_retry_shove_paren_all_first_components_01 | 1016 | 2 |
| sol_retry_shove_rtm_sdk_context_01 | 1016 | 2 |

Original GetAngleXY donor48B is a direct GFArcTan→GET_ROUNDED_ANGLE chain, with onlythis in its PDB local list. The actual helper bodies and both PDB local/type records were read. First-component parentheses, explicit aim copy and original definition-pointer cast/release bodies do not improve the root. All-component grouping adds a scheduling difference.

The complete original CCharString cleanup chain adds the genuine CBasicString::DeallocateData/destructor and CCharStringData::DecRef/GetRefs/destructor under new FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23. Original PDBs independently provide exact methods/access/types. Debug bodies and full native0099E9B0 corroborate semantics. Plain whole-string context expands calls (1088B); retaining the existing main24 policy only around CCharString ctor/dtor gives their exact61B/12B bodies and naturally expanded UnassignString87B. Existing two182B GetSubDef bodies also remain exact: five524B context bodies, zero new reserved credit.

Important failed binding retained: the first DeallocateData reconstruction usedfree. Native import analysis provedBFEB1C is array operator delete, IAT1440120; actualfree usesIAT1440154. Corrected to delete[] and fixture newchar[]. The legacy grade fallbackCRT <new> then still selected scalar delete because it omits the array declarations. A private genuineRTMSDK grade verifies the correct import. Final shared-header opt-in supplies the exact RTMSDK array-delete declaration (ownerF10), producing correct array??_V and scalar??3 bindings under the unchanged main grader. Failed/superseded grades and audit failures are retained; their masked-only helper matches are not accepted.

Final sol_retry_shove_context_final_03 passes61operands and10actual virtual sites, all five524B context bodies exact. Root remains1016B/276instructions with unchanged28-vs2c frame and slots; no root match. Native sol_native_retry_shove_context_05 uses realRTM13.10.3077 /Od /W3 WITHOUT/EHsc and passes1920 root cases,12 explicit sharing groups and a null-constructor check, zero warnings. GCC64 C++03 passes the same and the allocator oracle, with23 transparent pre-existing header warnings. The fixture observes actual array deletion before scalar deletion, exactly once per allocated data object; shared copies retain valid data until final release.

Four owned old/new text patch pairs reconstruct exactly, with default/old-selected-production/old-selected-fixture preprocessed token equality. No new packing or default-path change. Original root metadata/selection remains authoritative; this is an improved context candidate, not a replacement accepted root.

### Further targeted probes after frozen context
Five additional builds are preserved separately in RETRY_SOL_TESTPORTS/POST_CONTEXT_PROBES.json. Moving the exact genuine C3DVector component ctor definition out of class before/after the root is byte-inert. Constructing the aim vector before the angle (unlike prior main18, which declared the angle first), as a value, const reference or default+Initialise, remains1016/frame28 and worsens to31strict hunks. These are two new conservatively grouped families, not five. Current53production/diagnostic builds plus1preserved supplied baseline,21families. The corrected frozen context source/test/header/audit hashes remain untouched; no new behavior tests are claimed on rejected shapes.
Both original specialized CDefPointer complete records were freshly queried: FableWin4d9c7/Ego_r45df8, properties0x226, size4, private CDefPointeeBase* Object@0. No packedbit and no alignment change justified or made. Constructor-placement header is isolated under CTOR_PLACEMENT_HDR and is not part of the frozen four-header handback.


## Second Sol frame-led continuation

Twenty-four normal RTM production/control grades (all retained under sol_retry_shove_frame_*) bring the graded research count53→77, with the old supplied-baseline build separately preserved. One byte-identical frame-proxy compile and five preprocessor-only invocations (filename/raw-output readerpreparation failures retained; finallexicaltokenpairPASS) are separate, not source attempts. Seven conservative grouped families bring21→28: genuine wide cleanup, deferred real3DInitialise, real2Dreset construction/accessors, scalar reference lifetimes, explicit wide returned-value construction, real null-handle ctor body spelling and nontrivial sound-object lifetimes. All source/control receipts and exclusions are in RETRY_SOL_TESTPORTS/POST_FRAME_TRACE/RESULT.json.

No improvement. Frame diagnostic13nodes packs4+12+12+12=40B, showing how two2Dreset temps and aim join the scalar/string slot. Calling Initialise directly on the fields gives44B but shrinks963B by eliminating native-required stores; chainedreset985B, bothrejected. Named/default-initialized/reference/copy/reset-accessor forms retain1016/frame28. Deferred aim Initialises worsen31hunks; angle-sum-reference1025B and explicitwidecopies1054B also worse. Other genuine lifetime/constructor controls remain1016/frame28.

Fresh both-PDB C2DVector records and debug reset calls confirm existing packed8B model, implicit same-type assignment and two-float constructors. No alternate packing/base/type or artificialpadding/effect introduced. Actual68B CWideString dtor and3complete bindings independently match in unselected context (WIDE_DTOR_AUDIT.json); no standalone/native fixture claim or reserved credit. New private CDefPointer constructor opt-in is a discarded trial, default-profile tokens identical, fixedassert180preserved, not an integration request. All original main34 and frozenCONTEXT source/test/result/audit/four-header hashes verified unchanged. Existing1920-case native/GCC and61operand10virtual receipts remain current for the frozenbest; no reruns or passes fabricated for discarded shapes.
