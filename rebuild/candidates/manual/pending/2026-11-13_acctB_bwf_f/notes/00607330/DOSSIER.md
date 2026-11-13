# DOSSIER 0x00607330 CTCInventoryMap::ConstructLogbookList (9124 bytes, 0x00607330..0x006096B3 + jump table 0x006096B4..0x006096D3)

Prior work: NONE. $W/prior/ is empty and /opt/fable_lane/w22/agent4020 (the earlier agent for this row) has empty src/ hdr/ probe/ cand/.
So this is a first scout. Scout files: $W/ann.txt (full listing, calls + strings resolved), $W/compact.txt (same, `>` marks branch
targets), $W/callees.txt / callee_names.tsv (every direct call, count, manifest name, landed file, PDB leaf), ann.py / res.py.

## 1. Identity (PROVEN, 3 sources)
- Leaf `?ConstructLogbookList@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z`: Ego_r.pdb (egor_pdb_names.tsv 0x006595D0),
  FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x02084E60, 2505 instructions /Od), manifest row 0x00607330
  (`__thiscall`, returns CComponent*, 1 arg). The manifest label is RIGHT.
  => `public: NUISystem::CComponent* CTCInventoryMap::ConstructLogbookList(NUISystem::CComponent* parent);` (public, non-const, non-virtual).
- Twin: ConstructLogbookListPC = retail 0x006096E0 (right after; Ego_r 0x0065BBC0, same signature), not landed.
- GetMenuEntryFromCategory 0x005FAA90 (landed) names its callers 0x006094EC / 0x00609543 as this function.
- Only caller: 0x00421730 (xref.py). Shared header CTCInventoryMap.h has NO declaration yet -> add an opt-in
  `FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK` (declaration `public: NUISystem::CComponent* ConstructLogbookList(NUISystem::CComponent* parent);`
  plus `protected: NUISystem::CComponent* GetMenuEntryFromCategory(CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent*);`
  -- PDB `IAE` = protected, landed 0x005FAA90).

## 2. Retail shape (xdis / dis_all.txt)
- `push ebp; mov ebp,esp; and esp,0FFFFFFF8h; sub esp,10Ch; push ebx; push esi; push edi` ... `pop edi; pop esi; pop ebx; mov esp,ebp;
  pop ebp; ret 4`. The ebp frame comes from the 8-byte STACK ALIGNMENT (some local needs 8-alignment -- probably CRightHandedSet or a
  double inside a by-value temp), not necessarily /Oy-. Locals are esp-relative, `parent` is [ebp+8] (read once, 0x00607C32).
- NO SEH frame / no EH state stores although there are many destructible locals => the TU is compiled WITHOUT /EHsc (like the
  CTCInventoryMap siblings; check their `// Flags:`).  `this` spilled to [esp+54h] at entry and reloaded each use.
- Return value: the "UI_LOGBOOK_CATEGORIES" component (edi at 0x00607396, kept in [esp+88h], returned at 0x006096A4).
- Jump table 0x006096B4 (switch on entry category, `cmp eax,7; ja default`): case0 0x0060760F, case1 0x0060762D, case2 -> 0x00607649
  (default, no body), case3 0x00607573, case4 0x00607594, case5 0x006075B5, case6 0x006075D3, case7 0x006075F1.

## 3. Callees (direct; count; landed file; PDB leaf). All landed names agree with the PDB except where flagged.
- 0x0041E5F2 x2 NUISystem::CManager::GetInstance (landed NUISystemCManager_GetInstance_0041e5f2.cpp). First call at entry kept in
  [esp+24h] as `manager`; the UI_SUB_LIST for the books calls GetInstance() again inline (0x00607842).
