# Chunk 2007 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2007/hdr`)

Every change to an existing header is an additive opt-in block: with none of the new macros defined, each file preprocesses to the
repo's default text (`diff rebuild/include/engine/<f> hdr/engine/<f>` shows only the opt-in). Seven headers are new.
Three files are carried over unchanged from the chunk 2002 hand-back (`pending/2026-11-11_acctB_c2002/hdr/`, not yet in rebuild/include):
`CTCTavernGame.h` (FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE), `CRightHandedSet.h` (GETRIGHT_OUTOFLINE) and `CInputProcessTavernGame.h`
(PRESSED_ACCESS); land them with (or before) these.

## New headers
- `engine/Coin.h` (row 0x007CF9A0): Ego_r.pdb `Coin : Mass` (60 bytes): Owned +0x24, Thing (packed CIntelligentPointer<CThing>) +0x25,
  Radius +0x30, Owner +0x34, Sound +0x38; ctor / copy ctor / Update / UpdateSound(bool) declared. Evidence: Ego_r layout + the landed copy
  constructor 0x008EFF40 (same offsets) + CalcScores 0x007CF860 / ProcessInput (Pos +0x04, Vel +0x0C).
- `engine/CTCTavernGameCoinBase.h` (row 0x007CF9A0): RTTI `CTCTavernGameCoinBase : CTCTavernGame`, vtable 0x0126F224 overrides, the Ego_r
  method set; members at Ego_r - 0x18 (Arrow +0xF0 .. MousePosEffect +0x13C, sizeof 0x148). Evidence: Ego_r layout + the landed ctor
  0x007D09C0 (+0xF0, +0xFC..+0x108), NewRound / CalcScores (BoardSize +0x130), ProcessInput (MousePosEffect +0x13C).
- `engine/CTCTavernGameShoveHaPenny.h` (row 0x007CF9A0): RTTI `: CTCTavernGameCoinBase`, vtable 0x0126F2F4 slots, Ego_r method set;
  RoundsPlayed +0x148 (landed EnterGame / GetAdditionalInfo), AverageJoystickPos +0x14C (ProcessInput).
- `engine/CTavernGameShoveHaPennyDef.h` (row 0x007CF9A0): RTTI `: CTavernGameDef` (0x1A4) and `CShoveHaPennyRegionDef : CSubDefClassBase`;
  Ego_r members in order, Regions (12-byte STLport vector) +0x1A4, everything after 8 below Ego_r (sizeof 0x200, as the generator's
  retail container layout in engine/_quarantine/ computes). Evidence: CalcScores reads Regions +0x1A4/+0x1A8, NewRound PositionCoinDepth
  +0x1C8, ProcessInput SpeedPC / Minimum / Maximum +0x1BC..+0x1C4 and JoystickScalePCX/PCY/TranslateY/AverageJoystickMagAbove/AverageDrift
  +0x1DC..+0x1F0 (each name at its Ego_r offset - 8). StatGetTypeName inline (literal 0x01243D54 in GetSubDef 0x007D1260).
- `engine/CTCTrophy.h` (rows 0x006DE8C0 / 0x006DEEE0): RTTI `CTCTrophy : CTCBase`, vtable 0x0125FC54, the Ego_r / FableWin method set with
  PDB access (private SendPauseEvent / StartDialogBox / GameDebugText / PlaySoundW / StartMinigame / TerminateMinigame /
  UpdateGainedWitnessSounds), public static `InstructionsText[]` (0x013BAB84); members at the retail offsets (lists 4 bytes): PDef +0x0C ..
  Mountable +0x48, sizeof 0x4C. Evidence: Ego_r layout + the landed FrameUpdate / ctor / dtor offsets + both rows.
