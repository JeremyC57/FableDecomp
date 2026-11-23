NF310 held variants (not in rows/, not landing-ready):
  00766050.cpp, 007660c0.cpp  entries 5 / 6 selves retyped to EObjectAugmentationType + full Leaf (decision D1): RELOCATION_MATCH, ALL_EQ, but a
                              strong self under the PDB name duplicates the pick-any COMDATs 0x00839100 / 0x007677E0 emit (bytes equal; the one
                              relocation differs only in the GetSubDef symbol name). RENAMES_held.tsv has their section-1 lines.
  008367b0.cpp, 008385b0.cpp  entry 37 binder halves (protected CThing ctor, IAE): RELOCATION_MATCH, ALL_EQ, but the self 0x004C9030 still defines
                              the public QAE@K form and 0x005296B0 (NF308 / NF309) binds it (decision D2).
orig/ holds their HEAD copies.
