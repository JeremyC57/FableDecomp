# NF185 lessons (one line each)
- Probe the whole slice first (probe.py, ~0.4 s a row here) and key resumability on (row, target), not row: a row probed for one entry was silently skipped for the next (25 rows of 0x0099EFB0).
- Several queue lines carry the claim in the evidence column (q6206-8, q6492, q6507, q11773): build a hand-checked claim table before comparing bindings.
- The pick-any rule bites only across optimisation regimes: an /O2 or frame-pointer binder TU emits a non-identical copy of an /Os owner's inline wrapper (find 21/71 v 20 bytes, map::find 22 v 20); same-regime copies (_M_find 84 bytes, operator!= 22) are byte-identical.
- Declaration-only is the only form that cannot emit an owner copy; try it first, but it can lose the caller's register facts (0x006D0630 DIFFER 479v459).
- VC7.1 here emits every used inline template member of the shared STLport headers as a COMDAT, so owners.py must check every new symbol against landed Leaf lines (and prefixes), not only reloc_bindings.
- A local class derived from CCharString makes `a != b` pick STLport's rel-ops template unless fable_string.h declares the member operator (FABLE_RETAIL_CCHARSTRING).
- A landed test that links the row against a stand-in stub fails to link once the row binds the real spelling: check each test for the stand-in name before batching.
- Rows on the July stand-in pattern may still be cheap: 0x006E76E0 moved onto the shared header in one grade once the member type got an opt-in.
