# NF145 lessons (one line each)
- A /d2inlT sweep is cheap (~0.4 s per q.py grade): sweep every user of a header inline, not just two; here 5 of 12 needed no flag at all.
- mkpay.py takes flags only from the catalog CompilerFlags: add the `// Flags:` line to the payload yourself or vland shows a false DIFFER.
- A row whose Leaf line names a symbol its body does not define grades the largest function silently (0x009BFF10): compare the Leaf with objdump -t.
- Compare a shared vtable COMDAT across rows by section bytes + relocations (a small COFF reader), and check the slots against the retail vtable: matching copies can still both be wrong.
