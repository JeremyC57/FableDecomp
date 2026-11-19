# b26 (hand3c chunk 1, Account B, 2026-11-19): 0x00AC7EB0 QuadricOptimiserInternals::COptimisedPrimitive::ComputeSelfContraction -- READY

- Grade: RELOCATION_MATCH (official g.py grade + strict vland `RELOCATION_MATCH PASS`, LANDVERIFY_STRICT_HEADERS=1, no warnings).
  ATTEMPTS.log: 63 grades (62 earlier + 1 confirming grade); the advisor round ran as no-ATTEMPTS probes (probe/eb0/pp.py), kept in probes/.
  Flags: none. Overlay headers: none (repo headers only). Leaf
  ?ComputeSelfContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z (private), 215 bytes, `ret 4`.

## Identity
Ego_r.pdb private member (unchanged catalog name); Ego_d.pdb locals sub_contraction, vertex1, vertex2, tex_diff, normal_diff, cost (no others).

## What closed it (advisor reframe: one cause at both terms)
- Symmetric one-statement source (`t*t*Wt + n*n*Wn`, probes/sym.cpp): BOTH terms come out `x * w * x` (c2 reassociates the chained
  product; normal term evaluated first). Brackets, GFSqr, one-statement sums, squaring only one term in place: all keep a reassociated
  or reordered term (probes vB..vG, vI, vJ, fw2: 2 or 9 differing instructions).
- Matching form (probes/vH.cpp = src): each distance squared in place right after it is measured (`tex_diff *= tex_diff;`,
  `normal_diff *= normal_diff;`) and the cost built in two statements (`float cost = tex_diff * Wt; cost += normal_diff * Wn;`).
  Retail 0x00AC7F2B..0x00AC7F3D: tex squared in place then weighted, then `fld st(1) ; fmul st(2) ; fmul [ecx+78h]` for the normal.
- Marked FITTED in the source header (the two in-place squares + the two-statement cost; justification and measurements written
  there). The `cost +=` accumulation is FableWin.pdb's statement form for this function.

## Advisor checks done
- GFSqr: FableWin/Ego_r ?GFSqr@@YIMM@Z = `float __fastcall GFSqr(float)` by value, non-template = the shared C3DVector.h / CQuaternion.h
  spelling; no lever there.
- Parameters accessors: Ego_d type 0x72D1F QuadricOptimiserInternals::COptimisationParameters is a 40-byte data-only class (field list
  0x72D1E: Scaling .. MaximumBonesPerVertex, all public, no methods); no accessor on it in Ego_r / FableWin / Ego_d names.
- FableWin form (`cost = 0; cost += t*t*Wt; cost += n*n*Wn`, probes/fw2.cpp): 213v215 and the tex term also reassociated; not the lever.
- Sibling bodies: not needed (the row has no callees; the matching form needs no TU context).

## Test (test/00ac7eb0.cpp)
Links the REAL row object over raw native-layout storage (every offset cited to the instruction that reads it); a double-precision
reference model (13 targeted cases incl. dot exactly 0, tiny +-dot, zero penalty, a vertex with itself, tex-only / normal-only
terms that separate squared from unsquared; 4000 random); checks Cost, Weight2, ResultingPosition, and that nothing else in the
contraction / primitive / vertices is written. Mutation check: 19 one-decision mutants (sub-contraction index, both vertex indices,
both difference operands, each in-place square removed, each weight swapped, `+=` -> `=` / `-=`, `< 0` -> `<= 0` / `> 0`,
penalty source and `+=`, Cost source, ResultingPosition vertex, Weight2 value, Dot receiver): 19 KILLED, 0 SURVIVED.

## NAME_FIXES
None.
