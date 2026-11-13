# acctC reservation (2026-11-13, owner): 180 rows for the third account, working on branch `FullDecomp-B`

`RESERVED.tsv` lists the rows. They are off-limits to the coordinator's lane and to any other lane until this list is released
(renamed `RELEASED.tsv`). It is the only reservation list in the repository: the Codex lists and the acctB list are released.

| kind | rows | what they are |
|---|---:|---|
| parked | 120 | a differing draft exists (`closest_version`): the 19 acctB drafts, the 2 acctB rows that matched but were not landed, and the 99 largest other parked rows outside the coordinator's author pool |
| never_worked | 60 | no draft: the 23 acctB rows without a draft (16 have a dossier and outline in `../2026-11-13_acctB_parked/notes/`) and the 37 largest other rows nobody has authored |

`length_estimate` is the distance to the next known function start (an upper bound, not the function length): take the real length
from the retail bytes before building an oracle. The rows were chosen largest first, about 960 KB by that estimate, because what is
left of the project is mostly large functions. Rows classed impossible in plain C++ (CRT or hand-written asm in retail) and the
blocked acctB rows and fragments are not in the list.

## Before the first row
1. Merge `FullDecomp` into `FullDecomp-B`. `FullDecomp` now holds everything the acctB lane matched (landing fy), the merged headers
   and this list; the acctB overlays in the pending folders are STALE copies and must not be used as a starting point.
2. Read `../../batches/2026-11-13_landing_fy/README.md` and `tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/README.md`:
   they describe what it cost to land rows that were verified against old header copies.

## Rules that keep the hand-back landable (from that landing)
- **Stay on today's headers.** Merge `FullDecomp` into `FullDecomp-B` at least once a day and before every hand-back, then re-verify
  every matched row against the merged `rebuild/include`. A row verified only against an older copy is not a match yet.
- **Header changes are additions under new `#ifdef FABLE_RETAIL_*` opt-ins**, made against the CURRENT header. Hand back each change
  as a small diff or as (old text, new text) pairs per header, never as a whole-file copy of a header.
- **Never reuse an opt-in name that exists on `FullDecomp` for different content**, and never edit the body of an opt-in a landed row
  uses. To serve two rows from one existing line, extend its condition (`#if defined(A) || defined(B)`).
- **If a class already has a shared header, use it** (add what the row needs as an opt-in); do not write a second header or a local
  model of the class.
- **Prove header changes over the whole tree** before handing back: `lane_merge/equiv_check.py <include tree before> <include tree
  after> <out.tsv>` preprocesses every landed row against both; every row whose text differs must pass `recheck_rows.py`.
- The standing project rules apply unchanged (CLAUDE.md): byte match plus behaviour test, plain portable C++, PDB names from two
  sources, no retail bytes committed, nothing landed by the lane itself. Matched rows are staged in pending folders for the
  coordinator, with a final index like `../2026-11-13_acctB_FINAL/INDEX.tsv`, and with REAL lengths in it.
