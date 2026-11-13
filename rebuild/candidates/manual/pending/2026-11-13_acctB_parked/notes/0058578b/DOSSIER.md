# DOSSIER 0x0058578B -- CTCInventory::OpenInventory (8081 bytes, 0x0058578B..0x0058771B)
No earlier scout existed for this row ($W/prior/ empty). The only earlier work is agent4012's call map
(/opt/fable_lane/w22/agent4012/scout/ow/calls_resolved.tsv, reused here). $W/dis_annot.txt = retail listing with every
direct call named and every string pushed decoded (built by $W/annot.py).

## 1. Retail shape
- `push ebp ; lea ebp,[esp-78h] ; sub esp,0F4h ; and [ebp+64h],0 ; push ebx ; push esi ; mov ebx,ecx ; push edi` ...
  `pop edi ; pop esi ; pop ebx ; add ebp,78h ; leave ; ret` (0x00587714..0x0058771B). Plain `ret`: __thiscall, no args, void.
- Frame pointer with the EBP-biased form = `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (the same pair every
  landed CTCInventory*::OpenInventory row uses: 0x005C2F27 Weapons, 0x005CCF86 Stats, 0x0056FAD7 Experience).
- NO SEH frame, no /GS cookie, no EH states although many CCharString / CWideString / CCountedPointer locals: the TU
  is built without EH (no /EHsc). Conditional-destruction flags for `&&`/`||` temporaries live in a plain dword
  [ebp+64h] (zeroed in the prologue; bits 1/2 in quest loop 1, bits 4/8 in quest loop 2).
- `this` stays in ebx for the first part, saved to [ebp-14h]; the top-level menu component lives in edi and [ebp-78h]
  for the whole body.

## 2. Name (proven by 3 sources)
- `?OpenInventory@CTCInventory@@UAEXXZ` = `virtual void CTCInventory::OpenInventory()`. Manifest label `CTCInventory::vfunc_30`
  is a placeholder: slot 30 of CTCInventory vtable 0x01250DE4 (rebuild/manifest/rtti-vtables.tsv) is the slot of
  NInventory::CTCInventoryBase::OpenInventory 0x005BD4EA (base vtable 0x01253DBC slot 30), and every sibling's slot 30 is
  its OpenInventory (Weapons 0x005C2F27, Stats 0x005CCF86, Clothing 0x005B4E7C, Map 0x006116E0, Magic 0x0072DB70,
  Abilities 0x005F05E5, Experience 0x0056FAD7, Quests 0x00622A00, Trade 0x005A94BD -- landed names).
- Ego_r name set: ghidra_out/egor_pdb_names.tsv / egor_signatures.tsv `005f4d20 ?OpenInventory@CTCInventory@@UAEXXZ`;
  FableWin: ghidra_out/_fablewin_api.tsv + naming_stragglers `0212841c ?OpenInventory@CTCInventory@@UAEXXZ` (20383 B debug).
- First call after the prologue work is the qualified base call `NInventory::CTCInventoryBase::OpenInventory()` (0x00585867).
- Only reference: the vtable slot (`xref.py`: abs .rdata 0x01250E5C). No direct callers.
- Header: rebuild/include/engine/CTCInventory.h FABLE_RETAIL_CTCINVENTORY branch has NO OpenInventory declaration; the
  author must add it (overlay opt-in) as `virtual void OpenInventory();` (needs FABLE_RETAIL_CTCINVENTORYBASE so the base
  declares the virtual, or FABLE_RETAIL_CTCINVENTORYBASE_VTABLE slot 30).

## 3. Callees (landed file, landed name, PDB name)
| addr | n | landed row | PDB / note |
|---|---|---|---|
| 0x0041D122 | 1 | 00/41/NUISystemCManager_OnOpenMenus_0041d122.cpp | ?OnOpenMenus@@YIXXZ (static/fastcall; first call) |
| 0x00686D40 | 1 | 00/68/CTCBase_GetPlayerManager_00686d40.cpp | ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ |
| 0x00449980 | 1 | 00/44/global_Catch0042ae0a_00449980.cpp (BAD landed name) | returns an index into [0x013B8388]+0x2C (8-byte stride) -> object whose slot 0x48/4=18 is called. Probably CPlayerManager::GetMainPlayerNumber-like; check FableWin. |
| 0x005BD4EA | 1 | 00/5b/NInventoryCTCInventoryBase_OpenInventory_005bd4ea.cpp | ?OpenInventory@CTCInventoryBase@NInventory@@UAEXXZ (qualified call) |
| 0x005BC58F | 1 | 00/5b/NInventoryCTCInventoryBase_SetInventoryAnimRenderBox_005bc58f.cpp | ?SetInventoryAnimRenderBox@CTCInventoryBase@NInventory@@QAEXABVC2DBoxF@@@Z |
| 0x00437CD0 | 1 | 00/43/CWorld_DrawGetEnvironment_00437cd0.cpp (BAD file name) | ?GetPHeroInfoScreensManager@CPlayerGui@@QBE... (ecx=[0x013B8790]) |
| 0x0066E799 | 1 | 00/66/CObservable_AddExclusiveObserver_0066e799.cpp (BAD name) | NHeroInformationScreens::CManager::SetCurrentScreenType(long) (arg = this->CBase(+0x130) vslot 2 GetScreenType()) |
| 0x0041E5F2 | 11 | NUISystemCManager_GetInstance_0041e5f2.cpp | ?GetInstance@CManager@NUISystem@@SIPAV12@XZ |
| 0x0041DB1D | 12 | NUISystemCManager_CreateComponent_0041db1d.cpp | ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z (arg false) |
| 0x0099EBF0/0x0099EAE0 | 62/13 | CCharString ctor(const char*, -1) / dtor | ??0CCharString@@QAE@PBDJ@Z / ??1 |
| 0x0099AED0, 0x0099B720, 0x0099B510, 0x0099B8D0, 0x0099BE70 | | CWideString(), CWideString(const&), ~, operator+=, global operator+ (fastcall) | |
| 0x00405F00 | 5 | global_GFIntToWideString_00405f00.cpp | ?GFIntToWideString@@YI?AVCWideString@@J@Z |
| 0x005CC772 | 4 | CTCInventoryStats_GetXString_005cc772.cpp | ?GetXString@CTCInventoryStats@@SI?AVCWideString@@XZ |
| 0x00410DE0 | 10 | STLbitset_test (bitset<274>::test, TC presence) | part of inline CThing::GetTC/PeekTC |
| 0x0040F020 | 6 | CVectorMap::LowerBound (non-const) | GetTC on [this+4] (parent thing): ids 0x13,0x11,0x1B,0x6F,0x14,0x12 |
| 0x004365B0 | 4 | CVectorMap::LowerBound (const) | PeekTC: hero TCI_HERO_STATS(4), quest card 0x6C (x2), hero TCI_INVENTORY_WEAPONS 0x13 |
| 0x00449960 | 3 | CPlayerManager::GetMainPlayer | ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ |
| 0x00487DD0 / 0x00487DC0 | 2/1 | CPlayer::PeekPControlledCreature / GetPControlledCreature | |
| 0x00661F70 | 1 | CThingCreatureBase::IsChild | ?IsChild@CThingCreatureBase@@QBE_NXZ |
| 0x00686D30 | 4 | CTCBase::GetWorldMap | |
| 0x004FC190 / 0x004FB490 | 2/2 | CWorldMap::GetRegionNumberMapIsIn / GetRegion | |
| 0x005BEB57 | 1 | CTCInventoryBase_GetMiniMapGraphic_005beb57.cpp | GetMiniMapGraphic(CRegion&) (opt-in FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC) |
| 0x00603000 | 1 | NOT landed | CTCInventoryMap::CreateMiniMapMarkers (declared in engine/CQuestManager.h comment / CTCInventoryMap.h) |
| 0x0044C6B0 | 2 | CGameDefinitionManager::Get | |
| 0x009AD390 | 1 | CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, ..) | arg 1 |
| 0x0042B312 | 1 | GetDef<NUISystem::CUIMiscThingsDef> | ??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBV...@@@Z |
| 0x004CC340 | 2 | IS_THING_ALIVE(const CIntelligentPointer<CThing>&) | |
| 0x00A01B50 | 2 | CBaseIntelligentPointer::GetPItem | |
| 0x00702280/0x007022C0 | 3/2 | CTCQuestCard::IsCore / IsOptional | |
| 0x00702310/0x00702400/0x00702470 | 2/1/2 | CTCQuestCard::GetQuestName / GetQuestSummary / GetQuestObjective | by value |
| 0x00BFE94C / 0x00404280 | 1/1 | wcscmp / std::operator==(wstring, const wchar_t*) | inline CWideString::operator==(L"") |
| 0x00429C15 / 0x00429C6F / 0x004291DE | 9/2/9 | CCountedPointer<CComponent>(T*) / Reset / ReleaseData (dtor) | |
| 0x0042AA29 / 0x00429160 | 15/15 | CCountedPointer<CAction>(T*) / ~ | |
| 0x0042BE50 | 15 | NUISystem::CAction::CAction() (out of line here at EVERY site) | |
| 0x0042BED4 / 0x0042CD84 / 0x0042ABCA | 7 each | CDeletion() / CDeletion(const&) / ~list<CComponent*> (=CDeletion dtor inline) | |
| 0x0042B646 | 33 | list<CActionParam*>::push_back | |
| 0x00BFEA1A | 23 | operator new | CActionParamState (vptr 0x012366EC, type 2), CActionParamSLONG (0x012366F4, type 3) built INLINE after new(0xc) |
| 0x005BF482 | 3 | CTCInventoryBase::GetAllItems(vector<pair<long,long>>&) const | |
| 0x005D9970/97F0/9830 | | CTCInventoryItem::IsBook / IsWeapon / IsExpression (static fastcall J) | |
| 0x005D9A10/0x005D9AA0 | | CTCInventoryItem::GetHairCardDef / GetTattooCardDef (static, by value CDefPointer) | |
| 0x00593139 | 1 | vector<pair<long,long>>::erase(iterator) | |
| 0x006869D0 | 2 | CTCBase::GetDefinitionManager | |
| 0x0042AF3C / 0x0042B248 / 0x005935C4 / 0x0057FF92 | | GetDef<CThingBaseDef>, GetSubDef<CAugmentationDef>, GetSubDef<CHeroSuitDef>, GetSubDef<CAppearanceModifierDef> (x2) | |
| 0x009AD9E0 | 1 | CDefinitionManager::GetPDefFromGlobalIndex | |
| 0x005BDF08 | 1 | CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) const | on the CLOTHING TC ([ebp-10h]) |
| 0x004292C0 | 1 | _Rb_tree_base::_M_increment (std::set iterator ++ over CHeroSuitDef +0x28 set) | |
| 0x00486024 | 1 | vector<CComponent*>::push_back | this->Components.push_back(top_level) |
| 0x00BFEA14 | 1 | free | vector<pair> storage release (STLport alloc) |
Landed-name mismatches to flag: 0x00449980 (global_Catch0042ae0a), 0x00437CD0 (CWorld_DrawGetEnvironment), 0x0066E799
(CObservable_AddExclusiveObserver / SetMember_dword0x28) are wrong landed spellings; use the PDB names.

## 4. Members
- this+0x04 ParentThing (CTCBase, FABLE_RETAIL_CTCBASE); this+0x0C Components vector<CComponent*> (FABLE_RETAIL_CTCINVENTORYBASE);
  this+0x68 PDef CDefPointer<const CInventoryDef> (FABLE_RETAIL_CTCINVENTORYBASE_PDEF; two by-value GetPDef() copies,
  esi then edi), def +0xA4 (TL pos C2DVector) / +0xAC (extents) -> C2DBoxF -> SetInventoryAnimRenderBox
  (FABLE_RETAIL_CTCINVENTORYBASE_ANIMBOX). Exactly the 0x005C2F27 idiom with other members: find the two CInventoryDef
  members at +0xA4/+0xAC (Weapons row uses WeaponsAnimBoxTLPos/Extents).
- this+0x130: NHeroInformationScreens::CBase subobject; `lea ecx,[ebx+130h] ; call [vtbl+8]` = GetScreenType().
- Parent thing +0x8E (short): map number passed to GetRegionNumberMapIsIn (CThing member, movsx).
- CThing TC bitset +0x20, TC CVectorMap +0x44 (engine/CThing.h, FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN /
  FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN in agent4007 hdr overlay).
- CTCHeroStats +0x3C = gold (hero stats via PeekTC(TCI_HERO_STATS)).
- CTCQuestCard +0x0C PDef (CDefPointer<const CQuestCardDef>), CQuestCardDef +0x59 IsVignette (engine/CQuestCardDef.h, IsCore +0x58).
- CQuestManager singleton [0x013B89FC] +0x5C ActiveQuestCards list<CIntelligentPointer<CThing>> (FABLE_RETAIL_CQUESTMANAGER).
- CUIMiscThingsDef +0x3C, +0x44 (two CWideString members joined between quest name and summary/objective) -- look up in
  struct_layouts_egor.tsv for NUISystem::CUIMiscThingsDef.
- CHeroSuitDef +0x28 (std::set<long>, items checked with GetNumberOfItemsOfTypeInInventory, first >0 counts);
  CAppearanceModifierDef +0x4C (long, 1..5 counts as clothing) and +0x40 (flags: bit7 or bit8 -> [ebp+1Ch], bit4 -> [ebp+10h]).
  Hair card def +0x28 (global def index) -> GetPDefFromGlobalIndex -> GetSubDef<CAppearanceModifierDef>.
- globals: [0x013B86A0] = main game component (+0x1C player manager, +0x28 ->+0x2C ?, +0x14 text bank); [0x013B8388]
  (+0x2C players array 8-byte); [0x013B8394] sound/music manager? (slot 0xB8/4=46 called 3x with (0x10/0x40/0x80, 0.0f,
  [0x01237584] float)); [0x013B8790] CPlayerGui.

## 5. Visibility / inline decisions
- CAction() is CALLED (0x0042BE50) at all 15 sites (not expanded); CCountedPointer ctors/dtors called; CDeletion ctors called;
  list push_back called. CActionParamState / CActionParamSLONG constructors are EXPANDED (inline vptr store).
- CThing::GetTC / PeekTC expanded (bitset test + LowerBound called). Hero-side lookups use the const LowerBound 0x004365B0
  (PeekTC on PeekPControlledCreature); parent-thing lookups use 0x0040F020 (GetTC non-const).
- CWideString::operator==(const wchar_t*) expanded (wcscmp / std::operator==).
- The quest test is `IsCore() || (IsOptional() && !GetPDef()->IsVignette)` with two CDefPointer copies -> flags.
  agent4007 src line 175 has the matched spelling (`quest_card->IsOptional() && !quest_card->GetPDef()->IsVignette`).

## 6. Siblings / family
- rebuild/src/compiled/00/5c/CTCInventoryWeapons_OpenInventory_005c2f27.cpp and ..Stats_OpenInventory_005ccf86.cpp:
  `#pragma optimize("y", off)` + `("s", on)`, C2DBoxF(GetPDef()->X, GetPDef()->Y) idiom (local models; use shared headers).
- rebuild/src/compiled/00/56/CTCInventoryExperience_OpenInventory_0056fad7.cpp (big one, FABLE_RETAIL_STLP_BITSET_TEST_DECL,
  FABLE_RETAIL_CTHING_GETTC_EXPANDED, engine/CVectorMap.h).
- /opt/fable_lane/w22/agent4007 (matched CTCInventoryQuests::ConstructQuestList 0x0061B610): src/0061b610.cpp define list,
  hdr/ overlay (CTCQuestCard ACCESSORS, CQuestCardDef, NUISystem_CAction, CThing PEEKTC chain, CQuestManager); LESSONS.md.
- Same-class landed neighbours: 0x00584A73 AddCloseInventoryEvent, 0x0058509F OnItemAdded, 0x00587749/0x00587787
  ProcessDPadUp/DownReleased (right after this row), 0x00589D30 CloseInventory, 0x0058E99F OpenInventoryShortcut.
