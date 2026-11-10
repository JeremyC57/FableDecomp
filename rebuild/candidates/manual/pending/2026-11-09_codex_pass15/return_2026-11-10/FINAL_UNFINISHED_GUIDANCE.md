# Final guidance for the three unfinished rows

These are measured non-matches, not completed functions. `FINAL_003_handoff_metadata/RESULT.json` under each row's history verifies the current comment-annotated source against the integrated physical include tree. Older selections/checkpoints in row notes remain historical. Source-family counts below group variants, combinations and rechecks; they are not compiler-invocation counts.

| Row / retained body | Ours / retail bytes | Ours / retail instructions | Current measured residue |
|---|---:|---:|---|
|0064fdb4 quest bar,096|2593/2593|898/898|Frame 0x164 versus 0x16c; scalar/object homes and final fill geometry/UV push ordering;114 strict/20 loose hunks|
|00651273 minigame bar,096|1677/1681|601/602|Frame 0x214 versus 0x210; color/geometry homes and operation ordering;124 strict/59 loose hunks|
|00e5e350 commentary,036|584/585|187/186|Native first-timer zero absent; two alignment instructions present;3 strict/loose hunks|

Equal sizes or a native-sized frame are insufficient. The bar alternatives below still differ elsewhere; the minigame's four-byte length gap is not evidence of a single isolated four-byte fix. The integrated 159 sprite enum-field correction is retained in `hdr_integration/engine/CEnginePrimitive2DSprite.h`: original Layer and BlendMode enum types, unchanged default declarations. No new binary match resulted.

## 0064fdb4: quest bar

**Why attempts have not closed it.** Four independent early position snapshots 106/126 recover native 0x16c frame and three sprite homes but retain two extra early copy instructions;126 is 2598 bytes/900 instructions with 8 loose hunks. The late guarded-right construction 123 removes those copies but returns to 0x164 frame. Function-scope middle boxes 148/149 give 0x174 frame; exposing the complete actual sprite constructor 150/155 removes 16 bytes of frame space, not the required 8. Full box/vector copy/Initialise visibility 151–155, original floor local declarations 156 and enum fields 159 either canonicalize or retain the wrong frame. Reusing the already-computed left endpoint 157/158 gives 0x178 frame and removes 8 native instructions. These are specific negative measurements, not proof that a faithful source form cannot match.

**Next comparison to make.** Start with 096 and use 123/126 as diagnostic comparisons, not replacement selections. Native 0065033b/00650341 copies bar X into the old right/inner X home; final inner Y at 006506cc/006506e3 comes from bar Y, not the right icon's earlier Y home. `BOUNDARY_COVERAGE.md` records the exact PC trace, and both donors show original right/inner home sharing. A new lifetime hypothesis must preserve the three early X/Y snapshots and explain this component reuse while also fixing the final geometry/UV region. Use the existing validated frame/scheduler traces to identify the exact changed slot/conflict rather than optimizing loose percentage alone. No untried original expression spelling is claimed here; routine box/copy/body-visibility variants have already been measured.

**Tests and limits.** Current exact-body fixture:601 rendering cases,5 exits,81 portable floor inputs, PASS under VC7.1/GR- and optimized sanitized g++ C++03. The 25 added cases cover 8 zero,9 negative and 8 adjacent positive middle widths, unchanged skipped handle3, signed right-edge movement and a backwards fill box. Original second-icon height behavior is retained. Original floor versus portable twin separately passed 16385 eighth-step values and 15 boundary/exceptional inputs. Fixed bank dimensions/origin remain; percent exactly at epsilon, invalid targeting-component absence, zero denominator, exceptional coordinates and arbitrary callback changes are not established. See `rows/0064fdb4/{PROBES.md,BOUNDARY_COVERAGE.md,BOUNDARY_EXTENSION_AUDIT.json,TEST_RESULTS.json}`.

**15 substantive source families, conservatively grouped:**

