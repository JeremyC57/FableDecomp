# NF449 hand-back (2026-11-25): T1 special-member / operator caller respells
65 caller rows ACCEPT in 3 batches (s1 43, s2 8, d1 14), 65 callee rebinds onto 16 T1 targets; each row's self unchanged, no header changes.
- s1 / s2: the row's own bodiless model ctor takes the PDB parameter types (pointer -> reference, long -> unsigned long), and call sites pass `*x`.
- d1: destructor stand-ins become a qualified `p->Q::~Q()` (r2/r3); 2 tests respelt (q2).
- Land s1, s2, d1 in any order, then s1/CROSSBATCH.tsv.
Held: r1_ready_r449a (11, all gates pass, RESCOPE form, ruling r449a), r1_hold (4, objgate NOT_EQ), s2_hold (2, unlanded target, r449b), d1_hold (2, strict, r449d).
Details in NOTES.md, rulings in RULINGS_NEEDED.tsv, residue in RESIDUE.tsv.
