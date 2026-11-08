# Chunk 858 lessons (GOTCHAS one-liners)
- A float call result added into a running value (`y += f()`): naming it (`h = f(); y += h;`) lets VC7.1 schedule an unrelated integer load ahead of the `fadd`; the inline form keeps the `fadd` first (0x00494900, 10 sites; 0x00648820 tile BRY stores).
- VC7.1 `#pragma optimize("s")` TUs do not inline a plain `inline` helper that has float locals or builds a C2DBoxF return; a retail-inline FableWin helper there may need `__forceinline` to reproduce it (0x00648820 operator+(C2DVector, C2DExtentsF)).
- A C2DBoxF built from `pos.X, pos.Y, pos.X + w, pos.Y + h` copies TLX/TLY with integer moves; retail's single `fld X; fld Y` comes from reading pos into float locals first (0x00648820 borders).
- VC7.1 tail-duplicates a dtor epilogue at one return only; turning the other early returns into `goto` to the end moves the duplicate to the remaining return (0x00682810), at the cost of frame repacking.
- `IS_THING_ALIVE(const_cast<T*>(*it))` (a temporary for the by-reference argument) keeps the element in a register; passing the named local makes it memory-resident (0x00682810 wives loop).
- One `PeekTC` (HasTC test reused) instead of `HasTC` then `PeekTC` removes the second bitset test retail does not have (0x00682810 targeting).
