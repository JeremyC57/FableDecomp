# Chunk 793 -- notes from the authoring agent

No row matched. All six rows are parked in differ/ with their best source and a behaviour test. Line 2 of each source gives
the grade and the residue; line 3 gives the next idea. Every attempt is in ATTEMPTS.log and in STATUS.tsv. hdr/README.md lists the
four overlay headers. All overlay edits are opt-in, and the 11 landed includers recheck RELOCATION_MATCH PASS.

Names: (P) = proven from two sources (PDB name set or layout, plus bytes or callers). (D) = descriptive.

## 0x00A58B50 NBBBMaths::CSphere::GetFirstIntersectionWithTriangle (P) -- DIFFER(2211v2207), 155 difflines
- Shape:
  - normal = (t1-t0).Cross(t2-t0) (argument order matches retail's edge order). Return false unless |normal|^2 > DELTA^2.
  - AccurateNormalise. Signed plane distance; return false unless DELTA < |d| < Radius.
  - Circle radius^2 clamped to [0, Radius^2]. Plane basis axis_x/axis_y.
  - std::transform the triangle into plane coordinates with CTransformWorldCoordsToPlaneCoords.
  - Per edge, solve the circle-line quadratic and push_back hits with lambda in (-DELTA, 1+DELTA).
  - Hit, else a vertex inside the circle, else IsPointInsideTriangleInThePlane(points, 0,0). Each of the three exits writes
    intersection_pos = centre + axis_x*X + axis_y*Y and an AccurateNormalised outward normal.
- Key spellings:
  - push_back is a same-TU out-of-line body (`FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE`, vector<C2DVector> 0x004428B0).
  - coef_a is spelled inline XY.
  - push_back(start + C2DVector(l*X, l*Y)).
  - /Gr.
- Residue:
  - x87 operand order inside coef_b/coef_c. About 50 spellings were tried and all canonicalise to the same code.
  - 4 bytes of slot layout. Float-copy locals fix the operand order but move the slots (sph_a5/a6).
- Callees (P):
  - 0x00A5A9D0 = std::transform<const C3DVector*, back_insert_iterator<vector<C2DVector>>, (anonymous namespace)::CTransformWorldCoordsToPlaneCoords>;
  - 0x00A58A60 NBBBMaths::IsPointInsideTriangleInThePlane (bound);
  - DELTA_NOTIONAL_ZERO 0x0129BA3C (bound).
- Label fixes: 0x00A58720 is the functor's out-of-line operator(), not CProjectOntoPlane. 0x00A5A9D0 is not
  Vector_InsertMany_PairFloat. Both are in NAME_FIXES.tsv.
- Header debt: the source declares `std::back_insert_iterator` / `back_inserter` / `transform` locally. They belong in
  stlport/fable_containers.h. The new hdr/engine/NBBBMaths_CSphere.h goes into the repo.

## 0x00A593F0 NBBBMaths::CCylinder::GetFirstIntersectionWithTriangle (P) -- DIFFER(3043v3027), 540 difflines
- Shape: the sphere row's skeleton on an ellipse:
  - minor/major radius^2 are scaled by the cylinder axis tilt (the fabs(cos) test).
  - Everything after that test sits in one `if`, with a single trailing `return false` (cyl_a3).
- Key spelling: coef_b = 2*(line.Y*(line_start.Y*major_sq) + line.X*(line_start.X*minor_sq)). This was the only spelling of the
  t1 sweep that moved VC7.1.
- Residue:
  - The first ~230 instructions match. After that, VC7.1 hoists points._M_start out of the edge loop (retail reloads it).
  - Register colouring.
- Repo header engine/NBBBMaths_CCylinder.h is used unchanged. Callees as for the sphere row.

## 0x00ADE080 C3DBoundingBox::InitialiseFromOBB (P) -- DIFFER(2696v2735), 1280 difflines
- **Split the row.** The extent 0x00ADE080..0x00ADEBBB holds two functions: InitialiseFromOBB ends at 0x00ADEB2C (`ret 8`), and
  0x00ADEB30..0x00ADEBBB (`ret 4`) is `C3DBoundingBox::InitialiseFromSphere(const C3DBoundingSphere&)` (P: the PDB name, plus
  Min = Center - Radius and Max = Center + Radius with Radius at +0xC = PDB C3DBoundingSphere). Our local oracle was cut to 2735.
- Shape: `if (g_SSEAvailable)`:
  - aligned C4DVector centre (w 0) and 4 corners (+-hx, +-hy, hz, 0);
  - an aligned CMatrix4x4 filled transposed from the 3x4;
  - TransformTransposedAssumeW(centre, w 1), then AlignedTransformTransposedAssumeW(corners, 4, w 0) at 0x00A5CB90 (P);
  - a movaps/addps/subps/minps/maxps kernel over centre +- corner into Min/Max.
- Else x87:
  - four named half-extent vectors, each TransformVector'd in place (named locals reproduce retail's per-vector store/reload);
  - the centre TransformPoint'd;
  - Max = centre + v0, Min = Max;
  - seven unrolled LimitUpper/LimitLower pairs.
