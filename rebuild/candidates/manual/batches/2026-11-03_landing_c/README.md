# Landing 2026-11-03c: chunks 728, 728 round 2 and 731 (8 rows), plus 2 name-fix re-landings

## Result
- **8 rows landed.** Each is RELOCATION_MATCH with tests PASS under `vland --land` on the final tree (`STATUS.tsv`,
  `logs/land728.log`, `logs/land731.log`).
  - **Chunk 728:**
    - 0x0074B770 CTCCreatureGenerator::InitialiseCreatureGenerator
    - 0x00C267E0 CStreamLoadHandler::Process (libpk, /EHsc, descriptive name; see Names)
  - **Chunk 728 round 2:** 0x006D2BD0 CTCWife::OnSerialise
  - **Chunk 731:**
    - 0x00B573A0 / 0x00B57860 / 0x00B57D20 / 0x00B581E0 CEngineStateBlock{Alpha,Alpha2X,AdditiveAlpha,AddSmoothAlpha}::Apply
    - 0x00B26360 CEngine::Initialise
- **2 rows re-landed** for declaration spellings proven by both PDBs. Both are RELOCATION_MATCH (`logs/land_resp.log`).
  - 0x0074BAA0 CTCCreatureGenerator::UpdateCreatureTrigger: `CreateCreatures` is protected (IAE).
  - 0x00B25C00 CEngine::RenderColorFilteredQuad: `SetDefaultRenderStates` is private (AAE).
- **Held: none.**
  - The chunks' parked rows were not part of this landing: 0x0074BC20, 0x006D0830, 0x006D1E30 and 0x00B24850.
  - `HEADER_NEEDS.md` lists the headers they still need.
- **Checks:**
  - `bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` prints 0.
  - Coverage: 46,539 genuine (93.90% of the catalog); 5,701,106 bytes (55.74% of retail function bytes). Before this
    landing it was 46,531 (93.88%) and 55.61%.
  - Landed rows: 54,077 → 54,085.

## Procedure
1. Built one stage overlay from three sources:
   - chunk 731's `hdr/`;
   - chunk 727's typed CEngineSkyRenderer.h, plus `PeekSceneFilterFlags`;
   - the chunk 728 round 2 headers in the closure of the landed rows, with the parked-row hunks taken out (`HEADER_NEEDS.md`).
2. Copied it over `rebuild/include` into a full mirror and graded the 8 rows there. All were RELOCATION_MATCH + PASS
   (`logs/g728.log`, `logs/g731.log`).
3. Ran hdrcheck2 on the stage (`logs/hdrcheck2_final.txt`). Each of the 43 changed headers prints `default=same` with a
   non-empty line count; 10 headers are new.
4. Rechecked the includers (`includer_recheck_grades.tsv`). 3,236 landed sources and tests reach a changed header:
   - 3,138 preprocess token-identically (`logs/ppcheck.txt`).
   - The other 98 were run through `vland --recheck-landed` on the full mirror, which is identical to the final
     `rebuild/include` (`diff -rq` empty). 94 are RELOCATION_MATCH and 4 MATCH, all PASS (`logs/rc*.log`).
   - The 98 include the STLP_MAP / STLP_VECTOR definers, the CTCBase / CMainGameComponent / PLAYER / CWorld branch
     definers, 0x009DA9F0, 0x00B3BE10, 0x00B67920, 0x00B71880 and 0x0121E210 (the CTCWife static initialiser).
5. Installed the stage, then ran `fixmove.py`, `keypayload.py` (0 texts changed) and `vland --land`.
6. Ran `repair_remove.py` and `vland --land` for the two re-landings, then `git reset` on the staged deletes.

## Headers (one retail model per class)
**Changed (43).**
- **Chunk 731:**
  - CDefaultEngineMemoryInit, CDefinitionManager, CDisplayManager, CEngine (FABLE_RETAIL_CENGINE, sizeof 0x178)
  - CEngineBillboardGenerator, CEngineCamera, CEngineEnvironment, CEngineLandscapeRenderer, CEngineLightingManager
  - CEngineOcclusionManager, CEnginePrimitiveAttachmentManager, CEnginePrimitiveOcclusionManager
  - CEnginePrimitiveRenderer, CEnginePrimitiveRenderer2D, CEnginePrimitiveTypeInfo, CEngineResourceManager
  - CEngineScreenEffect{ColourFilter,DisplacementRenderer,GlowRenderer,OutlineGlow,RadialBlurRenderer}
  - CEngineShadowRenderer, CEngineSimulationManager, CEngineStateBlocks, CEngineSubPrimitiveRenderer
  - CEngineSurfaceCompositionManager, CEngineWeatherRenderer, CEngineWorldMap (0x1C0), CIEngine (slots 17..84)
  - CRenderManagerCore, CRenderStateManager
- **Chunk 727:** CEngineSkyRenderer.
- **Chunk 728 / 728 round 2:**
  - CMainGameComponent, CPersistContext, CPlayer, CTCBase, CTCCreatureGenerator (FABLE_RETAIL_CTCCREATUREGENERATOR)
  - CTCShapeManager, CTCWife (FABLE_RETAIL_CTCWIFE, 0xEC), CThingPlayerCreature, CThingSearchTools
  - stlport/fable_containers.h (vector::reserve), stlport/fable_map.h (FABLE_RETAIL_STLP_SET begin/end/size)

