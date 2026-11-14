# acctC hand-back c25a (agentC25, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x007C0690 | NThingShot::CTCShotDefinitionBase::Init (target-thing overload) | 1,088 | RELOCATION_MATCH | PASS |
| 0x007EF200 | CTCCarriedActionUseExpression::OnActionUse(CThing&) (catalog: vfunc_28) | 734 | RELOCATION_MATCH (`/GS`, cookie in the bytes) | PASS |
| 0x0072CF40 | CTCSpecialAbilities::UseSpecialAbility(EHeroAbility) | 564 (488 code + 76-byte jump table) | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.
Plain C++; header additions in `hdrdiff/` (additive opt-ins and four new class headers; evidence in HDR_README.md).

Review items for the owner / coordinator:
- 0x0072CF40 writes deed values 0x11 and 0x3D as casts: the repo's EOpinionDeedType.h names for those values look like a
  different enum revision.
- 0x007C0690 uses the fitted `MAGNITUDE2D_ONE_EXPRESSION` opt-in (now two users) and a `have_target_pos` flag (a goto form
  grades the same).
- Name fixes (NAME_FIXES.tsv, LABELS.tsv): 0x00802390 is CTCTargetingPlayer::GetMaxTargetingDistance (landed file
  `DrawGetSeaLevel_00802390.cpp` is misnamed); 0x005D8E50 CTCInventoryItem::ReturnToInventory; 0x0072C430 IsAbilityAvailable;
  0x0072AAF0 SetupMultiStrike (by elimination); 0x008111F0 CEventPerformedSpecialAbility ctor.
