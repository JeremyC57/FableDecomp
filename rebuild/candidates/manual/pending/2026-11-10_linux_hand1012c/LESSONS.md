# Lessons, chunk 1012c (GOTCHAS-ready)

- 0x0089CE70: a one-line squared-distance test written right before an inlined `Normalise()` of the same offset gets CSE'd as a whole sum; spell the test as statements (`float sq = Z*Z; sq += X*X + Y*Y;`) to keep retail's separate sums (masked diff down to 28 lines).
- 0x0089CE70: the earlier `max(smell, sound)` radius test likewise needs its own statement-form sum; `sq = Z*Z; sq += Y*Y + X*X;` reproduces retail's Z,Y,X load order (grid of 18 spellings, 28 -> 22).
- 0x0089CE70 / 0x0089C910: `if (GetTC(...)) { ...; return false; } return false;` with explicit nested `return false` arms reproduces retail's shared-return layout and register choice; a single-exit form flips perceiver/pos registers.
- 0x0089CE70: `FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER` (GFSqr) fixes the fstp interleave of the inlined Normalise sum; GROUPED_SUM / UNPAREN / default do not help the remaining X*f operand order.
- 0x0089CE70: not a lever: renaming the cone compare (`dot > cos`, named float/double cosine, named `const C3DVector& facing`, `Normalise().Dot`) — each moves the Dot to frame reads or flips registers (nd 43-160 v 22).
- 0x008A7A70: `/d2inlS<N>` has no effect on the GetSummonedCreature / push_back expansion; `/d2inlT85` expands all of them and overshoots (702 bytes v 738).
- 0x004A1840 CWorld::Load: catalog length 1025 stops inside the catch funclet (Catch@004a1c41); the function runs to 0x004A1CA8 (1129 bytes, post-catch epilogue at 0x004A1C86) — any EH row whose catch funclet is its own catalog row needs its extent checked before grading.
