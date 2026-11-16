#00590235 probes

All source attempts preserved. RTM3077 real grades; microprobes and metadata/profile regrades are not separate source families.

|Tag|Grade|Bytes|Strict|Loose|
|---|---|---|---|---|
|baseline_package|LEAF_REVIEW_REQUIRED|-|-|-|
|sol00_standin_actual_leaf_package_baseline|DIFFER|1393|0.7787234042553192|1.0|
|sol01_real_shared_inventory_rebase|COMPILE_FAIL|-|-|-|
|sol01b_real_shared_inventory_rebase|DIFFER|1393|0.7787234042553192|1.0|
|sol02_original_pointee_release_chain|DIFFER|1393|0.7787234042553192|1.0|
|sol03_original_local_names|DIFFER|1393|0.7787234042553192|1.0|
|sol04_visible_genuine_pair_pushback|DIFFER|1393|0.7787234042553192|1.0|
|sol05_visible_genuine_pair_erase|DIFFER|1393|0.7787234042553192|1.0|
|sol06_original_allitems_category_chain|DIFFER|1393|0.7787234042553192|1.0|
|sol07_erase_while_region|DIFFER|1393|0.7787234042553192|1.0|
|sol08_classification_while_region|DIFFER|1386|0.652452025586354|0.8891257995735607|
|sol09_both_while_regions|DIFFER|1386|0.652452025586354|0.8891257995735607|
|sol10_named_inventory_reference|DIFFER|1393|0.7787234042553192|1.0|
|sol11_selected_category_pointer_region|DIFFER|1392|0.8349307774227902|0.9946751863684771|
|sol12_ui_child_lifetime_regions|DIFFER|1393|0.6829787234042554|0.9702127659574468|
|sol13_named_item_reference_region|DIFFER|1375|0.8247863247863247|0.9700854700854701|
|sol14_caption_lifetime_regions|DIFFER|1332|0.510314875135722|0.7361563517915309|
|sol15_deferred_category_vector_lifetimes|DIFFER|1393|0.7787234042553192|0.9680851063829787|
|sol16_selected_category_pair_reference|DIFFER|1392|0.8349307774227902|0.9946751863684771|
|sol17_selected_shared18_parked_metadata|DIFFER|1393|0.7787234042553192|1.0|
|sol18_visible_original_counted_ctor|DIFFER|1393|0.7787234042553192|1.0|
|sol19_original_component_cast_chain|DIFFER|1393|0.7787234042553192|1.0|
|sol20_final_original_cast_parked|DIFFER|1393|0.7787234042553192|1.0|
|sol21_real_counted_ctor_before_target|DIFFER|1393|0.7787234042553192|1.0|
|sol22_real_counted_ctor_after_target|DIFFER|1393|0.7787234042553192|1.0|
|sol23_real_allitems_after_target|DIFFER|1393|0.7787234042553192|1.0|
|sol24_shared19_final_parked|DIFFER|1393|0.7787234042553192|1.0|

Distinct source families: true shared class/STLport rebase; original pointee Release chain; genuine pair push_back visibility; genuine pair erase visibility; original GetAllItems/CCategory chain; loop-region rewrites (07/08/09 one family); named inventory reference; selected-category destination region (11/16 one family); separate child lifetimes; named loop item reference; separate caption lifetimes; deferred category-vector lifetimes; genuine counted constructor visibility/definition order (18/21/22 one family); original component/cast chain. Original-local respelling alone is not counted. GetAllItems before/after definition order (06/23) is one family. This exceeds ten genuinely distinct source families.

Selected20/24 retains1393 bytes and all470 native instruction shapes. Actual registers cycle: native this/iterator=edi, zero=esi, index=ebx; ours this/iterator=esi, zero=ebx, index=edi. Frame-slot references follow that allocation. Selected-category-pointer1392 has a higher raw score but loses the native dead decrement and changes push/pop timing, so it is not selected.

