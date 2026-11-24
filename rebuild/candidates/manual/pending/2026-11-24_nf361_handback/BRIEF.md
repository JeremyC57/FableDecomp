# Agent NF361 (Account A naming queue, 2026-11-24): the /O2 emitters of the reopened Reset<CComponent> and GetDef copies

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read it in full (NF340 rulings at its end,
NF352 entry). Lock name: NF361. Folder: $S/w22/agentNF361/.

Read first: NF358's census, rebuild/candidates/manual/pending/2026-11-24_nf358_handback/ (NOTES.md, STATUS.tsv, evidence/CENSUS_*.tsv), and
the T3 lines tagged nf358 at the end of rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv. Do not repeat the census.

## Part 1: the /O2 group (TUs with no optimize pragma, shared-header bodies)
Rows (copy, our size / retail size):
- Reset<NUISystem::CComponent> 0x00429C6F, 103/59 B: 005fb7e0 005fe090 005ff190 006001e0 00600b60 00603000 006096e0 0060c900 0060eeb0,
  plus unreached 00607330 0061b610 0061e6d0 00622a00.
- GetDef<NUISystem::CUIMiscThingsDef> 0x0042B312, 116/98 B: 0047a9f0 005fc2d0 005fc8a0 005fcbe0 005fddd0 006001e0 00600b60 0060c900
  0060eeb0 0061b610 0061e6d0 00622a00, plus unreached 005ff190 00605570.
- GetDef<CThingObjectDef> 0x0042B040, 116/98 B: 00868e50, plus unreached 00607330.
Use the existing opt-ins only:
- Reset: FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY together with _RELEASEDATA_SIZE_BODY (engine/CCountedPointer.h, NF352;
  landed users 0x00605570, 0x00626110).
- GetDef: the NF349 per-type size-body blocks (grep NF349 / SIZE_BODY in engine/CDefinitionManager.h). If no block exists for a type, skip
  those rows with that measurement; do not add a header block in this brief.
Per row: grade unchanged (vland RELOCATION_MATCH or MATCH before and after); objsec equal except the fixed copies, leaf identical; the fixed
copy byte- and relocation-equal to the owner's COMDAT (compare against the owner object, as NF357 did); both checkers HEAD v new: nothing
turns NEQ, no new NEQ, 0 reach flips. A row that also emits another NEQ copy keeps it (record it). C2908 ordering: a specialization must
precede the TU's first use; an include move is allowed only where the move alone is byte-neutral (control compile).
Rows' own local GetDef / Reset bodies (for example 005fb7e0's 51 B GetDef) are out of scope for that copy.

## Part 2 (read-only, after Part 1): recount for the closed line acctB_b24B6 0x0066C340 GetDef<CMaterialDef>
nf347 closed it (batches/2026-11-24_landing_nf347_b1/T3_CLOSED.tsv) as "fully repaired" without a census. Do a census in NF358's form
(retail E8/E9 sites to 0x0066C340 mapped to landed rows, plus a source grep of GETDEF_GLOBAL_BODY / GetDef<CMaterialDef> users, HEAD
compile, pk_all + pvr) and report REOPEN or STAYS CLOSED with the emitter list. No fixes in Part 2.

Other agents running (stay off their rows): NF362, the size-pragma group of the same copies: 00427216 00582ace 0058315b 0058a0c7 0058e208
005907ab 0059cb90 0059d270 0059ed42 0059f51e 0059fee3 005a29f4 005a8c66 005a9cbe 005aaf28 005ad571 005cdd86 005d64dd 005d6748 005d7081
0041e6d3 00584b86 0058578b 005a04f7 005a8a5d 005b3aef 005b76d1 005b8743 005c6b26 005c7452 005c8459 005c90fc 005c9a95 005cacb9. NF363:
0x00455EF0 0x0044C1BB 0x0045C5DA 0x0045C5F6 0x0045C5C1 0x0042AF3C and the GetDef<CThingBaseDef> strong copiers 0x005B43A2 0x005B4795
0x005B4EBA 0x005B5BE6 0x00659164 0x0071DA70 0x007744A0 0x00801A50 0x00811470 0x008115A0 0x008ED1F0.

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
