NF313 helpers (new or changed; the rest are NF311's, paths set to agentNF313):
- offl.py v2: off-limits set; now also every reloc_bindings witness of an NF309 / NF312 target (their ENTRIES witness lists are cut).
- sig313.py <census> [entry...]: demangled PDB v stand-in signature + the witness's lines naming the member, per SIG / OPEN census line.
- owner313.py <entry line>...: the owner row's Leaf and Class::Member definition lines (cheap self screen before a binder fix).
- delchk.py <target> <member>: every exe call site of target (xsites.py) -> landed caller rows and their declaration lines of member,
  plus absolute-reference count; writes evidence/xsites_<target>.tsv. The delete-line census.
- ed313.py <group>: the exact edits (groups e18, A, M), each old string matched once (4th field 2 = replace every occurrence).
- srcof.sh / flags.sh: bounded to the row's own catalog block (NF294 fix).
