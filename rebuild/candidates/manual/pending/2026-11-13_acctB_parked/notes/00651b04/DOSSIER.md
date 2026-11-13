# DOSSIER 0x00651B04 -- NPlayerGui::CDrawAbilityButtons::Draw (5126 bytes)

Scout: identity scout, 2026-11-12. Read-only on the repo. Helper output under $W/scout/ (calls.txt = direct-call map with
reloc bindings, callee_names.txt = landed Leaf/Name lines of every callee).

## 1. Identity (manifest label is RIGHT)
- `?Draw@CDrawAbilityButtons@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z`
  = `virtual void NPlayerGui::CDrawAbilityButtons::Draw(const CEnginePrimitivePrepareInfo& info)` (public virtual, thiscall, `ret 4`).
- Source 1: Ego_r.pdb (ghidra_out/egor_pdb_names.tsv: `00694463 ?Draw@CDrawAbilityButtons@NPlayerGui@@UAEX...`; Ego_r addresses are
  not retail ones). Source 2: rebuild/manifest/rtti-vtables.tsv: `NPlayerGui::CDrawAbilityButtons 0125a6c8 slot 2 = 00651b04`; the
  derived `CDrawAbilityButtonsPC` vtable 0x0125A7D8 slot 2 = 0x006571F0 = `jmp 0x00651B04` (xref.py: the only code ref is that jmp,
  plus the vtable word 0x0125A6D0), i.e. CDrawAbilityButtonsPC::Draw (FableWin `?Draw@CDrawAbilityButtonsPC@...UAEX...`) forwards to
  this base Draw. Leaf to use: `?Draw@CDrawAbilityButtons@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z`.
- CDrawAbilityButtons vtable 0x0125A6C8 (rtti-vtables.tsv): slot0 0x00644FFC ClearPrimitivesIfNecessary, slot2 Draw (this),
  slot3 0x00643FC8 GetAssignableSpecialMoveIndexForButtonDef (called here via `call [edx+0Ch]`), slot4 0x0064DB2B DrawAbilityList
  (`call [eax+10h]`), slot5 0x0064D593 / slot6 0x0064D5E1 GetAbilityButtonMapping / GetAbilityPositionMapping, slot7 0x006571B4
  DrawExpressionList (`call [eax+1Ch]`), slot8 0x006571B7 (DrawExpressionButton). Slot 1 is not listed (ClearPrimitives, CDrawBase).

## 2. Retail shape (xdis 00651b04 0x1416, $W/dis_all.txt)
- Prologue `push ebp ; lea ebp,[esp-74h] ; sub esp,0E0h ; push edi` -- the BIASED ebp frame. Landed sibling
  0x0064499C CDrawPerceivers::Draw (same `lea ebp,[esp-0x74]`) proves: `#pragma optimize("s", on)` ALONE gives this frame; adding
  `optimize("y", off)` gives `mov ebp,esp` (its Shape note). So: **`#pragma optimize("s", on)` only, no "y" off** (unlike DrawButton /
  DrawEffects / DrawAbilityList which are `mov ebp,esp` frames with y-off).
- `this` (ecx) -> edi, spilled to [ebp+70h] at once; edi is then reused as the constant 0 for the whole function.
- Early-out shrink-wrap: `push ebx ; push esi` only after the IS_THING_ALIVE test (0x00651B37); the failing branch jumps to
  0x00652F02 `pop edi` (skipping pop esi/ebx). Epilogue `pop esi ; pop ebx ; pop edi ; add ebp,74h ; leave ; ret 4`.
- No `fs:0` anywhere: no EH frame, though CWideString / CInputEvent / CDefPointer locals with destructors exist -> **Flags: none**
  (no /EHsc), as the sibling DrawEffects / Melee Draw rows say "Flags: none". No /GS cookie.
