# Name-fix agent NF250 (2026-11-22, round 18): NF241's remaining CODEXP27 lines, 0x0049D800, the 0x01296F14 unit

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; (2026-11-22) "stop authoring and do another naming queue agent".
Two naming agents run: NF249 (`$S/w22/agentNF249/`: UnsheatheWeapon to protected, 0x00890D80, the CEngine ctor / dtor step 3,
0x00418DCA) and you. No authoring agent runs. Off limits: NF249's entries, every row whose lock owner (`$S/w22/nflocks/<addr>/owner`,
plain lowercase 8-hex directory names, no 0x) is not `released` or `NF250` (write NF250 there first), the 25 Codex pass 28 rows
(`pending/2026-11-22_codex_pass28/RESERVED.tsv`), 0x00B4D370 / 0x00BD3070, 0x00890250, `$S/w22/auth2*/`, `$S/w22/c2t*/`.

## Your brief is NF248's with overrides
Read `$S/w22/agentNF248/BRIEF.md` and the briefs it chains to (NF246, NF245, NF243, NF241 ... NF215 "Lane", `$S/w22/agentNF250/NF214_BRIEF.md`
and its read-first list). Paths -> `$S/w22/agentNF250`, names -> `NF250`. On a conflict: this file > NF248 > earlier. nf248 LANDED
(`batches/2026-11-22_landing_nf248/README.md`, whose rulings you work under; hand-back `pending/2026-11-22_nf248_handback/`: its NOTES /
LESSONS and APPLIED.tsv have the census and blocker for each open line). Start from the repo tree now. Never write into
/home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0) in a later tool call, never pgrep -f, NEVER any
foreground sleep; never launch a grade whose clang precheck failed (NF248 slipped: cl spun); never `rm -rf` a variable path (tools/clean.py).
Every grade under `timeout 900`. Disk is tight. Every edit to an existing header line goes under an opt-in macro; prove header changes with
equiv_check and recheck every differing row. No retail byte dumps in hand-back files.

## Entries: `OPEN_LINES.tsv` (31 lines = NF248's APPLIED rows not done), cheapest first
1. The ten one-row self ports (L71, L72 (its second parameter type differs from the claim: a self-signature change needs two sources),
   L74, L83, L86, L89, L92, L100, L104, L115).
2. E2 0x0049D800 (0x008EE720 still spells the CEngineRandomOwner stand-in; four binders already spell GetLocalSeed@CWorld).
3. The port units: L4 / L5 with 0x0074FCE0 (pair<int> -> pair<long> self signature needs two sources), L28, L57, L111 (add-only
   CLipSyncDataBank.h opt-in), L114, L120; L118 (vtable emitters first), L119 (catalog-only + one binder).
4. The `??_G` stand-ins L29, L32, L50, L95, L96.
5. E3 0x01296F14 with L101-L103 (7 rows; Ego_r puts the class in `?A0x4c815337` but three binders spell the global-namespace vtable:
   the `?A0x` hash is stripped when counting spellings, per the standing ruling, but the namespace itself must match retail).
6. The large families L37 (26 referencers) and L129 (about 90) only if context remains: census and a plan, no partial landings.
Each line is a claim to verify (two sources), count-neutral or count-reducing; REJECTED.tsv with the reason for any you cannot prove.
For every change: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv.
Hand back with the NF248 file set (+ hdr/ and orig/ for any header) and the decisions you need; hand back early if your context gets heavy.
