# Agent NF364 (Account A naming queue, 2026-11-24): the P1 sibling of NF352's Reset block for y-off size TUs

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full, including the NF352 and
NF361 / NF362 entries at its end). Lock name: NF364. Folder: $S/w22/agentNF364/.

Read first: NF362's hand-back rebuild/candidates/manual/pending/2026-11-24_nf362_handback/ (NOTES.md Step 1, evidence/probe_block.diff,
evidence/probe_*), its tools in tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf362/ (pragscan362.py etc.), and
docs/pipeline/GOTCHAS.md's 2026-11-24 SIZE_BODY line.

## Part 1: the block (advisor decision, owner delegated: P1 approved with these conditions)
Add to engine/CCountedPointer.h, add-only, a sibling of FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY for TUs that have already
set `optimize("y", off)` + `("s", on)`: it sets `("y", on)` + `("s", on)`, defines the same Reset<NUISystem::CComponent> specialization
(token for token the existing block's body), then restores `("y", off)` + `("s", on)` instead of `("", on)`. Conditions:
- its own macro name (descriptive, in the existing naming pattern), marked `// Fidelity: scaffolding, not retail`, with the reason;
- an `#error` if it is defined together with the existing RESET macro;
- pragscan on every user row: the optimize state before the block equals the state the block restores (the restore is hard-coded);
- per row: exactly one COMDAT changes (Reset 62 -> 59 B), byte- and relocation-equal to the owner 0x00429C6F's object; leaf identical;
  grade unchanged (vland RELOCATION_MATCH / MATCH before and after); both checkers HEAD v new: nothing turns NEQ, no new NEQ, 0 reach flips;
- the whole-catalog equiv_check (tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py, old v new include tree) shows 0
  differing rows, and a planted control in the new block is seen in exactly the user rows.
Rows (19): 00427216 00582ace 0058315b 0058a0c7 0058e208 005907ab 0059cb90 0059d270 0059ed42 0059f51e 0059fee3 005a8c66 005d64dd 005d6748
005d7081, and 005a29f4 005a9cbe 005aaf28 005ad571 (landed today with REREAD for GetDef in nf362_b1; their Reset copy is still 62 B). Use the
repo text as it is now. If ReleaseData also needs the y-on body in some row (NF352 Q1b shape), measure it and report; do not add a second
block without a ruling.

## Part 2 (optional, one row, diagnosis only): 00603000
NF361 skipped the /O2 rows 005ff190 00603000 00607330 0061b610 0061e6d0 because the `__declspec(noinline)` Reset block moves their leaf
(the leaf inlined Reset). Probe on 00603000 only: the same size-mode specialization WITHOUT `__declspec(noinline)` (a scratch header copy,
not a hand-back). It passes if the leaf is identical and the Reset copy is 59 B EQ. Report the measurement for a ruling; do not scale it.

Other agents running (stay off their rows): NF365: 0047a9f0 005fc2d0 005fc8a0 005fcbe0 005fddd0 005ff190 00605570 00607330 006001e0
00600b60 0060c900 0060eeb0 0061b610 0061e6d0 00622a00 00868e50 (Part 2's 00603000 is yours). The owner 0x00429C6F may be compiled
read-only by both of you; release its lock as soon as your owner build is done.

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad. In every Bash call run
  `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`. Run `mkdir -p /run/user/0` if it is missing.
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.
- Checkers: run both, HEAD against new, on every row you change.
  - rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py, with python3 -I.
  - tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py. It needs env FABLE_EXE=<retail exe> and
    FABLE_ROOT=/home/user/FableDecomp, plus --row-length=<catalog length> and --leaf=<the row's // Leaf: symbol>.
  - Diff the reach column HEAD v new: an already-NEQ copy whose reach flips n -> y holds the row (NF352 rule). `$S/tools/reachflip.py <dir>`
    pairs H_<addr>.pkall.tsv / N_<addr>.pkall.tsv in one folder.
- awk gotcha: compare addresses as strings (`$1""==a""`); `00e07640`-shaped values compare numerically otherwise.

## Hand-back
- land_scripted run dirs under batches/bN/: rows/, RESULTS.tsv (ACCEPT only when every gate passes), head/payload.json, TABLE_CHANGES.tsv,
  orig/ (rows + tests), tests/ if changed, hdr/engine/ plus orig/hdr_<name> and HEADERS.tsv for any header edit. Put prose notes in NOTES.md,
  never in head/HEAD_FROM_REPO.txt (land_scripted reads that file as an address list).
- NOTES.md (decisions, measurements), STATUS.tsv, evidence/, tools/ (every helper you wrote).

Rules:
- Read docs/COORDINATION.md, docs/pipeline/ASM_RULES.md, tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md and
  docs/pipeline/GOTCHAS.md before the first compile. Byte purity: no __asm / naked / _emit; `volatile` only where retail used it.
- Locks: $S/w22/nflocks/<addr>/owner holds the owner name; claim only a lock that is missing or reads `released`, release by writing
  `released`. Precheck a row's test before claiming its lock, and its source after claiming and before compiling.
- Stay off: Account B's claimed or staged rows (rebuild/candidates/manual/COORDINATION_CLAIMS.tsv, pending/*acctB*), Codex reservations,
  and the other running agents' rows (listed above).
- Never write into the repo and never run git. Run long jobs in the background; no sleep loops.
- Loop prevention: never repeat a failing call more than twice with the same arguments.
