# 009bbcc0 final handoff

Final status: **PARKED**. Selected grade: 51. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/009bbcc0/PROBES.md

# 009bbcc0 probe ledger

Status: incomplete, not a verified match. Native extent is 1106 bytes, ending after ret 8 at 009bc112. No standalone semantic fixture or complete relocation acceptance audit has been completed yet.

Every attempt retains source, full private headers, compiler log, object, exact symbols and native/built disassembly under history/009bbcc0/attempt_NN. Independent original type evidence is evidence/009bbcc0_Ego_r_types.json and evidence/009bbcc0_FableWin_types.json. Original locals from both donors are evidence/009bbcc0_locals.json.

The closest instruction structure currently is attempt09: 1140 bytes. Aggregate stack placement is the principal mismatch: native sub_parser+38/context+60/symbol+8c/name_length+90/parser+94; built sub_parser+38/parser+60/context+88/symbol+b4/name_length+b8. Attempt12 splits the loop guards and reaches1099 bytes but loses the native short-circuit temporary-lifetime code. Attempt14 pointer cleanup reaches1132, again with a less faithful cleanup loop. The shortest extent is not the selected source.

Attempt08 contains a recursive overload mistake and is rejected; attempt09 repairs it. Attempt07 register hint is not counted as a distinct strategy. Source/constructor/helper control-flow forms remain recorded even when they produced the same code. Flags were unchanged in every attempt.

- Attempt01: 01: structured nested contexts and original inline lifetime helpers — DIFFER(1140v1106).
- Attempt02: 02: early separator guard with all component locals in the while scope — DIFFER(1140v1106).
- Attempt03: 03: native mode-first context initialization and null-first conversion; restore original context destructor — DIFFER(1140v1106).
- Attempt04: 04: context declared at while-scope entry then initialized after parsing substring — DIFFER(1140v1106).
- Attempt05: 05: original parser memberwise copy visible to normal optimizer — DIFFER(1140v1106).
- Attempt06: 06: copy-initialization of the outer parser instead of direct construction — DIFFER(1140v1106).
- Attempt07: 07: register storage hint for outer parser lifetime — DIFFER(1140v1106).
- Attempt08: 08: both original constructors delegate through their original Init overloads — DIFFER(589v1106).
- Attempt09: 08 repair: explicitly select raw-text Init overload, retaining delegated constructors — DIFFER(1140v1106).
- Attempt10: 09: full original CheckUnknownSymbols body visible in same translation unit — DIFFER(1140v1106).
- Attempt11: 10: independently query parser positions for cleanup start and rewind start — DIFFER(1140v1106).
- Attempt12: 11: separate loop termination guards to shorten name-temporary lifetime — DIFFER(1099v1106).
- Attempt13: 12: full original PersistError body exposes const parser reads and callback dispatch — DIFFER(1140v1106).
- Attempt14: 13: cleanup pointer range traversal instead of index addressing — DIFFER(1132v1106).
- Attempt15: retry best delegated-constructor source with the fully restored original CBasicString comparison from00501d30. Still1140v1106; header repair/recheck, not counted as a new distinct source strategy.

## Current eligibility restoration (main)

Selected main19: DIFFER(1140v1106), sourceSHA53f12084a48210d8725ae2029e3b9a1a192e929a214fa79ee8de133a965820f0. Any older authored generated member source is INELIGIBLE and preserved in history/review backups. Current generated members are implicit or original external declarations. Exact-body GNU64 and VC7.1 tests PASS; root semantic/ten-valid-family parked acceptance remains pending. The older status prose and grade headings above are historical.


## Current corrected selection and eligibility

# Specialist probes and final selection

Final selected50 retains the root19 Main body but corrects active header eligibility and completes the source-family minimum. Both original PDBs mark CStringParser copy and CParsedItem default/copy/dtor0x103; none of those bodies is authored. CStringParser's native-called generated copy remains declaration-only. The original authored CStringParser() default constructor (attrs0x003) is restored as a declaration. Root-agreed CParsedItem opt-in selects genuine implicit construction while preserving the legacy default branch unchanged.

