# DOSSIER 0x005AAF28 -- CTCInventoryTrade::ConstructTradeDescriptionClothing (4550 bytes, 0x11C6)

## 1. Retail shape (xdis 005aaf28 0x11d6 / $W/dis_all.txt)
- `push ebp; mov ebp,esp; sub esp,74h; push ebx; push esi; mov ebx,ecx; push edi` ... `pop edi; pop esi; pop ebx; leave; ret 10h`.
  -> thiscall, 4 stack args, frame pointer + `leave` = `#pragma optimize("y",off)` + `#pragma optimize("s",on)` (every
  CTCInventoryTrade sibling: RefreshBuyPossibility 0x5A9A87, ConstructActualSpecialInventoryPC 0x5A8C66, BuyItem 0x5AF64E,
  RefreshDescriptions 0x5AE6AF all carry exactly these two pragmas). Size idioms present: `push 68h; pop ebx` (0x5ABD20),
  `and dword [x],0` stores, no alignment padding.
- NO EH frame (no fs:0 anywhere, no state stores) although CWideString / CCharString / CCountedPointer locals have dtors:
  same as the family model ConstructWeaponDescriptionContainer 0x5C90FC ("no EH frame"). No `// Flags:` line in any sibling;
  build default (no /EHsc). No /GS cookie.
- Return value: `mov eax,[ebp-10h]` = the container component created first.
- Param 2 ([ebp+0Ch], unsigned long) is never read. Params 3/4 slots ([ebp+10h]/[ebp+14h]) are reused by the compiler as
  temporaries once dead (and a bool local lives in byte [ebp+17h]) -- normal c2 slot sharing, do not model.

## 2. Identity (two sources) -- manifest label is CORRECT
- FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x017DD0CA / 0x0232C587, naming_stragglers/FableWin.exe.signatures.tsv) and
  Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x0060E67B): `?ConstructTradeDescriptionClothing@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z`
- Signature: `public: NUISystem::CComponent* CTCInventoryTrade::ConstructTradeDescriptionClothing(long, unsigned long, bool, bool)` (non-const, QAE).
- Caller: only CTCInventoryTrade::ConstructTradeDescription 0x005AD571 (xref.py: call at 0x005AD5B9, after IsWeapon 0x5D97F0 ->
  ConstructTradeDescriptionWeapon 0x5A9CBE, and 0x5D9C60 (IsClothing?) -> this); forwards all 4 args unchanged.
  Landed callers of ConstructTradeDescription spell the params `(long defIndex, unsigned long count, bool buying, bool special)`
  (RefreshSellDescriptionsPC 0x5AEAEE:165) -- parameter names unproven (descriptive). Semantics confirm "buying":
  `buying && !special` -> shop's GetCurrentSellPrice, else GetCurrentBuyPrice; `!buying` shows profit/loss vs GetBoughtPrice.
- Sibling ConstructTradeDescriptionWeapon 0x5A9CBE (4714 b) is NOT landed (peer scout dir $S/w22/bwf4/005a9cbe, empty).
- CTCInventoryTrade.h retail branch does NOT declare ConstructTradeDescription* -> the author must add an overlay opt-in
  (e.g. FABLE_RETAIL_CTCINVENTORYTRADE_TRADEDESCRIPTION: ConstructTradeDescription 0x5AD571, ...Weapon 0x5A9CBE, ...Clothing 0x5AAF28,
  all public QAE per the mangling).

