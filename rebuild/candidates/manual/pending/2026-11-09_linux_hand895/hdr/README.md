# chunk 895 header overlay

One header changed, behind a new opt-in; the default path is byte-identical (the block is only inside `#ifdef`).

| File | Opt-in | What | Evidence | Rows |
|---|---|---|---|---|
| `engine/CMatrix3x4.h` | `FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY` | `void SetTranslateOnly(const C3DVector& translation) { E41=translation.X; E42=translation.Y; E43=translation.Z; }` (inline member, placed right after the default constructor) | Ego_r.pdb / FableWin.pdb `?SetTranslateOnly@CMatrix3x4@@QAEXABVC3DVector@@@Z`; no out-of-line copy in retail: 0x00BEBAA0 stores the translation row as three dword copies (0x00BEBB89..0x00BEBB97) | 0x00BEBAA0 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix |

Grade 0x00BEBAA0 with `FABLE_HDR_OVERLAY=$A/hdr` exported (gall.sh was run that way).
