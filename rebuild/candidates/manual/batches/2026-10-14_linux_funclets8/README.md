# Batch `2026-10-14_linux_funclets8` — EH unwind funclets of landed parents: 7 bodies (6 landed; the hand-authored EH parents of batch eh3 and eh4)

Pass 8 of `tools/decomp_pipeline/families/funclets.py` after the hand-authored EH-framed parents
of `2026-10-14_linux_eh3 and eh4` landed: every entry is an `Unwind@<va>` funclet graded from its parent's
object at the funclet's offset into the `$L` block, with the parent's `/EHsc` flags and behaviour
test (see `2026-10-14_linux_funclets1/README.md`).

All landed funclets graded `RELOCATION_MATCH` / `PASS` on 2026-10-14.
