# NF372 lessons

- Screen a slice against T3_PORT_QUEUE before planning: 129 of 169 OPEN.tsv T1 targets were already T3 lines (the tracker does not read the
  queue). Read the T3 line's reason: two kinds of work were waiting behind it (a parked, measured unit in an earlier hand-back; a stale table
  premise).
- NF309's `reverted/unit5/` texts still applied byte for byte two days later; check orig/ against the landed source and test first, then
  re-measure at HEAD instead of re-deriving.
- repo recheck.py does not create its output directory; `mkdir -p` it first, or the payload write fails and vland runs on nothing.
- pk_all's `--leaf=` needs the row's Leaf identifier (nf.leaf_of); an `awk '{print $3}'` of a `// Leaf:` line can come back empty.
- land_scripted applies only TABLE_CHANGES lines witnessed by a landed row: a `rewitness` onto another (reserved) row and table-only fixes
  must be handed to the coordinator for apply_table.py by hand.
- A whole family of "PDB spellings" in the libpk / CODA block (one ctor name on ten addresses) is a propagated label, not a binder fix:
  check LIBRARY_MAP before respelling callers onto a tracker PDB spelling.
