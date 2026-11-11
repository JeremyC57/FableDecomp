# agentNF9 header overlay

Grade every row in `../rows/` with `FABLE_HDR_OVERLAY=$S/w22/agentNF9/hdr`. The changed repo headers are full copies of the current
repo files with `#ifdef` additions only: `hdrcheck2.py` reports `default=same` and `same` for every existing macro on each of them
(only the new macros differ). No landed row defines a new macro. Six headers are new (marked NEW).

## Changed headers

| Header | New opt-in | What it adds | Evidence | Rows |
|---|---|---|---|---|
| `engine/CThingBuilding.h` | `FABLE_RETAIL_CTHINGBUILDING_DEFINITION` | The inline protected `PeekDefinition() const` (fills a `CDefPointer<const CThingBuildingDef>` and returns `*def`). Declarations: `GetCurrentGraphic`, the virtuals `OnCreate` (MAE), `PeekPMaterial` (MBE), `GetMaterialOnHitByMelee` / `GetFloorMaterial` / `UseHighDetailQuadTree` (UBE), plus `PeekPCameraManagerSetDefinition` and `GetReverbSettings(EReverbEnvironmentType&, float&) const`. Includes CThingBuildingDef.h and CGameDefinitionManager.h | Ego_r.pdb / FableWin.pdb `@CThingBuilding@@` spellings. Ego_r donor order 0x00558D00.. maps to retail 0x0082D2D0..0x0082D8C0. vtable 0x0124509C slots 8 / 40 / 57 / 58 / 70. Ego_r has PeekDefinition out of line, but Fable.exe has no copy (every row expands it) | 0082D2D0 0082D5C0 0082D6B0 0082D6F0 0082D7D0 0082D870 0082D8C0 |
| `engine/CThingGameObject.h` | `FABLE_RETAIL_CTHINGGAMEOBJECT_ONCREATE` | protected virtual `OnCreate()` | `?OnCreate@CThingGameObject@@MAE_NXZ`; vtable 0x01244F7C slot 8 = 0x00838A40 | 0082D6F0 |
| `engine/NGraphicAppearance_CTCGraphicAppearance.h` | `FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC` (inside `_MOVEMENT`) | Inline `SetMainGraphic(const CEngineGraphic&)` and `SetScale(float)` (the latter only when `_SCALE` is not defined). Under this macro the MOVEMENT block also takes CEngineGraphic from the shared `engine/CEngineGraphic.h` instead of the quarantine copy, so a row can include a def header that embeds the type | `?SetMainGraphic@..@@QAEXABVCEngineGraphic@@@Z` is inline in retail: a 14-byte copy to +0x44, 0x0082D72A. `?SetScale@..@@QAEXM@Z` is at 0x004BFA50 | 0082D6F0 |
| `engine/CThingPhysical.h` | `FABLE_RETAIL_CTHINGPHYSICAL_SERIALISE` | public virtual `SerialiseGameState(CPersistContext&)` | `?SerialiseGameState@CThingPhysical@@UAE_NAAVCPersistContext@@@Z`; vtable 0x01244E84 slot 3 = 0x006A5AF0 | 007030C0 0083BB40 006628E0 |
| `engine/CThingObject.h` | `FABLE_RETAIL_CTHINGOBJECT_DEFINITION` | Types `PDef` as `CDefPointer<const CThingObjectDef>` (same 4 bytes at +0xD0). Inline private-to-class `PeekDefinition() const { return *PDef; }`. Virtuals `SerialiseGameState` / `ConstructFromLevelScript(const char*)` (MAE) and `PeekPMaterial` (MBE) | Ego_r member type and spellings. vtable 0x01264DCC slots 3 / 4 / 58. Retail reads PDef in place (0x00703150) | 00703050 007030C0 00703110 00703150 |
| `engine/CThingHolySite.h` | `FABLE_RETAIL_CTHINGHOLYSITE_DEFINITION` | Types `Def` as `CDefPointer<const CThingHolySiteDef>`. Private inline `PeekDefinition`. Protected `ConstructFromParams(long, const C3DVector&, long, bool)`. Virtuals `SerialiseGameState` and `ConstructFromLevelScript(const char*)` (UAE) | Ego_r `@CThingHolySite@@` (PeekDefinition is ABE, private). vtable 0x012452DC slots 3 / 4 | 0083BAB0 0083BB40 0083BBA0 |
| `engine/CThingCreatureBase.h` | `FABLE_RETAIL_CTHINGCREATUREBASE_CONSTRUCT` | protected `ConstructFromParams(long, const C3DVector&, long, CThingPhysical*, CThingPhysical*, const CCharString&, bool)` and the virtual `SerialiseGameState` | Ego_r / FableWin spellings. Donor order ConstructFromParams / SerialiseGameState / ConstructFromLevelScript = retail 0x00662880 / 0x006628E0 / 0x00662930 | 00662880 006628E0 |
| `engine/CThingCreatureBase.h` | `FABLE_RETAIL_CTHINGCREATUREBASE_ANIMEXISTS` | public const virtual `AnimExists(const CCharString&, bool)` | `?AnimExists@CThingCreatureBase@@UBE_NABVCCharString@@_N@Z`; slot 73 | 00662CC0 |
| `engine/CThingAICreature.h` | `FABLE_RETAIL_CTHINGAICREATURE_ANIMEXISTS` | the protected override `AnimExists` | `?AnimExists@CThingAICreature@@MBE_NABVCCharString@@_N@Z`; vtable 0x0127293C slot 73 = 0x00830F20 | 00830F20 |
| `engine/CThingDeadCreature.h` | `FABLE_RETAIL_CTHINGDEADCREATURE_PEEKDEFINITION` | protected `PeekDefinition() const`, declaration only | `?PeekDefinition@CThingDeadCreature@@IBEABVCThingCreatureDef@@XZ`. Out of line at 0x00835B90; callers 0x00835FD0 and 0x00836260 | 00835B90 |
| `engine/CThingPhysicalSwitch.h` | `FABLE_RETAIL_CTHINGPHYSICALSWITCH_PEEKDEFINITION` | protected `PeekDefinition() const`, declaration only | `?PeekDefinition@CThingPhysicalSwitch@@IBEABVCThingPhysicalSwitchDef@@XZ`. Out of line at 0x0083D900. Its only callers are TriggerMe 0x0083DDC0 and UntriggerMe 0x0083DF20. It calls GetDef<CThingPhysicalSwitchDef> 0x0083E0D0 | 0083D900 |
| `engine/CThingMarker.h` | `FABLE_RETAIL_CTHINGMARKER_DEFINITION` | inline protected by-value `PeekDefinition()`. Declarations: `GetCurrentGraphic() const` (by value) and the public const virtual `GetProperties()` | Ego_r `?PeekDefinition@CThingMarker@@IBE?AV?$CDefPointer@$$CBVCThingMarkerDef@@@@XZ`, `?GetCurrentGraphic@CThingMarker@@IBE?AVCEngineGraphic@@XZ`, `?GetProperties@CThingMarker@@UBEKXZ` (slot 48) | 0083D190 0083D280 |
| `engine/CThingVillage.h` | `FABLE_RETAIL_CTHINGVILLAGE_DEFINITION` | Types `CurrentGraphic` as `mutable CEngineGraphic`. Inline protected by-value `PeekDefinition()`. Declares `GetCurrentGraphic() const` returning a pointer | Ego_r `?PeekDefinition@CThingVillage@@IBE?AV?$CDefPointer@$$CBVCThingVillageDef@@@@XZ` and `?GetCurrentGraphic@CThingVillage@@IBEPBVCEngineGraphic@@XZ`. The const getter stores CurrentGraphic, which is why it is `mutable` (no layout or mangling effect) | 0083B520 |
| `engine/C3DAnimationInfo.h` | `FABLE_RETAIL_C3DANIMATIONINFO_LOADING` (in the generated default branch) | `TouchAnimation() const`, `BeginLoading(bool) const` | `?TouchAnimation@C3DAnimationInfo@@QBEXXZ` 0x00A26A50 and `?BeginLoading@C3DAnimationInfo@@QBEX_N@Z` 0x00A26730 (the retail model in the same header declares both) | 00663140 006631C0 00663610 |
| `engine/CAnimationSet.h` | `FABLE_RETAIL_CANIMATIONSET_GETNOANIMS` | `GetNoAnims(const CCharString&, const CAnimFilterBase&) const` | Ego_r `?GetNoAnims@CAnimationSet@@QBEJABVCCharString@@ABVCAnimFilterBase@@@Z`. Catalog row 0x005DC290 has the same name. Donor order GetNoAnims / AnimExists / GetAnim | 006631C0 00663610 |
| `engine/CAnimationEntry.h` | `FABLE_RETAIL_CANIMATIONENTRY_GETFLAGS` | `GetFlags() const` | `?GetFlags@CAnimationEntry@@QBEJXZ` 0x005DC210 | 00663140 |

