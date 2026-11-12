## 0x00B87870 CEngineScreenEffectDisplacementRenderer::ApplyDisplacement -- RELOCATION_MATCH (5 grades)
- Matched from the OUTLINE in one draft (RadialBlur template + shared headers); the only residue was the order of the two 0.0f home
  stores in the inline SetTextureBumpEnvMatrix: the body writes Mat00, Mat10, Mat01, Mat11 from the 1st..4th float params in order
  (column-major params m00, m10, m01, m11; names descriptive, PDB has no parameter names).
- Headers (hdr/README.md): FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_MEMBERS (typed members + private decl),
  FABLE_RETAIL_CRENDERSTATEMANAGER_DISPLACEMENT_INLINE (inline SetTextureBumpEnvMatrix, SetStencilOpFail/ZFail); also uses
  existing STENCIL_INLINE + OUTLINE_INLINE. No flags/pragma.
- Descriptive name: function-local `static bool UseStencil` (0x0139AB08, init true; no name found in PDBs/strings).
- No landed name proved wrong.
