# Chunk 732 -- CThingPlayerCreature melee rows + CQuestManager::TransferBoasts retry

Grading mirror (`mkmirror.sh`): repo `rebuild/include`, then the PENDING chunk 728 round-2 overlay
(`rebuild/candidates/manual/pending/2026-11-03_linux_hand728/round2/hdr`), then this chunk's `hdr/` on top.
`hdr/` also carries copies of pending work this chunk builds on: chunk 728's `CPersistContext.h` /
`CPersistContext_TransferableComponent.h` (unchanged), chunk 729's `CQuestManager.h` (edited here), and chunk 718 round 3's
`CTCPhysicsBase.h` GetFacingVector overload swap (re-applied over the 728 version, which keeps 728's StatGetInterfaceType line).
Land 728 round 2 (and 718 round 3's CTCPhysicsBase swap) first, or land this overlay as a superset.

| Row | Result | Attempts |
|---|---|---|
| 0x004B05C0 CQuestManager::TransferBoasts | RELOCATION_MATCH, vland PASS | 14 |
| 0x006AA2E0 CThingPlayerCreature::DrawPreparePrimitivesForRendering | RELOCATION_MATCH, vland PASS | 7 |
| 0x006AABE0 CThingPlayerCreature::SetActionForMelee | RELOCATION_MATCH, vland PASS | 29 (10 source + 19 header probes) |
| 0x006AA720 CThingPlayerCreature::SetPlayerCreatureActionForMelee | DIFFER 1218 v 1212, dl=5 | 16 |
| 0x006AB820 ResolveFacingDirection | not attempted | 0 |
| 0x006ACA30 OnCollisionWithObject | not attempted | 0 |

## 0x006AA720 residue (owner decision)
Only the proximity argument differs: `GetCombatManager().GetProximityForNavigation()` (FableWin `QBEMXZ`, inline) gets a dead
return-value store from VC7.1 in every form tried (in-class, __forceinline, out-of-class inline, const return). Reading
`GetCombatManager().ProximityForNavigation` directly is RELOCATION_MATCH (`work/p13x.cpp`, graded with the CCombatManager data
made public in a throwaway mirror). No PDB fact on disk shows the access level, so it is parked, not landed.

## Header changes (all default=same under hdrcheck2; every retail branch opt-in)
- `C3DVector.h` (FABLE_RETAIL_C3DVECTOR): RotateXY decl (0x00A14040); GetSquaredMagnitude2D and IsNonZero as running statements
  (bodies from 0x006AABE0's bytes; FableWin names, no retail out-of-line copy). GetSquaredMagnitude stays one-line: 0x00AE0710
  needs it (a statement-form GetSquaredMagnitude matches 0x006AABE0 but breaks 0x00AE0710; probes v1..v23 in LOG.tsv).
  `extern const float DELTA_NOTIONAL_ZERO` declared in the retail branch for IsNonZero.
- `CThing.h` (FABLE_RETAIL_CTHING): __forceinline GetTC(ETCInterfaceType) pair; PeekComponent (0x004C7A70), GetPlayer (0x004C7A30).
- `CThingCreatureBase.h`: GetNavigationLevelAt (0x00661D20), GetCombatManager (0x00661ED0), protected virtual DrawPreparePrimitivesForRendering.
- `CThingPlayerCreature.h`: full pack(1) retail branch (needs FABLE_RETAIL_CTHINGCREATUREBASE), members +0x160..+0x200 (Ego_r names).
- `CTCTargeted.h`, `CTCWeapon.h`, `CKickableDef.h` (+StatGetTypeName), `CTCCreatureNavigation.h`, `CTCCombat.h`, `NCreatureMode.h`,
  `CCombatManager.h`, `CPlayer.h` (IsZTargeting), `CGameCameraManager.h` (Camera typed CCamera + GetCamera), `CDefString.h`
  (ToWideString, FABLE_RETAIL_CDEFSTRING), `CPersistTraits.h` (value-init ctor), `CQuestManager.h` (CBoast + lists), `fable_game.h`
  (PeekDebugFont), `CTCPhysicsBase.h` (718's overload order), action headers (StabThing, KickThingOnGround, InterruptableMidAttackAutoTurn).
- New: `CCreatureAbilityDef.h`, `CTCCarrying.h`, `CTCStealth.h`, `NPlayerTargeting.h`, `NTargeting.h`, `NGraphicAppearance_CTCGraphicAppearance.h`.

Siblings: 46 landed rows whose preprocessed text changes under the final mirror (cpp diff vs repo+728, private stub dir last on
-I), all regraded with vland `--recheck-landed`: 38 RELOCATION_MATCH + 8 MATCH, all tests PASS (SIBLINGS.tsv). The 7
CTCCreatureNavigation rows needed StatGetInterfaceType guarded by FABLE_RETAIL_CTCBASE (they have no retail CTCBase).

## Names
Descriptive (no PDB data name on disk): g_MeleeUsesCameraDirection 0x01375750 (one reader; console bool, init 1),
g_DebugDrawHeroNavigationLevel 0x013B8640, g_DebugDrawStealth 0x013B8A80. ECreatureAbility values 1 (flourish) and 7 (block
counter) are written as casts: their enumerator names are not on disk. NAME_FIXES.tsv: 15 respellings, all exact Ego_r + FableWin.

## 0x006AB820 ResolveFacingDirection outline (for the next chunk)
1. If PCurrentAction (+0x100) is set, its byte +0x61 is clear and its virtual slot 49 returns true: LeanAngle = 0, set the
   physics RHSet (slot 75) from Up (0,0,1) and GetFacingVector(); return.
2. stealth = TCI_STEALTH and byte +0xF of it; if CPlayer +0x21A: camera (const GetCameraManager, slot 10) forward XY -> facing.
3. Else if IsFirstPersonTargeting: targeting slot 37 GetTargetedPos(pos, 1.0f) minus GetPos, Z=0, normalise, set RHSet.
4. Else if IsZTargeting: targeting slot 36 thing -> GetFocalPos (0x004C1060) or GetPos, 0x006A4D00 / 0x00661540, fcos/fsin
   of 0x0122ED70 against 0x0124F2B8, set RHSet; no target: RHSet from 0x00487050 (CRightHandedSet ctor).
5. Else, with MovementAcceleration (+0x180) non-zero: the turn angle (acos 0xBFEBA2 of the clamped dot, sign from the cross),
   clamp by 0x006A99D0 * 0x0125D1E8 / 0x01238E00, rotate (0x00A13C60), lean angle from MovementBandType (+0x200) 3/4 and
   PPlayerMovementDef (+0x1EC) +0x70/+0x74/+0x78, the 0x006ADC20 helper, LeanAngle clamp, then the MovementBandType 1/2 fpatan
   + fmod (0xBFEB28) wrap that zeroes MovementAcceleration.
