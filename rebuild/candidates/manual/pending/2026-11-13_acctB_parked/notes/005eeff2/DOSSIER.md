# DOSSIER -- 0x005EEFF2 (5619 bytes, ends 0x005F05E4) -- identity scout
Companion: $W/OUTLINE.md (frame map + block-by-block pseudo-C++), $W/scout/ann.txt (listing with strings and callee names).

## 1. Retail shape
- `__thiscall`, `ret 8` (two args), returns CComponent* in eax (`mov eax,[ebp+78h]`).
- Biased frame `push ebp ; lea ebp,[esp-70h] ; sub esp,0F4h`, saves ebx/esi/edi; epilogue `add ebp,70h ; leave ; ret 8`.
- No SEH (no fs:0) despite many destructors => NO /EHsc. No /GS. Size idioms (`xor esi,esi; inc esi`, `and [x],0`, `or ebx,-1`) =>
  `#pragma optimize("s", on)`. Copy the TU recipe of ConstructScreen 0x005F519F / ContructItemLists 0x005F077F / ConstructPCItemLists
  0x005F1526 (all biased frames): `#pragma optimize("s", on)` only, `// Flags:` empty. (OpenInventory 0x005F05E5 / AddAbilityToCorrectList
  0x005EEFB7 add `optimize("y", off)`, but they have plain `mov ebp,esp` frames; the biased-frame rows do not use "y" off.)

## 2. Identity -- manifest label `_gapscan::sub_005eeff2` is a placeholder; real name:
**NUISystem::CComponent* CTCInventoryAbilities::GetMagicListUIComponent(long& count, NUISystem::CComponent* parent)**
Leaf `?GetMagicListUIComponent@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@AAJPAV23@@Z` (public, non-virtual, non-const).
Evidence (independent):
 a. Ego_r.pdb (egor_pdb_names.tsv 0x0057DCEB) and FableWin.pdb (FableWin.exe.signatures.tsv 0x01EBF630, 11887 debug bytes; the
    sister GetExpressionsListUIComponent is 2649 debug bytes ~ retail 1296) give this leaf; in Ego_r it follows ForceAbilityLevelTo
    (0x0057DCB8), and retail has ForceAbilityLevelTo 0x005EEF84 -> AddAbilityToCorrectList 0x005EEFB7 -> 0x005EEFF2.
 b. The only caller, landed ConstructScreen 0x005F519F (call at 0x005F5284), is written `GetMagicListUIComponent(magicCount,parent)`;
    rebuild/include/engine/CTCInventoryAbilities.h:60 declares it `//005eeff2` under FABLE_RETAIL_CTCINVENTORYABILITIES_SCREEN.
 c. Behaviour matches (count out-param = number of owned abilities; arg2 searched for "UI_SCROLLING_VIEWPORT_TEXT_AREA"; returns the
    "UI_MAGIC_CATEGORIES" component); stdmap_operator_005f9ea5.cpp names 0x005EEFF2 as the user of UnassignedIcons/AbilitiesMenuEntries.
Parameter names: header uses `count`, `parent`.

