# Agent NF332 (Account A naming queue, 2026-11-24): T1 / T2 batch (access, standin_alias, signature)

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md
(read it in full, standing rulings included: this is exactly the T1 / T2 work that brief was written for).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP` in every Bash call
  (a TMPDIR inside the scratchpad makes Wine cl segfault); `mkdir -p /run/user/0` if missing.
- Your folder: $S/w22/agentNF332/ . Agent name for lock.sh: NF332. Donor builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (evidence only).
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.

## Scope
$S/w22/agentNF332/ENTRIES.tsv: 239 open tracker entries (OPEN.tsv columns), already screened against T3_PORT_QUEUE (column 2 and unit column 3), live locks and
Codex pass 26-28 reservations at hand-out: T1 access (25), T2 access (22), T1 standin_alias (91), T1 signature (101). Order: access first (T1, then T2),
then standin_alias, then signature. Batch by class where several entries share one (BATCHES.tsv). Re-screen each entry's BINDERS / TARGETS against the
stay-off lists before locking (NF309 Q5).
- `signature` entries are new ground for the T1 standard: a fix is in place only when byte-identical (objsec ALL_EQ, `.drectve` included), both PDB name sets
  agree on the signature, and no landed row binds the old spelling after the landing (or every binder moves in the same landing, NF304 Q6). Parameter
  stand-in types, a class port or a new header are T3: STATUS skipped-port with the reason.
- Hand back after about 60 changed rows (or sooner), as the standing brief says; the coordinator lands with naming_batch/apply_rows.py + apply_table.py.

## Stay off
- B's claimed rows (COORDINATION_CLAIMS.tsv newest line per item, B, not done / released) and pending/2026-11-24_acctB_b24/; Codex pass 26 / 27 / 28 RESERVED rows.
- Agent NF330 runs at the same time (combat / creature-action flat-header rows, lock owner NF330): skip its locks and every address in
  $S/w22/agentNF330/ENTRIES.txt.
- CScriptThing destructor / copy-ctor / operator= definers (units NF326 / NF328 / NF331, just landed), and every T3_PORT_QUEUE line.

## Hand-back
As in the standing brief, to $S/w22/agentNF332/. Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS BEFORE your first compile (two earlier agents
slipped on this). Keep files current as you go (container restarts happen).
