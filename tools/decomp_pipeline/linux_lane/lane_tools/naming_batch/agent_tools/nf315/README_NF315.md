NF315 helpers (new or changed; the rest are NF313's, paths set to agentNF315):
- offl.py v3: off-limits set retargeted to NF312 / NF314 (NF309 / NF313 landed), and now EVERY 0x address on a T3_PORT_QUEUE line (column 3 unit members, column 4 binder mentions; v2 read only column 2): their ENTRIES targets + every reloc_bindings witness
  of their targets, live locks, codex-p28 reservations, held rows, T3_PORT_QUEUE.
- screen315.py: per ENTRIES line, the target's off-limits reasons and every reloc_bindings spelling with its witnesses' reasons
  (work/screen.txt; full spellings, not the 160-character ENTRIES cut).
- setpscan.py: classify every landed declarer of SetPItem(CThing*) by its enclosing class / namespace (brace-depth scan); intersect
  the global-model ones with xsites.py of 0x00A01B90 to find the rows that really call it (the rewitness candidates).
- ed315.py <group>: the exact edits (groups B, G, Q, W), each old string matched once.
- The others (delchk.py, xsites.py, absrefs.py, gr.sh, lk.sh / lnk2.py, proof.sh / objsec.py, cmpfn.py, zsrun.sh / zs.py, rf.sh,
  mkpay311.py, prep.sh / pre.sh, pdbq.sh, srcof.sh / flags.sh) are NF313's unchanged but for the folder path.