## 3. Callees (retail addr | count | landed file | landed name | PDB name; FLAG = landed spelling wrong)
| addr | n | landed row / name | PDB / correct name |
|---|---|---|---|
| 0041E5F2 | 1 | NUISystemCManager_GetInstance | ?GetInstance@CManager@NUISystem@@SIPAV12@XZ (static fastcall) |
| 0041DB1D | 12 | NUISystem::CManager::CreateComponent | ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z |
| 00429C6F | 10 | CCountedPointer<CComponent>::Reset | ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z |
| 004291DE | 1 | CCountedPointer<CComponent>::ReleaseData | ?ReleaseData@...QAEXXZ (the inline dtor calls it; opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA[_DECL_ONLY]) |
| 0099EBF0 | 16 | CCharString(const char*, long=-1) | -- (`push -1; push lit`) |
| 0099EAE0 | 22 | ~CCharString | |
| 0099AED0 | 1 | CWideString() | ??0CWideString@@QAE@XZ |
| 0099B510 | 58 | ~CWideString | ??1CWideString@@QAE@XZ |
| 0099B7D0 | 10 | CWideString::operator=(const CWideString&) | |
| 0099B800 | 1 | CWideString::operator=(const wchar_t*) | with L"" 0x0122D70C |
| 0099B8D0 | 5 | CWideString::operator+= | ??YCWideString@@QAEABV0@ABV0@@Z |
| 0099BE70 | 33 | operator+(const CWideString&, const CWideString&) fastcall (ecx=ret, edx=left, stack=right) | ??H@YI?AVCWideString@@ABV0@0@Z |
| 0099BFF0 | 1 | operator+(const wchar_t*, const CWideString&) | with L"/" 0x0122DE74 |
| 0099B0F0 | 1 | CWideString::operator!= | ??9CWideString@@QBE_NABV0@@Z (vs CWideString::NullString 0x013BCA24) |
| 0099B2C0 | 8 | file CWideString_ConvertToCharString (CWideStringConversionView::ConvertToCharString) **FLAG** | ?ToCharString@CWideString@@QBE?AVCCharString@@XZ (fable_string.h:167) |
| 00405F00 | 11 | GFIntToWideString | `CWideString FABLE_FASTCALL GFIntToWideString(long)` (ecx=ret, edx=value) as declared in 0x5C90FC |
| 009C95E0 | 11 | NGameText::CDataBank::GetTextBySymbol | returns CWideString by value (hidden ret) |
| 0044C6B0 | 2 | CGameDefinitionManager::Get | ?Get@CGameDefinitionManager@@SIAAV1@XZ |
| 009AD390 | 1 | file COptimisedPrimitive_ActualToUniqueIndex **FLAG** | ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z (manifest + 0x5C90FC callee line) |
| 0042B312 | 1 | CDefinitionManager::GetDef_0042b312 **FLAG** | GetDef<NUISystem::CUIMiscThingsDef> (template; 0x5C90FC / 0x5C7452 comments) |
| 006869D0 | 2 | CTCBase::GetDefinitionManager | ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (CTCBase.h:148) |
| 009AD9E0 | 1 | CDefinitionManager::GetPDefFromGlobalIndex_b **FLAG(suffix)** | GetPDefFromGlobalIndex (returns CDefPointer<const CDefClassBase> by value) |
| 0059350C | 1 | CDefClassBase::GetSubDef<CStockItemDef> | ??$GetSubDef@VCStockItemDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCStockItemDef@@@@@Z |
| 0042AF3C | 1 | CDefinitionManager::GetDef_0042af3c (manifest "GetDef<CThingBaseDef>") **FLAG** | GetDef<T> template, T unproven (result only fed to GetSubDef<CWeaponDef>) |
| 0042B1A6 | 1 | CDefClassBase::GetSubDef<CWeaponDef> | ??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWeaponDef@@@@@Z |
| 005BC4C4 | 1 | global `DrawGetEnvironment` (manifest GetMember_dword0xb8) **FLAG** | ?GetBuildingThing@CTCInventoryBase@NInventory@@QBEPAVCThing@@XZ (NInventory_CTCInventoryBase.h:112-114, FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING) |
| 00410DE0 | 5 | std::bitset<274>::test | HasTC inside expanded GetTC/PeekTC (FABLE_RETAIL_STLP_BITSET_TEST_DECL) |
| 0040F020 | 3 | CVectorMap<..>::LowerBound (non-const) | GetTC expansion |
| 004365B0 | 2 | CVectorMap<..>::LowerBound const | PeekTC expansion |
| 007743B0 | 1 | CTCShop::GetCurrentSellPrice | ?GetCurrentSellPrice@CTCShop@@QBEJJJ@Z (CTCShop.h:40, FABLE_RETAIL_CTCSHOP_SELLPRICE) |
| 007742C0 | 1 | CTCShop::GetCurrentBuyPrice | ?GetCurrentBuyPrice@CTCShop@@QBEJJJ@Z (CTCShop.h:35) |
| 00772490/510/590 | 1 each | CTCShop::IsBarberShop / IsTattooShop / IsTitleShop | QBE_NXZ (CTCShop.h:79-81, FABLE_RETAIL_CTCSHOP_SHOPTYPE) |
| 007726B0 | 1 | CTCShop::GetStockQuantity **FLAG (probable)** | ?GetQuantity@CTCShop@@QBEJJ@Z -- the only CTCShop QBEJJ@Z name besides GetMaxStock/GetDaysTillRestock in both PDB name sets; body reads CShopItem::Quantity |
| 00772720 | 1 | CTCShop::GetMaxStock | ?GetMaxStock@CTCShop@@QBEJJ@Z |
| 00772790 | 1 | CTCShop::GetDaysTillRestock | ?GetDaysTillRestock@CTCShop@@QBEJJ@Z |
|  |  | (none of the three stock methods is declared in CTCShop.h -> overlay declarations needed) | |
| 00449960 | 2 | CPlayerManager::GetMainPlayer | ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ |
| 00487DD0 | 2 | CPlayer::PeekPControlledCreature | ?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ |
| 005D99D0 | 2 | global `GetInventoryCategory` **FLAG** | ?GetInventoryType@CTCInventoryItem@@SI?AW4ETCInterfaceType@@J@Z (CTCInventoryItem.h:48-52, FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY) |
| 005D97F0 | 1 | CTCInventoryItem::IsWeapon | ?IsWeapon@CTCInventoryItem@@SI_NJ@Z |
| 005BEAD5 | 1 | CTCInventoryBase::GetItemDescription(CWideString*,long,bool) **FLAG** | ?GetBoughtPrice@CTCInventoryBase@NInventory@@QBEJJ@Z (body: GetInventoryCategory 0x5D97B0, category find, tail-jmp CCategory::GetBoughtPrice 0x5BE839; header NInventory_CTCInventoryBase.h:77) |
| 005BDF08 | 1 | CTCInventoryBase::GetNumberOfItemsOfTypeInInventory | ?..@CTCInventoryBase@NInventory@@QBEJJ@Z (header :135) |
| 005B6767 | 1 | CTCInventoryClothing::GetItemDescriptionArmourRating | ?GetItemDescriptionArmourRating@CTCInventoryClothing@@QBEXAAVCWideString@@J@Z |
| 005B62FE | 1 | CTCInventoryClothing::GetItemDescriptionNoArmourRating | ?GetItemDescriptionNoArmourRating@CTCInventoryClothing@@QBEXAAVCWideString@@J@Z (FableWin) |
| 0056F26F | 1 | CTCInventoryExperience::GetExperienceStatName | ?GetExperienceStatName@CTCInventoryExperience@@QAE?AVCWideString@@J@Z (non-const; CTCInventoryExperience.h:49) |
| 006D6520 | 1 | file CTCInventoryAbilities_GetAbilityLevel, `GetAbilityLevel` **FLAG** | ?GetTrainableStatLevel@CTCHeroExperience@@QBEJW4EHeroTrainableStatType@@@Z (manifest + 0x5C90FC) |
| 004292C0 | 1 | std::_Rb_global<bool>::_M_increment (cdecl, `push; call; pop ecx`) | out of line (++it on the map iterator) |
| 0042D5B1 | 14 | map<unsigned long,NUISystem::CUIState>::operator[] | out of line; FABLE_RETAIL_STLP_MAP makes the body visible (0x5C90FC) |
| 00BFEA70 | 11 | __ftol2 | byte = float*0.5f |
| vslot +0xEC (59) | 9 | CComponent::AddChild(CCountedPointer<CComponent>&) | NUISystem_CComponent.h:128 |
| vslot +0x240 (144) | 9 | CText::SetValue(const CWideString&, bool, bool) | NUISystem_CText.h |
| vslot +0x148 (82) | 14 | CComponent::GetStates() | NUISystem_CComponent.h:151 |
| vslot +0x54 (21) | 2 | CTCBase::GetInterfaceType() const | CTCBase.h:100 |
| vslot +0x138 (78) | 1 | NInventory::CTCInventoryBase::GetItemDescription(CWideString&, long, bool) const | NInventory_CTCInventoryBase.h:257 |
| vslot +4 | many | CIVCountedPointeeBase delete (dec [x+4] / call [vtbl+4]) | CDefPointer / CCountedPointer release |

