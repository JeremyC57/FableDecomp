# NF212 header overlay (add-only opt-ins; `orig/` = the repo working-tree header at start = HEAD)

## stlport/fable_containers.h
- Base: `rebuild/include/stlport/fable_containers.h` (copy in `orig/stlport/`, byte-equal to the repo file).
- Added: `FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY` (row 0x004333B3): a declaration-only
  `template<class I,class N,class T> I __fastcall uninitialized_fill_n(I first,N n,const T& x);` (plain convention off MSVC), wrapped
  around the existing NOINLINE / inline `uninitialized_fill_n` block as `#ifdef ... #else <existing block unchanged> #endif`. The
  owner 0x00433504 defines that instance; the visible noinline body would give a 41-byte pick-any copy against the owner's 38.
- Default path: token-identical (the existing block is untouched; the new lines are only seen with the macro defined).
- pin_asserts.py: pinned 0.
- Includers: 2180 landed rows under rebuild/src/compiled include stlport/fable_containers.h; none defines
  FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY (grep of rebuild/src/compiled and rebuild/include: 0 hits), so the opt-in changes
  none of them.
