# Agent NF338 (Account A naming queue, 2026-11-24): T1 batch (fresh entries, all fix kinds)

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md
(read it in full, standing rulings included: this is exactly the T1 work that brief was written for).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP` in every Bash call
  (a TMPDIR inside the scratchpad makes Wine cl segfault); `mkdir -p /run/user/0` if missing.
- Your folder: $S/w22/agentNF338/ . Agent name for lock.sh: NF338. Donor builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (evidence only).
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.

## Scope
$S/w22/agentNF338/ENTRIES.tsv: 320 open T1 tracker entries (OPEN.tsv columns; 130 classes; fix kinds standin_alias 144, other 91, signature 83, access 2), none
attempted by NF332..NF337: callers' spellings only (the row already defines the PDB name, or there is no landed row). Read first: NF334's and NF332's landings
(`batches/2026-11-24_landing_nf33{2,4}/README.md`, `pending/2026-11-24_nf33{2,4}_handback/`), the standin_alias precedent (nf291 landing README) and the T1
"other" / signature precedents (nf305 / nf308 landing READMEs). Reuse NF332 / NF334 tools (`naming_batch/agent_tools/nf33{2,4}/`). Batch by class (BATCHES.tsv).
- A fix is in place: the binder's local model (or its shared header / an existing opt-in) gives the call the PDB spelling, objects byte-identical (objsec ALL_EQ,
  `.drectve` included) and the table line respelled (TABLE_CHANGES.tsv); before any `replace`, ref_scan.py proves no other landed row still uses the old spelling
  (else add + rewitness, NF334 landing). A respell that moves a call onto a DIFFERENT wrong form is a port (NF305 Q1). Stand-in parameter types, a class port,
  a new header: T3 (STATUS skipped-port, reason).
- A caller that binds a DIFFERENT function than retail calls (wrong target) is a fix target: respell it to the PDB name of the real target and record it in NOTES.
- Hand back after about 60 changed rows (or sooner); STATUS.tsv covers every entry you reached.

## Stay off
- B's claimed rows (COORDINATION_CLAIMS.tsv newest line per item, B, not done / released) and pending/2026-11-24_acctB_b24/; Codex pass 26 / 27 / 28 RESERVED rows.
- Agents NF336 (CV_HauntedHouseScript object 0x00E93D50..0x00E96E10, 0x00E95380; engine/CV_HauntedHouseScript.h, engine/TRetVoid.h) and NF337 (the `?Copy@<Def>` rows
  in $S/w22/agentNF337/ENTRIES.tsv and their operator= rows) run at the same time: skip their locks (lock owners NF336 / NF337), their entries, and do not edit
  those headers or any `*Def.h` header those agents' rows include without checking their locks first.
- CScriptThing destructor / copy-ctor / operator= definers (NF326 / NF328 / NF331), and every T3_PORT_QUEUE line.

## Hand-back
As in the standing brief, to $S/w22/agentNF338/. Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS BEFORE your first compile. Keep files current as you go
(container restarts happen).
