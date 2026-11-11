# Hand chunk 2002 (CTCTavernTable .. CTCCreatureGenerator): 2 of 4 byte-exact

## What matched (2, both RELOCATION_MATCH, vland RELOCATION_MATCH + TEST PASS)
- **CTCTavernTable::OnInitialActivate 0x007B90F0** (1257 bytes, 5 grades): the action-point loop over DUMMY_POINT_TANKARD_<n>, per-seat
  vector growth (two out-of-line resize(n) for the CIntelligentPointer vectors, the vector<bool> / vector<CThingAICreature*> resizes expanded),
  the def's AddTankards read through a same-TU GetSubDef<CTavernTableDef> body (release without a null test), tankard creation and its
  tankard / physics / created-entity / owned-entity set-up. Shape levers: FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND (retail expands the two
  in-loop GetTC LowerBounds and calls the entry one, 0x0040F020 -- a frequency split, no device needed); `long player = ...GetNumber();` before
  `Create(..., "")` (the implicit literal temporary is built after the player chain, slots line up); `GetParentThing()` as the first
  GetFreeNamedActionPoint argument (the inline accessor is evaluated before the by-value CCharString is built, so ParentThing lands in esi
  and action_points in edi as retail).
- **CTCTavernGameSpotTheAddition::ProcessInput 0x007D8570** (1340 bytes incl. the 4-entry jump table, 3 grades): states 0x27/0x28 nothing,
  0x29 -> 0x28, 0x2A moves the pointer with the tavern movement stick (offset . GetRight() / . Forward + stick * ImpulseScale, clamped to
  MinX..MaxY, PositionMesh) and judges a pick (lose / correct / win, highlights), default -> CTCTavernGame::ProcessInput; then the quit
  prompt in the addition states. Shape levers: `offset = Pos - GamePositionPos;` as an assignment (retail keeps the operator- temporary and
  copies it), position written field by field (a 3-argument constructor evaluates Y first), `GetGameState()` (inline PDB accessor) as
  SetStateAfterYesNo's last argument -- the last residue was one expression-temporary rotation step (c2prio --rotation: the accessor's
  return value removes one rotating temporary). Jump table 0x007D8A9C checked by hand against the object's relocations.

## Parked for Fable 5.1 (1)
- 006224c0 CTCInventoryQuests::OpenInventoryPC DIFFER(1335v1338): the local `quests` vector escapes into the declaration-only
  ConstructQuestListPC 0x0061E6D0, so it is zeroed at [esp+20h] (retail [esp+2Ch]; cards [esp+20h]) and its _M_finish is reloaded from memory
  in the card loop (ours `mov ecx,[esp+24h] ; cmp ecx,[esp+28h]`, retail 0x00622839 `cmp ebx,[esp+34h]` with _M_finish in ebx, plus retail's
  `xor ebp,ebp` 0x0062278C / 0x006227E8) | tried: CList-typed list (fixed the push_back temporary), named title child (fixed the vtable-before-
  argument order), explicit instantiation of vector<CThing*>::_M_insert_overflow (no effect, removed) | next: land as a pair with
  0x0061E6D0 -- retail compiles ConstructQuestListPC first in this TU; a same-TU noinline body of it (diagnostic stand-in,
  probe/006224c0_pair_standin_probe.cpp) makes the row RELOCATION_MATCH with no other change. Its real body is hand882's parked
  differ/0061e6d0.cpp (12357v12356).

## Skipped (1)
- 0074bc30 CTCCreatureGenerator::DrawDebugInfo: not a function start (the row begins at `push esi` 16 bytes into 0x0074BC20, see SPLITS.tsv);
  q.py grades the fragment, so it cannot match. The real row 0x0074BC20 (1290 bytes) was chunk 728's (DIFFER 1415v1290) and the inline-split
  research's (DIFFER 1351v1290); the oracle has no 0x0074BC20 entry, so it was not regraded here.

## Names (all exact PDB spellings, two sources each; see LABELS.tsv)
- Rows: OnInitialActivate (vtable slot 6 + PDB), ProcessInput (vtable slot 28 + PDB), OpenInventoryPC (slot 86 + PDB + Ego_r order).
- Callees proven here: CInputProcessGameBase::IsEventButtonPressed (IBE, protected) / IsEventGameAction / GetControlMovementFromGameActionEvent,
  CTCTavernGameSpotTheAddition::FindTarget (private, CThingObject*, float), CTCTavernGame::PositionMesh / GetGameState / PlaySoundW,
  CRightHandedSet::GetRight, CTCNamedUserActionPoints::GetFreeNamedActionPoint (reference parameters) / GetName,
  NInventory::CTCInventoryBase::Get/SetSelectedCategoryIndex, CTCInventoryQuests::InitialiseQuestCardList, CTCCreatedEntity::SetPCreator,
  CTCHeroCentre::GetQuestCards. EGameAction values by name (GAME_ACTION_TAVERN_GAME_MOVEMENT 79, _ANSWER_QUESTION_YES 22 / _THIRD 24,
  _TAVERN_GAME_QUIT 83), states by the ETavernGameStates names of engine/CTCTavernGame.h (ETGS_ADDITION_*; that header marks the
  enumerator names unproven), TCI_* by ETCInterfaceType.h.
- Descriptive: none in the matched rows. Parameter names of GetFreeNamedActionPoint (`user`, `reserve`) are descriptive.
- NAME_FIXES.tsv: 10 retail addresses whose bound spellings in reloc_bindings.tsv are stand-ins (FindTarget, PositionMesh, the three input
  helpers, SetAsPerformedEvent, GetFreeNamedActionPoint, GetSelectedCategoryIndex, SetPOwner, GetSubDef<CTavernGameSpotTheAdditionDef>).
  The landed FindTarget row 0x007D7C40 itself is spelled `FindTarget(long)` returning CThing* and the landed FrameUpdate 0x007D8E20 declares
  FindTarget / PositionMesh with those wrong spellings (respell at landing).

## Header debt / changes (hdr/README.md)
- New: CTavernGameSpotTheAdditionDef.h, CTCTavernGameSpotTheAddition.h (needs FABLE_RETAIL_C3DVECTOR_PACKED), CTCTavernTankard.h,
  CTCCreatedEntity.h. Opt-ins: CRightHandedSet GETRIGHT_OUTOFLINE, CInputProcessTavernGame PRESSED_ACCESS (access path unproven: friend or
  using), CTCTavernGame GETGAMESTATE, CTCInventoryBase SELECTEDCATEGORYINDEX, CTCInventoryQuests OPENPC, CTCHeroCentre GETQUESTCARDS,
  CTCTavernTable LAYOUT, CTavernTableDef (polymorphic retail def), CTCNamedUserActionPoints FREEPOINT.
- The landed CTCTavernTable / SpotTheAddition / CTCInventoryQuests rows still use local models; the new headers carry the same offsets.
- CTCInventoryQuests: the member at +0x157 is Ego_r's ActiveTestQuestMode by offset arithmetic (Ego_r +0x177 - 0x20); OpenInventoryPC sets it
  to IsInventoryScreen(), which reads oddly -- recheck the name if another row disagrees.
