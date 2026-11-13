# agentC6 NOTES (w22 acctC lane; continues agentC3 / agentC2). Tools: tools/ (C3's plus gen.py, gen2.py, scan.sh, nat2.py, n2.sh, mkh.py).

## 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive -- RELOCATION_MATCH (4540v4540), test PASS, vland clean, addmatch done
Source src/00bb3e80.cpp = C3's best text with the repo DEFAULT SetRenderState / AddStateToUpdateList (no DESIRED / BOOKMARK / DIRTY)
+ `#define FABLE_RETAIL_ASSERT_DEAD_ARM` + `#include "fable_assert.h"` (NEW shared header in hdr/) + nine FABLE_RETAIL_ASSERT lines:
pass < RENDER_PASS_COUNT, layer < OCCLUSION_LAYER_MAX, render_info, render_info->Lights, render_info->Mesh, material, primitive.MeshInfo,
graphic_bank, texture. No flags.
### Evidence v fit (for the owner)
- FableWin.exe itself is NOT on disk (only Fable.exe under $S/dl; no FableWin strings). What exists: FableWin.pdb names + per-function
  /Od sizes (signatures.tsv) and Ghidra decomp of a few FableWin functions (ghidra_out/audio_runtime_trace.c) that show Lionhead's
  debug ASSERT: `if (!(x)) { stop = GFGetSystemManager()->GetDebugManager()->DoErrorMessage(CCharString("x"), CCharString(""),
  CWideString(L"file.cpp"), line, 1); if (stop && IsDebuggerPresent()) int 3; }`. So assert SITES cannot be recovered byte-wise.
- Size evidence: FableWin row body 566 /Od instr v ours 440 (/EHsc) / 427 (plain) -> +126..139 = about TWO debug asserts (one debug
  assert costs ~63..83 /Od instructions: helper gaps GetOcclusionInfo +81, AllocateOccluderHandle +81, GetMaterial +83, StartCaptureBlock
  +166 = 2, SetRenderState +168 = 2, CArray::operator[] +178 = 2, FlushRender +204).
- Fit: with this macro the bookmark test/or order (O) needs 9..10 dead arms with plain conditions (8 -> O9, 11 -> O7). 2 natural asserts
  do not reach it; no single plausible assert form weighs ~4x more. => the COUNT (9) is FITTED; the conditions are plausible pointer /
  range checks of what the function dereferences, not recovered.
