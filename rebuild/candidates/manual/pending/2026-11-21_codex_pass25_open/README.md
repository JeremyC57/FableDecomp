# Codex pass 25 return: the rows that did not land (2026-11-21)

- `00f1eed0/`, `00d01630/`, `00e20600/`: the three rows Codex left in progress. Each has Codex's selected source and test, NOTES.md,
  RETRY_NOTES.md, PROBES.md and grades. `00e20600/alternative_exact_size/` is the equal-length variant. Apply the header patches
  `PATCHES_OPEN.json` (in order: each old_text once, an empty old_text creates the file) on top of the include tree. They are reserved
  for one Account A Opus agent (owner's request), in that order.
- `held/`: three rows Codex matched whose objects carry pick-any copies that differ from retail (HELD.md). They need
  `held/PATCHES_HELD.json`.
- `return/`: Codex's top-level hand-back files (START_HERE_CLAUDE.md, STATUS.tsv, LESSONS.md, BOUNDS_FIX.tsv, LABELS.tsv). Files that
  can carry retail bytes (audit JSON, callee byte dumps) were left out.
