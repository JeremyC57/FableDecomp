# Chunk 893 -- playbook family push: shared tails / cross-jumping and block placement (17 parked rows) (overrides PROMPT.md where they differ)

Owner, 2026-11-09: "do what you think is best. if you think 1 agent is good for these, one family at a time, then go for it".
This is the second family push (the first, chunk 892 scheduling / test-or, landed 24 of 27). These rows' hand-backs say the residue is
a shared tail (cross-jump survivor), the placement of an exit / epilogue / guard block, or tail duplication vs merging -- often with a
register residue riding on it. FAMILY.tsv: address, family, retail bytes, old grade, hand-back dir, source, note. Every row has its old
source and notes in prior/<addr>/ (from rebuild/candidates/manual/pending/<handback>/). Most were parked in October, before the c2 probes.

Order of work:
1. Baseline: regrade every prior source as-is (one at a time, `timeout 240`). Shared headers gained many opt-ins since October -- some
   rows may now match or have moved; record each in ATTEMPTS.log and STATUS.tsv. Copy a matching source to src/ and write its test.
2. Then per row, smallest residue first. What is already measured (MATCHING_PLAYBOOK section 2 "Cross-jumping"):
   - c2 orders exit edges as `break`s in source order, then `return`s, then edges into `default:`; the LAST edge with a matching tail
     keeps the tail and the others jump to it. Labels are a dead lever. Moving a tail = changing the edges (break v return v continue,
     case order, one shared call after a switch v one per case, if/else chain v switch), not wrapping code (a macro and an inline
     function give the same IL -- RunCutsceneMacro h01).
   - Block placement (chunk 892, 0x005FD5E0): an if/else chain inside a loop only ever produced the source-order or then-arm-first
     layout; only a block ending in a real `return` was ever moved past the epilogue. So for "exit block after the epilogue" rows try
     the early-`return` / guard-clause spellings a programmer would write, and the inverse.
   - Tail duplication v merging and loop rotation (0x005A4CDD Sell): retail duplicates/threads the loop head test where ours
     cross-jumps -- top-tested `while` v `for` v `do`/`while` with a guard, and where the test reads its operands, decide it.
   Find c2's block-ordering / cross-jump pass in c2.dll if the playbook rules do not settle a row (hook it with c2dbg.py; agent 892's
   copy is in ../agent892/tools/c2dbg.py), and add what you learn to LESSONS.md with numbers.
3. Rows that share a pattern (same note wording, same hand-back, e.g. the hand320 / hand295 pairs, the hand324 PC twin) get solved
   together: one finding, applied to all.
Behaviour tests: each prior hand-back may already have one (prior/<addr>/); otherwise write a C++03 test per matched row.

Hand-authoring (owner, 2026-11-09: "force ... real hand authoring based on its findings. i want to see actually grading"): one-token
generator batches are a probe, not authoring. Most of your time goes to hand-written region rewrites (Edit/Write on a copy of the best
source, tens of lines, aimed at the blocks the byte diff names), each graded with the real grader (g.sh / q.py) and logged.

## The all-means push (owner, 2026-11-09, verbatim): "why cant we fix them. We need to find any means possible to get them to match bytes, behavior, without compromising portability. Do anything you need to do to get a match within our rules."

Premise: VC7.1 13.10.3077 is deterministic and retail's bytes came out of it from C++ source (Rich header). So some source + header
set reproduces every register and every frame slot. "Nothing in our source can push it" is a statement about the spellings tried so
far, never a conclusion. Your job is to find what in retail's source differs, with the compiler's own decision procedure as the
oracle, not by guessing.

Read docs/pipeline/MATCHING_PLAYBOOK.md first: it is this method with every c2 rule measured so far (Action 0x0041E6D3 matched with it).
Method (in this order; log every step in TRACE.md, every grade in ATTEMPTS.log):
1. Quantify the first wrong decision. docs/pipeline/C2_ALLOCATOR_PROBE.md section 4 gives the exact formulas: priority = sum over
   blocks of w(b)*K(b)*cost_b minus live-through w(b)*K(b); order = priority, then key (last defining tuple number); choice =
   COSTS + copy-relation preferences (-1/-2/-3) + interference penalties, ties to eax,ecx,edx,esi,edi,ebx,ebp. For the first
   wrong register (and each later one), write down ours: each candidate's priority, key, allowed set, costs (c2prio --blocks
   --trace, pass --func <undecorated name>). Then write what retail's outcome REQUIRES (e.g. "game coloured before the action
   temp: prio(game) > 1015, or temp gone, or a -1 copy relation pulling it to edi"). That inequality is the search target.