## Globals / literals (read from the retail exe)
0x012537FC "PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", 0x012537B0 "UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE",
0x012537D8 "PC_TEXT_INSIDE_CONTAINER_TEMPLATE", 0x01247D70 "UI_TEXT_INSIDE_CONTAINER_TEMPLATE", 0x01230018 "UI_MISC_THINGS_DEF",
0x012501E4 "TEXT_GUI_PRICE", 0x01253798 "TEXT_GUI_MENU_REQUIRED", 0x0124E868 "TEXT_GUI_MENU_EXP_LEVEL", 0x0122D70C L"",
0x0122DE74 L"/", 0x0122F59C 0.5f, 0x013BCA24 CWideString::NullString (FABLE_RETAIL_CWIDESTRING_NULLSTRING),
0x013B86A0 CMainGameComponent::PMainGameComponent (+0x14 text bank -> GetTextBank(), +0x1C player manager -> GetPlayerManager()).

## 4. Members touched
- this (CTCInventoryTrade, engine/CTCInventoryTrade.h retail branch FABLE_RETAIL_CTCINVENTORYTRADE): +0x218 `bool InventoryTradeOpen`
  (header :194); +0x04 via base CTCBase `CThing* ParentThing` (CTCBase.h:139-143); GetBuildingThing() reads +0xB8 out of line.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED): +0x20 UsingComponents bitset<274>,
  +0x44 Components CVectorMap (begin +0x44 / end +0x48). GetTC<T> (non-const, LowerBound 0x40F020) and PeekTC(id,&p) const (0x4365B0).
  Interface ids (ETCInterfaceType.h): 0x53 TCI_SHOP, 0x68 TCI_HERO_EXPERIENCE, 0x16 TCI_INVENTORY_EXPERIENCE, 0x12 TCI_INVENTORY_CLOTHING.
