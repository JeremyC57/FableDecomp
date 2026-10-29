# Hand chunk 579 (CArenaCreatureDef::Transfer .. NScript CPersonalScript_EmptyScript::CCrate1::Main): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (gall.sh) and pass their tests under g++ (tall.sh) and the lane's VC7.1 (tall.py).
The read-only vland run reports 10 RELOCATION_MATCH + PASS with no RELOC_IDENTITY and no DIFFER. Its five warnings are all
RELOC_NAME, which means callee catalog labels: 0x00CB8930, 0x00E24760, 0x00F26C30, 0x00F273B0 and 0x00F339E0 (see Label corrections and
NAME_FIXES.tsv). No row uses retail_asm, flags or codegen pragmas. The only pragma is 0x00DDB570's `pack(1)` around the
17-byte CCharStringData layout, as in the landed CSecretBook model. Call targets and string literals were checked against
retail with tools/rel.py, which maps each leaf's COFF relocations onto the retail bytes. Every target is the declared callee's
retail address, and every string sits at the retail literal address.

## What matched (10)
Every name below is **proven**: it rests on a vtable slot or spawn store, plus the Ego_r.pdb / FableWin.pdb name set, the
landed neighbours, or both. No row carries a descriptive name. Every catalog label in this chunk was wrong.

- **0x00F25E70 CArenaCreatureDef::Transfer(CPersistContext&)**: `?Transfer@CArenaCreatureDef@@UAEXAAVCPersistContext@@@Z`.
  - Evidence: slot 18 of the def vtable 0x012FB358 (vt[-1] == 0); GetSizeofClass 0x00F25E10 returns 0x38; Ego_r has the
    CArenaCreatureDef members CreatureType, NumCreatures, HUDType and DeathScore.
  - Body: four Transfers (the two-argument CCharString inline plus the long/default form), on the landed FishingCompetition
    OnPersist model.
  - The object also emits the inline two-argument Transfer COMDAT; the Leaf pins the graded function. First grade.
- **0x00F1A8F0 NScript::CQ_ArenaScript::CArenaEnemy::OnPredicateFail()** (EAE).
  - Evidence: slot 5 of the entity vtable 0x012F9C50, which the landed Alloc 0x00F1A810 stores. Ego_r CArenaEnemy has exactly
    OnPredicateFail / Alloc / Main.
  - Body:
    - `--TotalCreatures[MyCreatureType]`;
    - a hero kill (`Me.MsgIsKilledBy("SCRIPT_NAME_HERO")`, CScriptThing slot 18) adds
      `Rounds[ArenaRound].Waves[ArenaRoundWave].Creatures[MyCreatureType].DeathScore` to NewCrowdPoints, using STLport
      `operator[] = *(begin() + n)` over the CArenaRoundDef (0x38) / CArenaWaveDef (0x3c) / CArenaCreatureDef def headers;
    - a big creature calls `RemoveQuestInfoElement(HealthBarIndex)` (interface slot 338).
  - Second grade: regraded after the coordinator note, with `engine/CBaseClass.h` replacing the local CBaseClass.
- **0x00F26B90 NScript::CQ_HeroSoulsArenaScript::Init()** (UAE).
  - Evidence: slot 3 of the quest vtable 0x012D3994. Ego_r places Init at 0x00F9B570, away from the class's other members, just
    as retail puts it beside the arena-def code.
  - Body:
    - `Rounds = pGlobalScriptDef->HeroSoulsRounds` (vector operator= 0x00F26C30; CScriptDef member at retail +0x1050);
    - CreatureCount, seven bools and ArenaLevelReached are cleared;
    - `GuardChatTimer = 0` goes through the inline CTimer::operator= (g_pGameScriptInterface 0x0143E8F8, slot 89 SetTimer);
    - `Game->AddQuestRegion("Q_HeroSoulsArena", "Arena")` (slot 271).
  - First grade.
- **0x00F0EDA0 NScript::CQ_AmbushTradersScript::WatchForSpyDeath(CScriptThing)** (QAE, `ret 0Ch`).
  - Evidence: the CSpawnedFunc1 member-pointer store `mov [esi+34h], 0F0EDA0h` at 0x00F0D7B9, after the thread name
    "ParentClass." + "WatchForSpyDeath"; the Ego_r + FableWin symbol; SpyDead +0x58; CQ_SunnyvaleMasterData::AmbushTradersSpyDead
    +0xd4.
  - Form: `for (;;) { if (term) return; if (spy.MsgIsKilledBy("")) { if (term) return; SpyDead = MasterScriptData->... = true; } frame; }`.
    The `while (!term)` form swapped the two inline-dtor exit copies (230v230).
  - Second grade, after one probe.
