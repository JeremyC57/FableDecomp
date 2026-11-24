# Name-fix agent NF235 (2026-11-22, round 10): the nf234 follow-ups (the three vtable families, in the coordinator's order)

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. Running: authoring agent AUTH24
(Fable 5.2 trial; locks AUTH24: 0x00839420, 0x00651273, 0x00816410, 0x00A68DE0, 0x00502E90). 0x00B4D370 / 0x00BD3070 are reserved for the
c2 readout follow-ups. Do not touch those rows or `$S/w22/auth2*/`, `$S/w22/c2t*/`. Codex pass 27 rows and 0x0042BEC0 are off limits.

## Your brief is NF234's with overrides
Read `$S/w22/agentNF234/BRIEF.md` and the briefs it chains to (NF233 ... NF215 "Lane", `$S/w22/agentNF235/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF235`, names -> `NF235`. On a conflict: this file > NF234 > earlier. nf234 LANDED (`batches/2026-11-22_landing_nf234/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf234_handback/`, NOTES entry 2 = the census). Start from the repo tree now.
Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f. Every grade
under `timeout 900`; a missing #include hangs the lane cl.exe (GOTCHAS). Disk is tight: clean your own scratch (guarded, literal paths).
equiv_check.py was fixed today (`-idirafter rebuild/include`): compare two include-tree copies, or use the fixed tool.

## Entries (ENTRIES.tsv), in this order (coordinator ruling 4, nf234)
1. **CFrontendGameComponent family** (q12655): 0x00495620, 0x00496070, 0x00496CF0 onto CFrontendGameComponent.h with a SetQuit override
   opt-in; 0x00496CF0's local non-virtual Run would split vtable slot 2, so it moves with them. Vtable names from the PDBs / RTTI first.
2. **CMeshBankManager family** (q12654 / q12656): 0x00A0A110 + 0x00A0A150 together (their vtables emit stand-ins for 0x0122DE50 / 0x0129C980).
3. **CEngine ctor 0x00B260B0** (q12657): the last 0x0122DE50 stand-in (q12652 / q12653: AddDeviceResetCallback, the vtable).
4. **CMainGameComponent 0x00418DCA** (q12658) LAST: a typed-layout claim on a header with 469 includers; only with the full equivalence proof.
Not yours: q12659 (NESTED_SET default: after the nine local models move, a later round), q12660 / q12661 (wait on a one-TU rule).
For every entry: recheck every changed row, pk.py every new copy, census spellings (vtable slots included), link-check, TABLE_CHANGES.tsv.
Hand back with the NF234 file set and the decisions you need.
