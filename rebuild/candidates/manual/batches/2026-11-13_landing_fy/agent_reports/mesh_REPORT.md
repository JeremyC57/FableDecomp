# acctB port, group "mesh" -- report

All 16 rows are STAGED. `acctb_stage_verify.py $WS`: 16 OK (RELOCATION_MATCH + test PASS), 0 regressed.
Global check of the final `$WS/inc` against the repo: 66 headers differ (15 new, 51 changed; list in `global/changed_headers.txt`).
hdrcheck2 gives `default=same` for all 51 changed ones. Every landed row that uses an opt-in whose meaning changed, plus
00a11d10 00a11f20 00a128e0 00a12ab0 00b399a0 00f39730 00a69b70, was rechecked with vland against `$WS/inc`: all RELOCATION_MATCH (`global/recheck.log`).

## One deviation from the brief (please review)
`acctb_stage.py` renames an opt-in to `<NAME>_B` whenever it can't read the opt-in's block, for example when the name appears in a
compound `#if defined(A) && !defined(B)` or in an in-header `#ifndef X / #define X`. The rename also fires when the content is the same.
It is also not idempotent: once a row's merge is in `$WS/inc`, a later row (or ACCTB_REDO) with the same overlay gets its own names renamed.
I added no `_B` aliases for these. I staged with a copy, `tools/acctb_stage_keep.py`. Its only change is the env `ACCTB_KEEP=<names>`,
which skips the rename for the names listed. All other gates are unchanged. I listed a name only after checking that the repo or `$WS/inc` block has the same content
(or that the row was respelled to the repo's name). The list is in `keep.txt`:
STLP_SORT_INLINE_FORCE, CMATRIX3X4_TRANSFORMPOINT_LTR, CMATRIX3X4_TRANSFORMPLANE, C3DANIMATIONSEQUENCEDATA_INTERPOLATE, CQUATERNION_MULASSIGN,
C3DMESH2_INVERSETRANSFORM, CENGINEPRIMITIVERENDERER_STATICMESHMANAGER, CMATRIX3X4_SETIDENTITY (the repo's FABLE_FORCEINLINE body; the overlay's plain-inline
copy is not needed: every row matches with the repo's), C3DANIMATION2_CALCLOCAL, C3DANIMATION2_INITSEQUENCES, C3DANIMATIONBLENDSTATE_SHAREDDATA,
C3DMESH2_BONECOUNT, C3DMESH2_BONES, CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP, C3DBOUNDINGSPHERE, CMATRIX3X4_TRANSFORMPOINT_COPY_SUMS, CVERTEXSHADER,
CMATRIX3X4_SETTRANSLATEONLY_XYZ, C3DMESHINFO_LODERROR, C3DMESHINFO_BEGINLOADINGTEXTURES (all FABLE_RETAIL_-prefixed).
**Deliverable = `$WS/inc` + `$WS/stage/rows`.** The `fix/` folders are per-step snapshots and some are stale (later rows changed the same header),
so do not replay `fix/` with the stock driver. The `headers` lists in `stage/rows/*.json` are empty or short for rows I restaged with ACCTB_REDO; use `global/changed_headers.txt`.

## Per row
- 00a12ee0 STAGED. CMatrix3x4.h: CPlane include + TransformPlane under TRANSFORMPLANE; TRANSFORMPOINT_LTR (same body) folded into the existing
  `#if defined(FABLE_RETAIL_CPARTICLE_SPRITE) || defined(..._TRANSFORMPOINT_LTR)` branch. CPhysicsMesh.h pinned. Source: CPHYSICSMESH_NAVVOLUMES -> repo CPHYSICSMESH_NAVIGATION
  (same member and accessor). Auto: CPlane, CContainmentVolume, CPhysicsMeshObject, new C3DMeshVolume.h.
- 00a4d920 STAGED. C3DAnimation2.h: CALCLOCAL (+CALCLOCALBONE) declarations, with their own forward-declaration block (the repo INFO_BUILD lines are untouched). C3DAnimationSequence.h: InlineGetKeyframeInterpolate
  under the existing INTERPOLATE. Source unchanged.
- 00bf12a0 STAGED. CMatrix3x4.h: SETMULTIPLY_POSTMULTIPLY nested in the repo SETMULTIPLY_B block. C3DMesh2.h: INVERSETRANSFORM (original line kept in #else).
  C3DMeshStats.h: GENERATORS (typed MeshGenerators + Begin/End only). New C3DDummyObject.h, C3DGeneratorObject.h, CEnginePrimitiveManagerStaticMeshes.h.
  Source: SETMULTIPLY -> SETMULTIPLY_B (identical content), C3DMESHINFO_LODS -> repo ISLOADED + DETAILLEVEL.
- 00bf18b0 STAGED. C3DMeshStats.h: NEW nested opt-in C3DMESHSTATS_DUMMYOBJECT_RESULT inside the repo DUMMYOBJECTS, giving the PDB's `C3DDummyObject&` out parameter
  for the two GetDummyObjectPositionAnimated virtuals. CEngineInternalPrimitiveAnimatedMesh.h: ANIM, as `#if defined(ORIGINAL_CACHE) || defined(ANIM)` on the Anim member.
  Source: SETMULTIPLY -> _B, LODS -> ISLOADED + DETAILLEVEL, ANIMATIONMANAGER -> repo LOADSTATS + CMESHDATABANK_GETANIMATIONMANAGER, plus DUMMYOBJECTS + DUMMYOBJECT_RESULT
  (the overlay's GENERATORS carried duplicate virtuals).
- 00a4e300 STAGED. CMatrix3x4.h: SWAP template under TRANSPOSE; MULTIPLYNOTRANSLATION (SetMultiplyNoTranslation via SetMultiply + PreScale(x,y,z)) guarded by
  `MULTIPLYNOTRANSLATION && SETMULTIPLY_B`; TRANSPOSE; NEW nested opt-in CMATRIX3X4_TRANSFORMVECTOR_GROUPED_X. This row needs the X row of TransformVector grouped;
  the repo body is unchanged. C3DAnimation2 NONLOOPINGDURATION, Sequence(+Data) GETKEYFRAME, C3DAnimationStats MOVEMENTVECTOR/PROTECTED(+GETTYPE).
  C4DVector.h pinned (the overlay's copy is a subset of the current one), which resolves HDR_USED_MACRO_DEFINED_BY_HEADER with no change to C4DVECTOR_ADDASSIGN.
  Source: SETMULTIPLY -> _B, + TRANSFORMVECTOR_GROUPED_X.
- 00a50510 STAGED. C3DAnimation2 INITSEQUENCES, C3DAnimationSequence(+Data) INITIALISE; new C3DBone2.h, C3DAnimationBuilder.h,
  N3DAnimationBuilder_COutputAnimation.h. Source: SETMULTIPLY -> _B; POSTMULTIPLY + POSTMULTIPLY_SETMULTIPLY -> SETMULTIPLY_POSTMULTIPLY (the same PostMultiply body;
  the repo's POSTMULTIPLY block is not wrapped).
- 00b4fc40 STAGED. Clean 3-way merges: CEngineLightingManager, CEngineShadowRenderer, CMatrix4x4 (FROMMATRIX3X4); new CShadowClipVolume.h, CShadowRendererSceneData.h. Source unchanged.
- 00aa0090 STAGED. C3DAnimation2 CONVERTTIME_BODY (body-or-declaration inside CALCLOCAL) + MANAGER; Sequence FRAMEKEYFRAME; SequenceData ISIDENTITY; Stats ISRELATIVE;
  C3DMesh2 BONECOUNT (guarded !BONES); C3DAnimationStateAlpha typed class under C3DANIMATIONSTATEALPHA (no in-header #define of C3DANIMATIONSTATE);
  C3DAnimationBlendState: the repo SHAREDDATA is kept, plus a NEW nested opt-in C3DANIMATIONBLENDSTATE_ACCESSORS (PeekBaseAnimation / GetCombinationBlendCount /
  PeekCombinationBlend); 3-way merges of C3DAnimationInfo, C3DAnimationState, GFGetInterpolatedValue. Source: + C3DANIMATIONSTATE, C3DANIMATIONBLENDSTATE_TYPED_DATA,
  CIVCOUNTEDPOINTER_CONVERT, C3DANIMATIONBLENDSTATE_ACCESSORS (the overlay header #defined the first three itself).
- 00a9e1e0 STAGED. CMatrix4x4 SETMULTIPLYTRANSPOSED(_FORCEINLINE); CQuaternion MATRIX4_FORCEINLINE + MATRIX4_TRANSPOSED; C3DKeyframe TOMATRIX4_INLINE (each of the
  two 4x4 declarations wrapped on its own, repo order kept); CMatrix4x4AlignedSSE.h 3-way (ALIGNEDSETMULTIPLY_ACCESSORS wraps the x87 arm). Source unchanged.
- 00a9f540 STAGED. Same overlay as 00aa0090; no header change of its own. Source: the same four added defines.
- 00bc25f0 STAGED. C3DMeshInfo BEGINLOADINGTEXTURES; C3DMesh2 PRIMITIVES; C3DPrimitive2 PARTICLEMESH = accessors only (the typed buffers are the repo's
  INDEXBUFFER + VERTEXBUFFER); 3-way merges of C3DMeshMaterial, CEngineInternalPrimitiveParticleMeshGroup, CEnginePrimitiveManager,
  CEnginePrimitiveManagerParticleMeshGroup and CShaderRenderManager, with the overlay's in-header `#ifndef X/#define X` removed for opt-ins the row defines itself.
  Source: CENGINEINTERNALPRIMITIVEPARTICLEMESH -> repo CENGINEINTERNALPRIMITIVEPARTICLEMESH_CLASS; + SCOPED_PTR, C3DPRIMITIVE2_INDEXBUFFER, C3DPRIMITIVE2_VERTEXBUFFER.
- 00bb8750 STAGED. 3-way merge of CEnginePrimitiveManagerParticleDecalGroup; new ParticleDecal / ParticleDecalGroup headers (in-header #define of
  CENGINEINTERNALPRIMITIVEPARTICLEGROUP removed), CTVertexNormTex1(.Base).h. Source unchanged.
- 00bb8ef0 STAGED. ParticleDecalGroup gets the RenderSubPrimitive override declaration (a base virtual, so no vtable change). Source unchanged.
- 00b9a4a0 STAGED. C3DMeshInfo: LODERROR (GetLODError only) + BEGINLOADINGTEXTURES (shared with 00bc25f0). The overlay's LODS block duplicated the repo's ISLOADED/DETAILLEVEL and is not kept.
  CShadowClipVolume CLIPBOUNDINGBOX; CEngineRenderInfoMeshBase: the repo's independent version is kept, and CEngineRenderInfoRepeatedStaticMesh is added under CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH;
  merges for the RepeatedStaticMesh / occlusion / render-util / shared-data / shadow-renderer headers; new CEngineSubPrimitiveBoundingBox.h,
  CEngineSubPrimitiveRepeatedStaticMesh.h. Source: LODS -> ISLOADED + DETAILLEVEL + BEGINLOADINGTEXTURES (LODERROR kept).
- 00b8df50 STAGED. CMatrix3x4: SETTRANSLATEONLY_XYZ nested in the repo SETTRANSLATEONLY; TRANSFORMPOINT_COPY_SUMS added as an `#elif` of the existing SOURCE_DEST_SUMS
  chain. The automatic merge had wrapped about 70 lines there, which is what broke the six landed CMatrix3x4 users. CEngineOcclusionManager MEMBERS (in-header CVERTEXSHADER define removed),
  CEngineStateBlocks APPLY_INLINE, CRenderManagerCore DRAWASTRIFAN, CEngineCamera CLIPSPHERE (it came in with 00b9a4a0, same overlay folder). Source unchanged.
- 00ba6ff0 STAGED. Merges for MeshBase / StaticMesh / ManagerOcclusionBase / StaticMeshesBase / RenderUtil FINDLOCALLIGHTS / RendererSharedData SILHOUETTES / CRGBColour
  GETASINT / LightingManager LIGHTSINSPHERE. CRenderStateCaptureBlock, CMatrix3x4 and CIVCountedPointer are pinned: the automatic CaptureBlock merge broke the landed 00f39730.
  fable_string.h is pinned to the repo copy: the automatic merge had added duplicated `_B` / `_B2` CWIDESTRING blocks, and those are removed. Source: LODS -> ISLOADED + DETAILLEVEL + BEGINLOADINGTEXTURES.

## Opt-in renames (old -> new, row sources)
FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY -> FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_B (the repo's existing block, same content) in 00bf12a0, 00bf18b0, 00a4e300, 00a50510.
Respelled to repo names (same retail meaning): CPHYSICSMESH_NAVVOLUMES -> CPHYSICSMESH_NAVIGATION; C3DMESHINFO_LODS -> C3DMESHINFO_ISLOADED + _DETAILLEVEL;
C3DMESHINFO_ANIMATIONMANAGER -> C3DMESHINFO_LOADSTATS + CMESHDATABANK_GETANIMATIONMANAGER; CMATRIX3X4_POSTMULTIPLY + _POSTMULTIPLY_SETMULTIPLY -> CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY;
CENGINEINTERNALPRIMITIVEPARTICLEMESH -> CENGINEINTERNALPRIMITIVEPARTICLEMESH_CLASS.
New opt-in names I introduced (no incoming equivalent fits): CMATRIX3X4_TRANSFORMVECTOR_GROUPED_X, C3DMESHSTATS_DUMMYOBJECT_RESULT, C3DANIMATIONBLENDSTATE_ACCESSORS.
Other source changes are only the added `#define`s listed per row. No code bodies were changed.

## For the coordinator to review
1. **Repo bug, landed row broken today:** in rebuild/include/engine/CIVCountedPointer.h, RESET_FORCEINLINE's `#endif` is followed by `#ifdef CASTTOBASE ... #else Reset`,
   so Reset is declared twice. The landed 0x00A69B70 (CTextureManager::AllocateTexture) fails to compile against today's repo header
   (C2535; checked with fg.py). `$WS/inc` fixes this with `#elif defined(FABLE_RETAIL_CIVCOUNTEDPOINTER_CASTTOBASE)`; with that fix 00a69b70 is RELOCATION_MATCH.
2. The repo's C3DMeshStats DUMMYOBJECTS (and C3DMesh2 HELPERPOINTS) spell the GetDummyObjectPositionAnimated out parameter `C3DVector&`. Both PDBs say
   `C3DDummyObject&` (`...@@UBEXABVC3DDummyObject@@AAV2@...`). This is a NAME_FIXES candidate; I added the correct spelling only as a nested opt-in.
3. Repo-side in-header opt-in #defines are still present in headers that were new or merged by the other lane: CShadowClipVolume.h (C3DBOUNDINGSPHERE)
   and CEngineShadowRenderer.h (CTEXTURE, CSURFACE). I left them because 00b4fc40 and 00b9a4a0 were staged with them.
4. CMATRIX3X4_MULTIPLYNOTRANSLATION is the overlay's "FITTED body, needs a ruling" (the FableWin /Od size fits the 9-sum body better); it is carried as is.
5. Rows 00bc25f0 / 00bb8ef0 carry descriptive names from the other lane for five render-switch globals (`g_particleMesh..._0139b7a0..b0`).
   00bc25f0 also carries a row-local `GFDebugPDynamicCast` template. Both are unchanged; please check them against the made-up-name rule.
