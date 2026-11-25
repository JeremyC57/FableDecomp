# NF414 hand-back (2026-11-25): void*-cast baked strings

28 rows, all ACCEPT, landed as `2026-11-25_landing_nf414_b1..b3` (RELOCATION_MATCH PASS -> PASS, 0 LITERAL_DIFF).

- b1 0x00452B4D: `(const void*)0x0122D70E` -> `""` (retail's pooled empty string).
- b2 24 StatGetClassTypeName rows (0x004D2A91, 0x004D2B04..0x004D2D13): literal spelt, re-formed in class
  (`?StatGetClassTypeName@<Class>@@SI?AVCCharString@@XZ`, `// Leaf:` line) in the NF406/NF408 pick-any form.
- b3 0x004D4369 / 0x004D5FF2 / 0x004D66C3: the same re-form, NF413 literals kept.

Coordinator applied: 27 self_symbols lines (col4 = `// Leaf:`), the 27 stand-in callee deletes at 0099ebf0
(by land_scripted), closed T3 lines `nf413 0x004D4369 self` and `nf413 0x00452B4D baked`, added the six `nf414` lines.
The purity detector still misses `void*` / typedef casts; NF414's widened scan is queued as its second pass.
