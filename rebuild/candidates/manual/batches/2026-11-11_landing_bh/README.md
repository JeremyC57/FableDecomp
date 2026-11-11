# 2026-11-11 landing bh -- agentNF8: 24 definition-system binders rewritten onto the shared headers

Source: `naming/2026-11-11_agentNF8/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). vland --recheck-landed: 24 RELOCATION_MATCH,
tests PASS (18 new tests).
- GetDef<CThingBaseDef> 0x0042AF3C: 7 rows (CTCShop x4, CTCWeapon / CTCVillageMember / CTCSmashableBase OnCreate); 60 left.
- GetSubDef<CAppearanceDef> 0x0042B0A2: 10 rows (9 CThingCreatureBase anim rows, CTCLook::OnCreate); 28 left.
- CTCBase::GetDefinitionManager 0x006869D0: 13 rows (CTCHeroStats x4 now members, CTCInventoryAbilities x2, + the above); 44 left.
- CThing::PeekDefinitionManager 0x004C7990: 1 row; 38 left.
Renames: 0x0057A753 CTCHeroStats::OnHealthModified, 0x0083C1E0 CThingSwitch::PeekDefinition (both PDBs).
Headers (base d75e089bb, hdrcheck2 default=same): CTCShop, CTCHeroStats, CTCInventoryAbilities, CThingCreatureBase, CAnimationSet, CTCWeapon,
CThingSwitch opt-ins. reloc_bindings recomputed for the 24 rows; bindings_multi_check at baseline. Queued land1011bh: 27 name fixes.
Left: mostly CThing-family virtual overrides whose class headers do not exist yet (CThingBuilding, CThingAICreature, CTCExpression,
CTCMeleeTrap, CAnimateThing), header gaps (CInventoryItemDef StatGetTypeName, opaque CTCNoise members), unproven GetDef type arguments
(0x0083E0D0, 0x0042B144), Preload rows binding CMeshDataBank / CThing::GetWorld under mixed spellings, and the script-interface rows.
