# DOSSIER 0x005F1B10 -- CTCInventoryAbilities::ConstructPCMagicList (8383 bytes)

Scout working files: $W/scout/ (calls_resolved.tsv = every direct callee resolved to manifest / landed file / reloc symbol;
map/ev0.txt = symbolic call map of the whole row, ebp-relative slots written Lp<x> = [ebp+x], Lm<x> = [ebp-x]; map.py regenerates it).
$W/prior/ was empty: no earlier scout of this row exists (agent4015 dir is empty too).

## 1. Identity (manifest label is RIGHT)
- Leaf: `?ConstructPCMagicList@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@PAV23@@Z`
  -> `public: NUISystem::CComponent* __thiscall CTCInventoryAbilities::ConstructPCMagicList(NUISystem::CComponent* parent)`, non-virtual, non-const.
- Sources: (a) ghidra_out/egor_pdb_names.tsv + egor_signatures.tsv (Ego_r.pdb, 0x005807E3 there); (b) _fablewin_api.tsv /
  naming_stragglers/FableWin.exe.signatures.tsv (FableWin.pdb 0x01ECC87B, debug body 19044 bytes / 4211 instr); (c) the only caller
  `python3 $S/w23/xref.py 005f1b10` = 0x00422D85 inside landed NUISystem::CManager::Action 0x0041E6D3
  (rebuild/src/compiled/00/41/NUISystemCManager_Action_0041e6d3.cpp:380 declares it `NUISystem::CComponent* ConstructPCMagicList(NUISystem::CComponent*); // 0x005F1B10`,
  line 1985 `CComponent* list = abilities->ConstructPCMagicList(screen);`). Three agreeing sources.
- Not in any vtable (no rtti-vtables hit). Shared header rebuild/include/engine/CTCInventoryAbilities.h has NO declaration yet:
  add one in the overlay under a new opt-in (suggest `FABLE_RETAIL_CTCINVENTORYABILITIES_PCMAGIC`, public, next to the
  EXPRESSIONS block that declares its twin `ConstructPCExpressionsList(CComponent*)` 0x005F59C7).
- Console twin: `GetMagicListUIComponent(long& count, CComponent* parent)` 0x005EEFF2, fully reconstructed (same TU, same-TU body)
  inside landed rebuild/src/compiled/00/5f/CTCInventoryAbilities_ConstructScreen_005f519f.cpp lines ~226-384. Copy its idioms:
  owned list loop, AssignedAbilities clear/fill, category counting, CategoriesLists[...] = list, per-ability entry, final
  3-category find/else MakeUnavailable + CDeletion. The PC row differs: PC_* template names, no palette reorder/swap, adds the
  expression-item and spell quick-slot containment sections, AddPositionChild on the PC frame, and UNROLLED final category blocks.

## 2. Retail shape
- `push ebp ; lea ebp,[esp-74h] ; sub esp,124h ; push ebx ; push esi ; push edi` -- the biased frame of a size-optimised TU
  (`#pragma optimize("s", on)`), exactly as sibling ConstructPCItemLists 0x005F1526 (`lea ebp,[esp-68h]`). Epilogue
  `pop edi ; pop esi ; pop ebx ; add ebp,74h ; leave ; ret 4` (0x005F3BC5..0x005F3BCC). thiscall, `ret 4`.
- NO SEH frame / no fs:0 / no EH state stores although ~60 objects with destructors live here: the TU is built without /EHsc
  like every landed sibling of this file (their `// Flags:` line is empty). No /GS cookie.
- this kept in ebx until 0x005F1EF3 then in [ebp+20h]; the returned root (CreateComponent("PC_MAGIC_CATEGORIES")) is in [ebp+1Ch]
  (`mov eax,[ebp+1Ch]` at 0x005F3BC2).
- Size idioms: `xor esi,esi ; inc esi` (loop start 1), `and [x],0` stores, `or eax,-1`, `neg/sbb/and` null-adjust for the
  `&comp->CActionParam base (+0x18)` conversions, `sub eax,edi/dec/dec` switch.
- The parameter slot [ebp+7Ch] (parent) is reused after 0x005F2BBF for the per-ability `CWideString displayName` (ctor 0x005F2E01).
  The list allocator temporary lives at [ebp+7Fh] (inside the parent slot) -- normal VC7.1 packing, nothing to model.