- **0x00DDB570 NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Init()** (EAE).
  - Evidence: slot 2 of the entity vtable 0x012DC97C (landed Alloc 0x00DDB4A0); Ego_r member order.
  - Body: DistanceToPlayAnim = 12, then an else-if chain over fresh `Me.GetDataString() == ...` temporaries (STANDARD_POINT 12,
    CS_POINT 16, ST_COME_ON 10, AGGRESSIVE_POINTING 12), then `EntitySetAsKillable(Me, false, true)` (slot 517). This is the chunk
    577 CSecretBook::Init model.
  - First grade.
- **0x00F27200 std::vector<CHeroSoulsCreatureDef>::operator=**.
  - Evidence: callees are the landed element operator= 0x00F27190 and copy ctor 0x00F27060, plus _M_allocate_and_copy 0x00F273F0
    and _M_clear 0x00F273B0, all with a 0x34 stride. Ego_r name set; six callers.
  - Built on the CConversation operator= 0x00E54AA0 model. The surplus destroy is inlined here as a virtual-dtor loop
    (`__forceinline` _Destroy(first, last) / __destroy / __destroy_aux).
  - First grade.
- **0x00E25AB0 NScript::CQS_GuardianSisterInfoScript::Main()** (UAE).
  - Evidence: slot 2 of the script vtable 0x012E1DAC, which the landed ctor 0x00E259D0 stores.
  - Body:
    - `AddEntityScriptBinding(new CEntityScriptBinding<CMazeAtTavern>("MazeAtTavern", this, &CMazeAtTavern::Alloc, 0))`;
    - both PostAddScriptedEntities calls;
    - SetQuestCardObjective("TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01", "BowerstoneSlums");
    - WAIT(IsRegionLoaded("BowerstoneSlums"));
    - SetTimeOfDay(10.0f) (slot 646).
  - Built on the landed CQS_GuardianTrophyDealerInfoScript::Main 0x00E27920 model.
  - Since this chunk started, CMazeAtTavern::Alloc 0x00E25C70 has been landed under the same Ego_r spelling the source declares.
  - First grade.
- **0x00F33810 std::vector<CConversation>::_M_fill_insert(pointer, size_type, const CConversation&)** (QAE, `ret 0Ch`).
  - Evidence:
    - callees are the landed CConversation copy ctor 0x00E54CA0, operator= 0x00E54C50 and ~CConversation 0x00E54D80 (0x5C
      stride), plus the landed vector<CConversation>::_M_insert_overflow 0x00F33A10, which names this row as its only caller;
    - this row's only caller is vector<CConversation>::resize 0x00F33730 (call at 0x00F3378D, found by a whole-.text scan);
    - the STLport text is the same as the landed vector<CWideString>::_M_fill_insert 0x004126E0.
  - Ego_r has no STLport spelling (it is Dinkumware), so the symbol follows the landed STLport rows.
  - Shape: __uninitialized_copy, __copy_backward_ptrs / __copy_backward and fill are `__forceinline` (retail inlines all three);
    uninitialized_fill_n 0x00F339E0 is an out-of-line `__fastcall` declaration; __false_type has no user ctor, so the overflow
    tag is zero-stored.
  - First probe.
- **0x00EF54A0 NScript::CPersonalScript_EmptyScript::CCrate1::Main()** and **0x00EF5670 ...::CCrate2::Main()** (EAE).
  - Evidence: slot 1 of the entity vtables 0x012F5F90 / 0x012F5FAC (landed Allocs 0x00EF1050 / 0x00EF1130); Ego_r has Alloc + Main.
  - Body:
    - `marker = GetThingWithScriptName("CrateDeathMarker")`;
    - `SetPredicate(CIsThingAlive(Me) && CIsThingAlive(marker))` (CPredicate_And ctor 0x00E24760, CIsThingAlive vtable 0x012C32F0);
    - frame;
    - WAIT(IsDistanceBetweenThingsUnder(Me, marker, 2.0f));
    - CreateExplosion("THING_EXPLOSION_BLASTBOX", Me.GetPos(), 0, "");
    - then CCrate1 calls `RemoveThing(Me, false, true)` and CCrate2 calls `FadeOutAndKillEntity(Me, 0.5f, true, true)`
      (0x00CBE716, `?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z`).
  - A plain in-class `virtual ~CScriptThing() {}` gives retail's mix: the first exit inlines the dtor and the later exits call
    0x004AA840.
  - Each matched on its first probe.

## Parked for Fable 5.1
None.

## Skipped
None.

