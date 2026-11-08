# Chunk 865 -- land the inline-budget parks under the owner's 2026-11-08 decision (overrides PROMPT.md where they differ)

## The main goal (owner, verbatim, 2026-11-08)
"Remember the main goal is readable clean c++ that is portable and byte matches retail code and behaves the same as retail."
And on inline controls: "as long as it does not affect portability, matches retail bytes, behaves the same as retail, and is clean
c++. then whatever you need to do, you can do it". Full rule: tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md,
section "Owner decision, 2026-11-08" -- read it first.

## What these rows are
Twenty parked rows whose remaining residue is an INLINE decision of VC7.1: retail expands an inline callee (a ~CScriptThing,
~CIsThingConscious, a filter / predicate special member, ~CStringParser, ~CTimer, an STLport range initialise, ...) at some
call sites and calls it out of line at others, or the inline depth / caller-size cap differs. Ten of them were already shown to
grade RELOCATION_MATCH under a diagnostic knob (column 4). They were parked only because the knob was banned. It is now allowed.

## How to land each row (preference order -- readability and portability first)
1. Plain C++ (EXTRA_RULES 2026-11-03 note) if a retail-plausible spelling matches -- but do not spend more than ~10 grades
   here on rows already proven under a knob.
2. **Per-row flag, source untouched** (preferred device): grade with `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/g.sh <addr> --flags /d2inlS<N>` or `/d2inlT<N>` and put
   `"flags": ["/d2inlS<N>"]` in the payload entry. Find the SMALLEST/closest-to-default N that matches by bisection (record the
   range that matches; default limits: probe neighbours, e.g. S15..S25, T30..T80). Also check `/Ob1` / `/Ob2` if they help.
   The source stays plain, readable C++ with no device in it.
3. Only if no flag value matches: a source-level control around the smallest region that needs it -- `#pragma inline_depth(0)`
   ... `#pragma inline_depth()`, `#pragma auto_inline(off)` ... `(on)`, or `__declspec(noinline)` / `__forceinline` on a
   row-local helper (wrap a declspec in `#ifdef _MSC_VER` so other compilers see plain C++). Never a second class model
   (two-view CScriptThing stays banned), never asm / naked / _emit / volatile.
4. Line 2 of the source: `// Inline-budget: <device, e.g. flag /d2inlS20> -- <which callee / which sites; match range; evidence>`.
   Keep the `// Flags:` header line consistent with the payload flags.
5. Every landed row needs a behaviour test that PASSes (`/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/tall.sh` / `tall.py`); rows without a test in start/ need one written.
6. Readability pass before handing back: real names (PDB spelling: grep ghidra_out/struct_layouts_egor.tsv / fable_types.h and
   the labels), shared headers `engine/<Class>.h` (never a local pad struct), no dead locals, comments that say what the game does.
   Fix a stale catalog name in the row list (e.g. `Script_Villager_Reputation_Dialog`, `Quest009_...`) to the PDB name and report it
   in NAME_FIXES.tsv.
7. A flag must not be a TU-wide change for other rows: it is per row (catalog CompilerFlags), so no TU check is needed, but say in
   NOTES.md which rows in the same retail TU share the same N (the coordinator may want one N per TU).

