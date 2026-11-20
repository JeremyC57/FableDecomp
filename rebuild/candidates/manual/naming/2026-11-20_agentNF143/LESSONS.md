# NF143 lessons (one line each)
- VC7.1 emits a pick-any COMDAT for every inline function a TU uses, expanded or not: a row-local inline body under a PDB spelling is a second definition of that address's owner (link proof catches it; objdump -h shows LINK_ONCE_DISCARD).
- Before a self rename, objdump the defined symbols of every row whose source mentions the callee: 0x009FC170 defined both new leaves (a strong auto_inline(off) copy and an in-class inline) while only binding one.
- A strong same-TU copy kept "for register liveness" may not be needed: try the declaration-only form first (0x009FC170 matched at once).
- An explicit `CWideString(L"...")` operand and an inline function returning the literal compile differently (eax push vs lea re-take and swapped slots): look for the TU-mate getter in Ego_r donor order before inventing a shape (0x00406610 = GetManualSaveBaseFileName() + GFIntToWideString()).
- A 4-byte match of a function address inside .text can be a call operand of another call; confirm refs.py hits with an E8 scan before treating them as callers.
