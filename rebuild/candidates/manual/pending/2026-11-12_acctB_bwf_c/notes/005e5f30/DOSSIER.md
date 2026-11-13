# DOSSIER 0x005E5F30 CTCQuestCompletionUI::ActivateReal (6580 bytes, no draft)

No earlier scout existed: $W/prior/ is empty, agent2092 (the old chunk owner) left src/ differ/ probe/ empty, and no other
w22 dir mentions 5e5f30. Everything below comes from this scout.

## 1. Retail shape (xdis 005e5f30 0x19c4 = $W/dis_all.txt)
- `push ebp; mov ebp,esp; and esp,-8; sub esp,0x8c; mov eax,[0x139c8a8] (__security_cookie); push ebx/esi/edi;
  mov [esp+0x94],eax`. Epilogue `mov ecx,[esp+0x94]; call 0xBFE9F9 __security_check_cookie; pop edi/esi/ebx; mov esp,ebp; pop ebp; ret`.
  So: **/GS on** (Deactivate 0x5E3DF0 already carries `// Flags: /GS`). `and esp,-8` = aligned frame. That comes from an 8-aligned local
  (none identified for sure. Candidates: a struct with a double or __int64. Check what VC emits before you go looking).
  ebp is the alignment anchor only (no ebp-relative accesses), so this is NOT evidence of /Oy-.
- **No SEH frame** (no fs:0 / state stores) even though CCharString / CWideString / CCountedPointer locals have destructors, so the TU is
  built without /EHsc (as GetQuestDescription 0x5E4BE0: flags only `/d2inlT60 /d2inlS48`).
- thiscall, no arguments, plain `ret` -> `void CTCQuestCompletionUI::ActivateReal()`. this = esi throughout. ebx = 0 for the first part, then
  the frame component.
- 3 jump tables (data right after the `ret` + `nop`):
  T1 0x5E78B4 (switch Completion at 0x5E5FE8, default 0x5E607A): 0->0x5E5FEF 1->0x5E6014 2->0x5E6036 3->0x5E6058
  T2 0x5E78C4 (0x5E622C, default 0x5E6310): 0->0x5E6233 1->0x5E625C 2->0x5E629A 3->0x5E62D5
  T3 0x5E78D4 (0x5E64B4, `ja` 0x5E6692 = no default): 0->0x5E64BB 1->0x5E64BB 2->0x5E6536 3->0x5E65B1 (cases 0 and 1 share a body)

## 2. Name / signature (two sources)
- Ego_r.pdb `?ActivateReal@CTCQuestCompletionUI@@QAEXXZ` (egor_pdb_names 0x00644FA0; egor_signatures) and FableWin.pdb same spelling
  (_fablewin_api / FableWin.exe.signatures 0x022C0DD0, FableWin debug size 12186 bytes). Manifest label is RIGHT.
- Leaf: `?ActivateReal@CTCQuestCompletionUI@@QAEXXZ`: public, non-virtual, `void ()`. It is not in vtable 0x0125589C.
- Only caller is 0x5E7E25 (inside CTCQuestCompletionUI::GlobalUpdate 0x5E78F0, not landed).
- The shared header rebuild/include/engine/CTCQuestCompletionUI.h does NOT declare ActivateReal. The row has to add it (opt-in decl).

