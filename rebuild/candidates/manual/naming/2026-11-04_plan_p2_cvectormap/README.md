# Plan phase 2, wave 2 (2026-11-04): CVectorMap onto the shared header, 473 rows

## Result
- **473 rows moved onto `engine/CVectorMap.h`.** The header is unchanged and no sub-macro was needed.
  - 317 local specialisation copies were replaced by the header, with no symbol change. About 160 of them had a pad after the
    by-value map, and that pad shrinks by 8 bytes: the local model was 8 bytes, the retail map is 0x10.
  - 156 stand-in maps (CInterfaceMap 155, CInterfaceMap_Retail 1) now bind the two retail LowerBound copies, 0x0040F020 (non-const)
    and 0x004365B0 (const), under their Ego_r spellings. `CALLERS.tsv` lists the rows and the 174 relocation respells.
- **Coordinator re-grade from the working tree:** gate.py 473 / 473 PASS. vland `--recheck-landed` 473 / 473, all
  RELOCATION_MATCH + PASS (`logs/`).
- **Queue:** 12 lines are narrowed, none closed (`QUEUE_LINES.tsv`). One NAME_FIXES line is queued as `plan_p2`: rows 0x0072C9C0,
  0x0074B770 and 0x00753380 bind the non-const LowerBound spelling to the const copy 0x004365B0.
- **Deferred:** 482 rows (`DEFERRED.tsv`).

  | Reason | Rows | Next step |
  |---|---|---|
  | bitset_stub | 160 | phase 3, with a new FABLE_RETAIL_STLP_BITSET_TEST_DECL. Retail calls `std::bitset<274>::test` out of line at 0x00410DE0 (63 bytes), and 337 landed callers already bind `?test@?$bitset@$0BBC@@std@@QBE_NI@Z`, so the declaration-only model is retail's (coordinator decision) |
  | const_lowerbound | 120 | |
  | alias shapes | 148 | |
  | compile_fail | 23 | |
  | stray_change | 11 | |
  | derived_pad | 10 | |
  | local_std | 7 | |
  | test_link | 3 | |

- **Strict mode:** `strict_classes.txt` lists the 147 header classes that no landed row defines locally. `strict_note.txt` proposes a
  per-class `LANDVERIFY_STRICT_CLASSES` list, and notes that the SHARED_HEADER regex misses base-clause and `template <>` local
  copies. None of the five value classes is fully migrated yet.
- **`NEXT.md`:** the phase 3 inputs. They are the rest of CVectorMap, C3DVector's top variant (1,371 rows, 45 of 45 in a trial),
  CArray's empty-vector variants (666 rows), steps 4-5, and the vendor headers.
