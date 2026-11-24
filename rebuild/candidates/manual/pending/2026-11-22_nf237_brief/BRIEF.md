# Name-fix agent NF237 (2026-11-22, round 11b): the Codex pass 27 and AUTH24 NAME_FIXES (a second naming agent beside NF236)

Owner (2026-11-22): "stop authoring and do another naming queue agent". Two naming agents now run: NF236 (`$S/w22/agentNF236/`, entries
q12663-q12667 + q12659: the 0x013B7D58 binders, 0x004065D0, 0x00690030, five header tests, the 54 held p27 bindings, 0x00418DCA) and you.
No authoring agent runs (AUTH25 stopped). Off limits: every NF236 entry and row, every row whose lock owner is not `released` or `NF237`
(`$S/w22/nflocks/<addr>/owner`: check before you touch a row and claim it there first), the 25 Codex pass 28 rows
(`rebuild/candidates/manual/pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF235's with overrides
Read `$S/w22/agentNF235/BRIEF.md` and the briefs it chains to (NF234 ... NF215 "Lane", `$S/w22/agentNF237/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF237`, names -> `NF237`. On a conflict: this file > NF235 > earlier. Rulings to work under:
`batches/2026-11-22_landing_p27/README.md`, `batches/2026-11-22_landing_nf235/README.md`, `batches/2026-11-22_landing_auth24_createeffectonhit/README.md`.
Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f. Every grade
under `timeout 900`; a missing #include hangs the lane cl.exe (GOTCHAS). Disk is tight (about 3 GB free): clean your own scratch (guarded,
literal paths). Every edit to an existing header line goes under an opt-in macro; prove header changes with equiv_check (two include-tree
copies) and recheck every differing row.

## Entries: `QUEUE_LINES.tsv` (137 lines)
The NAME_FIXES_QUEUE lines tagged land1122p27 (CODEXP27: Codex's pass-27 NAME_FIXES, minus the addresses of the 54 held bindings, which are
NF236's) and land1122auth24 (0x008EE790 / 0x008EE720 self rows onto the const PDB forms, the 0x0122DEE0 split, the 0x0129BA3C label).
Each line is a claim to verify, not an order: two sources per spelling; respell the landed self row and its binders onto the proven
spelling, count-neutral or count-reducing (census before / after). Group by class family so one header port serves many lines; take
the families with the most landed binders first. Lines you cannot prove go in REJECTED.tsv with the reason. Do as many as fit; list the rest.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF235 file set and the decisions you need.
