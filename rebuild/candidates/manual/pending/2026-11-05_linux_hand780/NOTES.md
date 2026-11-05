# Chunk 780 notes (five NScript quest-script rows; TU flag /Gr everywhere)

Result: 1 matched (0x00DECAD0 RELOCATION_MATCH), 3 parked near misses (0x00E7C010 1 byte, 0x00E4FA20 7 bytes, 0x00EA2080 two
inline dtor copies), 1 not attempted (0x00E6FC40: the catalog row is not one function). Every name is PDB-proven (two sources);
no descriptive names were needed. Grade logs of every attempt: tmp/g_<addr>_<variant>.txt; probe sources: probe/.

## 0x00DECAD0 NScript::CQ_SecretPassageScript::Main -- RELOCATION_MATCH (src/00decad0.cpp, test/00decad0.cpp)
- Ledger label CV_TravellingHeroesScript::Main is wrong (NAME_FIXES): vtable 0x012DDC30 slot 2 stored by the landed
  CQ_SecretPassageScript ctor 0x00DEC7F0, PDB ?Main@CQ_SecretPassageScript@NScript@@UAEXXZ, every literal an SP_ entity of the TU.
- Shape: InitialiseVariables 0x00DED0A0 (manifest label CQ_GuildTrainingScript::CMeleeApprentice::Init is propagated; FableWin
  ?InitialiseVariables@CQ_SecretPassageScript@NScript@@QAEXXZ -- NAME_FIXES), ten AddEntityScriptBinding(new
  CEntityScriptBinding<CQ_SecretPassageScript>(name, this, &CSP_X::Alloc, flags)) (SP_UndeadTrigger with flags 1), the two
  PostAddScriptedEntities, AddLogbookStoryEntry(210) 0x00CBE87F, SetQuestCardObjective(GetActiveQuestName(),
  "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_01", "", ""), spawns WatchForTermination 0x00DED0C0 and DoMission 0x00DEFF60,
  CacheMusicSet(MUSIC_SET_CUTSCENE_PRISON_MOTHER = 53) and (MUSIC_SET_CUTSCENE_JACK_CAPTURES = 56) (engine/EMusicSetType.h).
- Model: the landed quest-Main model (one ebx flag word guarding the name temporaries), headers fable_string.h (FABLE_RETAIL_CCHARSTRING),
  engine/CScriptThing.h, CGameScriptInterfaceBase.h, CLevelScriptBase.h (FABLE_RETAIL_NSCRIPT), CEntityScriptBinding.h,
  CSpawnedFuncBase.h, CSpawnedFunc.h, EMusicSetType.h. vland: RELOCATION_MATCH + test PASS (tmp/vland.log).

## 0x00E7C010 NScript::CV_FishermanScript::NPCAttitudeProcess(CScriptThing) -- parked DIFFER(1624v1623) (differ/00e7c010.cpp = variant j)
- Identity: FableWin ?NPCAttitudeProcess@CV_FishermanScript@NScript@@QAEXVCScriptThing@@@Z; spawned by the landed CFisherman::Init
  0x00E7BD60 through CSpawnedFunc1<CV_FishermanScript,CScriptThing> (SuspendableProcess 0x00E7C670 follows). Callees and interface
  slots: the source header (all checked with objdump -r against retail; handle members called directly: IsValid 0x00CD23B9,
  Release 0x00CD2770, GetScriptThing 0x007E7490, Speak 0x007E7390, IsPerformingScriptTask 0x007E7450, MoveToThing 0x007E7300).
- Shape (the Lua port Fisherman.lua NPCAttitudeProcess agrees): SetPredicate(CIsThingAlive(npc)); NewScriptFrame; wait until
  npc.MsgIsHitBy("SCRIPT_NAME_HERO") || (MsgIsHitByAnySpecialAbilityFrom && MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL))
  with GetHealth(npc) > 1; RelationCounters[2] = 2 (hdr overlay opt-in); FishermanMustRunAway = true (+0x50); scripted-thing
  handle (IsValid/Release, StartScriptingEntity wait, priority 4); movie + RAII CWideScreenMagicPauseEntities; SheatheHeroWeapons;
  NPC_NPCStand / NPC_HeroStand teleports when IsDistanceBetweenThingsUnder(..., 10.0f) 0x00CBE2FF; NPC_Camera CameraUseCameraPoint;
  EntitySetFacingAngleTowardsThing(npc, GetHero()); Speak TEXT_QST_023_IDLE_03_HATE + IsPerformingScriptTask wait when alive;
  MoveToThing(GetThingWithScriptName("MK_FI_HERO"), 3.0f, 1, ...); Pause(1.5f); FadeScreenOut(1.5f, 1.5f, CRGBColour(0,0,0,255));
  Pause(2.0f); RemoveThing(npc,false,false); CameraDefault(); FadeScreenIn() after the pause/movie dtors.
