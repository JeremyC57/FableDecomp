# Batch `2026-10-14_linux_funclets10` — EH unwind funclets of landed parents: 7 bodies (7 landed; the hand-authored EH parents of batch eh6)

Pass 10 of `tools/decomp_pipeline/families/funclets.py` after the hand-authored EH-framed parents
of `2026-10-14_linux_eh6` landed: every entry is an `Unwind@<va>` funclet graded from its parent's
object at the funclet's offset into the `$L` block, with the parent's `/EHsc` flags and behaviour
test (see `2026-10-14_linux_funclets1/README.md`).

All landed funclets graded `RELOCATION_MATCH` / `PASS` on 2026-10-14.
