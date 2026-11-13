# Hand chunk 2012 (CTCTrophy .. CTCLight): 1 of 4 byte-exact

All work graded under VC7.1 RTM 13.10.3077 with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2012/hdr`.

## What matched (1)
- **CTCTrophy::TerminateMinigame 0x006DE280** (RELOCATION_MATCH, test PASS g++ and VC7.1, read-only vland RELOCATION_MATCH PASS).
  Ends the trophy minigame: MinigameInProgress off; a creature carrying the trophy (thing type 2) finishes an interruptible action and
  sheathes it (CCreatureAction_SheatheItemToInventory(creature, 0, 100) temporary through SetCurrentAction); otherwise the holder is the
  main player's controlled creature and the trophy returns to the inventory (CTCInventoryItem::ReturnToInventory). Every gained witness
  gets SG_OPINION_REACTION_ROOT back and loses the trophy personality override; every available witness gets the state group and, when
  the def says so, the emote icon. A live holder gets the score dialog (TEXT_GUI_TROPHY_GAME_SCORED with gained / available / ahead =
  max(0, gained - available)), BestTrophyWitnesses raised, WaitingForScore; beating BestWitnessesAheadToDate gives renown ((ahead - best)
  x RenownGainedPerWitnessAhead), a deed 43 / 45 (evil / good trophy) through the holder's deed log, SoundEndNewHighScore and the
  TEXT_GUI_TROPHY_GAME_BEATEN dialog, else SoundEndNoNewScore and TEXT_GUI_TROPHY_GAME_FAILED. Finally the quest-info mini-game display is
  switched off (MINIGAME_TROPHY) and the world's region-load confirmation is marked given.
  Shape lessons: `holder` assigned inside the creature test (a separate `carrier` local) puts its frame home store after the
  PCurrentAction load (0x006DE2E5); a `sheathed` flag lets the failed null / type tests jump straight to the fallback; PlaySoundW's landed
  body in the TU (chunk 2007's form) expands at both score sites; CVectorMap::LowerBound inline (FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND)
  with **`// Flags: /d2inlT77`** (owner inline-budget decision, preference 2) and the available-witness loop behind `if (it != end)`:
  the c2 inline trace shows that two plain `for` loops give the loop-2 emote site and the after-loop hero-stats site the same score (68),
  so no flag can split them; with the guard the scores are emote 77 / hero stats 74 / PlaySoundW 76 / 76, and T77 expands exactly retail's
  three. Alternatives traced without a grade: for/while/do-while/for(;;)+break/post-increment/`!(it == end)`/`if (!empty())` (adds a
  `cmp [eax],eax` test)/guards round both loops/`&& true` in the condition (works without a flag but is not plausible source).

## Parked for Fable 5.1 (1)
- 006d1250 CTCWife::CheckAppearanceChanges DIFFER(1629v1608): ebx/ebp swap of this / hero from the end of the modifier block on (retail 0x006D15ED `mov ebp,[esp+20h]` / 0x006D15F1 `mov ebx,[esp+5Ch]`, ours the reverse), its knock-on frame homes (this [esp+14h] v [esp+1Ch]; tattoo index in the dead hero slot [esp+58h] v [esp+2Ch]+edi), and loop 1's covered_areas load order (0x006D132E) | tried: GetDef/GetSubDef bodies visible (fixed the `dec [reg+4]` releases), named / reversed find iterator (retail evaluates find before end()), ternary hair category (fixed `and/or 300h/shr 7`), declaration order, modifier / covers / fatness / tattoo-loop spellings (c2prio: allocation insensitive), `covered_areas |= modifier->...` (worse) | next: c2 piece-priority what-if on this piece #8 (prio -67) v hero piece #102 (prio -37) to find the block whose K / live-through penalty decides the order (only the find-first IL flips it; with end() first the allocation is retail's).
  Decoded body (the source in differ/ follows it exactly): hero's TC 94 modifiers -> clothing list (type 2): GetDef<CThingObjectDef>,
  GetSubDef<CAppearanceModifierDef>, covered_areas |= CoversBodyAreaFlags, unseen + covering -> FrameNoticedChangeInAppearance[SuitPart],
  insert; hair list (type 0): any hair covering 0x10 hides the hair (category 13), unseen + covering a not-yet-covered area -> category 8
  (0x100) / 7 (0x80) / 6; categories 14 ((covered & 0xE) == 0) and 15 ((covered & 8) == 0) through AppearanceBool; 15 cleared while 14 is
  set; then 13 from the hair flag; tattoo things (TC 3 CTCHeroMorph::GetTattooThings): a tattoo not covered by clothing whose def index
  (GetDefGlobalIndexFromName(GetInstantiationName())) is unseen -> category 9 + insert; hero stats fatness change beyond the marriage def's
  FatnessChangeThresholdForComment (GFFloatAbs) -> category 10 (fatter) / 11 (thinner), the other cleared.

