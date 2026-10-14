# Batch `2026-10-14_linux_funclets3` — EH unwind funclets of landed parents: 14 bodies (6 landed; the eight funclets of the two-funclet constructors F676 landed in `2026-10-14_linux_funclets4`)

Second/third pass of `tools/decomp_pipeline/families/funclets.py` after the EH-framed parents of the
assertion handlers (i47), mailbox queue steps (i163, i328), two-level constructors (i676), observer
registration (i1117) and pair destructors (i1121) landed: every entry is an `Unwind@<va>` catalog
function graded from its parent's object at the funclet's offset into the `$L` block, with the
parent's `/EHsc` flags and behaviour test (see `2026-10-14_linux_funclets1/README.md`).

All graded `RELOCATION_MATCH` / `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