## Label corrections
| address | catalog / landed label | really |
|---|---|---|
| 0x00DDB570 | Script_Guard_Pointing_Gesture | NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Init |
| 0x00EF54A0 | Script_CrateBlastBoxTrap | NScript::CPersonalScript_EmptyScript::CCrate1::Main |
| 0x00F27200 | std::vector_assign_pair_CCharString_CSymbolInfo | std::vector<CHeroSoulsCreatureDef>::operator= |
| 0x00F33810 | vector_insert_impl_CConversation (bound `?Insert@CRetailVector_elem92@@...`) | std::vector<CConversation>::_M_fill_insert |
| 0x00F337A0 | bound `?Erase@CRetailVector_elem92@@...` by 0x00F33730 | std::vector<CConversation>::erase (the landed Leaf) |
| 0x00F33730 | CRetailVector_elem92::resize (landed row) | std::vector<CConversation>::resize (0x00F33B60 already binds that) |
| 0x00F339E0 | _global::UninitFillN_CElement92_00e54ca0_00f339e0 ("Dinkumware _Uninit_fill_n") | STLport std::uninitialized_fill_n<CConversation*, unsigned int, CConversation> |
| 0x00F273B0 | CRetailVector_poly0x34::~CRetailVector_poly0x34 | vector<CHeroSoulsCreatureDef>::_M_clear |
| 0x00F273F0 | landed public `QAE` _M_allocate_and_copy | protected `IAE` (STLport 4.6) |
| 0x00F27060 | bound `??0CElement52_00f27060@@...` by 0x00F279E0 | CHeroSoulsCreatureDef copy ctor |
| 0x00F26C30 | _global::std_vector_Conversation_Assign3 | std::vector<CHeroSoulsRoundDef>::operator= (the landed Leaf is right) |
| 0x00E25C70 | CTriangleList | CQS_GuardianSisterInfoScript::CMazeAtTavern::Alloc (since landed so) |
| 0x00E24760 | CConsoleCommandParameters::CConsoleCommandParameters | CPredicate_And<CIsThingAlive, CIsThingAlive>::CPredicate_And |
| 0x00CB8930 | CTCVillage::OnInitialActivate | CScriptBase::PostAddScriptedEntities |
| 0x00F25E40 | ~CAIStateGroup_HoverHornetQueen | ~CArenaCreatureDef |
| 0x00E54D80 | CTCInventoryMap::ResetPrimitiveHandles | ~CConversation |
| 0x00CBE716 | bound `_NM1` by 0x00E0F4E0 | `?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z` (Ego_r + FableWin) |
| 0x00CB7940 | six invented spellings (CScriptBase_Retail, per-script copies, `@Q_Sunny_cb7940@4`) | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` |

## Header debt
- **CScriptThing.h and CGameScriptInterface.h are flattened.** The vptr is a `void* __vftable` data member and the classes
  have no virtuals, so these headers cannot express a virtual dtor or a slot call.
  - Rows 0x00DDB570, 0x00EF54A0, 0x00EF5670, 0x00F0EDA0 and 0x00F1A8F0 therefore model CScriptThing locally, on top of
    `engine/CBaseClass.h`: 12 bytes, vtable 0x01238C8C, slot-padded virtuals with MsgIsKilledBy at slot 18, and an inline or
    `__forceinline` virtual dtor.
  - Rows 0x00DDB570, 0x00E25AB0, 0x00EF54A0, 0x00EF5670, 0x00F0EDA0, 0x00F1A8F0 and 0x00F26B90 model CGameScriptInterfaceBase
    locally (vtable 0x01260F0C, generated by tools/iface.py with padded slots).
- **No headers exist** for CScriptBase (retail 0x40 bytes, polymorphic), CEntityScriptBase, CLevelScriptBase<T>, CTimer,
  CPredicateBase / CIsThingAlive / CPredicate_And<P1,P2>, or CEntityScriptBinding<T> / CEntityScriptBindingBase. All of them
  are local, slot-commented stand-ins.
- **Def classes**: CArenaCreatureDef, CArenaWaveDef, CArenaRoundDef, CHeroSoulsRoundDef, CHeroSoulsCreatureDef and
  CConversation have no engine headers. Each is declared locally on top of the shared `FABLE_RETAIL_DEF_CLASS_BASE`
  CDefClassBase with only the retail slots it touches (18 Transfer, 19 Copy, 20 GetSizeofClass, slot-0 dtor).
- No local CBaseClass, CSpawnedFuncBase, CBaseClassNonCopyable, CASuspendableProcess or CSuspendableProcessMicrothread
  anywhere. 0x00F0EDA0 is the spawned thread's own body, not the spawn object, so it needs only CBaseClass.h.