Compiler trace tooling was recovered locally (gdb16.3 and stock c2probe). Two bounded trace attempts could not attach WineDbg to the temporary compiler (error5; gdb remote timed out). Logs are preserved; no trace or allocation inequality is claimed. No toolchain was modified, and no shared Wine process was killed.

Original ctor and GetAllItems bodies before/after target are inert for the target. Further work should obtain a functioning compiler allocation trace and measure zero versus this/index priorities. No match is claimed; behavior/bindings/helper closure pass.

## Continue A: original scopes and working allocator diagnostics

All25 new RTM grades are separate from7 read-only diagnostic target compiles and3 diagnostic tool-DLL builds. Inherited27 target invocations, including package wrong-leaf compilation and one failed C++ compile, retained. Thus52 recorded target-grade invocations plus7 new diagnostic target invocations; two inherited failed debugger-attach traces remain separate. Four budget flags below are one bounded diagnostic family, not four source families.

|Tag|Bytes|Strict hunks|Loose hunks|Result|
|---|---|---|---|---|
|continue_a25_preserved_control|1393|79|0|DIFFER|
|continue_a26_shared36|1393|79|0|DIFFER|
|continue_a27_original_category_enum|1393|79|0|DIFFER|
|continue_a28_original_outer_expression_scope|1393|79|0|DIFFER|
|continue_a29_original_selfdeleting_release_scope|1393|79|0|DIFFER|
|continue_a30_original_vector_index_access|1393|79|0|DIFFER|
|continue_a31_original_release_decrement_and_else|1393|79|0|DIFFER|
|continue_a32_tu_fastcall_diagnostic|1393|79|0|DIFFER|
|continue_a33_both_real_vector_helpers_fastcall|1393|79|0|DIFFER|
|continue_a34_actual_counted_constructor_member_test|1393|79|0|DIFFER|
|continue_a35_original_inclass_counted_ctor_modref|1393|79|0|DIFFER|
|continue_a36_complete_original_allitems_chain|1393|79|0|DIFFER|
|continue_a37_original_chain_native_frame_profiles|1393|79|0|DIFFER|
|continue_a38_original_pcomponent_return|1393|79|0|DIFFER|
|continue_a39_original_tree_clear_accessor_chain|1393|79|0|DIFFER|
|continue_a40_natural_original_recursive_tree_chain|1393|79|0|DIFFER|
|continue_a41_plain_original_defptr_inline_chain|1342|104|46|DIFFER|
|continue_a42_plain_inline_budget_100|1342|104|46|DIFFER|
|continue_a43_plain_inline_budget_200|1342|104|46|DIFFER|
|continue_a44_plain_inline_budget_400|1342|104|46|DIFFER|
|continue_a45_plain_inline_budget_800|1342|104|46|DIFFER|
|continue_a46_final_original_source_rebase37|1393|79|0|DIFFER|
|continue_a47_final_complete_helper_chain_rebase37|1393|79|0|DIFFER|
|continue_a48_final_original_source_rebase38|1393|79|0|DIFFER|
|continue_a49_final_complete_helper_chain_rebase38|1393|79|0|DIFFER|

Conservative distinct-family accounting: inherited14 retained; four conservatively new shape families are genuine outer expression scope, actual separate-decrement/if-else pointee scope, original tree-clear/accessor/recursive chain, and plain DefPointer inline chain. Total18. Enum recovery and vector-index countercheck do not inflate the source-family count. Full original category/CItem chain extends inherited GetAllItems family; corrected stored-member counted constructor extends inherited constructor family. Original pcomponent return and /Gr or rebase controls are not new families.

25/26 preserved and accepted-header controls;27 raw enum restoration;28 outer IsExpression scope;29 self-deleting Release visibility;30 direct vector index baseline control;31 actual separate decrement and explicit else;32 /Gr diagnostic;33 joint real push_back and erase;34 stored-member constructor correction;35 actual in-class constructor visibility;36 complete typed category→item chain;37 native helper FPO profile and /Gr;38 actual pcomponent return;39 original tree clear accessor chain;40 natural recursive original tree chain;41 plain DefPointer inline;42–45 bounded inline budget100/200/400/800 diagnostics (all1342/46 loose);46/47 frozen37 controls;48/49 frozen38 final controls. All others1393/79 strict/0 loose. Secondary chain37/47/49 independently exact33+39+36, while36 carries same source under target FPO setting and gives3-extra-byte helper frames.

