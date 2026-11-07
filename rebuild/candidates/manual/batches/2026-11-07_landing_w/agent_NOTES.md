# chunk 846 notes

Result: 2 RELOCATION_MATCH (src/), 4 parked (differ/). Every attempt is logged in STATUS.tsv.

## 0x0051FD80 CThingManager::LoadSingleThing: RELOCATION_MATCH (src/0051fd80.cpp)
- Started from the Codex pass 12 RELOCATION_MATCH source, which used a second header model. As-is it was SRC_FAIL (61 errors).
  It is now rebased onto the shared headers with additive opt-ins: CPersistContext ISBINARY, CClassFactory ALLOCATECLASS,
  CQuestManager ONACTIVATEENTITY, CEntityRuntimePersistence DEAD, fable_map STLP_TREE_CONSTFIND, CThingManager LOADSINGLE and
  CThing CTHING_LOADSINGLE (see hdr/README.md).
- Callees (two sources each; full symbols in NAME_FIXES.tsv where the bound spelling is wrong):
  - AllocateClass 0x00A371C0.
  - GetDef<CThingBaseDef> 0x00528760 (member template).
  - The map _M_find member templates 0x00527D80 (UIDToThingMap) and 0x00527D20 (persisted-dead map).
  - CheckAndFixUID 0x0051F340.
  - OnActivateEntity 0x004AFA60.
  - The CThing slot-4/8/9/10/16 virtuals of vtable 0x01239ABC.
- vland: RELOCATION_MATCH PASS. The only WARN is a RELOC_NAME label note for CCharString::operator const char* (logs/vland.log).

## 0x00520D00 CThingManager::LoadEntitiesFromScript: RELOCATION_MATCH (src/00520d00.cpp)
- Also started from the Codex pass 12 source and was rebased the same way. Extra opt-ins: CLevelLoader LOADALL (static
  LoadAllSectionName 0x013BAB08 + inline GetLoadAllSectionName) and fable_string CCHARSTRING_LESS_INLINE (CCharString::operator<
  inline over CBasicString<char>::operator< 0x00429950).
- The vector<CThing*> growth path binds `_M_insert_overflow` 0x00436AC0 under its STLport spelling (NAME_FIXES).
- vland: RELOCATION_MATCH PASS (one RELOC_NAME WARN: the list dtor label).

## 0x00657580 NPlayerGui::CDrawQuickAccessIconsPC::Draw: parked, DIFFER(1057v1057) md=24 (differ/00657580.cpp)
- Name proven (was vfunc_2): vtable 0x0125A804 slot 2. Slot 0 is the landed ClearPrimitivesIfNecessary 0x00657550, slot 1 the
  `ret` body 0x00442760. The row calls the base Draw 0x00652FA4 first (slot 2 of the base vtable 0x0125A704). Both PDBs spell it
  ?Draw@CDrawQuickAccessIconsPC@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z.
- Shape: base Draw, then hero = PeekPControlledCreature() (return if null/dead), then PeekTC<CTCInventory>. lastUsed is the last
  slot with GetQuickAccessItemGraphicIndex > 0, counted over GetNumberOfQuickAccessItemButtons().
  - One slot: the HotBarSingleGraphic sprite into handle 0, and handle 1 is cleared.
  - Several slots: the HotBarMainGraphic sprite at the last slot's offset, then the end cap HotBarEndGraphics[lastUsed-2], shifted
    left by GetGraphicMaxWidth, into handle 1.
  - No slot used: both handles are cleared.
- Callees:
  - GetNumberOfQuickAccessItemButtons 0x00445950 (new header opt-in; the bound spellings are wrong: NAME_FIXES).
  - PeekGuiDef 0x00438570 (class-qualified spelling).
  - GFAnchorToBottomLeft 0x00643CDA, GFToScreen 0x00643BE2 (Ego_r ?GFToScreen@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z).
  - CTCInventory::GetQuickAccessItemGraphicIndex 0x00581FA4.
  - CDrawBase::GetPlayer 0x00643D7C and ScaleAlpha 0x00643D5D.
  - CGraphicDataBank::GetGraphicMaxWidth 0x00A014D0 (vslot 23).
  - The sprite position ctor 0x00493AC2 (W4 enum spelling, new header opt-in).
  - CIEngine::SetPersistentPrimitive (slot 23) and CEnginePrimitiveHandle::Clear.
