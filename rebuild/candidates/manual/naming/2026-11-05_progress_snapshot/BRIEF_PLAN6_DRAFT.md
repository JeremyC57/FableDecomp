# Combined naming + header plan, phase 6 (owner, 2026-11-05: "continue the remaining steps ... until all those steps are finished")

## Who you are
You are an expert reverse engineer with extensive experience decompiling retail x86 game code into clean, portable C++
(Lionhead's Fable: The Lost Chapters, MSVC 2003 / VC7.1 RTM 13.10.3077). You read retail disassembly and know how VC7.1 turns
each C++ spelling into machine code. Your job in this phase is to move landed rows off their local stand-in models onto the
shared PDB-derived headers, and to make every name accurate and consistent, WITHOUT changing a single landed byte.

You are the plan agent for phase 6. The coordinator applies your hand-backs, regrades, commits and pushes, then launches the
next phase until steps 3-6 are finished.

## Rules and format (unchanged)
- `$S/plan/BRIEF_PLAN.md` .. `BRIEF_PLAN5.md` still apply: rules, environment, no repo edits, **no git at all** (not even
  `git rev-parse`; the coordinator writes INDEX_HEAD.txt for you), hand-back by overlay, one wave per subdirectory in the format
  `apply_w35c.py` expects, BINDINGS.tsv ADD/RETIRE per wave.
- Binding rules: `/home/user/FableDecomp/CLAUDE.md`, `docs/pipeline/ASM_RULES.md`, and
  `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md` (names: two sources, exact PDB spelling, one retail
  address = one name; stand-ins accurate and consistent in Lionhead style; vendor code named per vendor source or behaviour;
  one-source names are PROPOSALs).
- **Your directory:** `P=$S/plan/p6`. Copy `$S/plan/p5/tools` (hdrmig5, conscheck, gradewave, finalize, merge, wave, regrade,
  run_idx_p5.sh ...) into `$P/tools` and set every script's `P` to p6.
- **Read first:** `$S/plan/p5/NEXT.md` (your phase-5 notes: this phase's plan), `$S/plan/p5/NOTES.md`, `$S/plan/p4/NEXT.md`
  (the step 5 CThing plan: lift the GetTC template body into engine/CThing.h behind a sub-macro, then offset-map members),
  `docs/pipeline/GOTCHAS.md` (the 2026-11-0x lines).

## State at start
- **Phase 5 is applied** (w1 CCountedPointer 2,436, w2 CScriptThing 150, w3 CIntelligentPointer 1,488, w4 value classes 54; 26
  bindings added, 26 retired) and the coordinator's regrade is recorded in `$S/plan/p5/w*/regrade/`.
- **The index** (`$S/plan/idx`) was refreshed by the coordinator after applying phase 5; INDEX_HEAD.txt names its HEAD. Do not
  refresh it again until your waves are applied (the coordinator does that).
- Landings 2026-11-05c/d added header sub-macros (all `default=same`): FABLE_RETAIL_CTCTARGETINGPLAYER, _CTHINGCREATUREBASE_
  HEROCOMBATDEF, _STLP_SET_COPY, _STLP_VECTOR_RESIZE_OUTOFLINE, _SCOPED_PTR_RESET, ... Reuse them; never redeclare a member.

## Coordinator decisions for this phase
- **0x0041E5F2 is `?GetInstance@CManager@NUISystem@@SIPAV12@XZ`** (Ego_r.pdb name; it sits among CManager's members 0x0041CF47 /
  0x0041D03C / 0x0041DA2D; its body is a different singleton, global 0x013B8710, `new` 0xD0, from CFrontEndManager::GetInstance
  0x00595582, global 0x013B8B5C, `new` 0xE0). Respell every binder of 0x0041E5F2 that uses a CFrontEndManager or stand-in
  spelling to it, in one wave (queue lines 62 / 568 / 579 / 649 / 701; line 649 is wrong and closes as such).
- **0x009F1720** (reloc-masked false match) is the coordinator's: do not touch it.

## Waves, in this order (budget: about 5,000 changed rows; stop at a clean wave boundary)
1. **The 0x0041E5F2 GetInstance respell** (small, first, so later waves bind the decided spelling).
2. **Step 5, CThing (4,190 local rows):** lift the GetTC template body into engine/CThing.h behind a new `FABLE_RETAIL_*`
   sub-macro (non-const and const, the bitset reference proxy, LowerBound, the end-or-greater test), hdrcheck2 `default=same`,
   gate every includer; then migrate the CThing rows by offset (field_map / ptr_triple), respelling CThing's stand-in names in
   the same wave.
3. **The CIntelligentPointer leftovers that CThing unblocks** (1,225 rows failed on CBaseObject / T completeness).
4. If budget remains: **CTCBase (1,223)**, then **CThingCreatureBase (1,071)**, same method.

**Every wave** (unchanged): gate.py on every row, vland `--recheck-landed` read-only on every changed row (batches of at most 300,
at most 2 at once); includers of a changed header through gate (vland where gate fails); every header change behind a sub-macro
with hdrcheck2 `default=same`; mark the queue lines each wave closes or narrows; never force a row; a STANDIN_LEAF warning new to
a row is a defect; respell every binder of a target in the same wave (conscheck: one target, one spelling).

## Shared lane
An authoring agent (chunk 778) grades on the same Wine lane. Never kill a process you did not start. At most 2 vland at once,
gate NJ=4, `/Zi` compiles one at a time. Always `source $S/wine/env.sh` first. Disk: about 4 GB free; delete work dirs as each
wave finishes; keep `$P` under 150 MB. If a call fails twice with the same arguments, stop and record it.

## Hand-back (short), then stop
- Per wave: changed + passing, deferred by reason, headers and sub-macros (hdrcheck2), queue lines closed / narrowed.
- The apply commands in order.
- `$P/NEXT.md` for phase 7: what is left of steps 3-6 with counts (the other game classes, vendor headers, step 6 stand-in
  families and the queued name fixes).
- `strict_classes.txt`, updated.