New diagnostics: baseline26 static-priority,use,constant-values,blockV1(invalid source-map but valid byte identity),blockV2(valid common join);31 constant-values;46 final constant-values. All7 actual diagnostic target compiles preserve the plain RTM target bytes. Three tool-DLL build invocations are not target compiles. The final source selected48 has exactly46 source bytes and root object equivalence confirmed by frozen rebase.

The first refreshed fixture omitted inherited GCC compatibility headers; native build/run passed, GCC build failed. Logs and FAILURE_NOTE preserved in continue_a_run_001; bridge copied without changing production body, runs002/003 pass both compilers. Current proofs use actual shared self-deleting Release instead of the older externally duplicated decrement contract. No existing failed output overwritten.

## Continue B original metadata and helper provenance retry

Selected continue_b62_final_closest_rebase42 preserves the exact same source SHA as a48 and every emitted non-debug section/relocation identity (CONTINUE_B_FULL_OBJECT_EQUIVALENCE).1393/1393bytes,79strict/0loose, no match.14new real target compiler grades (including preserved missing-include failure52 and repair52b) plus52old=66target grades;7prior readonly diagnostic compiles separate,73combined;3prior proxy tool-DLL builds separate.2conservative new families for20total; metadata repairs do not each count as a strategy.

50rebase40control;51actual TMapOfDefIndexToCount;52/52bcomplete original CTCInventory class (repaired map include);53authentic vendor vector core aliases;54actual static Hair/Tattoo getter bodies;55real GetDef/GetSubDef chain;56direct CList cast control;57original vector::iterator (raw PDB records Dinkumware vector iterator, not raw pair pointer);58direct vector declarations removes descriptive CItemVector;59both-PDB element_type;60original private Object;61release-erased original Validate path plus missing real Swap/DeleteNull declarations;62rebase42checkpoint. All except54/55 compile byte-identically to closest;54/55root1377/98strict/50loose is rejected. Static getters are in original tc_inventory_item.obj while root is tc_inventory.obj: no claim their body visibility belongs in target. Both137-byte getter bodies and HairGetSubDef182match independently, but GetDef108/98 and TattooSubDef182/162 are explicit unresolved debts in rejected branch, never hidden or carried into selected root.

Rejected mutable header profiles restored from baseline; immutable snapshots retain every byte. Owned5header requests unchanged. Fresh188old/default/current accepted token/returncode comparisons PASS:184successful,4identical baseline errors listed in contract. Fresh109operand audit and44-byte ReleaseData helper PASS; root binding alignment is not a masked-match claim. Prior4096VC/GCCbody tests and29ABI checks are reused ONLY because selected source and all non-debug code/data/relocation inventories are proven identical; no new runtime test claim. Full original-type evidence for CCountedPointer element_type, publicData/Info and private CDefPointer::Object is in CONTINUE_B_POINTER_ORIGINAL_TYPES; actual Validate and card getter procedures retain raw records and full donor assembly. Continue retry queue after other rows.

## Continue C (same Sol worker B), original special members and unmodified vendor source

63 fresh accepted43 baseline with all prior5owned header additions preserves closest1393/470/79strict/0loose.64 corrects prior52b complete inventory class: BOTH raw PDBs mark copyctor and assignment compiler-generated0x103; actual CThing& ctor user0x3 and dtor user0x7. New optin omits only generated declarations, retains full actual class; same root.69 full real CCountedPointer23-record instantiated member set/public fields after methods, implicit generated pseudo destructor closure omitted, preserves actual called(T*)ctor and44byte ReleaseData source; same root.71 corresponding actual CDefPointer17-record instantiation includes missing Swap, privateObject, omits absent Assign helper and generated closures, keeps actual self-deleting Release body and inherited inline request; same root. Uninstantiated converting-template absence is NOT inferred from concrete PDB class records. Order from LF_METHOD is recorded grouping, not proof of original textual source order.70 uniformly direct-initializes the two actual user-copy def handles (no added locals/scopes), same root.

