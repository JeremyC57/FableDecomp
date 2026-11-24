# Agent NF334 (Account A naming queue, 2026-11-24): T1 `other` batch

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md
(read it in full, standing rulings included: this is exactly the T1 / T2 work that brief was written for).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP` in every Bash call
  (a TMPDIR inside the scratchpad makes Wine cl segfault); `mkdir -p /run/user/0` if missing.
- Your folder: $S/w22/agentNF334/ . Agent name for lock.sh: NF334. Donor builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (evidence only).
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.

## Scope
/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF334/ENTRIES.tsv: 329 open T1 `other` tracker entries (OPEN.tsv columns): callers' spellings only (the row already defines the PDB name, or there is no
landed row). Read NF332's landing first (`batches/2026-11-24_landing_nf332/README.md`, `pending/2026-11-24_nf332_handback/`) and the T1 "other" precedents
(nf305 / nf308 landing READMEs: class / namespace / form respells). Reuse NF332's tools (`naming_batch/agent_tools/nf332/`). Batch by class (BATCHES.tsv).
- A fix is in place: the binder's local model (or its shared header / an existing opt-in) gives the call the PDB spelling, objects byte-identical (objsec ALL_EQ,
  `.drectve` included) and the table line respelled (TABLE_CHANGES.tsv). A respell that moves a call onto a DIFFERENT wrong form is a port (NF305 Q1).
  Stand-in parameter types, a class port, a new header: T3 (STATUS skipped-port, reason).
- A caller that binds a DIFFERENT function than retail calls (wrong target, as NF332 found at 0x008915B0) is a fix target: respell it to the PDB name of the
  real target and record it in NOTES.
- Hand back after about 60 changed rows (or sooner).

## Stay off
- B's claimed rows (COORDINATION_CLAIMS.tsv newest line per item, B, not done / released) and pending/2026-11-24_acctB_b24/; Codex pass 26 / 27 / 28 RESERVED rows.
- Agent NF333 runs at the same time on other tracker entries (lock owner NF333): skip its locks and every address in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333/ENTRIES.tsv.
- CScriptThing destructor / copy-ctor / operator= definers (units NF326 / NF328 / NF331, just landed), and every T3_PORT_QUEUE line.

## Hand-back
As in the standing brief, to $S/w22/agentNF334/. Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS BEFORE your first compile (two earlier agents
slipped on this). Keep files current as you go (container restarts happen).
