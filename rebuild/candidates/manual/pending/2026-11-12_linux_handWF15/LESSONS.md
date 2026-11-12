## 0x00A60D40

- Register-held vptr constant across an inline ctor/dtor pair (mov esi,vtbl reused for the dtor store) = the source has an early return through the dtor (multiple exits), not a nested if.

## 0x0091DDD0

- Frame-slot order residue (same instructions, swapped slots): VC7.1 ranks slots by block-frequency weight; a loop exit spelled break vs return reweights blocks. Read the /FAs '_sym$ = -N' map to search spellings quickly.

## 0x005FBAA0

- (wf15, 2026-11-12) An x87 commutative operand order no spelling moves followed the count of loops earlier in the function (period 4); a two-float aspect scale initialised by a 2-trip loop, declared after the extents, fixed both it and a frame-slot pack (0x005FBAA0).