- 0x0041DB1D x39 CManager::CreateComponent(const CCharString&, bool) `?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@ABVCCharString@@_N@Z`.
- 0x00428494 x2 CManager::CreateMenuEntryComponent(const CWideString&, float, float, const CCharString&, float, const CCharString, float,
  CCharString) -- NO landed row; header NUISystem_CManager.h:97 declares it. Called exactly as in ConstructQuestList:
  (text, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE"[_NEW], 0.0f, "" (0x0122D70E), 0.0f, "PC_BUTTON_BASTARD").
- 0x0099EBF0 x60 CCharString(const char*, long=-1) / 0x0099EAE0 x42 ~CCharString; 0x0099B720 x4 CWideString copy ctor /
  0x0099B510 x6 ~CWideString.
- 0x00449960 CPlayerManager::GetMainPlayer (`?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ`), on [CMainGameComponent 0x013B86A0]+0x1C.
- 0x00619C50 map<ECategory, vector<CHeroLogBookEntry>> COPY ctor (landed stdmap_map_00619c50.cpp, label `std::map::map`) -- the local copy
  of `player.PHeroLogBook->LogEntries` (ptr +0xF8, +0x10 = LogEntries in the retail layout).
- 0x00613AE0 _Rb_tree<ECategory, pair<const ECategory, CComponent*>>::_M_erase (landed) -- inline CategoriesList.clear().
- 0x00618BE0 x8 map<ECategory, CComponent*>::operator[] (landed) -- `CategoriesList[cat] = list`, out of line.
- 0x0047C6A0 _Rb_tree<ECategory, pair<const ECategory, vector<CHeroLogBookEntry>>>::_M_erase (landed) -- the local map copy's dtor (tail).
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (landed) -- expanded GetTC, TCI 0x11 (TC bitset +0x20 bit 17).
- 0x005BF482 CTCInventoryBase::GetAllItems(vector<pair<long,long>>&) const (landed; PDB NInventory:: namespace).
- 0x005D9970 CTCInventoryItem::IsBook(long) static fastcall; 0x005D95C0 GetDescription(long); 0x005D9630 GetDetails(long);
  0x005D96A0 x2 GetEngineGraphic(long); 0x005D9730 GetStartingRightHandedSet(long) (all landed, PDB `SI` static).
- 0x00440990 vector<long>::_M_insert_overflow (landed) -- books.push_back(item.first).
- 0x006869D0 CTCBase::GetDefinitionManager (landed; PDB `IBE` protected const) ; 0x009AD9E0 CDefinitionManager::GetPDefFromGlobalIndex(long)
  (returns CDefPointer by hidden pointer) ; 0x0042B0A2 CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>*) const.
- 0x005DC340 CAnimationSet::GetAnim(const CCharString&, long, const CAnimFilterBase&) const -> const CAnimationEntry& (CAnimationSet.h:25;
  no landed row). Filter temp is a CAnimFilterNoGroup (vtable 0x0122FFDC, base CAnimFilterBase 0x0122FFCC restored at its dtor).
- 0x0042B4C3 x2 list<CComponent*>::_M_insert_dispatch (landed) + 0x0042ABCA ~list (landed) -- the CDeletion by-value copy
  (see OUTLINE "arrow deletion"). 0x00BFEA0E malloc / 0x00BFEA14 free (STLport node alloc), 0x00BFEA1A operator new /
  0x00BFE9BC operator delete (CCountedPointer data block {count, 0x00428B10 DeleteData, ptr}).