- Size idioms: `push 5Ch ; cdq ; pop ecx ; idiv ecx` for vector<CAbilityButtonGuiDef>::size(); `push 6Fh ; pop ebx` constant;
  ebx = 255 (0xFF) for most of the function, edi = 0.
- Constants: 0.6f 0x0123816C (tooltip scale + text-width factor), 0.5f 0x0122F59C, 1/3 0x0125A4DC, 255.0f 0x01230014, 5.0f 0x012462A8,
  1.0f 0x0122DED8 (fcomp), `fld1`, `fldz`. String 0x0125A6EC = "OBJECT_HERO_STICK".
- Globals: 0x013B8388 = `PInputManager` (labels_naming_wave13: ?PInputManager@@3PAVCInputManager@@A). 0x01375744 = an
  UNNAMED global `unsigned char` initialised 0xFF in .data, read `mov al,[01375744h]` 4x here as ScaleAlpha's argument for the ABXY
  ring sprites; also read at 0x0065316D/0x006531D2/0x006532B8/0x0065332D (CDrawQuickAccessIcons code after 0x00652F0A) and
  0x0065767B/0x00657784/0x00657878 (CDrawAbilityButtonsPC::DrawAbilityList 0x00657220). No name found in the PDB name lists
  (they hold functions only) -> descriptive name needed (e.g. a GUI-wide ring alpha byte), declared `extern unsigned char` (it is
  written somewhere or it would fold to an immediate).

