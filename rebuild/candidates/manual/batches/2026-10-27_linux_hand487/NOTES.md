# Hand chunk 487 (NUISystem::CViewport .. CVectorMap<EReactionSpeechType, long>): 40 of 40 byte-exact

12 MATCH, 28 RELOCATION_MATCH, 0 retail_asm, 0 parked, 0 skipped. Every behaviour test passes under g++ (tall.sh) and the
lane's VC7.1 (tall.py). vland dry run: WINS 40, no RELOC_IDENTITY (warnings listed under "Review" in LESSONS.md).

Names: 23 proven, 13 probable, 4 descriptive (`CUnproven*_<addr>` element or key types).

## What matched (40)

### NUISystem UI copy constructors (5, proven by vtables / RTTI and the Ego_r.pdb constructor sets)
- 0054ea60 `CViewport(const CViewport&)`: base copy 0x52CCA0, then the three vptrs, a fresh CEnginePrimitiveHandle, an
  empty STLport vector (`_Vector_base(const allocator&)` + `_STLP_alloc_proxy`), GraphicIndex 0, then GetDataFromDef 0x54E5D0.
- 005551d0 `CScrollableList(const CScrollableList&)`: base copy, then ScrollTimer, the two arrow pointers and the two flags
  zeroed from one register.
- 0055ba80 `CHoverable(const CHoverable&)`: CObserver subobject address formed before the vptr stores; members zeroed in
  declaration order; `CManager::GetInstance()->AddObserver(this)` (static `__fastcall` 0x41E5F2, slot 2).
- 0055b4c0 `CClickable(const CClickable&)`: base copy 0x55BA80, members zeroed, GetDataFromDef 0x55B040.
- 0055aa20 `CDraggable(const CDraggable&)`: `C2DVector(0, 0)` as two integer zero stores, SnapBackIfDroppedNowhere = true,
  GetDataFromDef 0x55A890.

### STLport helpers (8)
- 00569bc0 `_Rb_tree::_M_create_node` (map<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, vector<pair<float,
  CWideString> > >, probable; model from the landed _M_insert 0x56A0A0): protected in STLport 4.6 (`IAE`); placement new
  null test on `&node->_M_value_field`.
- 0056c5f0 `pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast>::pair(const&, const&)` (probable): both argument
  words loaded first, CBoast copy 0x4B73A0 on `lea ecx,[esi+4]`.
- 0056ce90 `_Construct<CUnprovenPod8_0056ce90>` (`__fastcall`, descriptive): placement-new null test then the two words.
- 005791cc `_Rb_tree<CUnprovenKey_005791cc, ...>::begin()` (/Os, descriptive): the value struct needs a `first_type`
  typedef for `_Select1st`.
- 005d8548 `map<unsigned long, long>::find` (/Os, probable): `iterator(_M_find(k))` over _M_find 0x5D83B3, result through
  the hidden pointer.
- 005e1f90 `__copy_aux<pair<unsigned long, CAnimationEntry>*, ...>(..., const __false_type&)` (`__fastcall`, probable):
  forwards to the landed __copy 0x5E18B0; the tag temporary is homed in the dead `result` slot, so no tail jump.
- 005f82bc `pair<CUnprovenKey_005f82bc, CIVCountedPointer<CUnprovenPointee_005f82bc> >::pair` (/Os, descriptive).
- 006145e0 `__find` over a list of CUnprovenPair8_006145e0 (`__fastcall`, descriptive; only caller is the unreferenced
  find wrapper 0x6157F0).

### Gameplay classes
- 0056e59b `CTCInventoryExperience::SetInventorySelectionState` (/Os): stores +0x1C and calls the empty hook 0x5BC58B
  (descriptive `NInventory::CTCInventoryBase::EmptyHook_005bc58b`); /Os keeps `call ; ret 4`.
- 0057a59b `CTCHeroStats::AddRewardMoney(long)` (/Os, donor include of engine/CTCHeroStats.h, RewardMoney +0x50).
- 005da530 `CTCWeapon::IsProjectileWeaponClass(EWeaponClass)` (static `__fastcall`, `return wc == 3`).
- 005df6b0 `CAnimComponentCombatComboChain()`: IVRefCount = 1 before the vtable store.
- 00624d90 `CTCInventoryQuests::OnItemSelected(long, bool& handled)`: `handled = true`.