- Residue / ASM_RULES:
  - The SSE kernel carries the inline-asm signature: a homed and reloaded pointer local, 0x00ADE329 `lea eax,[esp+0B0h]`,
    0x00ADE330 `mov [esp+24h],eax`, 0x00ADE334 `mov eax,[esp+24h]`. Intrinsics give a different frame and schedule.
  - Budget used so far: 4 intrinsic + 13 plain spellings. Then (owner rule) a shared helper header with a macro-wrapped `__asm`,
    a FABLE_PORTABLE twin and a dual-path test.
  - The x87 arm's slots follow the SSE arm's frame. VC7.1 hoists the obb load above the SSE test and canonicalises the
    GetCenter sum.

## 0x00BC8A60 CEnginePrimitiveManagerLines::RenderLine (P) -- DIFFER(3323v3416), 1006 difflines
- Shape:
  - ResetWorldTransform, then a CShaderEnableSaver(true, true) scope (D: the class name is ours; it saves and forces the
    vertex/pixel shader enables and restores them on every exit).
  - Start/End go to camera space with NoScaleViewTransform. Near-plane clip in place (`p += delta * ((near - p.Z)/delta.Z)`).
    Project.
  - Screen dir; return unless dir^2 > DELTA^2; C2DVector::Normalise 0x00A14540; times Width*0.5; perp.
  - Four corners go back through InverseNoScaleViewTransform (SUMS form: Y/Z rows left to right).
  - Render states: blend switch 3 / 4 / default.
  - SetPSConstant(2, brightness/255 x3, alpha/255).
  - ApplyPixelShader(BankIndex ? PixelShaderDiffuse : PixelShaderDiffuseOnly), vertex colours/UVs, ApplyVertexShader(this+0xC),
    PrepareForRender(1).
  - Frame texture fetch with CFrameData::BeginLoading 0x009FDE80 (P).
  - DrawAsIndexedTris(..., 0x18, 4, quad indices, 2).
- Names respelled to bindings/PDB this session:
  - g_pResourceManager 0x01436E98 (MainGraphicBank at retail +0x954, Ego_r +0xA8C);
  - g_pEngineCamera_01436ea0 (NoScaleViewTransform +0x114, InverseNoScaleViewTransform +0x144, CameraDesc.NearZPlane +0x6C:
    Ego_r offsets hold in retail);
  - g_pPrimitiveRendererSharedData 0x01436E78 (PixelShaderDiffuseOnly +0xDC, PixelShaderDiffuse +0xF4);
  - DELTA_NOTIONAL_ZERO 0x0129BA3C.
- Still descriptive (D):
  - g_LineQuadIndices_0139bac8;
  - CShaderEnableSaver;
  - the local view structs (CRenderManagerCoreView, CTrackedRenderState m_state28b8/291c/2930, CShaderRenderManager
    PSConstants +0x3CC).
- Residue:
  - VC7.1 sinks the both-points-behind early return (the inline saver destructor) to the function end; retail keeps it
    fall-through at 0x00BC8C1B.
  - `this` stays in ebp (retail homes it at [esp+60h]).
  - Slots, and the CRenderStateManager bookmark flip (chunk 792: whole-function).
- Header debt: replace the local views with engine/CEngineCamera.h, engine/CEnginePrimitiveRendererSharedData.h,
  engine/CEngineResourceManager.h (retail re-offset: MainGraphicBank +0x954) and engine/CShaderRenderManager.h (SetPSConstant,
  PSConstants +0x3CC). Add the SUMS TransformPoint form as a CMatrix3x4 opt-in for the corners.

