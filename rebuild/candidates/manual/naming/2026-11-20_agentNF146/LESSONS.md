# NF146 lessons (one line each)
- A row whose stand-in CWideString model carried four same-TU noinline member copies matched header-only on the first form (0x009C7A60): try the plain shared-header form before reproducing same-TU copies.
- A made-up row-local "format the index" helper was the PDB inline GFIntToWideString (engine/GFIntToWideString.h): grep the existing inline headers for the body shape (default ctor + Sprintf(L"%d") + copy + dtor) before keeping a descriptive helper.
- Data stand-ins named for format strings (FormatNumber / Prefix) were .rdata literals: read the exe (L"%d", L"%") and write the literal.
- The nf86 / nf88 pick-any scaffolding does NOT clear a clash with a row that defines the same symbol at COMDAT selection 1 (nodup): pick-any vs nodup is still LNK2005 in either link order (q11817, work/link/ctl_any*.log). It only helps when every other definer is pick-any.
- Before renaming a stand-in self row onto an STLport inline, census its exe-scan referencers: 0x0041C7A0 / 0x0041C7C0 were bound by every caller under the STLport spelling already, so the rename closed two splits at no cost.
- Pointer-vs-reference global stand-ins of a PDB fastcall overload (ConstructString(CWideString*, long)) respell to the namespaced reference form with no byte change.
