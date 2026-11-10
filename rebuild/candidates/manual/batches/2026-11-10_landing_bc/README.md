# 2026-11-10 landing bc -- chunk 1021: 8 of 8 fresh header-backed rows

Source: `pending/2026-11-10_linux_hand1021/` (NOTES, STATUS, LESSONS, LABELS, NAME_FIXES, SPLITS, ATTEMPTS). vland --land: 8 wins
(RELOCATION_MATCH), tests PASS (g++ and VC7.1 /Od). No flags, no inline devices.
- 0x0076D2F0 CTCEnemy::AreThingsEnemies (static __fastcall; IS_THING_ALIVE kept out of line in this TU, opt-in FABLE_RETAIL_IS_THING_ALIVE_NOINLINE)
- 0x0066BC10 CTCSoundPlayer::GetPotentialCriteriaStrings, 0x00BEC680 CEngineInternalPrimitiveMeshBase::CollectStats,
  0x00A47D10 CNavigatorExternalInterface::RemoveAgentFromLists, 0x0083D460 CThingMarker::Create (247 bytes; catalog 336 split, BOUNDS_FIX),
  0x00A0C570 CCamera::SetAngleXY, 0x0079AA90 CTCSummonSpell::GetValidTeleportPos
- 0x006B3E60 is CEnvironment::SetWeightedThemes (both PDBs; the catalog placeholder CEnvironmentThemePriorityStackEntry_SetWeight and
  landed callers' SetCameraThemes are wrong -- queued).
Headers: base 6f904edf0, 3-way merged onto HEAD; three conflicts with Codex pass 19 additions (ThingAlive.h, C3DMesh2.h, C3DMeshInfo.h)
were independent insertions at the same spot and keep both; hdrcheck2 default=same; new engine/CEntitySoundDef.h,
engine/CEnvironmentThemePriorityStack.h. Queued land1011bc: 5 name fixes (AreThingsEnemies x5 callers, GetSubDef<CEntitySoundDef>,
invented Inner_/SubMesh_CollectStats, SetCameraThemes, C3DVector::AccurateNormalise 0x00A14440).
