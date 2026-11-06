# Chunk 793 header overlay

Four headers. Three are opt-in additions to repo headers. The fourth is new, so nothing includes it yet. No default path
changed. The new macros are defined nowhere else in the repo, so existing includers compile the same bytes. The 11 landed
direct includers of the three edited headers were rechecked read-only with this overlay (`vland.py --recheck-landed`):
all 11 are RELOCATION_MATCH PASS. They are listed in ../SIBLINGS.tsv.

| Header | Change | Opt-in macro | Evidence |
|---|---|---|---|
| engine/NBBBMaths_CSphere.h | **New** class `NBBBMaths::CSphere`: `float Radius` +0, `C3DVector Base` +4 (16 bytes), the ctor and the two GetFirstIntersection* declarations | n/a (new file, no includers) | Ego_r.pdb layout (struct_layouts_egor.tsv: size 16, Radius +0, Base +4); Ego_r/FableWin `??0CSphere@NBBBMaths@@QAE@ABVC3DVector@@M@Z`; retail 0x00A58B50 reads Radius at [this+0] and Base at [this+4..0xC] |
| engine/C3DBoundingBox.h | adds the `InitialiseFromOBB(const C3DBoundingBox&, const CMatrix3x4&)` declaration | `FABLE_RETAIL_C3DBOUNDINGBOX_OBB` | Ego_r.pdb `?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z`; row 0x00ADE080 |
| engine/CMatrix4x4.h | adds `AlignedTransformTransposedAssumeW(const C4DVector*, C4DVector*, unsigned long, float) const` | `FABLE_RETAIL_CMATRIX4X4_ALIGNEDTRANSFORM` | Ego_r.pdb `?AlignedTransformTransposedAssumeW@CMatrix4x4@@QBEXPBVC4DVector@@PAV2@KM@Z`; out of line at 0x00A5CB90 (reloc_bindings has the same spelling from the landed caller 0x00ADF300); called by 0x00ADE080 with count 4, w 0 |
| engine/CStreamingFontBank.h | adds protected `GlyphLookup(unsigned short) const` and public static `C2DVector AlphaMaskUVMul`, `AlphaMaskUVAdd` (inside the existing retail class branch) | `FABLE_RETAIL_CSTREAMINGFONTBANK_GLYPHLOOKUP` (use it with FABLE_RETAIL_CSTREAMINGFONTBANK) | GlyphLookup: Ego_r.pdb `?GlyphLookup@CStreamingFontBank@@IBEPAUCStreamingGlyphLookup@@G@Z`, 0x009A1140 (chunk 792). The statics: Ego_r.pdb has ``dynamic initializer for 'CStreamingFontBank::AlphaMaskUVMul'`` and ``...AlphaMaskUVAdd'``. Retail 0x013CA2F8 is multiplied and 0x013CA300 is added in Render (0x009A25D0); both are written by SetAlphaMaskUVs 0x009A0EC0. **Access is an assumption** (public): the PDB name set does not record it |

Notes for landing:
- `CStreamingGlyphLookup` (16 bytes, PDB: `font:4`, `cachePos:12`, `ch`, `CStreamingGlyph indices`, `CStreamingGlyphData data`) and
  `CStreamingGlyph` (`GlyphIndex:24`, `AdjustmentIndex:8`) are defined in the Render differ source. No repo header has them yet. They
  belong in `engine/CStreamingGlyphLookup.h` / `engine/CStreamingGlyph.h` when the row lands (see NOTES.md, header debt).
- If AlphaMaskUVMul/Add are adopted, the landed row 0x009A0EC0 (SetAlphaMaskUVs) must be respelled. It uses
  `g_StreamingFontAlphaMaskUV0` / `g_StreamingFontAlphaMaskUV1` (NAME_FIXES.tsv).
