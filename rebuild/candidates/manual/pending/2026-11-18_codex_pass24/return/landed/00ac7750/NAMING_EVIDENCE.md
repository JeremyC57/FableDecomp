# 0x00AC7750 naming and relocation audit

Ego_r 0x00C60150 and FableWin 0x034C9810 identify the private non-const original leaf. Ego_d puts the method in the quadric optimiser module and gives `midpoint`, `distance`, `inverse`, `vect_part`, `optimal`, `error_optimal` and `error_midpoint`. The compiled object emits `?ComputeOptimalVertexPlacement@COptimisedPrimitive@QuadricOptimiserInternals@@AAE?AVC3DVector@@ABVCQuadric@2@ABV3@1@Z`.

The seven operands below resolve to their recorded targets. Applying them reproduces all 495 retail bytes.

| Offsets | Retail address | Exact symbol or payload |
|---|---|---|
| +0x02A, +0x034, +0x042 | 0x0122F59C | `__real@3f000000`, verified 0.5f |
| +0x09F | 0x0129BA3C | `?DELTA_NOTIONAL_ZERO@@3MB` from ESTABLISHED_SPELLINGS.tsv |
| +0x0BA | 0x00AC70F0 | `?GetInverseMatrixPart@CQuadric@QuadricOptimiserInternals@@QBE?AVCMatrix3x4@@XZ` |
| +0x13B, +0x149 | 0x00AC6D50 | `?EvaluateError@CQuadric@QuadricOptimiserInternals@@QBENABVC3DVector@@@Z` from reloc_bindings.tsv |

Both PDB donors name `GetInverseMatrixPart` (Ego_r 0x00C5F9C0, FableWin 0x034BF810), and the Ego_d module lists it in the same quadric family. At 0x00AC7809 the retail call passes a hidden CMatrix3x4 result, whose three rows are used as an inverse. The corrected retail label is in labels_chunk266_corrections_2026-10-21.tsv. Its out-of-line body remains separately parked. The helper definition supplied here lets the compiler see the original same-module call context.

Ego_d gives one `double det` local inside GetDeterminant. Naming the completed sum produces retail's two commutative determinant-product operand orders with the same matrix and vector types.

The same-TU helper bodies were checked independently in the final object. EvaluateError at 0x00AC6D50 reproduces its 104 retail bytes exactly. GetInverseMatrixPart at 0x00AC70F0 measures 231 against the 237-byte native extent, with eight instruction hunks in its cofactor expansion, mirrored writes and return. The 495-byte equivalence covers the reserved caller.
