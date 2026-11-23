# Landing binders 1 (Account A, 2026-11-23): 16 callers respelled onto the PDB access / const of their callee (T1, scripted)

Script: `tools/decomp_pipeline/linux_lane/lane_tools/access_fix.py --binders`, landed with `land_scripted.py`.

- **Shape.** A T1 target whose own row already defines the PDB symbol P (in the tracker's spellings column as `(self)`), while a caller (a
  `reloc_bindings.tsv` witness) still references a spelling W that differs from P only in access / const.
- **Edit.** The caller's own local model declaration of the callee takes P's access label or `const`, the same edit `access_fix.py` makes
  for a self. The caller keeps its own `// Leaf:`. Virtuality and return-type changes are not made for callers.
- **Proof per caller.**
  - the HEAD and new objects are ALL_EQ under the one rename W -> P (objsec), so the only change is the callee reference;
  - the caller's own graded symbol is unchanged, and its grade is MATCH / RELOCATION_MATCH PASS;
  - the target's own row defines P, so the reference now resolves.
- **Table.** 16 `replace W -> P` lines (`TABLE_CHANGES.tsv`), applied by `apply_table.py`: no misses, and no new multi-target symbol.
- **Run `bd1`.** 27 candidates, 22 generated, 16 accepted (73%). 6 rejects are queued as `binders1`. 5 rows were skipped (the
  declaration does not start its line, or a nested scope).

## Recheck
`logs/recheck.log` (landed tree, 16 callers): 16 MATCH / RELOCATION_MATCH PASS.
