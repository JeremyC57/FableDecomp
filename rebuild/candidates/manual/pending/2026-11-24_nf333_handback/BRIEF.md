# Agent NF333 (Account A naming queue, 2026-11-24): T1 / T2 batch

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md
(read it in full, standing rulings included: this is exactly the T1 / T2 work that brief was written for).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP` in every Bash call
  (a TMPDIR inside the scratchpad makes Wine cl segfault); `mkdir -p /run/user/0` if missing.
- Your folder: $S/w22/agentNF333/ . Agent name for lock.sh: NF333. Donor builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (evidence only).
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.

## Scope
/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333/ENTRIES.tsv: 173 open tracker entries (OPEN.tsv columns), the continuation of NF332 (landed 2026-11-24, `batches/2026-11-24_landing_nf332/README.md`
and `pending/2026-11-24_nf332_handback/`: read both first, and reuse NF332's tools in `naming_batch/agent_tools/nf332/`): T1 / T2 access (10), T1 standin_alias (~89),
T1 signature (~74). NF332's skipped / partial entries are excluded. Order: access, standin_alias, signature; batch by class.
- `signature` entries: in place only when byte-identical (objsec ALL_EQ, `.drectve` included), both PDB name sets agree on the signature, and every landed binder of
  the old spelling moves in the same landing (NF304 Q6). Parameter stand-in types, a class port or a new header are T3: STATUS skipped-port with the reason.
- Self renames onto the PDB name with every binder moving in the same landing are accepted (NF332 D3 ruling, NF304 Q6). A self moving onto a made-up owner is T3 (NF282 D2).
- Hand back after about 60 changed rows (or sooner).

## Stay off
- B's claimed rows (COORDINATION_CLAIMS.tsv newest line per item, B, not done / released) and pending/2026-11-24_acctB_b24/; Codex pass 26 / 27 / 28 RESERVED rows.
- Agent NF334 runs at the same time on other tracker entries (lock owner NF334): skip its locks and every address in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF334/ENTRIES.tsv.
- CScriptThing destructor / copy-ctor / operator= definers (units NF326 / NF328 / NF331, just landed), and every T3_PORT_QUEUE line.

## Hand-back
As in the standing brief, to $S/w22/agentNF333/. Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS BEFORE your first compile (two earlier agents
slipped on this). Keep files current as you go (container restarts happen).
