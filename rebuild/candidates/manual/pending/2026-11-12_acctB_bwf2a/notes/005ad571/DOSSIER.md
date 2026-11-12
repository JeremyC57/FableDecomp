# DOSSIER 0x005AD571 -- CTCInventoryTrade::ConstructTradeDescription (4414 bytes, 0x005AD571..0x005AE6AE)

## 1. Retail shape (python3 $S/w17/xdis.py 005ad571 0x114e; $W/dis_all.txt)
- `__thiscall`, `ret 0x10` (4 dword args), returns `CComponent*` in eax (= [ebp-0x14], the container).
- Frame pointer: `push ebp; mov ebp,esp; sub esp,0x74; push esi; push edi` ... epilogue `pop edi; pop esi; leave; ret 10h`
  -> `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (`leave`, no alignment padding: the TU neighbours 0x005AE6AF
  RefreshDescriptions and 0x005A3E47 ConstructBuyActionsList use exactly these two pragmas).
- NO SEH frame (no fs:0, no state stores) although CCharString / CWideString / CCountedPointer / CDefPointer locals are destroyed:
  same as every landed CTCInventoryTrade row (no /EHsc in this TU; landed rows have no `// Flags: /EHsc`).
- `push ebx` is late (0x005AD5C3, after the two early returns); the early returns `jmp 0x005AE6A9` (pop edi/pop esi only); the
  normal path pops ebx in the middle of the destructor run (0x005AE687). Compiler-made; no source trick needed.
- ebx = 0 for the whole body (all `cmp x, bl` / `mov [..], ebx`).
- Dead parameter slots are reused for temporaries/locals: [ebp+0xC] (count), [ebp+0x10] (bool 3), [ebp+0x14] (bool 4); bool
  isSpecialShop lives in the BYTE [ebp+0xF]. Compiler choice, not a source feature.
- x87: `fild / fmul dword [0x0122F59C] (=0.5f) / call __ftol2 0x00BFEA70` = `unsigned char *= 0.5f` (8 sites).
- No /GS cookie, no jump tables.

## 2. Identity
- Name: `?ConstructTradeDescription@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z`
  -> `NUISystem::CComponent* CTCInventoryTrade::ConstructTradeDescription(long, unsigned long, bool, bool)` (public, non-const).
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x00610B43), ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv
  (FableWin 0x0231C8CA, 12574 b / 2841 i). Siblings with the identical signature: ...Weapon (Ego_r 0x0060D3CD, retail 0x005A9CBE) and
  ...Clothing (Ego_r 0x0060E67B, retail 0x005AAF28), both called from this body. Callers: RefreshDescriptions 0x005AE6AF,
  RefreshBuyDescriptionsPC 0x005AE9DE, RefreshSellDescriptionsPC 0x005AEAEE (all landed, pass (defIndex, 1, true|false, false)).
  Manifest label CORRECT.
- Param names are not in any PDB we have. Suggested (semantics from the body): `long itemDefIndex, unsigned long count, bool buying,
  bool special`. `buying` (arg3): true -> "already owns N" line + heavy-weapon requirement lines; false -> profit/loss line.
  arg4: true -> short form (price line + item description), false -> long form. Landed callers already spell `(long defIndex,
  unsigned long count, bool buying, bool ...)`.
- Not a virtual (QAE, no vtable slot).

