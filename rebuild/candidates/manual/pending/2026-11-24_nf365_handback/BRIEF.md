# Agent NF365 (Account A naming queue, 2026-11-24): the GetDef "swap" for the /O2 rows with a row-local GetDef template

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full, including the NF361 /
NF362 entry at its end). Lock name: NF365. Folder: $S/w22/agentNF365/.

Read first: NF361's hand-back rebuild/candidates/manual/pending/2026-11-24_nf361_handback/ (NOTES.md, held/g1 with the 005fc2d0 / 00868e50
swap texts and evidence, evidence/variants/), its tools in tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf361/.

The swap: a row deletes its local `template<class T> GetDef(long, ...)` and defines FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY +
FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD + the NF349 per-type size-body macro, so its GetDef<T> copy (116 B) becomes the owner's 98 B.
Ruling (advisor decision, owner delegated, 2026-11-24): allowed as a copy repair gated by the drop test, ON ONE CONDITION checked first.

## Step 1: footprint check (decides everything else)
The swap also changes helper COMDATs (SetDynamicCast 50 -> 52 B, Assign / IsNull helpers dropped, a 3 B `CDefPointer<CDefClassBase>::Get`
added). Find already-landed rows that use GLOBAL_BODY + REREAD + an NF349 size body (grep the defines in rebuild/src/compiled; e.g. the
nf349 batches), compile two of them from HEAD, and compare: are the swapped objects' helper COMDATs (each SetDynamicCast<T>, Get, and any
other added helper) byte- and relocation-equal to the same-named COMDATs in those landed rows? If equal, the swap brings the shared body's
normal footprint and you continue. If any differs, stop: STATUS skipped for every row with the comparison, and hand back.

## Step 2: the swap, per row
Rows: 005fc2d0 00868e50 (NF361 held/g1), then 0047a9f0 005fc8a0 005fcbe0 005fddd0 006001e0 00600b60 0060c900 0060eeb0 0061b610 0061e6d0
00622a00; measure 005ff190, 00605570, 00607330 separately (their leaves likely expand GetDef). Repo text as it is now (006001e0 / 00600b60
gained the NF352 Reset defines today in nf361_b1). Per row:
- grade unchanged (vland RELOCATION_MATCH / MATCH before and after); leaf identical; target GetDef copy byte- and relocation-equal to the
  owner object (0x0042B312 / 0x0042B040, plus any other GetDef<T> copy the row emits, e.g. 00868e50's GetDef<CArmourDef> 0x00817B40);
- both checkers HEAD v new: nothing turns NEQ, no new NEQ, 0 reach flips;
- drop test on every dropped external symbol (agent_tools/nf357/droptest.py form or the coordinator's agent_tools/coordinator/droprow.py):
  no reloc_bindings line, no landed object needing it, no EQ copy with a retail address lost;
- a row whose leaf moves is skipped with the measurement.

Other agents running (stay off their rows): NF364: 00427216 00582ace 0058315b 0058a0c7 0058e208 005907ab 0059cb90 0059d270 0059ed42
0059f51e 0059fee3 005a8c66 005d64dd 005d6748 005d7081 005a29f4 005a9cbe 005aaf28 005ad571, and a one-row probe on 00603000. Owners
0x0042B312 / 0x0042B040 / 0x00429C6F may be compiled read-only; release their locks as soon as your owner builds are done.

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
