# NF345 lessons
- Most of a "leaf bound elsewhere" census is noise: `_$E<n>` statics (storage class 3, per TU) and EH funclet rows (graded with the parent's
  symbol, bound to the parent). Filter them out before you hand out the queue (census_leaf.py patch).
- Before a table `replace` / `delete`, run ref_scan.py, then xr every other referencer it finds. Then rerun pk_all under the APPLIED table: build a
  shadow FABLE_ROOT with symlinks and a copied reloc_bindings.tsv, and apply the changes there with apply_table.py. That run shows the copies
  the change repairs (0040cca0 YZ NEQ -> EQ) and any it breaks.
- When ref_scan says TOO_MANY / "no identifier", an xscan of retail callers of the target settles it: if no other landed row calls the target,
  no landed relocation depends on the line.
- Retail call order in sibling functions, checked against the donor's call order and object order and sizes, proves a swapped naming of
  same-shape functions (PointIsWithinCell TR/BL/TL/BR) even when the bodies differ only in one fadd.
