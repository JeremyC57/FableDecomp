# DOSSIER 0x005A9CBE -- CTCInventoryTrade::ConstructTradeDescriptionWeapon (4714 bytes, 0x005A9CBE..0x005AAF27)

## 1. Retail shape (dis_all.txt)
- `push ebp ; mov ebp,esp ; sub esp,7Ch ; push ebx ; push esi ; mov ebx,ecx ; push edi` (0x005A9CBE..C8). Saved order ebx, esi, edi.
  Epilogue `pop edi ; pop esi ; pop ebx ; leave ; ret 10h` (0x005AAF21..25). Frame size 0x7C.
- thiscall, `ret 10h` = 4 stack args. NO SEH frame (no fs:0, no EH state stores) although CWideString / CCountedPointer / CDefPointer
  locals exist: build without /EHsc like every landed CTCInventoryTrade row. No /GS cookie.
- `leave`, `push 68h ; pop ebx` (0x005AAB44), `and [ebp+14h],0` zero stores, no alignment padding: size TU ->
  `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (same as every landed CTCInventoryTrade / CTCInventoryWeapons row,
  e.g. 0x005A9A87, 0x005A731F, 0x005C90FC).
- Dead argument slots are reused as temporaries: [ebp+10h] (arg3) and [ebp+14h] (arg4) hold CWideString / CCharString temps and
  map keys; the high byte [ebp+17h] holds the `special` bool (0x005AA38E). Arg2 [ebp+0Ch] is never read.

## 2. Identity
- Name: `?ConstructTradeDescriptionWeapon@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@JK_N0@Z`
  - ghidra_out/egor_pdb_names.tsv (Ego_r 0x0060D3CD, size to next 0x0060E67B = 4782 bytes ~ retail 4714)
  - ghidra_out/_fablewin_api.tsv / naming_stragglers/FableWin.exe.signatures.tsv (FableWin 0x023283BC, 13475 bytes / 3056 insns debug).
  - Manifest label `CTCInventoryTrade::ConstructTradeDescriptionWeapon` is CORRECT (two PDBs).
- Signature: `public: NUISystem::CComponent* CTCInventoryTrade::ConstructTradeDescriptionWeapon(long, unsigned long, bool, bool)`
  (`JK_N0`: back-ref 0 = `_N` bool; manifest functions.tsv agrees: `long,ulong,bool,bool`). Not virtual. Not const.
  Param use: arg1 [ebp+8] = item def index; arg2 [ebp+0Ch] unsigned long, unused; arg3 [ebp+10h] bool (buy/sell side: selects
  GetCurrentSellPrice when true and arg4 false, decides fade colours, gates "already owns" and the heavy-weapon requirement);
  arg4 [ebp+14h] bool (short form: price + item description, early return). Parameter names are not in the PDB: descriptive.
- Sole caller: CTCInventoryTrade::ConstructTradeDescription 0x005AD571 (call 0x005AD598, after `CTCInventoryItem::IsWeapon(def)`;
  forwards all four args unchanged; ConstructTradeDescriptionClothing 0x005AAF28 is the IsClothing 0x005D9C60 twin, NOT landed).
  Not in a vtable. Neither the caller nor the twin is landed.
- Not declared in engine/CTCInventoryTrade.h: add `NUISystem::CComponent* ConstructTradeDescriptionWeapon(long, unsigned long, bool, bool);`
  under a new nested opt-in in the overlay (e.g. FABLE_RETAIL_CTCINVENTORYTRADE_TRADEDESCRIPTION).

## 3. Callees (direct; count; landed file; PDB / name to use) -- `python3 $W/tools/calls.py`
| addr | n | landed row | name to use (PDB) | flag |
|---|---|---|---|---|
| 0041E5F2 | 1 | 00/41/NUISystemCManager_GetInstance_0041e5f2.cpp | NUISystem::CManager::GetInstance (static) | |
| 0041DB1D | 13 | 00/41/NUISystemCManager_CreateComponent_0041db1d.cpp | ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z | |
| 0099EBF0 / 0099EAE0 | 17 / 23 | 00/99/CCharString_* | CCharString(const char*, long=-1) / ~CCharString | |
| 00429C6F | 11 | 00/42/CCountedPointerNUISystemCComponent_Reset_00429c6f.cpp | ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z | |
| 004291DE | 1 | 00/42/...ReleaseData_004291de.cpp | CCountedPointer<CComponent>::ReleaseData (the dtor; FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA) | |
| 0099AED0 | 1 | 00/99/CWideString_CWideString_0099aed0.cpp | CWideString::CWideString() | |
| 0044C6B0 | 2 | 00/44/CGameDefinitionManager_Get_0044c6b0.cpp | ?Get@CGameDefinitionManager@@SIAAV1@XZ | reloc binding noise `?Resolve@@YGPAXH@Z` |
| 009AD390 | 1 | 00/9a/COptimisedPrimitive_ActualToUniqueIndex_009ad390.cpp | CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, bool) (0x005C90FC spelling) | landed FILE name wrong (ActualToUniqueIndex); binding says ?GetDefIndex@CDefinitionManager@@QAEJABVCCharString@@_N@Z -- use the 0x005C90FC call spelling |
| 0042B312 | 1 | 00/42/CDefinitionManager_GetDef0042b312_0042b312.cpp | ??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@@Z | |
| 005BC4C4 | 1 | 00/5b/global_DrawGetEnvironment_005bc4c4.cpp | ?GetBuildingThing@CTCInventoryBase@NInventory@@QBEPAVCThing@@XZ (header FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING) | landed "DrawGetEnvironment"/GetOwner propagated (header says NAME_FIXES) |
| 00410DE0 | 5 | 00/41/STLbitset_test_00410de0.cpp | std::bitset<274>::test (FABLE_RETAIL_STLP_BITSET_TEST_DECL) -- HasTC inside expanded GetTC/PeekTC | |
| 0040F020 | 3 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<..>::LowerBound non-const => CThing::GetTC (shop 0x53, hero exp 0x68, inv exp 0x16) | |
| 004365B0 | 2 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<..>::LowerBound const => CThing::PeekTC (hero creature -> inventory) | |
| 007743B0 | 1 | 00/77/CTCShop_GetCurrentSellPrice_007743b0.cpp | ?GetCurrentSellPrice@CTCShop@@QBEJJJ@Z (FABLE_RETAIL_CTCSHOP_SELLPRICE) | |
| 007742C0 | 1 | 00/77/CTCShop_GetCurrentBuyPrice_007742c0.cpp | ?GetCurrentBuyPrice@CTCShop@@QBEJJJ@Z | |
| 006869D0 | 2 | 00/68/CTCBase_GetDefinitionManager_006869d0.cpp | ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ | |
| 009AD9E0 | 1 | 00/9a/CDefinitionManager_GetPDefFromGlobalIndex_009ad9e0.cpp | ?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z (hidden result [ebp-2Ch]) | |
| 0059350C | 1 | 00/59/CDefClassBase_GetSubDefCStockItemDef_0059350c.cpp | ??$GetSubDef@VCStockItemDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCStockItemDef@@@@@Z | |
| 00405F00 | 11 | 00/40/global_GFIntToWideString_00405f00.cpp | ?GFIntToWideString@@YI?AVCWideString@@J@Z (__fastcall: ecx = result, edx = value) | |
| 009C95E0 | 11 | 00/9c/NGameTextCDataBank_GetTextBySymbol_009c95e0.cpp | ?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z | |
| 0099BE70 | 34 | 00/99/global_operator_0099be70.cpp | ??H@YI?AVCWideString@@ABV0@0@Z (fastcall op+) | |
| 0099BFF0 | 1 | 00/99/global_operator_0099bff0.cpp | ??H@YI?AVCWideString@@PB_WABV0@@Z (L"/" + wide) | |
| 0099B7D0 | 11 | 00/99/CWideString_operatorassign_0099b7d0.cpp | CWideString::operator=(const CWideString&) | binding noise FrontEndWideString |
| 0099B800 | 1 | 00/99/CWideString_operatorwchartconst_0099b800.cpp | ??4CWideString@@QAEABV0@PBG@Z (= L"") | |
| 0099B8D0 | 5 | 00/99/CWideString_operator_0099b8d0.cpp | ??YCWideString@@QAEABV0@ABV0@@Z (+=) | |
| 0099B510 | 59 | 00/99/CWideString_Destructor_0099b510.cpp | ~CWideString | |
| 0099B2C0 | 8 | 00/99/CWideString_ConvertToCharString_0099b2c0.cpp | ?ConvertToCharString@CWideString@@QBE?AVCCharString@@XZ | manifest label "ToCharString" is wrong; use ConvertToCharString |
| 0099B0F0 | 2 | 00/99/CWideString_operator_0099b0f0.cpp | ??9CWideString@@QBE_NABV0@@Z (!=) | |
| 0042D5B1 | 14 | 00/42/CComponentStateMap_operator_0042d5b1.cpp | std::map<unsigned long,NUISystem::CUIState>::operator[] (FABLE_RETAIL_STLP_MAP makes its body visible, still out of line) | landed file "CComponentStateMap" local spelling |
| 00BFEA70 | 11 | 00/bf/global_ftol2_00bfea70.cpp | __ftol2 (from `byte *= 0.5f`) | |
| 00449960 / 00487DD0 | 2/2 | 00/44, 00/48 | ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ / ?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ | |
| 005D99D0 | 2 | 00/5d/global_GetInventoryCategory_005d99d0.cpp | **?GetInventoryType@CTCInventoryItem@@SI?AW4ETCInterfaceType@@J@Z** (engine/CTCInventoryItem.h:52) | landed file / manifest say GetInventoryCategory: WRONG (that is 0x005D97B0) |
| 00772490/0510/0590 | 1 each | 00/77/CTCShop_Is*Shop | IsBarberShop / IsTattooShop / IsTitleShop const (FABLE_RETAIL_CTCSHOP_SHOPTYPE) | |
| 005BEAD5 | 1 | 00/5b/CTCInventoryBase_GetItemDescription_005bead5.cpp | **?GetBoughtPrice@CTCInventoryBase@NInventory@@QBEJJ@Z** (header NInventory_CTCInventoryBase.h:77) | landed FILE/func name GetItemDescription is WRONG |
| 005C7F07 / 005C7D0D / 005C7B10 | 1 each | 00/5c/CTCInventoryWeapons_GetItemDescription{Damage,Weight,NoDamageNorWeight}_* | QBEXAAVCWideString@@J@Z / J@Z / J_N@Z (FABLE_RETAIL_CTCINVENTORYWEAPONS_ITEMDESCRIPTION) | |
| 005BDF08 | 1 | 00/5b/CTCInventoryBase_GetNumberOfItemsOfTypeInInventory_005bdf08.cpp | ?GetNumberOfItemsOfTypeInInventory@CTCInventoryBase@NInventory@@QBEJJ@Z (FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED) | |
| 007726B0 | 1 | 00/77/CTCShop_GetStockQuantity_007726b0.cpp | CTCShop::GetStockQuantity(long) (landed row: manifest label GetMaxStock there was propagated) | not in CTCShop.h: add to overlay |
| 00772720 | 1 | 00/77/CTCShop_GetMaxStock_00772720.cpp | ?GetMaxStock@CTCShop@@QBEJJ@Z | not in CTCShop.h |
| 00772790 | 1 | 00/77/CTCShop_GetDaysTillRestock_00772790.cpp | CTCShop::GetDaysTillRestock(long) | not in CTCShop.h |
| 005D97F0 | 1 | 00/5d/CTCInventoryItem_IsWeapon_005d97f0.cpp | ?IsWeapon@CTCInventoryItem@@SI_NJ@Z | |
| 0042AF3C | 1 | 00/42/CDefinitionManager_GetDefT_0042af3c.cpp | ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z | |
| 0042B1A6 | 1 | 00/42/CDefClassBase_GetSubDefCWeaponDef_0042b1a6.cpp | ??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWeaponDef@@@@@Z | |
| 006D6520 | 1 | 00/6d/CTCInventoryAbilities_GetAbilityLevel_006d6520.cpp | ?GetTrainableStatLevel@CTCHeroExperience@@QBEJW4EHeroTrainableStatType@@@Z (out of line) | landed FILE name GetAbilityLevel wrong |
| 0056F26F | 1 | 00/56/CTCInventoryExperience_GetExperienceStatName_0056f26f.cpp | CTCInventoryExperience::GetExperienceStatName(long) -> CWideString (hidden result) | |
| 004292C0 | 1 | 00/42/stdRbtreebase_Rbtreeincrement_004292c0.cpp | _Rb_global<bool>::_M_increment (cdecl, `push ebx ; call ; pop ecx`) = map const_iterator ++ | |
Virtual calls: description (+0xEC) = CComponent::AddChild(CCountedPointer<CComponent>&) slot 59; text line (+0x240) =
CText::SetValue(const CWideString&, bool, bool) slot 144 (engine/NUISystem_CText.h:23); (+0x148) = CComponent::GetStates() slot 82;
inventory (+0x138) = NInventory::CTCInventoryBase::GetItemDescription(CWideString&, long, bool) const slot 78;
inventory (+0x54) = CTCBase::GetInterfaceType() const slot 21 (== TCI_INVENTORY_WEAPONS 19 = 0x13).

## 4. Members and globals
- this+0x218 `bool InventoryTradeOpen` (engine/CTCInventoryTrade.h:194, FABLE_RETAIL_CTCINVENTORYTRADE; Ego_r +0x268). Read 9x to pick
  "PC_..." (true) v "UI_..." (false) templates. (Read through `[ebp-0Ch]`=this each time; not cached.)
- this+0x4 ParentThing (CTCBase, FABLE_RETAIL_CTCBASE): GetTC(0x68) and GetTC(0x16) at 0x005AAB3E / 0x005AAB7C (re-read each time).
- this->GetBuildingThing() (+0xB8 inside the out-of-line 0x005BC4C4).
- CThing +0x20 bitset<274>, +0x44 Components CVectorMap (engine/CThing.h, FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED).
- CMainGameComponent::PMainGameComponent 0x013B86A0: +0x14 TextBank (GetTextBank(), FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK),
  +0x1C PlayerManager (PeekPlayerManager(), FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER).
- NUISystem::CUIMiscThingsDef (engine/NUISystem_CUIMiscThingsDef.h): +0x3C SpaceSeparator, +0x44 ColonSeparator, +0x4C OpenBracket,
  +0x50 CloseBracket, +0x80 TradeCostString, +0x84 TradeProfitString, +0x88 TradeLossString, +0x8C TradeAlreadyOwnsString,
  +0x90 TradeNumberInStockString, +0x94 TradeDeliveryString, +0x98 TradeNoDeliveryString, +0x9C TradeDaysString (all CWideString).
- CStockItemDef +0x28 DefaultPrice (engine/CStockItemDef.h, FABLE_RETAIL_CSTOCKITEMDEF).
- CWeaponDef +0x2C Class (EWeaponClass; `== 2` heavy) (engine/CWeaponDef.h, needs FABLE_RETAIL_DEF_CLASS_BASE).
- CTCHeroExperience +0x58 PHeroCombatDef (GetPHeroCombatDef() FABLE_RETAIL_CTCHEROEXPERIENCE_GETPHEROCOMBATDEF);
  CHeroCombatDef +0x13C HeavyWeapons.StatThresholds (map<EHeroTrainableStatType,long>; FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS,
  FABLE_RETAIL_EHEROTRAINABLESTATTYPE); node key +0x10, value +0x14.
- CUIState +0x18 Colour (CRGBColour {B,G,R,A}): +0x18 = B, +0x19 = G, +0x1A = R (engine/CRGBColour.h:8).
- Globals: CWideString::NullString 0x013BCA24 (FABLE_RETAIL_CWIDESTRING_NULLSTRING); 0.5f 0x0122F59C (compiler literal);
  L"" 0x0122D70C; L"/" 0x0122DE74.
- Strings: "PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT" 0x012537FC, "UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE"
  0x012537B0, "PC_TEXT_INSIDE_CONTAINER_TEMPLATE" 0x012537D8, "UI_TEXT_INSIDE_CONTAINER_TEMPLATE" 0x01247D70, "UI_MISC_THINGS_DEF"
  0x01230018, "TEXT_GUI_PRICE" 0x012501E4, "TEXT_GUI_MENU_REQUIRED" 0x01253798, "TEXT_GUI_MENU_EXP_LEVEL" 0x0124E868.
- Interface ids (engine/ETCInterfaceType.h): TCI_SHOP 83 (0x53), TCI_HERO_EXPERIENCE 104 (0x68), TCI_INVENTORY_EXPERIENCE 22 (0x16),
  TCI_INVENTORY_WEAPONS 19 (0x13).
- No draft, so no locally modelled classes yet: use the shared headers above, do NOT model CTCInventoryTrade / CThing / CTCShop /
  CUIMiscThingsDef locally (several older siblings, e.g. 0x005A731F, still do: do not copy that).

## 5. Callee visibility (what retail's compiler saw)
- GetDef<CUIMiscThingsDef> 0x0042B312, GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CStockItemDef> 0x0059350C, GetSubDef<CWeaponDef>
  0x0042B1A6: out of line, but VISIBLE same-TU noinline bodies are the lever (0x005C90FC lines 88-118: `#pragma auto_inline(off)`
  template bodies of CDefClassBase::GetSubDef / CDefinitionManager::GetDef). Evidence here: miscDef's address is taken by GetDef,
  yet after the call retail keeps miscDef in edi and reuses its slot [ebp-24h] for other temps (0x005AA12C/0x005AA32A), and the
  final release is `test edi,edi ; dec [edi+4]` (0x005AAEFE): the out-pointer must be known not to escape.