## 3. Callees (landed file / landed name / PDB name)
All under rebuild/src/compiled/00/<bb>/. "OK" = landed spelling matches PDB.
| addr | n | landed row | name to use (PDB) | note |
|---|---|---|---|---|
| 0x00449970 | 3 | CPlayerManager_GetMainPlayer_00449970 | CPlayerManager::GetMainPlayer() (non-const, returns CPlayer&) | OK |
| 0x00487DD0 | 1 | CPlayer_PeekPControlledCreature_00487dd0 | `const CThingPlayerCreature* CPlayer::PeekPControlledCreature() const` | OK |
| 0x0043DBF0 | 1 | global_ISTHINGALIVECThingBuilding_0043dbf0 | `IS_THING_ALIVE<const CThingPlayerCreature*>(const CThingPlayerCreature* const&)` __fastcall | file name says CThingBuilding: name only |
| 0x0040DCE0 | 2 | global_ISTHINGALIVECThingBuilding_0040dce0 | `IS_THING_ALIVE<CThing*>(CThing* const&)` __fastcall | operand is a stack local (`lea ecx,[ebp+58h]`) |
| 0x00438570 | 1 | CPlayerGui_PeekGuiDef_00438570 | static __fastcall `CPlayerGui::PeekGuiDef()` | OK (FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF) |
| 0x00440D70 | 2 | stdvector_erase_00440d70 | `vector<CEnginePrimitiveHandle>::erase(iterator,iterator)` (STLport, out of line) | `push end ; push begin` |
| 0x00443F90 | 2 | stdvector_Resize_00443f90 | `vector<CEnginePrimitiveHandle>::resize(size_type)` `ret 4` (row text calls it CEnginePrimitiveHandleVector::Resize) | single-arg resize with the default value inside |
| 0x00410DE0 | 12 | STLbitset_test_00410de0 | `std::bitset<274>::test(size_t) const` | out of line; CThing::UsingComponents (+0x20) |
| 0x004365B0 | 10 | CVectorMap_LowerBound_004365b0 | `CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound(const ETCInterfaceType&) const` | const one => PeekTC (const) |
| 0x00643D0C | 11 | NPlayerGui_GFAnchorToBottomRight_00643d0c | `C2DVector __fastcall NPlayerGui::GFAnchorToBottomRight(const C2DVector&)` (hidden result in ecx, arg in edx) | OK; same retail file, defined earlier |
| 0x00643D5D | 13 | NPlayerGuiCDrawBase_ScaleAlpha_00643d5d | `unsigned char CDrawBase::ScaleAlpha(unsigned char) const` (`ret 4`) | OK; NAME_FIXES note in DrawButton: some rows still call it GetFadedAlpha |
| 0x00493AC2 | 8 | CEnginePrimitive2DSprite_CEnginePrimitive2DSprite_00493ac2 | 16-arg ctor `??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z` | OK |
| vtbl+0x5C | 8 | CIEngine::SetPersistentPrimitive (slot 23) on info.Engine (+0x34) | `(CEnginePrimitiveHandle*, sprite&, 0xC0)` | engine/CIEngine.h, FABLE_RETAIL_CIENGINE |
| 0x00625930 | 1 | GetCanItemBeQuickAssigned_..._00625930 (Leaf `?M@C_00625930@@QAEEXZ` placeholder) | **CTCQuickAccessMenu::IsActive() const** `?IsActive@CTCQuickAccessMenu@@QBE_NXZ` | NAME_FIX: body `mov al,[ecx+0Ch] ; ret` = Ego_r CTCQuickAccessMenu::Active +0x0C; Ego_r 0x0066A150 + FableWin both have IsActive |
| 0x00A01B50 | 2 | CBaseIntelligentPointer_GetPItem_00a01b50 | `CBaseObject* CBaseIntelligentPointer::GetPItem() const` | OK |
| 0x006A1440 | 2 | CTCCarrying_GetThingInPrimarySlot_006a1440 | `CThing* CTCCarrying::GetThingInPrimarySlot() const` | OK (engine/CTCCarrying.h:17) |
| 0x005DA7D0 | 3 | CTCWeapon_GetWeaponType_005da7d0 | `EWeaponType CTCWeapon::GetWeaponType() const` (manifest label GetCategoryIdentifier is wrong) | called 3x, not CSE'd |
| 0x004C7CC0 | 1 | CThing_GetDefName_004c7cc0 | `CDefString CThing::GetDefName() const` (hidden result [ebp+50h]) | temp CDefString has no dtor call |
| 0x005B3440 | 1 | CDefString_operator_005b3440 | `bool CDefString::operator==(const char*) const` | OK |
| 0x005D92D0 | 2 | CTCInventoryItem_GetEngineGraphic_005d92d0 (Leaf is a bogus free-function `?CTCInventoryItem_GetEngineGraphic@@YI...`) | `const CEngineGraphic& CTCInventoryItem::GetEngineGraphic() const` `?GetEngineGraphic@CTCInventoryItem@@QBEABVCEngineGraphic@@XZ` | NAME_FIX for the Leaf line; header engine/CTCInventoryItem.h:86 (FABLE_RETAIL_CTCINVENTORYITEM_ENGINEGRAPHIC); row reads BankIndex +0 |
| 0x0040D2A0 | 9 | CUserProfileManager_Get_0040d2a0 | static __fastcall `CUserProfileManager& CUserProfileManager::Get()` | OK |
| 0x00408C90 | 6 | CUserProfileManager_GetAssignedInputForAction_00408c90 | `bool GetAssignedInputForAction(EGameAction, bool passive, CActionInputControl*)` `ret 0Ch` | actions 0x0D, 0x0E, 0x06 |
| 0x0099AED0 / 0x0099B510 | 3/3 | CWideString ctor / dtor | `CWideString::CWideString()` / `~CWideString()` | OK |
| 0x00446720 | 3 | global_GetActionName_00446720 | `bool __fastcall GetActionName(const CActionInputControl&, CWideString&, bool)` (ecx=&control, edx=&name, push 0) | check row for exact PDB leaf |
| 0x009E21D0 | 3 | global_GFACoordToVCoord_009e21d0 | `void __fastcall GFACoordToVCoord(const C2DVector&, C2DVector&)` | OK |
| 0x00437210 | 3 | CFontBank_GetVStringExtents_00437210 | `C2DExtentsF CFontBank::GetVStringExtents(const CWideString&) const` | hidden result [ebp+14h]/[ebp-10h]; first float read |
| 0x00438010 | 3 | CPlayerGui_Draw2DText_00438010 | `void CPlayerGui::Draw2DText(const C2DVector&,const CWideString&,const CFontBank&,const CRGBColour&,float,EEngine2DLayer,bool)` | FABLE_RETAIL_CPLAYERGUI_DRAWTEXT |
| 0x0064B1AC | 1 | **UNLANDED** | `void CDrawAbilityButtons::UpdateWeaponAbilities()` (Ego_r/FableWin `?UpdateWeaponAbilities@CDrawAbilityButtons@NPlayerGui@@IAEXXZ`, protected) | declare; earlier retail address = earlier in TU |
| 0x00439AB0 | 2 | CPlayerGui_GetButtonAlpha_00439ab0 | `float CPlayerGui::GetButtonAlpha(long, bool, bool, bool) const` `?GetButtonAlpha@CPlayerGui@@QBEMJ_N00@Z` | `ret 10h`, float in st0 |
| 0x00A04410 / 0x00A03B30 | 2/2 | CInputEvent_CInputEvent_00a04410 / E2_E2_00a03b30 (Leaf `?M@C00a03b30@@QAEXXZ` placeholder) | `CInputEvent::CInputEvent()` / **`CInputEvent::~CInputEvent()`** (Ego_r ??1CInputEvent@@QAE@XZ) | 0x00A03B30 is a bare `ret`, called out of line: NAME_FIX |
| 0x00487510 | 2 | CPlayer_GetJoystickDeviceNumber_00487510 | `long CPlayer::GetJoystickDeviceNumber() const` | OK |
| 0x009F4C60 | 2 | CInputManager_IsJoystickEventInQueue_009f4c60 | `bool CInputManager::IsJoystickEventInQueue(EInputEventType, long button, CInputEvent*, long device)` | on PInputManager; type 0x14 |
| 0x00438990 | 1 | CPlayerGui_GetAbilityButton_00438990 | `long CPlayerGui::GetAbilityButton(long)` `?GetAbilityButton@CPlayerGui@@QAEJJ@Z` (non-const) | OK |
| 0x0085F560 | 1 | NCreatureMode_IsAvailableForHeroRun_0085f560 | `bool __fastcall NCreatureMode::IsAvailableForHeroRun(const CThing&)` | ecx = creature |
| 0x00645099 | 4 | NPlayerGuiCDrawAbilityButtons_DrawButton_00645099 | `void DrawButton(const CEnginePrimitivePrepareInfo&, const C2DVector&, float, long, long, bool, EEngine2DLayer)` protected | OK, `ret 1Ch` |
| 0x005EE31D | 1 | CTCInventoryAbilities_GetSpecialAbilityGraphicSetToButton_005ee31d | `long ...GetSpecialAbilityGraphicSetToButton(long) const` | OK |
| 0x005EE365 | 1 | CTCInventoryAbilities_GetSpecialAbilityPDefSetToButton_005ee365 | `CDefPointer<const CHeroAbilityDef> ...GetSpecialAbilityPDefSetToButton(long) const` (hidden result [ebp+64h]) | OK |
| 0x005EDF08 | 1 | CTCInventoryAbilities_IsAbilityAvailable_005edf08 | `bool CTCInventoryAbilities::IsAbilityAvailable(EHeroAbility) const` | OK |
| 0x005EE49A | 1 | CTCInventoryAbilities_IsAbleToUseAbility_005ee49a | `bool ...IsAbleToUseAbility(EHeroAbility, bool) const` | OK |
| 0x0072C430 | 1 | **UNLANDED** (manifest label "CTCSpecialAbilities::GetAbilityAvailability") | **`bool CTCSpecialAbilities::IsAbilityAvailable(EHeroAbility) const`** (FableWin `?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EH...`; body `ret 4`, bool) | manifest label wrong |
| 0x006451DD | 1 | NPlayerGuiCDrawAbilityButtons_DrawEffects_006451dd | `void DrawEffects(long, const C2DVector&, const CHeroAbilityDef&)` protected | OK |
| 0x005EE146 / 0x005EE183 / 0x005EE24B | 1 each | CTCInventoryAbilities_GetNumAssigned{ReceivedAbilities,AbilityRows,ReceivedExpressions} | `long ... const` (`?GetNumAssigned...@CTCInventoryAbilities@@QBEJXZ`) | OK |
| 0x006445BA | 1 | NPlayerGuiCDrawBase_ClearEffects_006445ba | static __fastcall `CDrawBase::ClearEffects(vector<CIntelligentPointer<CParticleEmitter>>&)` | ecx = this+0x48 (Effects) |
| 0x00BFEA70 | 1 | global_ftol2 | `__ftol2` (compiler helper of `(unsigned char)(255.0f*Alpha)`) | CPlayerGui::GetAlpha() inline |
| 0x004375E0 | 2 | **CMap_DrawGetSeaLevel_004375e0 -- WRONG NAME** | **`float CPlayerGui::GetAbilityButtonScale() const`** (Ego_r 0x0044A260 `?GetAbilityButtonScale@CPlayerGui@@QBEMXZ`) | body `fld [ecx+278h] ; fabs ; ret` = fabs(AbilityButtonScale +0x278); called with ecx = PlayerGui. NAME_FIX |
| vtbl+0x0C | 1 | slot 3 0x00643FC8 | `virtual long GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef&) const` | |
| vtbl+0x10 | 1 | slot 4 0x0064DB2B | `virtual void DrawAbilityList(const CEnginePrimitivePrepareInfo&, float)` | |
| vtbl+0x1C | 1 | slot 7 0x006571B4 | `virtual void DrawExpressionList(const CEnginePrimitivePrepareInfo&, float)` | |