## Rows
| Address | Function (catalog) | Bytes | Known knob / residue | Earlier pending dirs (rebuild/candidates/manual/pending/) |
|---|---|---|---|---|
| 0x00CF18D0 | NScript::CQ_ArenaHoldingScriptScript::COakvaleFan::Main | 4539 | /d2inlS20 (bow also /d2inlS15) | 2026-11-06_linux_hand804 |
| 0x00CF2B70 | NScript::CQ_ArenaHoldingScriptScript::CBowerstoneFan::Main | 4298 | /d2inlS20 | 2026-11-06_linux_hand804 |
| 0x00CF3D20 | NScript::CQ_ArenaHoldingScriptScript::CHookCoastFan::Main | 4298 | /d2inlS20 | 2026-11-06_linux_hand804 |
| 0x00DAC760 | NScript::CQ_NewOakValeIntroScript::CNOVI_Guard::Main | 5867 | /d2inlT72 | 2026-11-06_linux_hand809, 2026-11-06_linux_hand822 |
| 0x00E8EF80 | NScript::CV_GhostGrannyNecklaceScript::CGhostGrannySon::Main | 5996 | /d2inlT72 | 2026-11-06_linux_hand809, 2026-11-07_linux_hand826 |
| 0x00D17F20 | NScript::CQ_BountyHuntScript::CBH_GangDeputy1::Main | 3068 | /d2inlS15 | 2026-11-02_codex_pass13, 2026-11-07_linux_hand833 |
| 0x00D19BD0 | NScript::CQ_BountyHuntScript::CBH_Hostage1::Main | 2918 | /d2inlS15 | 2026-11-02_codex_pass13, 2026-11-07_linux_hand833 |
| 0x00EF4B40 | NScript::CPersonalScript_EmptyScript::CTestPerson::Main | 1268 | /d2inlT30 | 2026-11-07_linux_hand837, 2026-11-08_linux_hand856 |
| 0x00D22F30 | NScript::CQ_BreakSiegeScript::CKG_Sheriff::Main | 3399 | /d2inlS15 | 2026-11-02_codex_pass13, 2026-11-07_linux_hand843 |
| 0x00E799F0 | NScript::CV_ExposeMayorScript::CEM_Rhodri::Main | 2580 | inline_depth(0) round the row-local ~CIsThingConscious | 2026-11-02_codex_pass13, 2026-11-07_linux_hand836, 2026-11-08_linux_hand856 |
| 0x00531EC0 | CGuiComponent::Update | 3642 | a /d2inlT52 split (hand862) | 2026-11-08_linux_hand862 |
| 0x00DAADD0 | _global::Script_Villager_Reputation_Dialog | 3306 | pure inline-budget split (hand862) | 2026-11-08_linux_hand862 |
| 0x00D18DF0 | NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Main | 3321 | /d2inlS15 (hand859) | 2026-11-05_linux_hand797, 2026-11-06_linux_hand822, 2026-11-07_linux_hand832, 2026-11-08_linux_hand859 |
| 0x00D067A0 | _global::Quest009_Gamesmaster_ResidentialBanditCamp_Setup | 2628 | "same source is RELOCATION_MATCH" under a knob (hand852) | 2026-11-08_linux_hand852 |
| 0x004CE1B0 | CSubtitledMessage::AddMasterShotCameraTagFromText | 816 | documented inline split; no knob probe yet | 2026-10-23_linux_hand299, 2026-10-30_parked_retry2_sec1b |
| 0x0083F650 | _global::ProcessTextWithRegex | 442 | documented inline split; no knob probe yet | 2026-10-24_linux_hand480 |
| 0x00E03F70 | CMagicBarrier::Main | 1920 | documented inline split; no knob probe yet | 2026-10-23_linux_hand319, 2026-10-31_parked_retry2_sec6 |
| 0x00DC08C0 | CQ_OakValeBanditRaidScript::WatchForGateTimer | 1830 | documented inline split; no knob probe yet | 2026-10-23_linux_hand322, 2026-10-31_parked_retry2_sec6 |
| 0x00E2EAC0 | CV_AmbushScamScript::TricksterApproach | 368 | documented inline split; no knob probe yet | 2026-10-21_linux_hand256, 2026-10-31_parked_retry2_sec6, 2026-10-31_parked_retry2_sec6b |
| 0x00CDDCB0 | CPersonalScriptMainScript::Main | 1488 | documented inline split; no knob probe yet | 2026-10-23_linux_hand321, 2026-10-31_parked_retry2_sec6b |

`/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/start/<addr>.cpp` (+ `.test.cpp` where one existed) is the newest earlier source. Earlier dirs hold their STATUS / NOTES /
LESSONS and header overlays: copy only the headers a row needs into `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/hdr/` (diff against rebuild/include first; keep opt-ins
`#ifdef FABLE_RETAIL_*`, default must stay the same) and set FABLE_HDR_OVERLAY as the earlier logs did. Read each row's log before
the first grade; never repeat a logged spelling.

