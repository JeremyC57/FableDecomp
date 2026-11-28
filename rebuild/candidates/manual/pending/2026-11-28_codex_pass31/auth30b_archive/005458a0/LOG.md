g1: row-local ~_List_base specialization removed; shared-header size-body opt-in (overlay fable_containers.h, owner 0042ABCA pragmas) + clear declaration-only; hypothesis: the copy becomes the 23-byte owner body and the leaf is unchanged.
g2: same as g1 but the size-body specialization plain inline (noinline moved behind a sub-macro, not set); hypothesis: noinline changed the leaf frame.
g3: g1 with clear visible-but-noinline (FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE) instead of declaration-only; hypothesis: the leaf frame change in g1 came from the opaque clear call inside ~_List_base.
g4: c0 minus the row-local ~_List_base explicit specialization (shared header default body); hypothesis: same leaf and the same 62-byte copy as HEAD 00545B10 (debt), so the row-local STLport text is not needed.
g5: c0 minus the row-end optimize("s",on) (row-text pragma not allowed); hypothesis: leaf unchanged, the end-of-TU copies turn /O2 and are identical to HEAD 00545B10 copies (debt).
