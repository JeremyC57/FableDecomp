# acctC open rows: latest drafts and notes (2026-11-16)


The 19 reserved rows that did NOT byte-match when the round-six workflows stopped. Nothing here is a match and nothing here lands.
Each batch folder holds the best differing draft per row (`<addr>.cpp`), the batch `NOTES.md` / `LESSONS.md` (grep the address;
newest sections first), the row line(s) of the batch `STATUS.tsv`, and `hdrdiff/` = the header-overlay diffs the drafts use
(`NEW_<name>.h` = a header the repo does not have yet). This supersedes the older `*open` / `*/open` folders for these rows.

| row | batch | last grade (ours v retail bytes) |
|---|---|---|
| 0x00981410 | wfA3 | DIFFER — BLOCKED |
| 0x00BAA870 | wfA4 | DIFFER(4600v4600) |
| 0x00ADE080 | wfA6 | DIFFER(2735v2735) |
| 0x00AE3C30 | wfA6 | DIFFER(2310v2310) |
| 0x005F59C7 | wfB1 | DIFFER(2649v2652) — BLOCKED |
| 0x009FDE80 | wfB5 | DIFFER(1265v1265) |
| 0x00502E90 | wfE10 | DIFFER(13587v13622) |
| 0x0078FD60 | wfE10 | DIFFER(9291v9268) |
| 0x00795300 | wfE10 | DIFFER(9291v9268) |
| 0x00C4F600 | wfE12 | DIFFER(9204v9204) |
| 0x00F1EED0 | wfE3 | DIFFER(9262v9262) |
| 0x00E20600 | wfE4 | DIFFER(14526v14510) |
| 0x00B4D370 | wfE6 | DIFFER(7162v7176) |
| 0x00BD3070 | wfE7 | DIFFER(11288v11288) |
| 0x00A2F740 | wfE8 | DIFFER(3017v3051) |
| 0x005AC10E | wfE9 | DIFFER(3532v3532) |
| 0x00648820 | wfE9 | DIFFER(3464v3464) |
| 0x00879900 | wfE9 | DIFFER(3370v3415) |
| 0x00931390 | wfE9 | DIFFER(3863v3863) |

**Released 2026-11-16 (landing gn).** These rows are back in the normal pool. When reusing a `hdrdiff/`, merge it into today's
`rebuild/include`. One opt-in was renamed at landing: wfA4's inline probe `FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SOURCE_DEST`
is `FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SOURCE_DEST_INLINE` in the repo, because the landed 0x00B01900 / 0x00B03CA0 use the old
name as a declaration-only opt-in. The best 0x00BAA870 draft does not use it.

Useful findings for whoever continues (details in the batch NOTES):
- 0x009FDE80, 0x00795300 / 0x0078FD60 (twins): instructions equal retail; the residue is stack-slot order / frame size, traced into the
  VC7.1 code generator frame packer (models and probes described in wfB5 and wfE10 NOTES).
- 0x00ADE080: residue is x87 term order in five transforms, traced to value-number ids of matrix-entry addresses (wfA6 NOTES).
- The Anniversary 360 PowerPC line table was used as third-source evidence for statement order (wfB5 NOTES); owner has not yet ruled on it.
- 0x006D0830 closed after eleven failed passes by defining a same-TU template COMDAT callee in the row: check that first on stuck rows.
