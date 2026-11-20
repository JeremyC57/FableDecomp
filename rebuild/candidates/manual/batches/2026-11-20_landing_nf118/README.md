# 2026-11-20 landing nf118: PRIORITY slice 6 (12 rows)

Records: `naming/2026-11-20_agentNF118/` (selfcheck 0 problems). 35 queue lines over 27 addresses: 12 done, 22 open + 1 deferred
(q4490), all 23 moved to NF122. 15 addresses settled as not-a-binder (STATUS).

## Rows (12): 2 MATCH + 10 RELOCATION_MATCH, every test PASS (`logs/recheck.log`, rechecked again after merging FullDecomp-B ef3d963e66)
- 0x0072A860: self rename CASuspendableProcess::CheckTermination -> `?IsChargingUpAbility@CTCSpecialAbilities@@QBE_NXZ` (Ego_r 0x00741700 +
  FableWin 0x0254F790; Ego_r donor order matches retail's TU head 0x0072A830..0x0072A860; Ego_d carving puts IsChargingUpAbility in
  tc_special_abilities.obj, while GetAbilityMutexLock is a header-inline COMDAT). The queue's posed GetAbilityMutexLock is rejected
  (REJECTED.tsv). Body `if (AbilityMutexLock) return true; return false;` MATCH (`// Inferred:`).
- 0x0068F2A0 / 0x00690030: binder ports from the CASuspendableProcess / CTCHeroState stand-ins onto engine/CTCSpecialAbilities.h FULL.
- 0x006AF220: descriptive CDistanceFilter_006af220 -> CMessageEventFilter_CanBeSeenOrHeard::operator() const (PDB); callees now the
  PDB CThingAICreature range tests (`// Inferred:` line in the row).
- 0x006B32A0 CEnvironment::UpdateInterpolation, 0x006D5AD0 CThingAICreature::GetCurrentStateGroupScriptPriority: off the `T::Run` stand-ins.
- 0x006D4C00 CPersistContext::TransferVector<pair<CCharString,long> > onto engine/CPersistContext.h (invented CNamedLong model gone).
- 0x00718EC0: STLport `_List_base<pair<long,EConversationAttitude> >::~_List_base` (q4018; descriptive-by-library, no PDB holds this list;
  the only caller 0x00719C70 already binds this spelling).
- 0x00729BC0 private SetUpPEntranceConnectedToFromUID; 0x006A8930 / 0x006BBF70 in-place leaf respells; 0x0092E540 onto the shared
  CThingFilter_IsOccupiableOccupied.

## Headers (add-only opt-ins, default path token-identical)
CAIBrain.h GETCURRENTSTATEGROUPSCRIPTPRIORITY; CEnvironmentSpellEffectInterface.h FRAMEUPDATE; CTCSpecialAbilities.h ISCHARGINGUPABILITY,
GETCURRENTABILITYSTRAFEANIM, GETCURRENTABILITYBEINGUSED (FULL branch).
Needs header claim: CTCSpecialAbilities.h STRAFE_CALLS_PASS24 still declares `GetAbilityMutexLock() const` for the unlanded pass24 draft
0x008E9B70; that draft's 0x0072A860 call should use IsChargingUpAbility.

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. Referencing + includer rows: 517 good / 0 bad. No-new-split gate 0
(`logs/split_gate.txt`). Table spelling sets: 0x0072A860, 0x008332E0, 0x006A8A30 2 -> 1. New table rows for 0x00885010, 0x0125ED48,
0x006D3A30, 0x006D4B40, 0x0088B760 (their callers had no rows in the table before). The last three are existing splits, not created:
before the landing, 0x006D4C00 referenced the non-template `GFSerialiseVectorBinaryIn/Out(CNamedLong)` and 0x006D5AD0
`Sub::Run_0088b760`, and none matched the self rows' definitions. Queued as land1120nf118 NAME_FIXES (self rows still on stand-ins).
