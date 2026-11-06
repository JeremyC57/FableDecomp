# Chunk 793 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 folds `x - 0.0f` / `x + 0.0f` inside an inlined vector expression but never `0.0f * k`. Spell an XY side vector as `C3DVector(dir.Y * k, -dir.X * k, 0.0f)`, not `C3DVector(dir.Y, -dir.X, 0.0f) * k` (0x00715A70: retail's camera Z is from.Z untouched).
- A branch that must skip a shared normalise tail: a `goto` past it (or a bool flag VC7.1 threads away) gives retail's direct jump. Per-branch normalise copies are not tail-merged by VC7.1 (0x00715A70, 1300n vs 1374n).
- Several vectors built from one half extent: separate named C3DVector locals made from v0's components reproduce retail's per-vector store/reload (v1.X copied from v0.X through eax). An aggregate-initialised array lets VC7.1 share products across the vectors (0x00ADE080 x87 arm).
- A near-plane clip reaches retail's in-place fadd/fstp into the point with `p += delta * ((near - p.Z) / delta.Z)`. `p = p + delta * t` goes through a temporary (0x00BC8A60).
- Two FP compares feeding nested ifs: keep them as conditions. Bool locals make VC7.1 materialise setcc bytes, which retail never does (0x00BC8A60).
- PDB bitfields (CStreamingGlyphData Advance:8 / BigChar:1, CStreamingGlyph AdjustmentIndex:8, CStreamingGlyphLookup cachePos:12) compile to the same shr/and/js as hand masks. Use the PDB structs (0x009A25D0).
- Width/x1 then height/y1 statement order matches retail's per-axis fild/fst/fmul/fadd interleave. Converting both first does not (0x009A25D0, 288n -> 280n).
- Float constants at 0x0122xxxx/0x0126xxxx (2.0, 0.4, 0.25, 0.5 at 0x0122F59C ...) are the shared literal pool. Read the value from Fable.exe and write the literal rather than inventing an extern name.
- CEngineCamera's NoScaleViewTransform +0x114, InverseNoScaleViewTransform +0x144 and CameraDesc.NearZPlane +0x6C have their Ego_r offsets in retail. CEnginePrimitiveRendererSharedData PixelShaderDiffuseOnly +0xDC / PixelShaderDiffuse +0xF4 do too. CEngineResourceManager does not (MainGraphicBank retail +0x954, Ego_r +0xA8C).
- The 0x00ADE080 row extent holds a second function: C3DBoundingBox::InitialiseFromSphere 0x00ADEB30..0x00ADEBBB (ret 4). Check the first `ret` against the extent before trusting a row size.
- The bdiff normaliser prints a symbol+4 relocation (a static C2DVector's .Y) as `[4]`, which costs 2 difflines that are not real code differences.
- 0x00A58720 is `(anonymous namespace)::CTransformWorldCoordsToPlaneCoords::operator()` (Ego_r ctor + std::transform instantiation), not CProjectOntoPlane. Data globals are absent from the PDB name sets, but class statics and anonymous-namespace objects with dynamic initializers (GFontTextureCache, CStreamingFontBank::AlphaMaskUVMul/Add) do appear as `dynamic initializer for '...'`.