- Measured on the way (probe/a/*, ATTEMPTS.log): (1) dead-arm weight depends on the arm's IL: `Fail(#c,__FILE__,__LINE__)` ~4.3 units
  (27..31 needed), FableWin-shaped arm (error call && IsDebuggerPresent -> break) ~11 units (9..10), the chain GetSM()->GetDM()->Do..()
  ~15 (8..9); conditions with inline calls weigh more AND move the load order L (e.g. primitive.MeshEffect.Get() in the 5 outline arms,
  PeekParentBase, GetParticleProgram) -- so asserts that call accessors must be avoided or balanced; (2) class-typed temporaries in a dead
  arm (the literal debug form with CCharString / CWideString) change LIVE codegen (zero-register reuse, 4532 bytes) -> the release form
  cannot have had them; (3) dead arms inside the inline SetRenderState / AddStateToUpdateList change the frame / inlining (4514 / 4704
  bytes) -> FableWin's SetRenderState asserts cannot be dead arms in the retail text (contradiction with a uniform release macro: report);
  StartCaptureBlock dead arms behave like row-level ones (O only); (4) helper-site variants (FlushRender x2 expansions, PeekGraphic) reach
  only O9 with plausible counts.
- C3's suspicion about the landed FITTED DESIRED/BOOKMARK/DIRTY opt-ins: consistent (they are per-expansion record adders), but dead
  arms placed IN SetRenderState are not equivalent here (they change code) -- no evidence either way for those rows; untouched.

### Row 1 follow-up (agentC6)
- Ported onto the SHARED engine/CEngineClothRenderer.h (new nested opt-in FABLE_RETAIL_CENGINECLOTHRENDERER_RENDER) and a new
  engine/C3DClothPrimitive.h (the rows had local class models -- shared-header rule); all overlay FABLE_STATIC_ASSERTs pinned to the repo
  line numbers with lane_merge/pin_asserts.py (341 pins, incl. C2's earlier unpinned moves). Still RELOCATION_MATCH; vland clean.

## 0x00BB30A0 CEngineClothRenderer::Render2DClothPrimitive -- IN PROGRESS, DIFFER(3538v3538), 2 real sites
Source src/00bb30a0.cpp (written fresh on the row-1 TU text; NOT the stale draft). Structure from retail + FableWin.pdb's COMDAT order (the
inline helpers listed right after Render2DClothPrimitive in FableWin are new in this row): SetupAlphaBlendingStatesFromRenderFlags,
GetCurrent2DAlphaMaskInfo (via GFDebugPDynamicCast -> CEnginePrimitive2DViewportManager::GetCurrentAlphaMask), Set2DAlphaMaskTextureMode /
DisableTextureFiltering (fwsize SAME 24i / 22i), SetUserVertexShaderConstant(0, C3DVector(factors.XYZ), factors.W), ApplyTriVertexShader(0,..),
GetPixelShader2DDiffuse(mode) (+0x1AC), SetTextureFactor, GetDiffuseTexture(mipmap 1) / Get2DAlphaMaskTexture, fog off, RenderPolygons.
New overlay opt-ins: RenderUtil _CLOTH2D, SharedData _CLOTH2D (2D pixel shaders), MeshBase _2DALPHAMASK; viewport manager retail branch.
Fixed: forceinline helpers (2954 -> 3544 bytes), frame 0x44 -> 0x34 by scopes: textures in a block after the morph info, preloading in an inner
block that ends after the preloading bail-out (two_sided then shares preloading's cloth-param slot, lights gets primitive's param slot).
RESIDUE (record-keyed, same class as row 1 but opposite sign): O at expansion 0x00BB33B7 (retail test eax,esi = 'a'; ours 'b') and L at the
fog expansion 0x00BB3D28 (retail mask first). Late or early rulers (`const ERenderPassType ruler_K = pass;`) show: L fog=M at +7..+15, O loses
another 'a' at ~+26: the row text has ~20-24 units TOO MANY (need fewer records; dead asserts only add). tools/meter.sh measures a variant's
unit shift (baseline 7). Measured 0-unit changes: GFDebugPDynamicCast vs static_cast, GetPixelShader2DDiffuse forms, GetRGBColour named bytes,
named shader refs, C3DVector vs ToC3DVector (-1), scope blocks; -2: Set2DAlphaMaskTextureMode/DisableTextureFiltering written inline.
GetRGBColour WITHOUT a named CRGBColour result gives O right but wrong colour code (3522 bytes); the named colour is needed for code.
Removing one FlushRender expansion (diagnostic) moved O by +2 and L by -3: O and L are NOT one axis.
Best source: differ/00bb30a0.cpp (the probe tools expect it at src/00bb30a0.cpp: copy it back before using meter.sh / rul.sh / s2.sh).
More measured (all no change of either order): SetRenderState DESIRED / BOOKMARK / DIRTY opt-ins (EARLY_RETURN does not compile with this
text: cl spins), ApplyTriVertexShader texts (one index local, clamp-in-place, shader pointer), a named CRGBColour tint, a named CRGBFloatColour,
C4DVector(const CRGBFloatColour&) in SetTextureFactor (FABLE_RETAIL_C4DVECTOR), static_cast instead of GFDebugPDynamicCast.
Order-fixing but code-breaking (diagnostic only): SetTextureFactor(CRGBFloatColour(r/255,...)) without the CRGBColour temporary (O right,
colour loads differ: retail copies the four bytes into a CRGBColour first), GetRGBColour without a named result, deleting the
ApplyTriVertexShader call (O right, L meter 11), deleting the bail-out FlushRender.
NEXT: (1) find the ~20 surplus record units: candidates not yet metered -- CScopedShadersEnable / capture-block texts (shared with row 1),
the FlushRender loop text (2 expansions; FableWin FlushRender 252i carries asserts), GetMaterial / PeekTextureBank / PeekRenderInfo accessors,
the textures-array / preloading forms; (2) every shared-helper text change must be re-checked on row 1 (its 9-assert count is fitted and can be
re-fitted: window was 9..10); (3) no behaviour test written yet for this row (write one from test/00bb3e80.cpp's model when it matches).

## 0x00B783F0 CEngineWaterRenderer::Render -- UNTOUCHED (context ran out on rows 1-2).