- NUISystem::CUIMiscThingsDef (engine/NUISystem_CUIMiscThingsDef.h): +0x3C SpaceSeparator, +0x44 ColonSeparator, +0x4C OpenBracket,
  +0x50 CloseBracket, +0x80 TradeCostString, +0x84 TradeProfitString, +0x88 TradeLossString, +0x8C TradeAlreadyOwnsString,
  +0x90 TradeNumberInStockString, +0x94 TradeDeliveryString, +0x98 TradeNoDeliveryString, +0x9C TradeDaysString (all CWideString).
- CStockItemDef (engine/CStockItemDef.h, FABLE_RETAIL_CSTOCKITEMDEF): +0x28 DefaultPrice.
- CWeaponDef (engine/CWeaponDef.h): +0x2C Class (2 = heavy) -- as 0x5C90FC `weaponDef->Class == 2`.
- CTCHeroExperience (engine/CTCHeroExperience.h): +0x58 PHeroCombatDef via `GetPHeroCombatDef()` (FABLE_RETAIL_CTCHEROEXPERIENCE_GETPHEROCOMBATDEF; the inc/inc/dec copy).
- CHeroCombatDef (engine/CHeroCombatDef.h, FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS): +0x13C HeavyWeapons.StatThresholds (map<EHeroTrainableStatType,long>;
  node key +0x10, value +0x14; header->_M_left at +8 = begin).
- NUISystem::CUIState (engine/NUISystem_CUIState.h): +0x18 CRGBColour Colour; CRGBColour bytes B +0, G +1, R +2 (CRGBColour.h:8).
- CCountedPointer<CComponent> (engine/CCountedPointer.h): Data +0, Info +4 (frame -0x74/-0x70).
- No old draft exists -> no local models to replace. Use shared headers only.

## 5. Visibility (Q32 lesson)
- **CDefPointer locals must be unaliased**: retail keeps miscDef in esi for the whole body and REUSES its frame word [ebp-24h] for
  temporaries / line pointers, so VC7.1 must see that GetDef / GetSubDef only write through the out pointer. Copy the 0x5C90FC device:
  same-TU template bodies of `CDefClassBase::GetSubDef<T>` and `CDefinitionManager::GetDef<T>` under `#pragma auto_inline(off)`
  (they stay out of line: 0x42B312, 0x42AF3C, 0x42B1A6, 0x59350C). FABLE_RETAIL_DEF_CLASS_BASE / FABLE_RETAIL_CDEFINITIONMANAGER /
  FABLE_RETAIL_CGAMEDEFINITIONMANAGER as in 0x5C90FC.