|Family|Representative numbered histories|
|---|---|
|Original rectangle constructor overloads, including corners/extents|073b,114–116,128–129|
|Persistent edge-sprite lexical lifetimes|074|
|Success guard with shared zero-return tail|076|
|Named rectangle lifetimes, including persistent middle/inner boxes|077,088,090–094,148–149|
|Named geometric endpoints and scaled endpoint values|078,103–105|
|Original STLport bitset call contract|079|
|Complete real hero-alive predicate visibility|080–081|
|Separate right/inner position lifetimes and late construction|089,106–109,121–123,152|
|Scalar-coordinate and explicit rectangle-field dependencies|086,134–137|
|Real vector translation/addition-assignment dataflow|110–113,130–133|
|Complete actual sprite-constructor visibility/COMDAT forms|097–100,124–125,150,153–155|
|Scalar maximum-result branches|141–142|
|Complete highlight branch results|143–144|
|Complete optional-dimension branch results|145–147|
|Reuse of constructed left endpoint/box edge after callback|101,157–158|

References: `rows/0064fdb4/PROBES.md`, `RESUME_BATCH_151_159.json` and matching `history/0064fdb4/<number>_*/{candidate.cpp,RESULT.json}`. The 163 numbered histories include 3 compile failures, flags, aliases and audits; they are not 163 source mechanisms. Setup, rounding diagnostics002–071, inline-budget variants, punctuation/const aliases, enum/name audits and comment-only/final integration checks do not inflate the15 groups. Failed approximate floor forms are not admissible replacements.

## 00651273: minigame bar

**Why attempts have not closed it.** Retained096 has601 instructions against 602 native and broad local-home/order differences. Alternative079 gets native 0x210 frame and 90.77% loose alignment but exchanges left/centre object homes; selected 096 retains native sprite order with a4-byte displacement. The real constructor bodies089–095 remove16 bytes from selected frame, or48 from all-named geometry, rather than 4. Color RHS 134/135 increases the traced shared color/height slot weight 17→20 but leaves its home; it also replaces native member-channel reloads with local-channel reads. Assignment136/137 compresses the native channel copies. Branch-result138/139 and coordinate-copy140/141 regress or canonicalize. The actual CRGBColour::Interpolate wrapper uses one weight conversion plus MMX blending, so it cannot be substituted for this row's four channel conversions.

**Next comparison to make.** Keep096 and 079 side by side with their validated RTM frame traces. Preserve five distinct84-byte sprite homes and investigate the particular scalar/temporary lifetime responsible for the remaining4-byte packed-slot difference; a native frame by itself has already proved insufficient. Require any new source hypothesis to account for the measured color/height bucket and geometry-before-UV dependencies. Preserve snapshots across renderer callbacks: the mutation fixture changes Scale, Percentage, input position, GUI offset and alpha at specified calls. Do not infer that those values are immutable or move their reads across calls. Preserve the native fishing-dimension queries, including the unused width result. No new original-source spelling is established by these observations.

**Tests and limits.** Exact-body VC7.1 and g++ C++03 tests pass 1152 ordinary cases plus 1 callback-mutation sequence. Ordinary inputs cover all8 valid types, percentages0/0.25/0.5/1, scales0.5/1/2, optional icon/arrow presence and 3 alpha levels. Fixed dimensions/origin and one chosen mutation sequence do not establish every possible callback, epsilon-adjacent/negative/out-of-range percentage or exceptional coordinate. All47 relocation identities/multiplicities and literals were audited. See `rows/00651273/{PROBES.md,FOLLOWUP_TEST_RESULTS.json,RELOCATION_AUDIT.json}`; older015/069/088 references are checkpoints, not the final selection.

**15 substantive source families, conservatively grouped:**

|Family|Representative numbered histories|
|---|---|
|Original corner/extents rectangle constructors|004–005,097|
|Direct versus named optional-icon argument lifetimes|006,048–049,073–078|
|Real vector addition and precomputed opposite endpoints|009,020–021,053–056|
|Named/temporary rectangle lifetime combinations|010,022–029,087|
|Icon dimensions held as real vector/scalar objects|011–012,047|
|Sprite lexical lifetimes and original nested icon/fishing scopes|016–019,110|
|Height materialization and placement relative to position copies|030–034,041,079|
|Coordinate/endpoint reference-bound temporary lifetimes|014–015,059–062|
|Reuse of one actual rectangle object|035|
|Explicit scalar coordinate dependencies|038,042–045|
|Original named bar positions and copy/update forms|050–052,063–068,127–128,140–141|
|Complete actual position/box sprite-constructor visibility|089–095|
|Coordinate/product grouping with measured scheduler tuples|007,111–118|
|Original colour copy body versus default/copy assignment|119–120,136–137|
|Complete colour results assigned in Type branches|138–139|

