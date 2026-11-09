# Chunk 896 -- EH unwind funclets whose real parent has landed (146 rows)

Result: **140 PASS** (payload_land.json / oracle.tsv), **6 PARK** -- all six parent-model defects, all six FIXED in
`parent_fixes/` (parent regraded RELOCATION_MATCH + PASS, funclet then RELOCATION_MATCH + PASS + offcheck2 OK; landable
after the parent fix: `parent_fixes/payload_land_after_parent_fixes.json`). No compile failures, no Leaf-less parent
problems, no not-an-EH-funclet rows.

Pipeline: the Nov-3 mechanism unchanged (`$S/funclets/` ROWS3.tsv + offcheck2.py; `mkpayload.py` copied here), 7 batches by
parent (`todo_00..06`, `batch.sh`, two lanes via `runall.sh`), READ-ONLY vland (no `--land`), `LANDVERIFY_WORK` under `$A`.
ROWS3 was checked against ROWS.tsv first: all 146 rows agree on parent, block offset and length; every parent source named
by build_candidates.ps1 exists (parent sources are resolved from the ps1 at run time).
Gates per row: vland MATCH/RELOCATION_MATCH + behaviour PASS + offcheck2 `OK` (expected offset, exactly one `$L` block of the
retail block length, `__ehhandler$` names the parent). All 140 passes are `leaf-ok`, 37 parents.

## Parked (6) -- parent model defects (details + evidence: parent_fixes/README.md)
| Funclet | Parent | Raw grade | Defect |
|---|---|---|---|
| 0x012134AB | 0x00C287E0 | DIFFER | vector at +0x440, retail +0x43C (allocator word) |
| 0x012134E3 | 0x00C28AE0 | RELOCATION_MATCH (false, any-offset fallback) / offcheck2 NOHIT | 3 unwind actions v retail 1 (stand-in throwing call placed after the vector) |
| 0x01213B0A | 0x00C2D3B0 | DIFFER | member at +0x324, retail unwinds a holder at +0x320 |
| 0x01216DE0 | 0x00CAA790 | DIFFER | CMyBaseObject member at +4, retail CUnknown base at +0 |
| 0x01216E60 | 0x00CAAAC0 | DIFFER | same |
| 0x012173E0 | 0x00CB1890 | DIFFER | node list at +0x14, retail CNodeCache at +0xC |

## Other defect found (not fixed: build_candidates.ps1)
- 0x00C14F90 (PKCDeferredBank ctor) has `CompilerFlags = '/O2 /Oy /W3 / E H s c /EHsc'` in build_candidates.ps1 (spaced-out
  `/EHsc`; cl would take `E`, `H`, `s`, `c` as file names). The local `mkpayload.py` keeps only `/x`-shaped tokens, so its 8
  funclets carry `flags: ["/EHsc"]` and pass. The ps1 entry should read `/O2 /Oy /W3 /EHsc`.

## Files
- `payload_land.json`, `oracle.tsv` -- the 140 passing rows (land as-is).
- `STATUS.tsv` -- one line per row (146).
- `parent_fixes/` -- six fixed parent sources (+3 mirror tests), `parent_fixes.diff`, README, and the 6 post-fix funclet rows.
- `logs/b00..b06/` -- per-batch read-only vland log, offcheck2 output, grades; `logs/parent_fix_*/` and `logs/parent_regrade/`.