- Residue (all 12 variants, see STATUS.tsv): retail's two prelude terminate checks (0x00E7C054, 0x00E7C063) and the hit-loop
  bottom check (0x00E7C170) share ONE inline ~npc exit block (0x00E7C17F); VC7.1 never produced that sharing: plain flow (e,
  1636: P0 and P1 merged by `||`, B outline), nested-REST `do {} while (!T)` (j, 1624: P0 inline alone, blocks reordered),
  `while (!T)` (f/g), rotated for(;;) (h/i), dwall (c/d: REST exits turn inline). Everything else (REST exits, handle/movie/pause
  tails, cold tail, the RelationCounters store) matches in e and j.
- Header debt: none beyond the opt-in statics (hdr/README.md).

## 0x00EA2080 NScript::CV_KnotholeGladeGatesScript::ControlMainBalverines -- parked DIFFER(2279v1682) (differ/00ea2080.cpp = probe l)
- Identity: PDB name sets; called twice by the landed PhaseOne 0x00EA1630. Callees in the source header (objdump -r checked):
  push_back 0x008ADF90 / ~vector 0x008AC970 (stlport/fable_containers.h with FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE),
  AreAllThingsInVectorDead 0x00CBED00 (__fastcall), ~CScriptThing 0x004AA840, CBaseClass 0x0099A2D0/0x0099A2E0.
- Shape: `std::vector<CScriptThing> balverines; CScriptThing balv1 = GetThingWithScriptName("KGG_Balv1"); CScriptThing target;
  for (long i = 0; i < 1; ++i) { Term; target = GetRandomThingWithScriptName("KGG_VillagerOutside"); if (target.IsAlive()) {
  Term; while (IsCameraPosOnScreen(balv1.GetPos())) wait; Term; balv = CreateCreatureNearby("CREATURE_BALVERINE_01",
  balv1.GetPos(), 6.0f, "KGG_Balverine", false); PlayCriteriaSoundOnThing(TheGate, "KNG_HORN"); SetThingPersistent(balv, true);
  GiveThingBestEnemyTarget(balv, target); EntitySetAsOpinionSource(balv, "OPINION_SOURCE_INANIMATE_EVIL_HIGH"); push_back }
  else { Term; target = GetHero(); same wait/spawn without the opinion source; push_back } } Term; while
  (!AreAllThingsInVectorDead(balverines)) wait; Term;`. Retail reads the KGG_Balv1 marker (F+0x40) for the camera and spawn
  positions (LESSONS); `inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }` gives the
  `test al,al`; the final check folds to a bare call.
- Residue: only the two arm-first checks (0x00EA21B9 / 0x00EA2311) are inline dtor copies in ours and cold-tail jumps in retail;
  /d2inlT62 (diagnostic) reproduces retail's pattern, so they are marginal. 14 forms tried (STATUS.tsv). Layout, frame 0x6c,
  every temporary slot and the cold tail match. Neighbour DoInitialBalverineAttack 0x00EA1720 is still unlanded (not in this chunk).

## 0x00E4FA20 NScript::CV_BeardyBaldyScript::Main -- parked DIFFER(1763v1756) (differ/00e4fa20.cpp = variant a)
- Identity: PDB name sets; RegisterMain 0x00E4F730 spawns "Main"; literals and the seven watcher names in the source header.
  Spawned members (Ego_r/FableWin names, all CV_BeardyBaldyScript): WatchForQuestFinished 0x00E50100 (landed under the wrong class
  CV_AmbushScamScript -- NAME_FIXES), WatchForQuestCardConditions 0x00E50910 (chunk 778 parked), WatchForBarberLadyDeath
  0x00E504C0, WatchForBeardyBaldyDeath 0x00E503D0, WatchForNewHairdo 0x00E50150, WatchForNewBeard 0x00E50210, WatchForNewTash
  0x00E502E0; GoTalkToBeardyBaldy 0x00E50650; CBB_BeardyBaldyMan::Alloc 0x00E507E0. Layout: QuestFinished +0x48, QuestComplete
  +0x49, QuestActivated +0x4a (retail = Ego_r - 0x14, the landed OnPersist 0x00E4F7F0 order).
- Shape (BeardyBaldy.lua Main agrees): bind BB_BeardyBaldyMan; the two PostAddScriptedEntities; `CCharString regionName;`
  (ctor 0x0099E4B0 after Game->PostAddScriptedEntities); Term; region loop `for (;;) { while (!MsgOnRegionLoaded(regionName))
  { NSF; Term } Term; if (regionName == "BowerstoneSlumsWarehouses") break; NSF; Term }` (the inline CBasicString ==: hdr
  overlay opt-in FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE; the bottom check has its own epilogue copy 0x00E4FBB7); Term; Term;
  seven AddSpawnedFunction; `if (!QuestActivated) { Term; QuestActivated = true; }`; GoTalkToBeardyBaldy(); `while
  (!QuestFinished && (!QuestComplete || IsRegionLoaded("BowerstoneSlumsWarehouses"))) { NSF; Term }` (condition bool in
  [esp+0x13]); Term; beardyBaldy = GetThingWithScriptName("BB_BeardyBaldyMan"); barber = GetNearestWithScriptName(GetHero(),
  "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER"); `if (barber.IsAlive()) { Term; MiniMapRemoveMarker; ClearThingHasInformation }`
  (IsAlive inline) and the same for beardyBaldy (IsAlive called, 0x004AB130); SetQuestAsCompleted(GetActiveQuestName(), false x3);
  RemoveQuestCardFromHero; DeactivateQuestLater(..., 0).