References: `rows/00651273/PROBES.md` and corresponding `history/00651273` source/results. The 142 numbered histories include 2 compile failures. Flags 002/003/057/058, names/enums008/046/088/096, canonicalized aliases/getters/copy initializations, combination reruns and final verification are not extra families. The disproven local-colour RHS 134/135 and rejected float-colour path 125/126 are documented negative diagnostics, not additional evidence for an original implementation.

## 00e5e350: henchman commentary

**Why attempts have not closed it.** Native zero at 00e5e355 addresses entryESP−16, the same home receiving the first timer index. It is not vector initialization. Native out-of-line constructor 00cd4450 has no initial zero, and both raw donor timer classes contain only a four-byte long handle. TRetVoid is empty. The validated004 scheduler entry has no zero-store node to reschedule;029–035 all reproduce the same target bytes. Selected036 restores actual names/private field/direct registration initialization and removes the ineffective speculative constructor zero, with unchanged residue. Native's missing8-byte store versus 7 extra alignment bytes explains the net one-byte size gap; branch offsets necessarily differ. A fabricated zero-only object/helper or vector seed would not recover evidenced source.

**Next evidence to obtain.** Seek the actual generated script/constructor/header lifetime spelling that could explain the initial first-handle store. Current donor types and native callees do not justify an extra member or nonempty failure carrier. Another parenthesized assignment, array-size variation or repeated budget sweep would repeat already measured families. The final outer-loop cleanup tests037/038 also canonicalize: both retain584/585 bytes,3 hunks, exactly the final selected target bytes and every relocation record. Their explicit-return and guarded-do/while forms preserve one termination query per original check. They establish one more tested family, not an untried lead.

**Tests and limits.** Current036 exact bodies pass 101 defined pre-comparison paths under VC7.1 and optimized sanitized g++ C++03. They cover termination/empty-list frames, commentary flag/timer paths, timer queries and reverse release order, stopping before the first position comparison. Native reads HeroLastPosition before initializing its float members. The fixture deliberately leaves them uninitialized and uses rejecting later stubs; no full-loop comparison, weapon update, position update or idle-comment execution is claimed. See `rows/00e5e350/{PROBES.md,CURRENT_RESULT.json,ORIGINAL_FUNCTION_LOCALS.json,ORIGINAL_TIMER_RETURN_TYPES.json}`. Old004 audit/resume documents are historical;036 and FINAL_003 describe the retained source.

**10 established clean source families:**

|Family|Representative numbered histories|
|---|---|
|Substantial reconstruction on real shared engine/STLport headers|001b|
|Timer constructor zero initialization in list/body|002,007|
|Real timer temporary/copy/direct initialization|005–006,015,031,033|
|Timer declaration scope/grouping|008,032|
|Actual timer/Return bodies before/after caller or in class|009–010,030–031,034–035|
|Actual timer arrays and initialization/cleanup lifetimes|011–012,019,021|
|Empty vector declaration before/between timers|023–024|
|Constructor handle read-modify-write from an initial zero|025–027|
|Original TRetVoid return-carrier lifetime/value initialization|028–029|
|Outer termination-loop condition/cleanup structure|037–038|

The shared-header reconstruction counts once because the README lists rebasing onto real shared types as a source lever; there are 9 additional families. The 43 numbered histories through 038 include 1 compile failure. The two loop forms 037/038 count together once, despite identical emitted bytes. Invented TimerPair/IdleTimer wrappers 013/014/020/022 are preserved rejected diagnostics and excluded from the clean-source minimum. Flags 003, the inline-budget pragma 004, punctuation 016–018, combinations and 036/final fidelity rechecks do not increase the count. This establishes 10 grouped families without counting the invented wrappers, flags or punctuation; it does not mean 43 distinct mechanisms or a matching source. See `rows/00e5e350/PROBES.md`, `RESUME_BATCH_029_035.json` and numbered source/results.
