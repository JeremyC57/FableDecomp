# DOSSIER 0x00D30030 (8589 bytes) -- scout, 2026-11-12

No prior scout material existed ($W/prior/ empty; /opt/fable_lane/w22/agent4016 has only its brief, no probes/cand). Everything
below is from this pass. Scout helpers: $W/scout/ann.txt (listing annotated with callee names, landed row files, string
literals, floats), $W/scout/trace.txt (call/string skeleton), $W/scout/slots.txt (every vtable offset -> CGameScriptInterfaceBase slot).

## 1. Identity -- the manifest label is WRONG
- Real name: `NScript::CQ_EndGameBossBattleScript::JackOfBladesBossBattle()`, leaf
  `?JackOfBladesBossBattle@CQ_EndGameBossBattleScript@NScript@@QAEXXZ` (public, non-virtual, void, no args, __thiscall).
- Evidence (>=2 sources):
  a. Ego_r.pdb names: `ghidra_out/egor_pdb_names.tsv` 00d3d9c0 and FableWin.pdb `ghidra_out/_fablewin_api.tsv` 036ee230 / 0183abc6
     both hold this leaf. FableWin source order: Main 036ede00, **JackOfBladesBossBattle 036ee230 (0x58C0 bytes, the class's
     biggest)**, JackOfBladesBossFightCutscene 036f3af0, PhaseWatcher 036f43b0. Retail: this row (8589 bytes, the biggest) sits right
     before the landed JackOfBladesBossFightCutscene 0x00D321C0 -- same order.
  b. The landed neighbour rebuild/src/compiled/00/d3/NScriptCQEndGameBossBattleScript_JackOfBladesBossFightCutscene_00d321c0.cpp
     line 7-12 already states: "The only caller is the direct call at 0x00D31358 inside CQ_EndGameBossBattleScript::JackOfBladesBossBattle
     0x00D30030 (bound by the landed Main 0x00D2F7F0)".
  c. Body: it calls JackOfBladesBossFightCutscene (0x00D31358) and spawns `new CSpawnedFunc<CQ_EndGameBossBattleScript>
     ("JackOfBladesPhaseWatcher", 0, this, &JackOfBladesPhaseWatcher)` (literal 0x012CC89C, member fn 0x00D32630 stored at +0x34).
  d. Only xref: `python3 $S/w23/xref.py 00d30030` -> one call from 0x00D2FA20 (inside CQ_EndGameBossBattleScript::Main 0x00D2F7F0).
- Manifest "_global::CScriptGameResourceObject_LoadGuildComplex" is a made-up catalog label (no PDB hit at 00d30030 in any tsv).
- 0x00D32630 (no manifest row) = JackOfBladesPhaseWatcher (only member fn stored into the CSpawnedFunc built here; FableWin order).

## 2. Retail shape (xdis 00d30030 0x219d; dis_all.txt)
- Prologue `sub esp,0C8h; push ebx; push ebp; push esi; mov esi,ecx; mov dword [esp+0Ch],0; ... push edi` (edi pushed late,
  interleaved with the first Game load). **No frame pointer, no SEH/fs:0 frame, no /GS cookie.** `/Oy` on, no EH (destructors run
  explicitly at every exit) -- same as all landed quest-script rows (FocalSites Main 0x00D2AA40: "thiscall, plain ret").
- Plain `ret` (thiscall, no stack args). Epilogue `pop edi; pop esi; pop ebp; pop ebx; add esp,0C8h; ret` -- 5 copies:
  0x00D30533, 0x00D30C87, 0x00D30E06, 0x00D30F36 (inline-expanded early exits of the first limbo loops) and the main one 0x00D321B2.
- Speed-optimised (/O2): `lea esp,[esp]` / `lea ecx,[ecx]` alignment pads before loops, rotated loops.
- Last byte 0x00D321BC `ret`; 0x00D321BD..BF int3 pad; Cutscene starts 0x00D321C0. Length 8589 confirmed.

