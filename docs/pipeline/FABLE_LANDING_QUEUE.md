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

## Step 3 -- docs after each landed part (Fable)
GOTCHAS: one `- Wave 25x (...)` line built from the batches' NOTES "What matched" lessons and each pending dir's `LESSONS.md` (the agent's final-report lessons plus any review item, e.g. a local base model next to an existing engine header -- check those before accepting the batch); journal
`docs/journal/2026-10/LINUX_CONTAINER_LANE_2026-10-13.md` (new part section above the previous one); `docs/HANDOFF.md`
and `docs/ROADMAP.md` counts; `python3 tools/write_decomp_dashboard.py && python3 tools/update_readme_progress.py`;
commit `land: ...` as BuffJesus with the session's Co-Authored-By / Claude-Session lines; `git push -u origin FullDecomp`.

## Also open from the Fable sessions (optional retries)
Parked rows of the Fable-authored chunks 59 and 60 are listed with evidence in
`rebuild/candidates/manual/batches/2026-10-14_linux_hand59/README.md` and `..._hand60/README.md` (sources in
`$S/w22/agent59/parked/`, `$S/w22/agent60/parked/`; scratch only).

## Generated status
<!-- queue:begin -->
| pending batch | chunk | matched (to land) | parked | skipped | state |
|---|---|---|---|---|---|
| `2026-10-14_linux_hand58` | Hand chunk 58 (CMessageEventFilter_IsOriginatedByThing .. CMouseCursor): 34 of 40 byte-exact | 34 | 6 | 0 | STAGED: land it |
| `2026-10-15_linux_hand66` | Hand chunk 66 (CPersistContext .. CPhysicsForceModelGotoOutsideSimple): 36 of 40 byte-exact | 36 | 3 | 1 | STAGED: land it |
| `2026-10-15_linux_hand71` | Hand chunk 71 (CRenderManagerCore .. CRightHandedSet): 37 of 40 byte-exact | 37 | 3 | 0 | STAGED: land it |
| `2026-10-15_linux_hand73` | Hand chunk 73 (CScriptGameResourceObjectScriptedThing .. CScriptThing): 38 of 40 byte-exact | 38 | 1 | 1 | STAGED: land it |
| `2026-10-15_linux_hand76` | Hand chunk 76 (CSmallVector .. CSpellContainerList): 33 of 40 byte-exact | 33 | 6 | 1 | STAGED: land it |

Totals: **178 matched rows staged for landing**, **19 parked rows** for the Fable retry pass.

### Parked rows per batch (hand-over lines written by the authoring agents)

#### `2026-10-14_linux_hand58` (6 parked; sources in `rebuild/candidates/manual/pending/2026-10-14_linux_hand58/differ/`)
- 00ac4ec0 GetChunkInfo: the lane reads the hidden result pointer before the sprintf and copies `this` (explicit / implicit CCharString conversion, an inline accessor, optimize("s"), all under /GS).
- 004afdd0 (labelled FindMostRecentMessage): the SGI `__find` value temporary -- retail homes it in the dead `key` slot (`mov eax,[esp+10h] ; mov [esp+10h],eax`, shared with the empty tag), the lane gives it a `push ecx` frame slot; the rest of the body matches.
- 0082a340 SetMarkerGraphic: only the hidden-result slot of `__find_if` (retail the dead `graphicIndex` slot shared with the tag, the lane the dead `thing` slot).
- 0082a390 GetNonAutoMarkers: prologue only -- retail pushes ebx/esi at the entry, the lane shrink-wraps them into the non-empty branch; the loop (malloc, placement test, link) is byte-identical.
- 00741d60 CModeExplosiveForcePushed `??_G`: the lane calls the implicit ~CAnimFilterOfGroup and ~CModeBase out of line where retail inlines them; verified in the lane's object that an implicit destructor stores only the BASE vtable (so a user `__forceinline` destructor adds a store retail lacks); `novtable` and the inline pragmas do not help.
- 006af2d0 IsOriginatorAlive::operator(): the inlined GetPOriginator<CThing> constant-false check (`mov [esp+8],eax ; xor al,al ; jne`) that the lane folds (`return false`, `thing != thing`, `(unsigned long)thing < 0` through a `CThing* const&` tried) -- the same gap as the parked 0x006AF4E0.


#### `2026-10-15_linux_hand66` (3 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand66/differ/`)
- 00403fc0 CPersistContext_TransferableComponent::TransferIn DIFFER(33v36): retail `xor eax,eax ; mov [esp],eax ; mov byte ptr [esp],al` (dword zero-fill of the CPersistTraits<bool> slot, then the byte) vs lane `xor al,al ; mov byte ptr [esp],al` | tried: ctor `DefaultVal()` / `DefaultVal(T())` / defaulted `const T&` argument, aggregate `{false}` / `{bool()}`, temporary `CPersistTraits<bool>().TransferIn(..)`, value-initialised POD `= CPersistTraits<bool>()` (closest), padded 4-byte POD (dword only) | next: lane gap: value-initialisation of a 1-byte POD (RTM zero-fills the 4-byte slot first)
- 0040fba0 CPersistContext_TransferableVector::TransferOut DIFFER(37v37): `mov edx,[esp+0Ch]` (index) before `mov ecx,[eax]` (first) in retail, swapped in the lane | tried: `(*TransVector)[index]`, iterator-object `*(begin()+n)` (+2 bytes), named `first` pointer local, vector reference local, parameter reuse `index = ..; Write(&index)` | next: lane gap: load scheduling (the same named-local trick fixes the two sibling writers 005663f0 / 00614450, not this one)
- 00b1b4d0 CPersistContext::Init DIFFER(69v69): `mov dword ptr [eax],1Eh` right after the +0x4 zero store in retail, sunk below the argument copies in the lane | tried: init list, base class owning +0x0/+0x4, body assignments in retail order, `#pragma optimize("s",on)` (adds a frame) | next: lane gap: immediate-store sinking (same family as the 24b "1.0f store sunk past zero stores" gap)


#### `2026-10-15_linux_hand71` (3 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand71/differ/`)
- 00def6a0 CVtable_012ddf3c::CVtable_012ddf3c DIFFER(91v91): `mov ecx,edi` for the +0x28 CBase constructor sits between the +0x20 and +0x24 zero stores in retail, before all three in the lane | tried: three plain init-list zeros, a 3-word member object with inline ctor + CScriptThing's two words as a member object (fixed the two vtable-store positions), the zero words and the default CScriptThing as one member object | next: lane gap: member-constructor `this` scheduled after a register-zero store run (the 24i gap)
- 009fbd00 CRenderTarget::`scalar deleting destructor' DIFFER(66v60): retail stores no CRenderTarget vptr (`mov dword ptr [esi],offset vftable` extra in ours) | tried: implicit destructor (the lane emits ??1 out of line and calls it: 30v60, also with /Ob2 and a __forceinline CSurface destructor), `__forceinline virtual ~CRenderTarget() {}` (inlined but stores the vptr), `__declspec(novtable)` (no ??_G emitted) | next: lane gap: the lane keeps the implicit destructor out of line instead of inlining it into `??_G`, as retail did. In this chunk the lane also emitted the implicit copy constructors (0x0046649A, 0x00737370) out of line.
- 009fc4a0 CResource::SetAsMRU DIFFER(78v80): retail copies the previous node through eax before linking (`mov eax,edx ; mov [eax+10h],ecx ; mov edx,[ecx+10h] ; mov [edx+0Ch],ecx`), the lane stores through edx and reloads into eax | tried: landed ListInsert(prev, next), four links written out, a next-first insert helper with `prev->` and with `PrevResource->` | next: the form that fixed 0x009FBFE0 (head reached through an inline `GetHead()` accessor at each use, maybe under optimize("s"))


#### `2026-10-15_linux_hand73` (1 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand73/differ/`)
- 00853260 NCameraModes::CScriptModuleSpline::IsFinished DIFFER(38v38): retail `je 85327Ch` after the dead-bit test with the shared `mov al,1 ; pop esi ; ret` exit as its fall-through and the `mov al,[esi+58h] ; test al,al ; setne al` tail last; the lane emits `jne` to a `mov al,1` exit placed after the setne tail (same instructions, block order only) | tried: `if (!p || p->m_dead) return true; return RF != 0;`, `if (p && !p->m_dead) return RF != 0; return true;`, `return !p || p->m_dead || RF;` (bool: full-register 0/1, 42 bytes), nested `if (p) { if (dead) return true; return RF != 0; } return true;` | next: the lander's optimize("s"/"t") sweep, then lane gap: block placement of a shared constant-return exit


