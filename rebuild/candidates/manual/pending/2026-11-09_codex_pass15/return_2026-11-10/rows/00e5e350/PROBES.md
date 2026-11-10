# 00e5e350 current attempts

IN_PROGRESS: no final park decision. Each history directory contains the exact source/header snapshot and compiler result. Older package attempts remain untouched.

| Trial | Result | Bytes (ours/retail) | Strict hunks |
|---|---|---|---|
| 001_shared_headers | COMPILE_FAIL | -/- | - |
| 001b_counted_pointer_include | DIFFER | 544/585 | 16 |
| 002_initial_timer_zero | DIFFER | 544/585 | 16 |
| 003_budget_T12 | DIFFER | 493/585 | 20 |
| 003_budget_T20 | DIFFER | 509/585 | 20 |
| 003_budget_T4 | DIFFER | 427/585 | 19 |
| 003_budget_T40 | DIFFER | 491/585 | 15 |
| 003_budget_T8 | DIFFER | 427/585 | 19 |
| 004_scoped_exit_with_zero | DIFFER | 584/585 | 3 |
| 005_value_initialised_first | DIFFER | 584/585 | 3 |
| 006_value_initialised_both | DIFFER | 584/585 | 3 |
| 007_ctor_body_zero | DIFFER | 584/585 | 3 |
| 008_nested_timer_scope | DIFFER | 584/585 | 3 |
| 009_destructor_after_caller | DIFFER | 584/585 | 3 |
| 010_timer_bodies_after_caller | DIFFER | 584/585 | 3 |

Selected: 004_scoped_exit_with_zero.

001 failed for a missing counted-pointer include.003 budget flags are one inline-budget investigation, not five source attempts.004 restores the native mixed destructor calls under the owner ladder; only then does the near-miss stream return. No new match. Zero initialization, value initialization, nested scope and body order do not preserve the entry zero store.

## Follow-up source strategies and native initialization

011–012 two-element timer arrays (references versus indexing) expand lifetime cleanup and emit664 bytes.013–014 local pair aggregate, with implicit versus explicit constructor, changes destructor grouping (575 bytes) and does not preserve native entry store.015 copy initialization canonicalizes to004.016–018 constructor result/assignment parentheses and comma initialization test whether tuple boundaries retain the entry store: all584, identical residue.019 initialized array656;020 initialized aggregate575.021 one-element array644;022 one-element aggregate returns584.023–024 move the empty vector declaration before or between timer declarations; both canonicalize to004.025–027 zero-initialized timer addition/bitwise accumulation all fold back to004. No new shape selected and no match claimed. All exact source/header/compiler logs preserved; aliases/parentheses with identical output are not counted as distinct mechanisms.

The actual unmodified C2 scheduler entry DAG for004 contains the two virtual timer registrations and handle stores, but no initial zero store. Diagnostic leaf was checked equal to normal RTM by trace_schedule.py. Thus scheduling alone cannot restore the removed zero. Entry disassembly and stack accounting place native zero at first timer: after two pushes [esp+c], after third push [esp+10], also the first RegisterTimer return destination. The vector occupies [esp+14],[esp+18],[esp+1c]. Its first access is fld at00e5e47d, preceding its first writes at00e5e4f3/4fd/504. Native therefore does not initialize lastHeroPos before comparison; empty shared default constructor is faithful. Native out-of-line CTimer constructor00cd4450 itself has no zero store; it registers and writes the returned index. Destructor00cd4470 deregisters the stored index.

Standalone exact-body test covers101 defined executions: first termination, repeated empty-list frames and exits, nonempty early exit, commentary state/timer combinations, state clear, and termination at the weapon-timer branch. It verifies timer registration/reverse release order, timer-query sequence, frames, state and untouched weapon flag. Both VC7.1/Od and g++03 pass. GetHero/GetPos and all post-comparison effects are rejecting stubs; test leaves the vector uninitialized and never executes its comparison. Full-loop behavior cannot be claimed from standard C++ tests because that first comparison reads indeterminate floats. No fixture seeding or source zeroing was introduced.

## Original script return type and locals

028 restores the raw-donor original local spellings and the actual TRetVoid `_Fail_Val` plus `Return()` form. The emitted body remains584/585 with the same entry-store residue. This authentic source form supplies no byte improvement; selected004 remains unchanged. Raw donor evidence is in ORIGINAL_FUNCTION_LOCALS.json; candidate and compiler output are preserved in history028.

