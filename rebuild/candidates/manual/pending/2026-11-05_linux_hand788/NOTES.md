# chunk 788 notes

All six rows RELOCATION_MATCH + behaviour PASS (g++ and VC7.1), plain C++, under the hdr/ overlay. Every row is slot 2 (Main) of its
script vtable (xref.py: the address appears once in .rdata), the vtable is stored by the script's Alloc (directly or through its ctor),
and the Ego_r.pdb name set has ?Main@<Class>@NScript@@UAEXXZ. Binding / thread names come from the literals paired with each Alloc /
member pointer in the row; every bound Alloc is a landed <Class>::<Nested>::Alloc row.

Common model: the landed quest-Main binding model (CQ_TraderConflictGoodScript::Main 0x00DFA450):
`AddEntityScriptBinding(new CEntityScriptBinding<Q>("Name", this, &CName::Alloc, flags))` (the ctor stores ClassAllocValid = true at +0x14
and `flags` at +0x18), `PostAddScriptedEntities(); Game->PostAddScriptedEntities();`, `AddSpawnedFunction(new CSpawnedFunc<Q>("Name", 0,
this, &Q::Fn), CCharString(""))`, WAIT_UNTIL as `while (!cond) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`
followed by its own terminate test.

## 0x00D8DD10 NScript::CQ_HobbeCaveScript::Main (RELOCATION_MATCH, attempt 1)
- Vtable 0x012D5540 (Alloc 0x00D99DC0). Base CLevelScriptBase<CQ_SunnyvaleMasterData>.
- Shape: objective 01/02 by SavedBeforeCaves (each arm with a leading terminate test), 15 bindings (Granny flags 0, others 1), four threads,
  ActivateQuest Q_HobbeCaveBarrier / Q_HobbeCaveGranny, `while (!SavedBeforeCaves && !IsRegionLoaded("GrannysHouse"))`, the once-only
  KickOffQuestStartScreen, then a `for (;;)` key wait (tunnel + key breaks; GreatwoodCaves + key auto-saves once), objective 03, ReachedHobbeCave.
- Callees: interface slots by CGameScriptInterfaceBase.h; the four threads RescuedBandit 0x00D91740, WatchForTermination 0x00D8E990,
  WatchForHavingKey 0x00D8EE50, WatchForGrannysRegion 0x00D8F040 (thread literals + Ego_r name set).
- Label: 0x00D8E990 is landed as a global-namespace `CQ_HobbeCaveScript::WatchForTermination` (NAME_FIXES.tsv).
- Header: new engine/CQ_HobbeCaveScript.h.

## 0x00D84D60 NScript::CQ_HeroSoulsGuildmasterScript::Main (RELOCATION_MATCH, attempt 2)
- Vtable 0x012D4804 (ctor 0x00D84B50, Alloc 0x00D87A80 -- no catalog name, LABELS.tsv).
- Shape: 7 bindings (flags 0,1,1,0,0,0,0), KilledBriar autosave, arrival cut-scene once, a local CTimer taunt timer (200) in the GuildWoods
  wait, GM start cut-scene, card / start screen / objective 14 / FadeScreenIn, the guard-count conversation loop, then the GM_CHOICE
  outcome on `cs_flags["GMKILLED"]` (std::map<CCharString,bool>::operator[] 0x008ADF10, out of line).
- Key spellings: FadeScreenIn written at the end of BOTH arms (a1 with one call after the if/else: 119 md lines of eax/edx swaps);
  `SetQuestAsFailed(name, false, L"", true)` with the implicit CWideString temporary (FABLE_RETAIL_CWIDESTRING) for retail's `lea` re-take;
  CTimer with plain inline ctor/dtor (pDSTGame 0x0143E8F8 RegisterTimer / DeregisterTimer) and __forceinline operator= / GetTime gives
  retail's inline-dtor-at-hot-exits / call-0x00CD4470-elsewhere split; pGlobalScriptDef 0x0143E90C ->HS_GoodSoulTaken (+0xC9C).
