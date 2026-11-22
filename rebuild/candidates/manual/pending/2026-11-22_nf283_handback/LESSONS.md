# NF283 lessons (one line each)
- Before compiling a self-leaf move, grep every landed source for another definition of the NEW name (out-of-class `Class::Method(` and in-class bodies): 0x0099F9B0 (AllocStringData IAE) and 0x00B80F90 (ConvertVectorToLocal IBE) hold strong same-TU copies already on the PDB spelling, so the self move adds an LNK2005; the link check found them only because those rows happened to be controls.
- Put every other definer of the entry's symbol (any spelling) into the link controls: the HEAD v new duplicate count is what exposes a same-TU copy clash.
- Precheck the landed test of every row you mean to change right after locking and before editing: 7 of the first 60 entries' only binders have tests that clang cannot parse (VC7.1 <cwchar> / iostream, `_assert`), and the row cannot be rechecked under the rules.
- A clang-failing binder can still be settled by reading its source: if it takes the declaration from a shared header that already has the PDB spelling (0x00753630 via engine/CAnimationEntry.h), the entry is complete without compiling it.
- A binder whose local model emits the class's vtable (inline ctor / dtor in the TU) cannot take `virtual` in place without changing that table (CDiskFileWin32 Read / Write): check `??_7` in the clang census before choosing the virtual fix.
- Some tracker "access" entries are signature splits (I v K on a ctor parameter, J v K on a return): read the wrong spelling's letters past the access code before treating it as a letter fix.
- `virtual` on a non-polymorphic local model needs the leading pad shrunk by 4 on its own line so member offsets hold (0x00B4B830); objsec ALL_EQ proves it.
- A const respell can cascade into callees (GetPFaction -> inline overload -> Find -> LowerBound stand-in): if the const body needs a non-letter change to a callee with its own row, it is T3.
- vland rechecks of these small rows run about 8 rows a minute, so a 47-row recheck fits well inside one `timeout 900`.