#### `2026-10-15_linux_hand76` (6 parked; sources in `rebuild/candidates/manual/pending/2026-10-15_linux_hand76/differ/`)
- 004570d3 CSpecialAbilitiesBattleChargeDef::CSpecialAbilitiesBattleChargeDef DIFFER(61v61): `lea eax,[esi+90h]` placed before the +0x58 zero store (retail: before +0x60) | tried: SZeroed member object (65 bytes), body assignments, optimize("s") + PDB CRetailVector members (61v61), allocator-tag vector ctor | next: lane gap: lea scheduling (24i)
- 00457da9 CSpecialAbilitiesSummonSpellDef::CSpecialAbilitiesSummonSpellDef DIFFER(58v58): `lea ecx,[esi+64h]` hoisted above the three SummonTintColour zero stores (retail: before the last) | tried: optimize("s") (58v58), no pragma (61, -1 as a 7-byte mov) | next: lane gap: lea scheduling (24i)
- 00458c19 CMeleeCombatAbilityDef::CMeleeCombatAbilityDef DIFFER(45v45): `lea ecx,[esi+68h]` hoisted above the ValidWeaponClasses zero stores | tried: optimize("s") init-list vector, body-assignment vector ctor | next: lane gap: lea scheduling (24i)
- 004e00bc CQuestCardDef::CQuestCardDef DIFFER(45v45): `lea ecx,[esi+5Ch]` hoisted above the RewardObjects zero stores | tried: optimize("s") init-list vector | next: lane gap: lea scheduling (24i)
- 0045cfad CSoundMap::operator= DIFFER(49v49): last Dirty byte copied through cl with `pop edi ; mov eax,esi ; pop esi` between load and store (retail: through al before the pops) | tried: explicit memberwise outer + inner operator=, implicit outer operator=, void inner operator=, optimize("s") (out-lines the inner operator=, 33 bytes) | next: lane gap: register/scheduling (try `__forceinline` inner operator= under optimize("s"))
- 005bc6d6 CUIBackgroundOwner_005bc6d6::RenderBackground DIFFER(20v23): float argument re-pushed `mov eax,[esp+4] ; push eax` (retail `fld [esp+4] ; push ecx ; fstp [esp]`) | tried: plain float forwarder, /Op, 4-byte float wrapper with a user copy ctor, optimize("g",off) | next: optimize("s") (the unaligned 0x005BC6D6 start suggests a /Os TU), else lane gap: float argument copy

<!-- queue:end -->