## 3. Callees (retail addr / landed row / landed name / PDB-true spelling)
| addr | landed row | landed name | true name (evidence) |
|---|---|---|---|
| 0x005D97F0 | 00/5d/CTCInventoryItem_IsWeapon_005d97f0.cpp | IsWeapon | `static bool __fastcall CTCInventoryItem::IsWeapon(long)` ?IsWeapon@CTCInventoryItem@@SI_NJ@Z (engine/CTCInventoryItem.h:73) |
| 0x005D9C60 | 00/5d/CTCInventoryItem_IsClothing_005d9c60.cpp | IsClothing | ?IsClothing@CTCInventoryItem@@SI_NJ@Z |
| 0x005A9CBE | -- (not landed) | ConstructTradeDescriptionWeapon | ?ConstructTradeDescriptionWeapon@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z (needs a header decl) |
| 0x005AAF28 | -- (not landed) | ConstructTradeDescriptionClothing | ?ConstructTradeDescriptionClothing@...JK_N0@Z |
| 0x0041E5F2 | NUISystemCManager_GetInstance_0041e5f2.cpp | CManager::GetInstance | static, fastcall-like (no args) |
| 0x0041DB1D | NUISystemCManager_CreateComponent_0041db1d.cpp | CreateComponent | ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z (x11) |
| 0x00429C6F | CCountedPointerNUISystemCComponent_Reset_00429c6f.cpp | CCountedPointer<CComponent>::Reset(CComponent*) | ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z (x9, out of line) |
| 0x004291DE | CCountedPointerNUISystemCComponent_ReleaseData_004291de.cpp | ReleaseData | dtor body; FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h) |
| 0x0099EBF0 / 0x0099EAE0 | CCharString(const char*, long=-1) / ~CCharString | | fable_string.h |
| 0x0099AED0 | CWideString() | | |
| 0x0099B510 | ~CWideString (x51) | | |
| 0x0099B7D0 | CWideString::operator=(const CWideString&) (x9; incl. `= CWideString::NullString` 0x013BCA24) | | |
| 0x0099B860 | CWideString_operator_0099b860.cpp | `operator=(wchar_t)` | retail passes `movzx ax,al` of ConstructString's bool -> the original source assigned the BOOL result: `text = NGameText::ConstructString(fmt, args);` (implicit bool->wchar_t) -- this row is one of only 2 callers |
| 0x0099B8D0 | CWideString::operator+=(const CWideString&) ??YCWideString@@QAEABV0@ABV0@@Z | | |
| 0x0099BE70 | global_operator_0099be70.cpp | operator+(const CWideString&, const CWideString&) | ??H@YI?AVCWideString@@ABV0@0@Z (fastcall: ecx=result, edx=lhs, push rhs) x26 |
| 0x0099BFF0 | global_operator_0099bff0.cpp | operator+(const wchar_t*, const CWideString&) | ??H@YI?AVCWideString@@PB_WABV0@@Z, with L"/" 0x0122DE74 |
| 0x0099B2C0 | CWideString_ConvertToCharString_0099b2c0.cpp | file says ConvertToCharString | `CCharString CWideString::ToCharString() const` (fable_string.h:167 ?ToCharString@CWideString@@QBE?AVCCharString@@XZ) -- FILE NAME DISAGREES |
| 0x00405F00 | global_GFIntToWideString_00405f00.cpp | GFIntToWideString | `CWideString FABLE_FASTCALL GFIntToWideString(long)` (decl as in 0x005A3E47) x10 |
| 0x009C95E0 | NGameText::CDataBank::GetTextBySymbol | | `const CWideString GetTextBySymbol(const CCharString&) const` (NGameText_CDataBank.h:53) x12 |
| 0x009C7B70 | global_ConstructString_009c7b70.cpp | ConstructString | `bool __fastcall NGameText::ConstructString(CWideString&, long)` (?ConstructString@NGameText@@YI_NAAVCWideString@@J@Z, header line 23) |
| 0x009C7C20 | NGameText_ConstructString_009c7c20.cpp | ConstructString | `(CWideString&, const std::vector<CWideString>&)` fastcall |
| 0x0044C6B0 | CGameDefinitionManager::Get | | ?Get@CGameDefinitionManager@@SIAAV1@XZ |
| 0x009AD390 | COptimisedPrimitive_ActualToUniqueIndex_009ad390.cpp (!) | manifest: GetGlobalIndexFromClassIndex | `CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, bool/long)` as spelled in landed 0x005C90FC -- landed FILE NAME WRONG |
| 0x0042B312 | CDefinitionManager_GetDef0042b312_0042b312.cpp | GetDef<T> | GetDef<NUISystem::CUIMiscThingsDef>(long, CDefPointer<const T>*) (template, out of line) |
| 0x0042AF3C | CDefinitionManager_GetDefT_0042af3c.cpp | GetDef<CThingBaseDef> | same template, T=CThingBaseDef |
| 0x006869D0 | CTCBase::GetDefinitionManager | | ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (x2) |
| 0x009AD9E0 | CDefinitionManager::GetPDefFromGlobalIndex | | returns CDefPointer<const CDefClassBase> by value (hidden ptr) |
| 0x0059350C | GetSubDef<CStockItemDef> | | ??$GetSubDef@VCStockItemDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCStockItemDef@@@@@Z |
| 0x0042B1A6 | GetSubDef<CWeaponDef> | | ??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_N... |
| 0x005BC4C4 | global_DrawGetEnvironment_005bc4c4.cpp (!) | DrawGetEnvironment / GetMember_dword0xb8 | `CThing* NInventory::CTCInventoryBase::GetBuildingThing() const` (NInventory_CTCInventoryBase.h:112-114, FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING) -- LANDED NAME WRONG |
| 0x00410DE0 | STLbitset_test_00410de0.cpp | bitset<274>::test | inside the GetTC/PeekTC expansion (FABLE_RETAIL_STLP_BITSET_TEST_DECL) x5 |
| 0x0040F020 / 0x004365B0 | CVectorMap::LowerBound non-const / const | | inside GetTC (non-const) x3 / PeekTC (const) x2 |
| 0x007743B0 / 0x007742C0 | CTCShop::GetCurrentSellPrice / GetCurrentBuyPrice | | ?..@CTCShop@@QBEJJJ@Z (long defIndex, long count) |
| 0x00772490/0x00772510/0x00772590 | CTCShop::IsBarberShop / IsTattooShop / IsTitleShop | | QBE_NXZ |
| 0x007726B0 / 0x00772720 / 0x00772790 | CTCShop::GetStockQuantity / GetMaxStock / GetDaysTillRestock | | (long defIndex) |
| 0x00449960 | CPlayerManager::GetMainPlayer | | QBEABVCPlayer@@XZ |
| 0x00487DD0 | CPlayer::PeekPControlledCreature | | QBEPBVCThingPlayerCreature@@XZ |
| 0x005D99D0 | global_GetInventoryCategory_005d99d0.cpp | GetInventoryCategory | `static ETCInterfaceType __fastcall CTCInventoryItem::GetInventoryType(long)` (CTCInventoryItem.h:48-52; PDB ?GetInventoryType@CTCInventoryItem@@SI?AW4ETCInterfaceType@@J@Z) -- LANDED NAME WRONG |
| 0x005BDF08 | CTCInventoryBase::GetNumberOfItemsOfTypeInInventory | | ?..@CTCInventoryBase@NInventory@@QBEJJ@Z |
| 0x005BEAD5 | CTCInventoryBase_GetItemDescription_005bead5.cpp | GetItemDescription | `long NInventory::CTCInventoryBase::GetBoughtPrice(long) const` (FableWin ?GetBoughtPrice@CTCInventoryBase@NInventory@@QBEJJ@Z; header NInventory_CTCInventoryBase.h:77 "native005bead5"; retail: thiscall, 1 arg, value used as a number) -- LANDED NAME WRONG (GetItemDescription is vslot 78, 0x005BC6B3) |
| 0x006D6520 | CTCInventoryAbilities_GetAbilityLevel_006d6520.cpp (!) | GetTrainableStatLevel | `long CTCHeroExperience::GetTrainableStatLevel(EHeroTrainableStatType) const` (CTCHeroExperience.h:88) -- FILE NAME WRONG |
| 0x0056F26F | CTCInventoryExperience::GetExperienceStatName | | `CWideString GetExperienceStatName(long)` (CTCInventoryExperience.h:49) |
| 0x004128A0 | vector<CWideString>::push_back(const CWideString&) | | out of line, x4 |
| 0x00412130 | CCombatWheel_ResetRings_00412130.cpp (!) | ResetRings | `std::vector<CWideString>::~vector()` (manifest + landed 0x00E64FB0 decl `template<> std::vector<CWideString>::~vector(); // 0x00412130`) -- LANDED NAME WRONG |
| 0x0042D5B1 | map<unsigned long,CUIState>::operator[] | | out of line x14 (FABLE_RETAIL_STLP_MAP makes the body visible, as 0x005C90FC) |
| 0x004292C0 | _Rb_global<bool>::_M_increment | | cdecl static (`push; call; pop ecx`) |
| 0x00BFEA70 | __ftol2 | | |
Virtual calls: CComponent slot 59 +0xEC AddChild(CCountedPointer<CComponent>&) x7; CText slot 144 +0x240 SetValue(const CWideString&, bool, bool) x7
(called on the raw text pointer with (text,1,1)); CComponent slot 82 +0x148 GetStates() -> map<unsigned long,CUIState>& x13;
CTCInventoryBase slot 78 +0x138 GetItemDescription(CWideString&, long, bool) const x2 (args text, defIndex, true);
pointee slot 1 +0x4 = CDefPointeeBase::Delete (the CDefPointer release; x6).