## 0x009A25D0 CStreamingFontBank::Render (P) -- DIFFER(3715v3722), 282 difflines
- Shape:
  - Preload(text, block-on-load). The flag is read at `*(GFGetSystemManager()+0x84)+0x38`; the member names are (D).
  - Update the texture cache if dirty. Optional centring (flags & 8).
  - Vertex format 0x144/0x244. Build the quad index list once.
  - AttachTextureToStage 0/1. Seven stage-1/2 texture states when a second texture is used without pixel shaders.
  - Per glyph:
    - GlyphLookup 0x009A1140 (P);
    - adjustment = glyphTextureCacheGlyphAdjustment[indices.AdjustmentIndex];
    - cache cell cachePos (P bitfields);
    - BigChar picks the second cache layout;
    - quads of CFontVertex / CFontVertexTextured built as temporaries through a `const C4DVector&` ctor (rep movsd);
    - alpha-mask UVs = screen * AlphaMaskUVMul + AlphaMaskUVAdd.
  - Flush every 0xFC vertices.
- Names this session (P):
  - CStreamingGlyphLookup / CStreamingGlyph / CStreamingGlyphData / CStreamingGlyphAdjustment from the PDB layouts;
  - (anonymous namespace)::GFontTextureCache 0x013C5A80 (PDB dynamic initializer);
  - glyphTextureCacheGlyphAdjustment 0x013BDA40 (landed 0x009A3640 spelling);
  - CStreamingFontBank::AlphaMaskUVMul / AlphaMaskUVAdd 0x013CA2F8 / 0x013CA300 (PDB dynamic initializers; NAME_FIXES.tsv).
- Still descriptive (D):
  - g_FontTextureCacheDirty 0x013C5A70;
  - g_FontQuadIndicesDirty 0x0138E0B5;
  - g_FontQuadIndices 0x013CA308;
  - g_FontCacheLayout / g_FontCacheLayoutSecond 0x013C5A98 / 0x013C7AC8 with CFontCacheLayout {CellsPerRow, Rows};
  - CFontManagerView, CShaderEnableView, CFontVertex*.
- Residue: slot order of the line/cursor floats, eax/edx colouring around the glyph byte loads, the bookmark flip. The frame
  (0x25F0) matches.
- Header debt: engine/CStreamingGlyphLookup.h + CStreamingGlyph.h (PDB layouts above), and the CSystemManager +0x84 member
  (font manager, BlockOnLoad +0x38).

## 0x00715A70 NTCScriptedControl::CActionTalkBase::SetUpCamera (P, private) -- DIFFER(2860v3356), 1269 difflines
- Shape:
  - FarApart if |other - me| > 2.
  - GetCameraSetupPositions 0x00712340 (P). Store CameraFromPos/CameraAtPos/CameraSetUp.
  - dir:
    - at - from (things differ);
    - -facing for the talking-to-self case (vtable slot 70, no normalise: `goto` past the shared normalise reproduces the
      retail jump);
    - me + other;
    - else from - at.
    - Then GFFastOneOverSqrt normalise.
  - zoom = clamp((|from-at| - 0.4) * (1/2.6), 0, 1). offset = (0, 0, -zoom^2).
  - Focus switch 1..7: scales 0.1 / 0.25 / 0.33 / 0.5, height +-0.75.
  - Height correction. at += offset.
  - Position switch 0..12 (11 forced when ParentIsDemonDoor): side = (dir.Y*k, -dir.X*k, 0) with k 0.4 or 2.6, back steps
    0.5/2.0, torso offset (0, 0, -0.7).
  - Creature head-height pass over GetAllThingsInRoughArea 0x007197C0 (box +-2).
  - CScriptModuleGoToLookAt or CScriptModuleMoveBetweenLookFromAndLookTo 0x008525F0, with frames = GFFloatToLongNear(FPS * 15 or 0.8).
  - Transition 1 / 0.2. SetGameScriptedCameraMode 0x00697B70.
- The constants are now retail's literal values (read from Fable.exe; they sit in the shared literal pool). No invented globals
  remain except g_CameraTorsoOffset 0x01380C38 (D, a C3DVector (0, 0, -0.7) in .data).
- Still descriptive (D):
  - CTCPhysicsBase::Radius +0x34;
  - CMainGameComponent +0x24 / +0x20 (the search tools);
  - CScriptModuleBase TransitionType +0x44 / TransitionTime +0x48;
  - CActionTalkBase FarApart +0x6D, CameraFromPos +0x74, CameraAtPos +0x80, CameraSetUp +0x8C (the retail re-offset from chunk 792).
- Residue (ours is 496 bytes short):
  - VC7.1 cross-jumps focus cases 1..4 into one tail and keeps offset.Z in st. Retail keeps a cached 0.0 in st0 across both
    switches and writes every case out in full.
  - Ours hoists from.Z above the position jump table. Retail reloads it in each case.
- Header debt: the retail CActionTalkBase layout (Ego_r 16-byte CArrays become 12-byte STLport vectors). Chunk 792's NOTES
  have the re-offset.
