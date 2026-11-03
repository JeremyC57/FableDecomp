# Header needs left open by landing 2026-11-03c

What this landing installed is in `README.md`. Below are the header changes that are **not** installed, because no landed
row needs them. Each item names the parked row that will need it. A row that uses one of them must first be graded
against the installed headers. Every item goes into the class's existing `FABLE_RETAIL_<CLASS>` branch, with the default
path unchanged. Then run hdrcheck2 and recheck the includers.

## Chunk 728 rounds 1 and 2 (parked: DrawDebugInfo 0x0074BC20, GlobalUpdate 0x006D0830, FrameUpdate 0x006D1E30)
Source overlay: `pending/2026-11-03_linux_hand728/round2/hdr`.

**Installed unchanged:**
- engine/CTCWife.h
- engine/CTCCreatureGenerator.h
- engine/CTCShapeManager.h
- engine/CThingSearchTools.h
- engine/CThingPlayerCreature.h
- engine/CThingFilter_HasDefGlobalIndex.h
- engine/CPersistContext.h
- engine/CPersistContext_TransferableComponent.h
- stlport/fable_containers.h

engine/CDefinitionManager.h comes from chunk 731's superset, which declares the same two members.

**Installed without these hunks.** The rows that need them are still parked.

| Header | Hunks left out | Needed by |
|---|---|---|
| engine/CTCBase.h | `AddToGlobalTCs` 0x00686ED0, `GetPWorldSeed` 0x00686DA0 | GlobalUpdate |
| engine/CTCBase.h | `PeekComponent` 0x00686D60 and `class CMainGameComponent;` | DrawDebugInfo |
| engine/CPlayer.h | `RemoveMode(EPlayerMode)` 0x00634190 | GlobalUpdate |
| engine/CMainGameComponent.h | `PDebugFont` typed as `CCountedPointer<CFontBank>`, the `CCountedPointer.h` / `CFontBank` include, inline `PeekDebugFont()` | DrawDebugInfo |
| stlport/fable_map.h | `_Rb_global::_Rebalance_for_erase` 0x0042A34E, `_Rb_tree::erase(iterator)`, `map::erase(iterator)` | FrameUpdate |

Typing `PDebugFont` changes the FABLE_RETAIL_CMAINGAMECOMPONENT branch. Recheck its definers when it goes in.

**Not installed: GlobalUpdate / FrameUpdate.**
- Changed headers:
  - engine/CDisplayEngine.h, CGameCameraManager.h, CHeroMarriageDef.h, CHeroMarriageInfo.h
  - engine/CPlayerGui.h, CSubtitleRenderer.h, CTCBuyableHouse.h, CTCCreatureOpinionOfHero.h
  - engine/CTCHeroStats.h, CTCPerceiveThing.h, CTCPhysicsBase.h, CTCSoundPlayer.h, CTCVillage.h
  - engine/CThingAICreature.h, CThingBuilding.h, CThingCreatureBase.h, CWorld.h
  - fable_game.h
- New headers:
  - CGameTimeManager, CMessageEventManager, COpinionReactionManager, COpinionSet
  - CTCHeroOnlineScoreboard, CTCHeroOpinionDeedLog, CTCVolumeContainmentTracker
  - CWifeDef, NHeroInformationScreens_CManager

**Not installed: DrawDebugInfo.** The new CEnginePrimitive*.h headers: CEnginePrimitive, CEnginePrimitiveLine,
CEnginePrimitivePrepareInfo, CEnginePrimitiveText and CEnginePrimitiveThemeChannelDesc. Before they go in, choose one
home for CEnginePrimitiveLine: agent724b's overlay also has a CEnginePrimitive.h.

**engine/CIEngine.h.** Chunk 728's slots 17-23 are not needed. Chunk 731's slots 17-84 are installed, and their
signatures agree with 728's, including slot 23 SetPersistentPrimitive.

## Chunk 729 (parked: CTCInventoryMap marker builders)
- **CTCWife.** Its CTCWife model is superseded on size: the installed branch is the retail 0xEC class. Its two members
  IsMarriedToThePlayer 0x006CEC80 and IsAttractedToHero 0x006CFE80 are in the installed branch.
