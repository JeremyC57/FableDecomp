# 2026-11-11 landing bi -- agentNF9: CThing-family headers completed, 28 definition-system binders moved onto them

Source: `naming/2026-11-11_agentNF9/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). vland --recheck-landed: 28 RELOCATION_MATCH, tests PASS.
Headers (opt-ins only, base a8e206b63, hdrcheck2 default=same): CThingBuilding, CThingObject, CThingHolySite, CThingCreatureBase,
CThingAICreature, CThingDeadCreature, CThingPhysicalSwitch, CThingMarker, CThingVillage, CThingGameObject, CThingPhysical,
NGraphicAppearance_CTCGraphicAppearance, C3DAnimationInfo, CAnimationSet, CAnimationEntry (real def-handle members, inline PeekDefinition,
PDB methods); new CThingBuildingDef / CThingHolySiteDef / CThingPhysicalSwitchDef / CThingMarkerDef / CThingVillageDef (Ego_r offsets - 4).
CThing slots 29-86 still not modelled (the proven slot names are in hdr/README.md).
Rows: CThingBuilding 7 (renames GetMaterialOnHitByMelee, UseHighDetailQuadTree, GetReverbSettings), CThingObject 4, CThingHolySite 3,
CThingCreatureBase 7 (incl. the Preload rows), CThingAICreature / CThingDeadCreature / CThingVillage 1 each, CThingMarker 2,
CThingPhysicalSwitch 0x0083D900 (renamed PeekDefinition; was labelled CThingDeadCreature), NUISystem::CManager::Action 0x0041E6D3
(two def-handle types were swapped). Settled: 0x0083E0D0 GetDef<CThingPhysicalSwitchDef>, 0x0042B144 GetDef<CDefClassBase>.
Left: PeekDefinitionManager 16, GetSubDef<CAppearanceDef> 21, GetDef<CThingBaseDef> 59, GetDefinitionManager 44 (CTC component rows);
0x0072F170 CTCDummyVillager::SetupFromData still binds GetDef<CDefClassBase> at 0x0042AF3C (full local model). Queued land1011bi: 21 fixes.