## 3. BIG ASSET: a full semantic draft already exists
rebuild/src/compiled/00/5f/CTCInventoryAbilities_ConstructScreen_005f519f.cpp lines 199-384 hold a complete same-TU body of
GetMagicListUIComponent (ungraded for this address). Start from it, but fix these retail differences (each verified in the listing):
 1. The three category-list builds (0x5EF600 / 0x5EF75C / 0x5EF8B5) and the three category finishes (0x5F005E / 0x5F0213 / 0x5F03CB)
    are UNROLLED straight-line code in retail (distinct key stores, distinct string pushes, no loop counter) -- the draft's
    `for(n<3)` loops over arrays must be written out (order: SURROUND, ATTACK, PHYSICAL for the build; ATTACK, SURROUND, PHYSICAL
    for the finish, each finish using its own count: attack[ebp+34h], surround[ebp+38h], physical[ebp+64h]).
 2. `isAssigned` loop -> `std::find(AssignedAbilities.begin(), AssignedAbilities.end(), *it) == AssignedAbilities.end()`
    (call 0x005F8150 at 0x5EFD11: ecx=first, edx=last, push &val; compared against [this+0x180]).
 3. `displayName = ...; caption = displayName;` -> chained `caption = displayName = experience->GetExperienceAbilityName(*it);`
    (0x5EFB7A: operator= on displayName, its return value pushed straight into caption's operator=).
 4. Stack arrays emptyWide/spaceWide/levelPrefix -> string LITERALS: retail pushes globals L"" 0x0122D70C, L" " 0x0122D820,
    L" - " 0x0122E070 (CWideString(const wchar_t*) 0x0099B6B0). Spell `CWideString(L"")` as CTCInventoryExperience_GetXString_0056ea4e does.
 5. `received`: retail `cmp size,total ; ja -> total ; else re-read size` (0x5EF09B) -- try std::min<unsigned long> first, then the ternary.
 6. The CategoriesLists / UnassignedIcons / AbilitiesMenuEntries clears and finds are out-of-line calls (see section 5).

## 4. Callees (retail target -> landed row -> name; FIX = landed/manifest spelling wrong, proposed for NAME_FIXES.tsv)
| addr | xN | landed row file | correct name (evidence) |
|---|---|---|---|
| 0x00686D60 | 1 | CTCBase_PeekComponent_00686d60.cpp | CTCBase::PeekComponent (then +0x14 GetTextBank) |
| 0x005F88ED | 1 | global_stdCircularListConstruct_005f88ed.cpp | FIX: std::_List_base<EHeroAbility>::_List_base(const allocator&) (body: and [esi],0; malloc 0xC sentinel; self-link; ret 4) |
| 0x0041E5F2 | 2 | NUISystemCManager_GetInstance_0041e5f2.cpp | NUISystem::CManager::GetInstance (2nd call at 0x5EF4D2 for "UI_LIST_MAGIC_ACTIONS": NOT the cached `ui`) |
| 0x005EE3B3 | 1 | CTCInventoryAbilities_GetMiscThingsDef_005ee3b3.cpp | CTCInventoryAbilities::GetMiscThingsDef (hidden ret) |
| 0x005F88C8 | 1 | global_LinkedListInsertNodeBefore_005f88c8.cpp | FIX: std::list<EHeroAbility>::insert(iterator, const EHeroAbility&) (from push_back) |
| 0x005F8F89 | 2 | stdvector_erase_005f8f89.cpp | vector<EHeroAbility>::erase(iterator,iterator) (from clear()) |
| 0x005F9BCD | 4 | stdvector_pushback_005f9bcd.cpp | vector<EHeroAbility>::push_back |
| 0x0099EBF0 / 0x0099EAE0 | 45/32 | CCharString_Constructor / CCharString_CCharString_0099eae0 | CCharString::CCharString(const char*, long=-1) / ~CCharString |
| 0x0041DB1D | 28 | NUISystemCManager_CreateComponent_0041db1d.cpp | NUISystem::CManager::CreateComponent(const CCharString&, bool) |
| 0x00429C15 / 0x00429C6F / 0x004291DE | 22/6/23 | CCountedPointerNUISystemCComponent_* | CCountedPointer<CComponent> ctor(T*) / Reset / ReleaseData (dtor body) |
| 0x0044C6B0 / 0x009AD410 | 2/2 | CGameDefinitionManager_Get / CDefinitionManager_GetDefGlobalIndexFromName | |
| 0x005F8EC0 | 1 | stdvectorEHeroAbility_vector_005f8ec0.cpp | vector<EHeroAbility>::vector(size_type) |
| 0x00BFEA1A / 0x00BFEA14 | 5/1 | global_operatornew / global_free | operator new / free (vector dtor inline) |
| 0x0042BE50 | 3 | NUISystemCAction_CAction_0042be50.cpp | NUISystem::CAction::CAction() |
| 0x0042AA29 | 3 | CCountedPointer00429f43_CCountedPointer00429f43_0042aa29.cpp | FIX: CCountedPointer<NUISystem::CAction>::CCountedPointer(CAction*) |
| 0x00429160 | 3 | CCountedPointer_ReleaseData_00429160.cpp | CCountedPointer<CAction>::ReleaseData |
| 0x005F8890 | 1 | CTCCarryable_OnKill_005f8890.cpp ("RbTree_Clear") | FIX: map<EUISpellCategory,CComponent*> tree clear (CategoriesLists.clear(), this+0x194) |
| 0x00579353 | 2 | CTCCarryable_OnKill_00579353.cpp ("RbTree_Clear") | FIX: tree clear used for map<EHeroAbility,CComponent*> (this+0x1A0/+0x1AC); likely an identical-COMDAT fold -- run label_trust.py |
| 0x005EDE37 | 2 | CTCInventoryAbilities_GetCategory_005ede37.cpp | CTCInventoryAbilities::GetCategory(EHeroAbility) |
| 0x005F9E60 | 4 | stdmap_operator_005f9e60.cpp | map<EUISpellCategory,CComponent*>::operator[] |
| 0x005F9EA5 | 2 | stdmap_operator_005f9ea5.cpp | map<EHeroAbility,CComponent*>::operator[] |
| 0x00410DE0 / 0x0040F020 | 1/1 | STLbitset_test / CVectorMap_LowerBound | inline GetTC<CTCInventoryExperience> expansion (type 0x16) |
| 0x0056F2BD | 1 | CTCInventoryExperience_GetExperienceAbilityName_0056f2bd.cpp | CTCInventoryExperience::GetExperienceAbilityName |
| 0x0099AED0 / 0x0099B6B0 / 0x0099B7D0 / 0x0099B8D0 / 0x0099B510 | 2/3/2/4/35 | CWideString_* | CWideString() / (const wchar_t*) / operator= / operator+= / ~ |
| 0x00405F00 | 8 | global_GFIntToWideString_00405f00.cpp | GFIntToWideString (__fastcall, ret ptr ecx, value edx) |
| 0x0099BE70 | 14 | global_operator_0099be70.cpp | operator+(const CWideString&, const CWideString&) |
| 0x009C95E0 | 1 | NGameTextCDataBank_GetTextBySymbol_009c95e0.cpp | NGameText::CDataBank::GetTextBySymbol |
| 0x00428494 | 1 | (not landed) | NUISystem::CManager::CreateMenuEntryComponent (NUISystem_CManager.h:97) |
| 0x005F8150 | 1 | vectorEWeaponClass_UmoveEWeaponClass_005f8150.cpp | FIX: std::find<EHeroAbility*, EHeroAbility> (std_find_005f7acc.cpp + fable_algorithm.h FABLE_RETAIL_STLP_FIND comment name this exact address) |
| 0x0048524C | 1 | CCountedPointerNUISystemCAction_ShareData_0048524c.cpp | FIX: used as CCountedPointer<CMenuEntry>::ShareData (SetStaticCast; ContructItemLists header says so) |
| 0x00429460 | 1 | CCountedPointerNUISystemCAction_ShareData_00429460.cpp | FIX: used as CCountedPointer<CText>::ShareData (ditto) |
| 0x0042925C | 1 | Destval...COptimisedPrimitive_0042925c.cpp | CCountedPointer<NUISystem::CText>::~CCountedPointer (leaf in file is right; filename wrong) |
| 0x004850F9 | 1 | CCountedPointerNUISystemCMenuEntry_ReleaseData_004850f9.cpp | CCountedPointer<CMenuEntry>::ReleaseData |
| 0x005EDFD9 | 1 | CTCInventoryAbilities_GetAbilityPDef_005edfd9.cpp | GetAbilityPDef (hidden ret CDefPointer) |
| 0x005F840C | 3 | stdRbtree_Mfind_005f840c.cpp | _Rb_tree<EUISpellCategory,...>::_M_find (from map::find) |
| 0x005EE288 | 6 | CTCInventoryAbilities_GetMenuEntryFromCategory_005ee288.cpp | GetMenuEntryFromCategory(EUISpellCategory, CComponent*) |
| 0x005CC772 | 6 | CTCInventoryStats_GetXString_005cc772.cpp | CTCInventoryStats::GetXString (static, hidden ret) |
| 0x0042BED4 / 0x0042CD84 / 0x0042ABCA | 3/3/3 | NUISystemCDeletion_* / stdlist..._list_0042abca | CDeletion() / CDeletion(const CDeletion&) / ~list<CComponent*> (CDeletion's inline dtor) |
| 0x005F8908 | 1 | CActiveFile_OnReadFinished_005f8908.cpp | FIX: std::_List_base<EHeroAbility>::~_List_base (clear 0x5F8197 + free sentinel) |
Virtual slots: CComponent 3 FindChild(CCharString by value) +0xC, 59 AddChild +0xEC, 62 SetDeletion(CDeletion by value) +0xF8,
65 GetType +0x104, 66 AddAction +0x108, 92 MakeUnavailable +0x170, 97 FindFirstChildOfTypeNonRecursive +0x184; CSpellContainerList 142
AddSpellSelectionChild +0x238, 143 AddSpellIndicationChild +0x23C, 147 AddChildAndSetOffsets +0x24C, 148 InitialiseOffsets +0x250;
CSpellContainer 145 SetEmptySlotDef +0x244, 147 AddChildAndSetOffsets +0x24C; CSprite 145 SetGraphicIndex +0x244; CList 145
AddNonScrollingChild +0x244, 152 AddScrollingChildAndSetOffsets +0x260, 156 InitialiseOffsets +0x270; CViewport 142 AddFakeChild +0x238;
CText 142 GetValue +0x238, 143 SetValue(unsigned long, const CWideString&, bool) +0x23C, 144 SetValue(const CWideString&, bool, bool)
+0x240 (NUISystem_CText.h:21-24); CMenuEntry 151 AddUIActionParam(CActionParam*) +0x25C.

## 5. Members (all in rebuild/include/engine/CTCInventoryAbilities.h, retail layout sizeof 0x1D8, opt-in FABLE_RETAIL_CTCINVENTORYABILITIES)
this+0x154 ReceivedAbilityOrder vector<EHeroAbility>; +0x160 Abilities vector<pair<long,CDefPointer<const CHeroAbilityDef>>> (.first level,
.second def -> CHeroAbilityDef+0x44 IconGraphicIndex, +0x60 LevDescription vector<long>); +0x17C AssignedAbilities, +0x188
AssignedAbilitiesSave; +0x194 CategoriesLists map<EUISpellCategory,CComponent*>, +0x1A0 UnassignedIcons, +0x1AC AbilitiesMenuEntries
(maps typed only under FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC; else byte arrays). ParentThing (+4) for GetTC.
misc (NUISystem_CUIMiscThingsDef.h): +0x3C SpaceSeparator CWideString, +0x148 TotalSpellsInPalettes, +0x14C TotalSpellsInContainer.
EUISpellCategory (header): ATTACK=0, SURROUND=1, PHYSICAL=2. CAction::ActionType 0xCA/0xCD/0xCE (draft's UI_ACTION_TYPE_GIVE_EXCLUSIVE_INPUT /
SHOW_SPELL_SELECTION / SET_SPELL_TO_ASSIGN). CDeletion::Method=1 (UI_DELETE_THIS_INSTANCE). No local struct models needed: every class has
a shared header (the ConstructScreen row's include list is the complete set).

## 6. Opt-in macros (start from ConstructScreen 0x005F519F's list, which already compiles this body) + per-TU STLport opt-ins to TRY
ConstructScreen's: CCOUNTEDPOINTER_RELEASEDATA, CTCINVENTORYABILITIES(+_MAGIC,_SCREEN,_EXPRESSIONS for GetMiscThingsDef), CSPELLCONTAINERLIST_
VIRTUALS/_OFFSETS, CSPELLCONTAINER_VIRTUALS, CCLICKABLE_SHARED_ACTION, CMENUENTRY_ACTIONPARAMS, CACTIONPARAMSLONG, CACTIONPARAMULONG,
CTCINVENTORYEXPERIENCE, CDELETION, CTHING_GETTC_EXPANDED, CTCBASE, CTHING, CTCINVENTORY(_QUICKACCESS), CTCINVENTORYBASE(_BUILDINGTHING,
_VTABLE), CDEFINITIONMANAGER, CGAMEDEFINITIONMANAGER, CMAINGAMECOMPONENT(_TEXTBANK), CCHARSTRING (+ CWIDESTRING, STLP_BITSET_TEST_DECL
from the expressions row). Retail calls these OUT OF LINE (the draft in ConstructScreen does not yet opt in; add and check with
/opt/fable_lane/w22/agent4007/tools/cd.sh):
- FABLE_RETAIL_STLP_LIST + FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE (0x5F88ED / 0x5F8908) + FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE (0x5F88C8)
- FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE (clear -> erase 0x5F8F89), FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE (0x5F9BCD),
  FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE (0x5F8EC0)
- FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE (0x5F8890, 0x579353); map::operator[] out of line (0x5F9E60/0x5F9EA5) -- default decl-only
  form, or FABLE_RETAIL_STLP_MAP (visible body: key store scheduled right before the call, as at 0x5EF9EB `mov [ebp+5Ch],2`)
- _M_find 0x5F840C out of line: default visible body, or FABLE_RETAIL_STLP_TREE_MFIND_DECL
- FABLE_RETAIL_STLP_FIND (std::find 0x5F8150 forwarder, __fastcall)
- FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE (GetAbilityPDef temp: `add eax,4; dec [eax]; cmp [eax],0` + `and [ebp-4],0`; the misc
  handle's final release at 0x5F05C3 has NO null test -- residue risk; it is `const CDefPointer<const CUIMiscThingsDef> misc = GetMiscThingsDef();`).

## 7. Siblings to copy (same TU, retail neighbourhood)
- CTCInventoryAbilities_ConstructScreen_005f519f.cpp (caller; biased frame; optimize("s"); macro list above; same-TU bodies).
- CTCInventoryAbilities_ContructItemLists_005f077f.cpp (biased frame `lea ebp,[esp-68h]`; CreateMenuEntryComponent spelling,
  SetStaticCast<CText>/<CMenuEntry>, AddFakeChild, GetType()==0 -> SetGraphicIndex idioms identical to this row's menu loop).
- CTCInventoryAbilities_ConstructPCItemLists_005f1526.cpp (same recipe + CACTIONPARAMSLONG/ULONG).
- CTCInventoryAbilities_GetExpressionsListUIComponent_005f4c8f.cpp (sister console builder; ITEMLISTS, TREE_CLEAR_OUTOFLINE, VECTOR_PUSHBACK_OUTOFLINE).
- CTCInventoryAbilities_OpenInventory_005f05e5.cpp (next function; "y" off + "s" on, plain frame).
