# Current work checkpoint

Scope: only 0041e6d3 (NUISystem::CManager::Action) and 00cbfb7d (RunCutsceneMacro). No commits or remote writes. Supplied sources and history preserved under input/. Selected rows are the compiled partial checkpoints recorded in CURRENT_STATUS.json; original supplied versions remain preserved.

## Verified baseline

- Compiler is MSVC 7.1 RTM 13.10.3077. Unmodified c2.dll SHA256 bcd28f39b1798b07989c30a1df471cf87fc20e3802d130901a08f83800e6b81e.
- Retail executable SHA1 39fd416ba23a91a89713ec80c789f57ddb22e2c8 verified for grading.
- Action: 35,593 vs 35,651 bytes, frame 0x800 exact. Includes complete switch tables in byte gate.
- RunCutsceneMacro: 74,965 vs 74,778 bytes, frame 0x18e4 vs 0x18dc.
- No new full match. Behavior tests and complete name/header audit remain outstanding.

## New diagnostic findings

The supplied debugger probe could not attach under this Wine environment. A local diagnostic backend proxy now records the same allocator sites without debug attachment. The proxy changes only the child compiler process in memory, never the original c2.dll file. Every diagnostic result must match the separately built ordinary RTM object section exactly; baseline Action, baseline cutscene and Action raw-pointer probe passed that gate. Tool lives in runtime/c2trace_jumps.cpp and trace.py. Do not grade or deliver a row using its diagnostic build.

Action: cached action raw-pointer candidate 1395 has priority 1015 / 106 refs and chooses ESI. game candidate 102 has priority 571 / 62 refs and chooses EDI. This is a priority imbalance, not a declaration-order tie. The raw-pointer-local probe increases action priority to 1117 / 130 refs and worsens residue. The uses are distributed across case bodies, not concentrated in one hot loop. Inspect actual helper/member source shapes rather than adding fake register-shaping operations.

Cutscene SlideTeleport named-value probe saves 24 bytes and slightly reduces diagnostic residue but still fails local x87 shape and full byte gate. Other prefix-control rewrites largely compile identically or introduce broad allocation regressions. No speculative candidate promoted.

## Reproduction

Source env.sh. python grade.py ADDRESS UNIQUE_TAG --source PATH [--headers PATH] creates immutable ordinary-compiler receipts. python trace.py ADDRESS EXISTING_TAG creates a diagnostic allocator trace and checks selected section byte equality. Compiler calls use the task-specific Wine reaper and require local network permission for its Unix IPC in this sandbox. Keep the reaper alive. Native preprocessing runs before Wine.

