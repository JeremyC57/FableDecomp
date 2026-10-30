# Hand chunk 582 (std::vector<CScriptThing> .. NScript::CQ_WizardBattleScript): 9 of 10 byte-exact

All nine are RELOCATION_MATCH, behaviour tests PASS under g++ (tall.sh) and VC7.1 (tall.py); vland: 9 wins, no RELOC_IDENTITY
(only RELOC_NAME warnings for 0x006E7AB0, see Label corrections). Every name is proven (two sources each, in the file's
`// Identity` line); none is descriptive.

## What matched (9)
- NScript::CV_TalentlessBardScript::WatchForCompletedQuests 0x00ED7A40 -- marks the bard's song things available for already
  completed quests, then waits on MsgOnAnyQuestCompleted and updates SongThingAvailable (QS_ScytheInfo clears slot 3,
  Q_DragonBossFight sets it unless the hero wore the mask). Shape: SongThingAvailable is a 12-byte byte-per-element
  `std::vector<bool>` (retail +0xD4, class 0xE4); `questName == "..."` is the inline CCharString::operator==(const char*)
  over CBasicString<char>::Compare 0x00411570 with its body in the TU under auto_inline(off).
- NScript::CQ_HeroSoulsMotherScript::PlayCutscene 0x00D848D0, CQ_HeroSoulsGuildmasterScript::PlayCutscene 0x00D872D0,
  CQ_HeroSoulsThunderScript::PlayCutscene 0x00D7C470, CQ_WizardBattleScript::CutsceneMazeDeath 0x00E1D7B0 -- the
  CQ_HeroSoulsScript::PlayCutscene 0x00D7AA30 body (pause, hero cast map, movie, RunCutsceneMacro, camera fix, unpause) with
  the per-class map argument (Mother/Thunder csargs +0x4C as args, Guildmaster cs_flags +0x58 as flags), Thunder adding
  `csargs.clear()` (out-of-line 0x009AACE0), WizardBattle using the "Hero" key, no camera fix and SetQuestAsCompleted.
  Shape: the landed model moved onto the shared NScript headers is byte-neutral; real STLport map<> needed.
- NScript::CV_BeardyBaldyScript::OnPersist 0x00E4F7F0 -- 20 CPersistContext::Transfer calls in retail key order (order is
  not member order: QuestActivated before QuestComplete, the kill flags before the Acquired* flags). Members at Ego_r - 0x14.
- NScript::CExpression_FollowScript::Main 0x00EEA020 -- CIsThingConscious(hero) predicate, take the hero under scripted
  control, movie sequence, fade out to black, spawn FollowThread, CameraDefault, fade in. Shape: the spawn must go through the
  `__forceinline SpawnFunction(new ..., "")` helper (new before the region CCharString, both temps to end of statement); a
  local `CSpawnedFuncBase* thread` gave frame 0x38 v 0x3c.
- NScript::CQ_HangingTreeEvilScript::WatchForHeroNearExit 0x00D6CA90 -- collect REGION_EXIT_POINT things, each frame warn the
  hero once (one-line conversation TEXT_QST_029_BANDIT_DONT_EXIT) when within 8 units of one. Shape: real STLport
  `vector<CScriptThing>` local (_Vector_base dtor + _Destroy loop) destroyed inline at all five exits; do-while on the flag.
- NScript::CQ_NewOakValeIntroScript::StartBarrelTimer 0x00DB4F70 -- wait for WatchTimer > 0, AddQuestInfoBar(45, 0, green,
  green, "HUD_CLOCK_ICON", "", 1), then until BarrelManSpokenToHeroOnReturn colour the bar green within 2 units of
  M_WHouse_GuardPoint (red otherwise) and UpdateQuestInfoBar with the watch time; RemoveQuestInfoElement. Shape: inline
  CTimer::GetTime defined after engine/CTimer.h (g_pGameScriptInterface slot 90); members after the eight 12-byte vectors at
  Ego_r - 0x14 - 0x20 (WatchTimer +0x108), and C3DVector must be byte-aligned (Ego_r puts it at +0x8A / +0x99). First grade.

## Parked for Fable 5.1 (1)
- 0x00D34B30 std::vector<CScriptThing>::_M_fill_insert DIFFER(474v480): VC7.1 inlines the CScriptThing copy constructor in both raw-end __uninitialized_copy loops where retail calls 0x004ABE90 (x_copy inline in both) | tried: verbatim STLport over the retail CScriptThing header, #pragma inline_depth(0/1/2) around the explicit specialization (1/2 no effect, 0 un-inlines _Construct too), plain-inline __uninitialized_copy, non-inline same-TU copy body before/after, QFE 4035 | next: same residue as chunk 581's parked vector rows; try a copy ctor declared out of line in the header with the x_copy copy written as an explicit inline expression, else lane gap: inline-depth budget

## Skipped
None.

## Label corrections
- 0x00D848D0 `Main_InitializeFourierAnalysis_1`, 0x00D7C470 `Main_InitializeFourierAnalysis`, 0x00D872D0
  `Script_InitializeComponents` -> PlayCutscene of CQ_HeroSoulsMother / Thunder / Guildmaster Script.
- 0x00E1D7B0 `DoMission_WizardBattleMazeDeath` -> CQ_WizardBattleScript::CutsceneMazeDeath (landed EndMission 0x00E1D640
  calls it `FinishMazeStage`, invented).
- 0x00ED7A40 `TalentlessBard_OnScript` -> CV_TalentlessBardScript::WatchForCompletedQuests.
- 0x00DB4F70 `Script_Guard_Duty` (propagated) -> CQ_NewOakValeIntroScript::StartBarrelTimer.
- 0x00D34B30 `std_vector_InsertRangeWithElement` -> std::vector<CScriptThing>::_M_fill_insert.
- 0x00EEA020, 0x00E4F7F0, 0x00D6CA90 were `sub_` labels.
- 0x009AACE0 landed as `CTCCarryable_OnKill` (and bound as CCutsceneArgMap junk): it is the map<CCharString,CCharString>
  clear (`csargs.clear()`); whether the symbol is map::clear or _Rb_tree::clear is unproven.
- 0x006E7AB0 is labelled `CCountedPointer_Release_006e7ab0`; landed rows call it ~CMovieImpPointer / ~CResourcePImp; this
  chunk binds it as `??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ` (the PImp member dtor of the movie
  resource object, as in landed 0x00D7AA30).
- Landed CV_TalentlessBardScript::OnPersist 0x00ED85A0 models SongThingAvailable as a 0x14-byte bit vector; retail is the
  0xC-byte byte vector (class 0xE4).

## Header debt
- CBaseObject (vtable 0x0129A7D8), CScriptGameResourceObjectBase / ScriptedThingBase / MovieBase (0x0126008C / 0x0127094C /
  0x01260EF4): local models in 00d848d0, 00d872d0, 00d7c470, 00e1d7b0, 00eea020 (no shared retail header yet).
- NScript::CIsThingConscious (vtable 0x012C2FE8) and CSpawnedFunc<T> (CSpawnedFunc<CExpression_FollowScript> vtable 0x012F55CC):
  local in 00eea020.
- CRGBColour (local in 00eea020, 00db4f70), C3DVector (local, `#pragma pack(push, 1)`, in 00db4f70), CCharString,
  CCountedPointer, minimal std::vector / std::map: local, as in every landed NScript row.
- CQ_SunnyvaleMasterData opaque in 00ed7a40 (HeroWoreMask at +0x133 only).
- CGameScriptInterface* g_pGameScriptInterface 0x0143E8F8 extern spelled as the landed rows do (several junk bindings exist).
