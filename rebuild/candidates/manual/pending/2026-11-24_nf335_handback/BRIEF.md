# Agent NF335 (Account A naming queue, 2026-11-24): NF324 Q2, 81 flat rows whose own landed test calls the flattened name

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md (read it in full, standing rulings included: the NF326-NF331 entries at the end are new).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP` in every Bash call; `mkdir -p /run/user/0` if missing.
- Your folder: $S/w22/agentNF335/ . Agent name for lock.sh: NF335. Donor builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (evidence only). Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.

## Ruling you work under (coordinator, owner delegated, 2026-11-24)
NF324 Q2 option 1: a row whose landed TEST declares and calls the flattened free function `Class_Method` (extern) moves together with its test. The test is
rewritten (by script where the shape repeats) to call the PDB member: through a pointer to the class model cast from the test's buffer / object, as a qualified
call `p->Class::Method(...)` (no vftable or virtual dispatch needed), keeping every case and the PASS line. Tests are separate TUs: they may gain lines; they stay
C++03, /Od /W3 without /EHsc, include/ only, self-contained, no UB (no reference bound through a null pointer, no read of uninitialised memory).

## Scope
$S/w22/agentNF335/ENTRIES.txt: 81 addresses (pending/2026-11-24_nf324_handback/TRIAGE.tsv rows whose bucket is "the row's own test calls X"). Each has one nf324 T3 line
(column 2): the coordinator sends you into it and lands with `land_scripted.py --release-t3 nf324` (run dirs) plus the tests (tests/ per batch: the coordinator copies them).
- The row side is NF329 / NF330's route, landed today: switch / header route onto `FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` or existing landed opt-ins (118 + 55 headers are
  in the repo now; reuse them), flatten_pilot v5 + genmodel (tools in naming_batch/agent_tools/nf329/ and nf330/), probes, hdrcheck, equiv_check + planted control,
  per-row objcheck ALL_EQ (or + exactly libcp), graded symbol = PDB symbol, link HEAD v new, new-name census. Read batches/2026-11-24_landing_nf329_b3/README.md and the
  nf330_b* READMEs first, and pending/2026-11-24_nf324_handback/NOTES.md (Q2).
- Re-triage first against the current tree: some may now pass the route only needing the test change; others still need a model (NF329 rules) or are ports
  (`PDB member takes parameters` with stand-in parameter types, no layout, template base, ...) -> STATUS skipped-port with the reason.
- Each changed row's NEW test must pass under VC7.1 against the NEW row source (grade it), and fail to link / fail against nothing else (it no longer names the flat symbol).
- Hand back as land_scripted run-dir batches (rows/, RESULTS.tsv, head/payload.json, HEADERS.tsv, hdr/engine, orig/, PROBES/EQUIV files) plus tests/<addr>_test.cpp
  per batch, about 60 rows at most.

## Stay off
- B's claimed rows (COORDINATION_CLAIMS.tsv newest line per item, B, not done / released) and pending/2026-11-24_acctB_b24/; Codex pass 26 / 27 / 28 RESERVED rows.
- Agent NF334 runs at the same time (T1 `other` tracker entries, lock owner NF334): skip its locks and the addresses in $S/w22/agentNF334/ENTRIES.tsv.
- CScriptThing destructor / copy-ctor / operator= definers; every T3_PORT_QUEUE line that is not your entries' nf324 line.

## Hand-back
As in the standing brief, to $S/w22/agentNF335/. Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS BEFORE your first compile. Keep files current (container restarts happen).
