# Name-fix agent NF240 (2026-11-22, round 13): two unprobed held lines, the CModeSitBench ctor, the CEngine unit, CMainGameComponent

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF241 (`$S/w22/agentNF241/`: NF239's open CODEXP27 lines, the list<CComponent*> family incl. the held
0x0042ABAF / 0x0042ABCA, 0x004046B0, 0x0049D800, 0x01296F14) and you. No authoring agent runs. Off limits:
NF239's lines, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`) is not `released` or `NF240` (write NF240 there first), the 25
Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF238's with overrides
Read `$S/w22/agentNF238/BRIEF.md` and the briefs it chains to (NF236 ... NF215 "Lane", `$S/w22/agentNF240/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF240`, names -> `NF240`. On a conflict: this file > NF238 > earlier. nf238 LANDED (`batches/2026-11-22_landing_nf238/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf238_handback/`). Start from the repo tree now. Never write into
/home/user/FableDecomp: vland / equiv_check need it as cwd, so list the root before and after (NF238 did) and leave nothing there; never
run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f. Every grade under `timeout 900`. Disk is tight (about 3 GB
free). Every edit to an existing header line goes under an opt-in macro; prove header changes with equiv_check and recheck every
differing row. GOTCHA from NF238: CDefendingCombatantInfo.h hangs cl unless the CThing prelude comes first.

## Entries (ENTRIES.tsv), in this order
1. q12673, together with NF237's L38 / L39 (rows that tail-call 0x006A15A0 / 0x006A1620; landing nf239 ruling 4
   gives them to you: `pending/2026-11-22_nf239_handback/APPLIED.tsv`).  2. q12674.  3. q12675 (the CEngine unit: the four placeholder names first, then the override opt-in, then both rows).
4. q12676 as blockers fall.  5. q12659 only with the full proof.
For every entry: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF238 file set and the decisions you need; hand back early if your context gets heavy.
