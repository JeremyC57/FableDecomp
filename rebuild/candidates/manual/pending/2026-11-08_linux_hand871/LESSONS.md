# Chunk 871 lessons (GOTCHAS one-liners)

- VC7.1 `strcmp(buf, "literal")` is an intrinsic: `mov edi,lit ; lea esi,buf ; mov ecx,strlen+1 ; xor r,r ; repe cmpsb`. `memcmp` loads ecx first, so a `repe cmpsb` with edi / esi / ecx order is strcmp (0x00C52E40).
- Two local arrays with disjoint lifetimes share one frame slot under VC7.1. When retail keeps them apart (frame 0x5c vs 0x4c), put them in one local struct (0x00C52E40 vendor / descriptors).
- An expression-temporary register one rotation step off (edx vs eax) can come from an EARLIER conditional expression. `else x = !strcmp(..) ? 4 : 0;` takes a rotating temporary. A two-return inline helper (`if (!strcmp(..)) return 4; return 0;`) makes the value a register candidate (`hint`) instead, which moves every later temporary by one step. Find the site with `c2prio --rotation` (0x00C52E40).
- A single-return inline (`return c ? 4 : 0;`) is forwarded like a plain expression and moves nothing. Only a return value with two or more defs becomes a candidate (0x00C52E40).
- A `__forceinline` helper whose returns cover the WHOLE if-else chain merges the per-arm stores into one shared store (`mov eax,imm` per arm). Wrap only the last arm (0x00C52E40).
- MASM operand names: a C variable named `type` (the TYPE operator) breaks an `__asm` statement (C2400). An `__asm` operand that is a reference parameter addresses the pointer slot, not the referent.
- A statement clamp macro `if (in range) d = v; else d = v > hi ? hi : lo;` stores in each branch. The expression form merges the stores (0x00C4E3C0).
- A round-half-down helper written `return r < 0 ? r + 1 : r;` keeps the value in a register. `if (r < 0) ++r;` homes it and stores twice, +4 bytes (0x00C76E90).
- A `>=` conditional expression on floats gives `test ah,1 ; je`. The `<` form with swapped arms gives `test ah,5 ; jp` (0x00C770B0).
- An inline member accessor on a sub-object stops VC7.1 from CSE-ing the member loads across a call, which matches retail reloads (0x00C5B480 resampler +0x1E8).
- When `engine/C3DVector.h` declares a member `operator*(float)`, the libpk free `__fastcall operator*(const C3DVector&, float)` (0x00C258A0) is ambiguous in infix form. Call it as `operator*(v, s)` (0x00C45F30).
- Oracle rows can run into the next function: 0x00C5DD70 is 604 bytes, not 728, and 0x00C770B0 is 857 bytes, not 910. Check the `ret` and the 0xCC padding with xdis before chasing a size difference.
