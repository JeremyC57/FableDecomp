# Agent NF369 (Account A naming queue, 2026-11-24): 0x00749A70 in the option-A form (NF367-Q2 ruling)

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full). Lock name: NF369.
Folder: $S/w22/agentNF369/.

Read first: NF367's hand-back rebuild/candidates/manual/pending/2026-11-24_nf367_handback/ (NOTES.md Part 2, held/, evidence/ incl. the p2e /
p2f measurements) and its tools in tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf367/ (pk_addrs.tsv, pkpair.sh,
links367*.sh); the precedent batches/2026-11-24_landing_nf363_b2/README.md and batches/2026-11-24_landing_acctB_b24f (the `// Pick-any:` note).

Ruling (advisor decision, owner delegated, 2026-11-24): NOT the ?A0x helper (P2h). Option A: the row 0x00749A70 (graded on its leaf, the
implicit `??1CTCNoise`) carries 0x00749810's landed text (the CTCNoise ctor on the shared headers), so its object emits the same ctor, vftable
and ~CTCNoise. **The deciding check:** objcmp the new 0x00749A70 object against 0x00749810's landed object (HEAD build): if every section is
SAME (bytes, relocations, selection; only .debug$S may differ), it can land; if anything else differs, hold and keep HEAD.
Then the `??0CTCNoise` duplicate has identical bytes (the nf184 LNK2005 pattern), `??_7CTCNoise` becomes EQ (T3 nf367 0x01268A74), and the
two NEQ CTCNoiseDef copies (??0 29 B, ??_G 30 B) enter under the identical-HEAD-copy exception: identical to 0x00749810's HEAD copies; add the
`// Pick-any:` note to the row as 0x00DEE2E0 did, naming the copies and T3 nf359 0x0044C1BB / 0x0044F71B.
Gates for the run dir:
- vland grade HEAD v new (RELOCATION_MATCH / MATCH, PASS), with the row's test on the shared headers (rewrite it if it uses a local model);
- both checkers HEAD v new, run WITH tools/pk_addrs.tsv (plain pk_all reads ??_GCTCNoiseDef as NOADDR): no copy turns NEQ, no new NEQ other
  than the two identical-HEAD copies covered by the exception (show them byte-equal to 0x00749810's), 0 reach flips;
- drop test on everything HEAD defined that the new object does not (the 59 B ctor, the old ??_7CTCNoise, any helper);
- a link of the new object with 0x00749810 and the other CTCNoise emitters NF367 did not compile (0x00749A50, 0x004D3658, and any other
  landed definer of ??0CTCNoise / ??_7CTCNoise / ??1CTCNoise you find by scan): report every duplicate; one with a DIFFERENT body holds the row.
- If the row's catalog leaf / self / file names change, RENAMES.tsv (proposals) and NAME_FIXES.tsv.


Other agents running (stay off their rows): NF366 the eleven @DeleteIfSet@4 rows (00447660 0044ac20 00991e10 009a6c00 00a0bb40 00a28370
00a281e0 00aa4310 00b3e080 00cd42e0 00f38920); NF369 0x00749A70 (0x00749810 read-only, plus the CTCNoise emitters it links); NF370 the T0 / T2
addresses in $S/t02.txt. If one of your addresses is in another agent's list, skip it.

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