- Descriptive name: `g_GuiButtonAlpha` for the unsigned char at 0x01375744 (initial 0xFF; read only by the ability-button /
  quick-access draws). No PDB name was found.
- 0x00643BE2's catalog label "NUISystem::CComponent::CanMove" does not match the Ego_r GFToScreen body. It is a 13-byte fastcall
  shape and may be an identical-code fold, so it is noted here and not put in LABELS.tsv.
- Residue: branch 2 only. Retail emits fld [offset]; fadd [anchor], and VC7.1 emits the operands swapped for every spelling tried
  (10 attempts).
- Header debt (left local in the row, too large for this chunk):
  - engine/NPlayerGui_CDrawBase.h has the wrong slot order. Retail: 0 ClearPrimitivesIfNecessary, 1 the `ret` body (ClearPrimitives),
    2 Draw. The header also lacks GetPlayer / ScaleAlpha and types PrimitiveHandles as vector<CEnginePrimitiveHandle*>; retail holds
    vector<CEnginePrimitiveHandle> at +0x1C.
  - engine/CGraphicDataBank.h has no vptr (its CBankFile base is on HOLD).
  - There are no headers for CPlayerGuiDef (Ego_r names, retail offsets: DPadRingsPos +0x174, HotBar* +0x700..+0x724,
    QuickAccessItemButtons +0x728), CQuickAccessItemButtonGuiDef (0x50: ButtonGraphic +0x3C, Position +0x40, Offset +0x48),
    CDrawQuickAccessIcons or CDrawQuickAccessIconsPC (HotBarPrimitiveHandles +0x94). CTCInventory has no retail header.

## 0x00816410 NHitLocations::CPhysicalPrimitiveCylinder::GetClosestPointToPos2D: parked, DIFFER(1122v1174) md=194 (differ/00816410.cpp)
- Name proven (was vfunc_10): Ego_r ?GetClosestPointToPos2D@CPhysicalPrimitiveCylinder@NHitLocations@@UBE?AVC3DVector@@ABVCThing@@ABV3@AAV3@@Z.
  Both copies call the GetRootsNumerically instantiation (Ego_r 0x00810A10 / retail 0x008185B0).
- Shape:
  - GetCylinderBaseAndTop 0x008158D0, then the inline GetCylinderRadius (as in the landed IsPosInside; GetCombatRadius 0x00661F90
    for creatures).
  - XY axis, length, centre. If length > radius: unit axis, perp = (axis.Y, -axis.X), half = length/2. Otherwise: axis (1,0,0),
    perp (0,1,0), half = radius.
  - major/minor, then NMaths::GetRootsNumerically(0, 1, functor(major.Dot(offset), -minor.Dot(offset), |major|^2-|minor|^2), 2, 0).
  - With two roots it keeps the nearer of centre + major*sin(2*pi*r) + minor*cos(2*pi*r); the normal is the XY direction from the
    centre (AccurateNormalise 0x00A14440 when its squared length exceeds DELTA_NOTIONAL_ZERO squared).
  - TWO_PI is ?TWO_PI@@3NB 0x0124F2B8.
- Functor: anonymous-namespace CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint {float Ca, Cb, Cc} (Ego_r layout;
  FableWin has its ctor, IsValid and GetValue).
- The anonymous-namespace hash differs per TU (?A0x5ebd2a70 in Ego_r), so the emitted 0x008185B0 symbol will not equal the PDB text.
- Residue: frame 0x6C v 0x60.
  - Retail reuses top's slot for the roots vector, keeps axis at 0x20 and spills perp to 0x14, packing major over it.
  - Base/top in a block plus the functor as a call temporary (e1_11) already reproduce retail's slots for base, top, the functor and
    length (in the dead `thing` argument slot).
- 14 attempts.

## 0x00D0B3E0 NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS::Main: parked, DIFFER(3802v3802) md=36 (differ/00d0b3e0.cpp)
- Name: both PDBs ?Main@CCROWDBANDITS@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ; the catalog label already agrees. The nested
  class is defined in the row: ParentClass* +0x14, PMasterData +0x18 (Ego_r).
- Shape: the full decode is in the source.
  - Untargetable spectator until BanditKingFightStarted.
  - Then attacks under 3.5 and reacts beyond 6.5 when the timer has run out: boast set (king hurt), wave set (hero more than 8 from
    the king; 1/8 taunt conversation) or cocky set, until BanditKingFightEnded.
  - Then removes itself if its data string is "1", or joins the fight when AngryBanditNeeded > 0 or the hero hits it (heal-life
    excluded), decrementing the counter.
