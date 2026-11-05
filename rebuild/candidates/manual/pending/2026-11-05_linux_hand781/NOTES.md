# Chunk 781 notes (the CV_DemonDoorsScript TU rows + two quest-script Mains; script TUs, no Flags)

## Result
| Row | Name | Result | Source |
|---|---|---|---|
| 0x00E70360 | CDF_SummonsCreatures::GetParentScript | MATCH | src/ |
| 0x00E6FC40 | CDF_Dragon::Main | RELOCATION_MATCH | src/ |
| 0x00E702C0 | CDF_SummonsCreatures::Alloc | RELOCATION_MATCH | src/ |
| 0x00E74D70 | CDF_SummonsCreatures::Main | RELOCATION_MATCH | src/ |
| 0x00E9CFD0 | CV_IntroductionToTrophiesScript::CTrophiesIntroWatcher::Main | RELOCATION_MATCH | src/ |
| 0x00D6A560 | CHTE_FreedomFighter::Main | parked, DIFFER(2058v2058) | differ/00d6a560.cpp |

- The five matched rows pass vland (tmp/vland.log; the jump-table run is tmp/jt.log).
- 0x00E6FC40 also passes `verify_land_jumptable.py` (1664/1664 with the table, behaviour test PASS).
- The parked row has one instruction placed differently and is the same length as retail.
- All six names are PDB-proven from two sources: the vtable slot plus the Ego_r/FableWin name sets. No name is descriptive
  except the locals (`exitPoint`, `fighter`, `guard`, `conversation`, `dragon`, `face`, `answer`, `generators`, `creatures`).

## 0x00E6FC40 NScript::CV_DemonDoorsScript::CDF_Dragon::Main -- RELOCATION_MATCH (1664 bytes = body + 8-entry jump table)
- Row extent: the body 0x00E6FC40..0x00E7029F, then the switch table 0x00E702A0..0x00E702BF, which belongs to the function.
  The 1828-byte chunk780 ledger row also swallowed CDF_SummonsCreatures::Alloc and GetParentScript; split them (LABELS.tsv).
- Jump table, checked by hand. The built object's `$L` relocations at +0x660..+0x67C and the retail dwords at 0x00E702A0
  agree case by case:

  | Cases | Retail target | Built label | Line |
  |---|---|---|---|
  | 0, 1 | 0x00E6FE99 | $L6069 (+0x259) | RENOWN_ZERO |
  | 2, 3 | 0x00E6FF39 | +0x2F9 | ONE |
  | 4, 5 | 0x00E6FFD9 | +0x399 | TWO |
  | 6 | 0x00E70079 | +0x439 | THREE |
  | 7 | 0x00E70162 | +0x522 | FOUR + door + ActivateQuest + return |
  | default (`ja`) | 0x00E7011B | | the break target |

  The table base $L6499 is at +0x660 = 0x00E702A0.
- Model: shared headers only.
  - engine/CV_DemonDoorsScript.h with FABLE_RETAIL_BASE_OBJECT, CCHARSTRING, CLEVELSCRIPTBASE and NSCRIPT, plus the movie,
    interface, CIsThingAlive and CWideScreenMagicPauseEntities headers.
  - The one lever: `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` declared out of line.
    Retail calls 0x006E7AB0 at the loop-continue movie dtor.
  - The pause FORCEINLINE_CTOR opt-in is defined for one model across the TU.
- Callee and slot list: the source header. Every target was checked with objdump -r against retail.

## 0x00E702C0 CDF_SummonsCreatures::Alloc -- RELOCATION_MATCH
- The landed CDF_Dragon::Alloc 0x00E6FB70 model, renamed. Vtable 0x012E8E80; the "DF_SummonsCreatures" binding of
  CV_DemonDoorsScript::Main.
- Its test is the landed Dragon Alloc test, renamed.

## 0x00E70360 CDF_SummonsCreatures::GetParentScript -- MATCH
- `return &ParentClass;` on engine/CV_DemonDoorsScript.h, the landed CDF_Dragon::GetParentScript model.
- The name is FableWin-only (slot 3 of vtable 0x012E8E80).

## 0x00E74D70 CDF_SummonsCreatures::Main -- RELOCATION_MATCH
- Shape:
  - the Dragon prelude;
  - on talk, the WARNING line, then GiveHeroYesNoQuestion and a MsgIsQuestionAnsweredYesOrNo wait;
  - on yes, the INTRO line, then pGlobalScriptDef->DemonDoor_SummonCreature_NumCreatureTypes rounds. Each round:
    GetAllThingsWithScriptName("DemonDoor_NastyGen"), CreateCreature at each generator's GetPos ("DD_GeneratedNasty"),
    then a wait on AreAllThingsInVectorDead 0x00CBED00;
  - then, if the face lives, the OPENING line and FadeOutFaceAndOpenDoor(Me, "DD_SummonsCreatures").
