# Chunk 522 lessons

- GetTC copy-key form: `ETCInterfaceType key = type; it = Components.LowerBound(key); return (it==end || type < it->first ? end : it)->second;` gives retail's immediate compare; a Find(const K&) form reloads the key.
- __forceinline GetTC / Read / ReadSLONG / ceil in /Os GUI TUs; at /Os VC7.1 leaves them out of line otherwise.
- A real same-TU IS_THING_ALIVE body under `#pragma auto_inline(off)` lets VC keep the hero in a register across the call (an extern declaration reloads it).
- C2DVector returned by value needs a user ctor, or VC7.1 returns the 8-byte struct in edx:eax instead of retail's hidden pointer.
- /Os GUI TUs keep a frame pointer: `#pragma optimize("y", off)` + `optimize("s", on)`.
- Two `(*it)->Clear()` calls in if/else arms cross-jump into retail's single call site; write both, do not hand-merge.
- Retail ceil (Ego_r `?ceil@@YAMM@Z`) is a retail_asm candidate: fld/fadd 0.5/fistp/fild/fsub 1.0/fstp in asm, the integer compare in C; putting fild/fsub inside the asm stops the 1.0 constant fold.
- Access specifier is part of the leaf: private members mangle AAE (0x0064D7A6, 0x0064B5C1); check the PDB before declaring public.
- objdump reloc lines are `dir32` / `DISP32` followed by a tab; relocation-map regexes must allow it.
- Never run a recursive glob over the scratchpad: the Wine trees make it run for tens of minutes.

## Review at landing
- RELOC_CONFLICT: STLport vector<CEnginePrimitiveHandle>::~vector is byte-identical at 0x00440C40 and 0x00AF7210 (two link units); the matched rows bind 0x00440C40.
- 0x0040F8A0 CPersistTraits<unsigned long>::TransferBinaryIn is COMDAT-folded with CPersistTraits<float>; one retail address, two template spellings -- pick the one the landed float callers use or keep the (unsigned long&) spelling here.
- 0x00643D5D carries two landed names (GetFadedAlpha on two classes); descriptive `GetFadedAlpha_00643d5d` here, needs one owner class.
- 0x00654438 is a retail_asm candidate (inline ceil, `// Retail-asm:` evidence 0x006544EC/0x006544EF/0x006544F2); still DIFFER on frame-slot order, so parked.
- 0x004FA0A9 catalogue name says CAbilityButtonGuiDef; evidence says CCardDef -- respell the catalogue row.
- Header debt as in NOTES.md (five classes).
