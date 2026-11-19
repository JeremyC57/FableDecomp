# NF105 lessons (one line each)
- Compute the closure from the exe scan BEFORE porting: a header class (here CReverbFns) moves every overload a ported row calls, so one binder can pull a chain (0x00C76B60 -> 0x00C766F0 -> 0x00C76C20 -> 0x00C76560 / 0x00C767E0 -> 0x00C75820 / 0x00C89350): eight rows nobody had listed.
- Grade the riskiest rows of a whole-or-nothing family first (go / no-go); here all four went first time, so the rest was mechanical.
- The CODAApiDbg.lib /Od bodies call every header inline out of line (Token, MatchToken, GetWriteCursor, Create / Clear of the buffers): list their calls to recover the source statement order and the inline API before writing a port.
- The release COMDAT of a header inline (e.g. ?Clear@CFloatCyclicBuffer) is in CODAApi.lib even when every caller expands it: disassemble it for the exact body.
- Release relocation names beat Dbg calls: the Dbg Create calls CFloatFilter::Create, the release names Clear at the same site (Create is an inline `jmp Clear`); spell what the release names.
- A gate on callers only misses a callee's own definition: check self leaves of non-family targets too (work/n5/gatecheck.py prints them).
- Not-landed callees have no catalog oracle: compare Fable.exe bytes to the SDK member directly (work/n5/sdkcmpx.py) before putting a name in NAME_FIXES; one of seven (0x00C75050) came from another SDK build.
