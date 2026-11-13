# DOSSIER 0x0058807D -- CTCInventory::OpenPCInventoryMenu (4997 bytes)

Annotated listing (strings + callee names): $W/scout/ann.txt (made by $W/scout/ann.py). $W/prior/ and $W/hdr/ are EMPTY: no earlier draft, no overlay.

## 1. Identity (label is RIGHT)
- `void CTCInventory::OpenPCInventoryMenu()`, leaf `?OpenPCInventoryMenu@CTCInventory@@QAEXXZ` (public, thiscall, no params, void).
- Source 1: ghidra_out/egor_pdb_names.tsv `005f28c1 ?OpenPCInventoryMenu@CTCInventory@@QAEXXZ` (Ego_r address, same name set as
  ?OpenPCSkillsMenu / ?OpenPCMapMenu siblings).
- Source 2: callers. xref: 0x0058EA07 (CTCInventory::OpenInventoryShortcut 0x0058E99F, landed, cases 0/1/2 -> OpenPCInventoryMenu) and
  0x0062C747 (CWorld::EAOpenPCInventoryMenu 0x0062C700, event handler; ?EAOpenPCInventoryMenu@CWorld@@QAEXPBVCGameEvent@@@Z in Ego_r).
  Header rebuild/include/engine/CTCInventory.h already declares `void OpenPCInventoryMenu(); // 0x0058807D` (non-optional, line ~308).
- Body agrees: "PC_INVENTORY_LIST" (0x01250CA8), CurrentOpenMenu = 3.

