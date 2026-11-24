# Agent NF367 (Account A naming queue, 2026-11-24): 0x00749810's NEQ CTCNoiseDef copies (T3 nf359 0x0044C1BB PRIORITY, 0x0044F71B, 0x00749A70)

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full). Lock name: NF367.
Folder: $S/w22/agentNF367/.

Read first: NF359's and NF363's hand-backs (rebuild/candidates/manual/pending/2026-11-24_nf359_handback/, _nf363_handback/; the landed unit
batches/2026-11-24_landing_nf363_b2/README.md), engine/CThingNoiseDef.h, engine/CTCNoiseDef.h, and the T3 lines tagged nf359.

Background: since nf363_b2 the owners 0x0044C1BB `??0CTCNoiseDef@@QAE@XZ` (18 B) and 0x0044F71B `??_GCTCNoiseDef@@UAEPAXI@Z` (34 B) are EQ
pick-any copies, so 0x00749810's NEQ copies (29 B and 30 B: its CSubDefClassBase ctor is expanded; `??1CTCNoiseDef` is called out of line
because the header's default `virtual ~CTCNoiseDef();` is used without _IMPLICIT_DTOR) now win silently whenever 0x00749810 links first.
Also: `??0CTCNoise@@QAE@AAVCThing@@@Z` is defined strongly by 0x00749810 (79 B, its self) and by 0x00749A70 (59 B): one symbol, two bodies.

## Part 1: make 0x00749810's two copies equal to the owners
Find the minimal fix with existing opt-ins first (the CTCNoiseDef.h / CThingNoiseDef.h / CThingBaseDef.h opt-ins nf355 / nf363 used:
_IMPLICIT_DTOR, the ctor forms), else a byte-neutral include change. Gates: grade unchanged; objsec equal except the fixed copies, leaf
identical; each fixed copy byte- and relocation-equal to the owner object (0x0044C1BB / 0x0045C5F6's landed objects); both checkers HEAD v
new: nothing turns NEQ, no new NEQ, 0 reach flips; drop test. A fix that needs a new device is skipped with the measurement.
## Part 2 (diagnosis): the ??0CTCNoise strong duplicate
Which body is retail's (0x00749810's self at 79 B; compare 0x00749A70's 59 B copy)? Why does 0x00749A70 define it (its TU emits ~CTCNoise
0x00749A70)? Propose the fix (e.g. 0x00749A70 declares the ctor) with a measured probe: grade unchanged, objsec, both checkers, link of
the two objects. Hand it back as a run dir only if every gate passes.


Other agents running (stay off their rows): NF366 the eleven @DeleteIfSet@4 rows (00447660 0044ac20 00991e10 009a6c00 00a0bb40 00a28370
00a281e0 00aa4310 00b3e080 00cd42e0 00f38920); NF367 0x00749810 0x00749A70 (owners 0x0044C1BB / 0x0045C5F6 read-only); NF368 0x0060C900
0x0060EEB0 (owner 0x005FF190 read-only). Release any read-only owner lock as soon as its build is done.

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
