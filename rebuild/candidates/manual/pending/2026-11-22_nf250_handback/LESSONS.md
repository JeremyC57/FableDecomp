# NF250 lessons
- A ctor row ported off a row-local model onto the shared class header starts emitting that class's vtable and its ??_G pick-any copy:
  map each ??_G symbol to its owner (vtable slot 0, tools/vslot.py) in owners.tsv and pk.py it. Four were EQ; one (PickUpTankard, whose
  base has a CIntelligentPointer member) was not, which holds the row.
- Emission harnesses for ??_G rows go in an anonymous namespace: a global `EmitIntelligentPointerDeletingDestructor()` is already defined
  by the landed 0x0069BAF0, so a second row with the same helper name is an LNK2005.
- A self row whose retail TU is /Gr (STL instances next to a /Gr caller) can need `// Flags: /Gr` even for a thiscall member: at the
  default convention VC7.1 kept uninitialized_fill_n out of line in vector(n). The caller's own pick-any copy of the function (pk.py on
  the caller's HEAD object) tells you which flags reproduce it.
- An opt-in `#define` placed after an include that already pulled the header in does nothing (header guard): put it with the other
  defines, before the first include.
- equiv_check over the whole tree takes about 20 minutes on the cpp-only path; run it in the background beside the cl grades. A hard-link
  copy (`cp -al`, then `cp --remove-destination` of the overlay files) is a cheap second include tree that leaves the repo files intact.
- A `// Flags:` comment line used for prose is read as a flags line by tools that take the first match: write prose as `// Flags note:`.
- Porting a ctor (or a ??_G harness) onto the class header makes the object emit the class's full vtable from the header's virtual set.
  Run vtcheck.py on every new `??_7` against the retail vtable BEFORE calling the row done: a header that does not declare the class's
  overrides binds base-class spellings at the override slots (new slot splits). If a landed row already emits that vtable at HEAD,
  copying that row's #define / #include block reproduces its vtable slot-for-slot (diff 0), which is debt-neutral.
