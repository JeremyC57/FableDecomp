# Hand chunk 575 (NScript quest WatchForTermination .. STLport vector<TeleportData>): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (gall.sh) and pass their tests under g++ and the lane's VC7.1 (tall.sh / tall.py);
the read-only vland run reports 10 RELOCATION_MATCH + PASS, no RELOC_IDENTITY and no warnings (the first run's WARN
SHARED_HEADER_AVAILABLE for CBaseObject went away once 0x00EFC300 carried its `// Layout:` line; see Header debt). No retail_asm, no flags, no pragmas.

## What matched (10)
- **Quest-script `WatchForTermination` threads (7):** 0x00D01290 NScript::CQ_BanditCampScript, 0x00DC3BD0
  CQ_OpeningGraveyardSecretPassageScript, 0x00F01A40 CQ_CinemaTestScript, 0x00D68600 CQ_GuildTrainingWoodsWillScript,
  0x00D9B2F0 CQ_HobbeToothContestScript, 0x00D62140 CQ_GuildTrainingWoodsDepartureScript, 0x00D04260
  CQ_BanditCampBossBattleScript. Wait `while (!MissionFailed && !MissionSucceeded) { NewScriptFrame(); if (term) return; }`,
  re-test the thread, then an if/else on one flag with a thread test in each arm: SetQuestAsCompleted(GetActiveQuestName(),
  a, b, c) / SetQuestAsFailed(GetActiveQuestName(), a, L"..", b), then DeactivateQuestLater(<active name or a literal>, 0).
  Variants: which flag the if tests (the arm order), the bool arguments (read them push by push: the first push is the
  LAST argument -- 0x00D04260 took one grade for a swapped SetQuestAsFailed pair), a real failure text
  (L"TEXT_QST_035_QUEST_FAILED" + DisplayQuestInfo(false), 0x00D9B2F0), SetExperienceSpendingAsEnabled(true) before the
  outcome test (0x00D62140), SetPrizeTavernTable(true) / Pause(1.0f) / FadeScreenIn() in the success arm (0x00D04260),
  a named-quest DeactivateQuestLater("Q_...") through an implicit CCharString(const char*, long = -1) temporary (0x00D68600,
  0x00D62140). Shape lesson: the landed CQ_WizardBattleScript::WatchForTermination 0x00E1AD60 form matches every variant on
  the first probe; the L"" message as an implicit CWideString(const unsigned short*) conversion temporary.
- **STLport `vector<T>::_M_insert_overflow(pos, x, const __false_type&, fill_len, atend)` (2):** 0x00D18CA0
  (T = NScript::CQ_BountyHuntScript::TeleportData, 24 bytes) and 0x00F0D4E0 (T = NScript::CQ_AmbushTradersScript::TeleportData,
  28 bytes, holds a vector<CCharString>). STLport 4.6 verbatim with the allocator over malloc, `(max)` by reference, placement
  new with its null test, out-of-line copy ctor and `_M_clear()`. Shape lesson: `__uninitialized_copy` must be
  `__forceinline` -- plain `inline` keeps the prefix copy inline but calls the suffix copy out of line (266v280).
- **0x00EFC300 NScript::CHeroTurningDebuggerScript::MoveHeroTest (1):** `while (!term) { { handle; StartScriptingEntity(GetHero(),
  handle, 4); handle.MoveToPosition(GetThingWithScriptName("MarkerToFace").GetPos(), 0.0f, 0, false, true); Pause(30.0f); }
  NewScriptFrame(); }`. Shape lesson: the handle is a block-scoped local destroyed before NewScriptFrame; its implicit dtor
  releases PImp inline and the inline `~CScriptGameResourceObjectBase() {}` stores the base vtable 0x0126008C before the
  out-of-line ~CBaseObject; the virtual MoveToPosition is called directly on the local (devirtualised); GetPos stays a
  vtable call on the by-value temporary. Matched on the first grade.

## Names and their evidence (all proven; none descriptive)
- Every WatchForTermination row: the row address is referenced exactly once (xref), as the CSpawnedFunc member pointer
  stored at +0x34 right after the "WatchForTermination" CCharString (0x012C84B0) inside that quest's Main. The Mains are
  landed (0x00D00AF0 BanditCamp, 0x00F018B0 CinemaTest, 0x00D61D20 WoodsDeparture) or slot 2 of the quest vtable the landed
  ctor/RegisterMain stores (0x00DC27C0 in 0x012DA11C, ctor 0x00DC2570; 0x00D67890 in 0x012D1E08, ctor 0x00D677A0;
  0x00D9A190 in 0x012D65CC, ctor 0x00D99E40; 0x00D038E0 in 0x012C88E0, RegisterMain 0x00D03820). Ego_r.pdb holds
  `?WatchForTermination@<class>@NScript@@QAEXXZ` for all seven classes. Member offsets = Ego_r - 0x14 (the PDB flag names).
- 0x00EFC300: referenced once at 0x00EFC2BA in the landed CHeroTurningDebuggerScript::Main 0x00EFC260 (thread
  "MoveHeroTest"); Ego_r.pdb `?MoveHeroTest@CHeroTurningDebuggerScript@NScript@@QAEXXZ`.
- 0x00D18CA0 / 0x00F0D4E0: sole callers are the landed push_backs 0x00D18C00 / 0x00F0D490 (`_M_insert_overflow(_M_finish,
  x, __false_type(), 1, true)`); element sizes from the divide/multiply constants; protected per STLport 4.6 (IAE).
