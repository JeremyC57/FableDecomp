# Agent NF362 (Account A naming queue, 2026-11-24): the size-pragma emitters of the reopened Reset<CComponent> and GetDef copies

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read it in full (NF340 rulings at its end,
NF352 entry). Lock name: NF362. Folder: $S/w22/agentNF362/.

Read first: NF358's census, rebuild/candidates/manual/pending/2026-11-24_nf358_handback/ (NOTES.md, STATUS.tsv, evidence/CENSUS_*.tsv), and
the T3 lines tagged nf358 at the end of rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv. Do not repeat the census.

## The group: TUs that set optimize pragmas and use the shared-header body
- Reset<NUISystem::CComponent> 0x00429C6F, 62/59 B, all `optimize("y", off)` + `("s", on)` TUs: 00427216 00582ace 0058315b 0058a0c7
  0058e208 005907ab 0059cb90 0059d270 0059ed42 0059f51e 0059fee3 005a29f4 005a8c66 005a9cbe 005aaf28 005ad571 005cdd86 005d64dd 005d6748
  005d7081. NF358: the 25 EQ emitters use `("s", on)` alone or NF352's size-body opt-in.
- GetDef<NUISystem::CUIMiscThingsDef> 0x0042B312, 101/98 B: 0058578b 005a04f7 005c6b26 005c8459 005c90fc 005c9a95 005cacb9 (`("s", on)`
  alone) and 005a29f4 005a8a5d 005a9cbe 005aaf28 005ad571 005c7452 (`("y", off)` + `("s", on)`).
- GetDef<CThingObjectDef> 0x0042B040, 101 or 105/98 B: 005b3aef 005b76d1 005b8743 0041e6d3 (`("s", on)` alone) and 00584b86 (both).

## Step 1 (prove before any fix): why each sub-group differs
For each copy find the exact optimize state the owner's COMDAT is compiled in (owner source pragmas and CompilerFlags; NF352's block as
used by 0x00605570 / 0x00626110) and the state at the copy's point of instantiation in each sub-group TU (preprocess the TU: where do the
row's pragmas sit relative to the include?). Measure on 2-3 rows whether NF352's existing opt-in (Reset) or the NF349 block (GetDef) gives
an EQ copy in this TU as it stands. If not, name the pragma state that differs. Write this into NOTES.md before fixing anything at scale.

## Step 2: fix only with an existing opt-in or a byte-neutral move
Allowed: the existing NF352 / NF349 opt-ins; an include move where the move alone is byte-neutral (control compile). Not allowed in this
brief: a new header device, a change to an existing header block, or any change to the row's own pragmas (that changes the row's leaf). A
sub-group that needs one of those is STATUS skipped with the measurement and a proposal for the coordinator.
Per row: grade unchanged; objsec equal except the fixed copies, leaf identical; the fixed copy equal to the owner's COMDAT (compare against
the owner object); both checkers HEAD v new: nothing turns NEQ, no new NEQ, 0 reach flips. Rows' own local bodies are out of scope.

Other agents running (stay off their rows): NF361, the /O2 group of the same copies: 0047a9f0 005fb7e0 005fc2d0 005fc8a0 005fcbe0 005fddd0
005fe090 005ff190 006001e0 00600b60 00603000 00605570 00607330 006096e0 0060c900 0060eeb0 0061b610 0061e6d0 00622a00 00868e50, then a
read-only census of 0x0066C340. NF363: 0x00455EF0 0x0044C1BB 0x0045C5DA 0x0045C5F6 0x0045C5C1 0x0042AF3C and 0x005B43A2 0x005B4795
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
