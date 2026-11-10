# Identity and names

- Ego_r.pdb name set: donor 00bae3c0 `?GFGetHermiteSplinePosition@@YIXAAVC3DVector@@ABV1@111M@Z`.
- FableWin.pdb name set: donor 0324b560, identical spelling.
- Independent ordering/call evidence: both donor lists place GFDXGetHermiteSplinePosition immediately before this function. Retail 00a565c0 forwards the same argument order to D3DXVec3Hermite. Retail caller 00a56a90 belongs to CCardinalSpline::GetSplinePosition. The supplied xref tool was rerun.
- C3DVector class key V confirmed in pdb_class_keys.tsv; public X/Y/Z offsets 0/4/8 and size12 in supplied C3DVector.h (PDB TPI0x2783). Its multiply and add operators are in FableWin at018c0af0 and018c0b60 and agree with native component arithmetic.
- No named external callees or globals. Only float literal relocations: 3.0f and1.0f. Their COFF values and retail targets are audited separately.
- Exact emitted Leaf checked with objdump -t; no prefix or largest-symbol fallback.