65..68 are isolated genuine unmodified STLport4.6.2 vector/list/map diagnostics.65 failed because MSVC quoted-include ancestor search loaded shared container header before overlay;66 fixed prefix but original stdstring/no-own-iostream integration failed;67 narrowed to actual vector/list/map, duplicate char uninitialized_copy prevented compile;68 removes only shared duplicate of identical existing vendor _uninitialized.h83..86 overload. Original vendor source unmodified; no shared toolchain/runtime/header changed.68 compiled1393bytes whose ROOT RAW bytes exactly equal ordinary63.108root relocation symbols also agree except anonymous TITLE_NAME namespace suffix tied toTU path. No helper closure/match claim for vendor diagnostic; no original-container source lever found at this root. Full logs and source under continue_c/vendor65..68.

Counts:66inherited ordinary+5new ordinary=71 ordinary;7inherited diagnostic+4vendor=11diagnostic;82actual target compiler invocations. Three vendor integration failures retained and counted; tool DLL builds/tests/layout separate.21conservative families (20inherited+full true vendor implementation family), no inflation for repairs or metadata refinements. Selected production source62 remains unchanged and its4096each VC/GCC fixtures/29ABI checks remain existing evidence only. No match/byte credit.

72 restores155real method signatures/access/virtual kind from both211-member inventory-base records, keeps generated special members implicit and existing304byte native data model. Actual original namespace enum names recovered (explicit correction mapping in continue_c/ORIGINAL_ENUM_NAME_CORRECTIONS.tsv), class keys allV.73 enables existing actual user CIV ctor;74 adds proven user public CDefPointeeBase/private CDefClassBase default ctor declarations (no fabricated bodies/friendship).75 one bounded header-vs-root optimization-boundary control: defaultspeed header parse then originalOs/Oy-minus root; nochange.76 adds actual user default ctor/nonvirtual dtor/Copy andthree original pure virtuals to secondary screen base; preparation misplaced a class brace and fails, retained.77 corrects that syntax and is identicalroot; noextra family.78 groups only existing actual scalar pmanager/pcomponent/plist declarations atentry, with original initialization execution sites preserved; no extra local/value/scope. Same root, no placement grid. All successful ordinary72..78 retain1393/470/79strict/0loose.

Substantive original source comparison: downloaded five public4.5.2 headers from same pinned Nival/Blitzkrieg commit authenticated byscoreboard; alloc SHA exactly equals prior bundle. Pair/default-vector ctor bodies are unchanged versus4.6.2, but allocator allocate/deallocate/construct/destroy have constthis in4.5.2 versusnonconst4.6.2, rebindfeaturegate differs, resize argument changes constref→value and destruction qualification differs.79 therefore compiles one exact older core set (_alloc,_construct,_pair,_vector.h/.c,_iterator_base,_uninitialized,type_traits) ahead of full4.6.2 support/config. This is explicitly an original-core comparison, NOT a complete4.5.2 installation claim.79 succeeds and root raw1393-byte SHA01a48cad... is IDENTICAL to full4.6.2 profile68 and ordinary63. Separate108relocations bound, anonymous TITLE_NAME suffix changes only byTU path. Actual old-core provenance/headerSHA androotidentity report preserved. No new vendor body hypothesis left from those differences.
Counts now78ordinary=66inherited+12new (includes76failure);12diagnostic=7inherited+5vendor (includes65..67failures);90actual target compiles.21conservative families unchanged: original metadata/boundary refinements not inflated. Current selected production62/source and its existing4096eachVC/GCC/29ABI proof untouched; no new fixture pass or target match credit claimed.

## Continue C match and final proofs

