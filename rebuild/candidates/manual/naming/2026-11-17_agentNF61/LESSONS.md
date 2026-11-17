# NF61 lessons (round 7)
- A binder that only needs its container call fixed can move just the container onto stlport/fable_map.h and keep its other local
  views; the same-TU stand-in bodies that told VC7.1 "only reads its argument" map to the header's visible-but-noinline opt-ins
  (FABLE_RETAIL_STLP_MAP for insert_unique / operator[], STLP_TREE_MFIND_NOINLINE, STLP_VECTOR_OVERFLOW_OUTOFLINE).
- A local `std::pair` / `std::bitset` / `std::vector` stand-in collides with fable_containers.h: delete it with the map model; the
  real bitset<274> is 36 bytes, so drop the pad that followed the empty stand-in.
- CTCBase.h forward-declares CCharString only: include fable_string.h yourself before using string literals with CTCHaste.h.
- Inline members the header has (CTCHaste::SetActionSpeedMultiplier) can still be called out of line in big callers; /d2inlS20 did
  not help, a member-specific FABLE_FORCEINLINE opt-in did.
- STLport __destroy_aux: the decl-only opt-in also makes the POD members' range destroys calls; the inline __fastcall variant
  (FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL) lets VC7.1 drop those and keep the two non-trivial COMDAT calls.
- A row with no `// Leaf:` line can be graded on the wrong function (q.py warns "graded the largest function"); add the exact leaf.
- Prefix Leafs fool a split check: compare against the old row's real defined symbol (its local declaration), not the Leaf prefix.
- vland needs the lane env (`source $S/wine/env.sh`), or verify_and_land.py looks for D:\Documents\FableTLC.
- Edit row text with python, not sed, when the text has quotes or `&`.