- map<unsigned long,CUIState>::operator[] 0x42D5B1: out of line but body visible -> FABLE_RETAIL_STLP_MAP (0x5C90FC: "key temporary
  shares the int->float temporary's slot and the second key store is scheduled before the colour byte store" -- exactly retail
  0x5AB21C / 0x5ABF6B here).
- _Rb_global<bool>::_M_increment 0x4292C0 out of line (prefix ++it, as 0x5C90FC).
- CCountedPointer dtor -> out-of-line ReleaseData 0x4291DE: FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (0x5C90FC) or
  FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (the Trade siblings); try DECL_ONLY first.
- Neighbouring retail addresses: 0x5A9CBE ConstructTradeDescriptionWeapon (before, not landed), 0x5AC0EE/0x5AC0FE destval stubs,
  0x5AC10E ConstructActualBuyInventoryPC, 0x5AD571 ConstructTradeDescription (caller, after). None is called here.
- GetPlayerManager / GetTextBank / GetParentThing / StatGetInterfaceType / CCountedPointer::Get are inline (header bodies).

## 6. Sibling landed rows to copy idioms from
- **rebuild/src/compiled/00/5c/CTCInventoryWeapons_ConstructWeaponDescriptionContainer_005c90fc.cpp** (THE family model: CCountedPointer
  textComponent + CWideString text, misc def via GetGlobalIndexFromClassIndex/GetDef, GFIntToWideString chains, PC/UI if-else
  CreateComponent cross-jump, `container->AddChild(textComponent); line->SetValue(text,true,true)`, `text = CWideString::NullString`,
  GetStates()[0].Colour.B *= x, HeavyWeapons stat-threshold walk with GetTrainableStatLevel, GetPHeroCombatDef copy, same dtor tail
  incl. `and [ebp-x],0` on the GetPDefFromGlobalIndex def). Pragma: optimize("s",on) only there (its TU), macros listed in its header block.
- rebuild/src/compiled/00/5a/CTCInventoryTrade_RefreshBuyPossibility_005a9a87.cpp: `optimize("y",off)` + `("s",on)`, CTCINVENTORYTRADE,
  CTCINVENTORYBASE_BUILDINGTHING, CTHING_GETTC_EXPANDED, CTCSHOP_SELLPRICE, CTCINVENTORYITEM_CATEGORY, STLP_MAP ...
- rebuild/src/compiled/00/5a/CTCInventoryTrade_ConstructActualSpecialInventoryPC_005a8c66.cpp: same pragmas + CTCSHOP_SHOPTYPE,
  CCOUNTEDPOINTER_RELEASEDATA, CMAINGAMECOMPONENT_TEXTBANK / _PLAYERMANAGER, CPLAYERMANAGER, PLAYER.
- rebuild/src/compiled/00/5a/CTCInventoryTrade_BuyItem_005af64e.cpp: same pragmas + CTCINVENTORYBASE_VTABLE, DEF_CLASS_BASE,
  CDEFINITIONMANAGER, CRESOURCELIST, STLP_TREE_POSTINC.
- rebuild/src/compiled/00/5c/CTCInventoryWeapons_ConstructWeaponDescriptionDamage_005c7452.cpp (misc def + text concatenation).

## NAME_FIXES (landed spellings this row proves wrong)
0x005BEAD5 GetItemDescription -> NInventory::CTCInventoryBase::GetBoughtPrice; 0x005D99D0 GetInventoryCategory -> CTCInventoryItem::GetInventoryType;
0x005BC4C4 DrawGetEnvironment -> NInventory::CTCInventoryBase::GetBuildingThing; 0x006D6520 GetAbilityLevel -> CTCHeroExperience::GetTrainableStatLevel;
0x009AD390 ActualToUniqueIndex -> CDefinitionManager::GetGlobalIndexFromClassIndex; 0x0099B2C0 ConvertToCharString -> CWideString::ToCharString;
0x007726B0 GetStockQuantity -> CTCShop::GetQuantity (probable, one PDB slot + body evidence).