## 4. Members
- this: CTCInventoryTrade (engine/CTCInventoryTrade.h, FABLE_RETAIL_CTCINVENTORYTRADE): +0x218 `bool InventoryTradeOpen` (header line 194/363,
  static assert line 409; FableWin also has inline ?IsInventoryTradeOpen@CTCInventoryTrade@@QBE_NXZ 18 b, either spelling) -> true selects the
  "PC_*" templates; +0x4 CTCBase::ParentThing (CThing*); GetBuildingThing() = [this+0xB8].
- CUIMiscThingsDef (engine/NUISystem_CUIMiscThingsDef.h, retail offsets asserted): +0x3C SpaceSeparator, +0x44 ColonSeparator, +0x4C OpenBracket,
  +0x50 CloseBracket, +0x80 TradeCostString, +0x84 TradeProfitString, +0x88 TradeLossString, +0x8C TradeAlreadyOwnsString,
  +0x90 TradeNumberInStockString, +0x94 TradeDeliveryString, +0x98 TradeNoDeliveryString (all CWideString; the Trade* ones hold text
  SYMBOLS: `bank.GetTextBySymbol(miscDef->TradeCostString.ToCharString())`). Needs FABLE_RETAIL_DEF_CLASS_BASE.
- CStockItemDef +0x28 DefaultPrice (engine/CStockItemDef.h, FABLE_RETAIL_CSTOCKITEMDEF).
- CWeaponDef +0x2C `EWeaponClass Class`, ==2 = WC_HEAVY (engine/CWeaponDef.h, engine/EWeaponClass.h).
- CTCHeroExperience +0x58 PHeroCombatDef, inline `GetPHeroCombatDef()` by value (FABLE_RETAIL_CTCHEROEXPERIENCE_GETPHEROCOMBATDEF);
  CHeroCombatDef +0x13C HeavyWeapons.StatThresholds (map<EHeroTrainableStatType,long>: node +0x10 first, +0x14 second;
  FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS, engine/CHeroAbilityStatPackage.h).