Inline (no call): CThing::PeekTC<T> (bitset test + LowerBound + end-or-greater select), CEnginePrimitiveHandle::Clear (on
InteractBorderPrimitiveHandle), CUserProfileManager::IsHUDTooltipEnabled (FableWin `?IsHUDTooltipEnabled@CUserProfileManager@@QBE_NXZ`:
`UseNewProfileValues(+0x0C) ? HUDTooltipEnabledNewProfile(+0xD9) : HUDTooltipEnabled(+0x19)`), CPlayerGui::GetAlpha (`255*Alpha`
via __ftol2), CPlayerGui::SetAbilityButtonScale (FableWin `?SetAbilityButtonScale@CPlayerGui@@QAEXM@Z`, store +0x278),
CWorld::IsPaused (+0xA4), CMainGameComponent::PeekCutSceneMainFont (+0x1614C object word), CTCHero WeaponsUsable (+0x0C),
CDefPointer<const CHeroAbilityDef> copy/release (count +4, vslot 1 delete), CActionInputControl ctor (5 zero dwords + 2 zero floats).

## 4. Members touched
CDrawAbilityButtons (no shared header; Ego_r layout in struct_layouts_egor.tsv minus 4/8; the landed ctor 0x00651A7E and
ClearPrimitivesIfNecessary 0x00644FFC model it locally, the DrawButton row too):
- CDrawBase (engine/NPlayerGui_CDrawBase.h): PlayerGui +0x04, PlayerManager +0x08, World +0x0C, Component +0x10, Drawing +0x18
  (SetAsDrawing: FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING), PrimitiveHandles +0x1C (vector<CEnginePrimitiveHandle> with
  FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES; elements used: [0] +0, [1] +8, [6] +0x30, [7] +0x38, [8] +0x40, [9] +0x48), ScaleAlpha
  (HANDLES opt-in), ClearEffects (FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS).