Initial20–37 replays are preserved but do not supply the final counted ledger. In particular20–23 had a changed overload boundary in old constructor forms and are not counted; stale active CParsedItem authored constructor definitions are not treated as validated merely because unused.38–48 repeat eleven genuinely distinct source/control/helper-context families with the fully corrected implicit CParsedItem model: natural delegated constructor/short-circuit loop; flattened original constructor operations using the raw Init overload; positive/negative separator control; inner context lifetime before parsing; full CheckUnknownSymbols F1 context; full PersistError/callback/concat F1 context; separate loop exits; character-pointer cleanup traversal; staged defaults virtual result; explicit owned name-argument/boolean lifetime; and context lifetime before the outer parser/reinitialization per transfer. This excludes copy-init spelling, register hints, store-order-only variants, duplicate GetStringPos spellings, annotation/flags, generated bodies and header repairs. Even conservatively excluding the final outer-context alternative, ten genuine eligible families remain.49/50 gate wiring/final header confirmation are not counted.

All52 actual semantic operand identities and three original pure-interface slots are independently reaudited on50. The exact Main fixture passes1536 scenarios/compiler under a unique source/test receipt. Same native frameac but aggregate slot permutation and cleanup remain; smaller candidates lose native temporary-lifetime structure and are not promoted.

20. Eligible original parser control/helper-context family 1 with original external generated copy declaration, no authored generated body — DIFFER(1105v1106).
21. Eligible original parser control/helper-context family 2 with original external generated copy declaration, no authored generated body — DIFFER(1105v1106).
22. Eligible original parser control/helper-context family 3 with original external generated copy declaration, no authored generated body — DIFFER(1104v1106).
23. Eligible original parser control/helper-context family 4 with original external generated copy declaration, no authored generated body — DIFFER(1104v1106).
24. Eligible original parser control/helper-context family 9 with original external generated copy declaration, no authored generated body — DIFFER(1140v1106).
25. Eligible original parser control/helper-context family 10 with original external generated copy declaration, no authored generated body — DIFFER(1140v1106).
26. Eligible original parser control/helper-context family 11 with original external generated copy declaration, no authored generated body — DIFFER(1140v1106).
27. Eligible original parser control/helper-context family 12 with original external generated copy declaration, no authored generated body — DIFFER(1099v1106).
28. Eligible original parser control/helper-context family 13 with original external generated copy declaration, no authored generated body — DIFFER(1140v1106).
29. Eligible original parser control/helper-context family 14 with original external generated copy declaration, no authored generated body — DIFFER(1132v1106).
30. Eligible original parser control/helper-context family new_staged_defaults with original external generated copy declaration, no authored generated body — DIFFER(1120v1106).
31. Freeze eligible retained original parser source after ten genuine control/helper families; original authored default constructor declaration restored, generated copy remains original external declaration, no authored generated bodies — DIFFER(1140v1106).
32. Eligible genuine parser shape flattened_raw_init_01; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
33. Eligible genuine parser shape flattened_raw_init_02; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
34. Eligible genuine parser shape flattened_raw_init_03; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
35. Eligible genuine parser shape flattened_raw_init_04; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
36. Eligible genuine parser shape explicit_name_argument_lifetime; original generated copy external only, original authored context/default helpers — DIFFER(1099v1106).
37. Eligible genuine parser shape context_lifetime_before_outer_parser; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
38. Fully eligible parser family: natural original delegated constructors and short-circuit while; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
39. Fully eligible parser family: flattened authored parser constructor original operations with raw Init overload; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
40. Fully eligible parser family: negative separator guard instead of positive if/else lifetime; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
41. Fully eligible parser family: inner context declared before token and substring parsing; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
42. Fully eligible parser family: full genuine CheckUnknownSymbols callee body context; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
43. Fully eligible parser family: full genuine PersistError and callback/concatenation context; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
44. Fully eligible parser family: separate Finished and name-skip loop exits; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1099v1106).
45. Fully eligible parser family: cleanup traverses original character pointer range; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1132v1106).
46. Fully eligible parser family: defaults virtual result staged before context constructor arguments; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1120v1106).
47. Fully eligible parser family: explicit owned name argument and boolean lifetime before token parsing; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1099v1106).
48. Fully eligible parser family: context lifetime begins before outer parser and is reinitialized per transfer; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
49. Final eligible parser gate wiring: original generated CParsedItem constructor implicit under agreed opt-in, default legacy branch preserved; authored CStringParser default declaration restored; not a new source family — DIFFER(1140v1106).
50. Confirm final eligible source with exact agreed root CParsedItem opt-in header branch and untouched legacy default line; final documentation/header wiring only, not a source family — DIFFER(1140v1106).