- map<unsigned long,CUIState>::operator[] 0x0042D5B1: FABLE_RETAIL_STLP_MAP (body visible, still called): its key temporary shares
  the int->float temp slot ([ebp+14h] / [ebp-44h] / [ebp-70h]) exactly as 0x005C90FC.
- CVectorMap::LowerBound 0x0040F020 / 0x004365B0 and bitset::test 0x00410DE0: called (header declarations).
- _Rb_global<bool>::_M_increment 0x004292C0: called (map const_iterator ++; header STLport).
- CDefPointer releases: FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE gives both retail forms (`dec [ecx+4] ; jne` and the
  hidden-return temporary's `add eax,4 ; dec [eax] ; cmp [eax],0` + `and [ebp-2Ch],0`, 0x005AAEE2) -- 0x005C90FC defines it.
- CCountedPointer dtor = out-of-line ReleaseData 0x004291DE (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA, or _DECL_ONLY as 0x005C90FC).
- Small getters inline: CMainGameComponent::Get / GetTextBank / PeekPlayerManager, CTCHeroExperience::GetPHeroCombatDef
  (`inc ; inc ; dec`, 0x005AABCB), CCountedPointer::Get / operator->.

## 6. Sibling / model rows
- **0x005C90FC CTCInventoryWeapons::ConstructWeaponDescriptionContainer** (rebuild/src/compiled/00/5c/..._005c90fc.cpp) -- THE model:
  same misc-def lookup, same PC/UI if-else text creation, same GetStates()[0].Colour.x *= f fades, same HeavyWeapons StatThresholds
  walk with GetPHeroCombatDef and GetTrainableStatLevel, same NullString idiom, same no-EH size TU. Copy its macro block (lines
  23-82) and its same-TU GetSubDef/GetDef bodies. NB it uses `#pragma optimize("s", on)` only (no "y" off line shown in grep).
- 0x005A9A87 CTCInventoryTrade::RefreshBuyPossibility (00/5a/): `optimize("y",off)`+`("s",on)`, shared headers CTCInventoryTrade.h /
  CThing.h / CTCShop.h / CTCInventoryItem.h / CMainGameComponent.h / CPlayerManager.h / CPlayer.h, the hero PeekTC idiom
  (`CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()` + local `class CThingPlayerCreature :
  public CThing {};`), `CTCShop* shop = 0; GetBuildingThing()->GetTC(TCI_SHOP, &shop);`.
- 0x005C7F07 / 0x005C7D0D / 0x005C7B10 CTCInventoryWeapons::GetItemDescription* (00/5c/): `optimize("y",off)+("s",on)`.
- 0x005AE9DE RefreshBuyDescriptionsPC, 0x005A8C66 ConstructActualSpecialInventoryPC (00/5a/): same pragmas, FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA.
- Family (UI builders): /opt/fable_lane/w22/agent4007 (ConstructQuestList, LESSONS.md: `||` placement, call-v-expand by frequency,
  tools/cd.sh full call comparison, tools/bt.sh branch targets).
