# Batch `2026-10-14_linux_funclets32` — EH unwind funclets of landed parents: 26 landed (the hand-authored EH parents of batch eh27: the first nine parents)

Pass 32 of `tools/decomp_pipeline/families/funclets.py` after the hand-authored EH-framed parents
of `2026-10-14_linux_eh27` landed: every entry is an `Unwind@<va>` funclet graded from its parent's
object at the funclet's offset into the `$L` block, with the parent's `/EHsc` flags and behaviour
test (see `2026-10-14_linux_funclets1/README.md`). The pass also re-tries the standing DIFFER
funclets (8–14-byte unwind bodies whose retail block order differs from the DDK object); they are
not landed.

All landed funclets graded `RELOCATION_MATCH` / `PASS` on 2026-10-14.
