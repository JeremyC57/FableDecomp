# Hand chunk 578 (NScript::CQS_MeetSisterScript .. std::vector<CFeatDef>): 9 of 10 byte-exact

All 9 graded RELOCATION_MATCH; 0 MATCH; 0 retail_asm. Every test PASSes under g++ (`tall.sh`) and VC7.1 (`tall.py`). The read-only
`vland.py` run: 9 wins, no RELOC_IDENTITY, no DIFFER. It gave two kinds of WARN: RELOC_NAME on 0x00CB8930 and 0x00F2D1C0 (both in
NAME_FIXES.tsv), and SHARED_HEADER_AVAILABLE CSpawnedFuncBase on 0x00E27920 (see Header debt). Every relocation target and every
string literal was checked against retail with `probe/rel.py`: 0 mismatches.

## What matched (9)
- **NScript::CQS_MeetSisterScript::Main** 0x00E28DE0. Slot 2 of quest vtable 0x012E2194.
  - Two AddEntityScriptBinding calls: MeetSisterMessenger (Alloc 0x00E28F70, Flags 1) and MeetSisterSister (Alloc 0x00E29040, Flags 0).
  - Then CScriptBase::PostAddScriptedEntities, Game->PostAddScriptedEntities, and SetQuestCardObjective(GetActiveQuestName(),
    "TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY", "HauntedHouse", "BarrowFields").
  - Shape: the landed 0x00E9C4B0 binding model. Each new-expression's name string is a conditional temporary; the objective
    strings are built right to left.
- **NScript::CV_MazeResearchScript::Main** 0x00EA7800. Vtable 0x012EE444.
  - Same shape: EmptyGrave (0x00EA7990) and HistoryBookcase (0x00EA7A60) bindings, both Flags 0.
  - Objective "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01", "HeroGuildComplexInside", "".
- **NScript::CQS_GuardianTrophyDealerInfoScript::Main** 0x00E27920. Vtable 0x012E2018.
  - GTDI_Maze binding (0x00E27BB0), then SpawnFunction(new CSpawnedFunc<T>("WaitForPieceOver", 0, this, &T::WaitForPieceOver)).
  - Then the objective, with "HeroGuildComplexInside" twice.
  - Shape: SpawnFunction is a `__forceinline` wrapper; the region-name CCharString is built after the new-expression.
- **CFeatDef::Transfer(CPersistContext&)** 0x00F2D330. Slot 18 of CFeatDef vtable 0x012FB778 (no RTTI).
  - 15 `CPersistContext::Transfer<T>(key, member, default)` calls, in Ego_r.pdb member order.
  - Uses `#define FABLE_RETAIL_DEF_CLASS_BASE` + `engine/CDefClassBase.h`. CFeatDef declares its real overrides (dtor, Transfer,
    Copy, GetSizeofClass).
  - Shape: the defaults are temporaries built per call (`CDefString()` with `TablePos(-1)`, `0.0f`, `0L`, `false`,
    `EFeatAttackType(0)`).
- **std::vector<CFeatDef>::operator=** 0x00F2D010. The landed CConversation 0x00E54AA0 model with the real
  STLport vector, over the shared def header. Callees: `_M_allocate_and_copy` 0x00F2D690, `_M_clear` 0x00F2D1C0,
  `CFeatDef::operator=` 0x00F2D200, `_Destroy` 0x00F28F20, `CFeatDef(const CFeatDef&)` 0x00F2D270.
- **NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Init** 0x00F05F30. Vtable 0x012F7D88.
  - The ally / follow / information calls, then SetBanditAllegiences 0x00F060D0.
  - `InvalidRegionTimer = 0`, written through the inline CTimer::operator= (g_pGameScriptInterface slot 89 SetTimer).
  - Five separate flag clears, the SG_MINION_SIT_OUTSIDE stategroup off, then the
    CSpawnedFunc1<…, CScriptThing> "ParentClass.WatchForHeroAttackOnBandit" spawn (the landed CAT_Sentry::Init 0x00F08E90 block).
- **NScript::CV_FisticuffsClubScript::CFisticuffsMember::Init** 0x00E83A20. Vtable 0x012EB0F8.
  - Shape lesson: retail's `sub edx,0` / `jne` chain needs the `switch (rand() % 3)` to have an EXPLICIT `default:` that repeats
    case 0's two assignments. Cases 0/1/2 alone gave 406v411.
  - `rand` is `extern "C"` (0x00BFEB16, `_rand`).
- **NScript::CV_HauntedHouseScript::SendBodyguardsToTheRightPlace(CCharString, CCharString)** 0x00E96C30.
  - The real STLport `std::vector<CScriptThing>` local.
  - VC emits the inline `~vector` (`_Destroy` loop + free 0x00BFEA14) on the normal exit, and the out-of-line 0x008AC970 on the
    terminating returns.
- **NScript::CQR_EscortTraderScript::TravellingWrongPath()** 0x00E245E0.
  - Iterator loops over `RouteDynamic[TradeRoute]` / `RouteStatic[TradeRoute]`, with an erase 0x004B8770 + break.
  - Shape lesson: putting the CBasicString<char>::Compare body (landed 0x00411570) in the TU under `#pragma auto_inline(off)`
    lets VC7.1 treat the compare as not clobbering the members. Without it: 363v372. The Leaf pins the graded symbol.

