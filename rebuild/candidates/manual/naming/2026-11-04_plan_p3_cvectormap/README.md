# Plan phase 3, wave w1 (2026-11-04): CVectorMap onto the shared header, continued (1,291 rows)

## Result
- **1,291 more rows** now use `engine/CVectorMap.h` instead of a local copy. Phase 2 moved 473.
  - Among them: phase 2's 671 READY rows, 159 of the 160 `bitset_stub` rows, 110 of the 120 `const_lowerbound` rows, and 20 stand-in
    maps respelled to the decided LowerBound spellings (`CALLERS.tsv`).
- **New sub-macro `FABLE_RETAIL_STLP_BITSET_TEST_DECL`** in `stlport/fable_containers.h`. It declares
  `std::bitset<N>::test(size_t) const` with no inline body, because retail calls the out-of-line copy at 0x00410DE0 (63 bytes; 337
  landed callers bind `?test@?$bitset@$0BBC@@std@@QBE_NI@Z`). The inline body stays in the `#else` branch, so hdrcheck2 reads
  `default=same`. 184 rows define the macro.
- **Coordinator re-grade from the working tree:**
  - gate.py: 1,291 / 1,291 PASS.
  - vland `--recheck-landed`: 1,291 / 1,291, 7 MATCH + 1,284 RELOCATION_MATCH, all PASS (`logs/`).
  - The plan agent also ran the 4,065 other includers of `fable_containers.h` through gate.py with the overlay: all PASS.
- **Queue:** 15 lines on 0x0040F020 / 0x004365B0 narrowed (`QUEUE_LINES.tsv`).
- **Deferred:** 1,155 (`DEFERRED.tsv`). Among them:
  - 53 rows use `bitset::operator[]`, which the header does not model;
  - 10 const rows hold a non-const iterator;
  - the rest are compile_fail, local_std and byte changes.
