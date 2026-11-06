# chunk 800 notes

Two rows matched (RELOCATION_MATCH + PASS in vland), four parked. All sources are plain C++ (no asm). Tests: g++ (tall.sh) and VC7.1 (tall.py) PASS for all six.

## 0x00D3BC60 NScript::CQ_GuildTrainingScript::Main -- RELOCATION_MATCH (src/00d3bc60.cpp)
- Shape: GameState 0 arrival branch (weapons, GuildArrivalHSP, LookoutPoint wait, melee card, start screen, RunArrivalCutscene, fade out),
  then the training branch: GuildTrainingHSP, HeroGuildComplex wait, 14 CEntityScriptBinding<CQ_GuildTrainingScript> (binding-name
  literals give the nested script names), PostAddScriptedEntities, guards, 5 CSpawnedFunc threads (thread-name literals).
- Header: engine/CQ_GuildTrainingScript.h opt-in FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN (member and nested-class declarations,
  CWillDummy::Alloc 0x00D43360). The 13 nested classes other than CWillDummy are completed in the row with only their Alloc
  (FABLE_GT_ENTITY macro): header debt -- move them into the header once their layouts are needed.
- Names: all from Ego_r.pdb + the binding / thread-name literals (LABELS.tsv). No descriptive names.

## 0x00544D20 NUISystem::CSpellContainerList::ProcessEvent -- RELOCATION_MATCH (src/00544d20.cpp)
- Shape: rate limit (Time - LastInputTime < InputDelay), switch on the event with cases in source order LEFT/BACK, LIST_UP, LIST_DOWN,
  ROTATE_LEFT, ROTATE_RIGHT, ASSIGN, UNASSIGN (block order follows it); `this == GetExclusiveObserver()` operand order; the created
  UI_SPELL_TEMPLATE component's slot-145 call is spelled CSprite::SetGraphicIndex(SpellToAssign) (descriptive: the cast type is an
  inference -- CSprite slot 145 is SetGraphicIndex(unsigned long), and a spell icon takes a graphic index; any class whose slot 145
  takes one unsigned long gives the same bytes); CAction::AddParam (inline, non-retail branch of NUISystem_CClickable.h) pushes the
  new CActionParamULONG; action types UI_ACTION_TYPE_ASSIGN_SPELL 209 / UNASSIGN_SPELL 210 / RESET_ASSIGNED_SPELLS 211.
- Headers: shared NUISystem_CChangingStateComponent / CClickable (CAction) / CSprite / CManager / NUISystem_CSpellContainer.h
  (opt-in FABLE_RETAIL_CSPELLCONTAINER_VIRTUALS, slots 142..148). CSpellContainerList is a local class derived from the shared
  CChangingStateComponent (no repo header): its 11 new virtuals in slot order (rtti-vtables.tsv) with Ego_r.pdb names, members at
  Ego_r.pdb - 0x30 (- 4 after the vector). Header debt: engine/NUISystem_CSpellContainerList.h with that class, and
  CActionParamULONG (local here; also local in 3 landed rows, cf. NUISystem_CActionParamSLONG.h).
- Declared-only specialisations: CCountedPointer<CAction> ctor 0x0042AA29 / dtor, CCountedPointer<CComponent> ctor 0x00429C15 /
  dtor, list<CActionParam*>::push_back 0x0042B646 (declared before NUISystem_CClickable.h, whose inline AddParam instantiates it).
- vland WARN RELOC_CONFLICT (for the coordinator, not a failure): this row binds ??1?$CCountedPointer@VCAction@NUISystem@@@@QAE@XZ
  to 0x00429B72 and ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ to 0x00429C53; the landed 0x0056F615 / 0x00596A68 bind
  the same names to 0x00429160 / 0x004291DE. 0x00429B72 is a 5-byte `jmp 0x00429160`, 0x00429C53 a 5-byte `jmp 0x004291DE` (both
  catalogued TailJmp__Dest_val..., descriptive). Retail calls the stub here and the body there; one name, two addresses. Not
  resolved (no second source for which is the dtor proper); NAME_FIXES has no entry for it.
- Label corrections: SetSpellToAssign 0x00544100 (was vfunc_144), SetEmptySlotDef 0x00546060 (vfunc_145), GetBaseSpells
  0x00546070 (vfunc_148); new labels AddChildAndSetOffsets 0x005453D0, SetNextSpellContainerAlpha 0x005455A0,
  SetNextSpellContainerPosition 0x005455B0. 0x0042B646 is labelled CUIState::operator= in one catalog line; the wave-1 decision
  (std::list<CActionParam*>::push_back) is right.

