# NF241 lessons (one line each)
- A family of STLport base-member binders can move with the EXISTING per-TU opt-ins (`_DTOR_OUTOFLINE` -> `_BASE_DTOR_VISIBLE_NOINLINE`, `+ _DEFAULT_CTOR_NOINLINE` -> `_BASE_VISIBLE_NOINLINE`): both pairs are TU-wide alike, and a relocation diff against the HEAD compile proves no other instantiation moved.
- The binders that UND a list member (row-local `~list();` models) are the ones to port: once the call names the base member, a TU that also defines it pick-any is not needed for the link.
- Compare pk classes HEAD v NEW per (row, symbol), not NEW alone: moving a bound symbol moves the existing NEQ pick-any copies with it (0x00545B10 / 0x00607330's 62-byte clear-inlined destructor went from ~list to ~_List_base: NEQ both times).
- A header comment fix must keep its line count, or every FABLE_STATIC_ASSERT below it moves (their typedef names carry __LINE__).
- pkrun-style scripts that call lnk.py must source wine/env.sh first, or link.exe resolves to the Windows path and fails.
- A `ret 4` self row can still carry a real PDB name (CBankFile::UpdateEntry: 525 / 1104 bytes in the PDBs, empty in TLC): check every exe reference (E8/E9 + absolute) before calling it a folded no-op.
