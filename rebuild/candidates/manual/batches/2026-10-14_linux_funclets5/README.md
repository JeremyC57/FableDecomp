# Batch `2026-10-14_linux_funclets5` — EH unwind funclets of landed parents: 3 bodies (3 landed; the observer-registration parents F1117)

Fifth pass of `tools/decomp_pipeline/families/funclets.py`, after the three EH-framed
`CParticleProgram::AddConstraintRepeat` bodies of `2026-10-14_linux_i1117` landed: one `Unwind@<va>`
funclet each (the scoped lock's destructor on the frame slot), graded from the parent's object at the
funclet's offset into the `$L` block with the parent's `/EHsc` flags and behaviour test (see
`2026-10-14_linux_funclets1/README.md`).

All graded `RELOCATION_MATCH` / `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
