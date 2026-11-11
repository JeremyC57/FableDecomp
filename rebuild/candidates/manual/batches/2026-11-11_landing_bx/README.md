# 2026-11-11 landing bx -- agentNF16: 335 landed rows respelt to proven callee names (Codex pass 20 queue + NF15 claims)

Source: `naming/2026-11-11_agentNF16/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES / hdr README). vland --recheck-landed in three
parts: 335 WINS (1 MATCH, 334 RELOCATION_MATCH), VC7.1 tests PASS with their catalog PassPatterns. g++ PREFAIL on 81 rows is pre-existing
(STATUS.tsv). Bindings re-derived from the recheck objects (rebind.py); bindings_multi_check unchanged (NullCoord baseline only).
- Largest targets: 0x013BA848 NThingMap::MaxLayers (86 rows, was g_NumMapwhoLevels), 0x008FCFC0 CAIStateGroupBase::GetWorld (55),
  0x0070C050 CAnimInit::Init (22, was Set), 0x0049D850 CWorld::GetPlayer (19), 0x00669D30 CSpecialEffectsDef::GetEffectFromString (19),
  0x009D49B0 CDefStringTable::GetString (18), 0x007446A0 CTCPerceiveThing::GetPerceivedThing (13), CPlayer HasMode / RemoveMode / AddMode
  with enum EPlayerMode (13), CPlayerGui singleton PClass (12), IS_THING_ALIVE<CThing*> (9), PeekPDeedEffectsDef (7), 0x0040F020 /
  0x004365B0 CVectorMap LowerBound (15 + 16).
- Own names: 0x006AE1C0 renamed CDefinitionManager::GetDef_006ae1c0 -> GetDef<CPlayerMovementDef> (member template, Ego_r + FableWin);
  nine more rows got their PDB signatures (virtual / access / CThing&) under names they already carried (RENAMES section 1).
- Header: engine/CCreatureActionBase.h gains opt-in FABLE_RETAIL_CCREATUREACTIONBASE_GETTHINGMANAGER (0x00692980); default path unchanged.
- New name fixes queued as land1011bx (4): 0x00692910 PeekDefinitionManager, 0x00842150 DropGenericBox ctor, 0x008E5670
  CTCPhysicsStandard::PerformForceUpdates, 0x004874D0 IsFirstPersonTargeting.
- Pre-existing RELOC_CONFLICT warnings seen in the recheck (not introduced here): g_MapwhoLevels 0x0137DADC v 0x0137DAD8 (element +4 folded
  into the symbol), NUISystem::CAction() 0x0042BED4 v 0x0042BE50 (0x005907AB), CTCDependents::AddDependentThing 0x0080CBF0 v 0x006AEBA0
  (0x008DC900) -- queued for agentNF17.