- Callees: PlayCutscene 0x00D872D0, AddLogbookStoryEntryString 0x00CBE960 (__fastcall), interface slots.
- Header: new engine/CQ_HeroSoulsGuildmasterScript.h.

## 0x00D7C890 NScript::CQ_HeroSoulsArenaScript::Main (RELOCATION_MATCH, attempt 1)
- Vtable 0x012D3994 (Alloc 0x00D80BA0). Header: repo engine/CQ_HeroSoulsArenaScript.h unchanged.
- Shape: ArenaLevelReached state blocks (ALR_* enum), three threads, once-only cut-scenes, Jack's pre-arena message, `while (!KilledThunder)`
  with the arena-start block (a local `std::vector<CScriptThing>` filled by GetAllThingsWithScriptName: inline dtor on the main path,
  ~vector 0x008AC970 at the early exit, as retail) and the CollectedSoulFromArena `break`.
- Label: 0x00D7FE90 WatchForRoundsInArena has no catalog name (LABELS.tsv).

## 0x00F024C0 NScript::CQ_AmbushTradersScript::Main (RELOCATION_MATCH, attempt 1)
- Vtable 0x012C6CAC (slot 3 = landed Init 0x00CF9A50; Alloc 0x00CFA2F0 -> ctor 0x00CF9350). Retail compiled Main in the second
  AmbushTraders TU (with the entity scripts and threads), not with Init.
- Shape: `CCharString region;` first, 10 bindings (all flags 1), 9 threads, objective 01, DoQuest thread, fail/success wait, the
  three-reason SetQuestAsFailed chain with wide literals and a final `else if (IsActiveThreadTerminating()) return;`, success completes
  and waits on `!SpyExited && !MsgOnRegionLoaded(region)`, WatchForRegionClear, region wait, three DeactivateQuestLater on the phase
  sub-quests plus the quest, generators back on.
- Labels: 0x00F032C0 WatchForHeroDeath and 0x00F03330 WatchForAllTradersDead have no catalog name (LABELS.tsv).
- Header: opt-in block in engine/CQ_AmbushTradersScript.h (thread methods + nested classes); landed includers rechecked.

## 0x00E693F0 NScript::CV_ChapelOfEvilScript::Main (RELOCATION_MATCH, attempt 2) -- class proven from the vtable
- Class proof: 0x00E693F0 is slot 2 of vtable 0x012E8080, whose slot 1 is the landed CV_ChapelOfEvilScript::RegisterMain 0x00E69030 and
  slot 3 the landed Init 0x00E68FA0; the "V_ChapelOfEvil" Alloc 0x00E6E230 stores it; Ego_r.pdb ?Main@CV_ChapelOfEvilScript@NScript@@UAEXXZ;
  the bindings are the CCOE_* entity classes and the threads spawn through CSpawnedFunc<CV_ChapelOfEvilScript> 0x012E8094.
- Shape: 3 bindings (flags 0 -- a1 used 1 and VC7.1 then kept constant 1 in edi instead of 0: 56 md lines), region local, a
  WatchForRegionEntry-shaped loop (`for (;;) { if (term) return; WAIT_UNTIL(MsgOnRegionLoaded(region)); if (region.Contains("Darkwood"))
  break; NewScriptFrame(); }` -- VC7.1 rotates the loop-top test into a pre-test plus a bottom test with its own epilogue copy), two
  terminate tests, five threads, the once-only QuestActivated block (ActivateQuest, HeroReceiveMessageFromGuildMaster, GiveHeroQuestCardDirectly,
  objective 01), two more threads, then the DarkwoodChapel loop (same rotation): WAIT_UNTIL(InsideChapel), `CScriptThing altar =
  GetThingWithScriptName("M_SoundAltar")`, first visit (intro conversation x3 lines when IsAlive, ChapelIntroComplete, objective 02,
  MoralityInitial = GetHeroMorality()) or later visits (NOT_WELCOME / WELCOME_BACK by AttackedByHero, WeaponWarningComplete = false).