### CWorld game-event actions (6, proven by Ego_r.pdb source order and the dispatch table 0x13B9288 / initializer 0x121B9B0)
- 00629240 EAControlledCreatureDeactivateZTarget, 006292a0 EACreatureCheat, 00629da0 EACloseLiveGUI, 00629f30
  EAClickPastText, 0062c6e0 EAOpenPCMSNChatMenu, 0062d380 EAControlledCreatureJump. Shapes: the player in a NAMED local
  (else the mode push precedes GetPlayer and the tail jump is lost); the jump action as a TEMPORARY bound to the const
  reference (a named local pushes its own `lea`).

### CPlayer mode handlers (6, probable: Ego_r.pdb / FableWin.pdb handler order + mirror bodies)
- 00633530 InitModeFirstPerson (mirror UninitModeFirstPerson 0x634410), 00634510 UninitModeConsole (sibling InitModeConsole
  0x6344C0; CConsole::SetActive(bool) and CTBaseSingleton<CConsole>::Get inline), 00634560 UninitModeViewHero (undoes the
  landed InitModeViewHero 0x636E90), 00634a30 InitModeSpecialAbilityFreezeControls (byte-twin of 0x6349B0 =
  InitModeFreezeControls by donor order), 00636270 InitModeNavigateInGameMenu, 00636b00 UninitModeTakePhotojournalScreenshot
  (mirror InitModeTakePhotojournalScreenshot 0x636A80). All six matched on the first grade with the landed screen-mode model
  (InitModeNavigateInventoryExperienceScreen 0x635080: donor include of engine/CPlayer.h, STLport bitset reference proxy
  GetTC, CIntelligentPointer::GetPItem).

### World map / minimap / geometry
- 0063dd70 `CWorldMap::IsThemeDestroyableAt(const C3DVector&) const` (proven: Ego_r.pdb, reads CEngineThemeDef
  DestructionThemeDef retail +0x98): the landed DestroyThemeAt 0x63EB20 model (same-TU GetDef stand-in; def homed in the
  dead `pos` slot).
- 006427f0 `CMinimapData::SetMinimapExtents`, 00642820 `CMinimapData::LoadMinimapTGAFile(const CWideString&)` (probable),
  00642a30 `CMinimapData()`: retail CMinimapData is 0x3C bytes (STLport vectors), CSurface at +0x14 (vtable 0x122F84C);
  the constructor zeroes each member in declaration order; CPixelFormat() leaves FormatIndex = -1 in the `push ecx` slot.
