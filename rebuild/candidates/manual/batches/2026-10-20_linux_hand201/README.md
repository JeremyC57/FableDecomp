# Hand chunk 201 (CPlayer .. CQ_FireHeartScript): 29 of 40 byte-exact

All 29 rows in `src/` re-graded RELOCATION_MATCH with `gall.sh` at the end of the run (two need `/GS`, recorded as
`// Flags: /GS`). Every row has a self-contained behaviour test in `test/` that passes under g++ (`tall.sh`) and VC7.1
under Wine (`tall.py`). The run was resumed after a container restart: the 15 sources from the first run were
re-graded and re-tested before the remaining rows were authored. 10 rows are parked in `differ/` and 1 is skipped.

## What matched (29)

- **CPlayer** (7, first run): SetUpControlsBasedOnAggressiveMode 0x00487DE0 (local CDefPointer copy, Release() with no
  null test, action temporary); InitCharacterFromSave 0x00487EF0 (camera out pointer in its own block, nested success
  path, inline PhysicsTC accessor); UnInitActiveMode 0x00633BF0 / InitActiveMode 0x00633CB0 (`/GS`, current mode in a
  local, 8-byte member-function pointer through `pointers_to_members`); InitModeBoastUI 0x00633F10 (engine/CPlayer.h,
  new-expression into SetView); InitModeFirstPersonTargeting 0x00635A50 (zero-initialised out-parameter GetInterface).
- **CPlayerManager** (2, first run): SaveGameState 0x00449F90 (CPlayerSave frame local, inline by-value EmptyString());
  DestroyPlayers 0x0044A070 (`__forceinline` GetPlayer, STLport clear() as erase(begin, end)).
- **CProgressDisplay** (2, first run): WordWrapText 0x00497A60 and its free `__fastcall` twin 0x009BC900 (box width
  with the left edge in a local).
- **CHeroBoastsScript** (5, first run; catalog says CQR_EscortTraderScript::WatchForRegionLoads): boast threads
  0x00CE5180 / 0x00CE5EF0 / 0x00CE5FC0 / 0x00CE63A0 / 0x00CE6550, one template (`gen_boast.py`): the rotated watch
  loop with re-checked termination before each SetBoastAsCompleted / SetBoastAsFailed.
- **Library sound loop** 0x00C37840 (catalog CPlayer::OnModeActivityUpdate): a 16-byte {owner, voice, sound id,
  volume} record's update. The 0x50-byte play parameters are a local whose inline constructor runs the two member
  constructors with the scalar initialisers in declaration order; the filter setter 0x00C20440 is a plain thiscall
  member (the landed definition models it as a free `__fastcall` with a dummy edx argument).
- **CQR_EscortTraderScript::WatchForFailure** 0x00E1F2D0: `bool waiting = !(hero.MsgIsKilledBy("") || ...)` in a
  `for (;;)`: the named bool gives the frame byte, the conversion temporary's destruction flag goes to ebx.
- **CQR_EscortTrader_ManagerScript::Activate** 0x00E255D0: inline loop-test helper with a named region string and
  `? true : false` (setne bl), quest name as an implicit conversion temporary.
- **CQ_CinemaTestScript::WatchForLeaving** 0x00F02030: top-tested `while (a && b && !c && !d)`; with no named bool
  the destruction flag lives in a frame word and the condition rides in bl.
- **CQ_AmbushTradersScript** (4): WatchForBanditSpeechTimer 0x00F035E0 (`for (;;)` closed by a termination return,
  the break path ends in a discarded IsActiveThreadTerminating() tail jump); WatchForSpySpeechTimer 0x00F036C0
  (`while (!terminating)` around two rotated waits, inline CTimer GetTime/SetTime over g_pGameScriptInterface);
  CheckForValidRegion 0x00F0B890 (one `||` chain over three conversion temporaries: VC folds the destruction mask to
  `mov ebx,1/3/7`); WatchForBanditsDeath 0x00F0B960 (size() re-read per test, `*(begin() + i)`, unsigned index).
