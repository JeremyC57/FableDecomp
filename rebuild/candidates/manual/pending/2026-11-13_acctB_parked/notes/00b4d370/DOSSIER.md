# DOSSIER 0x00B4D370 CEngineShadowRenderer::UpdateShadowSpace (7176 bytes) -- identity scout

$W/prior/ is EMPTY (no earlier scout/notes/grades for this row; nothing to reuse). No draft exists.

## 1. Retail shape (xdis / dis_all.txt)
- `__thiscall`, `ret 4` (0x00B4EF75): one stack arg (const CEngineCamera&), read once at 0x00B4EF5C `mov ecx,[esp+3E4h]` and passed to CalcClipVolume.
- NO frame pointer: `sub esp,3D0h` FIRST, then pushes interleaved with code: ebx (0x00B4D37B), ebp (0x00B4D38B), esi (0x00B4D38C),
  edi (0x00B4D397). Epilogue pop edi/esi/ebp/ebx; add esp,3D0h. So locals = 0x3D0, all esp-relative; arg at final [esp+3E4h].
- NO SEH (/EHsc state machine absent: no fs:0), NO /GS cookie, no calls to ctors/dtors: every object is POD / trivially destructible.
- /O2 speed shape (loops unrolled x2, `lea ecx,[ecx]` / `mov edi,edi` / `lea esp,[esp]` alignment pads at 0x00B4D8ED, 0x00B4DAFC, 0x00B4E4EE,
  0x00B4E97E, 0x00B4EDC8). Not optimize("s"). this kept in ebx throughout.
- Only 5 calls: 0x00A14440 C3DVector::AccurateNormalise (x2), 0x00B4B8B0 CalcLocalWorldToBaseShadowSpace, 0x00A14510
  C2DVector::AccurateNormalise, 0x00B4BA60 BuildDerivedShadowMatriciesFromWorldToShadowMatrix, 0x00B4BCC0 CalcClipVolume. Everything else
  (matrix inverse, PostMultiply x3, PostScale x3, PostTranslate x3, PerspectiveTransformPoint x8 sites, SetIdentity, GFLimit, TransformPoint)
  is expanded INLINE.
- Constants (read from $FABLE_EXE): 0x0122DED8 1.0f, 0x0122DEDC 0.0f, 0x0122DEE0 -1.0f, 0x0122F59C 0.5f, 0x0123078C 0.25f,
  0x012A22A8 1e30f, 0x0129DCA4 -1e30f, 0x012A23B8 **double** 0.9 (fmul qword, 0x00B4E7EF); immediates 0x44000000 = 512.0f (push),
  0xF149F2CA = -1e30f, 0x7149F2CA = 1e30f, 0x80000000 = -0.0f, 0x3F800000 = 1.0f.
- Global: 0x01436E9C g_pEngineLightingManager (labels_*.tsv spelling "g_pEngineLightingManager"; CEngineLightingManager*), read +0x30..+0x38
  = DiffuseVector (Ego_r CEngineLightingManager +48 C3DVector DiffuseVector).

## 2. Identity (two+ sources) -- manifest label is RIGHT
- Ego_r.pdb 0x009F0370 and FableWin.pdb 0x02D74FB0 (debug body 4922 b / 1202 insns) both:
  `?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z` -> **private: void CEngineShadowRenderer::UpdateShadowSpace(const CEngineCamera&)**.
  Mangled leaf for `// Leaf:`: `?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z`.
- Not virtual (no vtable slot; AAE). Only caller: 0x00B4F418 tail `jmp` in UpdateShadowScene 0x00B4F390 (python3 $S/w23/xref.py).
- NAME_FIX: landed caller rebuild/src/compiled/00/b4/CEngineShadowRenderer_UpdateShadowScene_00b4f390.cpp declares
  `void UpdateShadowSpace(CEngineCamera* camera)` (and UpdateShadowScene(CEngineCamera*, bool, ...*)); PDB is `const CEngineCamera&`
  (and UpdateShadowScene `(const CEngineCamera&, bool, const CEngineLightingManagerSpotlightParams&)`). Same ABI; spelling wrong.
