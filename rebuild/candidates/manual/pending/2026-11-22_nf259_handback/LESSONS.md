# NF259 lessons (one line each)
- A slot getter that is `fld dword ptr [K] ; ret` reads a pooled float literal when K has other readers (absrefs): write the literal, not an extern; the object then binds `__real@<hex>` and the made-up data name disappears.
- `return Virtual() ? true : false;` keeps VC7.1's non-tail call + `test al,al ; setne al` for a bool forwarder (retail 0x00692670); try it before any other spelling.
- Before trusting a HEAD comment about a def-layout shift, check a landed reader of a neighbouring member on the same header (0x007FDE00's +0x40 read proved CSummonDef.h's offsets, so +0x44 is SummonEffect).
- The linker deduplicates an object named twice on its command line, so the "same object twice" link control proves nothing: copy the object under another name for the positive control.
- `cmp a b && echo same` followed by an unconditional evidence append is a trap: write evidence only from the command's own output.
- A HEAD `// Flags: (none)` text reaches payload builders as a literal flag (0x007C3FB0); filter `(none)` / `none` / `default` as well as empty.
- An implicit destructor already modelled in a shared header (CastAssassinRush, owner 0x0072D990) replaces a row's hand-declared out-of-line `~T();`: the row then emits the pick-any copy; check it EQ the owner with pk.py.
