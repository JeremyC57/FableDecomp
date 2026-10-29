# Hand chunk 571 (CEngineVSConstantLayoutBasic .. CQ_AmbushTradersScript::CAT_Sentry): 5 of 5 byte-exact

All five rows are RELOCATION_MATCH. None is MATCH and none is retail_asm. Each has a behaviour test that prints
`<addr>_TEST PASS` under both g++ (tall.sh) and VC7.1 (tall.py).

## What matched (5)

- **CEngineVSConstantLayoutBasic::CEngineVSConstantLayoutBasic() 0x00BDBB70** (909 B). This is the basic vertex-shader
  constant layout constructor:
  - It runs the base CVertexShaderConstantLayout ctor (0x0098D490) and stores its own vtable.
  - It builds a local `CArray<CVertexShaderConstant>` holding {0,1,2,0.5} and {256,256,256,256}.
  - It sets Reserved, ReservedPresets and the ten basic ranges (Ambient, FogColour, CameraPos, ...).
  - Shape: STLport 4.6 `push_back` / `_M_insert_overflow` / `~vector` expand inline, while `__uninitialized_copy`
    (0x00BDB6D0) and `vector::operator=` (0x00BDB9F0) keep same-TU noinline bodies. Without the operator= body the
    local vector was treated as aliased: 952v909, with extra zero stores and reloads after malloc.
- **NScript::CQ_HobbeCaveScript::CheckIfGotTreasure() 0x00D99A30** (911 B). This is a spawned thread:
  - Each time HobbeCaveMainChamber is loaded, it looks up HC_Treasure1 and HC_Treasure2.
  - While the region stays loaded, it counts GotTreasure each time the hero uses a chest, and has the BanditSacrifice
    bandit comment.
  - Shape: `while (!IsActiveThreadTerminating())` outer loop, with the chests in an inner block. `~CScriptThing` is
    `__forceinline`, with `#pragma inline_depth(0)` around the returns and block end where retail calls 0x004AA840 out
    of line.
  - CScriptThing needed all padding slots t001..t019 so that MsgIsUsedBy lands on slot 20 (+0x50).
- **NScript::CV_SickChildScript::PlayCutscene(CCharString name, CCharString arg) 0x00ECE460** (908 B):
  - Takes the hero under scripted control and sets `$ARG1` unless arg is "NULL" (inline CCharString::operator!= =
    `_stricmp` on GetAsCharArray).
  - Casts WITCH, MUM and HERO, then runs RunCutsceneMacro inside FixMovieSequenceCamera(true/false).
  - Shape: the landed CQ_HeroSoulsNostroScript::PlayCutscene model. The implicit
    CScriptGameResourceObjectScriptedThingBase::operator= was emitted out of line (777v908) until it was written
    `__forceinline` (base operator= + PImp copy).
- **NScript::CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze::Main() 0x00EE3B20** (952 B). This is Maze's
  guild-trigger entity script. It matched on the first probe, using the landed CTH_Maze::Main 0x00EE1E40 model:
  - CIsThingAlive predicate on Me, then SetThingPersistent.
  - After the post-save point it removes the "TravHero Maze-Scythe" rumour and Me. Otherwise it calls GossipSetup.
  - It sets PreMeleeMaze's information, then runs `while (!ExitThisLoop)`:
    - wait until the hero is under 2 units away, or a guild warning starts;
    - past the post-save point, exit;
    - during a warning, wait it out, clear the information, wait until the hero is over 40 units away, then set it again;
    - otherwise play CS_TRAVHERO_MAZEARGUMENT and exit.
  - Tail: remove the rumour, set ParentClass->MazeCutsceneDone, clear the information and remove Me.
  - VC7.1 itself chose to expand `~CScriptThing` inline at one return (inside the 2-unit wait), so no pragma was needed.
- **NScript::CQ_AmbushTradersScript::CAT_Sentry::Init() 0x00F08E90** (961 B):
  - Calls SetWanderCentrePoint(Me, Me.GetPos()), SetWanderMinDistance 0, SetWanderMaxDistance(pGlobalScriptDef
    AmbushTradersSentryWanderDistance) and SetScriptingStateGroup 4.
  - Makes the sentry and the hero enemies of each other, then the sentry and each live bandit in AllBandits (stopping if
    the thread is ending).
  - Spawns ParentClass.WatchForHeroAttackOnSentry and ParentClass.WatchForEnemyDeath on Me, then sets
    SentrySeenSpy = false.
  - Shape: the landed CAT_Phase1Guard::Init spawn block, written with the real template
    `CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>`. The template ctor must be `__forceinline` (plain inline left a
    call). `~CScriptThing` is declared out of line (0x004AA840), because retail calls it for the by-value ctor parameter.
  - By-value CScriptThing arguments use the landed `InlineCopy` tag, so they expand inline; Par1 uses the out-of-line
    copy 0x004ABE90.
  - C3DVector needs the memberwise copy ctor to get retail's load/store interleave.
  - CSpawnedFuncBase is 0x34 bytes: the vptr plus 0x10 bytes of CScriptProcessBase.

## Names and evidence (all PDB spellings, checked with objdump; every call target paired with retail)

- 0x00BDBB70 `??0CEngineVSConstantLayoutBasic@@QAE@XZ`:
  - RTTI vtable 0x012A7D40, which this ctor stores, is CEngineVSConstantLayoutBasic.
  - rtti_bases makes Lights and RepeatedMesh derive from Basic, and their ctors 0x00BDB400 / 0x00BDB080 call 0x00BDBB70.
  - Ego_r.pdb 0x00A8EAD0 and FableWin.pdb 0x02ED9BD0 both place it right before PeekName, as in retail.