- 00643890 `vector<CMinimapData::MinimapTheme>::resize(size_type, const MinimapTheme&)` (element type proven by caller
  ResizeMinimapThemeList 0x643070): STLport resize; erase inlines `__copy_ptrs(..., __false_type)` = the counted `__copy`
  loop (an enum is not trivially assignable in STLport's traits); insert stays the call to _M_fill_insert 0x6436C0.
- 00643970 `C2DBoxF::GetTRPos() const` (probable: FableWin.pdb order SetTLPos, SetWidth, SetHeight, GetTRPos).
- 00661fc0 `CThingCreatureBase::IsIndestructible() const` (proven: slot 67 in all three creature vtables; Ego_r.pdb
  IsIndestructible / IsWillable adjacent; CThingCreatureDef flag byte bit 7). Declared non-virtual on the donor-include
  class because engine/CThingCreatureBase.h carries the vptr as data (the PDB symbol is `UBE`, ours `QBE`; same bytes).
- 00666ea0 `CThingCreatureBase::IsThingInLOSAndFOV(const CThing&) const` (probable): the matching form is three guard
  returns `if (dead) return false; if (!FOV) return false; if (!LOS) return false; return true;` (see LESSONS.md).
- 0066c6d0 `CRegionMinimap::GetRegionMapExtents(C2DCoordI&, C2DCoordI&) const` (proven: second overload, after the landed
  0x66C6A0).
- 00680500 `CVectorMap<EReactionSpeechType, long, ...>::UpperBound(const EReactionSpeechType&) const` (proven instance:
  CSimVoice::Voices at +0x08): the step-past branch must be the `if` (`if (!comp(key, *mid)) {...} else count = half;`).

## Parked for Fable 5.1 (0)

## Skipped
None (no `Catch@` rows in this chunk).

## Label corrections
- 0x0057A59B catalog "CTCCoopSpirit::AddExperience" -> `CTCHeroStats::AddRewardMoney(long)`.
- 0x005DA530 catalog "CComponent::HasState" -> `CTCWeapon::IsProjectileWeaponClass(EWeaponClass)` (static `__fastcall`).
- 0x00661FC0 catalog "CThingPlayerCreature::vfunc_67" -> `CThingCreatureBase::IsIndestructible() const` (inherited slot 67);
  by the same evidence the LANDED 0x00661FD0 "CThingPlayerCreature::vfunc_75" is `CThingCreatureBase::IsWillable() const`.
- 0x0056E59B "CTCInventoryExperience::vfunc_46" and 0x00624D90 "CTCInventoryQuests::vfunc_33" are slot placeholders.
- 0x0055B040 bound as `InitialiseInputDelay@CClickable_Retail` (label "SetupRumbleAction_Strong") = `?GetDataFromDef@CClickable@NUISystem@@QAEXXZ`.
- 0x0055A890 bound as `RegisterDragging@CDraggable_Retail` (label "SetupRumbleAction_Weak") = `CDraggable::GetDataFromDef`.
- 0x0054E5D0 bound as `InitialiseFromDef@CViewport` = `CViewport::GetDataFromDef`.
- 0x0052CCA0 carries a stale `??0CDraggableInto` binding; it is `CChangingStateComponent(const CChangingStateComponent&)`.
- 0x006804C0 LANDED as "CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound" = `CVectorMap<EReactionSpeechType, long,
  ...>::LowerBound const` (all six callers are CSimVoice members on the +0x08 map; the TC map's are 0x40F020 / 0x4365B0).
- 0x00487DC0 / 0x00487DD0 (identical `add ecx,2Ch ; jmp GetPItem` bodies): Ego_r.pdb orders GetPControlledCreature (QBE,
  CThingPlayerCreature*) then PeekPControlledCreature (QBE, const*) adjacently, so 0x487DC0 = Get and 0x487DD0 = Peek. This
  chunk binds GetPControlledCreature -> 0x487DC0; the LANDED 0x00896EE0 binds GetPControlledCreature -> 0x487DD0 (vland
  RELOC_CONFLICT warning) and should become PeekPControlledCreature.
- 0x005BC58B labelled "_global::Nop_ret0_005bc58b" is an empty CTCInventoryBase member (descriptive EmptyHook_005bc58b).
- 0x0066E73D labelled "_global::SetMemberConst_0066e73d" is `NHeroInformationScreens::CManager::Close()`.
- 0x004830A1: the label note "CTCInGameMenu::OpenInGameMenuPC" takes two pointers in the PDB; retail passes none (plain
  `ret`, sole caller 0x6362B2) -> `CTCInGameMenu::OpenInGameMenu()`.
- 0x00698F50 = `CGameCameraManager::PushSpecialCameraMode` (Ego_r.pdb order after PushZTargetMode 0x698E40).
- Neighbour names derived on the way (not landed here): 0x006334F0 / 0x00633510 one-process Uninit handlers (ClickPastText
  / CutScene), 0x00633590 / 0x006335A0 UninitModeFreezeControls / UninitModeSpecialAbilityFreezeControls, 0x00634410
  UninitModeFirstPerson, 0x006344C0 InitModeConsole, 0x006349B0 InitModeFreezeControls, 0x006362D0
  UninitModeNavigateInGameMenu, 0x00636A80 InitModeTakePhotojournalScreenshot, 0x00643070 CMinimapData::ResizeMinimapThemeList,
  0x006438E0 vector<MinimapTheme>::resize(n), 0x00643930 / 0x00643950 / 0x00643960 C2DBoxF SetTLPos / SetWidth / SetHeight,
  0x006807F0 CSimVoice::GetNoOfSampleIndexVariations, 0x0063E6F0 CWorldMap::IsLineOfSight(const CThing&, const CThing&, bool);
  CWorld EA neighbours 0x6291F0 UnfreezeControls, 0x629210 ApplyAllScriptedMapBrushes, 0x629220 StartThrowObject, 0x629260
  MoveHeroToRegion, 0x629270 UpdateFrame, 0x629280 FirstPersonViewStart, 0x629D60 CloseInGameMenu, 0x62C640 Scoreboard,
  0x62C690 Skills, 0x62C700 Inventory, 0x62CF90 CreatureBlock, 0x62D190 UseProjectileWeaponThirdPerson, 0x62D3D0 Roll.

## Header debt
- engine/CHoverable.h, engine/CTCInventoryExperience.h, engine/CPlayerGui.h (0x629F30 partial view) are flattened layout
  structs; CViewport, CScrollableList, CClickable, CDraggable, CMinimapData, CThingCreatureDef, CThing have no header.
- engine/CWorldMap.h uses the PDB's 16-byte containers and carries the vptr as data: 0063dd70 keeps the landed
  DestroyThemeAt local polymorphic model (vland SHARED_HEADER_AVAILABLE CWorldMap).
- engine/CThingCreatureBase.h carries the vptr as data, so slot-67 IsIndestructible is declared non-virtual (`QBE`).
- engine/CSurface.h flattens the vptr; CMinimapData rows use a local polymorphic CSurface.
