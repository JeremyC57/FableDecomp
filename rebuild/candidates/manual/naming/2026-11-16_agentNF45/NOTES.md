# NF45 round 7 notes (the NEngineInternal render globals: 55 worklist binder rows, 5 queue entries)

## Outcome (counted from STATUS.tsv / APPLIED.tsv / work/recheck_final.log)
- **Rows (55):**

  | Status | Rows | Detail |
  |---|---|---|
  | ported | 48 | 47 changed by NF45; 0x00B63530 is NF41's landed port, unchanged here (one fresh compile, RELOCATION_MATCH, lock released). 3 of the 47 still have a GP read I did not attempt: 0x00B33010 (0x01436E60, 0x01436EA0), 0x00B63010 (0x01436E8C) and 0x00B77E20 (0x01436E8C); see below |
  | not-a-binder | 0 | |
  | blocked | 7 | 5 register-tie, 2 authoring; all unchanged, locks released |
  | deferred | 0 | |

- **Final recheck:** `recheck_final.log` is one vland `--recheck-landed` run over the 47 changed rows. All 47 are RELOCATION_MATCH and every test PASSes.
- **Entries (APPLIED.tsv):**
  - done: 30, 38, 82.
  - blocked: 29 (7 rows left, listed below) and 31 (0x00B33010 and 0x00BF4570).
- **Extra queue lines.** Each line below now has more binders EXACT, but none is closed; most of their binders are NF46's.

  | Line | Global | Rows now EXACT |
  |---|---|---|
  | 32 | GPPrimitiveRendererSharedData | 0x00B25D80 |
  | 33 | GPPrimitiveTypeInfo | 0x00BE1E70 |
  | 78 | GPLightingManager | 0x00B54310 |
  | 79 | GPCamera | 0x00B32660, 0x00B4D2C0, 0x00B54310 |

  Not attempted in rows I hold (the rows are `ported` for their other globals; I investigated these reads but did not change them; findings are in STATUS.tsv):
  - 34 (0x01436E8C): 0x00B63010 `g_pEngineState_01436e8c` and 0x00B77E20 `g_pEngineEnvironment`.
    - Both chains are `GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams()`, as in the landed 0x00B65A20.
    - engine/CIDrawEnvironment.h pulls engine/CArray.h, which collides with each row's local CArray template.
  - 31 / 79: 0x00B33010 `g_pOcclusionRenderer` / `g_pLightingManager_01436ea0`. The PDB CEngineScene signatures that take them are in STATUS.tsv.
- **Non-queue globals respelled along the way:**
  - 0x01436E74 → `NEngineInternal::GPSubPrimitiveRenderer` (0x00BE1E70). Both PDB name sets, codex pass22 NAME_FIXES.tsv, and the landed 0x00BCA6B0 binding.
  - 0x01436E1C → `NEngineInternal::GPDisplayManager` (0x00B4D2C0).