- Callees: CScriptBase::IsActiveThreadTerminating `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` (Ego_r; queue rows 534/571
  already fix the other spellings), CWideString `??0CWideString@@QAE@PBG@Z` / `??1CWideString@@QAE@XZ`, CCharString
  `??0CCharString@@QAE@PBDJ@Z` / `??1CCharString@@QAE@XZ`, CBaseObject `??0CBaseObject@@IAE@XZ` / `??1CBaseObject@@MAE@XZ`
  (Ego_r), MoveToPosition `?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z`
  (Ego_r + vtable 0x0127094C slot 4), `??1CBaseClass@@UAE@XZ`, malloc, the TeleportData copy ctors
  `??0TeleportData@CQ_<..>Script@NScript@@QAE@ABU012@@Z`, `_M_clear` IAE (landed rows). Vtables `??_7CScriptThing@@6B@`,
  `??_7CScriptGameResourceObjectBase@@6B@`, `??_7CScriptGameResourceObjectScriptedThingBase@@6B@`.
- Interface slots (virtual calls, not relocated) are named from ghidra_out/cgsi_slots_aligned.tsv, each checked against the
  retail CGameScriptInterface vtable 0x01260F0C entry: 7 NewScriptFrame 0x006E7410, 8 StartScriptingEntity, 70 GetHero,
  72 GetThingWithScriptName, 281 DeactivateQuestLater 0x00892F10, 288 SetQuestAsCompleted 0x00892F80, 289 SetQuestAsFailed
  0x00892FD0, 321 DisplayQuestInfo 0x00891920, 374 FadeScreenIn 0x0088E4C0, 376 Pause 0x0088E4F0, 572
  SetExperienceSpendingAsEnabled 0x0088F460, 655 GetActiveQuestName 0x00891880, 866 SetPrizeTavernTable 0x0088FB20 (landed
  under that name). Const-ness as the PDB mangles them (UBE vs UAE).
- Strings paired by hand (vland skips literal relocations): L"" 0x0122D70C (all fail arms), L"TEXT_QST_035_QUEST_FAILED"
  0x012D67F0, "Q_GuildTrainingWoodsWill" 0x012C5AC4, "Q_GuildTrainingWoodsDeparture" 0x012CDFD8, "MarkerToFace" 0x012F6C98.

## Parked for Fable 5.1 (0)

## Skipped
None (every extent checked: `sub esp` first, one `ret`/`ret 14h` last, no int3 inside).

## Label corrections
- 0x00D01290 `CScriptBase_WaitForEntityAndExecuteOnePair` -> NScript::CQ_BanditCampScript::WatchForTermination
- 0x00DC3BD0 `Script_Assignment_Failed` -> NScript::CQ_OpeningGraveyardSecretPassageScript::WatchForTermination
- 0x00F01A40 `Script_Hero_State_Thread` -> NScript::CQ_CinemaTestScript::WatchForTermination
- 0x00D68600 `Quest_GuildTrainingWoods_Will_Init` -> NScript::CQ_GuildTrainingWoodsWillScript::WatchForTermination
- 0x00D9B2F0 `Script_Quest_Failed_Notify` -> NScript::CQ_HobbeToothContestScript::WatchForTermination
- 0x00F0D4E0 `NScript_CQ_AmbushTradersScript_TeleportDataEmplace` -> std::vector<NScript::CQ_AmbushTradersScript::TeleportData>::_M_insert_overflow
- 0x00D18CA0 `TeleportData_OnReadFinished` -> std::vector<NScript::CQ_BountyHuntScript::TeleportData>::_M_insert_overflow
- 0x00D62140 `Quest_GuildTrainingWoods_Departure_Init` -> NScript::CQ_GuildTrainingWoodsDepartureScript::WatchForTermination
- 0x00D04260 `CScriptBase_WaitForEntityAndExecuteTwoPair` -> NScript::CQ_BanditCampBossBattleScript::WatchForTermination
- 0x00EFC300 `Script_MarkerToFace_Thread` -> NScript::CHeroTurningDebuggerScript::MoveHeroTest
- Catalog Mains still unnamed: 0x00DC27C0, 0x00D67890, 0x00D9A190 are `_gapscan::sub_*` but are Main (slot 2) of
  CQ_OpeningGraveyardSecretPassageScript / CQ_GuildTrainingWoodsWillScript / CQ_HobbeToothContestScript; 0x00D038E0 is
  CQ_BanditCampBossBattleScript::Main.

## Header debt
- engine/CBaseObject.h (and the other generated script headers, CScriptThing.h, CGameScriptInterface.h,
  CScriptGameResourceObjectScriptedThingBase.h) are flattened: the vptr is a data member, so they cannot be polymorphic
  bases. 0x00EFC300 models CBaseObject / CScriptGameResourceObjectBase / CScriptGameResourceObjectScriptedThingBase /
  CScriptThing locally (`// Layout:` line, which clears the vland SHARED_HEADER_AVAILABLE warning); the quest rows model CScriptBase /
  CLevelScriptBase / CGameScriptInterfaceBase locally (no engine header exists for them), as the landed quest rows do.
- CLevelScriptBase is modelled non-template (`CScriptDataBase* MasterScriptData`) in the WatchForTermination rows, as the
  landed BanditCamp ctor 0x00D006A0 does: the master data type enters no symbol of these functions. 0x00EFC300 keeps the
  landed Main's `CLevelScriptBase<CQ_SunnyvaleMasterData>`.
