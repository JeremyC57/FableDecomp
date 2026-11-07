# Chunk 860 header overlay

All changes are additive `#ifdef FABLE_RETAIL_*` opt-ins, each with a `// Retail opt-in (chunk 8xx): ...` evidence comment.
No default-path text changed: `diff rebuild/include/engine/<f> hdr/engine/<f>` shows only added lines. No landed includer can
change bytes, so no includer recheck was needed. Not touched: engine/CBankFile.h, rebuild_abi.h. Every row is parked, so none of
these opt-ins has a landed user yet. Land them with the rows, or keep them for the next agent on these rows.

| Header | Opt-in macro | What it adds | Evidence | Used by |
|---|---|---|---|---|
| engine/CRenderStateManager.h | `FABLE_RETAIL_CRENDERSTATEMANAGER_STENCILOP_INLINE` (chunk 857, unchanged) | inline SetStencilOpPass / Fail / ZFail via SetRenderState | 0x009D91A4..0x009D92DC expands them; no out-of-line copy | 0x009D8CF0 |
| engine/C3DMeshFileBoneChunk.h | `FABLE_RETAIL_C3DMESHFILEBONECHUNK_GETBONE` (chunk 857, unchanged) | GetBone() (0x00AB9DD0) declaration, inline GetName() | 0x00A8B9E3 / 0x00A8BB04 | 0x00A8B990 |
| engine/C3DMeshFileSubMeshChunk.h | `FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK_FINDBONE` (chunk 857, unchanged) | FindFirstBone / FindNextBone (0x00ABA6A0 / 0x00ABA730) | calls at 0x00A8B9BC / 0x00A8BB63 | 0x00A8B990 |
| engine/C3DAnimationSequenceData.h | `FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_VECTORWORD` (new) | defines the nested C3DVectorWord {short X, Y, Z} after the class: default ctor, ctor from C3DVector (three truncations), operator== | Ego_r.pdb ??0C3DVectorWord@C3DAnimationSequenceData@@QAE@ABVC3DVector@@@Z / ??8...; 6-byte stride (the 0x2AAAAAAB division at 0x00A4F68D); __ftol2 x3 at 0x00A4F654..0x00A4F66C; compares X, Y, Z at 0x00A4F6B0..0x00A4F6CA | 0x00A4EFC0 |
| engine/C3DAnimFileSequenceChunk.h | `FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_MATRICES` (new) | `CArray<CMatrix3x4> Matrices` in place of the 12-byte placeholder, `friend class C3DAnimationSequenceData`; includes CArray.h / CMatrix3x4.h under the macro | Ego_r.pdb member type; 0x30 element stride divided at 0x00A4F054..0x00A4F06E; InitialiseFromFileData reads the chunk members directly | 0x00A4EFC0 |
| engine/CArray.h | `FABLE_RETAIL_CARRAY_GETBYTESIZE` (new) | `unsigned int GetByteSize() const { return size() * sizeof(T); }` | Ego_r.pdb ?GetByteSize@?$CArray@VCQuaternion@@@@QBEIXZ (and the unsigned char instance), inline in retail (0x00A4F56F..0x00A4F578 `sar ecx,4 ; shl ecx,4`) | 0x00A4EFC0 |

Removed before hand-over: an experimental CRGBColour.h opt-in (4-argument ctor through Initialise) that no best source used.
CShaderRenderManager.h was dropped from the overlay because it was identical to the repo copy.