Main51 independently rebuilt and reaudited this corrected selection;1536 scenarios passed under both compilers. Earlier generated-body diagnostics are historical only.


## workers/parked_expert/hard_batch2/009bbcc0/PROBES.md

# Parser expert completion

The inherited record1–15 was accurately incomplete: no fixture or full operand review existed. All prior source deltas were read. Invalid recursive constructor8 remains rejected. Private16 restores the actual implicit CStringParser copy instead of authoring it and declares the missing authored default constructor. Both original raw PDB method lists give copy0x103/dtor0x107 and default ctor0x3; CTOR_DTOR_TPI_EVIDENCE.json preserves this.16 remains1140/1106.17 uses independently observed direct nested-context construction after sub_parser and separate parser position queries; same1140.18 freezes this eligible source with grade documentation, not a new family.

The private original donor Main02fec010(1461 bytes) proves substring reset/parse, nested context single-transfer setup, apply-default flag timing, unknown-symbol check, source cleanup, and empty fallback. Main's exact protected Leaf and signatures exist in both original name sets/procedures. Original types supply every member. No new authored generated special member or stronger annotation is selected.

The new exact-body fixture passes1536 cases each on GNU C++03 and VC7.1 /GR-. It checks absent/one/multiple components, two name lengths, preexisting defaults, initially finished context, parent finish during transfer, missing semicolon with returning error callback, invalid separator, nullable cleaning buffer, preservation of newlines, nested versus parent callbacks/settings, default-only fallback, original parser position preservation and balanced string/parser/context lifetimes. Tokenization/helper return boundaries are mocked; this tests Main's protocol and effects, not all retail parser internals. An initial exception-injection diagnostic was removed because this project's native fixture lane does not enable exception unwinding; failed receipt remains immutable. No exception behavior claim is made.

All52 COFF operands are independently bound to decoded native calls/global/vtable/literal payloads. Three pure virtual methods are proven by both complete original TPI records and native offsets; no absent out-of-line pure-virtual public is claimed. Exact1106-byte native span ends ret8 at009bc10f then14CC through009bc11f. Object remains1140; no byte parity or match is claimed.


## workers/parked_expert/hard_batch4/009bbcc0/PROBES.md

# Specialist probes and final selection

Final selected50 retains the root19 Main body but corrects active header eligibility and completes the source-family minimum. Both original PDBs mark CStringParser copy and CParsedItem default/copy/dtor0x103; none of those bodies is authored. CStringParser's native-called generated copy remains declaration-only. The original authored CStringParser() default constructor (attrs0x003) is restored as a declaration. Root-agreed CParsedItem opt-in selects genuine implicit construction while preserving the legacy default branch unchanged.

Initial20–37 replays are preserved but do not supply the final counted ledger. In particular20–23 had a changed overload boundary in old constructor forms and are not counted; stale active CParsedItem authored constructor definitions are not treated as validated merely because unused.38–48 repeat eleven genuinely distinct source/control/helper-context families with the fully corrected implicit CParsedItem model: natural delegated constructor/short-circuit loop; flattened original constructor operations using the raw Init overload; positive/negative separator control; inner context lifetime before parsing; full CheckUnknownSymbols F1 context; full PersistError/callback/concat F1 context; separate loop exits; character-pointer cleanup traversal; staged defaults virtual result; explicit owned name-argument/boolean lifetime; and context lifetime before the outer parser/reinitialization per transfer. This excludes copy-init spelling, register hints, store-order-only variants, duplicate GetStringPos spellings, annotation/flags, generated bodies and header repairs. Even conservatively excluding the final outer-context alternative, ten genuine eligible families remain.49/50 gate wiring/final header confirmation are not counted.

