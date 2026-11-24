# NF224 lessons (one line each)
- A decl-only opt-in can clear a pick-any copy only where retail calls the member out of line. If retail expands it at the site (check the disassembly for the call first), every DECL_ONLY form DIFFERs the row, and the /O2 COMDAT copy is inherent to the TU.
- VC7.1 emits the COMDAT of an inline member it has fully expanded. A /O2 TU's copy then differs from an owner that a size TU supplied, even with the same source.
- To emit a protected, non-virtual class's ??_G from a header-only TU you need access: a derived class's `delete` through a base pointer failed C2248 (the message names CManager; the base-pointer reading is inferred), and naming the base dtor unqualified failed C2300. Without a friend, only an inline member of the class can do it, and that member then emits its own COMDAT, which clashes with a strong owner row.
- Check that a "catalog-only" address really has no landed row (grep the build entry) before treating the fix as a table edit. 0x00415B80 was landed under a stand-in.
- MS mangling of static data: 0 private, 1 protected, 2 public. GPUniqueInstance is `1PAV12@A` in all three PDBs.
