# Batch `2026-10-14_linux_funclets4` — EH unwind funclets of landed parents: 8 bodies (8 landed; the two-funclet constructors F676)

Fourth pass of `tools/decomp_pipeline/families/funclets.py`, limited to the eight `Unwind@<va>`
funclets of the four two-level `CIntelligentPointer` constructors landed in
`2026-10-14_linux_i676` (parents 0x00C1FCC0, 0x00C1FD60, 0x00C1FE00, 0x00C20230; two funclets each).
Every entry is graded from its parent's object at the funclet's offset into the `$L` block, with
the parent's `/EHsc` flags and behaviour test (see `2026-10-14_linux_funclets1/README.md`).

Two fixes were needed before they reproduced (both recorded in `docs/pipeline/GOTCHAS.md`):

- the state-0 funclet destroys the root base on `this` with no offset, so the root is a
  polymorphic `__declspec(novtable)` class owning the vptr at +0, not an empty base placed at +4
  (the parent body is byte-identical either way; only the funclet tells the two models apart);
- the parent's inline base constructor is also emitted out of line beside it under `/Gy`, with a
  `$L` block of its own, so the funclet extractor now tries every `$L` block in section order
  instead of the first one.

All graded `RELOCATION_MATCH` / `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