- Source-file neighbourhood (same TU CEngineShadowRenderer.cpp; Ego_r order ...DrawFrustrumDebugScreen 0x9EF770, UpdateShadowSpace 0x9F0370,
  UpdateShadowScene 0x9F2070): retail order 0x00B4B8B0 CalcLocalWorldToBaseShadowSpace, 0x00B4BA60 BuildDerived..., 0x00B4BCC0
  CalcClipVolume, 0x00B4C550 CalcSpotLightClipVolume, 0x00B4CE20 SetShadowClipVolumesToNeverShadow, 0x00B4CED0 CShadowClipVolume::ClipBoundingBox,
  0x00B4D210 dtor, 0x00B4D2C0 PostLayerRender, **0x00B4D370 UpdateShadowSpace**, 0x00B4EF80 UpdateSpotLightShadowSpace, 0x00B4F390
  UpdateShadowScene, ... 0x00B4FC40 SetupShadowFuncRenderStates, 0x00B50370 ctor, 0x00B507E0 CMatrix4x4::PostMultiply (out-of-line COMDAT).
- DrawFrustrumDebugScreen (FableWin 0x02D78080, Ego_r 0x009EF770) is NOT called in retail (EnableFrustrumDebugDisplay +0x2AE never read);
  presumably compiled out in the final build. Do not call it.

## 3. Callees
| retail | PDB name (Ego_r + FableWin) | landed row | note |
|---|---|---|---|
| 0x00A14440 | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` C3DVector& () | rebuild/src/compiled/00/a1/C3DVector_AccurateNormalise_00a14440.cpp | out of line; returns *this |
| 0x00A14510 | `?AccurateNormalise@C2DVector@@QAEAAV1@XZ` C2DVector& () | rebuild/src/compiled/00/a1/C2DVector_AccurateNormalise_00a14510.cpp | manifest says `float` return -- wrong, PDB returns C2DVector& |
| 0x00B4B8B0 | `?CalcLocalWorldToBaseShadowSpace@CEngineShadowRenderer@@AAE?AVCMatrix3x4@@ABVC3DVector@@MM@Z` private CMatrix3x4 (const C3DVector&, float, float) | NOT landed | hidden return ptr = &local at [esp+100h]; pushes 512.0f, MaxShadowDistance*2, &lightVector |
| 0x00B4BA60 | `?BuildDerivedShadowMatriciesFromWorldToShadowMatrix@CEngineShadowRenderer@@AAEXXZ` private void () | ..._00b4ba60.cpp (00/b4) | landed with LOCAL CMatrix4x4 helpers named `Scale`/`Translate`: PDB names are CMatrix4x4::PostScale / PostTranslate (NAME_FIX for those local spellings) |
| 0x00B4BCC0 | `?CalcClipVolume@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z` private void (const CEngineCamera&) | NOT landed | last call, then return |
Inline (no call) accessor: FableWin `?PeekDiffuseVector@CEngineLightingManager@@QBEABVC3DVector@@XZ` 0x02D767C0 sits right after
UpdateShadowSpace in FableWin (its first user) -> source is `g_pEngineLightingManager->PeekDiffuseVector()` copied into a local.

## 4. Members touched (this = ebx)
Ego_r CEngineShadowRenderer is byte-packed (StaticDataOrigin +0x261); SceneData = CShadowRendererSceneData at +0x0C:
| this+ | member (Ego_r) | type | access |
|---|---|---|---|
| 0x0C..0x4B | SceneData.WorldToShadowBufferMatrix (+0x00) | CMatrix4x4 | written (identity, PostTranslate, 2x PostMultiply) at the end |
| 0xFC | SceneData.ShadowFadeStart (+0xF0) | float | read/clamped 0x00B4E7CA |
| 0x100 | SceneData.ShadowFadeEnd (+0xF4) | float | written 0x00B4E4DE / 0x00B4E77A / 0x00B4E7C4 |
| 0x104..0x10C | SceneData.ShadowLightVector (+0xF8) | C3DVector | written 0x00B4D3C7.. |
| 0x110..0x118 | SceneData.CameraPos (+0x104) | C3DVector | read (negated) 0x00B4ED04 |
| 0x27C..0x2AB | CameraViewMatrix | CMatrix3x4 | read (rep movsd 12 dwords to a local) 0x00B4D3F6 |
| 0x2AC | PerspectiveShadowsEnabled | bool | read 0x00B4E0E6 (kept in cl, re-tested 0x00B4E654) |
| 0x2B0 / 0x2B4 | PerspectiveInterpStart / PerspectiveInterpEnd | float | 0x00B4E0F4 |
| 0x2B8 | ForegroundBufferSpan | float | 0x00B4E143, 0x00B4E76C |
| 0x2BC | MaxShadowDistance | float | many |
Shared header: rebuild/include/engine/CEngineShadowRenderer.h -- the CURRENT repo copy has only `FABLE_RETAIL_CENGINESHADOWRENDERER`
(opaque `_pad_0xc[0x334]`) or the generated flattened default (SceneData as `unsigned char[0x240]`). The member-level opt-in
`FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS` (+ new headers engine/CShadowRendererSceneData.h, engine/CShadowClipVolume.h,
`FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4` in CMatrix4x4.h) exists ONLY in the pending candidate overlay
**rebuild/candidates/manual/pending/2026-11-11_acctB_r3004/hdr/engine/** (row 0x00B4FC40 SetupShadowFuncRenderStates, same TU, matched).
Copy that overlay into $W/hdr (CEngineShadowRenderer.h, CShadowRendererSceneData.h, CShadowClipVolume.h, CMatrix4x4.h, CMatrix3x4.h,
CEngineLightingManager.h, C4DVector.h, CRenderTarget.h, CRenderStateCaptureBlock.h) and add PeekDiffuseVector / the private methods.
CEngineLightingManager: overlay `FABLE_RETAIL_CENGINELIGHTINGMANAGER(_MEMBERS)` has `C3DVector DiffuseVector; // +0x30`; repo default has
DiffuseVector_X/_Y/_Z at +0x30. Add `const C3DVector& PeekDiffuseVector() const { return DiffuseVector; }` (FableWin QBEABV).
CEngineCamera: only passed through (rebuild/include/engine/CEngineCamera.h).
Local models in landed siblings that have shared/overlay headers: UpdateShadowScene (local CMatrix3x4), BuildDerived (local CMatrix4x4 +
CShadowRendererSceneData), SetShadowClipVolumesToNeverShadow (local CShadowClipVolume/CShadowRendererSceneData/CEngineShadowRenderer) --
do NOT copy those local structs; use the headers.