- **The rest of its overlay is not installed:** CDefString, CMiniMapDisplay, CPlayerGui, CQuestManager, CRegion,
  CTCBuyableHouse, CTCInventoryMap, CTCQuestCard, CTCShop, CThing, CThingCreatureBase, CThingSearchTools, CWorldMap,
  NInventory_CTCInventoryBase, fable_definition_table.h, fable_containers.h, and the new headers. Rebase it on the tree
  before its rows are retried.

## Chunk 727 (parked: sky ctor 0x00B625E0, shadow clip volumes)
- **engine/CEngineSkyRenderer.h.** 727's typed FABLE_RETAIL_CENGINESKYRENDERER branch is now installed, plus
  `PeekSceneFilterFlags() const`. Chunk 731's generated opaque version was dropped.
- **engine/CEngineComponent.h is not installed in 727's form, and the two models conflict.**

  | | Tree branch (pass 12, 0x00B3BE10) | 727's branch |
  |---|---|---|
  | Virtual slots | 13 (0..12) | 14: adds `SetEffectQuality(const CEngineVideoOptionsDef&)` as slot 13 |
  | `PeekSceneFilterFlags` | pure | has a body |
  | Ctor | `CEngineComponent()` | `CEngineComponent()` out of line (0x00B59710) |
  | Extra methods | none | `SetGlobalComponentPointer`, `SetupEnableConsoleVariable` |

  - Settle the slot count and whether `PeekSceneFilterFlags` is pure from the RTTI vtable 0x012A2544.
  - Then merge both into the one branch and recheck its definers (0x00B3BE10, the 731 component headers, 0x00B26360).
- **CEngineInit / CEnvironmentDef.** The 0x00B625E0 source must drop its local models and include the installed
  engine/CEngineInit.h (0x64) and engine/CEnvironmentDef.h (0x134). Its members +0x118..+0x128 go into the opaque block of
  CEnvironmentDef.h.
- **CDefPointer::SetDynamicCast.** The re-read decision is still open: `pending/2026-11-03_linux_hand727/DECISION_00b625e0.md`.
- **engine/CEngineShadowRenderer.h.** The installed branch is chunk 731's: `: CEngineComponent`, ctor 0x00B50370,
  PeekSceneFilterFlags, and an opaque `_pad_0xc[0x334]`. CEngine::Initialise needs the base for
  `layer << g_pEngineShadowRenderer`. 727's (and 723b's) base-less member subset must go into this branch, keeping the
  base:
  - SceneData (CShadowRendererSceneData) at +0x0C
  - the nested CColorBufferDepthPackParameters and SetupShadowFuncRenderStates 0x00B50510
  - the private CalcClipVolume / CalcSpotLightClipVolume / UpdateShadowSpace
  - RenderingTechnique +0x334, UserVideoOptionShadowBufferSize +0x338, UserVideoOptionShadowDistanceScale +0x33C

  Never write a second, base-less branch.
- **engine/CEngineCamera.h.** The installed branch is 731's: `: CEngineComponent`, `CEngineCamera(bool)` 0x00B31700,
  PeekSceneFilterFlags, opaque 0x21C. 723b/c and 727 type the members (same 0x228, same base). Those typed members go
  into this branch.

## Chunk 731 (parked: SetDefaultRenderStates 0x00B24850)
- It needs no further header. Its residue is the bookmark operand-order lane gap (GOTCHAS 2026-11-03).
- The 20 component classes are installed as opaque `_pad_0xNN` blocks with RTTI bases, ctors and retail `new` sizes. A
  later typed model replaces the opaque block inside the same branch.
- CEngineWorldMap is corrected to 0x1C0, so the old unproven GameWorld / StaticMapOpenState / CommonHeaderPosLookupTable
  offset asserts are gone.

## Chunk 723 (rounds 2 and 3)
Unchanged from `../2026-11-03_landing_b/HEADER_NEEDS.md`, with one exception: CEngineLightingManager now has a
FABLE_RETAIL_CENGINELIGHTINGMANAGER branch from 731, which holds 727's members plus SetupDefaultFogStates, the ctor and
the overrides. 723's lighting-manager changes go into that branch.
