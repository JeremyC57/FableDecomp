# NF168 notes (round 10, TC component naming CTC I-Z)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 32 entries: 12 done, 5 done-already, 3 blocked (q4223, q4224, q5162), 2 deferred
(q4718, q5091), 10 open (handed back early for context). STATUS: 31 rows ported (1 MATCH + 30 RELOCATION_MATCH, every test PASS in
recheck_final.log, one vland run over all 31), 17 not-a-binder (entry self rows already right), 1 blocked (0x00811760). 38 compiles in all (work/grades.log). selfcheck.log: 0 problems. No test file changed (every touched row's test is a
standalone host model that does not link the row; none was rewritten).

## Entries
- q4183 GetXString 0x005CC772: 7 wrong binders (free `?GetXString@@YI`) ported onto the shared headers; the four FinalConstruct/RefreshSubtitles(PC)
  rows were stand-ins (`NInventory::CTCInventoryTrade`, local `CSubtitleMap` with a same-TU fake `find` body) and now use the real STLport
  map + `FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE` (the NF50 trade-refresh form), each RELOCATION_MATCH in one grade. That made them self renames
  (RENAMES s1); 0x0059D65A's caller 0x0059E4D7 bound the stand-in, so it was ported in the same hand-back (also a self rename to the PDB AAE /
  map& form; its callers 0x005A4CDD / 0x005A5B31 already bind that). 0x0059D761 is FinalRefreshSubtitlesPC (catalog said FinalRefreshSubtitles,
  the console twin 0x0059D1B1): catalog rename in RENAMES s1. 0x0056EA4E's own leaf was public QAE while its callers bound the PDB private AAE: fixed.
  Side respellings in these rows: 0x005D95C0 free GetDescription -> CTCInventoryItem::GetDescription (static), 0x005B1A8D stand-in
  CSubtitleMap::find -> the real `_M_find`.
- q4223 / q4224 (data): binders 0x006D0410 / 0x007E5F70 respelled in place inside the existing local CTCWife / CTCOracleMinigame models
  (private static const arrays). **Blocked (rows left):** each static's atexit destructor is landed as a stand-in that the exe scan cannot see
  (offset references): 0x012294A0 `global_CreateCRC_012294a0` binds `g_AutoTinyReverseLoopObjects` (0x013BAB30) and 0x01229DF0
  `global_CreateCRC_01229df0` binds `g_AutoTinyReverseLoopObjects_013bae94` (0x013BAE94). Each needs re-authoring as the dynamic atexit
  destructor of its static (initializer / destructor pair rule, 2026-11-20). Each address goes from 3 spellings to 2; none is created.
- q4467: 0x005C9962 / 0x0069F9E0 respelled in place (local CTCInventoryWeapons models).
- q4530: 0x00730070 / 0x00783380 in place (CThing* -> CThing&); 0x0089D3A0 ported onto engine/CThing.h + engine/CTCVillage.h (dropped the
  CTCVillage_Retail / CThing_Retail stand-ins). 0x00783380 was found by the exe scan (not in WORKLIST).
- q4718 / q5091 GetPerceivedThing 0x007446A0: 5 binders fixed (0x0089CD70, 0x0089D0E0, 0x00849320, 0x009577E0, 0x0091DB50) through four
  declaration-only flat-branch opt-ins in engine/CTCPerceiveThing.h. **Deferred:** 0x0093B8B0 and 0x0095F9A0 (locked NF166), 0x0093C4E0 (locked
  NF167) still bind CTCPerceiveThingAccess / CTCCreatureOpinionOfHero::GetTargetThing / CTCPerceiveThingMethods (QBEPAV). The address was
  already multi-spelled; nothing new was created.
  0x0091DB50's "CTCTargeting" stand-in (interface 0x34) is really CTCPerceiveThing: IsLockedFast 0x00661600 = CanSeeOrSmellPerceivedThing const,
  IsLockedSlow 0x00643B3B = IsAwareOfThing const (both spellings already bound by other landed rows).
- q4763: 0x005721FA / 0x005724CD ported onto engine/CTCInventoryAbilities.h. **New finding:** 0x005EE136 is not GetAbilityLevel (that is
  0x005EDE93, +0x160 Abilities, bound by 0x005D64DD) but `GetExperienceSpentOnAbility` (reads +0x148 ExperienceSpentOnAbilities; Ego_r.pdb
  0x0057CC91 + FableWin.pdb 0x01EBC6E6, QBEJW4EHeroAbility@@@Z). The first form bound the PDB GetAbilityLevel to 0x005EE136 and vland flagged
  RELOC_CONFLICT; the self row 0x005EE136 (stand-in `S::GetAbilityLevel`) was renamed and ported (MATCH); its sole referencer is 0x005724CD
  (exe scan). Also 0x0056FFC5 GetTCInventoryAbilities respelled in place to the PDB private AAE / `PAVCTCInventoryAbilities` in both rows.