- 0x005FAA90 x2 CTCInventoryMap::GetMenuEntryFromCategory(ECategory, CComponent*) (landed).
Virtual slots (offset/4): CComponent 3 FindChild(CCharString) +0x0C, 59 AddChild(CCountedPointer<CComponent>&) +0xEC, 62
SetDeletion(CDeletion) +0xF8, 65 GetType() +0x104, 92 MakeUnavailable() +0x170. CViewport 142 AddFakeChild(CComponent*) +0x238
(ConstructQuestList's description_viewport->AddFakeChild). CText 144 SetValue(const CWideString&, bool, bool) +0x240. CSprite 145
SetGraphicIndex +0x244. CMesh 143 SetMeshIndex +0x23C, 146 SetAnimationIndex +0x248, 147 SetMeshType +0x24C, 148
SetOrientation(CRightHandedSet&) +0x250. CList 145 AddNonScrollingChild +0x244, 152 AddScrollingChildAndSetOffsets +0x260, 156
InitialiseOffsets(unsigned long) +0x270, 158 +0x278 (one long arg = entries-1; by vtable order with MSVC overload reversal this is
SetDefaultChild(long) -- verify against agent4007/test mock, which read 0x278 off retail).

## 4. Members
- this+0x144 = CTCInventoryMap::CategoriesList, `std::map<CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent*>`
  (Egor name/type; Egor offset 0x160 is a different build, retail 0x144 is right: header asserts VisitedRegions 0x134 + vector 0x10 +
  AccumFadeTime 4). The shared header models it as `unsigned char CategoriesList[12]` -> needs an opt-in that retypes it (12 bytes =
  STLport map: header node ptr +0x144, node count +0x148). No FABLE_RETAIL_* macro exists for it yet.
- this+4 = CTCBase owner CThing* (PThing); thing+0x20 TC bitset, thing+0x44 CVectorMap of TCs (expanded GetTC<CTCInventoryBase?>
  with TCI 0x11). Copy the GetTC spelling from the matched ConstructQuestList (FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN /
  FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND); TCI 17 is the inventory TC (check enum ETCInterfaceType for 17).
- CPlayer+0xF8 PHeroLogBook (CCountedPointer<CHeroLogBook>), LogEntries at pointee+0x10 (CHeroLogBook.h: "native LogEntries+10").
- CHeroLogBookEntry is 12 bytes (3 CWideString: copied with three CWideString copy ctors at +0,+4,+8; vector size = (end-begin)/12 via
  0x2AAAAAAB). Map node: key +0x10, vector +0x14/+0x18.
- Thing def from GetPDefFromGlobalIndex: +0x70 CEngineGraphic Graphic (BankIndex +0x70, Type +0x7D compared with 4); CThingBaseDef is
  0x70 bytes so this is the derived thing def's Graphic (CThingPhysicalDef/CThingObjectDef family -- confirm in headers).
- CAppearanceDef +0x34 CAnimationSet; CAnimationEntry +0 read as the animation index.
- ECategory: CATEGORY_QUEST 0, STORY 1, TUTORIAL 2, BASICS 3, OBJECTS 4, TOWNS 5, HERO 6, COMBAT 7 (CHeroLogBook.h).

## 5. Visibility
- map::operator[] 0x00618BE0, both _M_erase, map copy ctor 0x00619C50, list _M_insert_dispatch / ~list are OUT of line (STLport bodies
  expanded elsewhere but called here). CategoriesList.clear() is expanded (size test + _M_erase + header reset). The final `find` on
  CategoriesList (0x00609460..0x0060949E) is EXPANDED (lower_bound loop) -> STLport tree find body visible.
- CCountedPointer<CComponent>(T*) ctor and dtor are expanded everywhere (new {1, DeleteData 0x00428B10, p}); copy ctor expanded
  (`inc [eax]`). Use the same CCountedPointer opt-ins as ConstructQuestList (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA ...).
- CDefPointer release `dec [eax+4]; jne; call [vtbl+4]` expanded (FABLE_RETAIL_DEF_CLASS_BASE / CDEFPOINTER_RELEASE_POINTEE family).
- CDeletion copy is expanded (malloc'd list head + _M_insert_dispatch) -> do NOT enable FABLE_RETAIL_CDELETION_BODIES (that makes the
  ctors out-of-line calls); match ConstructQuestList's `CDeletion deletion; deletion.Method = UI_DELETE_THIS_INSTANCE;
  arrow->SetDeletion(deletion);` (retail stores Method=1 then copies the empty AssociatedParents list).

## 6. Siblings / idiom sources
- MATCHED family model: /opt/fable_lane/w22/agent4007/src/0061b610.cpp (CTCInventoryQuests::ConstructQuestList, same CreateMenuEntryComponent,
  CCountedPointer, MakeUnavailable + SetDeletion arrow, AddFakeChild, InitialiseOffsets idioms) with hdr/ overlay; its macro list is
  the best start (copy hdr/ from agent4007).
- Landed: CTCInventoryMap_ConstructQuestReviewList_005fc2d0.cpp, CTCInventoryMap_ConstructPastQuestsList_005fcbe0.cpp (UI_SUB_LIST +
  CCountedPointer, AddNonScrollingChild, opt-ins listed there), CTCInventoryMap_ConstructRegionDescriptionShops_006001e0.cpp,
  CTCInventoryMap_GetMenuEntryFromCategory_005faa90.cpp (callee, same TU), CTCInventoryMap_OnCreate_00602e30.cpp (0x00602E30, nearest
  landed neighbour before this row).
