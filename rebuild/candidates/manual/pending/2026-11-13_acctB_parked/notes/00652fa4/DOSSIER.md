# DOSSIER 0x00652FA4 NPlayerGui::CDrawQuickAccessIcons::Draw (4653 bytes)

## 1. Retail shape (xdis 00652fa4 0x123d; $W/dis_all.txt)
- thiscall, `ret 4` (0x006541CE): one arg = `const CEnginePrimitivePrepareInfo& info` at [ebp+7Ch].
- Prologue `push ebp ; lea ebp,[esp-74h] ; sub esp,12Ch` (biased-ebp frame pointer = `#pragma optimize("y",off)`), then
  `push ebx ; push esi ; push edi` (interleaved with the first fldz stores). Epilogue `pop edi/esi/ebx ; add ebp,74h ; leave ; ret 4`.
- /GS: `mov eax,[0139C8A8h]` (__security_cookie, no xor: VC7.1) -> [ebp+70h]; tail `mov ecx,[ebp+70h] ; call 0x00BFE9F9`
  (__security_check_cookie). The early return (hero dead) jumps to the cookie check 0x006541BF.
- NO SEH frame (no fs:0) although CCharString / CWideString / CDefPointer / CIntelligentPointer temporaries are destroyed: build
  without /EHsc -> `// Flags: /GS` only (same as sibling Update 0x0064D6A8).
