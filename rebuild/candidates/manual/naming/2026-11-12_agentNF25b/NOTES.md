# agentNF25b notes (redo of NF25)

## Scope
- Part 1 (land1011df, 5 entries): all 5 verified and applied, 0 rejected. NF25's hints all held up when re-checked.
- Part 2 (NF24's open families): 0x005CC10E done (19 binders), 0x00658B20 done (self row + 7 binders), 0x00EF7AB0 partial (7 binders;
  0x00EF7800 skipped), 0x0095A420 skipped (not cheap).
- 45 rows changed: 44 RELOCATION_MATCH + 1 MATCH (0x005DA4E0). Every test PASSes with the catalog PassPattern. At most 3 grades per row
  (0x00555CF0); counts in STATUS.tsv include one final regrade of every row.
- No BUSY or pending RESERVED address touched (checked against BUSY.txt + every pending/*/RESERVED.tsv).

## Evidence for the entries (two sources each)
- 0x00555A90 UpdateVisibleList: protected IAE in both PDB name sets; no SetScroll in either. A byte scan of the catalog finds exactly 4 callers
  (0x00555CF0, 0x00555EB0, 0x00556200, 0x00557000).
- 0x006E92C0 IsAttacker const: Ego_r IsAttacked 0x00712490 / IsAttacker 0x00712500 / IsAttackerOn 0x00712570 follow the retail order
  0x006E9270 (tests +0x50, Defender) / 0x006E92C0 (tests +0x1C, AttackerInfo.Attacked) / 0x006E9310.
- 0x005DA4E0 ResetAiming: the Ego_r CTCWeapon void members appear in donor order, and the retail bodies at 0x005DA490..0x005DA510 line up
  with them (IsAimedForLongEnough reads +0x18/+0x14 with `ret 4`; SetAimingTimeOverride `+0x18 = 1`; SetAiming `-1 -> 0`; ResetAiming;
  FrameUpdate is landed; IsMeleeWeaponClass is in the header).
- 0x00658B20 IsWorkTime: reads CVillageDef +0x64/+0x70, which are WorkStartTimes/WorkDuration after the 12-byte vector shift. Ego_r order puts
  IsWorkTime before IsCookTime (0x00658B6B, landed).
- 0x005CC10E: the body pushes 'CCarryableDef' (the landed row says so), so T is CCarryableDef in every caller.
- 0x00EF7AB0: vtable 0x012C2FE8 is NScript::CIsThingConscious (header + Clone 0x00CDDAE0 + operator() 0x00CDDAB0).

## Rows left (gaps)
- 0x00EF7800 (CPersonalScriptEmptyScript::COpinionMan::Main): still binds ??1CIsThingStored@NScript@@UAE@XZ / ??_7CIsThing@NScript@@6B@. It
  needs a two-view model (an inline dtor for the temporary and an out-of-line call for the And member), so I skipped it.
- Other binders of vtable 0x012C2FE8 that this pass did not touch (other families): 0x00E16A80 (??_7CIsThingConscious@@6B@, global),
  0x00DB7E10 (??_7CIsThingPredicate@@6B@).
- 0x0095A420: the CQueenNative stand-ins (0x0095A6C0 / 0x0095B6F0) need CAIStateGroupBase slots 14-21 named and CircleMarkers typed in the
  HoverHornetQueen header. Not attempted.
- Other SetPredicate 0x00F35B10 binders still spell local members (CDarkwoodAssassinSpawn, CBanditCronies RegisterFilter,
  CMurderTwistBuriedGem WaitFor, CSignpost, NScript::CEntityScriptBase). Only the three rows ported here moved to the base spelling.

## Follow-ups seen (in NAME_FIXES.tsv)
- 0x00662410 IsInMeleeAsAttacker: 62 call sites, many landed with made-up spellings (IsRegisteredCombatant, CanAttackNow, IsCombatant). 0x0096EDB0
  (edited here) still declares it as IsRegisteredCombatant. I left it alone because it is a different family.
- 0x005DA4C0 is SetAimingTimeOverride (landed as a CXMVPlaybackInfo ctor). 0x005DA490 is IsAimedForLongEnough (landed as
  global_ComparisonFloatWithEpsilon).
- 0x00658A35 is IsSchoolTime and 0x00658A80 is IsStoryTime. Both landed rows are named IsCookTime.
- 0x008780D0: the manifest/catalog label is still GetPBestThingInRoughArea<...>. The row is parked in pending Q18, so nothing landed needs a respell.
- 0x009687B0 emits ?IsCarryingJug@CAIStateGroup_TavernServer@@QAE_NXZ, the same symbol as 0x00966CF0. Its caller 0x00968BC0 spells the class
  CAIStateGroup_TavernKeeper, which is in neither PDB name set. Leaf kept as the object emits it. This is one name on two addresses, so its
  real class needs proving.
- 0x00555CF0 (local ProcessEvent(int), observer/manager models) and 0x0096EDB0 still carry other unproven local names. They are untouched
  apart from the respelling done here.

## Header debt
- Overlay opt-ins to merge (hdr/README.md): FABLE_RETAIL_CENVIRONMENTTHEMEPRIORITYSTACKENTRY_STACK_FRIEND, FABLE_RETAIL_CTCWEAPON_RESETAIMING,
  FABLE_RETAIL_CCOMBATMANAGER_ISATTACKER, FABLE_RETAIL_CCARRYABLEDEF_ONKILLFX, FABLE_RETAIL_CTCVILLAGE_ISWORKTIME.
- Once 0x008BF500 lands, nothing uses FABLE_RETAIL_CCOMBATMANAGER_ISATTACKING (IsCreatureAttacking) or FABLE_RETAIL_CTCWEAPON_READYPROJECTILE
  (ReadyProjectile). Neither spelling is in a PDB, so both can be deleted.
- The CalculateTheme friend opt-in is an access device. FableWin's GetLevelThemeWeight() const is out of line in the debug build and absent
  from Ego_r, and its body is unproven, so I did not use it.

## Descriptive names
- None added. The made-up getters GetFillCarrySlot / GetItemID and the local types CJugDef / CPickUpDef / CDebrisDef / CDefLookup /
  CScriptThingFunctor_00ef7ab0 / N_00ef7ab0::CIsThingAlive / CIsThingAliveEndGame / CGameScriptInterface_Calls are all removed from the
  rows touched here.