2. List every source lever that moves those numbers and test them cheaply (c2prio / compile-only, not a full grade):
   references in heavy blocks (w, K), splitting or merging webs (a named local vs a CSE temp, `CAction* a = action.Ptr;`,
   `(*p).` vs `p->`, reading through a reference vs a copy), address exposure (an address that escapes removes a candidate),
   copy relations (a value assigned from / to another candidate or a fixed register), definition order inside a block (key ties),
   switch-case and statement order (block order), scope blocks (live ranges), inline expansion counts (an inlined accessor adds
   tuples and references), the header bodies retail compiled (a FABLE_RETAIL_* opt-in with a heavier or lighter but still
   retail-logic inline body changes CSE and tuple counts), and same-TU neighbour bodies (retail's TU had other functions; a
   visible callee body changes what c2 assumes a call clobbers).
3. Automate. Write a variant generator (one lever per variant) and a fast fitness that needs no full grade: c2prio's register for
   the first N candidates in retail order, the frame map (slot per local v retail's), the first-diverging-instruction index. Run
   variants serially (one compile at a time for you), keep a leaderboard, full-grade only a variant that improves the fitness.
   Combine winning levers. No attempt cap: keep searching until it matches.
4. Frame slots: the packer is not yet traced by the probe. Agent 890 session 2 read it from c2.dll (smallest size first, then
   reference count, most recently opened slot that fits; ../agent890/work/pk/, ../agent890/WORKLOG.md "Session 2") and agent887's
   packer tools are in ../agent890/work/pk/. Find the real tie key (what "ties shuffled" actually is: read the comparison in
   c2.dll and hook it with c2dbg.py), add a `--frame` trace to your copy of c2prio, and use it as a fitness term.
5. Each lever that flips a decision gets a GOTCHAS-ready line in LESSONS.md (retail address, the spelling, the numbers).

Rules (unchanged, and they are the whole limit):
- Byte match + behaviour + portability. The final source is readable C++03 that also compiles with g++/clang.
- No __asm / naked / _emit / volatile. No `register` keyword (removed in C++17).
- No compiler-specific register or allocation devices: none exist for VC7.1 anyway; use source shapes.
- Inline devices only per the owner's ladder: a per-row /d2inlS|T flag, a scoped `#pragma inline_depth`, a FABLE_FORCEINLINE body
  twin. Each one gets a comment naming the retail address, and the row gets a line-2 `// Inline-budget:` note.
- File-level `#pragma optimize` only with retail prologue evidence.
- FABLE_FORCEINLINE, never a bare __forceinline. MSVC-only spellings go inside #ifdef _MSC_VER.
- Header changes go only in your hdr/ overlay, behind FABLE_RETAIL_* opt-ins, with the default path byte-identical, documented in
  hdr/README.md.
- If a search finds an odd spelling, look for the natural C++ with the same IL before keeping it, and comment what it fixes.
- Exact PDB names. Never leaked source.
- Never run git (not even status). Never write inside /home/user/FableDecomp. Never run chunk_pool/check_extents.py. Never kill
  processes you did not start. Use timeouts. Grade only your own row, one compile at a time.
- Tool improvements stay in your dir's tools/ with a README line; the coordinator lands them.
On a match: C++03 behaviour test in test/, finish.py, tall.sh / tall.py PASS, report. Otherwise checkpoint WORKLOG.md (a new
session section) every few hours of work with the leaderboard and the next lever, and report only when the levers are truly
exhausted, with the measured reason for each.

## The main goal (owner, verbatim, 2026-11-08)
"Remember the main goal is readable clean c++ that is portable and byte matches retail code and behaves the same as retail."
Every landed row: MATCH / RELOCATION_MATCH under vland, a behaviour test that PASSes, real names in the exact PDB spelling
(ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv, struct_layouts_egor.tsv, fable_types.h; two sources), shared headers
`engine/<Class>.h` / `stlport/*.h` (never a local pad struct or a local copy of the STL; add an additive `#ifdef FABLE_RETAIL_*`
opt-in in your hdr/ overlay instead, default path unchanged, diff against rebuild/include), the full mangled `// Leaf:` line (never a
prefix) and the same `leaf` in the payload, comments that say what the game does. No `__asm` / `naked` / `_emit` / `volatile`.
A header body you add must be retail's own logic (a stand-in that does something else fails "behaves the same").

## Inline-budget residues (owner decision 2026-11-08; EXTRA_RULES "Owner decision, 2026-11-08")
If the ONLY residue is an inline decision (an inline callee expanded at some sites and called at others, inline depth, the caller-size
cap): (1) plain C++; (2) a per-row flag with the source untouched -- payload `"flags": ["/d2inlS<N>"]` or `/d2inlT<N>` (bisect, smallest
N; /d2inlS often cliffs at 15, default S9; T default 74), plus a matching `// Flags:` line; (3) only if no flag reaches it, a scoped
`#pragma inline_depth(0)` ... `()` / `auto_inline` / `_MSC_VER`-guarded `__forceinline`. Line 2: `// Inline-budget: <device> -- <evidence>`.
Never for register-choice / block-layout / x87-order residues.

## Budget and conduct
Up to 150 grades per row (huge budget, owner): keep working EVERY row until it matches or you can name, with probe evidence, the
compiler quantity no allowed spelling moves. Do not stop early: a row with fewer than 50 real source attempts is not finished unless it
matched. Diagnose first (c2prio.py, the call/ret sequence, the first differing byte), then spell. Log every grade in ATTEMPTS.log and keep
STATUS.tsv current. At most 2 concurrent compile jobs; never grade one address twice at once (q.py shares one work dir per address).
Never commit, never write under /home/user/FableDecomp (scratch only, no stray redirect files), no git commands.

## Recent lessons that paid (chunks 870 / 871, read them in GOTCHAS too)
- Wrap every grade in `timeout 240`: a cl that spins (usually a header overlay missing a sibling the overlay copy includes by bare
  name; MSVC searches the includer's directory first) then costs one attempt instead of stalling you.
- Catalog extents here are next-manifest-start gaps and often hold several functions: find the real end (`ret` + 0xCC padding) with
  xdis before chasing a size difference, grade against the true length, and list every extra start in NOTES.md.
- A `// Retail-asm-helper:` / `// Retail-asm:` tag must carry ALL its names / its VA + mnemonic on the tagged line itself.
- Same-TU `#pragma auto_inline(off)` bodies of a holder's accessors fix reloads (mod/ref); a private-convention static callee needs
  its real body in the TU; a two-return inline helper realigns a scratch-register rotation (c2prio --rotation).

## Rows (oracle `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/batch/chunk893_oracle.tsv`)
| Address | Name | Bytes |
|---|---|---|
| 0x00839940 | CThingGameObject::OnHit | 5195 |
| 0x005A4CDD | CTCInventoryTrade::ConstructActualSellInventoryPC | 3668 |
| 0x00D39960 | NScript::CQ_FireHeartScript::StartRound | 1774 |
| 0x00EB40C0 | CMurderTwistAssassin::DoWatchingForTrouble | 1601 |
| 0x00589402 | CTCInventory::GetStyleCardsListUIComponent | 1582 |
| 0x00EB5750 | CMurderTwistAssassinGuard::DoAskingHeroToSpeakToBossAgain | 1536 |
| 0x00683C10 | CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex | 1085 |
| 0x00922010 | CAIStateGroup_MinionWalkToMarker::Update | 651 |
| 0x00D8A240 | CQ_HerosOldHouseScript::WatchForBooty | 610 |
| 0x00D6C830 | CQ_HangingTreeEvilScript::CheckBanditFollowStatus | 607 |
| 0x00E36750 | NScript::CV_ArcheryCompetitionScript::DoCountDown | 536 |
| 0x00896B60 | CGameScriptInterface::KickOffQuestStartScreen | 445 |
| 0x005F6793 | CTCInventoryAbilities::CycleAssignedExpressions | 442 |
| 0x009D06F0 | CBankFile::OpenReadOnly | 385 |
| 0x00C11230 | CSoundEventManager::CanStartEvent | 242 |
| 0x0085FBC0 | NCreatureMode::IsAbleToBeEngagedInCombat | 142 |
| 0x00C73830 | CRegistryKey::QueryDWord | 72 |

## Research rules (2026-11-08; read docs/pipeline/VC71_CODEGEN_RESEARCH.md sections 2, 4-7 and docs/pipeline/C2_ALLOCATOR_PROBE.md in full)
- Before parking any row, read docs/pipeline/MATCHING_PLAYBOOK.md and apply it: a residue that looks like register / slot /
  scheduling noise is a c2 decision to measure (c2prio, frame packer, inline trace), never a park reason by itself.
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
As PROMPT.md: `payload.json` (matched rows only; `flags` per entry where used), `oracle.tsv`, `differ/<addr>.cpp` for the rest (line 2 = residue
+ probe quantity, line 3 = next idea), `NOTES.md`, `LESSONS.md` (GOTCHAS one-liners), `NAME_FIXES.tsv` (landed spellings you prove wrong),
`LABELS.tsv` (address, PDB name, two sources), `hdr/README.md` for every header change. Run finish.py last. Final report: matched / parked
counts, per-row device, header changes, names, and anything the coordinator must know.

## Tooling lessons from chunk 892 (they cost grades)
- gall.sh uses your hdr/ overlay only when FABLE_HDR_OVERLAY is exported; export it for every grade once you have an overlay.
- q.py / g.sh ignore the `// Leaf:` line: with no leaf argument they grade the largest symbol matching the catalog name (that graded
  the wrong overload of 0x008E8FF0). Pass the leaf to g.sh.
- Start hdr/ copies from the CURRENT rebuild/include (24 headers changed at landing 2026-11-09e).
