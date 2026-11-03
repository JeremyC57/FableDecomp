# Header merge: chunks 718-725 (landing 2026-11-03)

The coordinator built one overlay from the current `rebuild/include` plus every chunk's `hdr/` (final round per chunk:
718 round3, 719 round4, 720 round5, 721 round4, 722 none, 723/724/725 base). That meant a three-way merge against each
chunk's git base, pruning the blocks that only parked rows need, and folding per-feature macros into one
`FABLE_RETAIL_<CLASS>` branch per class. It then copied the result into `rebuild/include`. The `// Layout:` and
retail-evidence comments are kept.

**Result:** 41 headers changed and 15 new.

**hdrcheck2:** `default=same` for 39 of the 41 changed headers. The two exceptions are expected and behave as they did
before, because both headers are retail-only and are never preprocessed without retail macros:
- `engine/CDefString.h` reports `cpp-err`.
- `engine/CThingManager.h` reports `DIFF`.

Every `FABLE_RETAIL_*` branch that differs is the intended retail change. The log is `logs/hdrcheck2_final.txt`.

## Includer recheck (default-path risk)
- **2,824** sources include a changed header, directly or transitively: 2,594 in `src/` and 230 in `tests/`.
- **2,792** of them preprocess token-identically against the old headers (`logs/ppcmp.tsv`). The check uses each
  file's own macros and a private stub directory placed last in `-I`.
- **32** differ. All 32 were regraded on the final overlay, and all 32 are `RELOCATION_MATCH PASS`
  (`includer_recheck_grades.tsv`, `logs/rcF_*.log`).

**Default-path risk:** none observed.

## New headers
| Chunk | Headers |
|---|---|
| 718 | `engine/C2DExtentsF.h`, `engine/CCamera.h` |
| 723 | `engine/CEngineLandscapeMeshBuilder.h`, `engine/CEnginePrimitiveManager.h`, `engine/CEnginePrimitiveRenderUtil.h`, `engine/CEngineWaterSettings.h`, `engine/CLandscapeGPUUpdateQueue.h`, `engine/CLandscapeTesselatorBase.h`, `engine/CPixelShader.h`, `engine/CShaderBase.h`, `engine/CShaderResource.h`, `engine/CVertexBufferBase.h`, `engine/CVertexCompressionParams.h`, `engine/CWaterPatchTesselatorBase.h` |
| 725 | `engine/NCameraModes_CBase.h` |

## Macros folded into the class branch (no longer exist as macros)
| Was | Now | Rows that need it |
|---|---|---|
| `FABLE_RETAIL_CTHING_VILLAGE` | `FABLE_RETAIL_CTHING` (GetTypeFlags, GetTCGraphicAppearance x2, GetPlayerNumber, SetInLimbo, HasScriptThing, SetPersistOnUnloadWhenInLimbo) | 720 0x0065B854 |
| `FABLE_RETAIL_CTCVILLAGE_MEMBERS` | `FABLE_RETAIL_CTCVILLAGE` | 720 0x0065B854 |
| `FABLE_RETAIL_CWORLD_GAMETIME` | `FABLE_RETAIL_CWORLD` (PeekGameTimeManager, GetGameTimeManager, PeekMyPlayer, GetGameCamera, PeekGameCamera) | 718, 720 |
| `FABLE_RETAIL_CMAINGAMECOMPONENT_WF` | `FABLE_RETAIL_CMAINGAMECOMPONENT` (ConvertSecondsToWF) | 718 |
| `FABLE_RETAIL_CTHINGCREATUREBASE_PROPERTY` | `FABLE_RETAIL_CTHINGCREATUREBASE` (GetProperty) | 720 |
| `FABLE_RETAIL_CTHINGMANAGER_PEEK` | unconditional `PeekPThingWithUID` (`CThingManager.h` is retail-only) | 718 0x0089E190 |
| `FABLE_RETAIL_CDEFSTRING_GETSTRING` | single `CDefString` model: `operator CCharString()` forwards to `GetString()` (see below) | 720 0x0065B854 + includers |
| `FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR` | the tree's `FABLE_RETAIL_CINTELLIGENTPOINTER` branch already defines `CIntelligentPointer(T*)` (header unchanged; the define was removed from the row) | 720 0x0065B854 |
| `FABLE_RETAIL_CRENDERMANAGER2D_TEXT` | `FABLE_RETAIL_CRENDERMANAGER2D` (Draw2DText) | 718 |
| `FABLE_RETAIL_CVILLAGETASKPATROL_CTOR`, `..._PICKCROPS_CTOR` | `FABLE_RETAIL_CVILLAGETASKPATROL`, `FABLE_RETAIL_CVILLAGETASKPICKCROPS` | 720 |

