# Name-fix agent NF242 (2026-11-22, round 14): the held CModeSitBench unit with the vtable slot swap, the CEngine unit, the blocked IsThingCarryingRangedWeapon binders

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF241 (`$S/w22/agentNF241/`: NF239's open CODEXP27 lines, the list<CComponent*> family incl. the held
0x0042ABAF / 0x0042ABCA, 0x004046B0, 0x0049D800, 0x01296F14) and you. No authoring agent runs. Off limits:
NF241's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`) is not `released` or `NF242` (write NF242 there first), the 25
Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF240's with overrides
Read `$S/w22/agentNF240/BRIEF.md` and the briefs it chains to (NF238 ... NF215 "Lane", `$S/w22/agentNF242/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF242`, names -> `NF242`. On a conflict: this file > NF240 > earlier. nf240 LANDED (`batches/2026-11-22_landing_nf240/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf240_handback/`, the held q12674 rows and header among it). The three q12674
rows are locked for you. Start from the repo tree now. Never write into /home/user/FableDecomp: vland / equiv_check need it as cwd, so
list the root before and after and leave nothing there; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f,
and never as a foreground sleep loop (run the job in the background and check it). Every grade under `timeout 900`. Disk is tight. Every
edit to an existing header line goes under an opt-in macro, except where an entry says a declaration becomes the default: then prove the
whole include closure with equiv_check and recheck every differing row. GOTCHA: CDefendingCombatantInfo.h hangs cl unless the CThing
prelude comes first.

## Entries (ENTRIES.tsv), in this order
1. q12677 (the held unit + the slot swap, one landing).  2. q12678 (the CEngine unit, in its three steps; hand back after step 1 if the
   proof is large).  3. q12679.  4. q12676 as blockers fall.  5. q12659 only with the full proof.
For every entry: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF240 file set and the decisions you need; hand back early if your context gets heavy.
