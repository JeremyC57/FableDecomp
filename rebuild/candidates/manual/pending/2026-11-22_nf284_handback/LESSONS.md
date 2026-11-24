# NF284 lessons (2026-11-22)

- proof.sh builds its rename map from the two objects' own symbol diff, so ALL_EQ alone would also pass a mis-mangled new symbol; check
  the map explicitly against the expected PDB spelling (evidence/rename_check31.txt does it by string compare).
- For runs of byte-identical sibling deleters (0x00B36060..0x00B361B0), the file / catalog name is not evidence of T: the reloc_bindings
  lines whose TARGET is the binder give a real-typed caller (here one caller, 0x00B33B50, for all 22) that names T per call site.
- xf_ccp.py needs the HEAD object's symbol list (work/hc/sym_<b>.txt) for the Leaf pin: run hk.sh before the transformer.
- 31 generated binders + 31 owners compile in a few minutes on hk.sh / nc.sh; a vland recheck of 16 such rows took under a minute.