## 2. Retail shape
- thiscall, plain `ret` (0x00589401). `push ebp ; lea ebp,[esp-78h] ; sub esp,8Ch ; push ebx ; push esi ; push edi` -- biased-ebp frame
  pointer => `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (every CTCInventory sibling uses exactly this pair).
  Epilogue `pop edi ; pop esi ; pop ebx ; add ebp,78h ; leave ; ret`.
- NO SEH / no fs:0, no /GS cookie (siblings OpenPCSkillsMenu / OpenPCOptionsMenu also have none) -- do NOT add /EHsc.
- Size idioms: `push 13h ; pop edi` constants, `push ecx ; mov ecx,esp` for by-value CCharString args, `or edi,-1` reused as the
  CCharString length -1. `repe cmpsb` with ecx=0x12 at 0x005880F0 = the inline CCharString::operator==(const char*) empty-string branch.
- Temp-destructor flag word at [ebp+70h] (zeroed at entry from ebx=0, `or [ebp+70h],1..20h`), driven by the 6-term `||/&&` with
  CCharString temporaries at 0x0058883D..0x005889C4 -- it MUST be one expression with the temporaries inside it.

## 3. Callees (all landed; names agree with PDB unless flagged)
| addr | landed name / file | note |
|---|---|---|
| 0x00587A41 | CTCInventory::DeleteMenuComponents (00/58/..._00587a41) | first call |
| 0x0041E5F2 | NUISystem::CManager::GetInstance (x3) | |
| 0x0041DB1D | CManager::CreateComponent(const CCharString&, bool) | |
| 0x004115A0 | CBasicString<char>::operator== | from inline CCharString==(const char*) |
| 0x0099EBF0 / 0x0099EAE0 | CCharString(const char*, long) x61 / ~CCharString x8 | |
| 0x00449960 | CPlayerManager::GetMainPlayer (x2) | `this` = [0x013B86A0]+0x1C (CMainGameComponent player manager) |
| 0x00487DC0 | CPlayer::GetPControlledCreature | gold block |
| 0x00487DD0 | CPlayer::PeekPControlledCreature | TC block |
| 0x00410DE0 | std::bitset<274>::test (x5) | inline CThing::HasTC (FABLE_RETAIL_STLP_BITSET_TEST_DECL) |
| 0x0040F020 / 0x004365B0 | CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound non-const x4 / const x1 | inline GetTC / PeekTC |
| 0x00405F00 | GFIntToWideString(long) `__fastcall` (ecx=result, edx=value) x5 | ?GFIntToWideString@@YI?AVCWideString@@J@Z |
| 0x0099B510 / 0x0099B720 / 0x0099B8D0 | ~CWideString x13 / CWideString(const CWideString&) x4 / operator+= x8 | |
| 0x005BF482 | NInventory::CTCInventoryBase::GetAllItems(vector<pair<long,long>>&) const x3 | |
| 0x005D9970 / 0x005D97F0 / 0x005D9830 | CTCInventoryItem::IsBook / IsWeapon / IsExpression (static __fastcall, long) | IsWeapon NOT declared in CTCInventoryItem.h KINDS opt-in (IsBook/IsExpression are) -- add `static bool __fastcall IsWeapon(long); // 0x005D97F0` (?IsWeapon@CTCInventoryItem@@SI_NJ@Z) |
| 0x00593139 | vector<pair<long,long>>::erase(iterator) | FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE |
| 0x006869D0 | CTCBase::GetDefinitionManager() const (x8) | returns const CGameDefinitionManager& |
| 0x0042AF3C | CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const | |
| 0x0042B040 | GetDef<CThingObjectDef> | on CMainGameComponent::PeekDefinitionManager (0x00415FBC) |
| 0x0042B248 / 0x005935C4 / 0x0057FF92 / 0x005936C2 | CDefClassBase::GetSubDef<CAugmentationDef / CHeroSuitDef / CAppearanceModifierDef (x2) / CAbilityDef> const | |
| 0x005BDF08 | CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) const | |
| 0x009AD410 | CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const x6 | |
| 0x009AD9E0 | CDefinitionManager::GetPDefFromGlobalIndex(long) -> CDefPointer<const CDefClassBase> (QBE) | |
| 0x004292C0 | _Rb_global<bool>::_M_increment (cdecl) | map iterator ++ (stlport/fable_map.h) |
| 0x005D9A10 / 0x005D9AA0 | CTCInventoryItem::GetHairCardDef / GetTattooCardDef (static __fastcall, CDefPointer by hidden ptr) | FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS |
| 0x00581765 | CTattooDef::IsInvalidCustomTattoo() const | FABLE_RETAIL_CTATTOODEF |
| 0x005CC772 | CTCInventoryStats::GetXString() static __fastcall (x4) | |
| 0x00BFEA1A / 0x00BFEA14 | operator new x7 / free (vector dtor) | |
| 0x0042BE50 | NUISystem::CAction::CAction x4 | |
| 0x0042AA29 | CCountedPointer<CAction>(T*) -- landed file name is the bogus `CCountedPointer_00429f43`; real: ??0?$CCountedPointer@VCAction@NUISystem@@@@QAE@PAVCAction@NUISystem@@@Z (as OpenPCSkillsMenu cites) | FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR |
| 0x0042B646 | list<CActionParam*>::push_back x11 (landed file `global_ListPushFrontWrapper...`, misnamed) | FABLE_RETAIL_STLP_LIST |
| 0x00429160 | CCountedPointer<CAction>::ReleaseData x4 | FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA |
| 0x00486024 | vector<CComponent*>::push_back | |
Virtual slots: CComponent +0x0C FindChild(CCharString by value), +0x150 GetName (CCharString by hidden ptr), +0x1F0 AddPositionChild,
+0x114 AddPreAction(CCountedPointer<CAction>&), +0x170 MakeUnavailable (slot 92), +0xAC Initialise, +0xC0 ChangeState(5);
CViewport +0x238 AddFakeChild (slot 142); CText +0x238 GetValue (slot 142, const CWideString&), +0x240 SetValue(const CWideString&,bool,bool)
(slot 144); CObservable (manager) +8 AddObserver(CObserver*), CObserver (component+4) +0x0C ObserveEvent(0x19).

## 4. Members / layouts
- CTCInventory (engine/CTCInventory.h, FABLE_RETAIL_CTCINVENTORY + CTCBASE + CTCINVENTORYBASE): ParentThing +0x04, Components
  (vector<CComponent*>) +0x0C..+0x10 (`[ebp-4]` = &Components), CurrentOpenMenu +0x21C (typed enum only under
  FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU; store `(CURRENT_OPEN_MENU)3`).