See rows/*/PROBES_CURRENT.md for current attempts, input/ for all previous sessions, and history/ for detailed immutable receipts. Native object/disassembly files are internal evidence; exclude retail bytes and game files from a future hand-back.

## Updated Action handoff received mid-session

CManager_Action_0041e6d3_progress.zip (144353 bytes) was extracted separately to incoming_action_progress/. README, NOTES, ATTEMPTS and every new WORKLOG/LESSONS/CHUNK_BRIEF/header-readme section were read; unchanged documents and headers were compared with the original input. Source differences are preserved in evidence/incoming_v23_source.diff. Header code is unchanged.

The incoming v23 reproduces 35697/35651 bytes, frame 0x808/0x800. It already includes the 13 named-bool fixes independently found here, as well as reverse-iterator, POP_MAP_STATE, FindChild and scroll changes. Do not count those 13 boolean fixes twice. rows/0041e6d3 now selects that newer supplied baseline, retaining all prior variants and receipts.

Additional confirmed local forms: a CActionParamSLONG* count object emits push [eax+8] in BUY_BULK/SELL_BULK; a const long& instead emits add eax,8 / push [eax] and is rejected. NumberAugmentations is unsigned long; <=0 produces retail's unsigned branch, while ==0 produces jne. Both combined on v23 give 35695 bytes. In trade action menus, cache PInventoryTrade only AFTER FindChild succeeds, separately in each branch: native reloads it at 4254a9/4254ec (and buy equivalents), then uses the same receiver across GetNumber and Construct. An earlier probe cached it before FindChild, which moved the loads too early; retain only as research. v23_trade_branch_locals combines the properly placed caches plus count/guard fixes: 35699 bytes, frame still 0x808, diagnostic loose hunks 112 vs v23 baseline 126. It remains a candidate, not a completed row.

Cutscene loop-guard splits and C3DVector ctor/copy-ctor body spellings compile identically to baseline. Named SlideTeleport fraction removes a duplicated x87 load and saves 32 bytes but still misses native spill/order; do not treat shorter output as a match. All compiled strategies are enumerated in PROBES_CURRENT.md, with immutable receipts. No behavior test or full symbol/header audit has been completed.

## Frame cause found after v23 merge

Compiler /FAs listings were obtained for original v17 and incoming v23. Each listing build's selected section was checked byte-identical to its ordinary grade. frame_symbols.json records named and temporary slots. Alignment of matching instruction streams (diagnostic, not identity proof) found v23's `_texture` at -0x3c8 mapping consistently to retail -0x268, which other counted-pointer locals already occupy: exactly an extra eight-byte object. Source has FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR, which hides the real constructor body behind a declaration.

`v23_counted_ctor_visible` adds an isolated shared-header opt-in FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR. It uses the EXISTING faithful generic CCountedPointer(T*) constructor body from the pinned shared header, and guarded __declspec(noinline) preserves the retail call sites. No fake mod/ref body. Default header branch unchanged. RESULT: 35694/35651; frame restored to 0x800. All constructor relocation counts and exact symbols unchanged (CComponent 25, CAction 17, CTexture 1). LOAD_PHOTO also now emits retail's `push 8; pop ebx; push ebx` and uses ebx for the four CPixelFormat arguments; texture now lives at native -0x268. photoTexture alias still has the wrong home, and entry register swap persists. This is a substantive improvement, not a full match.

The two new reverse-iterator initializations were not the cause to remove: direct construction from end(), default construction then assignment, and postfix increments all compiled byte-identically to v23. Keep incoming session-5 semantic/iterator fixes.

`v23_combined_visible_ctor` combines this header with v23_trade_branch_locals (count-object pushes, unsigned guard and receivers cached AFTER successful FindChild in all four arms). Its immutable receipt records the result. Preserve original header and incoming baseline; do not mistake any partial match for a landed row.

## Cutscene frame sharing and continuation

`thing_alive_visible` replaces a declaration-only IS_THING_ALIVE template with its real read-only pointer/null/dead-bit logic, preserving the original exact instantiated name and the out-of-line call. CThing.h supplies IsAlive over Dead at +0x91 bit zero; native 0040dce0 independently confirms null rejection and that bit test. The helper body is semantically faithful but its emitted 20-byte body is NOT itself byte-identical to native's 21-byte helper; only the assigned RunCutsceneMacro Leaf is graded and returned. No helper landing is proposed. The extra `_pThing` frame word now shares +0x20, reducing the frame from 0x18e4 to 0x18e0 (retail 0x18dc). Assigned row: 74955/74778, still DIFFER. `current_checkpoint_01` recompiles the selected source after updating its status comments, with identical selected bytes. No behavioral tests or complete name/header audit yet.

The user cancelled the wrap-up/upload and asked to continue manual investigation. No ZIP was uploaded. Frame mapping is diagnostic only; source changes are individually reasoned from native disassembly, not generated in an automated search. Current remaining frame word is under investigation.

## Manual continuation: exact cutscene frame

The last extra frame word was boolean padding, not an unshared scalar: flagWasSet lived at -0x3f, creating a nine-byte bool group while the retail flag shares the four-byte group at -0x28..-0x25. Native also assigns it AFTER IsFalse(params[1]), before the map-call branch. Moving the assignment into EACH branch (`setflag_branch_assignment`) restores the observed store timing and the exact 0x18dc frame. Full size drops from 74955 to 74906 vs 74778; direct-call relocation symbol counts are unchanged. Its own flag slot is now -0x25 vs native -0x27, so ordering within the four-bool group is still a residue. Diagnostic loose hunks 292 -> 289. Native's shared CUTSCENE_FRAME tail remains in a different location.

Action `photo_caption_named_text`: the native virtual GetText return is followed by an explicit lea of the string before OnCaptionEntered. Naming the CWideString local restores that missing six-byte lea; size grows 35696 -> 35702 for the right local reason, loose hunks 108 -> 107. No new full match.

SlideTeleport follow-up hypothesis: name only the scaled offset vector, preserving both original int-to-float conversions. Native spills the Y product but keeps X/Z; previous named-factor/position probes changed too many temporary identities together. This is a manually reasoned single source-shape attempt.
