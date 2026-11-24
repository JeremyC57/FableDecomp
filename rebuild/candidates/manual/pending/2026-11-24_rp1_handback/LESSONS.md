# RP1 lessons (one line each)
- A "declarations only" header option cannot serve a row that `new`s the class: operator new's size is the class size. Check every
  binder for `new T` / `sizeof(T)` / embedding before promising it a declarations-only port (0x009C0E50 blocked on this).
- Retyping an inline member of a row-local model renames its defined COMDAT even when the bytes hold: compare the objdump -t defined set
  against the HEAD object, and prefer a cast at the call site (0x009BFB30).
- Save each row's HEAD-object symbol sets (defined / undefined) before deleting the baseline wd: the before / after acceptance test needs them.
- A forward declaration in the header (`struct IDirect3DQuery9;`) coexists with a row-local full definition; dead local models a row
  no longer needs can simply be deleted (0x009F7BB0).