- Key spellings: `region.Contains("Darkwood")` = new FABLE_RETAIL_CCHARSTRING_CONTAINS opt-in (FableWin ?Contains@CCharString@@QBE_NABV1@@Z);
  `region == "DarkwoodChapel"` with FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ + FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE (repe cmpsb vs ""
  / Compare 0x00411570 + neg/sbb/inc). CScriptThing::IsAlive is called out of line (0x004AB130) -- the default header path.
- Callees: interface slots 70 GetHero, 72 GetThingWithScriptName, 157 GetHeroMorality, 274/276/295/296, 364-366 conversation, 655
  GetActiveQuestName; strstr 0x00BFEB3A (vland RELOC_ALIAS: the MSVCR71 import thunk -- same as landed 0x00E6E050).
- Labels / names: 0x00E69F90 is WatchForQuestInfoDisplayConditions (catalog/landed WatchForQuestInfoDisplay); 0x00E6A0B0, 0x00E6A340,
  0x00E6A7F0, 0x00E6A9E0, 0x00E6AF20 are landed with a global-namespace CV_ChapelOfEvilScript (NAME_FIXES.tsv); 0x00E6A940
  WatchForLeaving unnamed. (0x00D27C00 "CV_ChapelOfEvilScript::EndMission" is already flagged doubtful in its own landed row.)
- Header: new engine/CV_ChapelOfEvilScript.h; fable_string.h opt-in.

## 0x00EDB190 NScript::CV_TempleOfLightScript::Main (RELOCATION_MATCH, attempt 1)
- Vtable 0x012F3C68 (slot 1 RegisterMain 0x00EDAE70, slot 3 Init 0x00EDAE20; Alloc 0x00EDEB40); CSpawnedFunc<CV_TempleOfLightScript> 0x012F3C7C.
- Shape: the Chapel Main with 2 bindings (flags 0), Contains("Witchwood"), threads WatchForAcolyteExit / WatchForTermination /
  WatchForWeaponUnsheathed / WatchForQuestInfoDisplayConditions / DoMission, QuestActivated block (ActivateQuest + guild master message only),
  WatchForInsideTemple / WatchForDonationConditions, then == "Witchwood3": WAIT_UNTIL(InsideTemple), altar, MiniMapAddMarker(altar,
  "HUD_ORB_QUEST_VIGNETTE") (slot 350), first visit adds `while (IsConversationActive(conversation))` (slot 368) after the three lines and
  moves the quest card + objective 01 into the first-visit block.
- Labels / names: 0x00EDC450 is landed and catalogued as CV_ChapelOfEvilScript::DoMission -- it is NScript::CV_TempleOfLightScript::DoMission
  (Temple Main's "DoMission" thread, the same CSpawnedFunc vtable 0x012F3C7C, calls Temple EndMission 0x00EDC640, waits for Witchwood3);
  its callee 0x00EDC5A0 is CV_TempleOfLightScript::WatchForLeaving (FableWin.pdb); 0x00EDBD10 is WatchForQuestInfoDisplayConditions.
- Header: new engine/CV_TempleOfLightScript.h.

## Descriptive names
None in the row sources: every function, callee, member, class and literal-derived name above is a PDB spelling (Ego_r.pdb, or FableWin.pdb
for CCharString::Contains and CV_TempleOfLightScript::WatchForLeaving). Test-local names (Sim, Ev, Frame, ...) are test scaffolding only.
Local variable names in the sources (region, altar, conversation, tauntTimer, tauntGiven, messageGiven, bodyguards, door, guildmaster) are descriptive.

## Header debt
- `enum ChapelOfEvilTortureType` enumerators are not recovered (declared empty in engine/CV_ChapelOfEvilScript.h; the landed Init
  0x00E68FA0 / GetTortureSoundEffect 0x00E6DE10 rows keep local models).
- The landed Chapel / Temple siblings still use local class models (CQuestScriptBase, global-namespace structs); retyping them onto the new
  headers would fix the NAME_FIXES spellings in one pass (retype_landed.py).
- fable_definition_table.h is in hdr/ only to keep fable_string.h single-copy under the overlay (see hdr/README.md).
