# 2026-11-20 landing nf122: PRIORITY slice 6 remainder, first hand-back (54 rows)

Records: `naming/2026-11-20_agentNF122/` (selfcheck 0 problems). 23 entries (NF118's leftovers): 9 done (q4266 q4490 q4985 q5010 q6228
q6229 q6325 q6929 q6936), 1 done-already (q6225), 5 blocked "port, not attempted" (q6226: self row 0x006E11F0; q6227: binder 0x008F7BD0;
q6278 / q6417 / q6471: binders 0x00CFD6C0 / 0x00D6AEA0; each needs about ten local or namespaced class models moved onto shared headers
first, NOTES names them), 8 open, moved to NF125 (the CTCHeroExperience family q4486 q4762 q4764 q4765 q4766; q5107 / q5151; q5137).
212 rows settled as not-a-binder by the probe.

## Rows (54): 54 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 7 self renames (RENAMES s1): CCombatManager::DeregisterCombatant, CGameTimeManager::IsDay, CTCCreatureNavigation::GetNearbyWanderPosition,
  CEnvironmentSpellEffectInterpolator::ScreenFilterFadeOut, CGameScriptInterface::TransitionToThemeExternals,
  CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<HasTC, InRegion>, CThing>, CCountedPointer<CScriptThing>::operator=.
- 47 binder respells (57 section-2 lines), including 0x006A1E00 (q4985): IsCarryingThing is now `(const CThing&) const`, closing the
  0x006A1060 split nf121 shrank; RemoveThingFromPrimarySlot returns CThing* (both PDBs) in 0x006A1E00 and its binders.
- 0x009647C0 was also changed by nf121; NF122's copy was taken after that landing (the only diff is NF122's own line).
- 0x00704580: leaves the free stand-in no row bound for the CScriptThing operator= spelling 20 of 24 referencing rows bind; 0x00CFD6C0 /
  0x00D6AEA0 (blocked) keep `??4CCountedPointer@@QAEAAU0@ABU0@@Z`: an existing split, 4 -> 3 spellings in the table. Possible linker fold
  with CCountedPointer<CDiskFileWin32>::operator= (Ego_r 0x00A35EE0); no landed row references the address under another type, so kept.

## Headers (add-only opt-ins, default path token-identical; pin_asserts pinned 0; assert_names_check 0 mismatches)
NScreenEffect_CScreenEffectPriorityStack.h DELETESCREENFILTERENTRY; CGameTimeManager.h ISDAY; CTCRegionFollower.h FOLLOWERS_ONLY and
CTCDParticleEmitter.h METHODS_ONLY (partial modes: member functions without the CTCBase base, so rows can drop local stand-ins; round-9 rule).

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. 653 rows rechecked: 653 good / 0 bad. No-new-split gate 0. Table spelling sets:
18 addresses shrank (e.g. 0x006AEDC0 4 -> 1, 0x006BB2C0 4 -> 1, 0x006ED9C0 5 -> 2, 0x006A1E00 3 -> 1); none grew.
NAME_FIXES queued: 0x008818A0 DeleteScreenFilterEntry (catalog label wrong), 0x006E11F0 CTCDParticleEmitter::Create (self row still on a
stand-in; every caller binds the PDB spelling), 0x006ED510 private DeregisterAttackingCombatant (two binders, two other spellings).
One source only, not queued as fixes: by Ego_r member order 0x006BB1E0 may be IsDaySounds and 0x006BB2F0 SetAsUsingVillageTime.
