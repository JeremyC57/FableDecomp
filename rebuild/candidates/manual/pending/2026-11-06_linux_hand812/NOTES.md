# Chunk 812 notes

Six rows, all PDB-named (each `?Main@...` spelling is in both the Ego_r.pdb and FableWin.pdb name sets, and each address is the only
reference from its vtable slot). Two rows match (RELOCATION_MATCH + PASS); four are parked in differ/ with their tests. No descriptive
names were introduced: every callee, global and class in the sources uses its PDB spelling or an already-bound reloc spelling.

## Matched

### 0x00D2AA40 NScript::CQ_EndGameFocalSitesScript::Main -- RELOCATION_MATCH (a6)
- Shape: the landed quest-Main binding model (FORCEINLINE binding ctor, flag word for the name temporaries), with region waits written as
  `while (!IsRegionLoaded) { NewScriptFrame; T }` + T, and a switch on CurrentFocalSiteRegion (retail's jump table 0x00D2C114).
- Key spellings: `offset += GetHero().GetPos()` in place (retail stores X/Y/Z back); the random angle as its own statement (in one
  expression VC7.1 loads Game before rand and keeps it in ebx: 31 lines).
- Header: engine/CQ_EndGameFocalSitesScript.h with chunk 805's existing JACKOFBLADES opt-in (no hdr/ change).
- Alloc 0x00D2D730 CPermanentBarrier::Alloc (Ego_r + FableWin `?Alloc@CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@SI...`); its
  current label is already right.

### 0x00EA59E0 NScript::CV_LostTraderScript::CLostTraderBrother::Main -- RELOCATION_MATCH (a3)
- The state-4 body must be its own `if (stateBrain == 4) { }` block, so the Trader thing shares HomePos's frame slot and both exit
  through the shared ~CScriptThing pad.
- The hero-lag compare is `GetDistance(Me, Home) < GetDistance(Hero, Home)` (left operand first).
- Header: engine/CV_LostTraderScript.h BROTHER opt-in (WatchForQuestFailureState 0x00EA7460, the thread it spawns).
- Name fix: the landed 0x00EA7460 leaf is `?WatchForQuestFailureState@CV_LostTraderScript@@` (missing NScript); my row binds the correct
  `?WatchForQuestFailureState@CV_LostTraderScript@NScript@@QAEXVCScriptThing@@@Z`. That landed row needs respelling to match
  (NAME_FIXES.tsv). There is no reloc_bindings entry for 0x00EA7460, so this is not yet a one-symbol-two-addresses clash.

## Parked (differ/, each with lines 2-3 and differ/<addr>.test.cpp; every test PASSes)

| Row | Best | Grade | SDIFF / lines | Residue |
|---|---|---|---|---|
| 0x00D0D910 CGate2Guard1::Main | a10 | DIFFER(4928v4752) | 304 / 551 | movie dtor + ~CTimer inline budget; exit pads |
| 0x00D09010 CCampHostageGuard::Main | a10 | DIFFER(5880v5864) | 110 / 203 | movie dtor vptr store; one exit copy; TalkedTo frame byte |
| 0x00CF4EC0 CFanMarker::Main | a6 | DIFFER(6244v6164) | 175 / 231 | nested inline ~CScriptThing inside ~CIsThingAlive; movie dtor |
| 0x00D13580 CQ_BountyHuntScript::Main | a5 | DIFFER(6831v6917) | 248 / 344 | movie dtor; cross-jumped post-loop thread checks |

**The shared blocker is the movie dtor.** Retail expands the implicit `CScriptGameResourceObjectMovieBase` dtor at every site:
`call 0x006E7AB0`, then the GROBase vptr store 0x0126008C, then `call 0x0099A430`. Our build either:
- calls the compiler-generated COMDAT 0x006E7B80 out of line (plain header), or
- expands every site but adds a MovieBase vptr store that retail lacks (`FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE`).

Chunk 810's 0x00D1B200 has the same residue. A spelling of the dtor without the vptr reset would probably unlock three of these four rows.

Per-row details:
- **0x00D13580:** the identity is level vtable 0x012C9E68, slot 2. VC cross-jumps the identical `if (IsActiveThreadTerminating()) return;`
  after the three exit-wait loops (retail has 39 thread checks, ours 37), which moves the shared tail. do-while(0), TRetVoid returns and
  `for(;;)` were all byte-identical.
- **0x00CF4EC0:** retail calls ~CScriptThing inside the inlined ~CIsThingAlive but expands it at the three loop-1 temporaries; the row-local
  inline ~CScriptThing expands at both. This is the same nested-inline gap as chunk 808 0x00DE6C20 and chunk 810 0x00D62480.
- **0x00D0D910:** `__forceinline ~CTimer` expands every exit, but retail expands 2 of 3.
- **0x00D09010:** the post-loop `TalkedTo = false` goes into frame byte 0x1F, where retail uses 0x23.

## Callee names bound (all checked in reloc_bindings.tsv: none bound elsewhere)
- 0x00D0EBC0 `?OpenGate@CQ_BanditCampBossBattleScript@NScript@@QAEXMVCCharString@@@Z`: unlabelled; LABELS.tsv.
- 0x00D15140 `??0TeleportData@CQ_BountyHuntScript@NScript@@QAE@XZ`:
  - chunk 797's GANGDEPUTY2 opt-in already binds it;
  - the catalog name of this landed row is wrong (`CMainGameComponentInit::CMainGameComponentInit`): LABELS.tsv + NAME_FIXES.tsv.
- 0x00D15170 `??4TeleportData@CQ_BountyHuntScript@NScript@@QAEAAU012@ABU012@@Z`: the catalog has it without its scope; LABELS.tsv.
- BountyHunt threads, each already named in the catalog:
  - 0x00D15090 WatchForQuestFailure
  - 0x00D150F0 WatchForQuestSuccess
  - 0x00D151D0 WatchForEnableFisherCreekHutDoor
  - 0x00D17340 WatchForAllGruntsKilled
  - 0x00D17A80 WatchForBoasts
  
  0x00D15090 and 0x00D17340 appear in reloc_bindings.tsv only as call *sites* (column 3), not as targets.
- 0x00D13100 is still labelled `CVtable_012c9edc::vfunc_4` in labels_*; the correct name is
  `CSpawnedFunc<NScript::CQ_BountyHuntScript>::SuspendableProcess` (LABELS.tsv).
- 0x00CDBFB0 (the cast-map dtor) has several bound spellings in reloc_bindings.tsv, none from this chunk (CCutsceneCastMap::Destroy,
  ~CCutsceneCastMap, AutoTinyFirstMemberDestructor_00cdbfb0). Naming wave 1 already proved the label as the
  `std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>` dtor, but the catalog vland reads still shows the placeholder
  `CRetailTree_erase00cd344a` (vland RELOC_NAME warning on 00ea59e0), so it is in LABELS.tsv.

## Header debt
- The movie dtor: an opt-in in engine/CScriptGameResourceObjectMovieBase.h that expands the implicit dtor without the MovieBase vptr store
  (call ~CScriptGameResourceObjectBase body 0x006E7AB0, store the GROBase vptr 0x0126008C, call 0x0099A430). I found no plain C++
  spelling that gives this.
- The hdr/ opt-ins for FanMarker, Gate2Guard1, CampHostageGuard and BountyHunt MAIN serve parked rows only; they are inert for every
  other includer.
