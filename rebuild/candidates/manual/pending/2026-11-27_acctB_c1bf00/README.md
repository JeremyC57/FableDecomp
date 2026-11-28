# 0x00C1BF00 BuildViewMatrix

Retail Fable.exe 0x00C1BF00, 174 bytes, `ret 8`. No symbol in Ego_r.pdb, FableWin.pdb or Ego_d. `BuildViewMatrix` is a descriptive name, not a PDB name (EXTRA_RULES: one descriptive name where no PDB symbol exists; also listed in `rebuild/oracles/descriptive_names.tsv`). The catalog label `C3DMeshObject_UpdateQuadricMatrix` is propagated. See NAME_FIXES.tsv.

The matching spelling is `__stdcall` (so the callee pops the two stack arguments), `forward` declared before `up`, SetIdentity's nine zeros then `m[0]`, `m[8]`, `m[4]`, and parentheses around each `DotProduct`. The same-TU caller has external linkage; a static caller is discarded and the body ends in `ret`.