## 3. Callees (retail addr, landed file, landed name; PDB spellings from the landed rows' Leaf lines)
| addr | landed row | name / note |
|---|---|---|
| 0x40D2A0 | 00/40/CUserProfileManager_Get_0040d2a0 | CUserProfileManager::Get() (fastcall) |
| 0x406CC0 | 00/40/..._CaptureBackBufferForUIBackground_00406cc0 | CUserProfileManager::CaptureBackBufferForUIBackground() |
| 0x41E5F2 | NUISystemCManager_GetInstance | NUISystem::CManager::GetInstance() (fastcall static), called 14x, NOT cached in some places (see outline) |
| 0x41DB1D | NUISystemCManager_CreateComponent | CManager::CreateComponent(const CCharString&, bool) |
| 0x40F020 | CVectorMap_LowerBound_0040f020 | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (inside the inline CThing::GetTC) |
| 0x5E9760 | stdmaplongunsignedlong_operator_005e9760 | std::map<long,unsigned long>::operator[] (out of line, x3) |
| 0x5E8EB0 | ..._find_005e8eb0 | _Rb_tree<long,pair<const long,ulong>>::find (non-const, x3) |
| 0x5E8F50 | unattributed_Remove005e8f50_005e8f50 | **NAME WRONG**: landed as `CRemovalTable_005e8f50::Remove`; it is STLport `map<long,unsigned long>::erase(iterator)` / `_Rb_tree::erase(iterator)` (body = _Rebalance_for_erase(node,&root,&leftmost,&rightmost); free; --count). Called once (Renown map); the Gold and Experience erases are EXPANDED inline. Put it in NAME_FIXES.tsv (exact PDB spelling not found in the name sets: descriptive). |
| 0x42A34E | stdRbglobalbool_Rebalanceforerase | _Rb_global<bool>::_Rebalance_for_erase (cdecl), from the 2 inline erases |
| 0xBFEA14 / 0xBFEA0E | global_free / global_malloc | STLport node dealloc/alloc (map nodes, list sentinels) |
| 0xBFEA1A / 0xBFE9BC | operator new / operator delete | CCountedPointer count block {RefCount=1, DeleteFunc=0x428B10, Data} |
| 0x429C15 | CCountedPointerNUISystemCComponent_CCountedPointer | ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z, out of line x3 (inline-expanded x3 elsewhere!) |
| 0x429C6F | ..._Reset_00429c6f | ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z (x4) |
| 0x42ABCA | stdlistNUISystemCComponent_list | std::list<CComponent*>::~list (CDeletion dtor) |
| 0x42B4C3 | ..._Minsertdispatch_0042b4c3 | list<CComponent*>::_M_insert_dispatch<const_iterator> (CDeletion copy into the by-value arg) |
| 0x485DBC | stdvectorNUISystemCComponent_Minsertoverflow | vector<CComponent*>::_M_insert_overflow (Components.push_back x2) |
| 0x44C6B0 | CGameDefinitionManager_Get | CGameDefinitionManager::Get() |
| 0x9AD390 | file COptimisedPrimitive_ActualToUniqueIndex_009ad390 | **file label wrong**: manifest says CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, bool). GetQuestDescription 0x5E4BE0 calls it under that name. |
| 0x42B312 | CDefinitionManager_GetDef0042b312 | CDefinitionManager::GetDef<CUIMiscThingsDef>(long, CDefPointer*) (template, out of line) |
| 0x42B040 | CDefinitionManager_GetDefCThingObjectDef | ??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z |
| 0x42B0A2 | CDefClassBase_GetSubDefCAppearanceDef | ??$GetSubDef@VCAppearanceDef@@@CDefClassBase@@QBE_N... |
| 0x5DC340 | **not landed** | CAnimationSet::GetAnim: PDB `?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z` (Ego_r 0x63BD40, FableWin 0x1FA17C0). Declared in engine/CAnimationSet.h. |
| 0x5D9730 | CTCInventoryItem_GetStartingRightHandedSet | static ?GetStartingRightHandedSet@CTCInventoryItem@@SI?AVCRightHandedSet@@J@Z (fastcall, hidden return at esp+0x4c) |
| 0x603000 | **not landed** | `?CreateMiniMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAVCRegion@@@Z` (Ego_r 0x652B10 / FableWin 0x2093750). Currently declared only in engine/CQuestManager.h retail branch; CTCInventoryMap.h lacks it. |
| 0x5BEB57 | CTCInventoryBase_GetMiniMapGraphic | ?GetMiniMapGraphic@CTCInventoryBase@NInventory@@QAEJPBVCRegion@@@Z (this = the CTCInventoryMap, no adjust) |
| 0x4FC190 / 0x4FB490 | CWorldMap_GetRegionNumberMapIsIn / _GetRegion | QBEJJ@Z / ?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z |
| 0x686D30 / 0x686D40 / 0x686A70 / 0x6869D0 | CTCBase_GetWorldMap / GetPlayerManager / GetWorld / GetDefinitionManager | protected CTCBase accessors (out of line, called) |
| 0x449970 / 0x6345C0 | CPlayerManager_GetMainPlayer / CPlayer_AddMode | GetMainPlayer().AddMode((EPlayerMode)0x25, false) |
| 0x6BE030 / 0x6BDFE0 | CMusicManager Clear/SetUIOverridingMusicSet | via GetWorld().GetMusicManager() (CWorld +0x6c, inline) |
| 0x4B2FE0 / 0x4B0160 | CQuestManager AddActivatedQuestName / OnQuestFinishScreenDisplayed | on CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass 0x13B89FC (inline Get()), arg &ScriptName (+0x50) |
| 0x991840 | CASoundManager_GetPSoundBank | PSoundManager(0x13B8394)->GetPSoundBank(1) |
| 0x41649C | CMainGameComponent_AddGameEvent | CMainGameComponent::Get() global 0x13B86A0 |
| 0x9F1760 | CGameEvent_AddData | AddData(1,&bool) = inline AddUBYTE/AddBool (FABLE_RETAIL_CGAMEEVENT_INLINE) |
| 0x9C95E0 | NGameTextCDataBank_GetTextBySymbol | CDataBank::GetTextBySymbol(const CCharString&) const -> CWideString |
| 0x5E4BE0 | CTCQuestCompletionUI_GetQuestDescription | private GetQuestDescription(NUISystem::CComponent*) (same TU, defined earlier) |
| 0x99EBF0 / 0x99EAE0 | CCharString(const char*, long=-1) / ~CCharString | 41 / 19 calls |
| 0x99AED0 / 0x99B6B0 / 0x99B720 / 0x99B510 / 0x99BA70 | CWideString() / (const wchar_t*) / copy / ~ / Sprintf (cdecl) |
| 0xBFEA70 | __ftol2 | (long)float x3 |
Virtual calls (slot = offset/4):
- NUISystem::CManager: +0xA8 slot 42 GetMiscThingsDef() -> CDefPointer<CUIMiscThingsDef> (hidden ret); +0x08 slot 2 AddObserver(CObserver*) (CObservable).
- CComponent: +0x0C slot 3 FindChild(CCharString by value: `push ecx; mov ecx,esp; CCharString ctor`); +0xEC slot 59 AddChild(CCountedPointer&);
  +0xAC slot 43 Initialise(); +0xC0 slot 48 ChangeState(unsigned long) (5); +0xF8 slot 62 SetDeletion(CDeletion by value);
  +0x104 slot 65 GetType(); +0x1F0 slot 124 AddPositionChild(CComponent*) (always on PFrame +0xB8, guarded by `if (PFrame)`).
