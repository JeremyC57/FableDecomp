# Name-fix agent NF254 (2026-11-22, round 20): the last open CODEXP27 lines (L4 / L5 family, L28, L57, L111, L114, L120; L37 / L129 plans)

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF253 (`$S/w22/agentNF253/`: ??_GCIEngine 0x00B23770, UnsheatheMeleeWeapon + 0x00890C80 + the UnsheatheWeapon
header ports, the GetLocalSeed binder 0x008EE720, the four blocked ctor rows L72 / L74 / L92 / L100, 0x00418DCA) and you. No authoring
agent runs. Off limits: NF253's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`, plain lowercase 8-hex names, no 0x) is
not `released` or `NF254` (write NF254 there first, before ANY compile of that row, controls included), the 25 Codex pass 28 rows
(`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080 (held), `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF252's with overrides
Read `$S/w22/agentNF252/BRIEF.md` and the briefs it chains to (NF250, NF248, NF246, NF245, NF243, NF241 ... NF215 "Lane",
`$S/w22/agentNF254/NF214_BRIEF.md` and its read-first list, in full). Paths -> `$S/w22/agentNF254`, names -> `NF254`. On a conflict: this
file > NF252 > earlier. nf252 LANDED (`batches/2026-11-22_landing_nf252/README.md`, whose rulings you work under, incl. ruling 1 on
anonymous-namespace hashes; hand-back `pending/2026-11-22_nf252_handback/`: NOTES has the L4 / L5 checks and the L114 sketch; nf250 /
nf248 APPLIED.tsv have each line's census). A row's compiler flags come only from `CompilerFlags` in build_candidates.ps1 (GOTCHAS): list
any a row needs. Start from the repo tree now. Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by
PID (kill -0) in a later tool call, never pgrep -f, NEVER any foreground sleep; never launch a grade whose clang precheck failed; never
`rm -rf` a variable path (tools/clean.py). Every grade under `timeout 900`. Disk is tight. Every edit to an existing header line goes under
an opt-in macro; prove header changes with equiv_check and recheck every differing row. No retail byte dumps in hand-back files.

## Entries: `OPEN_LINES.tsv`, in this order
1. L4 / L5 with 0x0074FCE0 as one family: the 3-row unit (pair<int> -> pair<long>, two sources found by nf252) plus 0x0074FCE0's own
   four binders 0x0074FE50 / 0x008A9AE0 / 0x008ED1F0 / 0x008ED440 (three table spellings today), so the leaf move leaves one spelling.
2. L114 (nf252 sketch), L28, L57, L111 (add-only CLipSyncDataBank.h opt-in), L120.
3. L37 (26 referencers) and L129 (about 90): census and a plan only, no partial landings.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, vtcheck every emitted vtable, link-check,
TABLE_CHANGES.tsv. Hand back with the NF252 file set (+ hdr/ and orig/ for any header) and the decisions you need; hand back early if
your context gets heavy.
