# egod lane hand-back e02

Rows staged from author batch `e02` (reserved list `../2026-11-17_egod_parked/`). Each was re-verified by the coordinator (`coordverify.log`: read-only vland, strict headers, minimal overlay = `hdrdiff/`) and rule-audited by an independent agent. `INDEX.tsv` lists the rows with true lengths and the audit notes; `NOTES.md` has what closed each row; `HDR_README.md` lists the header opt-ins (additions only). Nothing here is landed.

## Re-verified after merging FullDecomp (2026-11-18)
The main lane landed its own `FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSTATICMESH` opt-in (NF77) under the name row 0x00B9E120 used.
The egod hunk for that header is dropped: the row now compiles against the landed opt-in and still grades RELOCATION_MATCH with its
test passing (`coordverify.log`). All rows here were re-verified against the merged `rebuild/include`; `hdrdiff/` is rebased.
