# 0086be40 attempts

Original prior, handback, all six neighbours, Ego_r TPI and FableWin raw locals/donor body inspected.

- s01_original_locals: DIFFER, 292v294, 3 strict hunks.
- s02_empty_before_set: DIFFER, 300v294, 2 strict hunks.
- s03_user_empty_destructor: DIFFER, 292v294, 3 strict hunks.
- s04_iterator_pair_test: DIFFER, 292v294, 3 strict hunks.
- s05_iterator_loads_before_set: DIFFER, 296v294, 3 strict hunks.
- s06_branch_local_set: DIFFER, 294v294, 2 strict hunks.
- s07_size_nonzero: DIFFER, 314v294, 6 strict hunks.
- s08_empty_continue_guard: DIFFER, 292v294, 3 strict hunks.
- s09_genuine_texture_helper: DIFFER, 292v294, 3 strict hunks.
- s10_single_iteration_guard: DIFFER, 292v294, 3 strict hunks.
- s11_explicit_empty_constructor: DIFFER, 292v294, 3 strict hunks.

Best294-byte branch-local spelling keeps only the branch/store placement difference. Original donor places the set before the if; that form currently292 bytes because empty() materialises the element address first. Next: actual CArray const operator[] / empty helper IL, or genuine push_back visibility, to preserve folded loads while keeping original lifetime. No budget device is appropriate.

Main extended the behavior fixture to640 cases: multiple candidate meshes, five seeds including32-bit wraparound, existing output entries, all presence masks, ignore-head and texture-group paths. The actual initial fixture had32 cases, not the earlier reported48. Selected production body is unchanged. Both compilers pass the final640-case fixture; one test-only nested-type qualification error was corrected during setup.
