# NF305 lessons (2026-11-23)

- **Triage by mangled difference first.** `tools/mdiff.py` splits each stand-in against its PDB spelling into SCOPE / QUAL / SIG / CLASS /
  FREE. The QUAL ones (access or const letters only) were nearly all in place; CLASS / FREE were all ports. It saves opening most binders.
- **A table line is one witness; the stand-in usually has more binders.** 14 of 32 changed rows had no table line. Census per target:
  exe call sites (xsites), then the declaration each caller row uses (memacc: access label in force + const). Then confirm with objects:
  a matching-looking declaration can still bind a different stand-in (a `long` typedef return, a `_Retail` alias class).
- **Read the owner's HEAD object before an access fix.** Three owners defined the public stand-in themselves (0x00918880, 0x006D8170,
  0x006D83B0); a binder-only fix would have broken links that HEAD resolved. Fix owner and all old-spelling binders together.
- **A standalone test that defines the callee stub must follow the new spelling.** vland LINK_FAILed 0x00892F40's test (stub non-const);
  precheck the edited test (here a `this`-capturing global also had to become const).
- **Pointer -> reference return (both PDBs) is byte-neutral** for `->` uses; check every use for a non-const member call on the result
  (0x008D6DA0's entry calls a non-const stand-in, so it stays a port).
- **Do not `sed -i '/addr/d'` a one-line list file**: it empties the whole list (my first HEAD batch graded nothing).