- Columns +0x28 (unused), BackgroundPrimitiveHandle +0x2C (last sprite), InteractBorderPrimitiveHandle +0x34 (Clear, then sprite),
  SpecialPrimitiveHandles +0x3C (vector<CEnginePrimitiveHandle>: erase, resize, size() as `(end-begin)>>3`), Effects +0x48
  (vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>).
- Model CDrawAbilityButtons locally as DrawButton 0x00645099 does (`class CDrawAbilityButtons : public CDrawBase` with the four
  members), adding the virtuals in slot order so the slot-3/4/7 calls land at +0x0C/+0x10/+0x1C: slot0 ClearPrimitivesIfNecessary,
  slot1 ClearPrimitives (CDrawBase declares ClearPrimitives, ClearPrimitivesIfNecessary -- ORDER CONFLICT: retail slot0 is
  ClearPrimitivesIfNecessary 0x00644FFC; the header says "slot order unproven". Check the vtable 0x0125A6C8 words 0/1 before relying on
  the shared header's order), slot2 Draw, slot3 GetAssignableSpecialMoveIndexForButtonDef, slot4 DrawAbilityList, slot5/6 mappings,
  slot7 DrawExpressionList, slot8 DrawExpressionButton.
CPlayerGui (engine/CPlayerGui.h, FABLE_RETAIL_CPLAYERGUI + _MEMBERS): ShowSpecialAbilityButtons +0x250, ShowSpecialAbilityButtonsAbilities
  +0x251, AbilityButtonScale +0x278 (float), NormalToSpecial +0x27C, SpecialToNormal +0x27D, Alpha +0x2A0 (GetAlpha needs
  _QUEST_DRAW branch, which conflicts with _MEMBERS: write `(unsigned char)(255.0f*Alpha)` or add a MEMBERS-compatible GetAlpha opt-in).
  Missing opt-ins to add: GetButtonAlpha (0x00439AB0), GetAbilityButton (0x00438990), GetAbilityButtonScale (0x004375E0, out of
  line), SetAbilityButtonScale (inline). IsShowSpecialAbilityButtons exists (FABLE_RETAIL_CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS).
CPlayerGuiDef (engine/CPlayerGuiDef.h; retail offsets): ABXYRingsGraphic 0x210, ABXYRingsPos 0x214, ABXYRingsTopGraphic 0x220,
  ABXYRingsTopOffset 0x224, ABXYMiddleOffset 0x230, ABXYRingsFavouriteGraphic 0x248, ABXYRingsTopFavouriteGraphic 0x24C,
  WhiteButtonGraphic 0x250, WhiteButtonPos 0x254, WhiteButtonIconOffset 0x25C, WhiteButtonTooltipOffset 0x264, BlackButtonGraphic 0x26C,
  BlackButtonPos 0x270, BlackButtonIconOffset 0x278, BlackButtonTooltipOffset 0x280, AbilityButtonSheathe{Axe 0x774, Bow 0x778,
  Mace 0x77C, Stick 0x780, Sword 0x784}Graphic, ABXYButtonMiddleGraphic 0x79C, ABXYAbilityUncastableGraphic 0x7B4,
  ABXYAbilitySwitchGraphic 0x7B8, ABXYAbilityShieldOffGraphic 0x7BC, ABXYRunGraphic 0x7C0 (compared with GetAbilityButton(i)),
  InteractButtonBorderGraphic 0x7E8, InteractButtonBorderOffset 0x7EC, InteractButtonTooltipOffset 0x7F4, AbilityButtons 0x7FC
  (FABLE_RETAIL_CPLAYERGUIDEF_ABILITYBUTTONS gives the std::vector<CAbilityButtonGuiDef> + the 0x5C CAbilityButtonGuiDef:
  ControllerType +0x3C, ControllerButton +0x40, ButtonGraphic +0x48, Position +0x4C, Offset +0x54), SpecialAbilityButtons 0x808
  (opaque `unsigned char[0xc]` today -- needs typing as std::vector<CAbilityButtonGuiDef>; NEW opt-in). The C2DVector members
  above are split `_X/_Y` floats in the default header: retail passes `&def.ABXYRingsPos` (lea edx,[esi+214h]) and
  `&def.WhiteButtonPos`, `&def.BlackButtonPos` whole to GFAnchorToBottomRight and adds the offsets as vectors -> a NEW opt-in typing
  them C2DVector (pattern: FABLE_RETAIL_CPLAYERGUIDEF_FLOURISH / _TARGETING). Also the CAbilityButtonGuiDef Position/Offset as C2DVector
  (DrawEffects takes `const C2DVector&` = &b.Position, `lea eax,[ecx+eax+4Ch]`).
CThing / CThingPlayerCreature: UsingComponents +0x20 (bitset<274>), Components +0x44 (CVectorMap) -> engine/CThing.h with
  FABLE_RETAIL_CTHING_GETTC_EXPANDED (PeekTC(id,&p) body = exactly the retail expansion incl. the key copy `mov [ebp+64h],id` after
  the test and the map pointer local) + CVectorMap.h (LowerBound out of line).
TC classes via PeekTC (ETCInterfaceType.h): 0x6F TCI_HERO_ABILITIES -> CTCInventoryAbilities (its StatGetInterfaceType = 111),
  0xF6 TCI_QUICK_ACCESS_MENU -> CTCQuickAccessMenu, 0x13 TCI_INVENTORY_WEAPONS -> CTCInventoryWeapons (PActiveMeleeWeapon +0x134,
  PActiveRangedWeapon +0x141, header has both), 0x46 TCI_CARRYING -> CTCCarrying, 0x29 TCI_HERO -> CTCHero (WeaponsUsable +0x0C),
  0x1A TCI_INVENTORY_ITEM -> CTCInventoryItem, 0x48 TCI_WEAPON -> CTCWeapon, 0xDF TCI_HERO_SPECIAL_MOVEMENT -> CTCHeroSpecialMovement
  (pointer only tested), 0x93 TCI_PHYSICAL_SHIELD_SPELL (HasTC only, bitset test result used directly), 0xA7 TCI_SPECIAL_ABILITIES
  -> CTCSpecialAbilities.
CHeroAbilityDef +0x3C = HeroAbility (EHeroAbility; landed DrawEffects 0x006451DD models CHeroAbilityDef "PDB offsets - 4").
CUserProfileManager (retail): UseNewProfileValues +0x0C, HUDTooltipEnabled +0x19, HUDTooltipEnabledNewProfile +0xD9 (PDB 0xDD - 4).
CWorld Paused +0xA4 (IsPaused, FABLE_RETAIL_CWORLD_UPDATEINPUT). CMainGameComponent PCutSceneMainFont +0x1614C
  (PeekCutSceneMainFont, FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK). CEnginePrimitivePrepareInfo: Interpolate +0 (first float of
  CInterpolationInfo.Current read: `fld [info]`), Engine +0x34 (FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO).

## 5. Callee visibility (Q32 lesson)
- Same TU, defined earlier (retail addresses below 0x00651B04 in the NPlayerGui HUD file): GFAnchorToBottomRight 0x00643D0C,
  ScaleAlpha 0x00643D5D, DrawButton 0x00645099, DrawEffects 0x006451DD, ClearEffects 0x006445BA, UpdateWeaponAbilities 0x0064B1AC,
  CDrawAbilityButtons ctor 0x00651A7E (immediately before Draw). The Melee Draw row (0x00646580) found that giving
  GFAnchorToBottomRight its BODY under `#pragma auto_inline(off)` changes frame-slot reuse (callee known not to keep the pointer);
  copy that row's include of the GFAnchor body if slot residues appear.
- Unaligned out-of-line small inlines retail called: CPlayerGui::GetAbilityButtonScale 0x004375E0, CTCQuickAccessMenu::IsActive
  0x00625930, ~CInputEvent 0x00A03B30 (empty, still called -> declare it out of line, no body), IS_THING_ALIVE<> x2,
  bitset::test 0x00410DE0, CVectorMap::LowerBound 0x004365B0, vector erase 0x00440D70 / resize 0x00443F90 (STLport out of line).
- Inline expanded here: PeekTC, HUDTooltip getter, Handle Clear, CDefPointer copy/release, GetAlpha.

## 6. Sibling landed rows (copy their idioms)
- 0x00645099 DrawButton (same class): `#pragma optimize("y", off)` + `("s", on)`; defines DEF_CLASS_BASE, C2DVECTOR, CRIGHTHANDEDSET(+_CTOR,
  _RIGHT_COMPONENTS), CINTELLIGENTPOINTER(+_CTOR_OUTOFLINE, _ASSIGN_OUTOFLINE), CENGINEPERSISTENTPRIMITIVEBASE, CIVCOUNTEDPOINTER,
  CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS, CENGINEPRIMITIVEPREPAREINFO, CIENGINE, SCOPED_PTR, CRESOURCELIST, CDISPLAYMANAGER,
  NHEROINFORMATIONSCREENS_CMANAGER, CPLAYERGUIDEF_FLOURISH, CPLAYERGUI, CPLAYERGUI_MEMBERS, CPLAYERGUI_PEEKGUIDEF,
  CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS, CPLAYERGUI_QUEST_DRAW, NPLAYERGUI_CDRAWBASE_HANDLES, NPLAYERGUI_CDRAWBASE_SETASDRAWING;
  sprite expression + SetPersistentPrimitive idiom, `C2DVector(position)` by-reference copy.
- 0x00646580 CDrawMeleeActionButtonEffects::Draw: Flags none, y-off + s-on; GFAnchor body visible note; anchored-sum kept in x87 and
  copied to the sprite position temp (`fld st(1) ; fstp ; fst ; fstp st(0) ; fstp st(0)` -- same pattern here at 0x00652763,
  0x00652E36).
- 0x0064499C CDrawPerceivers::Draw: SAME biased frame `lea ebp,[esp-74h]`, `ret 4`; `#pragma optimize("s", on)` only;
  Draw2DText position as explicit C2DVector temporary, GFACoordToVCoord, CWideString dtor.
- 0x00651A7E CDrawAbilityButtons ctor: `#pragma optimize("s", on)` only; local class model; def vector size `(last-first)/0x5c`.
- 0x0064DB2B DrawAbilityList / 0x006451DD DrawEffects: y-off + s-on, Flags none.
- Family (HINTS): /opt/fable_lane/w22/agent4007 (matched 12 KB UI builder, hdr overlay with CAction/CCountedPointer/CList/CRGBColour
  opt-ins, tools cd.sh/bt.sh/gentest.py).

## 7. Open points for the author
- CDefPointer<const CHeroAbilityDef> local [ebp+58h]: zeroed before the `abilities != 0` test, then filled from the by-value
  temporary with addref ONLY (no compare, no old release), temporary released after. A plain `abilityDef = tmp` assignment with a
  generic operator= would release the old value; try the header's CDefPointer assignment opt-ins, else a declaration-with-init inside
  the if, and check the frame slot.
- GetButtonAlpha's 3rd argument in the special loop is spilled through a stack byte ([ebp+44h] `mov [ebp+44h],al ; push [ebp+44h]`)
  while the normal loop passes `sete al ; push eax` (the `!Show` expression). The byte spill suggests a named `bool` local or the
  inline IsShowSpecialAbilityButtons() result.
- Identical sprite tails are cross-jumped (0x00651D70, 0x00651E72): write each arm fully; the compiler merges them.
