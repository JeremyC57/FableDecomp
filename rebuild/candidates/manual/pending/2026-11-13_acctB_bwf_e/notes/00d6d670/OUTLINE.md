# OUTLINE 0x00D6D670 NScript::CQ_HangingTreeEvilScript::Conclusion() -- 5786 B, thiscall, plain `ret`

## Frame facts
- `sub esp,0FCh; push ebx; push ebp; push esi; push edi`; no ebp frame, no SEH, no /GS. Epilogue 0x00D6ECFF: pop edi/esi/ebp/ebx,
  `add esp,0FCh; ret`. ebx = 0 throughout; esi = this.
- Every `if (IsActiveThreadTerminating()) return;` jumps to the shared exit pad 0x00D6EC87 (destroys the function-scope locals),
  except block-2's inner wait loop, which first runs `~CSGRO` on its local (0x00D6E8B2) then jumps to 0x00D6EC87.

## Frame map (offsets from `body` = esp after the 4 pushes; locals live at body+0x10..0x10B)
| body+ | local | type | ctor / dtor |
|---|---|---|---|
| 0x10,0x14,0x1C,0x40 | CCharString temporaries (reused) | CCharString | 0x0099EBF0 / 0x0099EAE0 |
| 0x20..0x3C | temporaries CScriptThing (returned by value, inline release + ~CBaseClass), later block scopes: block-1 `hero` CSGRO @0x20 + map `cast` @0x30; block-2 `hero` CSGRO @0x30 + `cast` @0x20; block-3 `cast` @0x20 + `movie` @0x30 | | |
| 0x47 | `bool timeUp = false` (set to 0 at 0x00D6D73D) | bool | -- |
| 0x48 | compiler cond-temporary flags of the `||` at B14 (zeroed at 0x00D6D689) | int | -- |
| 0x4C | `freedomFighter` (decl #3) | CScriptThing | GetRandomThingWithScriptName ret / ~CScriptThing 0x004AA840 at exit |
| 0x58 | `guard1` (decl #1) | CScriptThing | CreateCreature ret / 0x004AA840 |
| 0x64 | `prisonerH` | CSGRO | inline ctor / 0x007E74D0 at exit |
| 0x74 | `executionerH` | CSGRO | inline / 0x007E74D0 |
| 0x84 | `guard1H` | CSGRO | inline / 0x007E74D0 |
| 0x94 | `guard2H` | CSGRO | inline / 0x007E74D0 |
| 0xA4 | `guard2` (decl #2) | CScriptThing | CreateCreature ret / 0x004AA840 |
| 0xB0 | `prisMarker` = GetThingWithScriptName("MK_HTG_EXEC_PRIS") (decl #5) | CScriptThing | 0x004AA840 |
| 0xBC | `execMarker` = ("MK_HTG_EXEC_EXEC") (decl #4) | CScriptThing | 0x004AA840 |
| 0xC8 | `spawnPoint` = ("HTE_PartySpawnPoint") (last decl; never used again) | CScriptThing | 0x004AA840 (first at exit) |
| 0xD4 | 5th CSGRO handle, constructed inline at 0x00D6DF21, never used except its exit dtor | CSGRO | inline / 0x007E74D0 |
| 0xE4 | block-1 `movie` (CSGROMovieBase); block-2 `movie`; block-3 `hero` CSGRO | | |
| 0xF4 | `prisPos` = prisMarker.GetPos() copy | C3DVector | -- |
| 0x100 | `execPos` = execMarker.GetPos() copy | C3DVector | -- |
Exit order (0x00D6EC87): ~CScriptThing(0xC8); ~CSGRO 0xD4, 0x94, 0x84, 0x74, 0x64; ~CScriptThing 0xB0, 0xBC, 0x4C, 0xA4, 0x58.
=> declaration order: guard1, guard2, freedomFighter, execMarker, prisMarker, (execPos, prisPos), prisonerH, executionerH,
guard1H, guard2H, unusedH(0xD4), spawnPoint. (Reconstruct with these as function-scope locals declared where first assigned
-- VC7.1 destroys them all at the single exit.)

## Body (retail order)
B1 0x00D6D670  Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_02",
               "ExecutionTree", "BowerstoneSlums");   // strings built last-arg first; dtors: name, TEXT, Exec, Bowerstone
B2 0x00D6D6FF  Game->EntitySetAsOpinionSource(Game->GetHero(), "OPINION_SOURCE_HERO_EVIL_HIGH");
B3 0x00D6D732  Prisoner = Game->GetThingWithScriptName("HTE_Prisoner");  // inline CScriptThing::operator=: ShareData 0x007044D0
               on &Prisoner.PImp (ebp = this+0x84), then the temp's inline dtor (vptr 0x01238C8C store, release, ~CBaseClass)
B4 0x00D6D7BA  if (Prisoner.IsDead()) {            // virtual slot 76 on the member; false -> 0x00D6D91F
                 CScriptThing marker = Game->GetThingWithScriptName("MK_HTE_OUTRO_PRIS1");   // named: inline GetPos
                 Prisoner = Game->CreateCreature("CREATURE_PRISONER", marker.GetPos(), "HTE_Prisoner", false);  // 0x00D6D84C
                 Game->MiniMapAddMarker(Prisoner, "HUD_ORB_GREEN_SMALL");                     // 0x00D6D8D6
               }                                   // marker inline dtor 0x00D6D8E5..0x00D6D91A
B5 0x00D6D91F  Executioner = Game->GetThingWithScriptName("HTE_Executioner");  // ShareData on this+0x78
B6 0x00D6D9A0  if (Executioner.IsDead()) {         // ebp = &Executioner (this+0x74) from here on; false -> 0x00D6DADF
                 CScriptThing marker = Game->GetThingWithScriptName("MK_HTE_OUTRO_EXEC");
                 Executioner = Game->CreateCreature("CREATURE_EXECUTIONER", marker.GetPos(), "HTE_Executioner", false);
               }
B7 0x00D6DADF  Game->ModifyThingHealth(Executioner, 150.0f /*0x43160000*/, false);
B8 0x00D6DAF1  CScriptThing guard1 = Game->CreateCreature("CREATURE_BS_GUARD",
                   Game->GetThingWithScriptName("MK_HTE_GUARD1").GetPos() /*virtual on temp*/, "GUARD1", false);  // into body+0x58
               // strings built "GUARD1", "MK_HTE_GUARD1", "CREATURE_BS_GUARD"; temp marker inline dtor; dtors 0x10,0x14,0x1C
B9 0x00D6DBB5  CScriptThing guard2 = ... same with "MK_HTE_GUARD2", "GUARD2";   // into body+0xA4
B10 0x00D6DC7C CScriptThing freedomFighter = Game->GetRandomThingWithScriptName("HTE_FreedomFighter");
               if (!freedomFighter.IsAlive()) {    // inline: PImp.Data==0 -> create; else virtual slot 75 on PImp; alive -> 0x00D6DDF8
                 freedomFighter = Game->CreateCreatureNearby("CREATURE_BANDIT_GRUNT", Game->GetHero().GetPos(), 3.0f,
                                                         "HTE_FreedomFighter", false);   // Game cached in a spill; ShareData on body+0x50
                 Game->MiniMapAddMarker(freedomFighter, "HUD_ORB_GREEN_SMALL");
                 Game->AddQuestInfoBarHealth(freedomFighter, CRGBColour(255,255,255,255), "HUD_QUEST_ICON_BANDIT", 1.0f);
               }                                   // (CRGBColour temp at body+0x1C, 4 byte stores of 0xFF)
B11 0x00D6DDF8 CScriptThing execMarker = Game->GetThingWithScriptName("MK_HTG_EXEC_EXEC");
               CScriptThing prisMarker = Game->GetThingWithScriptName("MK_HTG_EXEC_PRIS");
               C3DVector execPos = execMarker.GetPos();   // inline (NullCoord 0x0143E8E0), copied to body+0x100
               C3DVector prisPos = prisMarker.GetPos();   // body+0xF4
               CSGRO prisonerH, executionerH, guard1H, guard2H, unusedH;   // 5 inline ctors 0x00D6DEBB..0x00D6DF5B (edi = 0x0127094C)
B12 0x00D6DF42 Game->EntitySetInFaction(Executioner, "FACTION_VILLAGER");
               Game->EntitySetThingAsEnemyOfThing(Executioner, Game->GetHero());
               Game->StartScriptingEntity(Prisoner, prisonerH, 4);        // results ignored
               Game->StartScriptingEntity(Executioner, executionerH, 4);
               Game->StartScriptingEntity(guard1, guard1H, 4);
               Game->StartScriptingEntity(guard2, guard2H, 4);
B13 0x00D6DFEE {  // block 1 -- OUTRO cut-scene, everything inline
                 CSGRO hero;  Game->StartScriptingEntity(Game->GetHero(), hero, 4);      // body+0x20
                 std::map<CCharString,CSGRO> cast;                                        // body+0x30, malloc(0x24) 0x00D6E020
                 cast["HERO"] = hero; cast["PRIS"] = prisonerH; cast["EXEC"] = executionerH;
                 cast["GUARD1"] = guard1H; cast["GUARD2"] = guard2H;   // operator[] 0x00CD3D2E + inline operator= (0x0099A3B0 + PImp share)
                 CSGROMovieBase movie;  Game->StartMovieSequence("", movie);              // body+0xE4, inline ctor
                 CGameScriptInterfaceBase* game = Game; game->PauseAllNonScriptedEntities(true);   // edi = Game
                 RunCutsceneMacro("CS_HANGINGTREE_EVIL_OUTRO", cast, 0, 0, false, true);   // 0x00D6E35F  (NO FixMovieSequenceCamera here)
                 game->PauseAllNonScriptedEntities(false);
               }   // inline ~movie 0x00D6E378, inline ~map 0x00D6E39C (_M_erase/free), inline ~hero 0x00D6E3DD..0x00D6E412
B14 0x00D6E417 long c = Game->AddNewConversation(Executioner, false, false);
               Game->AddLineToConversation(c, "TEXT_QST_029_EXECUTIONER_INTRODUCTION", false, Executioner, Executioner);
               CScriptThing spawnPoint = Game->GetThingWithScriptName("HTE_PartySpawnPoint");   // body+0xC8, unused
               Game->DisplayQuestInfo(true);
               if (!Prisoner.IsAlive()) { if (IsActiveThreadTerminating()) return;      // 0x00D6E4A8
                   ReasonForFailing = "TEXT_QST_029_FAILURE_PRISONER_KILLED"; MissionFailed = true; }
               c = Game->AddNewConversation(guard1, false, false);
               Game->AddLineToConversation(c, "TEXT_QST_029_GUARD_SPOTTED_HERO", false, guard1, guard2);
               if (guard1H.IsValid()) guard1H.Release();
               if (guard2H.IsValid()) guard2H.Release();
               Game->GiveThingBestEnemyTarget(guard1, Game->GetHero());
               Game->GiveThingBestEnemyTarget(guard2, Game->GetHero());
               Game->GiveThingBestEnemyTarget(freedomFighter, guard1);
               if (IsActiveThreadTerminating()) return;                                  // 0x00D6E598
               c = Game->AddNewConversation(Executioner, false, false);
               Game->AddLineToConversation(c, "TEXT_QST_029_EXECUTIONER_RABBLE_ROUSING", false, Executioner, Executioner);
               prisonerH.MoveToPosition(prisPos, 0.0f, (EScriptEntityMoveType)0, false, true);    // direct 0x007E72F0
               Game->Pause(1.0f);
               executionerH.MoveToPosition(execPos, 0.0f, (EScriptEntityMoveType)0, false, true);
B15 0x00D6E623 loop:  while (!ExecutionerInterrupted) {           // +0x56 != 0 -> 0x00D6E7E7
                 Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
                 if (MissionTimer.GetTime() <= 0) {               // pDSTGame->GetTimer(this->MissionTimer.TimerIndex), `jg` skips
                   if (IsActiveThreadTerminating()) return;
                   Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
                   Game->Pause(1.0f);
                   if (Executioner.IsAlive()) { if (IsActiveThreadTerminating()) return; timeUp = true; }   // virtual slot 75 via ebp
                 }
           0x00D6E6B5    if (Executioner.MsgIsHitBy("") || Executioner.MsgIsKilledBy("")
                     || IsDistanceBetweenThingsUnder(Executioner, Game->GetHero(), 3.0f)) {   // flags bit0/bit1 @body+0x48
                   if (IsActiveThreadTerminating()) return;      // result kept in a bool @body+0x1B -> name it (`bool interrupted = ...;`)
                   ExecutionerInterrupted = true; AudienceLeggit = true;
                 }
           0x00D6E786    if (Prisoner.MsgIsKilledBy("")) { if (IsActiveThreadTerminating()) return;   // bool in body+0x1B again
                   ReasonForFailing = "TEXT_QST_029_FAILURE_PRISONER_KILLED"; MissionFailed = true; }
           0x00D6E7DD    if (timeUp) break;   // `cmp [esp+47h],bl; je 0x00D6E623`
               }
B16 0x00D6E7E7 if (IsActiveThreadTerminating()) return;
               if (!ExecutionerInterrupted && Executioner.IsAlive() && Prisoner.IsAlive()) {   // any false -> 0x00D6EA18
                 // block 2 -- EXECUTION (COMDAT forms)
                 if (IsActiveThreadTerminating()) return;
                 CSGRO hero;                                       // 0x007E72A0, body+0x30
                 if (hero.IsValid()) hero.Release();
                 while (!Game->StartScriptingEntity(Game->GetHero(), hero, 4)) {
                   Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;   // -> 0x00D6E8B2 (~hero, exit)
                 }
                 if (IsActiveThreadTerminating()) return;
                 std::map<CCharString,CSGRO> cast;                 // 0x00CDBF70 body+0x20
                 cast["HERO"] = hero; cast["PRIS"] = prisonerH; cast["EXEC"] = executionerH;   // 0x008ABD10
                 CSGROMovieBase movie; Game->StartMovieSequence("", movie);   // 0x006E7B60, body+0xE4
                 CGameScriptInterfaceBase* game = Game; game->PauseAllNonScriptedEntities(true);
                 RunCutsceneMacro("CS_HANGINGTREE_EVIL_EXECUTION", cast, 0, 0, false, true);   // 0x00D6E9CB
                 game->PauseAllNonScriptedEntities(false);
                 // ~movie 0x006E7B80, ~cast 0x00CDBFB0
                 ReasonForFailing = "TEXT_QST_029_FAILURE_PRISONER_KILLED"; MissionFailed = true;
                 // ~hero 0x007E74D0, jmp 0x00D6EC43
               } else {
B17 0x00D6EA18   if (IsActiveThreadTerminating()) return;   // block 3 -- ESCAPE
                 Game->RemoveQuestInfoElement(TimerGUIIndex);
                 Game->DisplayQuestInfo(false);
                 if (executionerH.IsValid()) executionerH.Release();
                 prisonerH.ClearCommands();
                 Game->GiveThingBestEnemyTarget(Executioner, Game->GetHero());
                 while (!Executioner.IsDead()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }  // test at bottom 0x00D6EAA2
                 if (IsActiveThreadTerminating()) return;
                 Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
                 Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
                 prisonerH.ClearCommands();
                 CSGRO hero;  Game->StartScriptingEntity(Game->GetHero(), hero, 4);   // 0x007E72A0 body+0xE4, result ignored
                 std::map<CCharString,CSGRO> cast;  cast["HERO"] = hero; cast["PRIS"] = prisonerH;
                 CSGROMovieBase movie; Game->StartMovieSequence("", movie);       // body+0x30
                 CGameScriptInterfaceBase* game = Game; game->PauseAllNonScriptedEntities(true);
                 Game->FixMovieSequenceCamera(true);
                 RunCutsceneMacro("CS_HANGINGTREE_EVIL_ESCAPE", cast, 0, 0, false, true);   // 0x00D6EBFC
                 Game->FixMovieSequenceCamera(false);
                 game->PauseAllNonScriptedEntities(false);
                 // ~movie, ~cast, ~hero (COMDATs)
                 MissionSucceeded = true;                           // 0x00D6EC3F
               }
B18 0x00D6EC43 if (guard1H.IsValid()) guard1H.Release();
               if (guard2H.IsValid()) guard2H.Release();
               Game->DisplayQuestInfo(false);
B19 0x00D6EC87 exit pad: function-scope dtors (see frame map), ret.

## Open points for the author
- Map keys PRIS/EXEC sources in blocks 1-2 inferred as prisonerH/executionerH (EXEC=body+0x74, GUARD1=0x84, GUARD2=0x94 read
  directly; PRIS=0x64 by analogy) -- confirm from the `lea` before each 0x0099A3B0 / 0x008ABD10.
- The inline-vs-COMDAT split (DOSSIER 5) is the main residue risk; write one source idiom and measure first.
- The 5th handle (body+0xD4) and spawnPoint (body+0xC8) are unused but must exist for the frame and exit dtor order.
