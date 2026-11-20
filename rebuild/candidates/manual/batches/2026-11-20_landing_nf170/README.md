# Landing nf170 (Account A, 2026-11-20): def class / persist / search tools names (22 rows)

Agent NF170 (round 10) handed back early (wide slice) with 27 entries: 11 done, 5 done-already, 1 rejected, 3 set aside for a
lock, 7 open after census. Its files are in `naming/2026-11-20_agentNF170/`. The 10 unfinished entries go back to the queue
unclaimed, each with its binder list in APPLIED.

- 22 rows:
  - 12 GetSubDef binders moved onto engine/CDefClassBase_GetSubDef.h.
  - 3 GetSubDef owners changed from strong explicit specializations to the shared body plus the nf135 helper scaffolding.
    NF170's link proofs exit 0 (`logs/agent_link_proofs/`).
  - 4 search-tool owners re-authored on engine/CThingSearchTools.h, with self renames to their template names: GetAllThings
    0x008E3BB0, GetPBestThing 0x00964230 / 0x009142B0, GetAllLeafThings 0x00964EF0.
  - 3 Transfer<T> self renames to the PDB enums: EPlaceInHeroSouls 0x00CDCDD0, EGameflowPosition 0x00CDCA70,
    EFeatAttackType 0x00F2D4D0.
  - 0x005EA400's +0x10C field now uses the Ego_r name MoneyBagGraphic.
- Headers (add-only, guarded, `orig` equals HEAD, existing asserts line-pinned): CCombatAbilityStrafeDef.h and
  CCombatAbilityUseProjectileWeaponDef.h (STATGETTYPENAME), CTavernGameDef.h (PRIZESCORES_VECTOR).
- q11675 rejected: the proposed QAE_NAAV form is in neither PDB; both have QBE_NPAV$$CB.
- Recheck (`logs/recheck.log`): 22 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created.
  tablecheck: 0x005ED4A0 6 -> 1 and three more shrink. 0x00443F90 shows a second spelling: 0x005EC990's text already declares the
  CRetailHandleVector stand-in at HEAD, but the binding was missing from the HEAD table. Queued.
- Includers: 212 good, 1 bad. 0x007D7EC0 grades DIFFER(182v66), and does the same with HEAD's headers
  (`logs/includer_007d7ec0_with_head_headers.log`). Its row, catalog and headers are unchanged, so the DIFFER is at HEAD and
  not caused by this landing. Queued.
- Link (`logs/link_landing.log`): 1 LNK2005 against 5 at HEAD (`logs/link_control_head.log`; the four GetSubDef_005ed4a0 /
  GetSubDef stand-in clashes are gone). The remaining one, CTCTavernGame::GetDef, is a header-inline pick-any copy against the
  strong owner 0x005EA000. It already clashes at HEAD through 0x005EB590 (`logs/link_control_head_getdef.log`), so it is DEBT.
- Queued: the remaining strong GetSubDef specializations, 0x005EA3C0's fake-body COMDAT, the search-tools opt-in mismatch, the
  GetPItem / SetPItem ruling (port namespaced local CBaseIntelligentPointer models onto engine/CIntelligentPointer.h), the missing
  CThingSearchTools_Calls bindings, 0x007D7EC0, the GetDef DEBT and 0x00443F90.
