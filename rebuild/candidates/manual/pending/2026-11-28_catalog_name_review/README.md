# Catalog name vs leaf: saved for later (2026-11-28)

The pass 30 audit's "about 4,110" compared only the last name segment with a crude split. Redone properly over 57,393 rows:
38,685 rows have a full mangled Leaf, 5,208 more have a prefix Leaf (not checkable in full), 12,818 have no Leaf line (they grade
the catalog name). Of the full-leaf rows, 6,721 differed under light normalisation; 850 of those are cosmetic only; 77 (+1 outside
that list) were relabelled (`batches/2026-11-28_catalog_relabel`). The rest, 5,794, are here in `BACKLOG.tsv`, by tracker bucket:

- standin T3 3,336, split_unknown T3 724, split_fixable T3 433, rtti_identified T3 338, rtti_conflict T3 73: the leaf is the weak
  side; fixing them is a code respell with regrades and usually a header port, under the T3 hold.
- agree_unverified 297, agree_descriptive 33, unmeasured 7, split_pdb_multi 2: no proof either way.
- agree_pdb 264, agree_std 258, split_fixable T1 17: leaf proven, but the relabel was held (T3 line, lock, claim, reservation),
  or is an operator, or would lose scope.
- `NAME_CONFLICTS.tsv` (12): catalog and leaf are different real names; decide per row.