The row sources were edited to match (`logs/src_edits.json` and the DROP/REN tables of the coordinator's `mkpay.py`).
The chunk-local global spellings were respelled to the tree's names:
- `g_pPrimitiveRenderer_01436e80` → `g_pEnginePrimitiveRenderer`
- `g_pShadowRenderer_01436e60` → `g_pEngineShadowRenderer`
- `g_pSurfaceComposition_01436eac` → `g_pEngineSurfaceCompositionManager`
- `g_pPrimitiveAttachmentManager_01436e70` → `g_pEnginePrimitiveAttachmentManager`

## Opt-ins kept separate (with the reason)
Each of these is a per-TU code-generation choice, not a class model. Folding any of them into the class branch would
change bytes of rows that are already landed.
- **`FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE`** (`fable_string.h`): whether `operator==` is out of line depends on the TU.
  Landed `FABLE_RETAIL_CCHARSTRING` rows inline it.
- **`FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND`** (`CVectorMap.h`): only the size-optimised TU forces `find` inline.
- **`FABLE_STLP_OUTOFLINE_UNINIT_FILL_N`** (`stlport/fable_containers.h`): TU-level noinline for row 0x00501150.

## Model decisions taken at the merge
- **`CDefString`:** the merge keeps one model. `operator CCharString() const { return GetString(); }` and `GetString()`
  read `GDefStringTable`. Evidence:
  - Retail 0x00415D70 (`??B`) and 0x00415DD0 (`GetString`) have identical bodies.
  - 0x00775C60 inlines both.
  - 0x0065B854 inlines the forwarder and calls `GetString`.

  The chunk's GETSTRING-only model made landed 0x00775C60 DIFFER(1148v1162). The tree's model gave a `??B`
  RELOC_CONFLICT in 0x0065B854. The forwarder model passes both, and every `CDefString` includer passes.
- **`CEngineResourceManager.h`:** the merge keeps the tree's retail model and adds chunk 722/723's typed members, accessors
  and forward declarations: `VShaderResources[27]`, `ShaderResourcePixel`, `GetIndexBufferMemPool`,
  `GetIndexBufferUsage`, `GetVertexBufferUsage`, `GetVShaderResource`, `GetPShaderResource` and
  `GetGPUScratchBufferTextureManager`. Chunk 723's second, independent model of the class was dropped.
- **`CEngineComponent.h`:** the tree is kept. Chunk 723's model had 14 virtual slots, but retail vtable 0x012A2544 has 13
  and slot 10 is `_purecall`.
- **`CTCCreatureNavigation.h`:** chunk 725's members are kept. Chunk 720's `StatGetInterfaceType` block (for a parked row)
  made the 725 rows fail to compile (`ETCInterfaceType` undeclared), so it was removed.
- **`C3DBoundingBox.h`:** the retail branch gains `CalcShortestDistanceToPointSquared`, `PeekMinCoord()` and
  `PeekMaxCoord()` (FableWin inlines). Row 0x00B46280 was moved off the private `MinCoord`/`MaxCoord`.
- **`C2DVector.h`:** the retail branch adds `GetMagnitude` (`sqrtf`) and `<math.h>`. Chunk 719's
  `C2DVector(const C3DVector&)` and its include were dropped, because only parked row 0x00509A20 needs them.

## Deferred (pruned from the merge; only parked rows need them)
These macros' chunk additions stay in the chunks' `hdr/` directories (see `drop_macros.txt` in the coordinator
scratch). Bring them in when the rows they serve match:
- **0x00509A20 parts:**
  - `C2DLineF.h`, `CNavigation*`, `CThingFilter_*` and `CTCGraphicAppearance`
  - `CFilter_Bind3`
  - the READERS additions to `C2DBoxI`, `C2DExtentsI`, `CStringParser` and `CDataInputStream`
  - `CWorldMap` 0x00509A20 parts
  - `C2DVector(const C3DVector&)`
- **`CMap.h`:** chunk 719's file is independent of the tree's version.
- **Chunk 723's `C3DTriangle2.h`.**
- **`CTCPhysicsBase.h`:** a vtable overload reorder for 0x0088EDC0.
- **`CThingPhysical.h`, `CANavQuadTreeNode`, `CVillageTaskSecuritySweep`.**
- **Chunk 719's `fable_string` additions.**
- **Macros:** `C3DVECTOR_NORMALISE`, `STLP_DEQUE`, `STLP_BACK_INSERTER`, `STLP_OUTOFLINE_UNINIT_COPY`,
  `STLP_LIST_MEMBERS` and `CTCHEROEXPERIENCE(_STATS)`.

## Debt
- `fable_gfuninitialise.h` still carries a second, stub `CEnginePrimitiveManager` model. The new
  `engine/CEnginePrimitiveManager.h` is the real one. Unify them when a row includes both.
