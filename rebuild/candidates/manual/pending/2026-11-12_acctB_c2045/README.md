# acctB hand-back, chunk 2045 (1 row matched, with a per-row inline-budget flag)

Authored by an Opus 5.5 agent from chunk 2038's headers, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00BB8EF0 `?RenderSubPrimitive@CEnginePrimitiveManagerParticleDecalGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z` | RELOCATION_MATCH 1941v1941 with `// Flags: /d2inlT189`, test PASS (g++ and VC7.1) | 2 grades (matched at the first). True length 1941: the 2659 estimate also covered 0x00BB9690..0x00BB9952, this source file's unreferenced inline copies (`SPLITS.tsv`) |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`, flags from the row's `// Flags:` line):
`coordverify/` (grade, both test runs, read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; one
`WARN RELOC_NAME`, item 4). Purity and padded-struct greps are clean. Two extra coordinator checks: with `/d2inlT188`
the row is DIFFER(1884v1941), so 189 is the smallest value; and chunk 2038's row 0x00BB8750 regrades RELOCATION_MATCH
under this folder's `hdr/`, so this header set serves both rows.

## Review items for landing

1. **`/d2inlT189` (inline budget), with an `// Inline-budget:` line in the row.** At the default threshold (74) the only
   difference from retail is three helpers retail expands and VC7.1 calls: `CGraphicDataBank::PeekGraphic` (score 110 at
   static frequency 0.125), the `CRenderStateScopedCaptureBlock` constructor (119 at 0.0625) and
   `~CScopedShadersEnable` (188 at 0.0625). Plain C++ was tried first: the four exit tests each halve the frequency and
   `~CScopedShadersEnable` would need f >= 0.38; an early-return arrangement still refuses all three and changes the code
   (1875v1941). c2's other inline rule (callee defined after the row; the row as an inline virtual) was measured and is
   not retail's arrangement. The flag has to reach the catalog CompilerFlags.
2. **`SetupBlendOpState` and `SetupAlphaBlendingStatesFromBlendMode` are `FABLE_FORCEINLINE` bodies written in the
   row**, as `engine/CEnginePrimitiveRenderUtil.h` directs: the second costs 693, over c2's hard 450 inline cap, the
   first scores 484; as plain inline the row is 1296v1941. The parked hand742 row 0x00BBE090 writes the same bodies: if
   both land they should share one text. The scoped shader helpers stay plain inline (retail has uncalled out-of-line
   copies: ctor 0x009D8AD0, dtor 0x009D8B40).
3. **Headers** (`hdr/README.md`; additions and new files only, started from chunk 2038's overlay):
   `CEnginePrimitiveManagerParticleDecalGroup.h` (the RenderSubPrimitive declaration), `CEngineSubPrimitiveBase.h`
   `FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE` (`GetParentBase` / `PeekParentBase`: FableWin.pdb names only, the
   second source is retail's inline read at 0x00BB8F0A), `CGraphicInfo.h` (chunk 2037's copy plus
   `FABLE_RETAIL_CGRAPHICINFO_GETFRAME` and `FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC`; the PeekGraphic body equals
   retail's unreferenced copy 0x0041C1B0), new `CEnginePrimitiveManagerParticleGroup.h` (Ego_r layout).
4. **Name fixes** (`NAME_FIXES.tsv`, 8 addresses): 0x009FF8D0 `GetFrame` (row 0x00BCB800 binds a wrong signature),
   0x009FCBA0 `Uninitialise` (bound inside an invented `NUISystem` namespace by 0x0041B550), 0x00A05840
   `RestoreCaptureBlock` (five wrong spellings), 0x00987FE0 / 0x00988110 / 0x009880E0 / 0x00988190 and 0x0098B5E0 (bound
   to invented classes `CShaderEnableState` / `CShaderLayoutState` by 0x00B5E700 and 0x00BAD2E0). The vland warning is
   label debt: 0x01396DC8 is `CRenderStateManager::D3DTextureUVModeTable` in the header, labelled
   `g_textureUVModeToD3D`.
5. Global spellings follow the landed ones (`NEngineInternal::GPResourceManager`, `GPRenderManager`,
   `GPShaderRenderManager`, `g_pEnginePrimitiveTypeInfo`, `g_pEngineLightingManager`); same open spelling question as
   c2028 / c2037 / c2038. `GFDebugPDynamicCast` is defined in the row with `engine/CThing.h`'s exact text, as landed
   rows such as 0x00984D90 do. `PrepareForRender(1)` is the minimal value consistent with the flag bits retail tests.

## How to land

`rows/00bb8ef0.cpp` + `rows/00bb8ef0.test.cpp` with `hdr/engine/` (also serves 0x00BB8750 of c2038), the flag in
CompilerFlags, then `payload.json` through your usual vland.