## 5. Inline-helper visibility (FableWin COMDAT cluster after the shadow TU, FableWin.exe.signatures.tsv)
FableWin places these right after the shadow-renderer functions -> they are the TU's inline helpers (names exact):
- 0x02D78C40 `??$GFLimit@MHH@@YIXAAMABH1@Z` GFLimit<float,int,int>(float&, const int&, const int&) -- shared rebuild/include/engine/GFLimit.h
  (its comment cites this very donor). Fits the t clamp at 0x00B4E10A..0x00B4E13D: `GFLimit(t, 0, 1); if (t > 0) {...}` (jump-threaded).
- 0x02D792F0 `??0CMatrix4x4@@QAE@XZ` (empty), 0x02D79310 `??0CMatrix4x4@@QAE@W4EMatrix34Init@@@Z` (identity ctor; landed model
  rebuild/src/compiled/01/22/CMatrix4x4_EIdentity_01224340.cpp: `for j<4 for i<4 M[i][j]=(i==j)?1:0`, column loop kept, rows unrolled
  -- EXACTLY retail 0x00B4EC67..0x00B4ECEF; both must be __forceinline), 0x02D79420 `??0CMatrix4x4@@QAE@ABVCMatrix3x4@@@Z`,
  0x02D79450 `?SetIdentity@CMatrix4x4@@QAEXXZ`, 0x02D79500 `?Initialise@CMatrix4x4@@QAEXABVCMatrix3x4@@@Z`,
  0x02D795F0 `?Initialise@CMatrix4x4@@QAEXMMMMMMMMMMMMMMMM@Z`, 0x02D796C0 `?PostMultiply@CMatrix4x4@@QAEXABV1@@Z`,
  0x02D797F0 `?PerspectiveTransformPoint@CMatrix4x4@@QAEXAAVC3DVector@@@Z`, 0x02D79920 `?PostTranslate@CMatrix4x4@@QAEXABVC3DVector@@@Z`,
  0x02D79A10 `?PostScale@CMatrix4x4@@QAEXABVC3DVector@@@Z`. Also CMatrix3x4 PostScale/PostTranslate/PreTranslate (probably CalcLocalWorld...).