- 0x00D99A30 `?CheckIfGotTreasure@CQ_HobbeCaveScript@NScript@@QAEXXZ`:
  - Ego_r.pdb 0x00DB72C0.
  - The spawner at 0x00D9806F stores 0x00D99A30 in a CSpawnedFunc (vtable 0x012D567C) named "CheckIfGotTreasure"
    (0x012D6488).
- 0x00ECE460 `?PlayCutscene@CV_SickChildScript@NScript@@QAEXVCCharString@@0@Z`:
  - Both PDB name sets; the function ends in `ret 8`.
  - The cast reads seh_Mother +0x48 and seh_Witch +0x58 (Ego_r +0x5C / +0x6C, less 0x14).
  - Its four callers sit inside CV_SickChildScript.
- 0x00EE3B20 `?Main@CTH_Trigger_Guild_Maze@CV_TravellingHeroesScript@NScript@@EAEXXZ`:
  - Slot 1 of vtable 0x012F42CC, which the landed CTH_Trigger_Guild_Maze alloc 0x00EE03E0 stores.
  - In both PDB name sets.
  - Ego_r layout: ParentClass +0x14, PMasterData +0x18, ExitThisLoop +0x1c. Init 0x00EE0490 clears ExitThisLoop.
- 0x00F08E90 `?Init@CAT_Sentry@CQ_AmbushTradersScript@NScript@@EAEXXZ`:
  - Slot 2 of vtable 0x012F7E30, which the landed CAT_Sentry::Alloc 0x00F04510 stores (slot 1 is the landed
    CAT_Sentry::Main 0x00F045E0).
  - Ego_r.pdb 0x00F89B40 and FableWin.pdb 0x0363A6D0.
  - Ego_r CAT_Sentry +0x1c is `SentrySeenSpy`, which the function clears at its end.
- Member names:
  - CQ_SunnyvaleMasterData::PostSavePosition +4 and GuildWarningOccuring +0xC0 (Ego_r; no shift).
  - CScriptDef retail +0x9F0 = Ego_r 0xA90 AmbushTradersSentryWanderDistance. The def's retail offsets are 0xA0 below
    the PDB's; the landed AmbushTradersBanditHireCost 0x9DC/0xA7C is the same shift.
  - The by-value CScriptThing slots are 763 SetWanderCentrePoint, 766 SetWanderMinDistance, 769 SetWanderMaxDistance and
    784 SetScriptingStateGroup (cgsi_slots_aligned.tsv, whose argument types match the pushes).

## Parked for Fable 5.1 (0)

## Skipped

None.

## Label corrections

| Address | Manifest label | Correct name |
|---|---|---|
| 0x00BDBB70 | CVertexShaderConstantLayout_Initialize | CEngineVSConstantLayoutBasic::CEngineVSConstantLayoutBasic (the base ctor is 0x0098D490) |
| 0x00D99A30 | Script_Bandit_Sacrifice | NScript::CQ_HobbeCaveScript::CheckIfGotTreasure |
| 0x00ECE460 | Script_InitializeWitchCharacter | NScript::CV_SickChildScript::PlayCutscene |
| 0x00EE3B20 | sub_ | NScript::CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze::Main |
| 0x00F08E90 | sub_ | NScript::CQ_AmbushTradersScript::CAT_Sentry::Init |
| 0x00E241B0 (callee) | CScriptThingFunctor_00e241b0::~CScriptThingFunctor_00e241b0 (vland RELOC_NAME warning) | NScript::CIsThingAlive::~CIsThingAlive (`??1CIsThingAlive@NScript@@UAE@XZ`, PDB; it destroys the predicate whose vtable 0x012C32F0 is ??_7CIsThingAlive@NScript@@6B@) |

Landed spellings proven wrong are in NAME_FIXES.tsv (29 rows). The main ones:

| Address | Correct symbol | Landed spelling it replaces |
|---|---|---|
| 0x00BDBB70 | the ctor above | `??0CVertexShaderConstantLayout@N_00bdbb70@@QAE@XZ` (callers 0x00BDB400 / 0x00BDB080) |
| 0x00EE07E0 | `?GossipSetup@CV_TravellingHeroesScript@NScript@@...` | the same name without `NScript` |
| 0x009AC700 | `std::map<CCharString,CCharString>::operator[]` | CCutsceneArgMap / CStringMap / CTextArgMap |
| 0x012F7EBC | `??_7?$CSpawnedFunc1@VCQ_AmbushTradersScript@NScript@@VCScriptThing@@@@6B@` | CSpawnedFuncEnemyDeath / CSpawnedFuncAmbush |

Further rows cover CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 and SetPredicate 0x00F35B10, and
NScript::CIsThingAlive (0x00E241B0, 0x012C32F0).

## Header debt

- engine/CVertexShaderConstantLayout.h and engine/CEngineVSConstantLayoutBasic.h flatten every CShaderConstantRange
  to `unsigned char[0xc]`. 0x00BDBB70 models the layout locally at the landed base ctor's offsets.
- engine/CSpawnedFuncBase.h is flattened and has no ctor or vtable, so 0x00F08E90 models it locally.
- No engine headers exist for CTH_Trigger_Guild_Maze, CAT_Sentry, CQ_HobbeCaveScript or CV_SickChildScript.
- `pGlobalScriptDef` keeps the landed `extern CScriptDef*` spelling (`?pGlobalScriptDef@@3PAVCScriptDef@@A`). The PDB
  has a dynamic initializer and an atexit destructor for it, and a `CDefPointer<CScriptDef const>` layout exists, so the
  real type is probably that def pointer. This is not proven, so the name is unchanged.