- Residue: variant a differs only by VC7.1's lazy `push edi` after the first terminate check. Every form that moves the push to
  the entry (check inside the loop e/f/g, nested body h, `while (!T)` c) cools the barber IsAlive site to a call (the c2
  diagnostic puts it at T* 150-170 against retail's 74), and every do-while(0) lever (q..D, 16 combinations) either does nothing
  or flips both IsAlive sites inline. 27 graded spellings; details in STATUS.tsv.
- Label: 0x00EA3490 (landed as CV_BeardyBaldyScript::WatchForQuestFinished) is referenced only from CV_LostTraderScript::Main
  0x00EA2E80 -- suspect, no PDB name to prove it (LABELS.tsv).

## 0x00E6FC40 NScript::CV_DemonDoorsScript::CDF_Dragon::Main -- not attempted (catalog row needs a split)
- The 1828-byte row = CDF_Dragon::Main 0x00E6FC40..0x00E702A0 (ret 0x00E70290 + cold blocks 0x00E70291..) + the 8-entry switch
  jump table 0x00E702A0 (cases 0..7 of the renown switch, default 0x00E7011B) + CDF_SummonsCreatures::Alloc 0x00E702C0 (__fastcall
  factory, vtable 0x012E8E80, `ret 8`, Ego_r ?Alloc@CDF_SummonsCreatures@CV_DemonDoorsScript@NScript@@SI...) + GetParentScript
  0x00E70360 (`mov eax,[ecx+14h]; ret`). Neither q.py nor vland can grade a source against it; the row must be split (Main with
  RELOC_JUMPTABLE, then the two small CDF_SummonsCreatures rows).
- Decode of Main (for the next agent): entity script (CEntityScriptBase: Game +4, Me +8, ParentClass +0x14); PDB
  ?Main@CDF_Dragon@CV_DemonDoorsScript@NScript@@EAEXXZ (private virtual, slot 2 of vtable 0x012E8DB8); IsActiveThreadTerminating
  0x00F35B30 and RegisterFilter 0x00F35B10 (the entity-script base, not CScriptBase's), filter CIsThingAlive (vtable 0x012C32F0,
  dtor 0x00E241B0); scripted-thing handle (vtable 0x0127094C: IsValid 0x00CD23B9, Release 0x00CD2770, GetScriptThing 0x007E7490,
  Speak 0x007E7390, IsPerformingScriptTask 0x007E7450, dtor 0x007E74D0); movie handle (vtable 0x01260EF4, dtor 0x006E7B80);
  CBaseIntelligentPointer 0x0099A380 / 0x0099A430. Prelude: filter, NewScriptFrame, Term, StartScriptingEntity wait (slot 8),
  SetThingPersistent(Me, true) (+0x774), slots +0x810 / +0x838 / +0x814 (immune / killable flags), GetNearestWithDefName-style
  slot +0x144 with "REGION_EXIT_POINT" 0x012CA510, slot +0x8DC (SetRegionExitAsActive) false. Forever loop: `Me.MsgIsTalkedToBy
  ("SCRIPT_NAME_HERO")` (slot +0x6C) -> movie StartMovieSequence(""), PauseAllNonScriptedEntities(true) (+0x5EC), `switch
  (Game->GetHeroRenownLevel())` (slot +0x288) cases 0..7 through the table: each case `if (GetHealth(handle.GetScriptThing()) > 0)
  { handle.Speak(GetHero(), TEXT_QST_088_DRAGON_RENOWN_{ZERO,ONE,TWO,THREE,FOUR} 0x012E8E60/0x012E8E40/0x012E8E20/0x012E8DFC/
  0x012E8DDC, GROUP_SELECT_NONE, false, true, false); wait IsPerformingScriptTask; Term }`; the top case (0x00E70162) ends with
  ParentClass->FadeOutFaceAndOpenDoor(Me, "DD_Dragon" 0x012E8DD0) 0x00E6F040, slot +0x8DC true and slot +0x450 with
  "Q_DragonBossFight" 0x012C45B4 (ActivateQuest); unpause (+0x5EC false), movie dtor; then NewScriptFrame; Term; loop. Retail
  lazily pushes ebx at 0x00E6FD57 (after the prelude's early return). The landed siblings CDF_ThunderHelmet::Main 0x00E6F1C0 and
  CDF_CombatMult::Main 0x00E6F710 used `#pragma inline_depth` and local stand-ins -- not allowed here; expect the same frequency
  fight as the three parked rows. No DemonDoors Lua port exists under refs/script_recovery.

## Header debt
- fable_string.h: CBasicString<T>::operator==(const T*) has no in-class body on the default path; the quest-script TUs expand it
  (opt-in added here). CCharString's own operator==(const char*) null arm is already inline.
- engine/CQ_SunnyvaleMasterData.h: the three relation statics (opt-in added here).