- Retail out-of-line COMDATs in this TU: CMatrix4x4::PostMultiply 0x00B507E0 (landed body = the i/j loop with `CMatrix4x4 result; ...; *this = result;`),
  Initialise(const CMatrix3x4&) 0x00B50770, ctor(const CMatrix3x4&) 0x00B50AE0 (per r3004 NOTES). They are inline-expanded here.
- r3004 lesson (rows/00b4fc40.cpp header): plain inline leaves Initialise (c2 score 79) and PostMultiply (313) CALLED (threshold 74);
  r3004 used `// Flags: /d2inlT314`; FABLE_FORCEINLINE on the CMatrix4x4 members gives identical code. Here PostMultiply is expanded 3x,
  so expect the same: force-inline (or /d2inlT) every CMatrix4x4 helper and CMatrix3x4::SetInverse (CMatrix3x4.h already FORCEINLINE under
  FABLE_RETAIL_CMATRIX3X4).
- CMatrix3x4.h opt-ins relevant: FABLE_RETAIL_CMATRIX3X4 (SetInverse, PostScale...), FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
  (X row of the 26-point transform is retail's `fld E31;fmul z;fld E21;fmul y;faddp;fld E11;fmul x;faddp;fadd E41`, 0x00B4DB2D..), and
  _TRANSFORMPOINT_SOURCE_DEST_SUMS / _SUMS variants -- Y and Z rows in retail are E12x,E32z,E22y and E23y,E13x,E33z (try each).
- No STLport, no CIntelligentPointer, no strings in this row.

## 6. Sibling landed rows (same TU) -- idioms
- 0x00B4F390 UpdateShadowScene (tail-calls this row; plain C++, no pragmas; LOCAL CMatrix3x4 -- bad pattern).
- 0x00B4BA60 BuildDerived... (`#include "fable_compiler.h"`, local CMatrix4x4 with FABLE_FORCEINLINE Scale/Translate loops
  `for (int i=0;i<4;++i){M[i][0]*=s.X;...}` and `M[i][0]+=M[i][3]*t.X` -- these ARE PostScale/PostTranslate's bodies; `x*1` folds to a copy,
  matching retail 0x00B4EB5B/0x00B4EB91/0x00B4EC5D `mov`-copies here).
- 0x00B4CE20 SetShadowClipVolumesToNeverShadow, 0x00B4CED0 CShadowClipVolume::ClipBoundingBox (retail_asm SSE via engine/C3DBoundingBoxSSE.h).
- pending 0x00B4FC40 SetupShadowFuncRenderStates (rebuild/candidates/manual/pending/2026-11-11_acctB_r3004/rows/00b4fc40.cpp): the full
  #define list to copy: FABLE_RETAIL_CENGINESHADOWRENDERER, _MEMBERS, FABLE_RETAIL_CENGINECOMPONENT, FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4,
  FABLE_RETAIL_CENGINELIGHTINGMANAGER(_MEMBERS), FABLE_RETAIL_C4DVECTOR, FABLE_RETAIL_CMATRIX3X4; lesson: operand order of inlined
  PostMultiply column sums keyed on the caller's named-symbol count mod 8 (add/remove named locals to steer).
- Also pending c2047 (rebuild/candidates/manual/pending/2026-11-12_acctB_c2047) has a CMatrix4x4.h overlay (AlignedSetMultiply) -- merge, don't clobber.

## 7. Facts the author must not miss
- The C2DVector `dir` at [esp+84h] is built from lightPts[13] - lightPts[0] (X,Y), then AccurateNormalise (0x00B4DCFD).
- rot = Initialise(d.Y, d.X, 0, 0,  -d.X, d.Y, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1) at [esp+34h]; its PerspectiveTransformPoint is
  constant-propagated (x*0.0 kept, `p.Z*0` CSE'd into one slot [esp+20h]).
- Flip branch = rot.PostScale(C3DVector(-1,-1,1)) (constant-folded to -0.0f stores 0x80000000).
- ShadowFadeStart clamp uses a DOUBLE 0.9 (`fmul qword [0x012A23B8]`): `ShadowFadeEnd * 0.9` unsuffixed.
- Frame slots are shared (0x8C: view copy -> frustum[5] -> PostMultiply result; 0x100: baseSpace 3x4 -> last PostMultiply result):
  the inline PostMultiply `result` temporaries and the earlier locals must have disjoint lifetimes (scope blocks may be needed).