## 3. Callees (direct)
| addr | landed name / file | PDB | note |
|---|---|---|---|
| 0x0099EBF0 | CCharString(const char*, long) CCharString_Constructor_0099ebf0.cpp | ok | 54 sites |
| 0x0099EAE0 | ~CCharString | ok | 57 sites |
| 0x00CB7940 | CScriptBase::IsActiveThreadTerminating | ok | 50 sites |
| 0x004AA840 | ~CScriptThing (out of line) CScriptThing_CScriptThing_004aa840.cpp | ??1CScriptThing@@UAE@XZ | 19 sites |
| 0x008AC970 | std::vector<CScriptThing>::~vector stdvectorCScriptThing_vector_008ac970.cpp | ??1?$vector@VCScriptThing@@... | 13 sites (other exits expand it inline: _Destroy loop of virtual dtor `push 0; call [vtbl]` + `free`) |
| 0x008AC9D0 | std::vector<CScriptThing>::erase(iterator,iterator) stdvector_erase_008ac9d0.cpp | ok | 6 sites = `v.clear()` |
| 0x00BFEA14 | free (MSVCR71) | | 47 sites, inline ~vector |
| 0x00BFE9BC | operator delete | | inline ~CCountedPointer in inline ~CScriptThing (early exits) |
| 0x0099A2E0 | ~CBaseClass | ??1CBaseClass@@UAE@XZ | in the inline ~CScriptThing of guildComplex at early exits |
| 0x00704580 | CCountedPointer<CScriptThing>::operator= (file CCountedPointerCDiskFileWin32_operator_00704580.cpp; leaf `@CCountedPtr_assign@12` -- folded COMDAT) | | the implicit CScriptThing::operator= expanded: Jack = ..., guildComplex = ... |
| 0x00CD23B9 | CScriptGameResourceObjectScriptedThingBase::IsValid | ?IsValid@...@QBE_NXZ | seh_Jack |
| 0x00CD2770 | ...ScriptedThingBase::Release (file ..._Release_00cd2770.cpp; functions.tsv path still says CMemoryDataOutputStream_Clear) | ?Release@...@QAEXXZ | x2 |
| 0x006E7A60 | CScriptGameResourceObjectBase() | ??0CScriptGameResourceObjectBase@@QAE@XZ | inline MovieBase ctor (then vtbl 0x01260EF4, zero +8/+C) |
| 0x006E7AB0 | ~CCountedPointer<CScriptGameResourceObjectMovieBase> | ok | inline MovieBase dtor (then vtbl 0x0126008C, ~CBaseObject) |
| 0x0099A430 | ~CBaseObject | ??1CBaseObject@@MAE@XZ | |
| 0x00D321C0 | CQ_EndGameBossBattleScript::JackOfBladesBossFightCutscene (landed) | ok | |
| 0x00BFEA1A | operator new(0x3C) | | CSpawnedFunc |
| 0x00CDD450 | CSpawnedFuncBase(const CCharString&, unsigned long) | ??0CSpawnedFuncBase | vtbl 0x012CC620 = CSpawnedFunc<CQ_EndGameBossBattleScript> |
| 0x00CB7E50 | CScriptBase::AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&) | ok | |
| 0x00CBED82 | KillAllThingsInVector (__fastcall) | ?KillAllThingsInVector@@YIXAAVCGameScriptInterfaceBase@@ABV?$vector@...@std@@_N@Z | decl spelled in landed rows: `void __fastcall KillAllThingsInVector(CGameScriptInterfaceBase& game, const std::vector<CScriptThing>& things, bool fade);` |
| 0x00CBE960 | AddLogbookStoryEntryString (__fastcall) | ?AddLogbookStoryEntryString@@YIXABVCCharString@@@Z | decl `void __fastcall AddLogbookStoryEntryString(const CCharString& text);` (HeroSoulsArena Main 0x00D7C890) |
No spelling conflicts found between landed names and PDB for these.

Interface calls: all through `Game` (+0x40, CGameScriptInterfaceBase*), see $W/scout/slots.txt. Slots used: 7 NewScriptFrame,
8 StartScriptingEntity, 11 IsLevelLoaded, 12 IsRegionLoaded, 70 GetHero, 72 GetThingWithScriptName, 80 GetAllThingsWithDefName,
81 GetNearestWithDefName, 108 RemoveThing, 128 GiveHeroExperience, 156 GiveHeroMorality, 157 GetHeroMorality, 264 GetHealth,
276 ActivateQuest, 280 DeactivateQuest, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 321 DisplayQuestInfo,
325 AddQuestInfoBarHealth, 369 PlayAVIMovie, 370 StartMovieSequence, 372 FadeScreenOutUntilNextCallToFadeScreenIn,
373 FadeScreenOut, 374 FadeScreenIn, 379 PauseAllNonScriptedEntities, 381 SetAllowScreenFadingOnNextRegionChange,
382 SetAllowScreenFadingIfAlreadyFaded, 425 OpenDoor, 440 SetVillageLimbo, 443 EnableGuards, 444 EnableVillagerDefTypes,
447 SetThingAsUsable, 453 EntitySetInLimbo, 472 EntityTeleportToThing, 477 SetThingPersistent, 511 EntitySetAsDrawable,
522 EntityGetBossPhase, 532 EntityEnableGravity, 563 SetTeleporterAsActive, 565 SetTeleportingAsActive,
569 SetRegionTextDisplayAsActive, 655 GetActiveQuestName, 656 TransitionToTheme, 657 ResetToDefaultTheme, 694 EnableSounds,
696 OverrideMusic, 697 StopOverrideMusic, 698 CacheMusicSet. All declared in engine/CGameScriptInterfaceBase.h.
Other indirect calls: `call [vtbl+0]` with `push 0` = CScriptThing virtual dtor inside inline vector _Destroy; `call [eax+4]`
(ecx=[eax+8]) = CCountBlock DeleteFunc in inline ~CCountedPointer.