## 3. Callees (landed file under rebuild/src/compiled/<aa>/<bb>/; * = landed name disagrees with the real identity)
| addr | what it is (use this spelling) | landed file / note |
|---|---|---|
| 0041E5F2 | NUISystem::CManager::GetInstance() (static) | NUISystemCManager_GetInstance_0041e5f2 |
| 0041DB1D | CManager::CreateComponent(const CCharString&, bool) x22 | NUISystemCManager_CreateComponent_0041db1d |
| 00428494 | CManager::CreateMenuEntryComponent(const CWideString&, float, float, const CCharString&, float, CCharString, float, CCharString) x1 | not landed; declared in NUISystem_CManager.h (used by ConstructScreen) |
| 0099EBF0 / 0099EAE0 / 0099EC30 | CCharString(const char*, long=-1) / ~CCharString / CCharString(const CCharString&) | landed |
| 0099F690 | operator+(const char*, const CCharString&) (fastcall) | global_operator_0099f690 |
| 0099F830 | GFIntToCharString(long) (fastcall, edx=value) | global_GFIntToCharString_0099f830 |
| 0099AED0 / 0099B6B0 / 0099B510 | CWideString() / CWideString(const wchar_t*) / ~CWideString | landed |
| 0099B7D0 / 0099B8D0 / 0099BE70 | CWideString::operator= / operator+= / operator+(const CWideString&, const CWideString&) (fastcall) | landed |
| 00405F00 | GFIntToWideString(long) (fastcall, edx=value) | global_GFIntToWideString_00405f00 |
| 005CC772 | CTCInventoryStats::GetXString() static fastcall | CTCInventoryStats_GetXString_005cc772 |
| 009C95E0 | NGameText::CDataBank::GetTextBySymbol(const CCharString&) const | landed |
| 00686D60 | CTCBase::PeekComponent() const (x2) | CTCBase_PeekComponent_00686d60 |
| 00415FBC | CMainGameComponent::PeekDefinitionManager() | landed |
| 0042B040 | CDefinitionManager::GetDef<CThingObjectDef>(long, CDefPointer<const CThingObjectDef>*) const | CDefinitionManager_GetDefCThingObjectDef_0042b040 |
| 005936C2 | CDefClassBase::GetSubDef<CAbilityDef>(CDefPointer<const CAbilityDef>*) const | CDefClassBase_GetSubDefCAbilityDef_005936c2 |
| 00410DE0 / 0040F020 | bitset<274>::test + CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound = CThing::GetTC expansion (types 17 CTCInventory, 22 CTCInventoryExperience) | opt-in FABLE_RETAIL_CTHING_GETTC_EXPANDED (as 0x005F1526 / 0x005F519F) |
| 00581FA4 | CTCInventory::GetQuickAccessItemGraphicIndex(long) const | landed; opt-in FABLE_RETAIL_CTCINVENTORY_QUICKACCESS |
| 0056F2BD | CTCInventoryExperience::GetExperienceAbilityName(long) -> CWideString | landed; FABLE_RETAIL_CTCINVENTORYEXPERIENCE |
| 005EE3B3 | CTCInventoryAbilities::GetMiscThingsDef() const (hidden-ptr return to [ebp-18h]) | landed; opt-in ..._EXPRESSIONS |
| 005EDE37 | GetCategory(EHeroAbility) x3 | landed; ..._MAGIC |
| 005EE288 | GetMenuEntryFromCategory(EUISpellCategory, CComponent*) x6 | landed; ..._MAGIC |
| 005EDFD9 | GetAbilityPDef(EHeroAbility) const (hidden-ptr return [ebp-2Ch]) | landed; ..._MAGIC |
| 005F88ED | std::list<EHeroAbility>::list(const allocator&) | * landed as global_stdCircularListConstruct |
| 005F88C8 | list<EHeroAbility>::insert(iterator, const EHeroAbility&) (out of line, = push_back(end)) | * landed as global_LinkedListInsertNodeBefore |
| 005F8908 | list<EHeroAbility>::~list (_List_base dtor) | * landed as CActiveFile_OnReadFinished_005f8908 (WRONG) |
| 005F8F89 | vector<EHeroAbility>::erase(first,last) (= clear()) x2 | stdvector_erase_005f8f89 |
| 005F9BCD | vector<EHeroAbility>::push_back x4 (out of line) | stdvector_pushback_005f9bcd; FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE |
| 005F8150 | std::find<EHeroAbility*,EHeroAbility>(first,last,val) (ecx/edx/push) | * landed as vectorEWeaponClass_UmoveEWeaponClass_005f8150 (WRONG name) |
| 005F8890 | _Rb_tree::clear -- CategoriesLists.clear() (+0x194) | * landed as CTCCarryable_OnKill_005f8890 (WRONG) |
| 00579353 | _Rb_tree::clear -- UnassignedIcons.clear() (+0x1A0), AbilitiesMenuEntries.clear() (+0x1AC) | * landed as CTCCarryable_OnKill_00579353 (WRONG; folded COMDAT for map<EHeroAbility,CComponent*>) ; FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE |
| 005F9E60 | map<EUISpellCategory,CComponent*>::operator[] x4 | stdmap_operator_005f9e60 |
| 005F9EA5 | map<EHeroAbility,CComponent*>::operator[] x2 | stdmap_operator_005f9ea5 |
| 005F840C | map<EUISpellCategory,CComponent*>::find (_M_find) x3 | stdRbtree_Mfind_005f840c |
| 0042BE50 | NUISystem::CAction::CAction() after `new(0x10)` x19 | landed (opt-in FABLE_RETAIL_CACTION_INLINE_CTOR NOT wanted: retail calls it) |
| 0042AA29 / 005791E5 / 00429160 | CCountedPointer<CAction>(CAction*) / ::Reset / ::ReleaseData (=dtor) | landed; FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA |
| 00429C15 / 00429C6F / 004291DE | CCountedPointer<CComponent>(ptr) / Reset / ReleaseData | landed |
| 0048524C / 004850F9 | CCountedPointer<CMenuEntry>::ShareData / ReleaseData (SetStaticCast) | * 0x0048524C manifest says CCP<CAction>::ShareData; reloc symbol is ShareData<CMenuEntry> |
| 00429460 / 0042925C | CCountedPointer<CText>::ShareData / ~CCountedPointer<CText> | * 0x00429460 manifest says CCP<CAction>::ShareData; reloc = ShareData<CText>; 0x0042925C landed as ??1CCountedPointer<CText> |
| 0053FDE0 | CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) (reloc also AddUIActionParam) x12 | NUISystemCAction_AddParam_0053fde0 |
| 0053FDB0 | CAction::AddParam(CActionParam*) x3 | landed |
| 0042B646 | list<CActionParam*>::push_back (CAction+4 = param list) x22 | global_ListPushFrontWrapper0042b646 (* descriptive name) |
| 0042BED4 / 0042CD84 / 0042ABCA | CDeletion() / CDeletion(const CDeletion&) / list<CComponent*>::~list (CDeletion's member at +4) | landed; FABLE_RETAIL_CDELETION |
| 00BFEA1A | operator new x41 | |
Vtable slots called: CComponent 3 FindChild(CCharString) (+0Ch, by value: `push ecx; mov ecx,esp; CCharString ctor`),
59 AddChild(+ECh), 62 SetDeletion(+F8h, CDeletion by value), 65 GetType(+104h), 66 AddAction(+108h), 67 AddActionOnSelected(+10Ch),
70 AddActionOnInactive(+118h), 74 AddActionOnDropped(+128h), 75 AddActionOnDroppedNowhere(+12Ch), 92 MakeUnavailable(+170h),
124 AddPositionChild(+1F0h). Subclass slots: CList 145 AddNonScrollingChild(+244h), 152 AddScrollingChildAndSetOffsets(+260h),
156 InitialiseOffsets(+270h) (same as console twin); CViewport 142 AddFakeChild(+238h); CText 142 GetValue(+238h, hidden ret),
143 SetValue(CWideString,CWideString,bool)(+23Ch), 144 SetValue(const CWideString&,bool,bool)(+240h); CSprite 145 SetGraphicIndex(+244h);
the assignment icon (CClickable/sprite) slot 152 (+260h) called with 0 at 0x005F3135 -- identify from the CClickable header
(ConstructPCItemLists 0x005F1526 uses the same call; copy its spelling). CDefPointee release: `dec [p+4]; jne; call [vtbl+4]`.

## 4. Members (CTCInventoryAbilities.h with FABLE_RETAIL_CTCINVENTORYABILITIES; layout asserted sizeof 0x1D8)
+0x04 ParentThing (CTCBase, CThing*) ; +0x148 ExperienceSpentOnAbilities ; +0x154/+0x158 ReceivedAbilityOrder vector<EHeroAbility> ;
+0x160 Abilities vector<pair<long,CDefPointer<const CHeroAbilityDef>>> (8-byte elems: .first level, .second def ->+0x44 IconGraphicIndex) ;
+0x17C AssignedAbilities, +0x188 AssignedAbilitiesSave ; +0x194 CategoriesLists, +0x1A0 UnassignedIcons, +0x1AC AbilitiesMenuEntries
(typed maps only with FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC). CUIMiscThingsDef (NUISystem_CUIMiscThingsDef.h): +0x3C SpaceSeparator
(CWideString), +0x148 TotalSpellsInPalettes. CTCInventory +0x148/+0x14C QuickAccessItems vector<long> (opt-in ..._QUICKACCESS).
CHeroAbilityDef +0x44 IconGraphicIndex, +0x60 LevDescription vector<long> (text ids). CMainGameComponent +0x14 text bank
(PeekComponent().GetTextBank(), FABLE_RETAIL_CMAINGAMECOMPONENT(_TEXTBANK)). CComponent +0x18 = CActionParam base (AddParam(&comp->base)).
CAction: +0 ActionType, +4 param list. CActionParamULONG (vptr 0x0122F844, type 5 at +4, value +8) / CActionParamSLONG (vptr 0x012366F4,
type 3) built INLINE after new(0xC): opt-ins FABLE_RETAIL_CACTIONPARAMULONG / FABLE_RETAIL_CACTIONPARAMSLONG.
Action types (NUISystem_EActionType.h): 244 ASSIGN_SPELL_PC, 245 OBSERVE_EVENT, 246 SET_SPELL_TO_ASSIGN_PC, 248 UNASSIGN_SPELL_PC,
249 IGNORE_EVENT, 252 UNASSIGN_EXPRESSION_ITEM_PC, 253 SET_EXPRESSION_ITEM_TO_ASSIGN_PC, 285 ASSIGN_SPELL_TO_ITEM_SLOT_PC,
327 SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC, 328 SET_SPELL_TO_ASSIGN_SWAPPING_PC, 329 SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC.
ULONG event params: 0x19 / 0x1E (EEvent values; OBSERVE/IGNORE event param).

## 5. Visibility
All CTCInventoryAbilities callees 0x005EDxxx-0x005EE3B3 are in the same source file but retail CALLS them (out of line) -- keep them
declared only. The STLport helpers at 0x005F8xxx-0x005F9xxx are this TU's COMDATs (unaligned, after the class code); retail calls
them, so use the out-of-line opt-ins (STLP_VECTOR_PUSHBACK_OUTOFLINE, STLP_TREE_CLEAR_OUTOFLINE) as ConstructPCExpressionCategoryList
0x005F45B5 does. CThing::GetTC IS expanded (bitset test + LowerBound + compare) -> FABLE_RETAIL_CTHING_GETTC_EXPANDED.
CCountedPointer ctor/Reset/ReleaseData, CAction ctor, CDeletion ctors all CALLED (not expanded). CActionParamULONG/SLONG ctors,
CDefPointer release, PeekComponent()->+0x14 text bank, vector size()/operator[] are expanded.

## 6. Siblings to copy (all rebuild/src/compiled/00/5f/, all `#pragma optimize("s", on)` after FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA, empty Flags)
- CTCInventoryAbilities_ConstructScreen_005f519f.cpp -- console twin GetMagicListUIComponent same-TU body; opt-in list incl. ..._MAGIC,
  ..._SCREEN, CSPELLCONTAINER*, CCLICKABLE_SHARED_ACTION, CMENUENTRY_ACTIONPARAMS, CACTIONPARAMSLONG/ULONG, CTCINVENTORYEXPERIENCE, CDELETION, CTHING_GETTC_EXPANDED.
- CTCInventoryAbilities_ConstructPCItemLists_005f1526.cpp -- same biased frame, `new CAction` + AddParam(param,self) + ULONG params,
  AddActionOnDropped / AddPositionChild; closest code idioms for the expression/spell slot sections.
- CTCInventoryAbilities_ConstructPCExpressionCategoryList_005f45b5.cpp -- PC frame registration, GetMiscThingsDef, STLP out-of-line opt-ins.
- CTCInventoryAbilities_UnAssignAbilityPC_005f1320.cpp / AssignAbilitySwappingPC_005f4519.cpp -- the PC slot actions' semantics.
- Family: /opt/fable_lane/w22/agent4007 (ConstructQuestList, matched; LESSONS.md, tools/cd.sh direct-call compare, tools/bt.sh).

## NAME_FIXES candidates (landed spellings proven wrong)
0x005F8908 CActiveFile::OnReadFinished -> std::list<EHeroAbility>::~list ; 0x005F8890 / 0x00579353 CTCCarryable::OnKill -> _Rb_tree::clear
(map<..>::clear) ; 0x005F8150 vector<EWeaponClass>::_Umove -> std::find<EHeroAbility*,EHeroAbility> ; 0x00429460 / 0x0048524C manifest
"CCountedPointer<CAction>::ShareData" -> ShareData<CText> / ShareData<CMenuEntry> (reloc symbols). Evidence: this row's call sites.