- CObserver sub-object at +4 of a component: its +0x0C slot 3 ObserveEvent((EEvent)0x19).
- CText (NUISystem_CText.h): +0x240 slot 144 SetValue(const CWideString&,bool,bool); +0x23C slot 143 SetValue(unsigned long textId, const CWideString&, bool).
- CSprite: +0x244 slot 145 SetGraphicIndex(unsigned long).
- CMesh (NUISystem_CMesh.h): +0x23C 143 SetMeshIndex, +0x248 146 SetAnimationIndex, +0x24C 147 SetMeshType(EEngineGraphicType 4), +0x250 148 SetOrientation(CRightHandedSet&).
- frame component (PC_UI_QUEST_*_FRAME): +0x244 slot 145 with CCountedPointer<CComponent>& = NUISystem::CList::AddNonScrollingChild (agent4007 decoded the same slot as AddNonScrollingChild in ConstructQuestList; the class of the frame is unproven).
- CComponentContainer +0x238 slot 142 on the description viewport (with ptr from the container) -- see outline 0x5E7107 (AddFakeChild of CViewport slot 142 is the better fit: arg = CComponent*).
- CASoundManager (PSoundManager 0x13B8394): +0xB8 slot 46 SetGainFader(long bits, float, float) (0x10/0x40/0x80, 0.0f, 500.0f);
  +0x24 slot 9 Get2DEmitter() -> CASoundEmitter&; emitter +0x10 slot 4 (engine/CASoundEmitter.h line 40: ..., const CSoundInit&, bool attached).
- CIEngine (CMainGameComponent::Get()->PDisplayEngine(+0x28)->Engine3D(+0x2c)): +0x130 slot 76 with (0). Name not in CIEngine.h. Check the
  retail CIEngine vtable / CloseInventory 0x56F30B (it does "3D scene rendering back on").
- CDefPointer release idiom (`dec [p+4]; jnz; call [vtbl+4]`) = CDefPointer dtor (intrusive count at +4).

