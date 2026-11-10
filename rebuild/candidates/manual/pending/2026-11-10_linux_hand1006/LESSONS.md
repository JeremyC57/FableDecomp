# chunk 1006 lessons
- 0x0056EF2D: a named local read of a member at the top of a branch moves that load ahead of the stores and into edi; 23 probes, 1 grade.
- 0x00570332 / 0x00571280: a named object with a destructor declared at a block's bottom is live, for the frame packer, from the block's opening brace; scoping it in its own `{}` frees the slot it blocked.
- 0x005B3E4E: a same-TU template body (GetSubDef<T> under `#pragma auto_inline(off)`) makes its out-param local uncaptured, enabling a CSE across later calls; picking between two const-ref results as `p = &a; if (...) p = &b;` (no else) shares a's slot.
- 0x005B3E4E: retail's inline CAnimInit argument constructor (fldz frame stores, then Init) differs from default ctor + Init; and passing a named update struct, not a temporary, fixed the last frame-slot difference.
- 0x00577AC0: VC7.1 reassociates a 4-term integer member sum by a cyclic rotation that depends on earlier IL (stores, calls); none of ~110 source changes produced retail's +0x268,+0x26C,+0x264,+0x260. Park such rows early.
- Rebase overlay headers on the current HEAD copy before handing back (HEAD gained another agent's opt-ins mid-session).
