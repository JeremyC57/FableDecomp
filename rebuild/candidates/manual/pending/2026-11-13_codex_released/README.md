# Codex passes 11-13 and 15-21 released to the normal parked pool (2026-11-13, owner)

The owner has handed back the Codex work, so the ten `RESERVED.tsv` lists are renamed `RELEASED.tsv`. Of their 558 rows, 430 have
landed; the 128 still unlanded are ordinary parked rows from now on.

Every source version on file for them (308: the Codex returns and the in-house parked drafts) was regraded against today's headers
with the fast grader (no overlay). `STATUS.tsv` has one line per row; `differ/<addr>.cpp` is a copy of the closest version:

| status | rows | meaning |
|---|---:|---|
| COMPILES_TODAY | 73 | the copy compiles on today's headers and differs from retail; chosen by fewest differing hunks (best: 19 rows at 1-5 hunks, 18 at 6-20, 23 at 21-60, 13 above 60) |
| NEEDS_PORT | 51 | no version compiles today (headers moved on, or the draft relied on a header overlay that was never committed; 45 of the 308 versions also hang the compiler); the copy is the version with the best grade recorded in its own `// Grade:` line, else the newest |
| NO_SOURCE | 4 | decode only so far: 0x007C0690, 0x007BFE30, 0x009FDE80, 0x0058F9BC |

No version matches retail. `source_of_differ_copy` names the file the copy was taken from; `note` names a version that was recorded
closer when written but does not compile today. The 124 rows with a source are in the author pool (residue class `codex-released` or
`needs-port`); the four without one need a first draft.
