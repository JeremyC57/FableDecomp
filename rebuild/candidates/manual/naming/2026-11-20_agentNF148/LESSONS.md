# NF148 lessons (one line each)
- A no-Leaf row's DIFFER x/y alone does not say "better or worse": compare the q.py listings (exact-instruction matches against retail) and the defined-symbol diff before and after.
- COMDAT vtables never raise LNK2005, so a link of mismatched vtable copies passes; the proof of one vtable is the section bytes + relocations (coffsec.py) of every emitter.
- rtti-vtables.tsv lists a class's override slots: read it before writing an override opt-in, and check each override's landed self row spelling, or the vtable creates a split.
- Removing a strong out-of-line member definition from an old row breaks its test link: give the test a fake of that member on the row's own model.
