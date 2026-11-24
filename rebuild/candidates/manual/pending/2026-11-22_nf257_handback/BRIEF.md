# Name-fix agent NF257 (2026-11-22, round 21b): CCreatureAction L72 SummonCreatures and L100 WillHealInto, 0x0096B600, 0x00418DCA

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF256 (`$S/w22/agentNF256/`: L28, L57, the L37 family, the CEngineSkyRenderer builders 0x00B61CD0 / 0x00B61FF0,
the L129 plan) and you. No authoring agent runs. Off limits: NF256's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`,
plain lowercase 8-hex names, no 0x) is not `released` or `NF257` (write NF257 there first, before ANY compile of that row, controls
included), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080
(held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF255's with overrides
Read `$S/w22/agentNF255/BRIEF.md` and the briefs it chains to (NF253, NF251, NF249, NF247, NF244, NF242, NF238 ... NF215 "Lane",
`$S/w22/agentNF257/NF214_BRIEF.md` and its read-first list, in full). Paths -> `$S/w22/agentNF257`, names -> `NF257`. On a conflict: this
file > NF255 > earlier. nf255 LANDED (`batches/2026-11-22_landing_nf255/README.md`, whose rulings you work under; hand-back
`pending/2026-11-22_nf255_handback/`: NOTES / LESSONS / evidence give the per-class recipe you repeat: vtable slot count and owners read from
Fable.exe first, one MELEE-branch prelude for all of a class's rows, the placement-copy `??_G` helper, every slot symbol checked against
its retail owner). Drafts: `pending/2026-11-22_nf250_handback/blocked/`. Start from the repo tree now. Never write into
/home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a later tool call, never pgrep -f, NEVER any
foreground sleep; never compile a row before its lock is yours and its clang precheck passed; never `rm -rf` a variable path
(tools/clean.py). Every grade under `timeout 900` with the row's CompilerFlags. Disk is tight (under 3 GB). Every edit to an existing
header line goes under an opt-in macro; prove header changes with equiv_check and recheck every differing row (L100's opt-in needs the
check over 0x00955620, which includes CCombatAction_WillHealInto.h). No retail byte dumps in hand-back files.

## Entries (ENTRIES.tsv), in this order, one class per pass
1. q12722 (L72).  2. q12723 (L100).  3. q12724.  4. q12659 only with the full proof.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Hand back with the NF255 file set and the decisions you need; hand back early (after any finished class) if your
context gets heavy.
