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
- **A header edit must not move a `FABLE_STATIC_ASSERT`** without pinning it: the macro names its typedef after `__LINE__`, and VC7.1's
  stack-slot order in a few rows depends on those names (landing fz). After editing a header run `lane_merge/pin_asserts.py` (it
  respells moved asserts `FABLE_STATIC_ASSERT_AT(<old line>, ...)`), then the exact `equiv_check.py` and `recheck_rows.py`; the
  steps are in `lane_merge/README.md`.
- **If a class already has a shared header, use it** (add what the row needs as an opt-in); do not write a second header or a local
  model of the class.
- **Prove header changes over the whole tree** before handing back: `lane_merge/equiv_check.py <include tree before> <include tree
  after> <out.tsv>` preprocesses every landed row against both; every row whose text differs must pass `recheck_rows.py`.
- The standing project rules apply unchanged (CLAUDE.md): byte match plus behaviour test, plain portable C++, PDB names from two
  sources, no retail bytes committed, nothing landed by the lane itself. Matched rows are staged in pending folders for the
  coordinator, with a final index like `../2026-11-13_acctB_FINAL/INDEX.tsv`, and with REAL lengths in it.

## Lane progress (acctC coordinator)
`STATUS.tsv` is the lane's ledger, one line per reserved row (OPEN / QUEUED / IN_PROGRESS / MATCHED_STAGED, true length, the pending
folder a matched row is staged in). Owner goal (2026-11-13): every reserved row matched; author agents (Opus, expert brief, large
budgets) are relaunched as they end. `lane/mkagent.py <agent> <addr>...` builds an agent's work folder and brief from
`lane/BRIEF_TEMPLATE.md` (scratch paths are filled in at run time; oracles stay in the scratch).

Re-cut rows (two reserved rows that were not function starts) and what the lander must do about them: `RECUTS.md`.

## Acceptance test for devices and fitted spellings (owner, 2026-11-14)
Before accepting a device (inline flag, scoped inline pragma, unused named locals that re-key a c2 record-count tie, a redundant
guard, a fitted named local, dead release asserts) the question is: does the row BYTE MATCH retail, does it BEHAVE THE SAME as
retail, and is it PORTABLE code. Three yeses: accept it, marked (`// Inline-budget:`, `// Fidelity:`, `// Inferred:`, or FITTED
in the header comment). Earlier in this lane the coordinator applied a stricter rule (no inline flags, no unused locals);
READMEs written before 2026-11-14 that call such a construct "not landable" should be read with this test instead. Asm has
extra rules (docs/pipeline/ASM_RULES.md: only where retail used asm; an intrinsic, else a shared-header macro kernel with a
`FABLE_PORTABLE` twin and a dual-path test; never in a row source) and the same three questions apply to it. Stand-in callee
bodies, diagnostic builds and local class models where a shared header exists fail the test.
