# Fable 5.1 landing queue (wave 25 onward)

**Owner's rule (2026-10-15):** while the session runs on Opus 5.5, as many sub-agents as the lane allows (16 at once)
hand-author the class-labelled chunk pool. They grade with the lane compiler while authoring, but the **final compiler
check, the landing, and every diff that keeps failing wait for a Fable 5.1 session.** This file is that session's
complete to-do list. Nothing listed here is in the catalog yet. The generated table at the bottom is rewritten by
`python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/queue_status.py` after every staging or landing.

Where the work is:
- **Staged batches** live in `rebuild/candidates/manual/pending/<name>/` (committed, so they survive a container reset):
  `payload.json` (sources + tests of the rows the agent graded MATCH/RELOCATION_MATCH), `oracle.tsv` (those rows'
  retail bytes, the same per-function rows a landed batch commits), `src/`, `test/`, `differ/` (best source of each
  parked row), `NOTES.md` (becomes the batch README), `STATUS.tsv` (one line per chunk row), `rows.tsv` (address,
  class, name, length of all 40 rows; no bytes).
- The agents' full working dirs are `$S/w22/agentNN/` (scratch, lost on a container reset; nothing in them is needed
  that is not staged).
- Scripts: `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/` -- `stage_pending.sh` (Opus side),
  `land_pending.sh` (Fable side), `oracle_from_exe.py`, `queue_status.py`, `mkprompt25.py` + `agent_scripts/` +
  `brief_tools_rules.md` (how the fleet's prompts and helper scripts were built).

## Step 0 -- bring the lane up (Fable)
1. Same container: `export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad;
   source $S/wine/env.sh; export S=...; cd /home/user/FableDecomp`, then prime Wine once:
   `timeout 120 wine cmd /c exit >/dev/null 2>&1 </dev/null` (a cold prefix makes the first grade hang for minutes).
   Prove it: `python3 $S/tools/q.py 00415b80 rebuild/src/compiled/00/41/SNavigationLayerDefaults00415b80_SNavigationLayerDefaults00415b80_00415b80.cpp` -> MATCH.
2. Fresh container: follow `HANDOFF_FOR_NEXT_SESSION.md` section 4 (game, DDK compiler, Wine prefix, scratch tools),
   then for each pending batch whose parked rows you will retry:
   `python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/oracle_from_exe.py rebuild/candidates/manual/pending/<name>/rows.tsv <name>`
   (writes `$S/batch/<name>_oracle.tsv` so `q.py` can grade those addresses; never commit that file).
3. `git pull origin FullDecomp` first: the Opus session keeps pushing staged batches.

## Step 1 -- final compiler check + landing of every STAGED batch (Fable)
For each table row whose state is `STAGED`, oldest first, one at a time (the lander is sequential):
```
bash tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/land_pending.sh <name>
```
`verify_and_land.py --land` inside it is the final check: it recompiles every source with the retail flags (plus the
pragma/flag sweep), compares bytes, compiles and runs every behaviour test with cl.exe under Wine, rejects
`__asm`/`naked`/`_emit`, and only then copies winners into `rebuild/src/compiled/`, `rebuild/tests/`, the catalog
and the oracle. Then:
- Compare `WINS: N` with the table's "matched" count. A shortfall is printed as `DIFFER`/`FAIL` lines: copy each
  refused source from `rebuild/candidates/manual/batches/<name>/` into `pending/<name>/differ/<addr>.cpp`, add a
  hand-over line for it under that batch's "Parked" section in `pending/<name>/NOTES.md`, and fix the batch README
  (`batches/<name>/README.md`) count line.
- The script trims `pending/<name>/` to `differ/`, `rows.tsv`, `NOTES.md`, `STATUS.tsv` (the open work).
- Spot-check 2-3 landed sources per batch for header comments, real member names and the shared engine headers
  (CLAUDE.md "Shared headers"); fix in place if needed (re-run the lander on that batch dir to re-grade).
After all staged batches: `python3 tools/decomp_pipeline/families/funclets.py . <date>_linux_funclets38` (land that dir
the same way if it found anything), then `python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/queue_status.py`.

## Step 2 -- the hard diffs (Fable)
Every parked row below carries the authoring agent's hand-over line: `DIFFER(ours v retail): what differs | tried:
forms tried | next: idea or "lane gap"`. For each one:
1. Grade the parked source: `timeout 200 python3 $S/tools/q.py <addr> rebuild/candidates/manual/pending/<name>/differ/<addr>.cpp [leaf] [--flags ...]`
   and `python3 $S/w18/udiff.py <(...) | grep -v "^ "` for the differing lines. (Needs the oracle rows of step 0.2
   on a fresh container; on the same container `$S/batch/chunkNN_oracle.tsv` already has them.)
2. At most 4 attempts per row (the loop-prevention rule). Rows marked "lane gap" are known DDK-4035-vs-retail
   codegen gaps (GOTCHAS 24k-24p lane-gap lists): give them one attempt at most, then leave them for the Windows
   RTM/QFE re-grade (`docs/pipeline/QFE4035_COMPILER_GATE.md`).
3. A row that matches: write its behaviour test, put source + test into a new dir `$S/w22/fretryNN/{src,test}` with the
   helper scripts (`cp tools/.../agent_scripts/*` and replace `__S__`/`agentNN`/`chunkNN`), run `tall.py`, `finish.py`
   (it reads `$S/w22/chunkNN.tsv`; point it at `pending/<name>/rows.tsv` + the step-0.2 oracle if the chunk TSV is
   gone), stage it as `<date>_linux_fable_retryNN` with `stage_pending.sh` and land it with `land_pending.sh`.
4. Delete the fixed row's `differ/` source and hand-over line; when a batch's `differ/` is empty remove its pending dir.

**Retry hint from chunk 75:** many rows parked as the "24i" lane gap (a member `lea` / `mov ecx,edi` placed before or
after the zero stores, argument stores before the vtable store) are MODEL errors, not compiler gaps: re-author them with
the STLport `_Vector_base` + `_STLP_alloc_proxy` model and a `__declspec(novtable)` intermediate base owning the argument
pair (see `pending/2026-10-15_linux_hand75/LESSONS.md`) before giving up on them.

## Step 3 -- docs after each landed part (Fable)
GOTCHAS: one `- Wave 25x (...)` line built from the batches' NOTES "What matched" lessons and each pending dir's `LESSONS.md` (the agent's final-report lessons plus any review item, e.g. a local base model next to an existing engine header -- check those before accepting the batch); journal
`docs/journal/2026-10/LINUX_CONTAINER_LANE_2026-10-13.md` (new part section above the previous one); `docs/HANDOFF.md`
and `docs/ROADMAP.md` counts; `python3 tools/write_decomp_dashboard.py && python3 tools/update_readme_progress.py`;
commit `land: ...` as BuffJesus with the session's Co-Authored-By / Claude-Session lines; `git push -u origin FullDecomp`.

## Skipped rows -- the permanent ledger (any session, any agent; no Fable needed to author them)
A row an agent could not author as ONE function is logged in `rebuild/candidates/manual/SKIPPED_ROWS.tsv` (address, row
length, source batch, original label, kind, status, pieces `addr:len,...`, note). `stage_pending.sh` adds every `SKIP`
line of an agent's STATUS.tsv automatically; `land_pending.sh` runs `skipped_ledger.py refresh`, which closes a row once
every piece is in the catalog. The chunk cutter never re-offers a used row, so this ledger is the only record. Kinds:
- `multi` -- several functions in one row (pieces found by `split_rows.py` from the exe: padding and terminators).
  Author each piece like a normal row.
- `prefix` -- the row is only the first bytes of a longer function (the piece length is the whole function, often
  2 KB+). Author the whole function (a hand EH-style pass, not the 17-96-byte chunk brief).
- `cold` -- a block that belongs to another function (pieces = the parent `addr:0`); it is covered when the parent lands.
- `ltcg` -- a function using link-time register conventions (a callee assumed to preserve ecx, the object passed in esi);
  one translation unit cannot reproduce it on the lane: leave for the Windows box (`/GL` + `/LTCG`) or keep as an oracle.
To work the open `multi` pieces (and `prefix` ones up to `--max-len`):
```
python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/skipped_ledger.py to-chunk 901      # writes $S/w22/chunk901.tsv + oracle, marks rows in-chunk:901
python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/mkprompt25.py 901                    # $S/w22/agent901/ + PROMPT.md
```
then give `$S/w22/agent901/PROMPT.md` to an agent, stage it (`stage_pending.sh agent901 <date>_linux_skipped901`) and land
it like any other batch. Use chunk numbers 901+ for ledger chunks (the pool cutter uses 00-99).

## Also open from the Fable sessions (optional retries)
Parked rows of the Fable-authored chunks 59 and 60 are listed with evidence in
`rebuild/candidates/manual/batches/2026-10-14_linux_hand59/README.md` and `..._hand60/README.md` (sources in
`$S/w22/agent59/parked/`, `$S/w22/agent60/parked/`; scratch only).

## Paused agent chunks (owner's pause, 2026-10-15) -- scratch only, NOT staged
The owner paused the fleet. Ten chunks stopped part-way; their work is only in `$S/w22/agentNN/` (lost on a container reset).
Each has `PROGRESS.md` (per-row state, attempt counts, next steps). To resume: SendMessage the agent (or give a fresh agent
its PROMPT.md plus "resume from PROGRESS.md"), then stage it with `stage_pending.sh` when finish.py has run.

| chunk | matched | parked | skipped | left to do |
|---|---|---|---|---|
| 80 | 35 graded + 5 matched in probes | 0 | 0 | grade 2, write 3 sources, 38 tests, gall/tall, STATUS/NOTES, finish |
| 83 | 32 | 8 | 0 | fix two header lengths, 32 tests, gall/tall, STATUS/NOTES/LESSONS, finish |
| 84 | 36 | 3 | 1 | 24 tests, gall/tall, STATUS/NOTES/LESSONS, finish |
| 85 | 27 | 8 | 0 | 1 row analysed, 4 not started, all tests, finish |
| 86 | 34 (tests pass) | 4 | 2 | re-grade 3 re-signatured sources with absolute paths, update 3 tests, STATUS/NOTES/LESSONS, finish |
| 87 | 36 | 4 | 0 | rename 4 methods + regrade, move 2 parked sources out of src/, 31 tests, finish |
| 88 | 28 | 3 | 0 | 1 in progress, 8 not started, all tests, finish |
| 89 | 34 (in probe/ only) | 6 | 0 | write the 34 final sources into src/, regrade 006ca040 / 007b29b0, all tests, finish |
| 90 | 32 | 4 | 0 | 0057adb8 (1 attempt left; its src/ file does NOT match yet), 3 rows, all tests, finish |
| 91 | 19 (tests pass) | 1 | 1 | 19 rows not started, finish |

Chunks 92-108 are cut and prepared (`$S/w22/agentNN/PROMPT.md`) but were never launched.

## Generated status
<!-- queue:begin -->
| pending batch | chunk | matched (to land) | parked | skipped | state |
|---|---|---|---|---|---|
| `2026-10-14_linux_hand58` | Hand chunk 58 (CMessageEventFilter_IsOriginatedByThing .. CMouseCursor): 34 of 40 byte-exact | - | 6 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand61` | Hand chunk 61 (CNavigatorQueueManager .. COpinionReactionMaskDef): 35 of 40 byte-exact | - | 5 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand62` | Hand chunk 62 (COpinionSourceDef .. CParentDefClassBase): 37 of 40 byte-exact | - | 3 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand63` | Hand chunk 63 (CParentDefClassBase .. CParticleEmitter): 40 of 40 byte-exact | - | 0 | 0 | done |
| `2026-10-15_linux_hand64` | Hand chunk 64 (CParticleEmitterDatabase .. CPatchTesselationEdgeStrip): 38 of 40 byte-exact | - | 2 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand65` | Hand chunk 65 (CPatchTesselationEdgeStrip .. CPersistContext): 35 of 40 byte-exact | - | 5 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand66` | Hand chunk 66 (CPersistContext .. CPhysicsForceModelGotoOutsideSimple): 36 of 40 byte-exact | - | 3 | 1 | landed; parked rows open |
| `2026-10-15_linux_hand67` | Hand chunk 67 (CPhysicsForceModelGotoOutsideSimple .. CPlayer): 37 of 40 byte-exact | - | 3 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand68` | Hand chunk 68 (CPlayer .. CQ_HangingTreeGoodScript): 36 of 40 byte-exact | - | 4 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand69` | Hand chunk 69 (CQ_MinionCampScript .. CQuickAccessItemButtonGuiDef): 37 of 40 byte-exact | - | 3 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand70` | Hand chunk 70 (CRandomAppearanceMorph .. CRenderManager2D): 38 of 40 byte-exact | - | 2 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand71` | Hand chunk 71 (CRenderManagerCore .. CRightHandedSet): 37 of 40 byte-exact | - | 3 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand72` | Hand chunk 72 (CRightHandedSet .. CScriptGameResourceObjectMovieBase): 36 of 40 byte-exact | - | 4 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand73` | Hand chunk 73 (CScriptGameResourceObjectScriptedThing .. CScriptThing): 38 of 40 byte-exact | - | 1 | 1 | landed; parked rows open |
| `2026-10-15_linux_hand74` | Hand chunk 74 (CScriptThing .. CShaderRenderManager): 35 of 40 byte-exact | - | 0 | 5 | done |
| `2026-10-15_linux_hand75` | Hand chunk 75 (CShaderRenderManager .. CSkeletalMorphResourceManager): 34 of 40 byte-exact | - | 6 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand76` | Hand chunk 76 (CSmallVector .. CSpellContainerList): 33 of 40 byte-exact | - | 6 | 1 | landed; parked rows open |
| `2026-10-15_linux_hand77` | Hand chunk 77 (CSpellContainerList .. CSubtitleRenderer): 34 of 40 byte-exact | - | 6 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand78` | Hand chunk 78 (CSubtitledMessage .. CSystemManager): 38 of 40 byte-exact | - | 2 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand79` | Hand chunk 79 (CSystemManager .. CTCActionUseBuyHouse): 37 of 40 byte-exact | - | 2 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand80` | Hand chunk 80 (CTCActionUseBuyHouse .. CTCAssassinRush): 40 of 40 byte-exact | - | 0 | 0 | done |
| `2026-10-15_linux_hand81` | Hand chunk 81 (CTCAssassinRush .. CTCBoastingPodium): 36 of 40 byte-exact | - | 4 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand82` | Hand chunk 82 (CTCBoastingPosition .. CTCCameraPointScripted): 33 of 40 byte-exact | - | 7 | 0 | landed; parked rows open |
| `2026-10-15_linux_hand90` | Hand chunk 90 (CTCHeroExperience .. CTCHeroStats): 35 of 40 byte-exact | - | 5 | 0 | landed; parked rows open |

Totals: **0 matched rows staged for landing**, **82 parked rows** for the Fable retry pass.

### Skipped rows (19 open of 19 in the ledger `rebuild/candidates/manual/SKIPPED_ROWS.tsv`)
Rows an agent could not author as one function. Work them with `skipped_ledger.py to-chunk <NN>` (multi/prefix pieces become an ordinary chunk for any agent); see the "Skipped rows" section above.
| address | kind | status | pieces (addr:len) | from | note |
|---|---|---|---|---|---|
| `00455fa0` | multi | open | 00455fa0:8 00455fa8:12 00455fb4:22 | `2026-10-15_linux_hand66` | three functions with no padding (a jmp [eax+48h] thunk, a bit getter, a bit setter) |
| `00581ab3` | multi | open | 00581ab3:46 00581ae1:32 | `2026-10-15_linux_hand91` | two functions with no padding: vtable slot 59 at 0x581ab3 and slot 60 at 0x581ae1 (chunk 91, paused before staging) |
| `005c150b` | ltcg | open |  | `2026-10-14_linux_hand51` | CIsSameItemType::operator() calls CItem::IsSlotFull with ecx left over from the previous call (LTCG knew the callee preserves ecx): needs /G |
| `007312f0` | multi | open | 007312f0:5 00731300:3 | `2026-10-15_linux_hand86` | CTCDoor::vfunc_10 row holds two functions (chunk 86, paused before staging) |
| `0073a840` | multi | open | 0073a840:49 0073a880:18 | `2026-10-14_linux_hand59` | two functions after the int3 padding |
| `00750960` | multi | open | 00750960:5 00750970:3 | `2026-10-15_linux_hand84` | two functions: jmp [eax+24h], int3 padding, then mov al,1 ; ret (chunk 84, paused before staging) |
| `007fa26c` | cold | open | 007fa160:0 | `2026-10-15_linux_hand79` | not a function: reached by a je from inside the preceding function, uses its saved registers and ends with its ret 38h -- covered when that  |
| `0088b7e0` | multi | open | 0088b7e0:17 0088b800:4 | `2026-10-15_linux_hand74` | row spans two functions (0x88b7e0 + 0x88b800) |
| `008aae20` | multi | open | 008aae20:22 008aae40:27 008aae60:4 008aae70:4 | `2026-10-15_linux_hand74` | row spans four functions |
| `00991620` | multi | open | 00991620:24 00991640:36 | `2026-10-15_linux_hand86` | EditFrameUpdate row holds two functions (chunk 86, paused before staging) |
| `009cbd30` | multi | open | 009cbd30:20 009cbd50:19 009cbd70:20 | `2026-10-15_linux_hand73` | three AskHeroQuestion-style forwarders separated by int3 padding |
| `00a446fa` | cold | open | 00a446ce:0 | `2026-10-14_linux_hand49` | continuation of Catch@00a446f4 inside NScript_CIngredientOwner_RunThread 0x00A446CE (referenced only by an absolute address from the parent) |
| `00aacf90` | multi | open | 00aacf90:5 00aacfa0:4 | `2026-10-15_linux_hand76` | a jmp [eax+4] stub + `lea eax,[ecx+4] ; ret` after int3 padding |
| `00ba1140` | prefix | open | 00ba1140:2344 | `2026-10-14_linux_hand35` | bad family-scan row: the first 28 bytes of a ~2.3 KB function; author the whole function |
| `00bd71b0` | prefix | open | 00bd71b0:2116 | `2026-10-14_linux_hand34` | bad family-scan row: the first 28 bytes of a ~2.1 KB function (sub esp,294h); author the whole function |
| `00c3eb60` | multi | open | 00c3eb60:25 00c3eb80:25 | `2026-10-15_linux_hand74` | row spans two functions (0xc3eb60 + 0xc3eb80) |
| `00c40e20` | multi | open | 00c40e20:8 00c40e30:11 00c40e40:11 00c40e50:14 | `2026-10-15_linux_hand74` | row spans four functions |
| `00c8a520` | ltcg | open |  | `2026-10-14_linux_hand57` | a real function with 7 callers that takes its object in esi (LTCG register argument, no prologue): needs /GL+LTCG |
| `00ca5510` | multi | open | 00ca5510:16 00ca5520:8 00ca5530:7 00ca5540:44 | `2026-10-15_linux_hand74` | row spans four functions |

### Parked rows per batch (hand-over lines written by the authoring agents)

#### `2026-10-14_linux_hand58` (6 parked; sources in `rebuild/candidates/manual/pending/2026-10-14_linux_hand58/differ/`)
- 00ac4ec0 GetChunkInfo: the lane reads the hidden result pointer before the sprintf and copies `this` (explicit / implicit CCharString conversion, an inline accessor, optimize("s"), all under /GS).
- 004afdd0 (labelled FindMostRecentMessage): the SGI `__find` value temporary -- retail homes it in the dead `key` slot (`mov eax,[esp+10h] ; mov [esp+10h],eax`, shared with the empty tag), the lane gives it a `push ecx` frame slot; the rest of the body matches.
- 0082a340 SetMarkerGraphic: only the hidden-result slot of `__find_if` (retail the dead `graphicIndex` slot shared with the tag, the lane the dead `thing` slot).
- 0082a390 GetNonAutoMarkers: prologue only -- retail pushes ebx/esi at the entry, the lane shrink-wraps them into the non-empty branch; the loop (malloc, placement test, link) is byte-identical.
- 00741d60 CModeExplosiveForcePushed `??_G`: the lane calls the implicit ~CAnimFilterOfGroup and ~CModeBase out of line where retail inlines them; verified in the lane's object that an implicit destructor stores only the BASE vtable (so a user `__forceinline` destructor adds a store retail lacks); `novtable` and the inline pragmas do not help.
- 006af2d0 IsOriginatorAlive::operator(): the inlined GetPOriginator<CThing> constant-false check (`mov [esp+8],eax ; xor al,al ; jne`) that the lane folds (`return false`, `thing != thing`, `(unsigned long)thing < 0` through a `CThing* const&` tried) -- the same gap as the parked 0x006AF4E0.


#### `2026-10-15_linux_hand61` (5 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand61/differ/`)
- 00768a70 SwapKeyedRecord (label CNearestBuildingWithProperty) DIFFER(73v81): retail builds the swap temporary's last three words in the 16-byte frame through esi (`mov esi,[ecx+4] ; mov [esp+8],esi` x3) and copies left=right through esi/edi only, lane keeps them in ebx/ebp/esi/edi | tried: POD, 4-word memberwise ctor (+operator=), pair<long,POD12> with pair ctor (matches 0086a690), member ctor, float members, array members, memcpy member, all again under optimize("s") | next: lane gap: register allocation (temporary not scalar-replaced in retail)
- 00769340 SwapKeyedRecord (label CNearestBuildingWithProperty) DIFFER(73v81): byte-identical to 00768a70, same frame-temporary difference | tried: same set as 00768a70 | next: lane gap: register allocation
- 008068b0 SwapKeyedRecord (label CNearestBuildingWithProperty) DIFFER(73v81): retail spills the temporary's first three words to the frame through eax and keeps the fourth in eax | tried: same set as 00768a70 (12-byte member first and last) | next: lane gap: register allocation
- 00700120 COpinionReactionManager::GetBlurredAttitude DIFFER(79v95): the lane drops the `range ? seed % range : 0` guard after the random step (`test esi,esi ; je` + duplicated `blurred[0]` arm in retail) because the entry test already proved count > 0 | tried: named unsigned count, size() re-evaluated for the range (keeps the guard, recomputes the size after the call: 105), early return with a long count (81) | next: lane gap: value-range propagation (QFE drops a redundant zero test RTM kept)
- 004e9550 CPersistContext::TransferHelper (label CObjectAugmentationParticleSet::TransferHelper) DIFFER(80v77): retail forms the traits `this` (`lea ecx,[ebp-2Ch]`) once above the mode switch's first `je` and the default arm falls into the destructor call without re-forming it; the lane re-forms it per arm | tried: switch, if/else chain (83), __forceinline traits TransferBinary(context, value) holding the switch (80) | next: lane gap: rematerialisation/hoisting of a frame address


#### `2026-10-15_linux_hand62` (3 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand62/differ/`)
- 004afc60 CQuestManager::IsCreatureGenerationEnabled DIFFER(65v63): retail `mov eax,[esi+44h] ; mov ebx,[esi+40h] ; sub eax,ebx` vs ours `mov ebx,[esi+40h] ; mov eax,[esi+44h] ; mov ecx,ebx ; sub eax,ecx` (entry block only; loop and exits identical) | tried: indexed `[i]` (61 bytes, element re-derived per iteration), running element pointer incremented in the for-header or in the body (65), optimize("s") on both (58/59) | next: lane gap: register copy/load order of a CSE'd first-iteration load (try /Oa or a `begin()` iterator object on the VC7.1 RTM box)
- 00acd900 COptimisedPrimitive::DetectFoldOver DIFFER(83v83): loop body `mov eax,[esi+4] ; lea ecx,[eax+edi*8] ; push ecx` + count in edx (retail) vs `mov ecx,[esi+4] ; lea edx,[ecx+edi*8] ; push edx` + count in eax (ours) | tried: CArray operator[], `m_pData + i`, signed `long` Size() | next: lane gap: register colouring
- 00b219a0 CPSCSplineBase::GetPointAtIndex DIFFER(86v86): inlined C3DVector copy `mov eax,[edi+10h] ; lea edx,[eax+ecx*4] ; mov ecx,[edx] ; mov eax,ebx ; mov [eax],ecx` (retail) vs `mov edx,[edi+10h] ; lea eax,[edx+ecx*4] ; mov edx,[eax] ; mov ecx,ebx ; mov [ecx],edx` (ours), plus the final params load in edx instead of eax | tried: POD assignment from the element, CArray operator[], by-value GetControlPoint accessor (all identical), user C3DVector::operator= (84 bytes, drops the `mov`) | next: lane gap: register colouring


#### `2026-10-15_linux_hand64` (2 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand64/differ/`)
- 006aee40 CTCRegionFollower::GetFollowers DIFFER(92v90): retail homes the push_back item AND the SScalarTag at the dead `out` argument slot (`mov [esp+10h],eax ; lea eax,[esp+18h] ; lea edx,[esp+1Ch]` = one address, no `push ecx` frame), the lane gives the item a fresh `push ecx` slot and the tag the arg slot | tried: retail push_back(const T&) with an rvalue `Get()` item, push_back(T) by value | next: lane gap: empty tag temporary overlaid on a live argument slot (GOTCHAS 24l)
- 006d5850 CTCFollowed::GetFollowers DIFFER(92v90): byte-identical shape to 006aee40 (CullNullFollowers 0x006D5260): the item and the tag share the dead argument slot in retail, the lane adds a `push ecx` slot for the item | tried: the 006aee40 forms | next: lane gap: empty tag temporary overlaid on a live argument slot (GOTCHAS 24l)


#### `2026-10-15_linux_hand65` (5 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand65/differ/`)
- 009ba9d0 CPersistContext::Init(6 args) DIFFER(58v58): retail copies each pointer argument through ONE register (`mov edx,[esp+10h] ; mov [ecx],edx ; mov edx,[esp+14h] ; mov [ecx+4],edx ...`) and zeroes eax before the +0x10 store, the lane interleaves eax/edx loads ahead of the stores | tried: plain body, `optimize("s"/"t"/"y")`, `g` off, assignments through a reference-taking inline helper, volatile members | next: lane gap: copy-register scheduling (RTM vs QFE)
- 009badd0 CPersistContext::CPersistContext(6 args) DIFFER(60v60): the same single-register (edx) copy sequence as 009ba9d0 inlined into the constructor; the lane alternates ecx/edx | tried: Init inlined into the constructor, `optimize("s")` (stops the inlining, 36 bytes) | next: lane gap: copy-register scheduling (same as 009ba9d0)
- 009bb0d0 CPersistContext::CleanOutLine DIFFER(50v50): only the SIB operand order: retail `cmp byte ptr [eax+ecx],0Ah` / `mov byte ptr [eax+ecx],20h` (index i as base), the lane `[ecx+eax]` (text as base) | tried: ternary text accessor (gave a jmp diamond), default-first accessor (fixed everything else), `i[text]`, `optimize("s")` | next: try `operator[]` on CCharString or an `unsigned char*` text; else lane gap: address-mode operand order
- 004310a7 CPersistContext::TransferWideString DIFFER(91v91): the two 4-byte string frame slots are swapped: retail default local `[ebp-4]`, read temporary `[ebp-8]`; the lane `[ebp-8]` / `[ebp-4]` | tried: body local after CheckCRC, named local in the case block, default as an argument temporary to an inlined helper, explicit `operator=` call, const-ref bound temporary | next: lane gap: frame-slot order of a local vs a later temporary
- 00431143 CPersistContext::TransferString DIFFER(91v91): same frame-slot swap as 004310a7 (CCharString twin) | tried: the same five forms | next: lane gap: frame-slot order (as 004310a7)


#### `2026-10-15_linux_hand66` (3 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand66/differ/`)
- 00403fc0 CPersistContext_TransferableComponent::TransferIn DIFFER(33v36): retail `xor eax,eax ; mov [esp],eax ; mov byte ptr [esp],al` (dword zero-fill of the CPersistTraits<bool> slot, then the byte) vs lane `xor al,al ; mov byte ptr [esp],al` | tried: ctor `DefaultVal()` / `DefaultVal(T())` / defaulted `const T&` argument, aggregate `{false}` / `{bool()}`, temporary `CPersistTraits<bool>().TransferIn(..)`, value-initialised POD `= CPersistTraits<bool>()` (closest), padded 4-byte POD (dword only) | next: lane gap: value-initialisation of a 1-byte POD (RTM zero-fills the 4-byte slot first)
- 0040fba0 CPersistContext_TransferableVector::TransferOut DIFFER(37v37): `mov edx,[esp+0Ch]` (index) before `mov ecx,[eax]` (first) in retail, swapped in the lane | tried: `(*TransVector)[index]`, iterator-object `*(begin()+n)` (+2 bytes), named `first` pointer local, vector reference local, parameter reuse `index = ..; Write(&index)` | next: lane gap: load scheduling (the same named-local trick fixes the two sibling writers 005663f0 / 00614450, not this one)
- 00b1b4d0 CPersistContext::Init DIFFER(69v69): `mov dword ptr [eax],1Eh` right after the +0x4 zero store in retail, sunk below the argument copies in the lane | tried: init list, base class owning +0x0/+0x4, body assignments in retail order, `#pragma optimize("s",on)` (adds a frame) | next: lane gap: immediate-store sinking (same family as the 24b "1.0f store sunk past zero stores" gap)


#### `2026-10-15_linux_hand67` (3 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand67/differ/`)
- 00a18760 CPhysicsMesh::GetPointIntersectionFromTo DIFFER(93v95): retail `mov edx,ecx` before the pre-test call and `[edx+0B4h]` / `mov ecx,edx` after it, plus `xor bl,bl` false result; lane keeps `this` in esi and `xor al,al` at the exit | tried: `bool result = false; if (pre-test) { if (g && seg) return A; return B; } return result;` (pointer and PDB reference signatures) | next: lane gap: whole-program (LTCG) register knowledge that 0x00A17690 preserves edx
- 00487aa0 CPlayer::InitJoystickDeviceNumber DIFFER(67v67): else arm `dec ecx ; and ecx,eax ; mov [esi+228h],ecx` vs lane `dec ecx ; and eax,ecx ; mov [esi+228h],eax` | tried: direct `Number == GetMainPlayerNumber()`, inline IsMainPlayer, negated condition, inline IsMainPlayer with a `number` local (the last fixes ebx/edi and the else-arm Number re-read) | next: write the else arm as `number != mainDevice ? number : 0` or `JoystickDeviceNumber = Number; if (Number == mainDevice) JoystickDeviceNumber = 0;` on top of the local-Number IsMainPlayer
- 00c21330 CPlayer::IsFirstPersonTargeting DIFFER(48v42): retail keeps `this` in ecx across `call 0x00C20EE0` (`add ecx,38h` after it, no `push esi ; mov esi,ecx`) | tried: `!= 0` byte result, bool member read from a named 16-byte record local (only the this-register difference remains) | next: lane gap: whole-program register knowledge of the 0x00C2xxxx callee (the 00c20cb0 family); identity also unproven (`ret 4` vs the PDB's no-argument IsFirstPersonTargeting)


#### `2026-10-15_linux_hand68` (4 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand68/differ/`)
- 004497e0 CPlayerManager::GetPlayerDefIndexFromNumber DIFFER(101v91): retail cross-jumps the NEUTRAL and SPIRIT `lea ecx ; call CCharString ; lea eax,[esp+8] ; push eax` blocks (HERO uses edx), the lane re-addresses with eax/ecx/edx and merges none | tried: explicit CCharString temporaries (73, all paths merged, ctor eax pushed), implicit conversions in three returns (101), if/else into a long local (101), `number != 4 && number == MainPlayer` + literal ternary (94) | next: lane gap: register allocation / tail-merge (RTM vs QFE)
- 00e24210 CQR_EscortTraderScript::InFinalRegion DIFFER(66v68): retail `mov eax,[ecx+eax*4] ; mov ecx,[ecx+114h] ; sub eax,4 ; mov eax,[eax]` vs lane `mov edx,[ecx+eax*4] ; mov eax,[edx-4] ; mov ecx,[ecx+114h]` | tried: operator== reading the left block first (70), right block first (66), back() via iterator `--it` (66) and `operator-(1)` (66) | next: lane gap: RTM vector-slot address materialisation (GOTCHAS 24l)
- 00ab37b0 CVertexStreamFormat::CalculateVertexSize DIFFER(44v58): retail keeps the index (`inc esi ; cmp esi,edi ; jb`) against a size re-derived in the preheader (`mov edi,[ecx+8] ; mov eax,edx ; sub edi,eax ; sar edi,3`), the lane reverses the counter (`mov edi,eax ... dec edi ; jne`) | tried: unsigned index with GetDataTypeSize defined in the TU (noinline, needed for edx live across the call), signed index, callee defined after the caller | next: lane gap: IPA-known callee + loop-counter reversal (QFE optimiser)
- 00c70a60 CopyAligned128 (catalog CPreTransposedBoneMatrix::Set) DIFFER(94v91): retail `push esi ; push edi ; mov esi,[esp+10h] ; mov edi,[esp+0Ch] ; movdqa xmm0..7` (inline-asm register discipline) vs intrinsics `push ebp ; mov ebp,esp ; ... and esp,-16` with rotated xmm order | tried: _mm_load_si128/_mm_store_si128 x8 | next: lane gap: inline-asm original (asm_bake only)


#### `2026-10-15_linux_hand69` (3 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand69/differ/`)
- 00bde080 CLocalDetailCacheMap::CQuadTreeElement::ValidationTest DIFFER(44v49): retail ends `mov ecx,[esi+0Ch] ; test ecx,ecx ; pop esi ; je ; jmp ValidationTest`, the lane emits `mov esi,[esi+0Ch] ; test esi,esi ; jne <body>` (self tail call turned into a loop) | tried: out-of-line member, in-class inline definition + forcing caller, `#pragma optimize("g",off)` | next: lane gap: tail-recursion elimination (retail keeps the self sibling-call jmp)
- 004afbb0 CQuestManager::OnEntityInterrupted DIFFER(72v72): retail keeps `this` in ebx and the thing in ebp (`mov ebp,[esp+0Ch] ; mov ebx,ecx`) and loads the id with `mov eax,[ebp+1Ch] ; mov ecx,[ebp+18h]`, ours swaps ebx/ebp and loads the id through ecx/edx | tried: plain member access (the thing is loaded in the preheader), a `CThing*` local (propagated away), an id-pointer local (entry load, registers still swapped), a pointer local for the list member | next: lane gap: register colouring (try a `CThing&` member-function helper for the two script calls, or the out-of-line `for` with an iterator object)
- 004b3810 CQuestManager::SetCreatureGenerationEnabledDuringScript DIFFER(76v76): `lea eax,[esp+14h]` (retail homes the empty find tag on the LIVE `enabled` slot) vs the lane's `lea eax,[esp+10h]` (the dead `script` slot) | tried: vector pointer declared before / after the cached `last` (the second fixes the esi/edi colouring) | next: lane gap: empty tag temporary over a live argument slot (wave 24l/24n)


#### `2026-10-15_linux_hand70` (2 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand70/differ/`)
- 004016f0 CDisplayManagerInit::CDisplayManagerInit DIFFER(78v78): retail `xor eax,eax ; mov ecx,1` both hoisted, NoBackBuffers `mov [esi+19h],cl` after the `mov byte [esi+18h],10h` immediate; the lane reuses eax (`mov eax,1` after the zero group) and keeps +0x19 inside the ones group | tried: member order (ones group first, eax=1/ecx=0), member order with the Present* trio reversed (fixes the in-group order), retail store order (zeros first; only register reuse and +0x19 left) | next: lane gap: constant-register grouping/scheduling of body-assignment stores (same instructions, same length)
- 0066c7b0 CRegionMinimap::InitialiseRegionBox DIFFER(90v94): retail copies the top-left words before subtracting (`mov edx,esi ; sub eax,edx`, `mov ebx,edi ; sub edx,ebx`) and builds the wide arm's right edge in ebx (`lea ebx,[eax+esi] ; mov eax,ebx`); the lane subtracts esi/edi directly and allocates the arm sums differently | tried: flat if/else side clamp, three arms storing a C2DBoxI temporary (struct copy via `add ecx,14h`), per-arm field stores, a brx/bry join on the header fields, the join over an inlined GetRegionMapExtents (best, in differ/) | next: by-value C2DVectorI/C2DExtentsI temporaries fed to an inline C2DBoxI(topLeft, extents) setter over the flat header fields (the retail copies look like the inlined GetRegionMapExtents' re-reads CSE'd into copy registers), else lane gap: copy-register allocation


#### `2026-10-15_linux_hand71` (3 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand71/differ/`)
- 00def6a0 CVtable_012ddf3c::CVtable_012ddf3c DIFFER(91v91): `mov ecx,edi` for the +0x28 CBase constructor sits between the +0x20 and +0x24 zero stores in retail, before all three in the lane | tried: three plain init-list zeros, a 3-word member object with inline ctor + CScriptThing's two words as a member object (fixed the two vtable-store positions), the zero words and the default CScriptThing as one member object | next: lane gap: member-constructor `this` scheduled after a register-zero store run (the 24i gap)
- 009fbd00 CRenderTarget::`scalar deleting destructor' DIFFER(66v60): retail stores no CRenderTarget vptr (`mov dword ptr [esi],offset vftable` extra in ours) | tried: implicit destructor (the lane emits ??1 out of line and calls it: 30v60, also with /Ob2 and a __forceinline CSurface destructor), `__forceinline virtual ~CRenderTarget() {}` (inlined but stores the vptr), `__declspec(novtable)` (no ??_G emitted) | next: lane gap: the lane keeps the implicit destructor out of line instead of inlining it into `??_G`, as retail did. In this chunk the lane also emitted the implicit copy constructors (0x0046649A, 0x00737370) out of line.
- 009fc4a0 CResource::SetAsMRU DIFFER(78v80): retail copies the previous node through eax before linking (`mov eax,edx ; mov [eax+10h],ecx ; mov edx,[ecx+10h] ; mov [edx+0Ch],ecx`), the lane stores through edx and reloads into eax | tried: landed ListInsert(prev, next), four links written out, a next-first insert helper with `prev->` and with `PrevResource->` | next: the form that fixed 0x009FBFE0 (head reached through an inline `GetHead()` accessor at each use, maybe under optimize("s"))


#### `2026-10-15_linux_hand72` (4 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand72/differ/`)
- 00c25860 C3DVector::CrossProduct DIFFER(57v57): Y component's second product retail `fld [ecx] ; fmul [eax+8]` vs lane `fld [eax+8] ; fmul [ecx]` | tried: `X * other.Z`, `other.Z * X`, inline Cross(a, b) helper, field-by-field named result | next: lane gap: x87 commutative operand canonicalisation (both spellings compile identically)
- 006c59a0 CTCHeroOpinionDeedLog::DecriminaliseDeed DIFFER(58v60): found iterator read retail `mov eax,[esp+4]` (frame slot) vs lane `mov eax,[eax]` (find's returned pointer); body after the compare exact | tried: `iterator it = find()` + `it->second` (61v60), inline IsCrime setter (61v60), reference to the node value (60v60), `it; it = find();` assignment (58v60) | next: iterator copy-constructed from a named temporary / `it = iterator(find())` with a user copy constructor so the slot is re-read
- 00cbe583 GetSquaredDistanceBetweenThings DIFFER(61v61): retail `mov esi,eax ; mov eax,[edi] ; mov ecx,edi ; call [eax+18h]` vs lane vptr fetched into edx first; squares retail Z, X, Y vs lane Z, Y, X | tried: POD difference (107v61), memberwise-copy C3DVector (61v61), named position references + reordered sum (no change) | next: lane gap: vptr eax/edx register choice (0x0053D000 family) + x87 sum canonicalisation
- 00caa3c0 CVtable_012bf83c::DeallocateScriptEntries DIFFER(80v69): retail keeps the pop loop top-tested (`test ; je D ; ... ; jmp L`), lane rotates it (entry test + bottom test + `lea ebx,[ebx]` alignment pad); body identical | tried: `while (m_pFirstEntry)` (83v69), `for (;;) { entry = m_pFirstEntry; if (!entry) break; ... }` (80v69) | next: lane gap: loop rotation (GOTCHAS Wave 23f)


#### `2026-10-15_linux_hand73` (1 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand73/differ/`)
- 00853260 NCameraModes::CScriptModuleSpline::IsFinished DIFFER(38v38): retail `je 85327Ch` after the dead-bit test with the shared `mov al,1 ; pop esi ; ret` exit as its fall-through and the `mov al,[esi+58h] ; test al,al ; setne al` tail last; the lane emits `jne` to a `mov al,1` exit placed after the setne tail (same instructions, block order only) | tried: `if (!p || p->m_dead) return true; return RF != 0;`, `if (p && !p->m_dead) return RF != 0; return true;`, `return !p || p->m_dead || RF;` (bool: full-register 0/1, 42 bytes), nested `if (p) { if (dead) return true; return RF != 0; } return true;` | next: the lander's optimize("s"/"t") sweep, then lane gap: block placement of a shared constant-return exit


#### `2026-10-15_linux_hand75` (6 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand75/differ/`)
- 00c40450 CTrackedTransform_00c40450::SetPosition DIFFER(79v75): retail `or dword ptr [ecx+48h],8000h` after the 12-byte copy; the lane emits `mov eax,[ecx+48h] ; or eax,8000h ; mov [ecx+48h],eax` interleaved with the third copy | tried: volatile flags, inline SetDirty, component-wise copy, CTrackedVector-typed member with operator!=, optimize("s")/("t"), /Oa, /GS | next: lane gap: RMW split by the scheduler
- 00c404a0 CTrackedTransform_00c404a0::SetDirection DIFFER(79v75): same as 00c40450 (`or [ecx+48h],10000h` split into load/or/store) | tried: the 00c40450 forms | next: lane gap: RMW split by the scheduler
- 0046313e CSimVoice::operator= DIFFER(43v43): retail `mov al,[edi+0Dh] ; mov [ebx+0Dh],al ; pop edi ; mov eax,esi`, the lane `mov dl,[edi+0Dh] ; pop edi ; mov eax,esi ; pop esi ; mov [ebx+0Dh],dl` | tried: explicit operator= with inline map op=, implicit (COMDAT) op=, STLport vector member / base for the map, optimize("s") | next: lane gap: epilogue scheduling of the last byte store
- 006808c0 CSimVoice::TransferBinaryOut DIFFER(61v73): after WriteLong, retail re-reads first/last for the byte count behind the GetData() join (`mov ecx,[esi+4] ; mov ebx,[esi] ; sub ; sar 3 ; shl 3`, ebx saved); the lane reuses the GetData count (`shl eax,3`) | tried: ternary / if-local / void* GetData, signed and unsigned Size, separate byte-count accessor, data local, iterator-object vector, STLport vector base | next: model the map as the real CVectorMap<..> template over the VC7.1 std::vector with checked iterators (the PDB names `_Vector_iterator`), or lane gap: cross-join CSE
- 00a5c570 CSimpleMemoryPool::CSimpleMemoryPool DIFFER(74v74): one instruction placement: retail `xor ecx,ecx` before `mov [esi+0Ch],eax`, the lane after it | tried: granularity in the init list with body assignments, everything in the body, everything in the init list | next: an inline SetGrowthGranularity / CArea constructor variant, else lane gap: scheduling
- 00a533f0 CSimpleMemoryPool::New<SSequenceKey> DIFFER(82v80): the lane keeps a dead `jb` after `test edx,edx ; ja` in the unsigned 64-bit size guard; retail drops it because `test` clears CF | tried: `<= 0xFFFFFFFF`, inverted `>`, `< 2^32`, a named 64-bit local, optimize("s"), /Oa, /GS | next: lane gap: dead branch after test (RTM peephole)


#### `2026-10-15_linux_hand76` (6 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand76/differ/`)
- 004570d3 CSpecialAbilitiesBattleChargeDef::CSpecialAbilitiesBattleChargeDef DIFFER(61v61): `lea eax,[esi+90h]` placed before the +0x58 zero store (retail: before +0x60) | tried: SZeroed member object (65 bytes), body assignments, optimize("s") + PDB CRetailVector members (61v61), allocator-tag vector ctor | next: lane gap: lea scheduling (24i)
- 00457da9 CSpecialAbilitiesSummonSpellDef::CSpecialAbilitiesSummonSpellDef DIFFER(58v58): `lea ecx,[esi+64h]` hoisted above the three SummonTintColour zero stores (retail: before the last) | tried: optimize("s") (58v58), no pragma (61, -1 as a 7-byte mov) | next: lane gap: lea scheduling (24i)
- 00458c19 CMeleeCombatAbilityDef::CMeleeCombatAbilityDef DIFFER(45v45): `lea ecx,[esi+68h]` hoisted above the ValidWeaponClasses zero stores | tried: optimize("s") init-list vector, body-assignment vector ctor | next: lane gap: lea scheduling (24i)
- 004e00bc CQuestCardDef::CQuestCardDef DIFFER(45v45): `lea ecx,[esi+5Ch]` hoisted above the RewardObjects zero stores | tried: optimize("s") init-list vector | next: lane gap: lea scheduling (24i)
- 0045cfad CSoundMap::operator= DIFFER(49v49): last Dirty byte copied through cl with `pop edi ; mov eax,esi ; pop esi` between load and store (retail: through al before the pops) | tried: explicit memberwise outer + inner operator=, implicit outer operator=, void inner operator=, optimize("s") (out-lines the inner operator=, 33 bytes) | next: lane gap: register/scheduling (try `__forceinline` inner operator= under optimize("s"))
- 005bc6d6 CUIBackgroundOwner_005bc6d6::RenderBackground DIFFER(20v23): float argument re-pushed `mov eax,[esp+4] ; push eax` (retail `fld [esp+4] ; push ecx ; fstp [esp]`) | tried: plain float forwarder, /Op, 4-byte float wrapper with a user copy ctor, optimize("g",off) | next: optimize("s") (the unaligned 0x005BC6D6 start suggests a /Os TU), else lane gap: float argument copy


#### `2026-10-15_linux_hand77` (6 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand77/differ/`)
- 00b1b5e0 CSprite::GetHeight DIFFER(8v34): retail `sub esp,0Ch ; mov [esp+8],float ; mov [esp+4],&result ; fld [esp+8] ; mov edx,[esp+4] ; fistp dword ptr [edx]` vs the lane's `fld [ecx+48h] ; jmp __ftol2` | tried: `(long)m_height` | next: lane gap: inlined inline-asm rounding helper (asm_bake, same as 006472b2)
- 00a5f290 CStreamingFontDataBank::CStreamingFontDataBank DIFFER(89v89): `mov ecx,edi` (Filename as `this`) comes between the 2nd and 3rd Fonts zero stores in retail, before all three in the lane | tried: init-list CArray constructor, body-assigned CArray constructor, declared `~CArray` | next: lane gap: member-constructor `this` setup scheduled before register-zero stores (the 24i gap)
- 009b9940 CStringParser::SetStringPos DIFFER(62v62): retail moves `this` to esi and keeps the quote flag in cl (end pointer in edi), the lane keeps `this` in ecx with the flag in bl (end in esi); loop-head padding is `jmp ; lea ecx,[ecx]` in retail and `lea esp,[esp]` in the lane | tried: pointer loop `p != String + pos`, index loop, `text` local, declaration order flag/pointer before the store | next: lane gap: register allocation / alignment padding
- 009ba390 CStringParser::ReadLineAsString DIFFER(76v71): retail reads `[ecx+4]` / `[ecx+20h]` AFTER the call to SkipPastNewline (`this` kept in ecx across the call) | tried: start pointer local + `String + StringPos - start` length | next: lane gap: whole-program (/GL) callee register knowledge
- 009ba4a0 CStringParser::ReadAsStringUntilWhitespace DIFFER(81v80): retail `mov eax,[esi+4] ; lea edi,[eax+ecx] ; cmp byte ptr [edi],0`, lane `mov edi,[esi+4] ; mov al,[edi+ecx] ; add edi,ecx ; test al,al`; the length is built in edx (retail) vs eax (lane) | tried: early empty return, reading path as the `if` body, `optimize("s")` (57 bytes, merges returns), `&String[pos]` form | next: lane gap: the lane folds the dereference into the address computation
- 0048dc3c CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic DIFFER(79v79): retail places the thumb-click return (`add eax,6A4h ; jmp`) right after its `||` pair (`je A ; cmp ; jne next ; A:`); the lane jumps (`je ; je`) to a copy below the default return | tried: else-if returns (+`optimize("s")`, which fixes the shared `ret 4`), a nested `!=` guard, a result-pointer local, a ternary chain | next: lane gap: block placement


#### `2026-10-15_linux_hand78` (2 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand78/differ/`)
- 009f31c0 CSurface::SetPixel DIFFER(84v82): retail `imul eax,[x] ; mov ecx,[rect] ; mov edx,[ecx+8] ; imul edx,[y] ; add edx,eax ; add edx,[ecx+0Ch]` vs ours `imul eax,[x] ; mov ecx,[rect] ; mov edx,[ecx+8] ; imul edx,[y] ; mov ecx,[ecx+0Ch] ; add eax,edx ; add ecx,eax` (register choice of the pixel-address sum) | tried: `PMem + y*Pitch + x*depth`, `&PMem[y*Pitch + x*depth]`, `PMem + Pitch*y + depth*x` (all 86v82: depth copied to ecx, rect in eax), a named `long offset` local (84v82) | next: an inline `CSurfaceLockedRect::GetPixelAddress(x, y, bytesPerPixel)` helper, or a `const CRGBColour&` colour parameter; else lane gap: register allocation.
- 004e06c3 CShopDef::CShopDef DIFFER(62v62): retail `mov [esi+4Ch],eax ; mov [esi+50h],eax ; lea ecx,[esi+64h] ; mov [esi+54h],eax` vs ours `lea ecx,[esi+64h]` right after the `[esi+48h]` store | tried: vectors as inline-constructed members, the same under optimize("s"), six plain init-list words (lea above all six stores) | next: lane gap: member-constructor `lea` scheduled after a run of register-zero stores (GOTCHAS wave 24i).


#### `2026-10-15_linux_hand79` (2 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand79/differ/`)
- 007eac10 CTCActionUseBase::OnCreate DIFFER(93v81): retail keeps the local def pointer in esi from GetSubDef through its release and stores its zero after the argument pushes; ours re-reads the frame slot | tried: plain CDefPointer local, raw pointer copy probe (83v81) | next: lane gap: CDefPointer local filled through `&local` re-read (GOTCHAS 24k; as hand82 0x007E3040)
- 007eac70 CTCActionUseBase::vfunc_6 DIFFER(63v63): retail loads ParentThing into ecx before the three argument pushes, ours after | tried: named string local, named thing local, inline GetThing() accessor, implicit CCharString conversion | next: lane gap: scheduling


#### `2026-10-15_linux_hand81` (4 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand81/differ/`)
- 007e93c0 CTCAvoidRegionExit::vfunc_2 DIFFER(78v78): `mov edx,[esp+8]` (range.first) is loaded after the second iterator's slot copy instead of before `push ecx` | tried: plain EqualRange+Erase, inline map forwarders (both loads hoisted, `mov ecx,esp` addressing), STLport base-class iterator copy constructor, a named first-iterator local | next: lane gap: argument-build scheduling of two by-value 4-byte iterators
- 007e9410 CTCAvoidRegionExit::OnKill DIFFER(78v78): same body as 007e93c0 (slot 14, base call 0x006868F0); the same single load is out of place | tried: plain form, inline forwarders | next: lane gap: argument-build scheduling (as 007e93c0)
- 007e9360 CTCAvoidRegionExit::vfunc_16 DIFFER(91v91): same removal under `if (flag)`, `__forceinline` helper; the key temporary already lands in the dead bool slot; only `mov edx,[esp+8]` is out of place | tried: plain helper (not inlined, 30 bytes), forceinline + forwarders, forceinline plain | next: lane gap: argument-build scheduling (as 007e93c0)
- 0064639c CPlayerGuiThingList_0064639c::RemoveThing DIFFER(69v69): the key temporary's store `mov [ebp+8],eax` is scheduled before `lea esi,[ecx+3Ch] ; mov ecx,[esi]` instead of after | tried: `const CThing* const&` key (binds directly, no copy), a derived-to-base key conversion (temporary and erase-result slot now exact), an inline list Find wrapper (out of line), the same wrapper `__forceinline` | next: lane gap: store scheduling of a conversion temporary homed in a dead argument slot


#### `2026-10-15_linux_hand82` (7 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand82/differ/`)
- 004fa9a0 CTCBulletTime::GetBulletTimeTicksFromAbilityLevel DIFFER(62v58): ebp frame with `mov [ebp-0Ch],0.5f ; mov [ebp-8],1.0f ; fld x ; fadd [ebp-0Ch] ; fistp dword [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-10h] ; mov eax,x ; cmp eax,[ebp-10h] ; dec dword [ebp-4]` | tried: volatile locals + /QIfist + optimize("y",off) (74v58), plain C ceiling + /QIfist (62v58) | next: lane gap: inline __asm (asm_bake only)
- 0079c5f0 CTCBulletTime::GetBulletTimeTicksFromAbilityLevel DIFFER(61v68): the same inline-asm ceiling inlined after `fild [ticksPerSecond] ; fmul [ecx+2Ch] ; fstp [ebp-10h]` | tried: plain C ceiling + /QIfist | next: lane gap: inline __asm
- 0079c680 CTCBulletTime::GetBulletTimeRemainingPercentage DIFFER(80v77): the same inline-asm ceiling, then `fild [ebp-18h] ; fidiv [ebp-4]` | tried: plain C ceiling + /QIfist | next: lane gap: inline __asm
- 0079c6d0 CTCBulletTime::GetBulletTimeElapsedTicks DIFFER(64v71): the same inline-asm ceiling, then `mov eax,[ebp-4] ; sub eax,[ecx+0Ch]` | tried: plain C ceiling + /QIfist | next: lane gap: inline __asm
- 006c0120 CTCBuyableHouse::GetPrice DIFFER(57v57): ours `mov edi,[eax+28h] ; mov eax,[eax+2Ch] ; sub eax,edi`, retail `mov edx,[eax+2Ch] ; mov edi,[eax+28h] ; sub edx,edi` | tried: `GetValueOfTrophies() + def->Price[level]` (48v57), call result in a named local (60v57), `prices` first-pointer local + explicit `last - prices` (57v57), `prices` local + size() (61v57) | next: lane gap: register/load order of the size() difference (maybe a `const vector&` local plus the `prices` local)
- 007e3040 CTCCameraCollision::vfunc_4 DIFFER(93v81): retail keeps the local def pointer's object in esi from GetSubDef's `this` through the release (`test esi,esi ; dec [esi+4]`) and stores the local's null after the argument pushes; the lane re-reads the frame slot after the call, stores the null first and expands the release (`mov esi,[eax+4] ; add eax,4 ; dec esi`) | tried: CDefPointer local (93v81; member-template specialisation declarations ICE the compiler first), explicit release on a raw copy (119v81), raw out-pointer local (89v81) | next: lane gap: CDefPointer local cached in esi across a call (GOTCHAS 24k)
- 0075cd30 CTCCameraPointScripted::GetEndPos DIFFER(81v81): X sum `fld [esi+40h] ; fadd [eax]` stored after two pushes (retail) vs `fld [eax] ; fadd [esi+40h]` stored after three | tried: free `operator+(pos, EndPos)`, member `pos + EndPos` (X and store order right, Z/Y reversed), member `EndPos + pos` | next: lane gap: x87 operand order / store scheduling of the last component


#### `2026-10-15_linux_hand90` (5 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand90/differ/`)
- 00c2d800 CAudioFormat::BytesToSamples DIFFER(36v30): retail keeps `this` in ecx across `call GetBlockAlign` and the quotient in edx across `call GetSamplesPerBlock` (`mov edx,eax ; call ; imul eax,edx`); the lane saves both in esi/edi | tried: natural member form | next: lane gap: LTCG callee register knowledge
- 00c2d820 CAudioFormat::SamplesToBytes DIFFER(36v30): same as 00c2d800 with the callees swapped | tried: natural member form | next: lane gap: LTCG callee register knowledge
- 00771d50 CTCHeroPodium::GetCrowdPos DIFFER(85v85): only the frame differs, retail `sub esp,10h` / `add esp,10h` (4 dead bytes above the 12-byte zero C3DVector temporary), lane `0Ch` | tried: temporary in the assignment, named local, an empty CDebugBreak-style temporary, the vector element through a begin() iterator object | next: lane gap: dead frame bytes (RTM vs QFE frame layout), or an unknown dead 4-byte local in the original
- 0057a962 CTCHeroStats::GetMaxExtraMaxStamina DIFFER(40v39): the best graded form emits `add eax,0DCh ; mov ecx,[eax]` for retail `mov ecx,[eax+0DCh]` | tried: reference copy + destructor release, raw-pointer ctor + pointee AddRef/Release, raw-pointer ctor + destructor release, reference copy + pointee members | next: the by-value holder accessor that matched the byte-twin 0057a939; differ/0057a962.cpp already holds it UNGRADED (budget spent), re-grade first
- 0057adb8 CTCHeroStats::ResetTrainingStatus DIFFER(70v63): retail loads the def object once into ecx and releases with `dec dword ptr [ecx+4] ; jne`; the lane loads it into eax and emits `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0` (or re-reads the frame word after the dec) | tried: pointee Release member + `if (Object) Object->Release()`, `if (Object && --Object->RefCount == 0)` in the destructor, local copy + direct dec, local copy + pointee Release | next: try the by-value accessor or out-parameter idiom that GetSubDef's other callers use; otherwise lane gap: def-pointer register allocation (cf. 24k "CDefPointer local re-read from its slot")

<!-- queue:end -->