- CUIState +0x18 CRGBColour Colour {B,G,R,A}: +0x18 = B, +0x19 = G, +0x1A = R (engine/NUISystem_CUIState.h, engine/CRGBColour.h).
  "red" = B,G *= 0.5f; "green" = B,R *= 0.5f.
- CMainGameComponent::PMainGameComponent 0x013B86A0: +0x14 text bank (GetTextBank(), FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK), +0x1C player
  manager (PeekPlayerManager(), FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER).
- CThing +0x20 UsingComponents bitset, +0x44 CVectorMap TCs (end at +0x48): via FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED.
- No class needs a local model: every one has a shared header (the landed RefreshDescriptions still uses a LOCAL model -- don't copy it;
  copy 0x005A3E47 ConstructBuyActionsList, which is header-based).

## 5. Visibility / inline expansions
- Inlined (expanded): CThing::GetTC<CTCShop>(&shop) (0x005AD6AF), PeekTC<const CTCInventoryBase>(GetInventoryType(i), &inventory) (x2),
  GetTC<CTCHeroExperience>(&exp), GetTC<CTCInventoryExperience>(&invExp). FableWin.exe.signatures lists ONLY the one-argument
  `??$GetTC@VCTCShop@@@CThing@@QAE_NPAPAVCTCShop@@@Z`, `..CTCHeroExperience..`, `..CTCInventoryExperience..` and the two-argument
  `??$PeekTC@$$CBVCTCInventoryBase@NInventory@@@CThing@@QBE_NW4ETCInterfaceType@@PAPBV..@Z` -> spell `GetTC(&shop)` (one arg), but
  `PeekTC(CTCInventoryItem::GetInventoryType(itemDefIndex), &inventory)` (two args). (The landed sibling 0x005A3E47 spells
  `GetBuildingThing()->GetTC(TCI_SHOP, &shop)`; lesson f4007: the one-argument form adds IL, prefer FableWin's.)
- CDefPointer copies/releases inline (FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE as 0x005C6B26: the `def` release at 0x005AE66B is the
  "add eax,4; dec [eax]; cmp [eax],0" form + `Object=0` store). CCountedPointer::Reset / ReleaseData out of line. CMainGameComponent::Get,
  GetTextBank, PeekPlayerManager, GetBuildingThing (out of line), IsInventoryTradeOpen inline.
- STLport out of line: map<ulong,CUIState>::operator[] 0x0042D5B1, _M_increment 0x004292C0, vector<CWideString>::push_back 0x004128A0
  (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE / _NOINLINE in stlport/fable_containers.h:466-472), ~vector<CWideString> 0x00412130 (no
  macro found; 0x00E64FB0 declares an explicit specialisation), bitset::test 0x00410DE0, CVectorMap::LowerBound.
- Neighbours in this TU (CTCInventoryTrade.cpp, /Os, unaligned): ConstructTradeDescriptionWeapon 0x005A9CBE, ...Clothing 0x005AAF28 (NOT landed),
  this row, RefreshDescriptions 0x005AE6AF, OpenInventoryPC 0x005AE86B, OpenActualBuyInventoryPC 0x005AE972, RefreshBuyDescriptionsPC 0x005AE9DE,
  RefreshSellDescriptionsPC 0x005AEAEE, ProcessTradeAction 0x005AEBFE.

## 6. Sibling rows to copy idioms from
- rebuild/src/compiled/00/5a/CTCInventoryTrade_ConstructBuyActionsList_005a3e47.cpp -- SAME TU, header-based; pragmas `optimize("y",off)` +
  `optimize("s",on)`; macros RELEASEDATA, CCHARSTRING, CWIDESTRING, CTCBASE, CIVCOUNTEDPOINTER, CTCINVENTORYBASE(_BUILDINGTHING,_CONTAINABLE),
  CTCINVENTORYTRADE(_ACTIONSLISTS), CTHING(_GETTC_EXPANDED), STLP_BITSET_TEST_DECL, STLP_LIST, STLP_MAP, CTCSHOP(_SHOPTYPE), CMAINGAMECOMPONENT
  (_PLAYERMANAGER,_TEXTBANK), CPLAYERMANAGER, PLAYER, CTCINVENTORYITEM(_CATEGORY), CCOUNTEDPOINTER_CALLED_CTOR. Shows shop-type tests,
  hero/PeekTC, bank, GFIntToWideString decl.
- rebuild/src/compiled/00/5c/CTCInventoryWeapons_ConstructWeaponDescriptionContainer_005c90fc.cpp -- the closest BODY model: misc-def lookup
  (`CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1)` + `GetDef(miscIndex, &miscDef)`), text +=
  chains, if/else PC/UI `textComponent.Reset(CManager::GetInstance()->CreateComponent(..., false))` cross-jumped, `GetStates()[0].Colour.B *= ..`,
  the HeavyWeapons StatThresholds walk with `combatDef = experience->GetPHeroCombatDef()` (`inc; inc; dec`). Macros: DEF_CLASS_BASE,
  CDEFPOINTER_RELEASE_POINTEE(0x005C6B26), CTCHEROEXPERIENCE(+_GETPHEROCOMBATDEF), EHEROTRAINABLESTATTYPE, CHEROCOMBATDEF_HEAVYWEAPONS,
  CWIDESTRING_NULLSTRING, CSTOCKITEMDEF, STLP_MAP, CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY, CGAMEDEFINITIONMANAGER, CDEFINITIONMANAGER.
- rebuild/src/compiled/00/5c/CTCInventoryWeapons_ConstructWeaponDescription_005c6b26.cpp -- stock-def DefaultPrice via GetPDefFromGlobalIndex +
  GetSubDef<CStockItemDef>, CDefPointer release form.
- rebuild/src/compiled/00/5a/CTCInventoryTrade_RefreshDescriptions_005ae6af.cpp -- next row, same pragmas (local model; for declaration only).
- Family lessons: /opt/fable_lane/w22/agent4007/LESSONS.md (`||` arm placement, inline-bool helpers, cd.sh full call compare).

## 7. Open points for the author
- weaponDef ([ebp+0xC]) is released WITHOUT a null test (0x005AE5B4) while thingDef (0x005AE5C3), stockDef, def, miscDef(esi) have one.
  Try: weaponDef obtained in a way whose release the compiler proves non-null, or a different pointer type; compare 0x005C90FC's weaponDef.
- miscDef: GetDef fills [ebp-0x20]; the value is then held in esi to the end (release through esi, 0x005AE685) and [ebp-0x20] is reused
  for `inventory`. Same as the landed 0x005C90FC idiom (`CDefPointer<const CUIMiscThingsDef> miscDef; ...GetDef(miscIndex, &miscDef);`).
- The final `if (buying && CTCInventoryItem::IsWeapon(itemDefIndex))` block is dead at run time (IsWeapon returned early) but compiled.

## NAME_FIXES candidates (landed spellings proven wrong)
0x005BEAD5 GetItemDescription -> NInventory::CTCInventoryBase::GetBoughtPrice (FableWin QBEJJ + header :77)
0x005D99D0 GetInventoryCategory -> CTCInventoryItem::GetInventoryType (Ego_r/FableWin SI?AW4ETCInterfaceType@@J@Z + header :48)
0x00412130 CCombatWheel::ResetRings -> std::vector<CWideString>::~vector (manifest + 0x00E64FB0)
0x005BC4C4 DrawGetEnvironment -> NInventory::CTCInventoryBase::GetBuildingThing (header :112)
0x0099B2C0 file ConvertToCharString -> CWideString::ToCharString (fable_string.h:167)
0x006D6520 file CTCInventoryAbilities_GetAbilityLevel -> CTCHeroExperience::GetTrainableStatLevel
0x009AD390 file COptimisedPrimitive_ActualToUniqueIndex -> CDefinitionManager::GetGlobalIndexFromClassIndex (manifest)