## New headers

| Header | Content | Evidence |
|---|---|---|
| `engine/CThingBuildingDef.h` | Behind `FABLE_RETAIL_CTHINGBUILDINGDEF`: `CThingBuildingDef : CThingBaseDef` with DieOffTimer +0x70, SimBuildingDef +0x74, Graphic +0x78, WallMaterial +0x88, FloorMaterial +0x8C, ReverbEnvironmentType +0x90, ReverbEnvironmentLevel +0x94, OutsideOcclusionLevel +0x98, CameraManagerSetIndex +0x9C and the four bool:1 at +0xA0. sizeof 0xA4. Also an enumerator-less `enum EReverbEnvironmentType`. Without the macro it includes the quarantine copy. Its guard differs from the quarantine guard, as in CSimBuildingDef.h | Ego_r struct_layouts at PDB offset - 4. The rows' reads are listed in the header |
| `engine/CThingHolySiteDef.h` | Graphic +0x70, PlayerStart +0x7E; sizeof 0x80 with the typed CThingBaseDef | Ego_r layout - 4. Retail reads `[eax+7Eh]` |
| `engine/CThingPhysicalSwitchDef.h` | TriggerType +0x70, UntriggeredGraphic +0x74, TriggeredGraphic +0x82, TriggerSound +0x90; sizeof 0x94. Enumerator-less `EPSwitchTriggerType` | Ego_r layout - 4. The landed TriggerMe reads +0x90 |
| `engine/CThingMarkerDef.h` | Graphic +0x70, TriggerType +0x80 (`ESwitchTriggerType`), Properties +0x84; sizeof 0x88 | Ego_r layout - 4. GetProperties reads +0x84 |
| `engine/CThingVillageDef.h` | Graphic +0x70 | Ego_r layout - 4. 0x0083B520 copies from +0x70 |

All def headers derive from the RTTI base (`rtti_bases.tsv`: CThingBaseDef@0). The sizeof assertions are made only under
`FABLE_RETAIL_CTHINGBASEDEF`, because the opaque default CThingBaseDef is 1-aligned.

## Virtual slots proven along the way (for the CThing slot 29-86 gap)

No slots were added to CThing.h. Declaring a partial run would break slot order. The overrides above are declared in the derived
headers, which is enough for the rows that define them (they do not call them through the vtable). These slot / name pairs are now
proven: CThing slot 40 = `GetFloorMaterial() const` (`?GetFloorMaterial@CThing@@UBEJXZ`, overridden at 0x0082D7D0). CThingPhysical
slot 57 = `UseHighDetailQuadTree() const` and slot 58 = `PeekPMaterial() const` (`?..@CThingPhysical@@UBE..`, base bodies 0x005297C0 /
0x005297F0). CThingGameObject slot 70 = `GetMaterialOnHitByMelee() const` (`?GetMaterialOnHitByMelee@CThingGameObject@@UBEJXZ`, base
0x005298B0). CThingCreatureBase slot 73 = `AnimExists(const CCharString&, bool) const`. CThingMarker slot 48 = `GetProperties() const`.