- Size idioms: `push 50h ; pop ecx` / `push 0Ah ; pop ecx`, `and [x],0` -> `optimize("s",on)`. Every landed sibling uses
  `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.

## 2. Name / signature (two sources)
- RTTI vtable 0x0125A704 (.?AVCDrawQuickAccessIcons@NPlayerGui@@) slot 2 = 0x00652FA4 (rebuild/manifest/rtti-vtables.tsv);
  slot 0 = ClearPrimitivesIfNecessary 0x0064D62F (landed).
- BSim port (ghidra_out/bsim_port_audit_final.tsv, sim 0.825, sig 960):
  `?Draw@CDrawQuickAccessIcons@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z`; Ego_r.pdb (egor_pdb_names.tsv 0x00695982) and
  egor_signatures carry the same mangled name. Caller: CDrawQuickAccessIconsPC::Draw (vtable 0x0125A804 slot 2 = 0x00657580) calls it
  at 0x0065758F (base-class call) -> virtual, public.
- Manifest label is RIGHT. Signature: `virtual void NPlayerGui::CDrawQuickAccessIcons::Draw(const CEnginePrimitivePrepareInfo& info)`.
  Mangled leaf: `?Draw@CDrawQuickAccessIcons@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z`.

## 3. Callees (landed file under rebuild/src/compiled/<aa>/<bb>/, landed name; PDB name from bsim/Ego_r)
| addr | landed file / name | notes |
|---|---|---|
| 0x0043A4D0 | CPlayerGui_SetGuildSealLevelUpPos_0043a4d0.cpp `CPlayerGui::SetGuildSealLevelUpPos(const C2DVector&)` | Ego_r ?SetGuildSealLevelUpPos@CPlayerGui@@QAEXABVC2DVector@@@Z; 3 calls |
| 0x00449970 | CPlayerManager_GetMainPlayer_00449970.cpp | engine/CPlayerManager.h; 3 calls |
| 0x00487DC0 | CPlayer_GetPControlledCreature_00487dc0.cpp | bsim label "_Cons_val" is wrong |
| 0x0043DBD0 | global_ISTHINGALIVECThingBuilding_0043dbd0.cpp, manifest `IS_THING_ALIVE<CThingPlayerCreature*>` | __fastcall, takes `const T&` (address of the local); Update 0x0064D6A8 models it `__declspec(noinline) bool __fastcall IS_THING_ALIVE(const T&)` |
| 0x00438570 | CPlayerGui_PeekGuiDef_00438570.cpp `static const CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef()` | PDB-confirmed |
| 0x00440D70 | stdvector_erase_00440d70.cpp `vector<CEnginePrimitiveHandle>::erase(first,last)` | ret 8; the 3 clear() calls |
| 0x00443F90 | stdvector_Resize_00443f90.cpp (landed as CEnginePrimitiveHandleVector::Resize) | ret 4; `resize(n)` of the handle vectors |
| 0x00410DE0 | STLbitset_test_00410de0.cpp `bitset<274>::test` | HasTC; opt-in FABLE_RETAIL_STLP_BITSET_TEST_DECL |
| 0x0040F020 | CVectorMap_LowerBound_0040f020.cpp (non-const LowerBound) | GetTC (TCI_INVENTORY 0x11) |
| 0x004365B0 | CVectorMap_LowerBound_004365b0.cpp (const LowerBound) | PeekTC (0x27 TCI_LIGHT, 0x6F TCI_HERO_ABILITIES, 0xA7 TCI_SPECIAL_ABILITIES) |
| 0x00581E6F | CTCInventory_UpdateContextSensitiveItemFades_00581e6f.cpp | PDB-confirmed |
| 0x00643CDA | NPlayerGui_GFAnchorToBottomLeft_00643cda.cpp `C2DVector __fastcall GFAnchorToBottomLeft(const C2DVector&)` | hidden result in ecx, arg in edx; 7 calls |
| 0x00643D5D | NPlayerGuiCDrawBase_ScaleAlpha_00643d5d.cpp `unsigned char CDrawBase::ScaleAlpha(unsigned char) const` | Ego_r ?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z (bsim "_Cons_val" wrong); 9 calls |
| 0x00493AC2 | CEnginePrimitive2DSprite_CEnginePrimitive2DSprite_00493ac2.cpp | ctor (u64 id, const C2DVector&, EEngine2DLayer, ulong, ulong, blend, interp, float, float, uchar a,r,g,b, ulong, const C2DVector&, const C2DVector&), ret 44h; 5 calls, object at [ebp-0B8h] (0x54 bytes, no dtor) |
| [info+34h]->vtbl[+5Ch] | CIEngine::SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) slot 23 | engine/CIEngine.h, engine/CEnginePrimitivePrepareInfo.h (Engine +0x34). Landed DrawIcon spells it `Renderer->AddPrimitive` (local model) -- PDB name is SetPersistentPrimitive |
| 0x00437820 | CPlayerGui_GetGuildSealAlpha_00437820.cpp `float CPlayerGui::GetGuildSealAlpha() const` | manifest/bsim "_Cons_val" WRONG; 3 calls |
| 0x00BFEA70 | global_ftol2_00bfea70.cpp (__ftol2) | 13 calls |
| 0x0099EBF0 / 0x0099EAE0 | CCharString(const char*, long) / ~CCharString | "OBJECT_GUILD_SEAL_1" @ 0x01251B4C, length -1 |
| 0x009AD410 | CDefinitionManager_GetDefGlobalIndexFromName_009ad410.cpp | on World->DefinitionManager ([World+10h]) |
| 0x005BDF08 | CTCInventoryBase_GetNumberOfItemsOfTypeInInventory_005bdf08.cpp | PDB `NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) const`; 2 calls |
| 0x00487510 | CPlayer_GetJoystickDeviceNumber_00487510.cpp | bsim name wrong |
| 0x004460C0 | CGamePlayerInterface_IsQuickAccessEventInQueue_004460c0.cpp | ?IsQuickAccessEventInQueue@CGamePlayerInterface@@QBE_NJW4EInputButtonEventType@@J@Z |
| 0x00439AB0 | CPlayerGui_GetButtonAlpha_00439ab0.cpp `float GetButtonAlpha(long, bool, bool, bool)` ret 10h | result DISCARDED (`fstp st(0)`) both times |
| 0x00581CFD | CTCInventory_GetContextSensitiveItemGraphicIndex_00581cfd.cpp | PDB ok |
| 0x00581FA4 | CTCInventory_GetQuickAccessItemGraphicIndex_00581fa4.cpp | (bsim gives the CS name - propagation; landed name is right) |
| 0x00581E33 | CTCInventory_GetQuickAccessItem_00581e33.cpp `long GetQuickAccessItem(long) const` | 2 calls |
| 0x0042B040 | CDefinitionManager_GetDefCThingObjectDef_0042b040.cpp `GetDef<CThingObjectDef>` | out CDefPointer |
| 0x0042AF3C | CDefinitionManager_GetDefT_0042af3c.cpp `GetDef<CThingBaseDef>` | |
| 0x005936C2 | CDefClassBase_GetSubDefCAbilityDef_005936c2.cpp `CDefClassBase::GetSubDef<CAbilityDef>` | |
| 0x005EDF08 | CTCInventoryAbilities_IsAbilityAvailable_005edf08.cpp | PDB ok |
| 0x005EE49A | CTCInventoryAbilities_IsAbleToUseAbility_005ee49a.cpp `(EHeroAbility, bool)` | PDB ok |
| 0x0072C430 | NOT LANDED; manifest `CTCSpecialAbilities::GetAbilityAvailability` (made-up) | Ego_r has ?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z (0x00742050 Ego_r) -- likely the real name; bool(EHeroAbility) thiscall ret 4. Unproven by 2 sources: declare it, flag in NOTES |
| 0x0040D2A0 | CUserProfileManager_Get_0040d2a0.cpp `static CUserProfileManager& FABLE_FASTCALL Get()` | 7 calls |
| 0x00408C90 | CUserProfileManager_GetAssignedInputForAction_00408c90.cpp `(EGameAction, bool, CActionInputControl&)` | action 0x4E |
| 0x00408F50 | ..._GetAssignedInputForContextSensitiveItemAction_00408f50.cpp `(EGameAction, bool, long, CActionInputControl&)` | action 0x38 |
| 0x00446720 | global_GetActionName_00446720.cpp; PDB `static bool __fastcall CGamePlayerInterface::GetActionName(const CActionInputControl&, CWideString&, bool)` | landed spelling is `_global::GetActionName` -- PDB says static member of CGamePlayerInterface (NAME_FIXES candidate) |
| 0x0099AED0 / 0x0099B510 | CWideString() / ~CWideString | |
| 0x009E21D0 | GFACoordToVCoord(const C2DVector&, C2DVector&) fastcall | |
| 0x009E1DA0 / 0x009E1DD0 | GFAXToVX(float) / GFAYToVY(float) | |
| 0x00438010 | CPlayerGui_Draw2DText_00438010.cpp (pos, text, font, colour, scale, layer, bool) ret 1Ch | 3 calls |
| 0x00405F00 | global_GFIntToWideString_00405f00.cpp | fastcall: ecx = result CWideString, edx = value |
| 0x0064546F | NPlayerGuiCDrawQuickAccessIcons_DrawEffectAtPos_0064546f.cpp (const C2DCoordI&, long effect, long index) ret 0Ch | 5 calls |
| 0x00645292 | NPlayerGuiCDrawQuickAccessIcons_DrawIcon_00645292.cpp (info, const C2DVector&, float, long index, long back, long icon, bool) ret 1Ch | 2 calls |
| 0x0064D7A6 | NOT LANDED: DrawChargeGraphic(const CEnginePrimitivePrepareInfo&, const C2DVector&, long) ret 0Ch | Ego_r ?DrawChargeGraphic@CDrawQuickAccessIcons@NPlayerGui@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC2DVector@@J@Z + bsim_port_audit_05final |
| 0x00A01B50 | CBaseIntelligentPointer::GetPItem | Effects[e] |
| 0x00AEE8C0 | NParticleEngine::CParticleEmitter::Kill | |
| 0x00655318 / 0x0065533C / 0x00A01C10 | CIntelligentPointer<CParticleEmitter>(CParticleEmitter*) ctor / operator= / ~CBaseIntelligentPointer | out of line: FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE / _ASSIGN_OUTOFLINE (as DrawEffectAtPos uses) |
| 0x00434840 / 0x00434870 / 0x0048DA30 / 0x004348D0 | CFadeInFadeOutBase::Open(float,float,CRGBColour) / Update(float) / IsOpen() / GetFadeInOutAlpha() | FlashFade +0x40 |

## 4. Members (this = esi)
- CDrawBase (engine/NPlayerGui_CDrawBase.h): PlayerGui +4, PlayerManager +8, World +0xC, Component +0x10, Drawing +0x18,
  PrimitiveHandles +0x1C (vector<CEnginePrimitiveHandle>, needs FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES which also declares
  ScaleAlpha). FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING gives SetAsDrawing (retail `mov byte [esi+18h],1`).
- CDrawQuickAccessIcons: engine/_quarantine/CDrawQuickAccessIcons.h only (quarantined, opaque members). Retail names (Ego_r, layout
  confirmed by ctor 0x00652F0A): ChargingSoundID +0x28, ChargingButtonIndex +0x2C (long; compared with -1, -2, slot), FlashButton +0x3C
  (EXboxControllerButton; 13..16), FlashFade +0x40 (CFadeInFadeOutBase: Active +0x40, Closing +0x4C, Opening +0x4D),
  ChargeUpPrimitiveHandle +0x5C, SpecialPrimitiveHandles +0x64, QuickAccessSpellCrossHandles +0x70,
  Effects +0x7C (vector<CIntelligentPointer<CParticleEmitter>>, 8-byte elements, size()= `sar 3`). Landed siblings model the class
  locally (ctor, ClearPrimitivesIfNecessary, Update, DrawIcon); copy the ctor 0x00652F0A model.
- CPlayerGui (engine/CPlayerGui.h): InputInterface +0xC, ShowSpecialAbilityButtons +0x250, NormalToSpecial +0x27C, SpecialToNormal +0x27D.
- CWorld (engine/CWorld.h): DefinitionManager +0x10, GuildSealRecallPos (C3DVector) +0x154 (header spells _X/_Y/_Z floats).
- CMainGameComponent (engine/CMainGameComponent.h): font counted pointer object word +0x1614C (PCutSceneMainFont? Ego_r +0x16144 is
  PCutSceneMainFont, +0x1613C PDebugFont; header line 114/232 has the retail word). Read in place: `mov eax,[eax+1614Ch]`.
- CPlayerGuiDef (engine/CPlayerGuiDef.h, retail offsets, edi): DPadRingsGraphic 0x170, DPadRingsPos 0x174, DPadRingsTopGraphic 0x17C,
  DPadRingsTopOffset 0x180, DPadRingsFavouriteGraphic 0x1B0, DPadRingsTopFavouriteGraphic 0x1B4, DPadRingsTopAggressiveGraphic 0x1B8,
  DPadGuildSealTooltipOffset 0x1C0, DPadMiddleOffset 0x1C8, DPadGuildSealRecallGraphic 0x1F0, GuildSealRecallGraphic 0x1FC,
  GuildSealLevelUpOffset 0x200, GuildSealMainLevelUpOffset 0x208, QuickAccessDPadGraphic 0x6DC, QuickAccessLampEffect 0x6E4,
  QuickAccessGuildSealEffect 0x6E8, QuickAccessMainGuildSealEffect 0x6EC, QuickAccessAbilityCrossOffset 0x6F8,
  QuickAccessItemButtons 0x728, ContextSensitiveItemButtons 0x734 (vector<CQuickAccessItemButtonGuiDef>, 0x50-byte elements,
  size() via `cdq ; idiv 50h` = signed ptrdiff), ABXYAbilityUncastableGraphic 0x7B4, ABXYAbilityShieldOffGraphic 0x7BC.
  The vector members are opaque `unsigned char[0xc]`; the C2DVector members are split _X/_Y. Opt-ins present: _QUICKACCESSEFFECTOFFSET
  (0x6F0 only), _ABILITYBUTTONS, _TARGETING, etc. -- none types 0x174/0x180/0x1C0/0x1C8/0x200/0x208/0x6F8 or the two vectors: an
  overlay opt-in is needed (C2DVector + std::vector<CQuickAccessItemButtonGuiDef>).
- CQuickAccessItemButtonGuiDef (engine/_quarantine/CQuickAccessItemButtonGuiDef.h, 0x50): vptr, ButtonGraphic +0x3C, Position +0x40,
  Offset +0x48.
- CThingObjectDef::Property (ulong, retail +0x90; Ego_r +0x94): bit 0x20 = guild seal, bit 0x01 = light/lamp. CAbilityDef::Ability +0x28.
- CTCLight (engine/CTCLight.h) Active at +0x1C (IsActive() const, inline).
- CUserProfileManager: UseNewProfileValues +0xC, HUDTooltipEnabled +0x19, HUDTooltipEnabledNewProfile +0xD9 -> inline
  `IsHUDTooltipEnabled() const` (FableWin ?IsHUDTooltipEnabled@CUserProfileManager@@QBE_NXZ; not declared in the shared header).
- CPlayer (GetMainPlayer result) byte +0x217 (aggressive/top-ring flag; name not resolved here -- check engine/CPlayer.h).
- CThing: UsingComponents +0x20 (bitset<274>), Components +0x44 (CVectorMap) -> FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED
  (GetTC non-const / PeekTC const bodies exactly as retail: key copied to a local inside the test, map through a pointer local,
  conditional-expression end-or-greater).
- Globals: 0x01375744 long 255 (.data, writable; read as byte for ScaleAlpha and as dword for the fimul) -- no name found (descriptive);
  0x01375404 static C2DVector (0.1f, 0.01f) .data -- no name; floats 0x0129BA3C DELTA_NOTIONAL_ZERO, 0x01230014 255.0f, 0x0122F59C 0.5f,
  0x0123816C 0.6f, 0x01230884 12.0f, 0x01230894 32.0f, 0x01238164 16.0f, 0x0125A4DC 1/3.

## 5. Visibility
- Out of line in retail (call, not expand): GFAnchorToBottomLeft 0x00643CDA and ScaleAlpha 0x00643D5D (both in the NPlayerGui TU at
  neighbouring addresses, unaligned -> inline members emitted out of line), DrawIcon / DrawEffectAtPos / DrawChargeGraphic (private, same
  class), GetGuildSealAlpha, vector erase/resize, bitset::test, both LowerBounds, CIntelligentPointer ctor/assign/dtor, GetPItem, all CFadeInFadeOutBase
  methods, IS_THING_ALIVE (noinline fastcall).
- Expanded in place: ChargeUpPrimitiveHandle.Clear() (0x0065307E..0x006530B2 = the __forceinline model in ClearPrimitivesIfNecessary 0x0064D62F),
  every CDefPointer destructor (`test ; dec [p+4] ; jne ; call [vtbl+4]`), GetTC/PeekTC/HasTC, IsHUDTooltipEnabled, the vector size(),
  C3DVector "non-zero" test (z,y,x loaded, eps*eps vs sum, `test ah,5 ; jp`), C2DVector +/+=, C2DCoordI from floats (two __ftol2),
  CActionInputControl default ctor (7 zero dwords/floats [ebp-34h..-1Ch]), CRGBColour stores, FlashFade "Close" (Opening=0 then Closing=1).

## 6. Siblings (same class / TU; all `#pragma optimize("y", off)` + `("s", on)`)
- rebuild/src/compiled/00/64/NPlayerGuiCDrawQuickAccessIcons_ClearPrimitivesIfNecessary_0064d62f.cpp (Clear() __forceinline model)
- .../00/64/NPlayerGuiCDrawQuickAccessIcons_Update_0064d6a8.cpp (`// Flags: /GS`, IS_THING_ALIVE model, CPlayerManager.h)
- .../00/64/NPlayerGuiCDrawQuickAccessIcons_DrawIcon_00645292.cpp (sprite + SetPersistentPrimitive idiom, `C2DVector(pos.X,pos.Y)` temp)
- .../00/64/NPlayerGuiCDrawQuickAccessIcons_DrawEffectAtPos_0064546f.cpp (opt-ins: CINTELLIGENTPOINTER_CTOR/ASSIGN_OUTOFLINE,
  CPARTICLEEMITTER_KILL, C2DCOORDI_*, C2DVECTOR_*, CPLAYERGUI_PEEKGUIDEF, CPLAYERGUIDEF_QUICKACCESSEFFECTOFFSET)
- .../00/64/NPlayerGuiCDrawQuickAccessIcons_ToggleCharge_0064d92a.cpp (CTHING / CVECTORMAP_FORCEINLINE_FIND / CPLAYERMANAGER opt-ins)
- .../00/65/NPlayerGuiCDrawQuickAccessIcons_CDrawQuickAccessIcons_00652f0a.cpp (immediately precedes this row: member layout model)
