# p989 batch b3 -- option (a) for 0x00989230 (separate from b2; lands only after b2)

**Advisor decision (owner delegated)**, per the coordinator ruling: a row-named nested macro inside the shared x87 arm, defined only by this
row, labelled `// Inferred:`. Prepared as the last step after the row-text axis came back empty; Account A's FableWin callee list for
0x030381B0 / SetVSConstants had not arrived. The coordinator decides at landing whether that answer supersedes this batch.

## Contents
| path | lands as |
|---|---|
| hdr/engine/CMatrix4x4AlignedSSE.h | b2's header + nested opt-in `FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD_RHSREF_00989230` inside `..._EGOD` (+ #error pin) |
| hdr/engine/CMatrix4x4.h | identical to b2's |
| src/CShaderRenderManager_UpdateObjectSpaceTextureTransform_00989230.cpp | NEW row (hand3c / ref_00989230 parked) |
| test/..._00989230_test.cpp + .mutants.py | rebuild/tests/00/98/ (base PASS, 9/9 mutants FAIL) |
| payload.json, oracle.tsv | tools/mkpayload_b2.py |

## Proof that the five confirmed users do not change
Under b3/hdr (tools/regress.sh): the b2 rows 0x00ADEFC0, 0x00AE1320, 0x00AE29D0, 0x00A9E1E0, 0x00BD2D90 and the landed expanders
0x00A5CB90 (/Zi), 0x00A9D480, 0x00A9D580, 0x00A9D750, 0x00AA0FA0, 0x00AA2110, 0x00A9E1E0, 0x00BD2D90: all RELOCATION_MATCH. No other file
defines the row macro, so their preprocessed text is b2's.
Row: official grade 20:56 RELOCATION_MATCH (HO = b3/hdr).

## Ego_d evidence and the Inferred line
- Row locals: Ego_d.pdb record (debug length 262): this; { c_transform { range (const CShaderConstantRange&) { transform (CMatrix4x4),
  rows (unsigned long) } } } -- the row text is exactly that.
- Inferred: `const CMatrix4x4& r = rhs;` in the AlignedSetMultiply x87 arm, row-only. Not an Ego_d local of AlignedSetMultiply. Stands in for one
  symbol the donor compiles that no evidence names (one extra live statement anywhere gives retail; the reference inside the inlinee is the
  zero-byte way to get it; the same reference in the row measures 0).

## Unit table (mod-4 x87 column phase)
| construct | units |
|---|---|
| live `if (x & K) x |= M;` | +1 |
| bare store | +2 |
| two flag statements written as one | +3 |
| Ego_d body vs ACCESSORS | +1 |
| named reference inside the inlinee | +1 |
| named reference in the row | 0 |
| std::min(range.Count, 4ul) | +1, but +10 bytes |
| `count` local before the clamp | phase moves, +2 bytes |
| caller locals (0x00BD2D90 render_info / bone_transform) | move the columns (so row text CAN matter; none found here) |

## Candidates tried in the row (all byte-neutral ones leave the phase; probes rv, rv2, rv3, rv4)
HRESULT hr in SetVSConstants (3 forms); SetVSConstants before / after / inline / non-inline; count tests (!= 0, > 0); loop cast / no cast;
post-increment; clamp spellings (<, <=, >, >=, 4 <, 4 >, if, if-else, default 4); data argument (&t(0,0), t.Data, &t.Data[0][0], cast);
direct AlignedSetMultiply; named texture reference; flags swapped. Byte-changing: flags before the loop (643), rows before transform (640),
flags as one statement (636 but +3). Ruled out by evidence: SetVSMatrixConstantTransposed(4x4) (retail 0x00988840 transposes into a local),
GetD3DDevice (340-byte debug function, not an accessor), PeekVertexShaderConstantLayout (returns an index), enum flag names (not visible in
this config; constants add no symbols).

Lane limits as in b2: fatal `#error` hangs VC7.1 under Wine (pins checked by inspection); no 32-bit libc for the gxx -m32 steps.