- `engine/CCreatureAction_UnsheatheToShowToSheatheTrophy.h` (row 0x006DEEE0): RTTI `: CCreatureAction_UnsheatheItemFromInventory`,
  ShowAnimName +0xB8, Trophy +0xBC, sizeof 0xC4; ctor 0x006DF800 / copy ctor 0x006DFC50 / dtor 0x006DF860 declared. Evidence: Ego_r layout
  (208 - 12 for the base's containers) + the landed ctors + GlobalUpdate's 0xC4-byte local at the end of its frame.
- `engine/CTCCreatureExpression.h` (row 0x00665860): RTTI `: CTCBase`, TCI_CREATURE_EXPRESSION (12), Update(CAnimateThing&, const
  CAnimateThingUpdate&) 0x00729200 (Ego_r / FableWin).

## Opt-ins added to existing headers
- `CInputProcessGameBase.h` FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE: protected `IsGameActionEventInQueue(EGameAction, EInputButtonEventType,
  float&) const` 0x00687D80 (Ego_r / FableWin IBE spelling; body shape in NAME_FIXES.tsv) + the MS forward enum EInputButtonEventType.
- `CInputProcessTavernGame.h` FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_EVENTINQUEUE_ACCESS: `using CInputProcessGameBase::IsGameActionEventInQueue;`
  (same unproven access route as chunk 2002's PRESSED_ACCESS: a using-declaration keeps the base symbol).
- `CTCCarryable.h` FABLE_RETAIL_CTCCARRYABLE_NONCONST_CARRIER: `CThing* GetPCarriedByThing()` 0x00705180 (QAE twin of the const 0x00705170;
  both PDBs list both overloads; the trophy rows call 0x00705180).
- `CMessageEventManager.h` FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL: the FindMostRecentMessage member template declared only
  (binds the out-of-line COMDAT 0x004AB8B0 for CMessageEventFilter_IsOfType).
- `CMessageEventFilters.h` FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR: the IsOfType inline constructor alone (added to the existing
  `#if` that already enables it for PAYMENT / YELLFORHELP).
- `CFactionManager.h` FABLE_RETAIL_CFACTIONMANAGER_BYCRC: `GetPFaction(unsigned long) const` 0x006B93A0 (Ego_r / FableWin
  `?GetPFaction@CFactionManager@@QBEPAVCFaction@@K@Z`).
- `CTrophyDef.h` FABLE_RETAIL_CTROPHYDEF: the polymorphic `CTrophyDef : CSubDefClassBase` (RTTI) with typed members at the Ego_r offsets
  (no containers); the generated flattened layout stays the default.
- `CThingAICreature.h` FABLE_RETAIL_CTHINGAICREATURE_STATEGROUPENABLED: `SetStateGroupEnabled(const CCharString&, bool)` 0x008302D0.
- `CThingCreatureBase.h` FABLE_RETAIL_CTHINGCREATUREBASE_UPDATEANIMATION: protected UpdateAnimation(float) 0x00665860 /
  UpdateAnimationBulletTime() 0x00665DE0, public GetAnimationSpeedMultiplier() const 0x006655A0 / IsAnimCombinationAnim(long) const 0x00662820.
- `CTCCreatureNavigation.h` FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE: GetNavigatorType() const 0x006CB6A0.
- `CTCTalk.h` FABLE_RETAIL_CTCTALK_UPDATE: Update(CAnimateThing&, const CAnimateThingUpdate&) 0x006E3B90.
- `CCreatureActionBase.h` FABLE_RETAIL_CCREATUREACTIONBASE_UPDATEANIM: public UpdateActionAnimation() 0x00694890.
- `CAnimateThing.h` FABLE_RETAIL_CANIMATETHING_UPDATEANIMATION: IsMainAnimFinished() const 0x0070C0E0 (FableWin public; Ego_r's
  IsAnimationFinished is private), inline IsInterruptable / SetInterruptable (bit +0x94) and GetMainAnimType (MainAnimSet.Type +0x50), all
  FableWin spellings; RemoveCombinationAnimsOfType(long) 0x0070D8C0.
- `CAnimSet.h` FABLE_RETAIL_CANIMSET_TYPE: inline GetType() (+0x50) / GetFlags() (+0x40) and IsFinished() const declared (COMDAT 0x00668A80).
- `CAnimationEntry.h` FABLE_RETAIL_CANIMATIONENTRY_BLENDTIMES: GetDelayTime() const 0x005DC1F0, GetTimeToBlendIn(const CThing&) const 0x005DC230.