## 4. Members (retail offsets; header engine/CTCQuestCompletionUI.h; real names from struct_layouts_egor, PDB +4 shift after the vector)
this: +0x04 CTCBase::Thing (CThing*), +0x0C Active (=1 at 0x5E75A7), +0x0D ReallyActive (=1 at 0x5E763F), +0x10 Components
(std::vector<CComponent*>, typed only under FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DRAW), +0x20 Completion (long; 0 start 1 complete 2 fail 3 death),
+0x2C PCardDef (CDefPointer<const CQuestCardDef>, typed under _DESCRIPTION), +0x30 Boasts (list<CQuestManager::CBoast>, _DESCRIPTION),
+0x34 RenderedObjectDef, +0x50 ScriptName (CCharString), +0x58 GoldReward, +0x5C RenownReward, +0x60 TargetGoldGained, +0x64 InitialGold,
+0x68 TargetExperienceGained, +0x6C InitialExperience, +0x70 TargetRenownGained, +0x74 InitialRenown, +0x78 ExperienceGainedDisplayed,
+0x7C RenownGainedDisplayed, +0x80 GoldGainedDisplayed (floats), +0x84 StartQuestExperience / +0x90 StartQuestGold / +0x9C StartQuestRenown
(std::map<long,unsigned long>; **opaque `unsigned char[0xc]` in the header, NO typed opt-in yet**: the row must add one, e.g.
FABLE_RETAIL_CTCQUESTCOMPLETIONUI_STARTMAPS, with stlport/fable_map.h), +0xA8 PRenownText, +0xAC PGoldText, +0xB0 PExperienceText (void* in the
header, CText* real), +0xB4 AccumFadeTime (float, zeroed via ebx), +0xB8 PFrame (CComponent* under _DRAW), +0xC4 QuestCardDef (long),
+0xC8 ExperienceCountingSound, +0xCC GoldCountingSound, +0xD0 RenownCountingSound.
Header opt-ins available: FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DRAW (Components vector, PFrame/PBlack typed), _DESCRIPTION (PCardDef, Boasts,
the CWideStrings, ScriptName, GetQuestDescription decl), _POSTBOAST (ScriptName), _VSLOTS. Missing: ActivateReal decl, typed maps, typed CText* members.
Other classes:
- CThing (engine/CThing.h): +0x20 UsingComponents bitset (bit 4 = TCI_HERO_STATS, bit 27 = TCI_INVENTORY_MAP tested as `test [edi+0x20],0x8000000`;
  bit 104 TCI_HERO_EXPERIENCE = `[+0x2c]>>8`), +0x44 Components CVectorMap, +0x8E CurrentMapNumber (short, movsx).
  FABLE_RETAIL_CTHING_GETTC_EXPANDED gives the LowerBound expansion that matches retail. HasTC result materialised in a byte (`shr; and cl,1;
  mov [esp+0x10],cl`) = the `const bool& found` HasTC variant (CThing.h:298, only under FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE) OR plain inline
  HasTC after the budget is spent (CThing.h:301-306 note). The death-path TCI_INVENTORY_MAP test is in place (`test dword`) instead.
- CTCHeroStats (engine/CTCHeroStats.h): +0x48 TotalMoneyEarned, +0x7C RenownTotal. CTCHeroExperience: +0x10 TotalExperiencePoints.
- CUIMiscThingsDef (NUISystem_CUIMiscThingsDef.h, named): +0xC0 CoreGraphic, +0xC4 VignetteGraphic, +0xC8 OptionalGraphic, +0x1AC CountUpSound
  (CCharString), +0x204 QuestStartScreenMusic, +0x208 QuestCompleteScreenMusic, +0x20C QuestFailureScreenMusic, +0x210 DeathScreenMusic.
- CQuestCardDef (retail = PDB-4 after RewardObjects): +0x28 QuestName (long text id), +0x58 IsCore, +0x59 IsVignette.
- CThingObjectDef: +0x70 Graphic.BankIndex, +0x7D Graphic.Type (CEngineGraphic.Type uchar, ==4). CAppearanceDef +0x34 Animation (CAnimationSet).
- CMainGameComponent (global ptr 0x13B86A0): +0x14 PTextBank object word (GetTextBank()), +0x28 PDisplayEngine; CDisplayEngine +0x2C Engine3D.
- CSoundInit (engine/CSoundInit.h): ctor variant at line 38 (no GainFaderBits init) matches the store order 0x4C,50,54,58,64,68 then
  PlaybackType(+0x10)=1 and GainFaderBits(+0x14)=0x20 assigned.
- CGameEvent (engine/CGameEvent.h, FABLE_RETAIL_CGAMEEVENT_INLINE): Type 9, Player -1, EndPos 0, Valid 1, Replacement 0, then AddUBYTE(1).
- CAnimFilterNoGroup (engine/CAnimFilterNoGroup.h, vtable 0x122FFDC, base CAnimFilterBase 0x122FFCC): stack temp, inline ctor/dtor.
- NUISystem::CDeletion (NUISystem_CDeletion.h): implicit ctor/copy (expanded: malloc sentinel); do NOT define FABLE_RETAIL_CDELETION.

