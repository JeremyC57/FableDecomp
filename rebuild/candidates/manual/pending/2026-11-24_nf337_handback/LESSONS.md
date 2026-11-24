# NF337 lessons
- The include tree copy goes stale while other lanes land: refresh work/tree from rebuild/include before every equiv run (b4's single
  differing row was drift in C3DVector.h, not an edit of mine; evidence/tree_drift.txt). Check `diff -rq rebuild/include work/tree` lists only
  the hand-back's headers.
- An `ALL_EQ` objcheck needs the whole rename map: callees can be made up too (0x004E14FD's `CloneFields`, 0x004EB6C8's `SubContainer::Copy`).
- `*this = *(const C*)other` with an implicit operator= matches the Copy bytes but leaves pick-any COMDAT bodies of the implicit operators in
  the object; compare DEF sets, not just the grade.
- PDB-typed float members pull in `UND __fltused`; flag it, don't retype.
- A local `namespace std` model is avoidable: stlport/fable_containers.h + FABLE_RETAIL_STLP_VECTOR_ASSIGN declares vector::operator= out of line.
- A drv.py run dir is single-use and checks gen/pre staleness: drop or move scratch test files out of work/gen before the next grade.
