# Landing dangling 1 (Account A, 2026-11-24): 7 rows stop referencing an old spelling that an earlier table replace removed

`tools/decomp_pipeline/linux_lane/lane_tools/ref_scan.py` compiled every landed row naming the identifiers of 268 old spellings. Those
spellings were dropped from `reloc_bindings.tsv` by a `replace` since 2026-11-20. Five spellings with no table line left are still
referenced by landed rows: one replace from nf286, one from nf305, and three from binders1. A `replace` is only safe when no landed row still
references the old spelling, and the table keeps one witness per line, so these references were missed.

- **Fix:** `access_fix.py --pairs` edits each referencing row's local-model declaration so the reference becomes the PDB spelling. The
  table already binds that spelling to the same target, so no table change is needed.
- **Proof:** objsec ALL_EQ under the one rename, the row's own graded symbol unchanged, grade PASS; HEAD and new at one path.
- **Rows:** 8 accepted, 7 landed. 0x0079EAA0 is held because it is reserved in T3_PORT_QUEUE.tsv. 0x00B84190 (a constructor reference) is
  outside the tool's scope.
- **Still open:**
  - 0x0079EAA0 and 0x00B84190 still reference spellings with no table line.
  - 39 old spellings whose identifiers appear in more than 80 sources are not yet scanned (a long background job).
  Both are recorded in the journal.

## Recheck
`logs/recheck.log`: 7 MATCH / RELOCATION_MATCH PASS.