## Not attempted (2) -- working budget; decoded outlines
- **00949ab0 CAIStateGroup_GoToPub::IsRunnable** (protected virtual, Ego_r `MAE_NXZ`; 1572 bytes, `and esp,-8` frame, three `ret` copies +
  the out-of-line ~list 0x00792240 at one exit). Outline: TC 0xAA of the creature (out pointer zero-initialised) -> UnderScriptControl
  (+0xA1) = its CCharString +0x74 != "" && +0x70 != "" (conditional temporaries: ebx flag bits 1 / 3). Under script control:
  PTable (+0x24) = GetObjectWithScriptName(copy of +0x70) (CIntelligentPointer<CThingObject> temporary, vtable 0x0125A858); the table's
  TC 0x33 -> PPub (+0x14) = its +0xD pointer (CIntelligentPointer<CThingBuilding>, vtable 0x01244344); return GetSeat(PTable, &name of
  +0x74) && GetBar(). Otherwise: NextFramePub (+0x68) < CWorld::GetFrame() and the creature's village member (TC 0x23, const LowerBound)
  village IsTavernTime -> PPub = FindNearestBuildingWithProperty(pos, 0x20); IS_THING_ALIVE(PPub) && IsStaffed() -> a local
  std::list<CThingObject*> (malloc'd head) of the pub's (TC 0x21, list +0xC) live things with TC 0x78; while not empty pick a random one
  (GetPWorldSeed LCG x 0x24A1 + 0x24DF, GFROR13, % size), PTable = it; its CTCTavernTable (TC 0x78, LowerBound expanded, key 0xB8)
  GetSeat(creature, Pos +0x40, RHSet +0x4C, ActionPointIndex +0x64, SeatIndex +0x3C, 0) -> PTankard (+0x34) = GetTankard(SeatIndex),
  clear the list, return GetBar() (else FreeSeat); a failed seat erases the node; an empty list sets ActionPointIndex = -1, FreeSeat,
  false. Names: Ego_r GetObjectWithScriptName 0x00947EB0, GetSeat 0x009482A0, GetBar 0x009490D0, IsStaffed 0x00947FB0, FreeSeat
  0x009486D0 (all `IAE` protected), CTCTavernTable::GetSeat 0x007B8840 / GetTankard 0x007B8590, CTCVillage::IsTavernTime 0x00658C0A.
- **006414a0 CTCLight::DrawLight** (protected, `ret 10h`, 1596 bytes incl. a 4-entry jump table at 0x00641ACC). Outline (sibling
  DrawFlare 0x00641AE0 is landed and shares the prologue): GetFadeDistance() dropped; Lights (+0x38) resized and a type-3 CPrimitive
  added when light_no is new; colour = GetColour() (override info +0x10 or def +0x28) to 4 floats x 1/255; intensity = lerp(OldAccumilative
  +0x24, Accumilative +0x20, draw_info factor); factor 1.0, or (def +0x42) a piecewise-linear day curve over the def's four times
  (+0x44..+0x50 x 0x0122DC88) against the world time (3-step loop, switch on the segment), or (def +0x41) a facing term from the camera
  direction (two GFFastOneOverSqrt normalisations, PeekWorld()+0x18 range +0x08 / +0x0C clamp 0..1) that also takes the colour from the
  world (+0x58); IsInverted -> 1 - factor; colour / inner / outer radius scaled; light mask by def +0x54 (switch: 1 << tc_appearance+0x40,
  -2, 1, -1); four __ftol2 bytes; CEnginePrimitiveLight built in place and sent with SetPrimitive (0x004BD620); ++0x013BA85C.

## Skipped
None (row extents checked: 0x006DE280 ends `ret` at 0x006DE8AC = 1581 bytes, next function 0x006DE8B0; 0x006D1250 ends at 0x006D1895
`ret 4` = 1608; 0x00949AB0 / 0x006414A0 extents match the oracle by the disassembly; DrawLight's jump table is inside its extent.)

## Names
- Proven (two sources each, LABELS.tsv): the two rows, ClearPersonalityOverride 0x00682310, ConstructString(JJJ) 0x009C7BA0, the
  sheathe action ctor / dtor, GetTattooThings 0x0071DE20; inline helpers named from FableWin.pdb COMDAT order (emitted right after the
  function that references them): CWorld::SetRegionLoadConfirmationAsGiven + CTCHeroStats::SetBestTrophyWitnesses (after
  TerminateMinigame), CTCWife::AppearanceBool (after CheckAppearanceChanges).
- Descriptive: local variable names; enumerator values without PDB names stay numeric (EWifeAppearanceCategories 6..15,
  EOpinionPostDeedType 2, EEmotePriority 2, thing type 2).
- Open: 0x00616580 (set<long> find, 12 callers incl. CheckAppearanceChanges) and 0x00B2EB20 (bound to the const member template
  ??$find@J@?$_Rb_tree@JJ...QBE...) have byte-identical bodies; the parked row binds the const member template through the shared set
  model, which would give one symbol two addresses -- decide at landing (the non-const QAE instance is the likely other name).

## Header debt / changes (hdr/README.md)
- New: CCreatureAction_SheatheItemToInventory.h, CTCHeroMorph.h. Opt-ins: CWorld REGIONLOADCONFIRMATION, CTCHeroStats TROPHYWITNESSES
  and FATNESS (accessors + layout branches), CTCCreatureOpinionOfHero CLEARPERSONALITY, NGameText CONSTRUCTSTRING_LONGS3, CTCWife
  APPEARANCEBOOL, CTattooDef MEMBERS. Carried over unchanged from chunk 2007's hand-back (not yet in rebuild/include): CTCTrophy.h,
  CTrophyDef.h, CTCCarryable.h, CThingAICreature.h and the rest of its hdr/.
- GFFloatAbs (FableWin ?GFFloatAbs@@YIMM@Z) is defined locally in the parked row as in the landed 0x006E3440; a shared header would be better.