## Fresh donor/header review and retries 029–035

The selected004 source, headers and test are unchanged. Both raw donor CTimer records are a non-polymorphic four-byte object with private `long TimerIndex`; both TRetVoid records are empty one-byte classes with only `Return()`. Full records are saved in ORIGINAL_TIMER_RETURN_TYPES.json. Native ctor00cd4450 has only the timer-registration call and returned-index store; dtor00cd4470 releases that index. Repeated entry-stack accounting confirms zero at entryESP−16 and the first timer's later returned-index store at the same address; vector components occupy entryESP−12/−8/−4. No constructor, return-carrier field, extra helper or vector initialization has been invented to retain the zero.

| Trial | Authentic source lever | Result |
|---|---|---|
|029_value_initialized_failure_carrier|Value-initialize the real empty TRetVoid `_Fail_Val`|DIFFER584/585,3hunks|
|030_late_real_return_body|Declare real Return before caller, put its proven empty body after caller; restore private TimerIndex in opt-in header|DIFFER584/585,3hunks|
|031_late_native_ctor_value_initialization|First timer copy-value-initialization with actual no-initial-zero ctor body after caller; destructor/operator= also later|DIFFER584/585,3hunks|
|032_joint_timer_declaration|Declare both real timers in one declaration|DIFFER584/585,3hunks|
|033_direct_temporary_timer_initialization|Direct initialization from a real CTimer temporary|DIFFER584/585,3hunks|
|034_in_class_timer_constructor_visibility|Move genuine ctor body into original CTimer class under opt-in|DIFFER584/585,3hunks|
|035_in_class_all_timer_bodies_visibility|In-class ctor,dtor,operator=,GetTime with original private field|DIFFER584/585,3hunks|

Every new leaf byte sequence equals selected004, including its missing eight-byte entry zero and seven alignment bytes. There are40 preserved normal-grade histories: numeric001–035 plus001b and four extra003 budget grades. These source/declaration spelling checks do not establish seven new compiler mechanisms. No match or semantic improvement is claimed. RESUME_BATCH_029_035.json records each result/hash.

Independent resumed checks verified the exact selected timer definitions and whole function body are present in the standalone fixture. VC7.1/Od/W3 and g++ C++03/O2 with address/undefined sanitizers pass all101 defined pre-comparison cases. RESUME_TEST_* logs preserve these independent runs. The fixture still stops before reading the uninitialized vector and exercises no full-loop comparison. Next evidence worth obtaining would be original generated script source or an independently recovered original constructor/lifetime spelling that explains the otherwise dead first-handle store; a new zero-only helper or stand-in is not a solution.

## Faithful selected model036

036 retains exactly004's584-byte target body and three hunks while restoring original donor local names and TRetVoid returns, placing TimerIndex behind an opt-in private access branch, and initializing it directly with RegisterTimer as in the real constructor. The ineffective speculative initial zero is removed. This is a fidelity cleanup, not another distinct matching mechanism or a new match. Fresh VC71 /GR- and optimized sanitized g++03 tests pass101 defined paths with the exact source bodies; output now exactly00e5e350_TEST PASS. Original004 and its previous fixture remain preserved. Full-loop comparison remains untested because it reads the native uninitialized vector. Selected036 supersedes004 for handoff.

Final handoff: selected body retained, with required line-2 DIFFER annotation. FINAL_003_handoff_metadata recompiles this comment-only change using the integrated physical include tree. It is a verification grade, not another source-shape attempt.

## Final distinct exit-structure family

037 replaces the outer while guard with an infinite loop and explicit termination return.038 uses an initial guard and do/while. Both preserve exactly one termination query at each original check, real timer types, and reverse destruction order. These are one additional condition/cleanup source family, not two independent mechanisms. Both compile584/585 with3hunks and exactly the selected raw target bytes and all relocation records; the compiler canonicalizes both. No improvement, and neither replaces the selected036 body. Invented local timer-wrapper diagnostics013/014/020/022 are excluded from the clean-source minimum; flags and punctuation variants are also excluded.

Final authority: CURRENT_VERIFICATION.json and root STATUS.tsv supersede historical selected/pending paragraphs. Final compile FINAL_003_handoff_metadata preserves the chosen body; later timer037/038 do not improve it.
