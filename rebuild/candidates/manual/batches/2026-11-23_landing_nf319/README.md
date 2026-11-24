# Landing nf319 (Account A, 2026-11-23): 44 alias-class rows onto the real member, 31 add-only header opt-ins

Hand-back: `pending/2026-11-23_nf319_handback/`; helpers under `naming_batch/agent_tools/nf319/`. Entries: 190 alias rows whose member
engine/X.h did not declare under one plain switch. Outcome: 44 done, 134 skipped-port, 12 skipped-held.

- **Landed: 44 rows.**
  - **35 by the header route.** 31 headers each get one add-only opt-in `FABLE_RETAIL_<CLASS>_RTTI_DECLS`, just before the class's closing
    `};`. It declares the RTTI-named member in the PDB's access, const and types, then restores the access label. Each declaration
    overrides a virtual of the base chain at the same slot; the agent's clang probe adds an `override` subclass and checks it, with a
    made-up-name control that fails.
  - **9 by existing switches,** with no header change.
  - In 15 of the 31 headers the existing `FABLE_STATIC_ASSERT` lines are pinned as `FABLE_STATIC_ASSERT_AT(<original line>, ...)` (45
    asserts), so the inserted lines do not rename their typedefs. These are the only edits to existing header lines.
  - 1 test (0x004E0F34) now calls the real member.
- **Proof:**
  - **Per row:** precheck; vland HEAD and new; objsec with exactly one DEF rename, equal UND / DEF sets, no `??_7` / `??_R`, and every
    non-debug section equal; `.drectve` equal or the libcp exception (16 rows, NF318 Q1); the graded symbol is the PDB symbol; a /FORCE
    link with 0 duplicates.
  - **equiv_check:** every landed row and test, old against new tree: 0 DIFF over 57,146. The planted control flags every header's
    includers (193 rows).
  - **At landing:** no stale orig/ or header copy. The recheck covers the 44 rows plus every direct includer of the 31 headers (185
    rows), all MATCH / RELOCATION_MATCH PASS. Every row's object defines its PDB symbol.
- **Rulings:**
  - **Q1:** triaging all entries by script and then working the feasible ones is the stop rule's intent.
  - **Q2:** the 16 libcp rows fall under NF318 Q1.
  - **Q3 (held):** 0x00452B14's class is written on one line; editing that line is a class-model change.
- **Queued:** T3 `nf319` lines for 134 ports and 12 holds. Of the ports, 89 have a flat header model with no virtuals, so a declaration
  cannot sit at the RTTI slot (a class-model port).
- **Agent slips** (none reached the repo): one refused `rm -f` in its folder, one refused write to /tmp, a `sleep 0`, and an early regrade
  order that the final run redid properly.

## Recheck
`logs/recheck.log` (landed tree, 185 rows = 44 rows + header includers): 185 PASS.
