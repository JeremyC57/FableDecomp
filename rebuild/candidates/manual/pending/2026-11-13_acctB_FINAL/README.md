# acctB lane: final hand-over (2026-11-13)

Start here to land the acctB lane's work. This folder indexes everything the lane produced for the 189 addresses in
`../2026-11-11_acctB/RESERVED.tsv`. Nothing in the lane was landed; no retail bytes or oracle files are committed.

## What is here
- `INDEX.tsv`: one line per reserved address with its state, grade, the path of the BEST version (source and test), the header
  overlay it was verified with, its notes, older superseded copies, and open review items.
- `payload.json`: the combined lander payload of the newest version of all 140 matched rows (source, test, module, name, flags).
  It has no oracle: build the oracle from retail on the landing machine.
- `HEADERS.tsv`: every header the lane made or edited (496 files), whether it is new or an edit of a `rebuild/include` header, how
  many distinct versions exist across rows, and where each copy is (folder:row:hash).
- `lane/`: the lane's history that lived only on the build machine: the agent brief with all lessons (`coord/brief.py`,
  `WF_BRIEF.md`, `CHUNK_BRIEF_lessons.md`), the coordinator's verify / stage scripts, the author workflow scripts, and the grading tools.

## State of the 189 rows
| state | rows | meaning |
|---|---|---|
| MATCHED | 140 | MATCH or RELOCATION_MATCH with a passing VC7.1 behaviour test; each re-verified by the lane coordinator (read-only vland, strict headers, the row's own overlay; logs in each folder's `coordverify/`) |
| DRAFT | 19 | best DIFFER draft, residue on the grade line, notes with every idea tried: `../2026-11-13_acctB_parked/` |
| NO_DRAFT | 23 | not authored; 16 have a dossier and outline, 7 have nothing: `../2026-11-13_acctB_parked/notes/` |
| NOT_AUTHORED | 7 | blocked rows (0x006224C0, 0x00AB2520) and fragments of other functions (0x0074BC30, 0x00D2988B, 0x00BD71CC, 0x00BA115C, 0x00BC3F3D) |

## How to land from this
1. Take the rows from `INDEX.tsv` (`best_source`, `test`), or use `payload.json` directly. Where a row has copies in several
   folders, the index points at the newest one; `older_copies` lists the superseded folders.
2. Headers: each row was verified with its OWN overlay (`headers` column): a full copy of the repo header at authoring time plus
   `#ifdef FABLE_RETAIL_*` additions. The overlays were made at different repo states, so the same header can exist in several
   versions (`HEADERS.tsv`, column `distinct_versions`). Merge the opt-in sections into `rebuild/include` and re-verify each row
   after the merge; do not copy an overlay file over a newer repo header. Overlay READMEs mark copies that are STALE.
3. Name fixes the agents proved are in each folder's `NAME_FIXES.tsv` / `notes/<addr>/NAME_FIXES.tsv`; labels in `LABELS.tsv`.
4. Read `open_items` before landing a row. The ones that need a decision:
   - `volatile` (owner ruling 2026-11-13: fine if it causes no portability issue and retail more than likely did it; standard
     `volatile` on a declaration, cited in a `// Retail-volatile:` line): 0x00B5FAB0 (matched) and the draft 0x00B89C30.
   - Six matched rows of the last workflow still carry verifier findings (comments, an unneeded declaration, a module spelling,
     unneeded force-inline marks): listed in full in `../2026-11-13_acctB_parked/README.md`; all verifier findings verbatim in
     `../2026-11-13_acctB_bwf_e/VERIFIER_FINDINGS.md`.
   - Relocation-name conflicts and other review items of earlier rows are in their folder READMEs (index column `open_items`).
5. Closest unmatched row: 0x00B89C30 is the same size as retail with ONE hunk left (operand order in one inlined
   `SetAlphaRef(1)` block). Six sessions are recorded in `../2026-11-13_acctB_parked/notes/00b89c30/NOTES.md`; three probes that
   match only through an invented inline helper are kept in `matching_probes_not_landable/` there for reference, not for landing.
