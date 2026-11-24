# Name-fix agent NF244 (2026-11-22, round 15): the CEngine unit (bisect first), CModeBase follow-ups, the IsThingCarryingRangedWeapon binders

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF245 (`$S/w22/agentNF245/`: the 0x004046B0 list family, the wrong DTOR_OUTOFLINE families, NF241's open
CODEXP27 lines, 0x0049D800, 0x01296F14) and you. No authoring agent runs. Off limits:
NF245's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`) is not `released` or `NF244` (write NF244 there first), the 25
Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF242's with overrides
Read `$S/w22/agentNF242/BRIEF.md` and the briefs it chains to (NF238 ... NF215 "Lane", `$S/w22/agentNF244/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF244`, names -> `NF244`. On a conflict: this file > NF242 > earlier. nf242 LANDED (`batches/2026-11-22_landing_nf242/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf242_handback/`; the CModeSitBench unit LANDED; its q12678_step1/ drafts are yours). The CModeSitBench
rows are locked for you. Start from the repo tree now. Never write into /home/user/FableDecomp: vland / equiv_check need it as cwd, so
list the root before and after and leave nothing there; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f,
and never as a foreground sleep loop (run the job in the background and check it). Every grade under `timeout 900`. Disk is tight. Every
edit to an existing header line goes under an opt-in macro, except where an entry says a declaration becomes the default: then prove the
whole include closure with equiv_check and recheck every differing row. GOTCHA: CDefendingCombatantInfo.h hangs cl unless the CThing
prelude comes first.

## Entries (ENTRIES.tsv), in this order
1. q12680 (the CEngine unit; the bisect first; hand back after step 1 if the proof is large).  2. q12681.  3. q12682.  4. q12679.
5. q12676 as blockers fall.  6. q12659 only with the full proof.
For every entry: recheck every changed row, pk.py every new copy, census spellings (NF242's `tools/vtmap.py` maps vtable slots onto
retail by RTTI), link-check, TABLE_CHANGES.tsv. Hand back with the NF242 file set and the decisions you need; hand back early if your
context gets heavy.