- **CQ_BountyHuntScript::WatchForQuestInfoDisplayConditions** 0x00D17DA0: writing the inactive-boast path and the
  post-removal path as separate `IsActiveThreadTerminating(); return;` arms lets VC cross-jump into the tail *after*
  `mov ecx,esi`, as retail does (the shared post-`if` tail landed 2 bytes early).
- **CQ_FireHeartScript** (4): SetupPuzzleBlocks 0x00D36140 (`Pattern[GoodRounds * 5 + EvilRounds].PadActive[i]`;
  retail vector<bool> is byte-based); IncNumberOfSuns 0x00D38310 / DecNumberOfSuns 0x00D38AA0 (the difference
  `NumberOfSuns - StartNumberOfSuns` written out twice, a named local rotates eax/ecx; Inc's leading termination test is
  hoisted by VC above the arm test); CheckCompletion 0x00D3AD60 (`new CSpawnedFunc0<CQ_FireHeartScript>("StartRound",
  0, this, &StartRound)` with a flag-guarded name temporary, the cutscene name built in PlayCutscene's argument slot).
- **CQ_NewOakValeIntroScript::WatchForGotGold** 0x00DBE2E0 (catalog CQ_FireHeartScript::CheckGameEnding): gold wait,
  then one nested SetQuestCardObjective(GetActiveQuestName(), "TEXT_...", "", "") call.

## Parked for Fable 5.1 (10)

- 00488620 CPlayer::GetPNearestTCVillage DIFFER(196v196): ThingSearchTools load / filter stores scheduled after the comparator push; lookup key and out pointer homed in frame+4 where retail reuses frame+0 | tried: first-run forms (see differ header) | next: lane gap: frame-slot sharing between the filter and the village local
- 004886f0 CPlayer::GetPNearestTCVillage const DIFFER(196v196): same as 00488620 | tried: the 00488620 forms | next: lane gap (same as 00488620)
- 00635b30 CPlayer::UninitModeFirstPersonTargeting DIFFER(185v185): `this` in esi / map pointer in edi, retail the reverse | tried: first-run forms | next: declare the map-pointer local first (callee-saved colouring is reverse declaration order)
- 00438310 CPlayerGui::ToggleLiveGui DIFFER(182v180): ours `mov eax,ecx` + flag in cl, retail keeps `this` in ecx and the flag in al | tried: first-run forms | next: lane gap: register choice
- 00449d90 CPlayerManager::InitPlayerCharacter DIFFER(195v195): frame 0xC v 8, retail reuses the dead GetDef out slot for the character name and the PLAYER_HERO temporary's slot for the CDefString | tried: block-scoped out slot, names as temporaries | next: lane gap: QFE/RTM frame-slot sharing
- 00d43190 CQ_AwakeningTheOracleScript::CPretendGuard::Main DIFFER(233v226): retail enters the final `while (!IsDone()) NewScriptFrame();` by jumping to its bottom test; the lane copies the test above the body | tried: while, for(;;)+break, goto-to-test do/while | next: lane gap: loop rotation
- 00878ce0 NCameraModes::CProjectileWeapon::SetUpParams DIFFER(208v208): the three ZAxis stores sit between the YAxis copy's stores, retail puts them ahead of the YAxis `lea ecx,[esi+0B5h]` | tried: first-run forms | next: lane gap: store scheduling
- 00e24100 NScript::CQR_EscortTraderScript::GetSuccessType DIFFER(172v172): ours `lea ebx,[eax+990h] ; fcomp [ebx]`, retail `mov ebx,eax ; fcomp [ebx+990h]` (def base kept across GetHealth) | tried: plain `ratio >= def->F`, per-tier `const CScriptDef* def` locals, `def->F <= ratio`, inline by-value float getters | next: bind `const CScriptDef& def = *g_pScriptDef;` per tier, or lane gap: address-vs-base register keeping
- 00d17090 NScript::CQ_BountyHuntScript::WatchForLeaderKilled DIFFER(249v235): the by-value leader's inline ~CScriptThing is expanded at both exits (retail: inline at the terminating return, out-of-line ~CScriptThing 0x004AA840 at the end), and the second copy makes edi a zero register | tried: in-class inline virtual dtor, out-of-class inline dtor, `__forceinline` counted-pointer dtor | next: lane gap: VC7.1 inline budget (retail expands an in-class inline member once per function)
- 00d361f0 NScript::CQ_FireHeartScript::RestoreFollowers DIFFER(184v186): retail `mov eax,[base] ; mov ecx,[off] ; add ecx,eax` for the handle address (ours `mov ecx,[base] ; add ecx,[off]`) and the follower address in eax with the vtable in edx (ours swapped) | tried: `*(begin() + n)` and `_M_start[n]` operator[] | next: lane gap: register/operand order only

## Skipped

- 00449b60 CPlayerManager::GetMultiplayerColour: the row holds a jump-table switch followed by two more function
  bodies (0x00449BF0, 0x00449C10).

## Label corrections

- 0x00CE5180 / 0x00CE5EF0 / 0x00CE5FC0 / 0x00CE63A0 / 0x00CE6550 "CQR_EscortTraderScript::WatchForRegionLoads" are
  NScript::CHeroBoastsScript::BoastBreakSiegeKilledLeader / BoastArenaShowNoMercy / BoastArenaShowMercy /
  BoastTCGTimeLimit / BoastTCETimeLimit (thread names stored by ControlNewBoast).
- 0x00C37840 "CPlayer::CPlayer::OnModeActivityUpdate" is a library sound-loop update (descriptive
  CSoundLoop_00c37840::Update); 0x00C377E0 "CPlayer::GetCurrentMode" and 0x00C377F0 "Fastcall_PlayerActivityCheck" are
  its sound-system accessor and finished-voice check; 0x00C13140 "GetBlockParryReward" is the sound system's play
  request (sound id, 0, &params); 0x00C20440 "bind2nd<greater<long>,long>" is a thiscall filter setter on the play
  parameters; 0x00C203A0 "CAtmosProcess" and 0x00C20390 "CNavigationLayer" are the play parameters' member ctors.
- 0x00DBE2E0 "CQ_FireHeartScript::CheckGameEnding" is NScript::CQ_NewOakValeIntroScript::WatchForGotGold (spawned by
  DoMission 0x00DBE003 under the name "WatchForGotGold"; the real CheckGameEnding is landed 0x00D360E0).
- 0x00D38400 "CTCGameModeController_UpdateSunCount" is CQ_FireHeartScript::TriggerProphetComment(const CCharString&).
- 0x00D39330 "Game_InitializeRound" is CQ_FireHeartScript::PlayCutscene(CCharString).
- 0x00CB7E50 "CGuiVarTransferStruct::Add" is CScriptBase::AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&).
- 0x00D17E70 "CBowerstoneIntroGuard::Init" is a CQ_BountyHuntScript member (DisplayQuestInfo(false), clear
  BoastQuestInfoDisplayed, RemoveQuestInfo(BoastTimerID)); no Ego_r.pdb symbol, named RemoveBoastQuestInfo_00d17e70.
- 0x009BC900 is a free `__fastcall` WordWrapText, not a CProgressDisplay member; 0x00D43190 "CPretendGuard::Main" is
  CQ_AwakeningTheOracleScript::CPretendGuard::Main; 0x00878CE0 is NCameraModes::CProjectileWeapon::SetUpParams.
- Names chosen where landed spellings disagree: 0x0099AED0 is the CWideString default ctor (it sits in the CWideString
  block 0x0099AExx..0x0099B7xx and bumps the wide-string counter; the manifest says CCharString); 0x00CB7E50 uses the
  Ego_r.pdb name AddSpawnedFunction (landed callers spell it `CQ_AmbushTradersScript::Add` / `CQR_EscortTraderScript::Add`);
  0x00CDD450 is `CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long)` (landed:
  `CSpawnedFuncBaseRuntime`); 0x0143E90C is `g_pScriptDef`, 0x0143E8F8 `g_pGameScriptInterface` (the majority landed
  spellings).