- Levers:
  - FORCEINLINE_CTOR, so the second pause ctor is inline.
  - The `!Me.IsAlive()` arm written first, so retail's fall-through exit is right.
  - No PImp-dtor specialization: VC7.1 then calls the whole movie dtor COMDAT 0x006E7B80 at every site, as retail does.
  - TU note: CDF_Dragon::Main needs that declaration and CDF_SummonsCreatures::Main must not have it. Variant d
    (FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR) also matched but was not kept, to keep one model.
- STLport: vector<CScriptThing> with the per-TU STLP_VECTOR_PUSHBACK_OUTOFLINE and ERASE_RANGE_OUTOFLINE opt-ins
  (push_back 0x008ADF90, erase 0x008AC9D0, ~vector 0x008AC970).

## 0x00E9CFD0 CV_IntroductionToTrophiesScript::CTrophiesIntroWatcher::Main -- RELOCATION_MATCH
- Label to correct: the ledger's `NScript::CTrophiesIntroWatcher::Main` lacks the enclosing script (NAME_FIXES.tsv).
- Model: shared headers. The parent and the watcher are local models at retail offsets; no engine header exists for
  CV_IntroductionToTrophiesScript, and the landed siblings use the same models.
- Header debt: an engine/CV_IntroductionToTrophiesScript.h, retail = Ego_r - 0x14. Offsets: AttackedByHero +0x4a,
  InsideTavern +0x55, CutsceneStarted +0x60, OpinionTimer +0x64, OutsideTavernTimer +0x68. The nested watcher has timer +0x1c,
  timerStarted +0x20 and initPos +0x21.
- Row-local inline bodies:
  - ~CScriptThing;
  - IsAlive;
  - CTimer::GetTime;
  - CTimer::operator= as `__forceinline`. Plain inline calls the Ego_r out-of-line copy at two of the three SetTimer sites
    (attempt a, 1814v1833).

## 0x00D6A560 CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Main -- parked DIFFER(2058v2058)
- Label to correct: the ledger's `NScript::CHTE_FreedomFighter::Main` lacks the CQ_HangingTreeEvilScript scope (NAME_FIXES).
- There is no FSE Lua port for HangingTree (seed_corpus has none), so the decode is from the retail bytes alone.
- Shape:
  - SetPredicate(CIsThingAlive(Me)), then wait for IntroCutscenePlayed.
  - Take scripted control (priority 3) and wait out SwitchingRegions. ShouldFollowHero = false, then three frames.
  - Loop while HTE_State != 6. While AllEnemiesDead, it gets "HTE_BanditExitPoint" and yells once, through
    AddNewConversation + AddLineToConversation. The line depends on MissionTimer.GetTime(): >= 90 CHARGE, >= 30 HURRY_MILD,
    else HURRY_URGENT.
  - If ShouldFollowHero:
    - ClearCommands, SetEntityAsRegionFollowing(GetHero(), Me, true), EntitySetAsAllowedToFollowHero and FollowThing(hero);
    - yell BANDITS_FOLLOWING once;
    - wait until SwitchingRegions, then ClearCommands.
  - Else, if HTE_State != 6, it does MoveToPosition(exit.GetPos(), 2.0f, 1, false, true) and walks. Each frame it counts hero
    hits; at 3 it calls GiveThingBestEnemyTarget(Me, hero) and returns. The walk ends near the exit (8.0f), or when the hero is
    within 13.0f of the exit, which sets ShouldFollowHero and clears the yell flag.
  - After the enemies loop: ClearCommands and GetNearestWithScriptName(Me, "HTE_AssaultGuard"). If the guard is alive:
    GiveThingBestEnemyTarget(Me, guard), then follow it while it is farther than 5.0f.
  - At state 6: FollowThing(hero).
- Key spellings (each is a grade-proven lever, see STATUS.tsv):
  - `const CScriptThing& guard` bound to the temporary. Retail calls IsAlive through the vptr; a by-value local is
    devirtualized, and its frame moves (k).
  - GetPos `!PImp.Data ? NullCoord : PImp.Data->GetPos()` and CTimer::GetTime, both `__forceinline`: retail expands both at
    both sites.
  - Inline ~CIsThingAlive, which gives the vtable in ebx at entry.
  - The guard block's trailing terminate check written in both arms of an if/else.
- Residue:
  - Retail: `lea ecx,[esp+2Ch]` is hoisted above the inner follow-wait loop's terminate `jne`, which jumps to 0x00D6AD54 past
    the exit block's lea; the latch has no lea.
  - VC7.1 here: `jne 0x00D6AD50`, with the lea kept in the latch IsAlive.
  - 20 spellings of that loop and its exits leave it unchanged. Next idea: the differ header, line 3.
- Header: engine/CQ_HangingTreeEvilScript.h completed in the overlay (hdr/README.md). The nested CHTE_FreedomFighter members
  and virtuals are needed by this row.

## Data names (descriptive / unproven)
- 0x0122DEDC (0.0f) is `g_fZero`, the CV_DemonDoorsScript TU siblings' spelling. reloc_bindings has 30+ spellings for this
  address and no PDB name is known. It is a named float constant, since VC7.1 would emit `fldz` for a literal 0.0f.
  This needs a lane-wide naming decision; it is not proven here.
- 0x0143E8F8 `pDSTGame`: the landed spelling (CTimer.h comment).