**New (10).** CEngineDef, CEngineInit, CEngineLayerRenderer, CEngineLocalDetailGenerator, CEngineMemoryInit,
CEnginePrimitiveRendererSharedData, CEngineVideoOptionsDef, CEnvironmentDef, CPersistContext_TransferableComponent and
CThingFilter_HasDefGlobalIndex.

**Model decisions per contested class:**

| Class | Model installed | Why / what was set aside |
|---|---|---|
| CTCWife | Round 2's single FABLE_RETAIL_CTCWIFE branch: CTCBase-derived, sizeof 0xEC | Alloc 0x004D32A7 `push 0ECh` and the ctor stores. Chunk 729's model is superseded on size; its IsMarriedToThePlayer / IsAttractedToHero are in the branch. |
| CEngineSkyRenderer | Chunk 727's typed branch (0x194, ctor 0x00B625E0, ~CEngineSkyRenderer, private BuildBuffers) plus `PeekSceneFilterFlags() const` | 731's opaque generated branch was dropped. 0x00B26360 matches with the typed one. |
| CEngineShadowRenderer | Chunk 731's branch with its RTTI base CEngineComponent (ctor 0x00B50370) | CEngine::Initialise needs the base for `layer << g_pEngineShadowRenderer`. 727's / 723b's base-less member subset goes into this branch later (`HEADER_NEEDS.md`). |
| CEngineCamera | 731's opaque branch, `: CEngineComponent`, `CEngineCamera(bool)` 0x00B31700 | 723 / 727's typed members (same base, same 0x228) go into it later. |
| CIEngine | 731's slots 17..84 | Superset of 728's 17..23, with the same signatures. |
| CDefinitionManager | 731's superset | GetDef(name, index, def), GetDef(name, def), GetPDefFromInstantiationName, GetPDefFromClassIndex. Round 2's two members are among them. |
| CEngineLightingManager | 731's branch | Chunk 727's members plus SetupDefaultFogStates, the ctor and the overrides. |
| CEngineWorldMap | Retail branch corrected to 0x1C0 | The `push 1C0h` before the ctor. The unproven asserts after MapList were dropped; landed 0x00B67920 is still RELOCATION_MATCH. |
| CRenderStateManager | 731's forceinline texture-op / arg / fog / dither / lighting / anisotropy / transform-mode setters replace the declarations in the retail branch | No out-of-line copy in Ego_r. Every includer was rechecked. |
| CEngineComponent | Unchanged (tree branch) | 727's 14-slot form is not installed (`HEADER_NEEDS.md`). |

**Trimmed from 728 round 2 because only parked rows use them:**
- CTCBase: AddToGlobalTCs, GetPWorldSeed, PeekComponent.
- CPlayer: RemoveMode.
- CMainGameComponent: the typed PDebugFont and PeekDebugFont.
- fable_map.h: erase and _Rebalance_for_erase.
- Not installed at all: CWorld.h, CThingCreatureBase.h and the 25 other GlobalUpdate / FrameUpdate headers.

## Names (`NAME_DECISIONS.tsv`)
**Catalog and manifest.** Names for the 8 rows come from their Leafs, through:
- `ghidra_out/labels_landing_2026-11-03c.tsv` (21 labels: the 8 rows plus 13 targets that both PDBs name and no landed
  Leaf or source contradicts);
- `apply_labels.py`, `sync_catalog_names.py` and `sync_manifest_from_catalog.py --labels`.

The net manifest change is 9 rows. The 473 bracket-spacing rewrites from `sync_catalog_names` were reverted by the
manifest sync.

**0x00C267E0.** The spelling evidence was checked before landing:
- **The row's own name.** No PDB, RTTI or trace string names this class. Nothing else binds 0x00C267E0, and the catalog
  label was a propagated wrong name. So the descriptive `CStreamLoadHandler::Process(CPooledResource*)` lands as is.
- **Its callees.** Their descriptive spellings disagree with older, equally unproven spellings:
  - The owner class is CPooledResourceManager here, but CSoundEventManager in the catalog and the FinishLoading Leaf,
    and CPanel in 0x00C22080.
  - The bank is CStreamSoundBank here, but CScriptHost in the catalog and the 0x00C0E870 Leaf.
  - 0x00C26550 is a destructor here, but a CBase ctor in its landed row.
- **Decision.** No landed row was respelled toward another invented name. 9 c728 REVIEW lines ask for one libpk naming
  wave.

**Queue.** 40 lines were added to `../../naming/NAME_FIXES_QUEUE.tsv`:

| Tag | PRIORITY | REVIEW | Note |
|---|---|---|---|
| c728 | 6 | 9 | 2 PRIORITY lines are closed (`done]`) |
| c728r2 | 10 | 3 | |
| c731 | 10 | 2 | |

The open queue line 529 (0x00B24850) was closed.

**Examples the brief named:**
- 0x00A02D20 `??0CHandleBasedMemoryPool@@QAE@XZ` is queued PRIORITY. Landed 0x00A148D0 must be rebased off its local
  `CEnginePrimitiveTypeInfo` base. Until then vland prints RELOC_CONFLICT for 0x00B26360.
- 0x009ADE10 GetPDefFromClassIndex is queued PRIORITY. It is not labelled, because the landed source defines
  GetPDefFromGlobalIndex.
- 0x00B25C00 SetDefaultRenderStates was respelled and re-landed.