## Grades
- **work/grades.log has 72 grades.** Per row: max 6 logged on 0x00B32AD0, 4 on 0x00BB8090, 3 on 0x00BE1E70; the rest 1-2.
  - 0x00B32AD0: grades.log and the session transcript both show 6 g.sh grades. My pre-compaction summary said 8, which I cannot confirm; it is at most 8, under the cap of 10. 0x00BB8090 / 0x00BAD2E0: 4 / 2 in both the log and the transcript (the summary's 5 / 2-3 were wrong).
  - Verdicts: 52 RELOCATION_MATCH, 18 DIFFER (all on the 7 blocked rows), 2 SRC_FAIL. The SRC_FAILs were 0x00B4D2C0 (CDisplayManager needs its SDK-type prerequisites) and 0x00B54310 (CVolumeTexture.h must precede CRenderManager.h).
- **Not grades:** the 47 original landed sources, compiled in the private `pO/` for the relocation diff (work/xr_old, work/xr_new). All 47 originals are RELOCATION_MATCH.

## Families and shapes
- **Render-core rows (entry 29, 41 rows).** These rows carried local CRenderManagerCoreView / CRenderManagerCore / CRenderStateManager / CTrackedRenderState / SetState models on `g_pRenderManagerCore` / `g_pRenderStateManager_01436e18`. They now read `NEngineInternal::GPRenderManager` (engine/CRenderManager.h; CRenderManagerCore / CRenderStateManager at offset 0), using:
  - the inline CRenderStateManager setters, with every `g_renderStateDefault_*` / `g_renderStateValue_*` table read spelled as its D3D*Table enumerator (`work/rsmap.py` reads the members, setters and tables from the headers and reloc_bindings.tsv);
  - the Ego_r.pdb inlines AttachTextureToStage / AttachVolumeTextureToStage / SelectVertexBuffer / SelectIndexBuffer (repo FORCEINLINE_HELPERS / VOLUMETEXTURE_INLINE);
  - CRenderStateScopedCaptureBlock or a member CRenderStateCaptureBlock for the open-capture / RestoreCaptureBlock pair on 0x013CB508 (`?StateManager@CRenderStateCaptureBlock@@0PAVCRenderStateManager@@A`).

  Scripts: xsb.py (the 6 state-block Apply rows), xss.py, strip.py, xcall.py, xse.py and xwater.py / xwhdr.py (the water rows).
- **GPScreenEffectDisplacementRenderer (entry 82).** 0x00BBBF70 and 0x00BE0CD0 were on a `CEngineSettings_01436e38` stand-in; 0x00B33010 on a local CEngineShadowRenderer with invented BeginShadows / ... names.
- **GPOcclusionManager (entry 30):** 0x00B662F0 (AddFlareTest) and 0x00BF7100 (RenderOcclusionBox).
- **GPShadowRenderer (entry 31):** 0x00B68610 (PeekRenderingTechnique) and 0x00B32660.
- **GPEngine (entry 38):** 0x00B32660 (PeekVideoOptionsDef).
- **Mist row 0x00B54310.** The local "mist parameter" view at 0x01436E9C +0xD0..+0xDC is CEngineLightingManager::MistEffectColour R / G / B / A. The local camera view's "ViewTransform" at 0x01436EA0 +0xE4 is CEngineCamera::InverseViewTransform. Both spellings come from the engine headers.

## Blocked rows (STATUS.tsv has the exact sites)
- **Register tie (5).** Each row is unchanged, has its port attempt in work/abandoned/, and had its lock released.
  - Rows: 0x00B32AD0, 0x00BB8090, 0x00BAD2E0, 0x00BCB800 and 0x00B76BC0.
  - In each, every site matches except one or two tracked-state sites. There, retail loads the two bookmark masks into the other register pair.
  - All five originals read one mask into a named local first: SetStateConst, MaskFirst or StateMaskFirst. The shared SetRenderState with DESIRED + BOOKMARK + ADDSTATE_DIRTY leaves the same diff.
  - What must come first: a per-site-faithful SetRenderState form in engine/CRenderStateManager.h. FINDINGS_LOG calls this class "register-tie".
- **Authoring (2).** 0x00BF4570 and 0x00BE6F70 are July rows on `candidates/*_compiled.h`, with stand-in helper bodies.
  - The landed bodies are 316 / 198 bytes; retail is 4,431 / 2,654 bytes.
  - Retail reads 0x01436E18 inline: 19 / 17 absolute refs, plus 2 to 0x01436E60 (work/absrefs.tsv).
  - Each was compiled once, unchanged: DIFFER.

## Self rows that now disagree with their callers (for the coordinator; not changed here, not mine)
I ran callers.py on each of these addresses.
- **0x00A0AA10** (`E2_E2_00a0aa10.cpp`, leaf `?M@C00a0aa10@@QAEXHH@Z`, catalog `Nop_ret8`):
  - its one caller, 0x00B32300, now binds `?SetMaterial@CRenderManagerCore@@QAEXABVCRGBColour@@0@Z`;
  - Ego_r.pdb / FableWin.pdb, `ret 8` = two references.
- **0x00B8D040** (`E2_E2_00b8d040.cpp`, leaf `?M@C00b8d040@@QAEXHHHH@Z`, catalog `Nop_ret16`):
  - its one caller, 0x00B662F0, binds `?AddFlareTest@CEngineOcclusionManager@@QAEXKABVC3DVector@@MJ@Z`;
  - Ego_r 0x00A34750 + the class delta 0x1588F0; `ret 16` = four arguments.
- **0x00B244F0** (catalog `CTextureManager::GetPoolTextureSize`):
  - 0x00B32660 binds `?PeekVideoOptionsDef@CEngine@@QAEABVCEngineVideoOptionsDef@@W4EEngineVideoOptionQuality@@@Z` (repo opt-in FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF);
  - the other landed caller is 0x00B3CB30;
  - two call sites (0x00B3297D / 0x00B3299B) are in no landed row.
- **0x00A0AA80** (leaf `?SetAWindow@CRenderManagerCoreWindowView@@QAEXABVC2DBoxF@@@Z`, catalog `Forward_SetViewport_via0x3a3c`):
  - 0x00B4F510 binds `?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z`;
  - the other callers are 0x009D9C80, 0x009DA9F0, 0x00F38930 and 0x00F39730.
- **0x00B86BB0** (`unattributed_ClearFlagAndRestoreCaptureBlock_00b86bb0.cpp`, catalog `AddToInterface`, a propagated label): 0x00B33010 binds `?FinishDisplacementStencilRendering@CEngineScreenEffectDisplacementRenderer@@QAEXXZ`.

## Other notes
- **`#ifndef _MSC_VER` fallback branches removed** (local class models for a non-MSVC parse) in 0x00B32230, 0x00B3B0C0 and 0x00B4D2C0. Each row now includes the shared headers unconditionally.
- **0x00B4D2C0:** its own leaf is still the pre-existing `ShadowMethods::PostLayerRender` stand-in (vland STANDIN_LEAF warning). This round did not change it.
- **0x00B63010:** the render-manager part is ported. Its 0x01436E8C read (`g_pEngineState_01436e8c`, entry 34) is not.
- **Row debt.** The pre-existing local models of other shared-header classes stay. The final recheck has 179 SHARED_HEADER_AVAILABLE warnings, for example CShaderRenderManager, CArray, CMatrix3x4 and the water / weather renderer views.
  - No new local class was added. A per-row comparison of the `class` / `struct` names in the new source against the original found none.
- **Tests are unchanged (no new test this round).** 19 of the 47 repo tests still spell the old stand-in globals (`g_pRenderManagerCore`, `g_renderStateDefault_*`, `g2D` / `gCamera` / `gDisplay` / `gState`, `g_pEngineSettings_01436e38`, `gOcclusionManager`, ...).
  - The rows: 0x00B25D80, 0x00B32100, 0x00B32230, 0x00B3B0C0, 0x00B47630, 0x00B4D2C0, 0x00B54100, 0x00B54210, 0x00B56B00, 0x00B56F50, 0x00B586A0, 0x00B592C0, 0x00B66190, 0x00B871B0, 0x00B9E750, 0x00BCD490, 0x00BE0CD0, 0x00BEACA0 and 0x00BF7100.
  - All 19 are self-contained behaviour tests: 14 embed a copy of the old row body, and 5 carry their own behaviour model. Each defines its own fakes and does not link the row object.
  - So they still check the unchanged behaviour (a name port changes no logic), and all PASS in recheck_final.log. But they now spell names the landed rows no longer use. Respelling them is a follow-up, not done here.
- **No EH funclet includes any changed row.** I grepped rebuild/src and rebuild/tests for each row's file name: no hits.
- **RENAMES.tsv:**
  - **Section 1 is empty.** For all 47 rows, the self symbol of the new object equals the original's.
  - **Section 2 has 174 lines:** `0xROW <TAB> old <TAB> new <TAB> 0xTARGET`, one per changed relocation binding, from a fresh compile of each original and each new source. Targets:

    | Target | Lines |
    |---|---|
    | 0x01436E18 | 41 |
    | 0x013CB508 | 17 |
    | 0x00A05840 | 9 |
    | D3D table rebinds 0x01396xxx | the rest, with the callee respellings |

    On the D3D*Table lines the fourth column is the operand address the row reads (table base + 4 x index, as xref resolves it from retail), not the table symbol's own address. For example `?D3DTextureUVModeTable@CRenderStateManager@@0PAKA` appears with 0x01396DC8 / 0x01396DCC / 0x01396DD0.

## Descriptive names
- No new function, global or class names.
- Local variable names in changed bodies (`capture_block`, `lighting`) are descriptive.
- Every global respelling is a PDB data symbol from gp_binders.tsv, or the PDB-evidenced GPSubPrimitiveRenderer / GPDisplayManager.
