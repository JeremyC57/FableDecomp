# Name-fix agent NF236 (2026-11-22, round 11): nf235 follow-ups, the IsEventButton second spelling, the p27 held bindings

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. Running: authoring agent AUTH24
(Fable 5.2 trial; locks AUTH24: 0x00839420, 0x00651273, 0x00816410, 0x00A68DE0, 0x00502E90). 0x00B4D370 / 0x00BD3070 are reserved for the
c2 readout follow-ups. Do not touch those rows or `$S/w22/auth2*/`, `$S/w22/c2t*/`. 0x00B260B0 / 0x00B261D0 (CEngine unit) are not yours.

## Your brief is NF235's with overrides
Read `$S/w22/agentNF235/BRIEF.md` and the briefs it chains to (NF234 ... NF215 "Lane", `$S/w22/agentNF236/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF236`, names -> `NF236`. On a conflict: this file > NF235 > earlier. nf235 LANDED (`batches/2026-11-22_landing_nf235/README.md`,
whose rulings you work under). Codex pass 27 LANDED too (`batches/2026-11-22_landing_p27/README.md`): 100 new rows, 354 header patches, and
the IsEventButtonHeld (0x00687B30) / IsEventButtonReleased (0x00687BD0) swap across 70 files. Start from the repo tree now.
Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f. Every grade
under `timeout 900`; a missing #include hangs the lane cl.exe (GOTCHAS). Disk is tight (about 3 GB free): clean your own scratch (guarded,
literal paths). Pass 27 lesson: every edit to an existing header line goes under an opt-in macro; prove it with equiv_check (two include-tree
copies) and recheck every differing row.

## Entries (ENTRIES.tsv), in this order
1. q12663: 0x013B7D58 binders onto GOldGameComponentToDelete.
2. q12664: 0x004065D0 GetDefaultProfileName (self rename: catalog + files + leaf; two sources; find GetAutoSaveFileName's real address).
3. q12665: 0x00690030's QAE IsEventButtonReleased onto the IBE spelling.
4. q12666: the five owed header tests.
5. q12667: the 54 held p27 bindings, as many as fit, each count-neutral or count-reducing, two sources per spelling.
6. q12659: 0x00418DCA only with the full proof, else leave it.
For every entry: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF235 file set and the decisions you need.