- q4963 UpdateStock 0x00775350: not landed; NAME_FIXES.tsv; binder 0x00775B20 in place.
- q5092: 0x006A81F0 onto engine/CTCNoise.h (GetCreator const). q5094: 0x0094BCA0 / 0x0094BDF0 onto engine/CTCShopKeeper.h with the retail
  CThing / CThingCreatureBase define chain (0x004FF750's set): GetShopThing -> GetCustomer const (Customer +0x0C, Ego_r order), and
  GetShopTC QAE -> the PDB QBE (0x00917DB0, not mine, still binds the QAE form: existing split, not grown).
- q5095 / q5096 / q5160: in place in existing local models. q5097: 0x00911A90 onto engine/CTCTargetingAI.h.
- q4480, q4957, q4986, q5134, q5135: done-already (all exe-scan referencers already bind the PDB spelling; reloc_bindings holds only it).
- q6192: not landed; catalog fix in NAME_FIXES.tsv.
- q5162 **blocked: port** 0x00811760 (see STATUS): needs its local CThing / CIntelligentPointer models moved onto the headers first.

## Open entries (not started) and resume pointers
Untouched locked rows of the open entries (rows.txt, no STATUS line): 005da950 005daa40 005dab10 005dabe0 005dad10 00615920 006159a0 0068eb20
00715110 0072ab40 007517b0 00751e80 007525c0 00753c70 0079acc0 0079b650 00800540 008ef190.
- q5203 0x006159A0: the only wrong binder is 0x00615920, an all-stand-in row (non-template `CPersistContext_TransferableVector`, local
  CUIMiniMapMarker / CCharString / C2DVector, leaf `TransferIn`). Port: engine/CPersistContext_TransferableVector.h template + engine/CTCInventoryMap.h
  nested CUIMiniMapMarker + an explicit member instantiation; it is a self rename (vtable-only referencer).
- q11927 / q11928 / q11931: all in the CTCLook family (rows 0x00753C70, 0x007517B0, 0x00751E80, 0x007525C0, 0x00753840); work them together.
- q6323, q11685, q11688 (29 referencers), q11929, q11932, q11990: not censused.

## Rows whose own leaf stays a pre-existing stand-in (not changed, outside the entries)
- 0x009577E0 `CBehaviourMethods::IsRunnable` (catalog CAIStateGroupBehaviourBase::IsRunnable); 0x0089CD70
  `CGameScriptInterfaceMethods::CanThingBe_Heard_ByOtherThing` (ScriptMethods params; GetSoundRadius 0x00744440 still on the PerceiveMethods stand-in).

## In-place respellings of pre-existing local models (no new class token; selfcheck NEWLOCAL 0)
0x006D0410 CTCWife, 0x007E5F70 CTCOracleMinigame, 0x005C9962 / 0x0069F9E0 CTCInventoryWeapons, 0x00730070 / 0x00783380 CTCVillage,
0x00775B20 CTCShop, 0x007EFE70 / 0x006BFFD0 CTCWallMount, 0x008B9C90 CTCWillLightning, 0x0089CD70 PerceiveMethods (a member dropped),
0x005721FA / 0x005724CD CTCInventoryExperience (GetTCInventoryAbilities return type and access).

## Other notes
- No new made-up names. No `// Flags:` lines added. No `FABLE_FORCEINLINE` in any opt-in. No funclet includes any changed row (grep).
- No self rename touches a row that EH funclets include.
- Locks: 0x00783380 / 0x005724CD were `released`; claimed (owner now NF168). Also claimed 0x0059CA95, 0x0059CFBD, 0x0059D0B8, 0x0059D1B1,
  0x0059D65A, 0x0059D761, 0x0059E4D7, 0x0094BDF0, 0x005EE136, 0x007EFE70, 0x006BFFD0, 0x00911A90, 0x008B9C90, 0x00811760.
- Leaf lines: rows that had a prefix-only or missing `// Leaf:` and were rewritten now carry the full mangled leaf (0x0059CA95, 0x0059CFBD,
  0x0059D0B8, 0x0059D1B1, 0x0059D65A, 0x0059D761, 0x0059E4D7, 0x006D0410, 0x0056EA4E, 0x005EE136).
- STANDIN_LEAF warning on 0x0059D761 in vland: the catalog name is the twin's (FinalRefreshSubtitles); RENAMES s1 renames it to FinalRefreshSubtitlesPC.

## Rule slips
- Queue order: q5203 was left open (all-stand-in port, see resume pointers) and q6192 (a catalog-only check) was done after it.
- 0x00811760 was blocked after two failed forms, not three; the next idea is in STATUS.
- RENAMES section 1 old_leaf values were derived from the HEAD source declarations (NInventory:: stand-in class, CSubtitleMap struct key, QAE),
  not read from compiled HEAD objects.
- The objdump defined-symbol diff (landing vs HEAD object) was not run. The new non-leaf definitions are shared-header COMDATs: the STLport
  `_Rb_tree<...>::_M_find` instance (noinline template, pick-any) in 0x0059D0B8 / 0x0059D1B1 / 0x0059D65A / 0x0059D761 and header inlines.
- The q4223 / q4224 "no other referencer" claim was first written from the exe scan alone; the atexit destructor rows (offset references) were
  found only at the final check, and the two entries were moved from done to blocked.
- 0x0059CA95: two compile failures before the matching form (a missing define, then a static assert) - counted as grades.
- 0x0059CA95 .. 0x0059D761 were drafted in work/ before their locks were claimed (claimed before any file went to rows/).
- STATUS grades_used for 0x0059CA95 was first written as 5 and corrected to 3 from grades.log.
