# NF193 lessons (one line each)
- An atexit-thunk stand-in (GlobalMethodTailThunk_<addr>) whose static's initializer row is landed re-authors in one grade: copy the initializer's source and set `// Leaf: _$E2`; check the _List_base / ctor COMDAT copies against the owner's address with q.py (row source + that leaf), not only against the initializer's object.
- A shared header's out-of-line template body (STLport basic_string(const char*)) can differ from its retail owner when the row instantiates it; a `#ifndef ..._DECL_ONLY` opt-in around the definition is the cheapest fix when the row calls it out of line anyway.
- A queue entry that says "PDB name unknown" may be wrong: grep the raw PUB32 dumps (/tmp/claude-0/egod_pdb/*.publics.txt) and the shared headers' opt-ins before calling a data name descriptive (0x013BAE1D BettingActive, 0x013BD6EC _hStrings).
- WORKLIST table witnesses go stale (0x00CF8A70, 0x00999230 already bound the PDB spelling): a HEAD probe of the witness row first saves a port.
- A shared header #included inside a row's private namespace makes a namespaced stand-in of the class (ExternalRetry_* rows): moving the include above the namespace is a one-line, symbol-neutral port.
- Access-only callee fixes in a row-local template model: put the declaration under `private:`; the explicit instantiation in the self row is not access-checked.
- Many witness rows in the old mega-families are July rows with no Leaf line: census them first and record them instead of grading them.
