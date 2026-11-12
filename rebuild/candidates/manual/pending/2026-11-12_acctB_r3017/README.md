# acctB hand-back, row 0x00BB1F50 CEngineClothRenderer::CalculateVertexNormals: matched (was parked at 420 / 425)

| address | result | notes |
|---|---|---|
| 0x00BB1F50 `?CalculateVertexNormals@CEngineClothRenderer@@AAEXABVCParticleSimulation@@ABVCParticleProgram@@@Z` | RELOCATION_MATCH 1508v1508, test PASS (g++ and VC7.1) | grade `retail_asm` (shared SSE helper, as parked). 47 grades this pass (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win, no warnings.

## What closed it

1. **Prologue choice at 0x00BB1FDB / 0x00BB1FED:** `PositionBuffer` read into its named local before `GetRenderTriangleCount()`.
2. **Triangle-arm scheduling at 0x00BB21CC..0x00BB221B:** the cross product with each component grouped,
   new opt-in `FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP`
   (`C3DVector((Y*o.Z-Z*o.Y),(Z*o.X-X*o.Z),(X*o.Y-Y*o.X))`), with subtraction left as the repo default. The
   triangle arm and the quad loop share the helper texts and this is the only grouping that fits both.
3. Corner indices through FableWin's inline `C3DTriangle2::GetA..GetC` / `C3DQuad2::GetA..GetD` (byte-neutral).

## Review items

- **FITTED:** the fetch order in 1 and the parentheses in 2 (a debug size cannot see parentheses). Support for 2: all
  six landed `operator^` users grade the same with it forced on (0x00A42170, 0x00E64210, 0x007BC6FA, 0x00BEBAA0,
  0x0063AB90, 0x00AC8C80), so it could become the default body and retire `CROSS_X_GROUP` / `CROSS_XY_GROUP`.
- **PROVEN:** `C4DVector::operator+=` in X,Y,Z,W order (MATCH against retail's uncalled copy 0x00A4B9B0; a W-before-Z
  body also matched the row but is contradicted by that copy); the corner accessors (FableWin COMDAT list, same size).
- **Asm evidence:** the per-site table chunk 2008 lacked now exists (`NOTES.md`; 31 plain C++ and 12 intrinsic
  spellings, none emits retail's SSE block). The `// Retail-asm:` line cites the helper's mnemonics with addresses.
- **Headers:** opt-ins only (`C3DVector.h`, `C3DTriangle2.h`, `C3DQuad2.h`, plus chunk 2008's overlay); re-base
  the copies on HEAD at landing. No name fixes.