## 0x00D2ED20 NScript::CQ_EndGameFocalSitesScript::CEndGameScythe::Main -- parked DIFFER(2255v2248), 14 lines (differ/00d2ed20.cpp)
- Residue: the implicit MovieBase dtor at the first speech-wait terminate exit is the COMDAT call; retail expands it and tail-merges
  both exits. Best = a10: FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR (new opt-in in the overlay) +
  FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN + a __forceinline STB operator= body + the declared-only
  CCountedPointer<MovieBase> dtor placed before CScriptGameResourceObjectMovieBase.h. 12 attempts.
- Header: new engine/CQ_EndGameFocalSitesScript.h (class + CEndGameScythe; offsets Ego_r.pdb - 0x14, read by this row at +0x81 / +0x83).
- The local `std::map<CCharString, STB>` ctor/dtor are declared-only specialisations (retail calls them).

## 0x00D0A830 NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main -- parked DIFFER(2700v2720), 22 lines (differ/00d0a830.cpp)
- Residue: one site -- the implicit MovieBase dtor at the merged "over"-speech terminate exit (0x00D0B27A) is the COMDAT call. The
  rest of the 22 lines is jump-table relocation noise (the begging switch table matches). Best = a7: MovieBase FORCEINLINE_CTOR +
  one do-while(0) as the speech-movie scope (`// Inline-split:` note, line 5 of the differ file after the Grade/Next idea lines).
  a2 (full FORCEINLINE: both sites expand but each gets a MovieBase vtable store) is also 22 lines. 8 attempts; same lane gap as
  chunk 797 0x00D18DF0 (14 grades there).
- Header: engine/CQ_BanditCampBossBattleScript.h opt-in class CBanditKing (HealthBarIndex +0x1C written by this row).

## 0x007D8E20 CTCTavernGameSpotTheAddition::FrameUpdate -- parked DIFFER(1900v1900), 22 lines (differ/007d8e20.cpp)
- Residue: one frame slot -- the board-bounds CDefPointer local is at [esp+0x14] in ours, [esp+0x18] in retail (3 instructions);
  the rest is jump-table relocation noise. 18 attempts (ATTEMPTS.log: scope, declaration and temporary variants).
- Key spellings: a same-TU body of GetSubDef<CTavernGameSpotTheAdditionDef> (the landed 0x007D9A80) so the def pointer stays in a
  callee-saved register; the vector<long> copy ctor left to the inline header body; the original precedence quirk
  `ItemsPerRound[CurrentRound + (FirstRound ? 0 : 1) % size]`; cover chain `t < A` / `t < A + B || !UpdatedKnickKnacks` / `t < A + A + B`.
- Names: FindTarget / UpdatePointerScale / PositionCover / UpdateHighlightedObject private per Ego_r.pdb; KillThing 0x005E9A60
  (CTCTavernGame). Descriptive: NGraphicAppearance::CTCGraphicAppearance::ClearMeshEffect_004bffc0 (no PDB spelling found).
- Header debt: the local CTavernGameSpotTheAdditionDef / KnickKnack / CTCTavernGameSpotTheAddition and the local
  CTCGraphicAppearance SetAsHighlighted 0x004C04C0 declaration (engine/NGraphicAppearance_CTCGraphicAppearance.h exists but lacks
  these members: an opt-in there is the follow-up).
- Header: engine/CSubtitleRenderer.h opt-in Clear(bool) 0x00492BAB (NAME_FIXES: the landed ClearText spelling is wrong).

## 0x0094AB00 CAIStateGroup_GoToPub::Update -- parked DIFFER(1968v2067), 743 lines (differ/0094ab00.cpp)
- Residue: inline budget. Retail expands the local list<CThing*> push_back and one CVectorMap::LowerBound (the table lookup) where
  VC7.1 here calls push_back out of line and LowerBound everywhere; register split and frame (0x190 v 0x188) follow. a3's structure
  is retail's (one table TC lookup reused for GetSeat / GetTankard; three Terminate exits; LowerBound call count 8 + 1 expanded);
  a2 (two lookups) is 468 lines but the wrong lookup count. 7 attempts; an exploratory plain-inline LowerBound opt-in (a5) expands
  4 of the 9 lookups, so it was not handed back.
- Names: CBrainOutput::SetAsPerformAction 0x00860130 (NAME_FIXES), CBrainOutput ctor 0x00835620 / dtor 0x006D60C0,
  CCreatureAction_GetUpFromSitting ctor 0x00860150, CTCTavernTable::GetSeat 0x007B8840 / GetTankard 0x007B8590 / SetSeatState
  0x007B8530 (Ego_r.pdb signatures), IS_THING_ALIVE<CIntelligentPointer<CThing>> 0x004CC340.
- Header debt: the model is local (CIntelligentPointer / CTCTavernTable / CTCAIScratchpad / CBrainOutput / CAIBrain stand-ins);
  shared headers exist for CAIStateGroupBase / CIntelligentPointer / CTCThingOwner / CTCCreatureModeManager / CAIBrain and are the
  next attempt (lesson from 0x00544D20), with the landed GoToPub::GetBar 0x009490D0 list shape.
