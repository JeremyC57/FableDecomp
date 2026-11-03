# Codex pass 12 landing: header needs (2026-11-03)

## Added (new files; no default path existed, hdrcheck2 = NEW; no includer to recheck)
- `engine/Vorbis.h` (wave A; Codex copy, libvorbis internal structs for 0x00C9F8C0 / 0x00CA4160).
- Wave D (25): `CArenaRoundDef.h CConversation.h CEmptyData.h CFeatDef.h CFireHeartPatternDef.h CHeroSoulsRoundDef.h
  CPersonalScript_EmptyScript.h` (the 0x00EF7BE0 superset) `CQ_AmbushTradersScript.h CQ_EndGameScript.h CQ_SunnyvaleMasterData.h`
  (Codex's vacuous `QUEST_SCRIPT_PERSIST` ifdef folded to one include of `stlport/fable_containers.h`) `CScriptDataBase.h
  CScriptDef.h CV_AssassinAttacksScript.h CV_BordelloScript.h CV_HiddenBootyScript.h CV_TourGuideScript.h ECutsceneBehaviour.h
  EHeroAbility.h EOpinionDeedType.h EScriptingStateGroups.h ETextGroupSelectionMethod.h NScript_CQR_EscortTraderScript.h
  NScript_CV_ChickenKickingScript.h NScript_CV_FisticuffsClubScript.h NScript_CV_HeroDuelScript.h`.
  None defines a `FABLE_RETAIL_*` branch. The E* enum headers complete the MSVC forward declarations in
  `fable_nscript_forward.h`; they are not a second model. Codex's `CCharString.h`, `CCRC.h`, `CDefStringTable.h`,
  `CWideString(Data).h`, `GFMaths.h` and `stlport46/*` were not added (second models of `fable_string.h`,
  `fable_definition_table.h`, `stlport/fable_containers.h`); the rows were rewritten onto ours.

## Existing headers changed: 0
Default-path risk: none (git shows no modified file under `rebuild/include`).

## Needed and NOT applied (refused or not needed by a landed row)
| header | change | why not |
| --- | --- | --- |
| `engine/EAX40.h` | 4 GUID `extern` lines (`EAXPROPERTYID_EAX40_FXSlot0..3` / `EAX_REVERB_EFFECT`) | hdrcheck2 default=DIFF[201]: additive on the default path; its rows (wave C) are all held on the class-model question anyway |
| `fable_string.h` | `CCharString::Truncate(unsigned long)` in `FABLE_RETAIL_CCHARSTRING` (Ego_r 0x00AFAB00, FableWin 0x02F61D70) | default=same, branch DIFF; only 0x00E5EBD0 needs it and that row still DIFFERs 1068v1064, so the edit (and a recheck of ~30 landed definers) was not worth it |
| `engine/C3DVector.h` / `engine/C3DBoundingBox.h` | `LimitLower/LimitUpper`, `BuildFromPoints/GetCenter` under their class macros | default=same, but 0x00AC2BE0 still DIFFERs 2775v2791 (needs Codex's C3DVector ctor variants = a second model) |
| `stlport/fable_containers.h` | user ctor for `__false_type` | 0x00DEFB10 DIFFERs by one `movb $0,0x27(%esp)` without it; changing the default STLport tag type is build-wide, owner decision |
| new `FABLE_RETAIL_CFILEFORMATBRUSH` (push(1)/pop) | 0x00821010 file-record structs | the row still fails on 20 other missing CMap members |
| CEAXListener.h / CEAXBuffer.h / CodaEnvironment.h | Codex's class model | second model of pass 11 `CEaxContext` / `CEaxSource` / `CPKObject` (see README, wave C) |
| Codex STLport `_M_find` member template config | `??$_M_find@` | contradicts landed non-template `?_M_find@` binders (see README, STLport) |