## Budget
Up to 100 grades per row; keep going on every row until it matches or you can say exactly why no device reaches it.
Goal: at least 14 of 20. Never grade one address twice at once; at most 2 concurrent compile jobs.
Log every grade in `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/ATTEMPTS.log` (addr, device/spelling, grade, diff lines) and keep `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/STATUS.tsv` current.
No stray files: never redirect output to a path outside /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865 (an earlier agent left `1.0f` in the repo root).

## Research rules (shared block)
## Research rules (2026-11-08; read docs/pipeline/VC71_CODEGEN_RESEARCH.md sections 2, 4-7 and docs/pipeline/C2_ALLOCATOR_PROBE.md in full)
These are proven on lane probes. Use them BEFORE spending grades on blind spellings:
- **Read the allocator, don't guess it.** For any register difference run
  `python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py <src.cpp> --trace` (25-50 s; `--func` / `--flags` default from
  `// Leaf:` / `// Flags:`; set FABLE_HDR_OVERLAY as for a grade; `--rotation` for eax/ecx/edx temporaries, `--blocks` for per-block
  priority shares). A probe run counts as one of your 2 concurrent jobs.
- **Callee-saved roles (esi/edi/ebx/ebp)** = priority colouring in table order eax,ecx,edx,esi,edi,ebx,ebp: priority = sum over blocks
  of w*K*2 per reference minus w*K per live-through block (w = 1<<loop depth, K = candidates referenced in the block); ties go to the
  larger tie key (last writing tuple); a copy relation gives a -1..-3 preference for the copied value's register. Move a role by
  adding/removing ONE real reference, changing which value is defined first, or breaking / creating a copy relation (`--trace` shows it).
- **Scratch rotation (eax/ecx/edx)** comes from a per-function rotating pointer, not the allocator: add or remove exactly one expression
  temporary BEFORE the site (a named value local, an inline accessor at one site, a folded cast); `--rotation` shows the pointer.
  A named local that becomes a callee-saved candidate is not a temporary (it moves other things).
- **Address-taken locals have no candidate** (they live in memory): to get a value into a register, pass a COPY by reference
  (`const T key = v; map[key]`) so the original is never address-exposed.
- **Zero in a register:** 3 zero stores -> `xor` scratch, 5 with calls -> `xor` callee-saved; cross the threshold by writing stores
  per arm or merging them.
- **Frame-slot sharing / dead stores / reloads:** give each callee that receives a local's address a same-TU body under
  `#pragma auto_inline(off)` that only READS through the pointer (landed bodies where they exist); a writing or declared-only callee
  keeps the slot and the stores (the reverse lever reproduces a retail dead store). Store->load forwarding of a member needs the same
  base expression on both sides (store through `T* p = &m;`, read through the member, or the reverse, to block it).
- **x87 order:** with parameter/member bases the larger displacement is loaded first (textual order irrelevant), equal -> earlier
  parameter; direct operands (globals, by-value, frame) keep textual order (right operand first) and beat indirect ones; sums of
  products are re-sorted regardless of parentheses / named locals / casts and pinned only by a store or a call; `float& r = x; f(r)`
  gives push-then-lea; naming a float call result before adding it lets an unrelated integer load schedule ahead of the `fadd`.
- **Flags:** /G5 = /G6 = /GB = default and retail is not /G7 -- never spend grades on them; a flag that matches one row must not break
  the TU's landed rows (`rebuild/candidates/manual/pending/2026-11-08_research861/tools/tu_flag_check.py <lo> <hi> <flag>`).
- **Pads / block layout:** an alignment-pad difference always follows an earlier byte difference: fix the first byte first.
- c2 is function-local: declarations, path length and neighbouring functions do not change a function's bytes (only visible bodies of
  callees do).



## Output
As PROMPT.md: `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/payload.json` (with `flags` per entry), `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/oracle.tsv`, `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/differ/`, `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/NOTES.md`, `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/LESSONS.md`, `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent865/NAME_FIXES.tsv`.
Final report: matched / differ counts, the device and N per row, and anything the coordinator must know.
