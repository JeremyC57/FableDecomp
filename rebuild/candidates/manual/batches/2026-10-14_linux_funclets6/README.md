# Batch `2026-10-14_linux_funclets6` — EH unwind funclets of landed parents: 21 bodies (21 landed; the five 3-body EH families i1116/i1123/i1130/i1131/i1136)

Sixth pass of `tools/decomp_pipeline/families/funclets.py`, after the fifteen EH-framed parents of
`2026-10-14_linux_i1116` (observer notification), `_i1123` (file visitor), `_i1130` / `_i1131`
(container constructors and destructors: two funclets each -- the root base and the node member)
and `_i1136` (factories: the operator-delete funclet) landed. Each entry is graded from its
parent's object at the funclet's offset into the `$L` block with the parent's `/EHsc` flags and
behaviour test (see `2026-10-14_linux_funclets1/README.md`).

All graded `RELOCATION_MATCH` / `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