- Strings: VILLAGE_BANDIT_CAMP_BOSS, BanditKing, SND_CROWDBOASTBOOS_01/02, SND_CROWDLARGEBOASTREACT_01/02,
  TEXT_QST_009_BOSSFIGHT_TAUNTING.
- GFCharStringToInt 0x0099E7F0 uses the bound spelling.
- Residue (two documented lane gaps):
  - The per-site CScriptThing copy split: retail expands the by-value SetIsPushableByHero copy and calls 0x004ABE90 inside the
    CIsThingAlive temporary.
  - Per-exit ~CTimer: retail's loop-bottom exit shares the pre-loop expanded block.
- 10 attempts.

## 0x00DB3FA0 NScript::CQ_NewOakValeIntroScript::CNOVI_BookTrader::Main: parked, DIFFER(4106v4042) md=53 (differ/00db3fa0.cpp)
- Name: both PDBs ?Main@CNOVI_BookTrader@CQ_NewOakValeIntroScript@NScript@@EAEXXZ; the catalog label agrees. Vtable 0x012D8E80:
  0 dtor 0x00DB3EE0, 1 Main, 2 Init 0x00DB3F00, 3 GetParentScript 0x00DB3ED0.
- Class: CNOVI_BookTrader (Ego_r 32 bytes: ParentClass +0x14, PMasterData +0x18, DoneIntro +0x1C) added as the opt-in
  FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BOOKTRADER, in the style of the header's other entity opt-ins.
- Model: the landed CNOVI_BarrelThug::Main 0x00DB6C60 (CIsThingConscious predicate vtable 0x012C2FE8, movie Construct 0x006E7B60 /
  DestroyOutOfLine 0x006E7B80, CWideScreenMagicPauseEntities forceinline ctor, TRetVoid).
- Shape: each frame, START(priority 3).
  - Walk home: if more than 0.1 from home, MoveToPosition until within 2.0, then face NOVI_Theresa.
  - Hit by the hero (heal-life excluded): mutual EntitySetThingAsAllyOfThing (slot 599), AddBadDeed(2), START(4), movie + pause,
    TEXT_QST_048_TRADER_ON_HIT, face Theresa.
  - Else talked to: START(4), movie + pause.
    - GivenSweets (+0x94): INTRO_10.
    - Otherwise INTRO once (then DoneIntro) or STILL_GOT, then GiveHeroYesNoQuestion(BUY_SWEETS, ANSWER_YES, ANSWER_NO, "", true)
      and wait for MsgIsQuestionAnsweredYesOrNo.
    - On yes with gold >= 3: GIVES_SWEETS, GiveHeroObject("OBJECT_CHOCOLATE_BOX_UNGIVEABLE", -1, false), GiveHeroGold(-3),
      SetQuestCardObjective(GetActiveQuestName(), "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_04", "", "") (slots 296 / 655),
      GivenSweets = true, ClearThingHasInformation(Me).
    - On yes without the gold: NOT_ENOUGH_CASH. On no: BUY_LATER.
    - Then face Theresa.
  - Then, when TalkIntermittentTimer (+0x104) is 0, rand()%200 == 0 and the hero is within 20: timer = 3, a conversation, START(4),
    ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED, TEXT_QST_048_TRADER_ROLL_UP.
- Callee spellings are the landed ones (IsDistanceFromThingToPositionOver 0x00CBE45C as the landed HTG_Boss / AffairMan rows bind
  it).
- Residue:
  - Retail expands the walk-home GetScriptThing temporary's ~CScriptThing but calls it at the GetHealth sites. A plain inline
    ~CScriptThing reproduces that.
  - That same inline makes ~CIsThingConscious all-or-nothing. With it __forceinline, the nested ~CScriptThing also expands where
    retail calls 0x004AA840 (GOTCHAS chunk 799: needs the forbidden inline_depth(0)).
- 9 attempts. Parked early on that documented blocker after trying definition order (a5 / a7 / a8), a named scoped predicate (a6)
  and plain returns (a9), none of which are in GOTCHAS.

## Other
- 0x01375714 NoThingsToLoadPerSuspend: noted only (not applied).
- 0x004365B0 has many junk bindings. The correct one is the CVectorMap<ETCInterfaceType,CTCBase*> LowerBound const spelling, which
  is already bound once. It is not used by any row here, so it is not in NAME_FIXES.