- CThing UsingComponents bitset +0x20, Components CVectorMap +0x44 (CThing.h, FABLE_RETAIL_CTHING [+ FABLE_RETAIL_CTHING_GETTC_EXPANDED]).
- CTCHeroStats::Money +0x3C (CTCHeroStats.h, FABLE_RETAIL_CTCHEROSTATS [+ _GETMONEY as the trade row]); Ego_r says +0x44 (Ego_r is +8 here).
- CHeroSuitDef::SuitParts map<EClothingSuitPart,long> +0x28 (CHeroSuitDef.h); node value `second` at node+0x14.
- CAppearanceModifierDef (CAppearanceModifierDef.h): CoversBodyAreaFlags +0x40 (tests 0x80, 0x100, 0x10), SuitPart +0x4C (1..5). Ego_r
  offsets are +8 (vector 16 v 12).
- CHairCardDef +0x28 = the def index passed to GetPDefFromGlobalIndex (check CHairCardDef.h for its name).
- CActionParamSLONG (NUISystem_CActionParamSLONG.h): vtable 0x012366F4, type 3 at +4, Value +8; inline ctor after new(0xC).
- CAction: ActionType at +0, UIActionParams list at +4. Action types: 69 CONSTRUCT_CLOTHING_LIST, 68 CONSTRUCT_WEAPONS_LIST,
  70 CONSTRUCT_ITEMS_LIST, 240 CONSTRUCT_PC_STYLE_CARDS_LIST (NUISystem_EActionType.h).
- CComponent's CActionParam base is at +0x18 (`lea eax,[esi+18h]`): `&static_cast<CActionParam&>(*entry)` as OpenPCSkillsMenu.
- Globals: 0x013B86A0 CMainGameComponent instance (CMainGameComponent::Get(); +0x1C player manager -> FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER).

## 5. Visibility / opt-ins
- Inline-expanded here: CCharString==(const char*) (FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ), HasTC/GetTC/PeekTC
  (FABLE_RETAIL_CTHING_GETTC_EXPANDED; bitset::test and LowerBound stay out of line), CDefPointer ctor/dtor (forceinline),
  vector<pair> ctor/dtor (`free` direct), CActionParamSLONG ctor, map begin/end.
- Out of line: vector erase (FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE), vector push_back (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE or
  the OpenPCSkillsMenu spelling), list push_back (FABLE_RETAIL_STLP_LIST), CCountedPointer called ctor + ReleaseData, _M_increment.
- Two CDefPointer release shapes: `dec [ecx+4] ; jne ; call [eax+4]` (no null store) for GetDef/GetSubDef out-param locals, and
  `add eax,4 ; dec [eax] ; cmp [eax],0 ; ... ; mov [x],0` for the values RETURNED by GetHairCardDef / GetTattooCardDef /
  GetPDefFromGlobalIndex (header CDefPointer.h notes this "temporary's" form; the trade row 0x005A8C66 holds them in named locals).

## 6. Siblings to copy idioms from
- rebuild/src/compiled/00/58/CTCInventory_OpenPCSkillsMenu_00587bff.cpp -- SAME skeleton (find-or-create loop, frame AddPositionChild
  chain, CAction pre-actions, scroll/back/close tail, ChangeState(5)); pragmas y-off + s-on; macro list to copy.
- rebuild/src/compiled/00/58/CTCInventory_OpenPCOptionsMenu_0058af10.cpp, ..._OpenPCMapMenu_00589a30.cpp, ..._OpenPCStatusMenu_0058a763.cpp.
- rebuild/src/compiled/00/5a/CTCInventoryTrade_ConstructActualSpecialInventoryPC_005a8c66.cpp -- the item half: GetAllItems into a local
  vector<pair<long,long>>, erase loops, GetHairCardDef/GetTattooCardDef/IsInvalidCustomTattoo, GetTC/PeekTC expanded, GetMainPlayer,
  GFIntToWideString/GetXString/CWideString +=, macro list (CTHING_GETTC_EXPANDED, STLP_VECTOR_ERASE_OUTOFLINE, CTCINVENTORYITEM_CARDDEFS,
  CTATTOODEF, CMAINGAMECOMPONENT_PLAYERMANAGER, PLAYER, CPLAYERMANAGER, CTCHEROSTATS).
- Unlanded neighbours 0x005850FD / 0x00589402 (labelled GetStyleCardsListUIComponent) share the erase / card-def code.