## 5. Visibility / inline facts
- Same TU (source file) as GetQuestDescription 0x5E4BE0 (defined just before, called), Activate 0x5E4430/0x5E4740, SaveInitialValues 0x5E4860,
  Copy 0x5E49E0; GlobalUpdate 0x5E78F0 follows. The STLport COMDATs that follow (0x5E8E..0x5E98xx: find 0x5E8EB0, erase 0x5E8F50,
  operator[] 0x5E9760) are this TU's map<long,unsigned long> instantiations: operator[] and find are called, erase is called once and expanded twice.
- CCountedPointer<CComponent>(CComponent*) is expanded inline at 0x5E6797, 0x5E69E6, 0x5E6C63 (new(0xc) count block, DeleteFunc 0x428B10) and called
  out of line (0x429C15) at 0x5E7162, 0x5E7352, 0x5E7385. Reset (0x429C6F) is always called. CCountedPointer release/dtor always expanded.
- GFIntToWideString (inline, defined in the GetQuestDescription row: `CWideString temp; temp.Sprintf(L"%d", n); return temp;`) expanded 3x
  (Sprintf + copy ctor + ~temp).
- /d2inlT60 /d2inlS48 are the TU-mate's flags (GetQuestDescription). Start with them plus /GS. Lesson c2058 / f4007: calls in place of expansions
  (or the reverse) are budget effects. Change the branch structure before you change flags.
- Opt-in macros seen in the TU mates: FABLE_RETAIL_STLP_MAP, _CTCQUESTCOMPLETIONUI_DESCRIPTION/_DRAW, _CTCBASE, _DEF_CLASS_BASE,
  _CDEFSTRINGTABLE, _CCHARSTRING, _CWIDESTRING, _CWIDESTRING_NULLSTRING, _CQUESTMANAGER, _CGAMEDEFINITIONMANAGER, _CDEFINITIONMANAGER,
  _CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, _CDEFCLASSBASE_GETSUBDEF_BODY, _CRESOURCELIST, _CENGINEPERSISTENTPRIMITIVEBASE, _CMAINGAMECOMPONENT,
  _CMAINGAMECOMPONENT_TEXTBANK, _CTHING, _CTHING_GETTC_EXPANDED (Activate 0x5E4740). The STLport erase: FABLE_RETAIL_STLP_TREE_ERASE +
  FABLE_RETAIL_STLP_TREE_ERASE_BODY (fable_map.h:282-298). Sound: CASoundManager.h; CGameEvent: FABLE_RETAIL_CGAMEEVENT_INLINE
  (as CTCInventoryMap::TeleportHeroToBoastingArea 0x5FB4B0).

## 6. Sibling rows to copy idioms from
- rebuild/src/compiled/00/5e/CTCQuestCompletionUI_GetQuestDescription_005e4be0.cpp: same TU, same family (CreateComponent / FindChild /
  CText::SetValue / GFIntToWideString / rTextBank / pmiscthingsdef / GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF",1)). Flags /d2inlT60 /d2inlS48;
  the full opt-in list is in its lines 8-38. **This is the template.**
- .../CTCQuestCompletionUI_Activate_005e4740.cpp: CThing GetTC expanded (FABLE_RETAIL_CTHING, _GETTC_EXPANDED, _CCHARSTRING, _CWIDESTRING).
- .../CTCQuestCompletionUI_DrawPreparePrimitivesForRendering_005e3f00.cpp: _DRAW opt-in, PFrame / Components typed.
- .../CTCQuestCompletionUI_SaveInitialValues_005e4860.cpp: the same three StartQuest map stores (renown 4/+0x7c, gold 4/+0x48, exp 0x68/+0x10) but
  modelled with LOCAL stand-ins (legacy); use it only for statement order: value-before-key for renown, `long key` first for the others.
- 00/56/CTCInventoryExperience_OpenInventory_0056fad7.cpp: PSoundManager->SetGainFader(0x10,0.0f,500.0f) idiom, CASoundManager.h; the
  CloseInventory 0x56F30B row plays a sound through Get2DEmitter.
- /opt/fable_lane/w22/agent4007 (ConstructQuestList, matched): hdr/ overlay (CList slots 0x240/0x244), tools/cd.sh (direct call compare), bt.sh.

## Open points for the author
- The 8-byte-aligned local behind `and esp,-8`.
- CIEngine slot 76 (+0x130) name.
- Exact class of the frame component (slot 145 AddNonScrollingChild) and of the description viewport (slot 142 +0x238).
