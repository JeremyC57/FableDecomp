# Name-fix agent NF253 (2026-11-22, round 19b): ??_GCIEngine, UnsheatheMeleeWeapon + 0x00890C80, the GetLocalSeed binder, four blocked ctor rows

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF252 (`$S/w22/agentNF252/`: the E3 0x01296F14 unit and the remaining CODEXP27 port lines) and you. No authoring
agent runs. Off limits: NF252's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex names, no 0x)
is not `released` or `NF253` (write NF253 there first, before ANY compile of that row, controls included), the 25 Codex pass 28 rows
(`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF251's with overrides
Read `$S/w22/agentNF251/BRIEF.md` and the briefs it chains to (NF249, NF247, NF244, NF242, NF238 ... NF215 "Lane",
`$S/w22/agentNF253/NF214_BRIEF.md` and its read-first list, IN FULL: GOTCHAS.md in full, not by grep). Paths -> `$S/w22/agentNF253`, names ->
`NF253`. On a conflict: this file > NF251 > earlier. nf251 and nf250 LANDED (`batches/2026-11-22_landing_nf251/README.md`,
`.../2026-11-22_landing_nf250/README.md`, whose rulings you work under; hand-backs `pending/2026-11-22_nf251_handback/`,
`pending/2026-11-22_nf250_handback/`). A row's compiler flags come only from `CompilerFlags` in build_candidates.ps1 (GOTCHAS); a row may
fold under another row's TU by `#include "../src/compiled/..."` (GOTCHAS; 0x00B2AB10). Start from the repo tree now. Never write into
/home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a later tool call, never pgrep -f, NEVER any
foreground sleep; never launch a grade whose clang precheck failed (controls included); never `rm -rf` a variable path (tools/clean.py).
Every grade under `timeout 900`. Disk is tight. Every edit to an existing header line goes under an opt-in macro; prove header changes with
equiv_check and recheck every differing row. No retail byte dumps in hand-back files.

## Entries (ENTRIES.tsv), in this order
1. q12710.  2. q12711.  3. q12712.  4. q12713.  5. q12659 only with the full proof.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Hand back with the NF251 file set and the decisions you need; hand back early if your context gets heavy.