## 4. Members / globals
- this = CQ_EndGameBossBattleScript (engine/CQ_EndGameBossBattleScript.h, needs FABLE_RETAIL_NSCRIPT; base CLevelScriptBase<CQ_SunnyvaleMasterData>
  needs FABLE_RETAIL_CLEVELSCRIPTBASE): Game +0x40, MasterScriptData +0x44 (CLevelScriptBase.h l.46-47),
  IntroFinished +0x48 (write 1 @0x00D3140C), FinalDecision +0x49 (write 1 @0x00D31558), FinalCutscenePlayed +0x4D (read, wait loop
  @0x00D31565), seh_Jack +0x50 (CScriptGameResourceObjectScriptedThingBase), Jack +0x60 (CScriptThing; PImp at +0x64).
  Ego_r layout = retail + 0x14 (struct_layouts_egor.tsv rows 48481-48488 IntroFinished 92 .. Jack 116).
- MasterScriptData (CQ_SunnyvaleMasterData, engine/CQ_SunnyvaleMasterData.h l.130-131): JackBossBattleResult long +0x88 (compared to 1
  and 2 @0x00D315C7 / 0x00D3171F), JackBossBattleHeroGoodAtEnd bool +0x8C (written 0 or 1).
- Global 0x0143E90C = `pGlobalScriptDef` (engine/fable_script_def_global.h: `extern CDefPointer<const CScriptDef> pGlobalScriptDef;`,
  used as `pGlobalScriptDef->X` in landed rows, e.g. HangingTreeEvil Init 0x00D68FF0). engine/CScriptDef.h: JackFightDefeatJackXP
  long +0x0D0C (l.755), EGJ_MoralityLoss float +0x0EA4 (l.857, evil branch), EGJ_MoralityGain float +0x0EA8 (l.858, good branch).
- Enums: EMusicSetType (engine/EMusicSetType.h) MUSIC_SET_GUILD_DAY=5, MUSIC_SET_BOSS=23, MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_INTRO=54,
  MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_OUTRO=55. EScriptAIPriority: no shared header; the Cutscene row defines it locally
  (SCRIPT_AI_PRIORITY_HIGHEST=4) -- copy it.
- CRGBColour (engine/CRGBColour.h): 4-arg inline ctor `R=r;G=g;B=b;A=a` (retail byte stores +2,+1,+0,+3 order is the scheduler).

## 5. Visibility / opt-ins
- STLport vector<CScriptThing>: stlport/fable_containers.h `class vector` (l.366); `clear()` inline -> `erase(begin(),end())`;
  retail CALLS erase 0x008AC9D0 => `#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE` (declaration only, l.519).
  ~vector: retail mixes inline expansion (early exits in the first limbo pass) and calls to the COMDAT 0x008AC970 (all late exits,
  shared tails 0x00D32173 / 0x00D3215E / 0x00D3216A / 0x00D3213A). No opt-in macro exists for an out-of-line ~vector; the landed
  HeroSoulsArena Main 0x00D7C890 had the same mix ("~vector 0x008AC970 (expanded on the main path)") with the plain inline ~vector --
  i.e. VC7.1's inline budget decides; start with the plain header.
- CScriptThing assignment: retail expands the implicit operator= (one call to CCountedPointer operator= 0x00704580 on PImp)
  => `#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT` (engine/CScriptThing.h l.108-111; precedent row 0x00DC1770).
- ~CScriptThing: out-of-line call 0x004AA840 at all normal sites, but EXPANDED inline (vtbl 0x01238C8C store, CCountedPointer release,
  operator delete, ~CBaseClass) for guildComplex at the four early-exit epilogues (0x00D304E9..0x00D30524 etc.). The landed Cutscene row
  uses `inline CScriptThing::~CScriptThing() { }` after the includes (row-local inline dtor) -- copy that.