All52 actual semantic operand identities and three original pure-interface slots are independently reaudited on50. The exact Main fixture passes1536 scenarios/compiler under a unique source/test receipt. Same native frameac but aggregate slot permutation and cleanup remain; smaller candidates lose native temporary-lifetime structure and are not promoted.

20. Eligible original parser control/helper-context family 1 with original external generated copy declaration, no authored generated body — DIFFER(1105v1106).
21. Eligible original parser control/helper-context family 2 with original external generated copy declaration, no authored generated body — DIFFER(1105v1106).
22. Eligible original parser control/helper-context family 3 with original external generated copy declaration, no authored generated body — DIFFER(1104v1106).
23. Eligible original parser control/helper-context family 4 with original external generated copy declaration, no authored generated body — DIFFER(1104v1106).
24. Eligible original parser control/helper-context family 9 with original external generated copy declaration, no authored generated body — DIFFER(1140v1106).
25. Eligible original parser control/helper-context family 10 with original external generated copy declaration, no authored generated body — DIFFER(1140v1106).
26. Eligible original parser control/helper-context family 11 with original external generated copy declaration, no authored generated body — DIFFER(1140v1106).
27. Eligible original parser control/helper-context family 12 with original external generated copy declaration, no authored generated body — DIFFER(1099v1106).
28. Eligible original parser control/helper-context family 13 with original external generated copy declaration, no authored generated body — DIFFER(1140v1106).
29. Eligible original parser control/helper-context family 14 with original external generated copy declaration, no authored generated body — DIFFER(1132v1106).
30. Eligible original parser control/helper-context family new_staged_defaults with original external generated copy declaration, no authored generated body — DIFFER(1120v1106).
31. Freeze eligible retained original parser source after ten genuine control/helper families; original authored default constructor declaration restored, generated copy remains original external declaration, no authored generated bodies — DIFFER(1140v1106).
32. Eligible genuine parser shape flattened_raw_init_01; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
33. Eligible genuine parser shape flattened_raw_init_02; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
34. Eligible genuine parser shape flattened_raw_init_03; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
35. Eligible genuine parser shape flattened_raw_init_04; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
36. Eligible genuine parser shape explicit_name_argument_lifetime; original generated copy external only, original authored context/default helpers — DIFFER(1099v1106).
37. Eligible genuine parser shape context_lifetime_before_outer_parser; original generated copy external only, original authored context/default helpers — DIFFER(1140v1106).
38. Fully eligible parser family: natural original delegated constructors and short-circuit while; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
39. Fully eligible parser family: flattened authored parser constructor original operations with raw Init overload; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
40. Fully eligible parser family: negative separator guard instead of positive if/else lifetime; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
41. Fully eligible parser family: inner context declared before token and substring parsing; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
42. Fully eligible parser family: full genuine CheckUnknownSymbols callee body context; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
43. Fully eligible parser family: full genuine PersistError and callback/concatenation context; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
44. Fully eligible parser family: separate Finished and name-skip loop exits; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1099v1106).
45. Fully eligible parser family: cleanup traverses original character pointer range; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1132v1106).
46. Fully eligible parser family: defaults virtual result staged before context constructor arguments; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1120v1106).
47. Fully eligible parser family: explicit owned name argument and boolean lifetime before token parsing; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1099v1106).
48. Fully eligible parser family: context lifetime begins before outer parser and is reinitialized per transfer; CParsedItem default constructor genuinely implicit, CStringParser generated copy external only — DIFFER(1140v1106).
49. Final eligible parser gate wiring: original generated CParsedItem constructor implicit under agreed opt-in, default legacy branch preserved; authored CStringParser default declaration restored; not a new source family — DIFFER(1140v1106).
50. Confirm final eligible source with exact agreed root CParsedItem opt-in header branch and untouched legacy default line; final documentation/header wiring only, not a source family — DIFFER(1140v1106).