80 genuine category-helper shared dependency/control headers44,81 full actual builder body AFTER root per bothC13s,82 bounded before-root visibility diagnostic all remain1393/470/79strict/0loose. True builder originally follows root (source3034..3175 vs1163..1250); initial native-address-order inference of before-root was corrected before source selection. Helper1570/native1587 is explicit rejected debt, never final. Full original CViewport/CText/CList/CDeletion/CTCTargetingBase methods and realmap used; no legacy stand-ins.

83 changes BOTH OTHER/default switch-arm exits from break tocontinue, no new locals/values/scopes: full1393/470MATCH.84 canonical old-header/source control with identical two exits alsoMATCH, proving completeness/dependency counts are not the lever.85 retains all genuine type/class corrections, removes category-only includes/body, guards existing pragmas, fullMATCH. Default proof catches inherited unguarded TMap typedef;86 guards it, fullMATCH. Full old-profile proof then detects lost accepted secondary-screen DTOR branch from older input copy;87 restores exact old44 branch under newoptin, and fixes duplicate RemoveAllConfiscatedItems in full-base+CONFISCATED integration exposed by native ABI fixture. Source85/86/87 bytes/SHA identical; active root1393+ReleaseData44 remains exact with109physical bindings and actual2-section DFS/zero debts. All earlier failed proofs retained; only87is final.

Later donor debug source usesbreak atthese arms; retailcontinue form is inferred from exact RTM/native code plus tested identical behavior, not asserted textualPDB evidence. No new proxy priority measurement or synthetic ruler. Final87fresh4096eachVC/Od/W3/GR-/noEH andGCC03/O0 casesPASS, exactstdout, explicitOTHER441/negativeunknown1496/positiveunknown1854 and cleanup/refcounts. All34nativeABI and244old/default/accepted token/returncode checksPASS. All2645testedheader paths/bytes equal frozen finaltree. Ready paths/hashes/11headerpatches and honest symbol supplements in continue_c/READY87.{json,md}.

Current worker counts86ordinary+12diagnostic=98target compiler invocations,23genuine families. New actual same-TU helper visibility and switch cleanup each addonefamily; repairs/controls/order variants not inflated. Main independent reviews are separate. Worker doesnotassignmatchcredit; main owns canonical row integration to avoid races. Continuing F3.

Main acceptance: independent85/87 rebuilds and final comment-annotated03 all1393/470 exact. Every109physicaloperand plus reachedReleaseData44 PASS; all17public-name-set omissions resolved explicitly through originalclassmetadata/vendor/native/import evidence, not false public-set claims. Fresh4096cases EACH VC71/Od/W3/GR-/noEH andGCC64C++03 PASS; mainfixture assertsOTHER441,negativeUnknown1496,positiveUnknown1854 andprints exactly one PASSline. Native18checks with exactrootflags PASS; worker34expandednativechecks and244oldprofilecontracts PASS. Combined45 recompiled87targets and3428namedbodies/relocations unchanged. Finalsource annotation explicitlymarks shippingcontinue wording inferred vs laterdebugbreak.98worker+3main+1combined=102actualtargetcompiles,23genuinefamilies; allsource/test/header histories preserved. Selected main_review03_inferred_source_note; no helperbyte progress credit.


## Pass23 self-contained test port

Production baseline current matches1393bytes/470instructions and real ReleaseData44 helper;109 physical operands PASS. Actual shared inventory/defs/component/list/string/STLport objects used. Both maps seeded nonempty before target so clear cannot silently pass.4096 scenarios retain exact OTHER441/unknown-negative1496/unknown-positive1854 counts, actual refcount/def/component/string lifetime checks. Native /Od /W3 without /EHsc and GCC64 C++03 PASS. No new headers; receipt sol_native_final_real_headers_02.

This is successful test-port verification, not an unmatched-source search. Production source attempts this pass:0; unchanged-source control grades:1. Earlier source family and invocation counts remain historical and are not reset or inflated by test fixture builds.