- CScriptGameResourceObjectMovieBase: same expansion as the Cutscene row (base ctor called, vtable + zero inline; dtor: ~CCountedPointer
  call + vtable + ~CBaseObject). Cutscene used no movie opt-in; Sister Main 0x00D328C0 used ..._MOVIEBASE_OUTOFLINE_* + FORCEINLI* (check).
- CSpawnedFunc ctor: engine/CSpawnedFunc.h (FABLE_RETAIL_SUSPENDABLE_PROCESS) __forceinline ctor; the name temporary has a construction
  flag (body slot 0x10, zeroed in the prologue, `test byte [esp+10h],1` @0x00D313CC) exactly as in the landed DoMission 0x00D66CA0.
- CScriptBase::AddSpawnedFunction (engine/CScriptBase.h l.38).

## 6. Siblings to copy idioms from
- JackOfBladesBossFightCutscene 0x00D321C0 (same class, next function, same TU): defines FABLE_RETAIL_CCHARSTRING, _NSCRIPT,
  _SUSPENDABLE_PROCESS, _BASE_OBJECT, _CLEVELSCRIPTBASE, _STLP_MAP; includes fable_string.h, CCountedPointer, CScriptThing,
  CGameScriptInterfaceBase, CQ_SunnyvaleMasterData, CEntityScriptBase, CScriptGameResourceObjectMovieBase, CQ_EndGameBossBattleScript,
  CScriptGameResourceObjectScriptedThingBase; local EScriptAIPriority enum; `inline CScriptThing::~CScriptThing() { }`;
  idioms: `if (seh.IsValid()) seh.Release(); while (!Game->StartScriptingEntity(x, seh, SCRIPT_AI_PRIORITY_HIGHEST)) {NSF; if ITT return;}
  if ITT return;`, `CScriptGameResourceObjectMovieBase m; Game->StartMovieSequence("", m); CGameScriptInterfaceBase* game = Game;
  game->PauseAllNonScriptedEntities(true); ... game->PauseAllNonScriptedEntities(false);` -- this row uses the SAME cached-`game` form.
- CEndGameSister::Main 0x00D328C0 (same class): CRGBColour include, movie opt-ins.
- CQ_EndGameFocalSitesScript::Main 0x00D2AA40 (previous quest, same Guild-complex code: GetNearestWithDefName(GetHero(),
  "VILLAGE_GUILD_COMPLEX_INSIDE"), EnableVillagerDefTypes, EnableGuards; WAIT form `while (!IsRegionLoaded) { NSF; if ITT return; } if ITT return;`).
- CQ_HeroSoulsArenaScript::Main 0x00D7C890: local std::vector<CScriptThing>, AddLogbookStoryEntryString decl, AddSpawnedFunction(new CSpawnedFunc..., CCharString("")).
- CQ_GuildTrainingWoodsMeleeScript::DoMission 0x00D66CA0: spawned-function flag word.
- CQ_HangingTreeEvilScript::WatchForHeroNearExit 0x00D6CA90: `for (i < n) { if ITT return; ... }` over a vector (it models vector locally;
  prefer stlport/fable_containers.h).
None of these carry a `// Flags:` line (default lane flags /O2 /Oy, no /EH).

## 7. Things the author must not miss
- 6 vectors + counts declared up front and REUSED: the second pass calls clear() on each then GetAllThingsWithDefName into the same
  vectors and re-assigns the same count variables.
- Every limbo loop is `for (long i = 0; i < n; ++i) { if (ITT) return; Game->EntitySetInLimbo(v[i], X, true); } if (ITT) return;`
  (first pass X=true, second pass X=false). The first pass loop over sconces is NOT preceded by an ITT check.
  Loop ORDER differs from the GetAll order (verified from each loop's element base): pass 1 = sconces, chandeliers, chandeliersLong,
  candleRounds, candleTrios, bookcases; pass 2 = sconces, chandeliers, bookcases, chandeliersLong, candleRounds, candleTrios.
- Both IsLevelLoaded/IsRegionLoaded waits: bool result held in bl via neg/sbb/inc across the ~CCharString => plain
  `while (!Game->IsLevelLoaded("..."))` with the literal converted in the condition (same as FocalSites/Cutscene).
- `GetHeroMorality() < 0.5f` (fcomp 0x0122F59C, test ah,5 / jp) chooses Sisterkilled (HeroGoodAtEnd=false) first; `GetHealth(Jack) > 10.0f`
  (0x01237F0C, test ah,41h / jnp).
- In each epilogue branch an ITT inside the morality if/else does `{ game->PauseAllNonScriptedEntities(false); return; }` (movie dies).
