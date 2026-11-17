# NF62 lessons (one line each)
- Copy the define/include block of a landed header-based row of the same TU before the first grade: missing defines cost 5 grades (007e7c90, 006d41f0).
- A header body that seems to need an "inline split" can reproduce it by itself: 006dcb20 matched on the plain STLport _M_insert once the inner map copy ctor was kept out of line.
- If retail reads an address-taken local after a temporary's base ctor, look for a default-construct + setter spelling of that temporary (00799900).
- An explicit-specialization declaration of an in-class member template after the class makes VC7.1 ICE (C1001); a declaration-only opt-in of the template works (004fe630).
- Absolute log paths only: a relative vland log path wrote into the repo work/ dir.
- End every Bash call with `; true` when the last command may fail (two exit-1 slips).
- A temporary whose destructor retail expands needs the class's destructor implicit: an in-class `{}` body was still called out of line
  (0x00713510). Try the implicit form first when the header declares the destructor only for the vtable.
- Named vector transfers are TransferVector in both PDBs (40 of 40); grep the PDB for the overload family before trusting the header's
  plainer Transfer overload (0x006C7370).
- Hoisting `end()` into a local and storing `find()` in an iterator local are retail's shapes in these loops (0x006C7370, 0x006C3620).
- Never put an instruction listing inside a double-quoted shell argument: backticks run. Write notes through a quoted heredoc file.
- Probe a row cheaply by script (work/mkdraft.py): delete the local blocks that have shared headers, include them, grade once; the error
  list names exactly the PDB members and opt-ins still missing.
