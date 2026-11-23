# NF287 lessons (2026-11-23)

- **The tracker's `spellings` witnesses can be stale.** For 22 of the first ~100 entries, the witness binder's HEAD object already binds
  the PDB spelling, so only the table line is wrong. Compile the witness at HEAD and check its symbols (tools/objbind.py) before
  classifying an entry. Then grep the code, with comments stripped, for the stand-in identifier (tools/codegrep.py): another row may
  still bind the same symbol while the table keeps only one witness.
- **Hidden binders are common.** The table witness is one row; 0x0041A410 had three more binders and 0x00AE18C0 one more, none with a
  table line. Map every exe call site of the address to its row (tools/xsites.py) and move them all in one pass.
- **A free stand-in for a class static or a namespace function is in-place-shaped.** Declare the real member in the row's existing
  local model (adding `friend class X;` when the header has it), or add a one-line `namespace N { ... }` declaration. The bytes do not
  change; only the symbol does.
- **A declared-but-undefined member template called from a row** makes VC7.1 add a `/alternatename` linker directive to .drectve.
  The code sections stay equal, but objsec flags .drectve. Check .drectve before calling such a respell ALL_EQ.
- **Replacing an inline wrapper with an out-of-line declaration** can drop a pick-any COMDAT. Compare objects by symbol
  (tools/cmpfn.py) as well as by section index: one missing section shifts every later section in a section-aligned compare.
- **Never pipe a tool that writes its output file at the end into `head`.** It dies of SIGPIPE before the write; pay.py did this once
  and the recheck then graded HEAD sources. Redirect to a file and read that.
- **A flat-view add-only opt-in works for protected members too.** The derived Methods struct can call them, and a member's mangling
  carries no class key. It does not help when the stand-in class is the binder's own class (a self port), or when the binder's flat
  struct does not derive the declaring class.
