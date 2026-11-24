# Name-fix agent NF252 (2026-11-22, round 19): the E3 0x01296F14 unit, the remaining CODEXP27 port lines

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF251 (`$S/w22/agentNF251/`: the held CEngine pair and 0x00B2AB10, the CIEngine stand-ins, the 0.0 constant,
0x00890C80, the UnsheatheWeapon header ports, 0x00418DCA) and you. No authoring agent runs. Off limits: NF251's entries, every row whose
lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex names, no 0x) is not `released` or `NF252` (write NF252 there first,
before grading), the 25 Codex pass 28 rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250,
0x0091C080 (held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF250's with overrides
Read `$S/w22/agentNF250/BRIEF.md` and the briefs it chains to (NF248, NF246, NF245, NF243, NF241 ... NF215 "Lane",
`$S/w22/agentNF252/NF214_BRIEF.md` and its read-first list). Paths -> `$S/w22/agentNF252`, names -> `NF252`. On a conflict: this file >
NF250 > earlier. nf250 LANDED (`batches/2026-11-22_landing_nf250/README.md`, whose rulings you work under; hand-back
`pending/2026-11-22_nf250_handback/`: its NOTES has the E3 plan and APPLIED.tsv each line's census). Start from the repo tree now. Never
write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a later tool call, never pgrep -f,
NEVER any foreground sleep; never launch a grade whose clang precheck failed; never `rm -rf` a variable path (tools/clean.py). Every
grade under `timeout 900`. Disk is tight. Every edit to an existing header line goes under an opt-in macro; prove header changes with
equiv_check and recheck every differing row. No retail byte dumps in hand-back files.

## Entries: `OPEN_LINES.tsv`, in this order
1. **E3 0x01296F14 with L101-L103** (the 7-row unit in nf250 NOTES: an add-only opt-in that puts CCreatureAction_ScreamerIntoAggressive
   in the anonymous namespace Ego_r gives it (`?A0x4c815337`; the hash is stripped when counting spellings, the namespace must match),
   plus the OnReplace / Update ports; land the vtable and every binder as one unit). Hand back after it if it is large.
2. The port units: L4 / L5 with 0x0074FCE0 (pair<int> -> pair<long> self signature needs two sources), L28, L57, L111 (add-only
   CLipSyncDataBank.h opt-in), L114, L120.
3. L37 (26 referencers) and L129 (about 90) only if context remains: census and a plan, no partial landings.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Hand back with the NF250 file set (+ hdr/ and orig/ for any header) and the decisions you need; hand back early if
your context gets heavy.
