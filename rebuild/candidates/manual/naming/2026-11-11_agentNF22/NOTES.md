# NF22 notes: Codex pass 21 name-fix queue (115 entries)

Result: 45 done, 70 partial, 0 rejected (APPLIED.tsv). 278 rows changed (rows/), 7 tests changed; every row grades MATCH or
RELOCATION_MATCH with no HARD reloc-identity finding and passes its catalog PassPattern (STATUS.tsv; work/<addr>.log). Most grades on
one row: 6 (0x004874D0). 27 self-row renames or signature fixes (RENAMES.tsv section 1), 97 caller respellings (section 2).
No row in BUSY.txt was touched; entries that need one say "row busy (NF21)" or "BUSY".

## Claims
- All the function and data symbols applied were found in Ego_r.pdb and FableWin.pdb name sets (ghidra_out/egor_pdb_names.tsv,
  fablewin_pdb_names.tsv), or in anniv360.pdb for the globals the PC PDBs lack (NPCAttitudes, NPCAttitudeStrings, ST_HEAD_TURN_H/V,
  NullCoord), plus a body, binding or vtable check.
- 113/114 corrected: ST_HEAD_TURN_H/V are `@@3VCCharString@@B` (const), not `@@A`.
- 39/40 (0x00789358 CHitParameters dtor) verified by layout: Ego_r DamageMap/Augmentations/PHitMaterialDef/PThingHitting/PWeapon land
  at retail +0x30/+0x3C/+0x4C/+0x50/+0x58. The row itself is left: rebuilding it over engine/CHitParameters.h is an author job.
- 93-96: already bound by 0x00DB5330 / 0x00DBB310 (0x00DBB310 is RESERVED).

## Partial reasons (details per entry in APPLIED.tsv)
- Most partials: the remaining binders call through stand-in classes, address-suffixed namespaces or free functions, and the correct
  class either has no shared header in those rows or the row models it locally (respelling would mean a new local model).
- 0x0122DEDC (0.0f): 0054ecb0 00697710 00732d00 008909f0 00895a10 0089d340 keep the alias; the literal compiles to `fldz` or a
  different schedule (DIFFER).
- 0x01375550 ConstantFPS: protected static, reachable only through the inline GetConstantFPS(), which changes VC7.1's schedule in
  the 21 remaining rows (11 tried, 10 DIFFER). Naming wave 18 already took every row where the accessor keeps the bytes.
- 0x0143E8E0 NullCoord, 0x0143E90C pGlobalScriptDef: reachable only through shared inline/def-pointer models that clash with the
  binders' 1000-line local models.
- 0x00780400 / 0x00780410: 0089bcd0 and 0089bd90 were tried over the shared CTCFlammable.h (C2011 against their local
  ETCInterfaceType/CVectorMap) and reverted (1 grade each).

## Extra fixes made on the way
- 0x007890EA: 00788229's GetTCDriver<CTCDPhysicalObstruction> specialization respells that binding too.
- 0x00749880: 008386e0's callee respelled to `?SetCreator@CTCNoise@@QAEXABVCThing@@@Z` (Ego_r). The landed row 0x00749880 itself is
  still the stand-in AssignTarget_00749880: needs a self-rename.
- 0x0099E9B0 UnassignString is protected (IAE) in the shared fable_string.h; 0x0129AAF4 now binds `?NullChar@CCharString@@1DB`
  where rows had g_EmptyCharString_0129AAF4.
- 0x0096E700: removing the EnemyMargin alias for the 1.0f literal re-associated the adds (HARD RELOC_IDENTITY on
  DELTA_NOTIONAL_ZERO); fixed by putting `limit += DELTA_NOTIONAL_ZERO;` in its own statement (3 grades).

## Gaps and mislabels found (not changed)
- Two landed spellings of DeregisterCombatant: 0x006EA430 is `?DeregisterCombatant@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z`
  (queue 31, self-renamed here), but rows 007355f0 / 007e5b40 / 0097b780 / 00733580 and the landed row
  CCombatManager_DeregisterCombatant_006ed9c0.cpp spell 0x006ED9C0 DeregisterCombatant(CThingCreatureBase*). Both PDBs have only one
  DeregisterCombatant; 0x006ED9C0 is something else (perhaps DeregisterDestroyedCombatant or DeregisterAttackedCombatant): unproven, needs a
  naming pass.
- 0x0099BDB0 is labelled CCharString_SetSubstring; fable_string.h says GetSubString.
- 0x006EA2C0: the repo's 006ea430 bound it as RemoveCombatant(CThingCreatureBase*); both PDBs say private PerformSequenceReset(CThingCreatureBase&)
  (`?PerformSequenceReset@CCombatManager@@AAEXAAVCThingCreatureBase@@@Z`), which 006ea430 now binds (the catalog label already says CCombatManager::PerformSequenceReset).
- 0x009BEDE0 / 0x009BEE00: stand-in callees (CResourceOwner_01436e1c, CEngineDisplay) whose real names are unproven.
- 0x00445BA0 / 0x00445BC0: stand-ins CGamePlayerInterface_Methods::GetToggleSneak and similar; by the same body pattern as 0x00445B80
  they are probably IsToggleSneak / IsRequireAttackButtonHeldForFlourish (CControlsDef bools). Not verified, not changed.
- 0x00DBCD00: CNOVI_Bully GivenTeddy stand-in, the same shape as 0x00DB0600.
- 00b7ad20 binds `g_pRenderManager` to 0x01436E18, a different global from GPDisplayManager 0x01436E1C (left as is).
- The address-suffixed globals `g_CCharStringInstanceCount_013BD800` and `g_EmptyCharString_0129AAF4` still appear in landed code
  outside this queue.

## Header debt
- engine/CEngineLightingManager.h line 6 comment still names g_pEngineLightingManager.
- engine/CQ_SunnyvaleMasterData.h FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS still declares RelationCounters /
  RelationSuffixes as class statics; after 00cdba10 / 01224ce0 they are the globals NPCAttitudes / NPCAttitudeStrings (anniv360.pdb)
  and no landed user is left.
- engine/CTCFlammable.h is not standalone (needs engine/ETCInterfaceType.h first).

## Naming-only changes
Some rows were respelled only in helper declarations that are not the leaf (no relocation change): 009994a0 00999620 009c7a60 (NoStrings) and
the pDSTGame declarers listed in APPLIED entry 103. They are named in APPLIED.tsv and graded like the others.