## Parked for Fable 5.1 (1)
- 00eb55d0 NScript::CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::Main DIFFER(362v370): retail has three differences —
  `sub esp,0x14` v `0x10` with an entry store of dword 0 into the extra slot (a never-tested conditional-temporary flag); a
  dead `mov bl,al` of the DoesAssassinTrustHero result; and, as a consequence, ebx/edi swapped for Me / MyEntityHandle
  | tried: braced predicate scope, identical if/else arms, `(b||!b)` guards, a ternary loop condition, a duplicated loop,
  unreachable code after the final loop, a `false &&` CCharString temporary, the QFE-4035 compiler (all compile-only probes,
  1 official grade)
  | next: grade the twin 0x00EB3F40 (the same quest's other entity Main, same dead slot + `mov bl,al`) jointly. Look for a
  discarded by-value temporary in an unused-result call (e.g. a `CCharString`-returning debug name) that the optimiser
  kills after the flag store.

## Skipped
None.

## Label corrections
Propagated manifest / catalog labels that are wrong:

| Address | Catalog label | Correct name |
|---|---|---|
| 0x00E245E0 | StringList_FindDuplicateEntry | NScript::CQR_EscortTraderScript::TravellingWrongPath |
| 0x00E96C30 | DoMission_BodyGuardSimple | NScript::CV_HauntedHouseScript::SendBodyguardsToTheRightPlace |
| 0x00F2D010 | std_vector_assign_CFeatDef | std::vector<CFeatDef>::operator= |
| 0x00EB6200 | CMurderTwistSheriff::Init | NScript::CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::AttackHero |
| 0x00CB8930 | CTCVillage::OnInitialActivate | CScriptBase::PostAddScriptedEntities |
| 0x00F2D1C0 | CRetailVector_poly0x64::~CRetailVector_poly0x64 | std::vector<CFeatDef>::_M_clear |
| 0x00F2D4D0 | Transfer<EKNAttackType> | CPersistContext::Transfer<EFeatAttackType> |

The rest of the rows were `sub_<addr>`. Landed spellings proven wrong are in NAME_FIXES.tsv (11 rows).

## Names and evidence
All 10 row names are proven (Ego_r.pdb / FableWin.pdb symbol plus at least one structural source). None is descriptive.
- Quest Mains (0x00E28DE0, 0x00EA7800, 0x00E27920): slot 2 of the class's own quest vtable (slot 1 RegisterMain, 3 Init, 4 OnPersist),
  plus the PDB `?Main@<Class>@NScript@@UAEXXZ`, plus the bound entity names and objective strings that belong to that quest.
- Entity Inits (0x00F05F30, 0x00E83A20) and the parked Main 0x00EB55D0: slot 2 (Init) or slot 1 (Main) of the entity vtable that the
  class's landed Alloc stores, plus the PDB `EAEXXZ` symbol. The members written are the PDB members at their PDB offsets.
- CFeatDef::Transfer: slot 18 of the vtable stored by the landed copy ctor 0x00F2D270. GetSizeofClass (0x00F2D2F0) returns 0x64 = PDB
  sizeof. The 15 keys are exactly the PDB members, in order.
- vector<CFeatDef>::operator=: the STLport source shape, plus callers of the landed CFeatDef copy/assign.
- TravellingWrongPath: the PDB `QAE_NXZ` (the class's one bool() method of this shape), plus the members it reads (WrongTurnDetected,
  TradeRoute, RouteDynamic/RouteStatic[3], Previous/CurrentLoadedRegion) at their PDB offsets.
- SendBodyguardsToTheRightPlace: the PDB `QAEXVCCharString@@0@Z` (FableWin.pdb also has the CQ_BreakSiegeScript twin), plus the
  "BodyGuard" literal. Its caller is CM_HeroInsideHouse::Main 0x00E96660 (call site 0x00E96888).
- Not recovered: the enumerator names of EAssassinState (0x00EB55D0) and EFeatAttackType (0x00F2D330). The sources use the numeric
  values, `EFeatAttackType(0)`.

## Header debt
- CDefClassBase: the shared header (FABLE_RETAIL_DEF_CLASS_BASE) expresses the retail layout fine for CFeatDef. No debt.
- CSpawnedFuncBase: RESOLVED at landing (owner rule, coordinator review 2026-10-29). The local stand-ins in 0x00E27920 /
  0x00F05F30 had wrong vtables (SuspendableProcess in slot 1 or pure, `void SuspendSuspendableProcess(bool)`, no
  OnPredicateFail / OnInterrupted). Both now use the new retail opt-in FABLE_RETAIL_SUSPENDABLE_PROCESS (engine/CSpawnedFuncBase.h
  : CSuspendableProcessMicrothread.h : CASuspendableProcess.h : CBaseClassNonCopyable.h : CBaseClass.h; vtable 0x012C2F9C,
  7 slots), and 0x00F05F30's local CBaseClass is engine/CBaseClass.h. Both regrade RELOCATION_MATCH + PASS (0x00F05F30 landed in
  a second vland pass after the swap).
- CScriptBase (retail 0x40, polymorphic), CGameScriptInterfaceBase (a padded vtable class, generated by `tmpl/gen.py`),
  CScriptThing, CEntityScriptBase and CCharString are local models, as in every landed NScript row.
