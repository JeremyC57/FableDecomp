# Agent NF366 (Account A naming queue, 2026-11-24): respell the eleven rows that still define the @DeleteIfSet@4 stand-in

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full). Lock name: NF366.
Folder: $S/w22/agentNF366/. Names rule: tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md (names right the first time:
two sources, exact PDB spelling; one retail address, one name).

Read first: NF360's hand-back rebuild/candidates/manual/pending/2026-11-24_nf360_handback/ (NOTES.md, NAME_FIXES.tsv) and the landed form
of 0x00428B10 (batches/2026-11-24_landing_nf360_b2/README.md and its row): an explicit specialization of engine/CCountedPointer.h's
`DeleteData` under FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY. The NAME_FIXES_QUEUE lines tagged nf360 list the rows.

Rows (each a landed gapscan row whose source defines the extern "C" `@DeleteIfSet@4` stand-in): 00447660 0044ac20 00991e10 009a6c00 00a0bb40
00a28370 00a281e0 00aa4310 00b3e080 00cd42e0 00f38920.
Per row:
1. Identify T with two sources: the retail site that stores this address as a count block's DeleteFunc (`mov dword ptr [eax+4], <addr>` in
   a CCountedPointer<T> constructor / Reset / ShareData; scan the retail exe), the PDB symbol set (FableWin.pdb / Ego_r.pdb names
   `?DeleteData@?$CCountedPointer@V<T>@@@@SIXPAX@Z`), reloc_bindings.tsv lines, and agentFAM10's ~60 renamed siblings (grep DeleteData in
   rebuild/src/compiled). A row whose T is not proven by two sources is skipped with the evidence found.
2. Respell in place in the 0x00428B10 form, with the PDB spelling; keep the landed body. Forward-declare T only where its shared header would
   not change the bytes; prefer `#include "engine/<T>.h"` when one exists (shared-headers rule).
3. Gates: vland grade unchanged (MATCH / RELOCATION_MATCH before and after); objsec: only the self name changes; both checkers HEAD v new:
   nothing turns NEQ, no new NEQ, 0 reach flips; drop test on the stand-in (reloc_bindings lines, other landed objects that reference it);
   a link of the row with its retail binders and every other landed definer of the new name: report duplicates and whether their bodies
   are identical. A new duplicate with a different body holds the row (report it; the coordinator rules).
4. NAME_FIXES.tsv for any landed spelling you prove wrong; RENAMES.tsv for file renames (proposals only).


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
