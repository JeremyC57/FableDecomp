# Agent NF339 (Account A naming queue, 2026-11-24): T3 "self respell" lines

This prefix overrides the standing brief where they differ. Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md
(read it in full, rulings included).

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad ; `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP` in every Bash call
  (a TMPDIR inside the scratchpad makes Wine cl segfault); `mkdir -p /run/user/0` if missing.
- Your folder: $S/w22/agentNF339/ . Agent name for lock.sh: NF339. Donor builds: "/tmp/claude-0/egod_pdb_dl/debug tlc/" (evidence only; FableWin.exe there is
  the debug build, useful for identity). Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget: 3 GB.

## Scope ($S/w22/agentNF339/ENTRIES.tsv: 227 T3_PORT_QUEUE lines whose unit is "self respell")
These are T3 lines (sources alias1 93, access1-4 124, binders1 6, nf334 3, flatten4 1) for rows whose OWN spelling is wrong while the target spelling is known.
The stay-off-T3 rule is lifted for exactly these lines (the coordinator lands them with `land_scripted.py --release-t3 <their source tags>`).
- First a scripted re-read (no compiles): for each line, is the recorded obstacle still there on today's tree (the self already respelled by a later landing; the
  access letter / virtuality now available via a landed header or opt-in; the alias now gone)? Write TRIAGE.tsv (address, tag, reason, still_holds y/n, plan).
- Then work them, cheapest first: the self moves to the PDB spelling (two sources) with every binder of the old self spelling in the same landing (NF304 Q6), objects
  byte-identical or grade unchanged with the graded symbol = PDB symbol; an access / virtuality change that needs a class model goes through the shared header
  (add-only opt-in + probe + hdrcheck2 + equiv_check + planted control; NF330 D1 for base `_RTTI_DECLS`); ref_scan.py before any table `replace`.
- A line that needs a real port (body change, class port, census) stays: STATUS skipped-port with the measured reason.
- Hand back as land_scripted run dirs (batches/bN/: rows/, RESULTS.tsv, head/payload.json, TABLE_CHANGES.tsv, orig/, tests/ + orig/tests/ if any test changes,
  UNITS.tsv where several rows must land together, head/HEAD_FROM_REPO.txt for byte-dump rows per D-f). About 60 changed rows per batch; STATUS.tsv for every line reached.

## Stay off
- Agents NF337 (Def-class `?Copy@` rows and their operator= rows; lock owner NF337) and NF338 (T1 targets re-read as T3; lock owner NF338) run at the same time:
  skip their locks and the addresses in $S/w22/agentNF337/ENTRIES.tsv and $S/w22/agentNF338/ENTRIES.tsv; do not edit a header one of their locked rows includes
  without checking with lock state first (list such a need in NOTES as a decision).
- B's claimed rows (COORDINATION_CLAIMS.tsv newest line per item, B, not done / released) and pending/2026-11-24_acctB_b24/; Codex pass 26 / 27 / 28 RESERVED rows.
- CScriptThing destructor / copy-ctor / operator= emission (NF326 / NF328 / NF331); every T3 line that is not in your ENTRIES.

## Hand-back
As in the standing brief, to $S/w22/agentNF339/. Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS BEFORE your first compile. Keep files current as you go
(container restarts happen).
