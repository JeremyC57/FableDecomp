# NF193 overlay

## stlport/fable_string.h (copy of rebuild/include/stlport/fable_string.h at the session start; base in orig/stlport/)
- Added `FABLE_RETAIL_STLP_STRING_CSTR_CTOR_DECL_ONLY` (q4191, row 0x008F9820 LSocket::Open): an `#ifndef` around the out-of-class
  definition of `basic_string(const C*, const A&)`, so a row whose retail TU calls 0x00568440 out of line emits no pick-any copy of the
  constructor nor of `_M_range_initialize<const char*>` (the header's body of the latter differs from 0x005672D0: 90 bytes, the throw call
  laid out mid-body, work/q4191/ri.txt). Same pattern as FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY.
- Without the macro the header is token-identical to the repo copy (the added lines are an `#ifndef`, a comment and an `#endif`).
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT).
- Landed includers: the opt-in is new, so no landed row defines it; their preprocessed text is unchanged (`grep -rl fable_string.h
  rebuild/src/compiled` lists the includers; none of them can see the new macro).
