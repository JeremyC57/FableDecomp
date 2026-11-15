# acctC workflow hand-back wfE7 (unworked rows round 3, 2026-11-15)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00B3CB30 | CEngineResourceManager::CEngineResourceManager | 5,399 | RELOCATION_MATCH | PASS |
| 0x00B69000 | CEnginePrimitiveManagerParticleSpriteGroup::CEnginePrimitiveManagerParticleSpriteGroup | 8,358 | RELOCATION_MATCH | PASS |
| 0x00B33B50 | CEnginePrimitiveRenderer::CEnginePrimitiveRenderer | 8,858 | RELOCATION_MATCH | PASS |
| 0x00A059E0 | CRenderStateManager::SetDefaultRenderStates | 7,958 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; review items are for the owner / landing account.

`open/` holds the best drafts of the batch's unmatched rows (full residues in NOTES.md):
- 0x00BD3070: parked, DIFFER(11272v11288). 306 hunks, 16 bytes short. Measured: arms 4, 8 and 6 jump into a shared ApplyPixelShader + SetEnableFadedShadowFactor tail owned by arm 7 (0x00BD3E16/17). Ours loads GPShaderRenderManager before the shader-argument push, so no shared tail forms (+24/-35/+35/+4

## 0x00B3CB30
Verifier audit: none. Warnings only: RELOC_NAME SetTextureBank@CMeshDataBank -> 00a23b50 (landed label _global::SetMember_dword0x3d4_00a23b50) and PeekVideoOptionsDef@CEngine -> 00b244f0 (landed label CTextureManager::GetPoolTextureSize); both are in NAME_FIXES.tsv. No __asm/naked/_emit/volatile, no inline control, no if(0). Local classes (CVertexStreamFormat, CVertexShaderInput, N3DPrimitive::CVertexShaderInputStatic/Animated, CAsyncLoadingMemoryPool, CEngineVideoOptions, struct _D3DVERTEXELEMENT9): none has an engine header (checked main and the overlay). They follow landed siblings 0x00A98640/0x00B24690/0x00B3B5D0. `CWideString unused_path;` (line 241) is a real retail object: retail calls the ctor at 0x00B3D723 (lea ecx,[esp+64h]; call 0x99AED0). It is not listed among the EVIDENCED spellings, which is a documentation gap only. Header diffs are additive FABLE_RETAIL_* opt-ins (e.g. CENGINE_PEEKVIDEOOPTIONSDEF, CSURFACE_CLEARCOLOUR), plus asserts pinned with FABLE_STATIC_ASSERT_AT (contents unchanged). The behaviour test is a model of the ctor run against recording stubs, with 27 real checks; it does not execute the compiled row.

Fitted items: none (header says FITTED: none). Descriptive names: has_mesh_bank, platform, unused_path, opaque_format, scratch_texture_count, g_Declaration* arrays, g_pVideoOptions.

## 0x00B69000
Verifier audit: none. No warnings in vland.log. No asm/volatile/inline control/dead code, and no local class models. Same-TU callee CalcForegroundFadeFactors (retail 0x00B671E0, which retail CALLS) is defined in the source. Its body is the landed row's text respelled on the shared header: landed m_fadeOutFactors +0x62C = Settings(+0x608).BackgroundFadeTransform(+0x24), m_fadeInFactors +0x61C = ForegroundFadeTransform, Set -> Initialise, same expressions. So it is the real callee, not a stand-in. Catalog name corrected to CEngineLandscapeRenderer ctor (vtable 0x012A2B54 + PDB). Header diffs are additive. The behaviour test is a model test with 12 checks.

Fitted items: none (FITTED: none). Descriptive: loop locals i / z.

## 0x00B33B50
Verifier audit: none. Warnings only: RELOC_NAME SetSoftwareVertexShaderEnable (009886f0), SetSoftwareFixedFunctionEnable (00988700), SetDP3BumpEnable (00a23b60) and AddEnableConsoleVariable (00bacd80); all point at landed placeholder labels and are in NAME_FIXES.tsv. Local classes with base clauses (CEngineScene, CEnginePrimitiveManagerAnimatedMeshes/WeaponTrails/Lines/Text, CEnginePrimitiveGroupManager, CEnginePrimitiveChangerManager, CEnginePrimitiveManagerDecal/WeatherMask): none has rebuild/include/engine/<Name>.h or an overlay header, and all are declaration-only. Same-TU callee AddPrimitiveManager (0x00B33AE0) has text identical to the landed row body. Header changes are additive opt-ins: CENGINEPRIMITIVERENDERER_MEMBERS is placed before the existing _SCENELIST branch as #ifdef/#elif, CENGINEDEF_LODERRORTOLERANCE before FOV_2D, plus the *_CLASS/_OVERRIDES opt-ins. These are listed in hdr/README.md. The behaviour test is a model test with 10 checks.

Fitted items: none (FITTED: none). Descriptive/landed-binding globals: g_pEnginePrimitiveTypeInfo, g_pEngineDef_01436e20.

## 0x00A059E0
Verifier audit: UNVERIFIED: vland WARN RELOC_CONFLICT ?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA -> 01396dd8 in this row, 01396de8 in landed 00bbc460 (CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive). The author's case for 0x01396DD8 is the CRenderStateManager.h comment plus this row reading [1] at 0x01396DDC. It is filed in NAME_FIXES.tsv but not resolved, so this row or the landed row binds the symbol wrongly. Also RELOC_NAME D3DTextureUVModeTable -> 01396dc8, labelled g_textureUVModeToD3D (in NAME_FIXES). Review items, not violations: (a) 19 setters that CRenderStateManager.h only declares are defined FABLE_FORCEINLINE in the row source. This works as an inline control (plain /O2 calls 18 of them) but has no `// Inline-budget:` marker. Precedent is landed 0x00BA1B00, and NOTES says it equals the /d2inlS114 flag result. (b) FableWin.pdb debug copies of SetPointSizeMin..DistSqr each carry a float ASSERT that the TU text omits (dead in release). No asm/volatile/if(0). The behaviour test is a model of the SetRenderState bookmark/queue logic with 24 checks, a real test.

Fitted items: FITTED (1): first texture-factor write as SetRenderState(0xFF000000, DeviceState.TextureFactor) instead of SetTextureFactor (c2 record-id tie). FITTED (2): SetPointSizeMin / SetPointSizeMax read the float bits into a named local `const unsigned long value` (record-id tie; FableWin debug text has no such local).

## Coordinator note: D3DAlphaFuncModeTable binding (RELOC_CONFLICT warnings)
0x00A059E0 and 0x00BC04F0 (wfE5), like the earlier 0x00B555A0, bind `CRenderStateManager::D3DAlphaFuncModeTable` to 0x01396DD8;
the single landed row 0x00BBC460 binds it to 0x01396DE8. Retail reads table[1] at 0x01396DDC (0x00A059E0) and table[4] at
0x01396DE8, so the base is 0x01396DD8 and the landed row recorded an element address: respell that row at landing.
