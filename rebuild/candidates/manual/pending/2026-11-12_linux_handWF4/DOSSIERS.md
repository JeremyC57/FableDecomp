## 0x004B3CE0

# DOSSIER 0x004B3CE0 -- CQuestManager::ActivateQuests (identity scout)

## 1. Retail shape (python3 $S/w17/xdis.py 004b3ce0 0x515; oracle $W/oracle.tsv)
- 0x004B3CE0..0x004B41E4, `ret 4`, int3 pad to 0x004B41F0. __thiscall (this=ecx -> ebp), 1 stack arg (quests -> ebx).
- NO frame pointer (`sub esp,34h; push ebx/ebp/esi/edi`), NO SEH prologue (no /EHsc frame although CCharString temps
  and counted pointers have dtors -> TU built without EH unwinding for this function, like the rest of the CQuestManager TU),
  speed-optimised (0x2aaaaaab magic divide for /12 element count, `lea ecx,[ecx]` loop alignment at 0x4B41BD).
- Locals: [esp+2c/30/34] = vector<CCharString> questsToLoad (start/finish/eos), [esp+14] loop counter i (pass 1),
  [esp+10] byte offset i*12 (pass 1) / id / quest Data (pass 2), [esp+18] byte offset (pass 2), [esp+1c] counter j,
  [esp+14] persistScript ptr (pass 2), [esp+20] script ptr, [esp+28] count block (bare branch), [esp+3c..44] C3DVector.
- Register roles: pass 1 this=ebp, quests=ebx, info=esi; pass 2 info=edi, ebx = ZERO register (`xor ebx,ebx` at 0x4B3EA9)
  and later count-block; quests re-read from the PARAM slot [esp+48] at 0x4B4176 (it spills).
- Loop form: both loops `if (size()) { i=0; do { ... } while (++i < size()); }` -- size() re-evaluated each iteration
  ((finish-start)/12 via imul), index-scaled via a separate running byte offset += 0xC (so `quests[i]` with a
  strength-reduced offset, not a pointer walk). Unsigned compare (`jb`).
- Pass-1 structure: `if (!IsQuestActive(n) && set.find(n) == set.end())` -- end() (`mov edi,[ebp+9Ch]`) loaded BEFORE the
  find call (LESSONS.md hand322: free `operator==(const it&,const it&)` gives this order). Then
  `if (info.LoadResources) { questsToLoad.push_back(n) (inline, _M_insert_overflow 0x433530 on growth with args (pos, x, tag, 1, true));
  set.insert(n) (0x4AC380, hidden pair return in [esp+24]) }`; then code-section: inline CTBaseSingleton<CCodeSectionManager>::Get()
  (`mov eax,[13CAA68h]; test; jne` .. `push 2Ch; call new; ctor 0x9F0320 or 0; mov ecx,13CAA68h; call Reset 0x413590`;
  then `mov edi,[13CAA68h]`), `if (mgr.IsSectionRegisteredLongName(n))` (by-value copy built in the outgoing slot: `push ecx; mov ecx,esp; push esi; call copy`),
  then a NAMED local copy at [esp+18] (`CCharString name = n`?) copied again by value into GetHandleFromLongName, `LoadSection(handle)`,
  ~name. NOTE: the local copy at [esp+18] then a second by-value copy from it -> source likely
  `CCharString section_name = info.QuestName; mgr.LoadSection(mgr.GetHandleFromLongName(section_name));` (or a helper taking by value).
- After pass 1: `if (questsToLoad.begin() != questsToLoad.end())` (cmp [esp+2c],[esp+30] -> i.e. `!empty()`) LoadResourcesForQuests(questsToLoad).
- Pass 2 per element: `if (IsQuestActive(n)) continue;` -> inline persist lookup on RuntimeQuestPersistence.PersistScripts (this+0xC):
  LowerBound 0x4B97D0, `last` (= [map+4]) cached in edx/[esp+14], (it==last || key<it->first) ? last : it, then
  `found != last ? &found->second.PersistScript (+8) : 0` -- exactly the inline
  `const CCharString* GetQuestScriptPersistScript(const CCharString&) const` (the "redundant compare" is that accessor's own test).
- Then `if (info.PScriptInfo == 0 || !g_scripts_enabled(0x01375454 byte)) -> bare branch (0x4B4063)` else scripted branch.
- Scripted: `id = CurrentQuestID++` (+0x84, old value kept); scriptData=0 (esi); if (PScriptInfo->MasterScript +0x10):
  `scriptData = PScriptInfo->MasterScriptDataAlloc()` (call [eax+4], no args, no cleanup), `scriptData->vslot2()` (call [edx+8],
  thiscall, 0 args), `LoadMasterScriptDataFromGameState(*scriptData, info.QuestName)` (0x4B3760), master=scriptData;
  else master=GetMasterScriptDataForScript(*info.PScriptInfo) (0x4AFA10).
  script = PScriptInfo->AllocFunc(master, World->GetGameScriptInterface()) -- called with ecx=master, edx=[World+0x38], NO
  stack args, no cleanup => retail function-pointer type behaves __fastcall (header CScriptInfo.h declares __cdecl -> mismatch!).
  new(0x34) CActiveQuest(id, info.QuestName, *info.PScriptInfo, script, scriptData, info.LoadResources) out of line 0x4B0310
  (bool pushed via `xor edx,edx; mov dl,[edi+9]; push edx`); null -> Data slot [esp+10]=0.
  CCountedPointer<CActiveQuest> built INLINE (new(0xC) block {1, 0x4BAEF0, obj}); list push_back INLINE (malloc(0x10) node,
  copy counted ptr into node+8 with ++refs, link before head [this+0x38]); then set.erase(n) OUT OF LINE 0x4BB270;
  script->Activate() 0x0CB7900 (script from [esp+20]); `if (persist && quest->PScript) quest->PScript->LoadGameState(*persist)`;
  AddMessage(0x37, 0x32, C3DVector(0,0,0), 0, 0, 0.0f, 0.0f, GetIndexFromRegisteredQuest(n)) on [World+0x60]; ~counted ptr inline
  (dec, destroy fn [blk+4](ecx=[blk+8]), operator delete 0xBFE9BC -- tail-merged with the bare branch's release at 0x4B416E).
- Bare: id=CurrentQuestID++; new(0x34) with INLINE ctor CActiveQuest(long, const CCharString&, bool): stores ID, +4..+0x18 = 0,
  +0x24=1 (Persistent), +0x25=LoadResources, +0x28/+0x2c=0, QuestName copy +0x30 (MarkerPosition +0x1C/+0x20 untouched);
  counted ptr inline; list push_back inline; then set range erase INLINE: `_M_upper_bound` 0x4B9D50 called FIRST, `_M_lower_bound`
  0x4B9D00 second (right-to-left pair args = equal_range), erase(first,last) 0x4B9C10 (iterators passed by value via push ecx/mov ecx,esp);
  NO distance call -> not the inline erase(key) (0x4BB270 calls __distance 0x4B84A0). Then persist LoadGameState as above, NO AddMessage,
  NO Activate.
- Epilogue: inline ~vector<CCharString> (loop ~CCharString 0x99EAE0, free 0xBFEA14 if start).

## 2. Identity (manifest label is RIGHT)
- Mangled leaf: `?ActivateQuests@CQuestManager@@AAEXABV?$CArray@VCActivateQuestInfo@CQuestManager@@@@@Z`
  = private: void __thiscall CQuestManager::ActivateQuests(const CArray<CQuestManager::CActivateQuestInfo>&).
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r addr 0x004B9520, not retail), egor_signatures.tsv, _fablewin_api.tsv +
  naming_stragglers/FableWin.exe.signatures.tsv (FableWin.pdb, 2 hits). Retail address tie: only caller 0x004B4386 lies in
  ActivateMultipleQuests 0x004B4260 (`?ActivateMultipleQuests@CQuestManager@@QAE_NABV?$CArray@VCCharString@@@@_N1@Z`, header
  CQuestManager.h, not landed); familypass5/7 labels name 0x4B3CE0 ActivateQuests as caller of the set helpers; decomp shape (CArray of
  12-byte {QuestName, PScriptInfo, GameScript, LoadResources} = Ego_r CActivateQuestInfo) matches. Not virtual (no vtable slot).
- CActivateQuestInfo is NESTED in CQuestManager (mangling `VCActivateQuestInfo@CQuestManager@@`), Ego_r layout 12 bytes:
  QuestName +0 CCharString, PScriptInfo +4 CScriptInfo*, GameScript +8 bool, LoadResources +9 bool. Draft names it top-level: WRONG.

## 3. Callees (landed file under rebuild/src/compiled/; PDB name; draft spelling)
| addr | landed file | PDB / correct | draft | flag |
|---|---|---|---|---|
| 004AF610 | 00/4a/CQuestManager_IsQuestActive_004af610.cpp | ?IsQuestActive@CQuestManager@@QBE_NABVCCharString@@@Z | IsQuestActive const | ok |
| 004B9370 | 00/4b/stdRbtree_Mfind_004b9370.cpp | _Rb_tree<CCharString,CCharString,_Identity..>::_M_find<CCharString> const (set::find inline -> _M_find out of line) | CStringSet::_M_find | model with real set (fable_map.h FABLE_RETAIL_STLP_SET / _SET_FIND, TREE_MFIND_DECL) |
| 004AC380 | 00/4a/stdRbtree_insertunique_004ac380.cpp | _Rb_tree::insert_unique(const V&) (set::insert) | CStringSet::insert | ok |
| 00433530 | 00/43/stdvector_Reallocate_00433530.cpp | vector<CCharString>::_M_insert_overflow | ok | header FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY / PUSHBACK_* |
| 0099EC30 / 0099EAE0 | 00/99/CCharString_CCharStringcopy_0099ec30.cpp / CCharString_CCharString_0099eae0.cpp | CCharString copy / dtor | ok | |
| 00BFEA1A / 00BFE9BC / 00BFEA0E / 00BFEA14 | 00/bf/global_operatornew / operatordelete / malloc / free | operator new/delete, malloc/free | draft `operator_new` free function | use real `new` |
| 009F0320 | 00/9f/CCodeSectionManager_CCodeSectionManager_009f0320.cpp | ??0CCodeSectionManager@@QAE@XZ | ok | |
| 00413590 | 00/41/CCountedPointerCCodeSectionManager_Reset_00413590.cpp | ?Reset@?$CCountedPointer@VCCodeSectionManager@@@@QAEXPAVCCodeSectionManager@@@Z | "CCodeSectionManagerPointer::Reset" | WRONG: it is CTBaseSingleton<CCodeSectionManager>::Get() inline (engine/CTBaseSingleton.h), static PClass 0x013CAA68; out-of-line Get COMDAT 0x00414CE0 exists but retail inlines it here |
| 009F0570 | 00/9f/CNameRegistry009f0570_Contains_009f0570.cpp (landed `CNameRegistry_009f0570::Contains`) | most likely ?IsSectionRegisteredLongName@CCodeSectionManager@@QAE_NVCCharString@@@Z (by-value string, bool; this = the code section manager) | Contains | WRONG name (landed one too -> NAME_FIXES candidate; verify vs GetHandleFromLongName sibling 0x9F0500) |
| 009F0500 | 00/9f/CCodeSectionManager_GetHandleFromLongName_009f0500.cpp | ?GetHandleFromLongName@CCodeSectionManager@@QAEKVCCharString@@@Z | GetSectionHandle | WRONG |
| 009F02C0 | 00/9f/CCodeSectionManager_LoadSection_009f02c0.cpp | ?LoadSection@CCodeSectionManager@@QAEXK@Z | ok | |
| 004B2510 | 00/4b/CQuestManager_LoadResourcesForQuests_004b2510.cpp | ?LoadResourcesForQuests@CQuestManager@@AAEXABV?$CArray@VCCharString@@@@@Z | takes CStringVector | param is CArray<CCharString> (engine/CArray.h) |
| 004B97D0 | 00/4b/CVectorMapCCharStringCQuestMapValue...LowerBound_004b97d0.cpp | CVectorMap<CCharString,CPersistInfo>::LowerBound (fastcall `ret 4`) | LowerBound_004b97d0 | |
| 00429950 | 00/42/CBasicStringchar_operator_00429950.cpp | CBasicString<char>::operator< | CCharStringData::operator< | |
| 004B3760 | 00/4b/CQuestManager_LoadMasterScriptDataFromGameState_004b3760.cpp | ?LoadMasterScriptDataFromGameState@CQuestManager@@AAEXAAVCScriptDataBase@@ABVCCharString@@@Z | LoadActiveQuest(CScriptDataBase*, const CActivateQuestInfo&) | WRONG name and types (ref, quest name) |
| 004AFA10 | 00/4a/CQuestManager_GetMasterScriptDataForScript_004afa10.cpp | ?GetMasterScriptDataForScript@CQuestManager@@AAEPAVCScriptDataBase@@ABVCScriptInfo@@@Z | takes CScriptInfo* | param is const CScriptInfo& |
| 004B0310 | 00/4b/CActiveQuest_CActiveQuest_004b0310.cpp | ??0CActiveQuest@CQuestManager@@QAE@JABVCCharString@@ABVCScriptInfo@@PAVCScriptBase@@PAVCScriptDataBase@@_N@Z | 4th? arg CScriptInfo* | scriptInfo is const CScriptInfo&; nested CQuestManager::CActiveQuest |
| inline | -- | ??0CActiveQuest@CQuestManager@@QAE@JABVCCharString@@_N@Z (inline in retail) | ok | |
| 004BAEF0 | 00/4b/CTCMapwho_OnDie_004baef0.cpp (landed name OnDie_3, manifest DeleteObject_CActiveQuest_004baef0) | CCountedPointer<CQuestManager::CActiveQuest> delete-data fn | Destroy | landed name is clearly wrong (ICF/propagated) |
| 004BB270 | 00/4b/stdRbtreeCCharStringpairconstCCharStringlong_erase_004bb270.cpp | _Rb_tree::erase(const key_type&) (labelled for map<CCharString,long>; body identical for set<CCharString> -> folded) | CStringSet::erase(key) | |
| 004B9D50 / 004B9D00 | 00/4b/stdRbtree_Mupperbound_004b9d50.cpp / TreestdTmaptraitsCCharString_Lbound_004b9d00.cpp | _Rb_tree::_M_upper_bound / _M_lower_bound (const) | _Ubound/_Lbound | landed 4b9d00 name `Tree_Lbound` is MS-STL style, wrong |
| 004B9C10 | 00/4b/stdRbtreeCCharStringpairconstCCharStringlong_erase_004b9c10.cpp | _Rb_tree::erase(iterator,iterator) | ok | |
| 00CB7900 / 00CB8690 | 00/cb/CScriptBase_Activate_00cb7900.cpp / CScriptBase_LoadGameState_00cb8690.cpp | ?Activate@CScriptBase@@QAEXXZ / ?LoadGameState@CScriptBase@@QAEXABVCCharString@@@Z | ok | engine/CScriptBase.h (needs FABLE_RETAIL_NSCRIPT) |
| 004AF740 | 00/4a/CQuestManager_GetIndexFromRegisteredQuest_004af740.cpp | ?GetIndexFromRegisteredQuest@CQuestManager@@QBEJABVCCharString@@@Z (const) | non-const | add const |
| 00687540 | 00/68/CMessageEventManager_AddMessage_00687540.cpp | ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z | type long | first param EMessageEventType (engine/CMessageEventManager.h already declares it) |
| vslot +8 | -- | CScriptDataBase virtual slot 2 on the fresh master data (draft "Init") | Init | unproven name: check CScriptDataBase vtable in rtti-vtables.tsv |
| 01375454 (data) | -- | name NOT found in any PDB dump / labels (only GConsoleSetRunScripts console fn exists) | g_bScriptsEnabled | made-up; mark descriptive |

## 4. Members (shared header rebuild/include/engine/CQuestManager.h, branch FABLE_RETAIL_CQUESTMANAGER; Ego_r names)
- +0x0C RuntimeQuestPersistence (NQuestManager::CRuntimeQuestPersistence, 0x20; PersistScripts CVectorMap +0, MasterData +0x10):
  typed under FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE but OPAQUE (`_pad_0x00[0x20]`), no GetQuestScriptPersistScript decl.
  Model of the inline lookup: landed twin 00/4b/NQuestManagerCRuntimeQuestPersistence_GetQuestMasterDataPersistScript_004b27f0.cpp.
- +0x38 ActiveQuests std::list<CCountedPointer<CActiveQuest> >: typed only under _LOADGAMESTATE+_ACTIVEQUEST+_SAVEGAMESTATE (or BOASTUI/POSTBOAST).
- +0x7C World CWorld* (typed under LOADGAMESTATE); CWorld.h: +0x38 PGameScriptInterface_ptr via inline GetGameScriptInterface(),
  +0x60 PMessageEventManager_Data via inline GetMessageEventManager() (FABLE_RETAIL_CWORLD).
- +0x84 CurrentQuestID long and +0x9C QuestsWithResourcesLoadedThatAreNotActivated set<CCharString>: NOT typed in any branch
  (padding `_pad_0x80[0x8f-0x80]` and `_pad_0x9c[...]` under LOADGAMESTATE) -> the author needs a new nested opt-in
  (e.g. FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUESTS) typing CurrentQuestID, the set, CActivateQuestInfo, ActivateQuests and the
  callee declarations (IsQuestActive, LoadResourcesForQuests, LoadMasterScriptDataFromGameState, GetMasterScriptDataForScript,
  GetIndexFromRegisteredQuest) plus CRuntimeQuestPersistence::GetQuestScriptPersistScript inline.
- CActiveQuest +0..+0x30 is in the header under FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST (private members; SAVEGAMESTATE adds
  `friend class CQuestManager`); needs the inline (long, const CCharString&, bool) ctor and the out-of-line 6-arg ctor declared.
- CScriptInfo (engine/CScriptInfo.h): AllocFunc +0, MasterScriptDataAlloc +4, Name +8, ID +0xC, MasterScript +0x10 -- AllocFunc
  declared __cdecl but retail passes ecx/edx (see 1).
- Locally modelled in the draft although a shared header exists: CCharString (fable_string.h), CCountedPointer
  (engine/CCountedPointer.h), CArray (engine/CArray.h), CScriptInfo, CScriptBase, CMessageEventManager, CWorld, CQuestManager,
  CActiveQuest (nested in CQuestManager.h), the set/vector/list (stlport/fable_map.h, fable_containers.h), C2DCoordI, CTBaseSingleton.

## 5. Callee-body visibility / opt-ins
- Same TU (CQuestManager.cpp, retail 0x004AF000..0x004BBxxx, defined earlier): IsQuestActive 0x4AF610, GetMasterScriptDataForScript
  0x4AFA10, GetIndexFromRegisteredQuest 0x4AF740, CActiveQuest 6-arg ctor 0x4B0310, LoadResourcesForQuests 0x4B2510,
  LoadMasterScriptDataFromGameState 0x4B3760 -- all called out of line; their bodies may have been visible (mod/ref effects).
- STLport out-of-line COMDATs after the function: _M_find 0x4B9370, _M_upper/_lower_bound 0x4B9D50/0x4B9D00, erase range 0x4B9C10,
  erase(key) 0x4BB270, LowerBound 0x4B97D0. Before: insert_unique 0x4AC380, _M_insert_overflow 0x433530.
- Opt-ins: stlport/fable_map.h FABLE_RETAIL_STLP_SET, _SET_FIND, _SET_RANGE, _SET_UPPER_BOUND, _TREE_MFIND_DECL, _TREE_ERASE,
  _TREE_ERASE_KEY (visible erase(key)/equal_range/erase(range) bodies -- note retail inlines equal_range+erase(range) in the bare
  branch but calls erase(key) out of line in the scripted branch), _TREE_CONSTFIND; fable_containers.h FABLE_RETAIL_STLP_VECTOR,
  _VECTOR_OVERFLOW_DECL_ONLY, _LIST, _LIST_PUSHBACK_NOINLINE (do NOT use: push_back is inline here), _LIST_INSERT_*;
  engine/CCountedPointer.h FABLE_RETAIL_CCOUNTEDPOINTER (+ _RELEASEDATA, _VISIBLE_CALLED_CTOR/DTOR); engine/CArray.h FABLE_RETAIL_CARRAY;
  CWorld.h FABLE_RETAIL_CWORLD; CScriptBase.h FABLE_RETAIL_NSCRIPT; CTBaseSingleton.h (Get inline, Reset out of line).

## 6. Sibling landed rows to copy idioms from
- 00/4b/CQuestManager_SaveGameState_004b5ee0.cpp -- defines FABLE_RETAIL_CCHARSTRING, _CINTELLIGENTPOINTER, _CTHING, _CTHINGMANAGER,
  _CCLASSFACTORY, _CWORLD, _NSCRIPT, _CQUESTMANAGER, _CQUESTMANAGER_LOADGAMESTATE, _ACTIVEQUEST, _SAVEGAMESTATE; includes
  CWorld.h/CScriptInfo.h/CScriptBase.h/CQuestManager.h. Closest layout model (ActiveQuests list, World, CActiveQuest friend).
- 00/4b/CQuestManager_LoadGameState_004b5500.cpp -- CQUESTMANAGER + LOADGAMESTATE, FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE,
  `#pragma auto_inline(off/on)` around stand-ins.
- 00/4b/CQuestManager_ActivateQuest_004b4a10.cpp -- vector<CCharString> push_back -> _M_insert_overflow idiom, same-TU stand-in bodies.
- 00/4b/NQuestManagerCRuntimeQuestPersistence_GetQuestMasterDataPersistScript_004b27f0.cpp -- the exact persist-lookup shape.
- 00/4b/CQuestManager_LoadResourcesForQuests_004b2510.cpp -- `#define CQuestManager CQuestManagerDonor` + header pattern.

## 7. Old draft model errors (judged against disassembly)
1. Everything is local structs; strict verifier rejects (CQuestManager, CArray, CCountedPointer, CScriptInfo, CWorld headers exist).
2. Wrong callee names/types: LoadActiveQuest -> LoadMasterScriptDataFromGameState(CScriptDataBase&, const CCharString&);
   GetSectionHandle -> GetHandleFromLongName; Contains -> IsSectionRegisteredLongName (probable); GetCodeSectionManager +
   CCodeSectionManagerPointer -> CTBaseSingleton<CCodeSectionManager>::Get(); CActiveQuest ctor / GetMasterScriptDataForScript take
   const CScriptInfo&; GetIndexFromRegisteredQuest const; LoadResourcesForQuests takes const CArray<CCharString>&; AddMessage first
   param EMessageEventType; CActivateQuestInfo/CActiveQuest nested in CQuestManager.
3. AllocFunc modelled __fastcall (matches the bytes) but the shared CScriptInfo.h says __cdecl -- needs a retail opt-in in CScriptInfo.h
   or the call shape will differ.
4. Residue per STATUS: this/quests register roles swapped; retail spills `quests` to the param slot in pass 2 and uses ebx as a
   zero register there -- typical of different live ranges: try `const CActivateQuestInfo& info = quests[i]` re-indexed each pass
   with the size() re-read (retail re-reads [ebx]/[ebx+4] at the loop bottom), and the real CArray/vector size().
5. The `CCharString name` copy in pass 1 then a second by-value copy: draft has it; keep it (retail has both copies).
6. Counted pointer: draft builds CCountedPointer inline with placement new over operator_new -- use the shared CCountedPointer ctor
   (block {refs=1, delete fn 0x4BAEF0, obj}); retail null-checks the CActiveQuest new and zeroes the Data slot.
7. Bare-branch erase: draft spells _Lbound/_Ubound explicitly -- the real spelling is likely `equal_range` inline + erase(first,last)
   (upper_bound evaluated first) or `erase(lower_bound(n), upper_bound(n))`; scripted branch is plain `erase(n)` out of line.

## 0x00A90040

# DOSSIER 0x00A90040 -- N3DPrimitive::CAnimatedBlock::ConvertToVBFormatBump (403 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 00a90040 0x1a3)
- __thiscall, `ret 8` (2 stack args), NO frame pointer (`sub esp,2Ch` first; no push ebp/mov ebp,esp) -> default /Oy, no pragma.
- No SEH prologue, no calls at all (leaf; every helper inlined). Frame 0x2C (sibling NonBump 0x00A8FF50 has 0x1C; extra
  slots here = cross-product temps cx [esp+18h], cy [esp+1Ch] after 4 pushes; dead Z stores [esp+2Ch], [esp+38h]).
- Pushes: ebx, esi, (count>0) ebp, edi. `this` stays in ECX the whole function (sibling moves it to ebp; here ebp = index).
- count = (VertexList._M_finish - _M_start) >> 2 (`sar ebx,2`), held in ebx, spilled [esp+0Ch]; i counter on stack [esp+10h].
- Out cursor edx = out+8, `add edx,38h` per vertex (sizeof CTVertexNormBlend4Tex1Bump = 0x38). index (VertexList[i]) in ebp,
  spilled into arg1's slot [esp+40h] across the bone loop, reloaded for tangent `lea eax,[ebp+ebp*2]` (index*12).
- Position: `fld X; mov eax,Z; fld Y; ... mov [edx],eax; fstp; mov [esp+2Ch],eax (dead store); fstp` = by-value
  `const C3DVector GetPosition()` through the memberwise copy ctor, exactly the NonBump sibling pattern. Normal same
  (Z via eax, dead store [esp+38h]); U/V copied as dwords (GetU/GetV).
- Bone loop: `movzx eax,[ecx+14h]` (BonesPerVertex re-read each iter), `imul eax,i ; add eax,j ; lea ebp,[BoneWeights+eax*8]`,
  `mov al,[ebp]; mov bl,3; imul bl` -> (uchar)(BoneIndex*3) to +0x0C+j, byte [ebp+4] (Weight) to +0x10+j; then pad loop j<4 zeros.
  Identical to landed sibling 0x00A8FF50.
- Tangent: eax = mesh.UTangents._M_start + index*12 (mesh+0x38). 3 dwords copied int-wise to out+0x28/+0x2C/+0x30 (UTangent[3]).
- Handedness: re-load source Normal (fld [edi+0Ch],[+10h],[+14h]) and compute Cross(N, U) in default order
  (Y*oZ-Z*oY, Z*oX-X*oZ, X*oY-Y*oX), then Dot with the SAME UTangent (eax unchanged; mesh+0x44 VTangents is never read --
  retail bug/quirk, must be reproduced), summed Z first: cz*Tz + cy*Ty, + cx*Tx  -> Dot ordered Z,Y,X.
  `fcomp [0x0122DEDC]` (0.0f, verified in PE) ; `test ah,1 ; jne -> -1.0f`, fallthrough stores +1.0f (0x3F800000) to +0x34.
  Fallthrough=+1 means the source condition is `>= 0.0f` -> 1.0f else -1.0f (same as StaticBump draft 0x00A8F1B0 line 23).

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @Ego_r 0x00BD1EC0) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x017CE7D7):
  `?ConvertToVBFormatBump@CAnimatedBlock@N3DPrimitive@@AAEXPAVCTVertexNormBlend4Tex1Bump@@AAVCDeviceIndependent@2@@Z`
  = private: void __thiscall N3DPrimitive::CAnimatedBlock::ConvertToVBFormatBump(CTVertexNormBlend4Tex1Bump*, N3DPrimitive::CDeviceIndependent&).
  Note: the second arg is N3DPrimitive::CDeviceIndependent (the MESH data, 100B Ego_r), NOT CAnimatedBlock::CDeviceIndependent;
  passed by non-const REFERENCE (manifest functions.tsv says pointer -- wrong). Non-const method, private.
- Manifest label `CAnimatedBlock::ConvertToVBFormatBump` is right except the missing N3DPrimitive namespace.
- Not a vtable slot. Only caller: 0x00A9130C in landed C3DPrimitive2::ConvertToVBFormatAnimated
  (rebuild/src/compiled/00/a9/C3DPrimitive2_ConvertToVBFormatAnimated_00a91180.cpp), branch `flags & 2`, passes C3DPrimitive2::Data (+0x2C).
- NAME_FIXES: that caller declares 0x00A90040 as `CAnimatedBlock::TransformVerticesWithNormals(CAnimatedVertexWithNormals*, const CDeviceIndependent*)`
  and 0x00A8FF50 as `TransformVertices(CAnimatedVertex*, const CDeviceIndependent*)` -- both made-up; PDB names are
  N3DPrimitive::CAnimatedBlock::ConvertToVBFormatBump / ConvertToVBFormatNonBump (0x00A8FF50 already landed under its PDB name).

## 3. Callees
- None (no call/jmp out; no vtable call). Inline-only helpers (FableWin.pdb, all expanded): CGenericVertex<CTVertexNormBlend4Tex1BumpBase>::
  SetPosition/SetNormal(const C3DVector&), SetU1/SetV1(float), SetBlendingIndex(ulong,ulong), SetBlendingWeight(ulong,float);
  C3DVertex2::GetPosition/GetNormal/GetU/GetV; CDeviceIndependent::PeekVertex; CArray operator[]/size(); C3DVector::Cross, Dot.
  (SetBlendingWeight takes float -- retail copies the byte directly, so do NOT use it; write BlendingWeights[j] like the sibling.)
- Old draft spellings vs PDB: class written without namespace; arg2 typed as a made-up `CPrimitiveVertexSource*` (PDB: N3DPrimitive::CDeviceIndependent&);
  out typed void* + local SBumpVertex (PDB: CTVertexNormBlend4Tex1Bump*); member PData (PDB: Data); Pos/Indices/Weights/Tangent/BinormalSign
  (PDB: X,Y,Z / BlendingIndices / BlendingWeights / UTangent[3] / VTangentSign; NormalX..Z, U1, V1).

## 4. Members (retail offsets; struct_layouts_egor.tsv names)
- this (N3DPrimitive::CAnimatedBlock, 0x30): +0x14 unsigned short BonesPerVertex; +0x2C CIVCountedPointer<CAnimatedBlock::CDeviceIndependent> Data.
  Shared header: rebuild/include/engine/C3DPrimitive2.h under FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS -- but there Data is only
  `unsigned char Data[4]`, no nested CDeviceIndependent, no ConvertToVBFormat* declarations. Landed sibling 0x00A8FF50 models the class LOCALLY instead.
- CAnimatedBlock::CDeviceIndependent (Ego_r 40B): +0 vfptr, +4 IVRefCount (CIVCountedPointeeBase), +8 CArray<unsigned long> VertexList,
  +0x14 CArray<C3DIndexedBoneWeight> BoneWeights (retail 12-byte CArray; Ego_r says +24 with 16-byte CArrays). No shared header.
- C3DIndexedBoneWeight {ulong BoneIndex +0; ulong Weight +4}: rebuild/include/engine/C3DIndexedBoneWeight.h.
- N3DPrimitive::CDeviceIndependent (mesh): +4 CArray<C3DVertex2> Vertices, +0x38 CArray<C3DVector> UTangents (+0x44 VTangents unused).
  Retail offsets confirmed by landed ctor rebuild/src/compiled/00/a9/N3DPrimitiveCDeviceIndependent_CDeviceIndependent_00a98100.cpp:38
  and StaticBump retail 0x00A8F23E `mov esi,[ebx+38h]`. No shared header (sibling models it locally : CBaseClassNonCopyable).
- C3DVertex2 (0x20): rebuild/include/engine/C3DVertex2.h (Position, Normal, U, V + by-value const getters) -- sibling redefines it locally;
  use the header if it does not clash.
- CTVertexNormBlend4Tex1BumpBase (Ego_r 56B): X,Y,Z +0; uchar BlendingIndices[4] +0xC; uchar BlendingWeights[4] +0x10; NormalX/Y/Z +0x14;
  U1,V1 +0x20; float UTangent[3] +0x28; float VTangentSign +0x34. CTVertexNormBlend4Tex1Bump : CGenericVertex<..BumpBase>. No shared header.
- C3DVector: rebuild/include/engine/C3DVector.h. Opt-ins relevant: FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX (Dot = Z, then +Y, then +X:
  matches retail exactly); default Cross body matches. CArray.h: default operator[] uses _M_start[i] (FABLE_RETAIL_CARRAY switches to begin()+i).
- Classes the old draft models locally that have a shared header: C3DVector, CAnimatedBlock (C3DPrimitive2.h, partial), C3DIndexedBoneWeight,
  C3DVertex2 (draft's SSourceVertex), CArray (draft uses raw pointers).

## 5. Visibility
- Leaf; nothing out of line. No STLport out-of-line helpers. All the vertex setters/getters/Cross/Dot must be inline (retail expanded them).
- Same TU (C3DPrimitive2/N3DPrimitive animated block .cpp): 0x00A8EFA0 StaticNonBump, 0x00A8F1B0 StaticBump (parked 1469v1469),
  0x00A8F770 RepeatingMesh, 0x00A8FF50 NonBump, then this row, 0x00A90450 CDeviceIndependent::Sizeof, 0x00A91180 ConvertToVBFormatAnimated.

## 6. Siblings to copy
- rebuild/src/compiled/00/a8/N3DPrimitiveCAnimatedBlock_ConvertToVBFormatNonBump_00a8ff50.cpp -- THE template: same signature shape,
  `// Flags: none`, no pragmas, local models (C3DVertex2 with by-value getters, CGenericVertex<T> SetPosition/SetNormal/SetU1/SetV1,
  CIVCountedPointer operator->, CAnimatedBlock::CDeviceIndependent, N3DPrimitive::CDeviceIndependent with PeekVertex),
  shared engine/CBaseClassNonCopyable.h, engine/C3DVector.h, engine/CArray.h. Loop:
  `count=Data->VertexList.size(); for(i..){ const C3DVertex2&src=mesh.PeekVertex(Data->VertexList[i]); T&vertex=out[i]; SetPosition(src.GetPosition())...;
  for(j=0;j<BonesPerVertex;++j){w=Data->BoneWeights[i*BonesPerVertex+j]; BlendingIndices[j]=(uchar)(w.BoneIndex*3); BlendingWeights[j]=(uchar)w.Weight;} for(;j<4;++j) zeros }`.
- rebuild/candidates/manual/pending/2026-11-02_codex_pass11/retry_differ/00a8f1b0.cpp (StaticBump, parked 4-byte scheduler tie): tangent +
  `C3DVector crossprod=src_vertex.GetNormal().Cross(u_tangent); if(crossprod.Dot(v_tangent)>=0.0f) VTangentSign=1.0f; else -1.0f;`
  (here both are the U tangent).
- rebuild/src/compiled/00/a8/C3DPrimitive2_ConvertToVBFormatStaticNonBump_00a8efa0.cpp, ..._RepeatingMesh_00a8f770.cpp (`// Flags:` empty, no pragmas).

## 7. Old draft model errors ($W/draft_original.cpp, DIFFER 384v403)
- Struct-assigns `out->Pos = in.Pos` / `out->Normal = in.Normal` -> registers only; retail goes through the by-value const GetPosition()/
  GetNormal() copy temporaries (X,Y on x87, Z via eax with a dead frame store) into SetPosition/SetNormal -> use the NonBump idiom.
- Needs index kept as a local reused for UTangents[index] (it does this right) but vertex access should be mesh.PeekVertex(index).
- Tangent: retail cross uses a FRESH src.GetNormal() (reloads +0x0C..+0x14) and Cross/Dot inlines: write
  `const C3DVector& t = mesh.UTangents[index]` (or copy), set UTangent[0..2], then `src.GetNormal().Cross(t).Dot(t) >= 0.0f ? ...`
  with FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX. Draft's `< 0 -> -1 else 1` ordering inverts the fallthrough block.
- Wrong types/names throughout (void* out, pointer arg, local PData/SBumpVertex, missing namespace); out++ cursor vs sibling's out[i].
- Draft's bone loop and zero pad match retail form (same as landed sibling).

## 0x0064546F

# DOSSIER 0x0064546F  NPlayerGui::CDrawQuickAccessIcons::DrawEffectAtPos (406 B)

## 1. Retail shape (xdis 0064546f 0x1a6)
- __thiscall, `ret 0Ch` (3 stack args), this in esi, pos ptr in ebx ([ebp+8]), index*8 in edi ([ebp+10h], `shl edi,3` BEFORE the epsilon compare).
- `push ebp; mov ebp,esp; sub esp,40h` ... `leave` => frame pointer: `#pragma optimize("y",off)` + `#pragma optimize("s",on)` (HUD TU mode, same as every landed CDrawQuickAccessIcons/CDrawAbilityButtons row). No SEH prologue (dtors of temporaries run without EH state) -> no /EHsc needed; Flags: none.
- Frame: CRightHandedSet temp [ebp-40h] (0x18), C3DVector(0,0,1) [ebp-28h], C3DVector(0,1,0) [ebp-1Ch], intelligent-ptr temp [ebp-10h] (create path + zero-pos path), [ebp-8] shared by the moved-kill temp ptr AND the create-path C2DVector position local.
- Epsilon test = inline C2DCoordI::ToC2DCoordF().IsNonZero(): `fild X; fild Y; fld [0x129BA3C]` (DELTA_NOTIONAL_ZERO) then Y*Y first, X*X, faddp, eps*eps from the register copy (`fld st1; fmul st2`), fcompp, `test ah,5; jp` -> zero branch 0x6455B2. The float vector (Y,X) and eps are popped after (3x fstp st0).
- Both branches begin with Effects[index].GetPItem(): `lea ecx,[edi+eax]` hoisted before the jp (tail-merged).
- Non-zero: if GetPItem && pos != EffectPositions[index] (cmp X jne kill; cmp Y je skip) -> GetPItem()->Kill(); Effects[index] = CIntelligentPointer<CParticleEmitter>(0) (ctor 0x655318 into [ebp-8], op= 0x65533C, dtor 0xA01C10). Then if !Effects[index].GetPItem(): PeekGuiDef(); pos.ToC2DCoordF() + QuickAccessEffectOffset (Y+offY computed first, then X+offX) stored to named local [ebp-8]; THEN the CRightHandedSet temp (args right-to-left: (0,1,0) stored first, then (0,0,1)); GetParticleEmitterManager(); Create2DEmitter(effectType,&p,0x43,true,set); ctor temp [ebp-10h], op=, dtor; EffectPositions[index] = pos (X then Y copy).
- Zero branch: if GetPItem: Kill; = CIntelligentPointer(0) via [ebp-10h]; EffectPositions[index].X = 0, .Y = 0 (`xor eax,eax; xor ecx,ecx` then two stores -> likely `EffectPositions[index] = C2DCoordI(0,0)`).
- Position local computed BEFORE the set/manager => named local (not an inline argument expression; contrast sibling DrawEffects where the arg expression is inline because position is a param).

## 2. Identity (manifest label CORRECT in leaf/class; signature in draft WRONG)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @Ego_r 0x0068804A): `?DrawEffectAtPos@CDrawQuickAccessIcons@NPlayerGui@@AAEXABVC2DCoordI@@JJ@Z`
  => private, void, `(const C2DCoordI& pos, long effect, long index)`, non-const. Param names not in PDB (descriptive).
- Second source (address order): Ego_r DrawIcon 0x687E66 -> DrawEffectAtPos 0x68804A -> ClearEffects 0x68822D; retail landed DrawIcon 0x00645292 -> 0x0064546F -> 0x00645605 (`add ecx,7Ch; jmp CDrawBase::ClearEffects 0x6445BA` = ClearEffects()). ret 0Ch = 3 args matches JJ + ref.
- Callers (xref): 5 calls 0x6536AF..0x653F52, all inside CDrawQuickAccessIcons::Draw 0x00652FA4 (functions.tsv), e.g. 0x6536A3 `push [edi+6E8h]` (CPlayerGuiDef QuickAccessGuildSealEffect) as effect type, pos = local [ebp-18h].
- Not in _fablewin_api.tsv / FableWin signatures / rtti-vtables (non-virtual).
- NAME_FIXES candidate: landed rebuild/src/compiled/00/64/unattributed_GetTarget00645605_00645605.cpp is really `?ClearEffects@CDrawQuickAccessIcons@NPlayerGui@@QAEXXZ` (Ego_r 0x0068822D + neighbour order + body = add ecx,Effects; jmp CDrawBase::ClearEffects).

## 3. Callees (landed file / landed name / PDB)
| addr | landed row | PDB name | draft spelling |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | draft returns void* -- WRONG, returns CBaseObject* |
| 0x00AEE8C0 | 00/ae/NParticleEngineCParticleEmitter_Kill_00aee8c0.cpp | ?Kill@CParticleEmitter@NParticleEngine@@QAEXXZ | draft has no NParticleEngine namespace -- WRONG |
| 0x00655318 | 00/65/CIntelligentPointerNParticleEngineCParticleEmitter_CIntelligentPointer_00655318.cpp | ??0?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@QAE@PAVCParticleEmitter@NParticleEngine@@@Z | namespace missing -- WRONG mangling |
| 0x0065533C | 00/65/CIntelligentPointerCThingCreatureBase_operator_0065533c.cpp (24 identical bodies) | ??4?$CIntelligentPointer@...@@QAEAAV0@ABV0@@Z (copy assign, out of line) | ok in shape |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ??1CBaseIntelligentPointer@@UAE@XZ (VIRTUAL dtor) | draft non-virtual dtor -- WRONG (class has vptr at +0, PData +4) |
| 0x00438570 | 00/43/CPlayerGui_PeekGuiDef_00438570.cpp | ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ (static __fastcall, const ref) | draft free fn returning ptr -- WRONG |
| 0x00487050 | 00/48/CRightHandedSet_CRightHandedSet_00487050.cpp | ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z, params (up, forward); members Up +0, Forward +0xC | draft names (forward, up) and members Forward,Up -- WRONG names (call is CRightHandedSet(C3DVector(0,0,1) up, C3DVector(0,1,0) forward)) |
| 0x00AEAAF0 | 00/ae/NParticleEngineCParticleEmitterManager_Get_00aeaaf0.cpp (generic getter stub) | ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ (free __fastcall, returns REFERENCE) | draft `static CParticleEmitterManager::Get()` ptr -- WRONG |
| 0x00AEB790 | 00/ae/NParticleEngineCParticleEmitterManager_Create2DEmitter_00aeb790.cpp | ?Create2DEmitter@CParticleEmitterManager@NParticleEngine@@QAEPAVCParticleEmitter@2@JABVC2DVector@@W4EEngine2DLayer@@_NABVCRightHandedSet@@@Z | draft layer is `long`, no namespace -- WRONG (EEngine2DLayer enum; 0x43 = ENGINE_2D_LAYER_HUD_EFFECTS as in sibling) |
- Global 0x0129BA3C = DELTA_NOTIONAL_ZERO (`extern const float DELTA_NOTIONAL_ZERO;` per CThingFilter_IsInFOV.h; C2DVector.h IsNonZero comment cites 129BA3Ch). Draft `g_fEpsilon` -- rename.

## 4. Members
- this +0x7C `Effects`: std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > (Ego_r +0x88; retail -0xC since STLport vector = 12 B vs PDB 16 B). this +0x88 `EffectPositions`: std::vector<C2DCoordI> (Ego_r +0x98). Names from struct_layouts_egor.tsv `CDrawQuickAccessIcons`. NO shared header for NPlayerGui::CDrawQuickAccessIcons (only NPlayerGui_CDrawBase.h) -> local class with pad to 0x7C, like landed siblings.
- Element size 8 for both (CIntelligentPointer = vptr+PData; C2DCoordI 8) -> `shl edi,3`.
- CPlayerGuiDef +0x6F0 QuickAccessEffectOffset (C2DVector) -- shared rebuild/include/engine/CPlayerGuiDef.h line 492 spells it as split floats `QuickAccessEffectOffset_X/_Y` (no opt-in macro covers it; header offers CRIME/MINIGAME_OFFSET/PROGRESS/QUESTINFO/TARGETING/SCRIPTSPRITES/ABILITYBUTTONS only). Either local CPlayerGuiDef pad model (as DrawIcon does) or C2DVector(def.QuickAccessEffectOffset_X, _Y).
- C2DCoordI pos.X/+0, .Y/+4 -> shared engine/C2DCoordI.h with FABLE_RETAIL_C2DCOORDI_TOFLOAT (gives inline ToC2DCoordF, needs FABLE_RETAIL_C2DVECTOR). Header has NO operator!= (FableWin ??9C2DCoordI@@QBE_NABV0@@Z, ??8 exist, inline) -> needs local/new spelling: `X!=o.X || Y!=o.Y`.
- Locally modelled in draft though a shared header exists: C2DVector, C3DVector, C2DCoordI (as CVector2<long>), CRightHandedSet, CParticleEmitter, CParticleEmitterManager, CBaseIntelligentPointer/CIntelligentPointer, CPlayerGuiDef.
- Opt-ins available: CIntelligentPointer.h: FABLE_RETAIL_CINTELLIGENTPOINTER (pack(1) class, Get/operator->), _CTOR_OUTOFLINE (T* ctor decl -> calls 0x655318), _ASSIGN_OUTOFLINE (op= decl -> 0x65533C). CRightHandedSet.h: FABLE_RETAIL_CRIGHTHANDEDSET_CTOR (out-of-line 0x487050 decl). CParticleEmitterManager.h: FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D (decl) -- header lacks GetParticleEmitterManager decl (sibling declares it locally in namespace NParticleEngine). CParticleEmitter.h: FABLE_RETAIL_CPARTICLEEMITTER_KILL. CPlayerGui.h: FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF. C2DVector.h: FABLE_RETAIL_C2DVECTOR, _SQUAREDMAGNITUDE_YX (Y*Y first, then += X*X -- matches retail order here), _ISNONZERO_DIRECT / _ISNONZERO_FORCEINLINE.

## 5. Visibility
- Out of line (called): GetPItem, Kill, CIntelligentPointer(T*) 0x655318 and op= 0x65533C (COMDATs right after this TU's neighbourhood, 0x6553xx), ~CBaseIntelligentPointer, CRightHandedSet ctor 0x487050, PeekGuiDef, GetParticleEmitterManager, Create2DEmitter.
- Inline (expanded): C2DCoordI::ToC2DCoordF, C2DVector::IsNonZero / GetSquaredMagnitude (YX order), C2DVector operator+, C2DCoordI operator!= / copy assign, STLport vector::operator[] (`*(begin()+n)` from stlport/fable_containers.h line 464 -- reloads [esi+7Ch] each use).
- Same TU, defined earlier: CDrawQuickAccessIcons::DrawIcon 0x645292, CDrawBase::ClearEffects 0x6445BA (static __fastcall), CDrawAbilityButtons::DrawEffects 0x6451DD (identical emitter idiom).

## 6. Siblings (copy idioms)
- rebuild/src/compiled/00/64/NPlayerGuiCDrawAbilityButtons_DrawEffects_006451dd.cpp -- BEST TEMPLATE: `y` off + `s` on, Flags: none, `#include "stlport/fable_containers.h"`, real std::vector Effects (its notes: the raw-pointer model SWAPPED this/index registers -- exactly this row's residue), GetParticleEmitterManager() __fastcall ref, EEngine2DLayer 0x43, CRightHandedSet(C3DVector(0,0,1),C3DVector(0,1,0)), named CIntelligentPointer local then `Effects[index] = emitter`.
- 00/64/NPlayerGuiCDrawQuickAccessIcons_DrawIcon_00645292.cpp -- same class, `y` off + `s` on, CPlayerGui::PeekGuiDef static __fastcall const ref, local CPlayerGuiDef pad model.
- 00/64/NPlayerGuiCDrawBase_ClearEffects_006445ba.cpp -- `s` on, STLport operator[] as `*(begin()+n)` note.
- 00/64/NPlayerGuiCDrawQuickAccessIcons_ToggleCharge_0064d92a.cpp -- `y` off + `s` on, Flags: none, FABLE_RETAIL_CINTELLIGENTPOINTER + engine/CIntelligentPointer.h.
- 00/64/NPlayerGuiCDrawQuickAccessIcons_Update_0064d6a8.cpp -- `y` off + `s` on, Flags: /GS.

## 7. Old draft model errors
1. Effects / EffectPositions as RAW POINTERS at +0x7C/+0x88 instead of std::vector members (STLport): sibling DrawEffects proved raw pointers swap the this/index registers -- matches the recorded residue (index*8 in ebx not edi, pos in edi not ebx). Use std::vector + `*(begin()+n)` operator[].
2. pos typed CVector2<long> template -> C2DCoordI (PDB). Epsilon test should be `pos.ToC2DCoordF().IsNonZero()` (float vector temp kept on the x87 stack, Y squared first) rather than two named floats + x*x+y*y (X first).
3. Create-path position must be a named C2DVector local computed before the emitter call (retail calls PeekGuiDef and stores it before building the set); `pos.ToC2DCoordF() + PeekGuiDef().QuickAccessEffectOffset`.
4. Wrong callee signatures: GetParticleEmitterManager (free fastcall ref, not static Get ptr), PeekGuiDef (CPlayerGui static fastcall const ref), Create2DEmitter layer enum, virtual ~CBaseIntelligentPointer, NParticleEngine namespace, CRightHandedSet param/member names (Up, Forward).
5. Kill-path temporary slots: moved-kill temp shares [ebp-8] with the create position local, zero-branch temp shares [ebp-10h] with the create emitter temp -> scope the temporaries per block (separate `{}` / full-expression temps).
6. Zero branch clears with two zero regs (`xor eax,eax; xor ecx,ecx`) -> `EffectPositions[index] = C2DCoordI(0,0);` likely (draft's .X=0;.Y=0 may emit one reg).

## 0x008565E0

# DOSSIER 0x008565E0 -- `anonymous namespace'::CThingFilter_CombatIsStrikeToStrike::operator()(const CThing&) const  (406 bytes)

## 1. Retail shape (xdis 008565e0 0x1a6)
- __thiscall, `ret 4`, bool in al. No frame pointer (`sub esp,1Ch; push ebx; push ebp; push edi`), no SEH, /O2 (long jcc, not size-opt).
- this in ebp, spilled to [esp+0xC] (later [esp+0x10] after `push esi`); thing in edi; esi pushed only on the creature path.
- Order: `test [thing+6Ch],8` -> creature branch first; non-creature arm at 0x856736.
- Creature: IsToEvadeAllHits -> false; IsRolling -> false; esi=thing.PBaseDef.Object, ebx=0;
  if esi: count +1,+1,-1 (three stores to [esi+4] from register arithmetic), ebx=esi, if 0 -> Delete (slot 1).
  => temporary `thing.GetPBaseDef()` (by value, +1) assigned into a NULL local `CDefPointer<const CThingCreatureDef>` (+1,
  Object!=new with Object known 0 -> no release), then the temporary dies (-1). Shared CDefPointer `SetDynamicCast`/`operator=<U>` = Assign.
  Then `test [esi+138h],1` (DeferHitsIfBehindHitter, read via esi = the temp's pointee, NOT ebx) -> facing test:
  GetPos(thing) (ebp), GetPos(*ThingStriking); delta = thingPos - strikerPos (X,Y,Z stored to [esp+14/18/1C]);
  ThingStriking->PhysicsTC->vslot70(+0x118) GetFacingVector() (hidden result at [esp+20]);
  dot = facing.Z*delta.Z + facing.Y*delta.Y + delta.X*facing.X (Z,Y,X order => C3DVector::Dot ZYX opt-in
  FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX, try delta.Dot(facing) and facing.Dot(delta));
  `fcomp [0x0122DEDC]` (0.0f literal, cited in landed 0x00B18AF0), `test ah,5; jp` => if (dot < 0.0f) { release def (dec [esi+4] RMW); return false; }
  After: re-test `[thing+6Ch]&8`: clear -> release def (ebx, null-tested, `dec [ebx+4]`) return true;
  set -> al = NTargeting::IsThingVulnerableToMeleeStrikesFromThing(thing, *ThingStriking) (edx=[ebp]), saved to [esp+0x30]
  (thing's arg slot), def released, return it. So the def local lives across the call.
- Non-creature arm: `ThingStriking->Type (byte +0x10) == 2` -> return true; else `mov ecx,[thing+70h]; test; je; inc [ecx+4]`
  (AddRef, NO null test after), read `[ecx+42h]>>7`, release load/dec/store `mov eax,[ecx+4]; dec; mov [ecx+4],eax; jne Delete`
  (no null test) => `return thing.GetPBaseDef()->AlwaysIncludeInObjectStrikeScans;` (temporary).

## 2. Name / signature (two sources)
- FableWin.pdb (_fablewin_api.tsv 0x017D873C / 0x028214F0, FableWin.exe.signatures.tsv) AND Ego_r.pdb (egor_pdb_names.tsv
  0x008364A0): `??RCThingFilter_CombatIsStrikeToStrike@?A0xad05d24c@@QBE_NABVCThing@@@Z`
  = `bool (anonymous namespace)::CThingFilter_CombatIsStrikeToStrike::operator()(const CThing&) const`.
  Ctor `??0CThingFilter_CombatIsStrikeToStrike@?A0xad05d24c@@QAE@ABVCThing@@@Z` (takes const CThing&).
- Manifest label class/name right, but the class is in an ANONYMOUS NAMESPACE (hash per TU) and the param is `const CThing&`
  (draft: `CThing*`, no namespace). Leaf should be the prefix `??RCThingFilter_CombatIsStrikeToStrike@?A0x` (as landed 0x0085B160).
- Member (struct_layouts_egor.tsv): `CThing* ThingStriking` +0 (size 4). Draft's `Striker` is wrong.
- Callers (xref): 0x0085B181/0x0085B41E/0x0085B636 = landed CFilter_Bind4<...>::operator()<CThingGameObject> 0x0085B160
  (rebuild/src/compiled/00/85/CFilterBind4_operator_0085b160.cpp) and GetAllThings 0x0085B5B0 (+ rough-area); both declare it
  `namespace { class CThingFilter_CombatIsStrikeToStrike { CThing* ThingStriking; bool operator()(const CThing&) const; }; }`.
- TU: sits among CCombatActionBase rows (0x008561xx..0x008574xx) -> combat action base source file; sibling filter
  CThingFilter_CombatCanCollideOrInteractWithThing 0x00857250 is in the same anonymous namespace.

## 3. Callees
| addr | landed row | landed / PDB name | draft spelling |
|---|---|---|---|
| 0x0085FA00 | 00/85/NCreatureMode_IsToEvadeAllHits_0085fa00.cpp | `?IsToEvadeAllHits@NCreatureMode@@YI_NABVCThing@@@Z` | `IsToEvadeAllHits(CThing*)` global -- WRONG (namespace NCreatureMode, const CThing&) |
| 0x0085EEB0 | 00/85/NCreatureMode_IsRolling_0085eeb0.cpp | `?IsRolling@NCreatureMode@@YI_NABVCThing@@@Z` | global `IsRolling(CThing*)` -- WRONG |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | `?GetPos@CThing@@QBEABVC3DVector@@XZ` (CThing.h declares `const C3DVector& GetPos() const`) | `GetPosition` -- WRONG |
| 0x007F59F0 | 00/7f/NTargeting_IsThingVulnerableToMeleeStrikesFromThing_007f59f0.cpp | `?IsThingVulnerableToMeleeStrikesFromThing@NTargeting@@YI_NABVCThing@@0@Z` | global, pointers -- WRONG |
| vslot 70 +0x118 | CTCPhysicsBase.h line 124 | `virtual C3DVector GetFacingVector() const` | `GetFacing` -- WRONG |
| def vslot 1 +4 | CDefPointer.h Release -> `object->Delete()` | Delete | `Destroy` -- WRONG |
| inline | CThing.h:259 | `?GetPBaseDef@CThing@@QBE?AV?$CDefPointer@$$CBVCThingBaseDef@@@@XZ` (FableWin) returns by value | `GetPDef` -- WRONG |
| inline | CClass.h:27 | `CClass::GetType() const` (FableWin inline QBEKXZ), member `Type` +0x10 | `ThingType` -- WRONG |

## 4. Members (retail offsets; shared headers)
- CThing (engine/CThing.h, retail layout, CheckRetailLayout asserts): PhysicsTC +0x60 (`CTCPhysicsBase*`), ThingTypeFlags +0x6C
  (`unsigned long`, accessor GetTypeFlags() under FABLE_RETAIL_CTHING), PBaseDef +0x70 (`CDefPointer<const CThingBaseDef>`),
  GetPhysicsTC() const, GetPos(). Members are protected -> use accessors (GetPBaseDef, GetTypeFlags, GetPhysicsTC) or the
  usual test-access idiom. Needs FABLE_RETAIL_CCLASS, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CTHING (see IsToEvadeAllHits row defines).
- CClass::Type +0x10 (unsigned char; engine/CClass.h, FABLE_RETAIL_CCLASS) on ThingStriking.
- CThingBaseDef (engine/CThingBaseDef.h, FABLE_RETAIL_CTHINGBASEDEF): bitfield AlwaysIncludeInObjectStrikeScans = bit 7 of +0x42.
- CThingCreatureDef (engine/CThingCreatureDef.h, FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS): `bool DeferHitsIfBehindHitter:1` +0x138
  (PDB +320). Used by landed 0x008302F0/0x00830300/0x00830310 (CThingAICreature getters).
- CDefPointeeBase IVRefCount +4 (engine/CDefPointeeBase.h via CDefPointer.h).
- CTCPhysicsBase (FABLE_RETAIL_CTCPHYSICSBASE) slot 70 GetFacingVector() const.
- C3DVector (engine/C3DVector.h): operator- (const member), Dot opt-ins DOT_ORDERED_ZYX / _YXZ / _XYZ / DOT_GROUPED.
- Draft LOCAL models that have shared headers: C3DVector, CThingDef(->CThingBaseDef/CThingCreatureDef), CDefPointer, CTCPhysicsBase, CThing.

## 5. Visibility
- All four direct callees are out of line in retail (call rel32) -- declare only. NCreatureMode/NTargeting are other TUs.
- CDefPointer members: always __forceinline (CDefPointer.h header note); GetPBaseDef/GetType/GetPos? -- GetPos 0x004C73D0 IS
  called out of line (7-byte body, COMDAT): declare only, do not give it an inline body.
- Release shapes: in-place default `--object->IVRefCount` gives `dec [ecx+4]` (creature def local, matches); the non-creature
  temporary's load/dec/store may need FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE or _COUNTREF variants (header lines ~70-100) --
  or comes naturally from the temporary (header note: "temporary releases as mov edi,[eax+4]; dec edi; mov [eax],edi").
- The Assign(+1,+1,-1 in registers) chain: shared CDefPointer `SetDynamicCast(thing.GetPBaseDef())` on a NULL_DEF local
  (or `CDefPointer<const CThingCreatureDef> def(thing.GetPBaseDef())` via the template converting ctor; try both; NOTES "next:
  a by-value CDefPointer temporary assigned into a zeroed local").

## 6. Siblings
- 0x00857250 CThingFilterCombatCanCollideOrInteractWithThing_operator_00857250.cpp (same TU, local models, no pragmas, uses CThing*).
- 0x0085B160 CFilterBind4_operator_0085b160.cpp (anon-namespace declaration idiom, Leaf prefix with @?A0x).
- 0x0085B5B0 CThingSearchTools_GetAllThings_0085b5b0.cpp.
- 0x0085FA00 NCreatureMode_IsToEvadeAllHits (define set: FABLE_RETAIL_CCLASS, DEF_CLASS_BASE, BASE_OBJECT, CTCBASE,
  CINTELLIGENTPOINTER, STLP_LIST, CTHING, ...).
- CCombatActionBase rows 0x00857070 / 0x00857520 (FABLE_RETAIL_CINTELLIGENTPOINTER, CTHING_GETTC_EXPANDED). No `// Flags:`/pragmas in this TU's rows.

## 7. Draft model errors
- Local CDefPointer::operator= AddRefs `other` before releasing self (not the shared compare/Release/AddRef Assign) -> wrong count-store chain.
- Uses `CDefPointer<CThingDef>` for both arms; retail creature arm is a CThingCreatureDef handle (DeferHitsIfBehindHitter +0x138)
  built from the CThingBaseDef temporary; note retail reads +0x138 through esi (the temp's pointer), not the local.
- Non-creature arm: draft `CDefPointer def(thing->PDef)` from raw pointer; retail = GetPBaseDef() temporary copy, field read, release.
- Wrong names: Striker, ThingType, GetPDef, GetPosition, GetFacing, Destroy, global IsToEvadeAllHits/IsRolling/IsThingVulnerable...;
  missing anonymous namespace and `const CThing&` param.
- Control flow (creature first, re-test of flag bit 3 before the vulnerability call, dot<0 -> false) is right.

## 0x00AC8AE0

# DOSSIER 0x00AC8AE0 (409 B) -- QuadricOptimiserInternals::COptimisedPrimitive::JoinAdjacentFaces

## 1. Retail shape (xdis 00ac8ae0 0x1a9)
- __thiscall, `ret 8` (3 exits: 0xAC8B5C false, 0xAC8C6A true, 0xAC8C76 false). Returns bool in al (`xor al,al` / `mov al,1`).
- NO frame pointer (`sub esp,1Ch` first, ebp used as a general register, pushed LATE at 0xAC8B66 only on the
  "some a-vertex not in b" path). No SEH. Normal /O2 speed idioms (cdq/idiv by 3 for `% 3` => signed index).
- Frame 0x1C = exactly: this spill [esp+8 pre-push], &faceB-record spill, b.UniqueVertexIndices[1] spill, 16-byte COptimisedEdge temp.
- Register map: ebx = a.UniqueVertexIndices[0] (live whole body until reused for a.U[edgeA]), esi = &a,
  edx/edi/eax = b.U[0]/b.U[1]/b.U[2] (CSE'd across the three inlined HasUniqueVertex), ebp = edgeA, edi = edgeB.
- Faces._M_start read once from [this+1Ch]; face stride 72 (`lea x,[i+i*8]` then `*8`).
- Push_back call: `lea eax,[esp+1Ch]; push eax; add ecx,28h; call 0x00AD5CE0` (Edges at +0x28).
- After the call faceB/faceA are re-read from the ARG slots (not callee-saved regs) and stored into a.Edges[edgeA] / b.Edges[edgeB].

## 2. Identity (manifest label CORRECT, namespace missing)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv) 0x00C61CA0 `?JoinAdjacentFaces@COptimisedPrimitive@QuadricOptimiserInternals@@AAE_NKK@Z`
- FableWin.pdb (ghidra_out/_fablewin_api.tsv) 0x017CBBFE / 0x034C6710, same decoration (also naming_stragglers/FableWin.exe.signatures.tsv).
- Signature: private, non-const: `bool JoinAdjacentFaces(unsigned long, unsigned long);`
- Homonym ruled out: `?JoinAdjacentFaces@CPrimitive@CTriangleStripifier@@QAE_NKK@Z` -- CTriangleStripifier::CPrimitive is
  Triangles +0 / Vertices +0x10 (struct_layouts_egor), whereas retail reads Faces at +0x1C with a 72-byte element = COptimisedFace.
- Caller: only 0x00AC9B7F inside COptimisedPrimitive::FindAdjacentFaces 0x00AC9A60 (unlanded; Ego_r ?FindAdjacentFaces@...@AAEXXZ). Not virtual.

## 3. Callees
- 0x00AD5CE0 (only call). NOT landed. manifest functions.tsv calls it `std::vector<COptimisedEdge>::push_back`; PDB name is
  `?push_back@?$CArray@VCOptimisedEdge@QuadricOptimiserInternals@@@@QAEXABVCOptimisedEdge@QuadricOptimiserInternals@@@Z`
  (Ego_r 0x00C6AB60, FableWin 0x034D16C0) => it is Lionhead's CArray<COptimisedEdge>::push_back with STLport vector::push_back
  inlined in it (body: _M_finish!=_M_end_of_storage -> 4-dword copy, +=0x10; else _M_insert_overflow 0x00AD4D10, landed as
  rebuild/src/compiled/00/ad/stdvector_Minsertoverflow_00ad4d10.cpp). Draft spelled it as an undeclared-body CArray::push_back
  on a local CArray -- name OK (CArray::push_back), but body invisible.
- HasUniqueVertex: Ego_r 0x00C5F860 / FableWin `?HasUniqueVertex@COptimisedFace@QuadricOptimiserInternals@@QBE_NK@Z` (public const). NOT out of line
  in retail (no manifest entry; inlined three+three times). Draft spelling matches PDB.
- Also in FableWin for COptimisedFace (candidate inline accessors): GetUniqueVertexIndex(unsigned long) QBEKK, GetUniqueA/B/C QBEKXZ,
  FindVertex QBEJK (returns long). COptimisedEdge: GetFaceA/B, GetUniqueVertex1/2 only -- no user ctor in FableWin (debug PDB) => aggregate fill.

## 4. Members
- COptimisedPrimitive (rebuild/include/engine/COptimisedPrimitive.h): Faces +0x1C (CArray<COptimisedFace>), Edges +0x28 (CArray<COptimisedEdge>).
  Opt-in FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE gives the real class (private members, CArray, namespace) -- but has NO
  JoinAdjacentFaces declaration (the _METHODS block lists GetFaceNormal/FindTextureTangent/FindFaceTextureTangents/
  PerformContraction/DetectFoldOver/PopFrontContraction/UpdateCost/Optimise; _SUBCOST adds ComputeSubContractionCost). The author needs a
  declaration: either a new header opt-in (e.g. under _METHODS) or the landed-row trick `#define COptimisedPrimitive COptimisedPrimitiveFields`
  + derived class (UpdateCost 0x00AC8A60, ComputeVertexCost 0x00AC8C80 do this with the flat default header).
- COptimisedFace (rebuild/include/engine/QuadricOptimiserTypes.h, opt-in FABLE_RETAIL_QUADRIC_COPTIMISEDFACE): UniqueVertexIndices[3] +0x0C
  (unsigned long), Edges[3] +0x18 (long); PDB-exact (struct_layouts_egor COptimisedFace 72). Header has NO HasUniqueVertex / GetUniqueVertexIndex -- would need adding.
- COptimisedEdge: struct_layouts_egor FaceA +0, FaceB +4, UniqueVertex1 +8, UniqueVertex2 +0xC (ulong). No shared header defines it (only forward-declared in COptimisedPrimitive.h).
- CArray: rebuild/include/engine/CArray.h (public STLport vector<T>, 12 bytes). Opt-ins: FABLE_RETAIL_CARRAY (operator[] via begin()),
  FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE (visible body, __declspec(noinline) -- matches retail's out-of-line CArray::push_back),
  FABLE_RETAIL_CARRAY_PUSHBACK (inline body + push_back()).
- Draft models LOCALLY (all have shared homes): COptimisedPrimitive, CArray, COptimisedFace, C3DVector (engine/C3DVector.h). COptimisedEdge has none.

## 5. Callee visibility
- CArray<COptimisedEdge>::push_back is out of line at a 16-aligned COMDAT 0x00AD5CE0 but its body was surely visible (template in CArray.h):
  use FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE (precedent: CParticleProgram::RemapBoneIndices 0x00AE3920) -- c2 then knows &edge does not escape
  and the callee clobbers only eax/ecx/edx (esi saved). Needs FABLE_RETAIL_STLP_VECTOR so vector::push_back has its body (check fable_containers.h ~l.467).
- HasUniqueVertex: inline member (QBE_NK), expanded; ordering U[0]==v || U[1]==v || U[2]==v confirmed by the cmp order.
- Neighbours same TU (QuadricOptimiser.cpp): UpdateCost 0x00AC8A60 just before, ComputeVertexCost 0x00AC8C80 just after.

## 6. Sibling landed rows (rebuild/src/compiled/00/ac/)
- QuadricOptimiserInternalsCOptimisedPrimitive_UpdateCost_00ac8a60.cpp: `#define COptimisedPrimitive COptimisedPrimitiveFields` + flat header, derived class; Flags: none.
- QuadricOptimiserInternalsCOptimisedPrimitive_ComputeVertexCost_00ac8c80.cpp: FABLE_RETAIL_STLP_SET/_VECTOR/_VECTOR_RESERVE/_TREE_POSTINC,
  FABLE_RETAIL_C3DVECTOR(_NORMALISE_UNPAREN), CArray.h, same Fields trick, local COptimisedFace (PDB layout); noinline same-TU callee bodies.
- QuadricOptimiserInternalsCOptimisedPrimitive_ComputeSubContractionCost_00ac82a0.cpp: FABLE_RETAIL_CARRAY, FABLE_RETAIL_C3DVECTOR,
  FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE(+_SUBCOST), QuadricOptimiserTypes.h + COptimisedPrimitive.h (the real-class route).
- QuadricOptimiserInternalsCOptimisedPrimitive_MakeOrthogonal_00ac7200.cpp: Flags /Gr, FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE + C3DVECTOR opt-ins.
- COptimisedPrimitive_CollapseSegments_00ac85d0.cpp: `#pragma auto_inline(off)` around a same-TU noinline stand-in callee; Faces +0x1c.

## 7. Old draft vs retail
- Logic/control flow matches retail exactly (early all-three reject; edgeA from U[2],U[0],U[1] -> 0,1,2; edgeB the same on b vs a; -1 tests;
  U[edgeA] vs b.U[(edgeB+1)%3]; Edges==-1 tests; edge {faceA,faceB,U[edgeA],U[(edgeA+1)%3]}; a.Edges[edgeA]=faceB; b.Edges[edgeB]=faceA).
- Residue was register allocation only (388 v 409): draft holds &b in ebp (frame 0x14), retail spills &b and b.U[1] (frame 0x1C) and pushes ebp late.
- Likely model errors: (a) local CArray::push_back declared without body -> &edge escapes / callee unknown; use the shared CArray with
  PUSHBACK_NOINLINE + STLP vector body; (b) local class models instead of shared headers (GOTCHAS Q20: porting to shared headers alone fixed
  ebx/ebp swaps on two rows); (c) edge built field-by-field -- retail stores FaceB before FaceA and UniqueVertex1/2 after the idiv/push, try
  aggregate init `COptimisedEdge edge = {faceA, faceB, ...}` or GetUniqueVertexIndex accessors; (d) indices are signed (`long`, cdq/idiv).

## 0x008F10B0

# DOSSIER 0x008F10B0 -- CTCTavernGameCardBase::ShuffleCards (410 bytes)

## 1. Retail shape (xdis 008f10b0 0x1aa)
- thiscall (this -> ebx), no args, plain `ret`, no frame pointer (`sub esp,20h ; push ebx/ebp/edi`; esi pushed later, only
  around the loop), no SEH frame. /O2-style, no size-idioms. Frame is 0x20.
- Order: def slot [esp+0xc] zeroed (ebp=0) AFTER its address is pushed -> `call 0x8F0B20 GetCardDef(&def)` (out of line);
  then `random_shuffle(this->[0x134], this->[0x138])` (__fastcall ecx=first, edx=last, 0x7D7860);
  then def loaded ONCE into edi; `CardPack.size()` = (end-begin)/0x44 via 0x78787879 imul/sar 5/shr 31 add (signed ptr diff);
  `je` past the loop if 0. The def's frame slot is then REUSED as the height counter (`mov [esp+0xc],35h`, `fild`, `dec ecx` at bottom):
  i.e. after GetCardDef the compiler knows the def slot is dead and holds the def in edi (the landed GetCardDef/GetPackPosition
  "stand-in body" idiom makes the address not escape).
- Loop (do/while, `cmp ebp,eax ; jb top`, index i in ebp unsigned, size re-read from [edi+78h]/[edi+7Ch] each pass):
  z = (float)height * def->CardSeparation(+5c) + def->TableSeparation(+60); pos = {0,0,z} (3 stack floats);
  then an INLINE copy of GetPackPosition() with GetCardDef inlined inside it:
  temp CDefPointer=0 (esi=0 also = the outer local), `ecx=ParentThing(+4)->PDef(+70)`, `call 0x8F27A0 GetSubDef<CTavernGameCardBaseDef>(&temp)`;
  operator= into the (null) outer local: `if (temp) { ++count; esi=temp; --count(temp dtor); if 0 Destroy }` (inc/dec in registers);
  reads PackOffset.X(+64)*CardWidth(+2c), PackOffset.Y(+68)*CardHeight(+30) into temps; then the outer local's dtor WITHOUT null test
  (`dec eax ... jne`, exactly GetPackPosition's landed dtor shape).
  pos.X += pack.X, pos.Y += pack.Y, pos.Z copied to itself (pack.Z 0.0 folded) -> pos = pos + GetPackPosition() (or +=).
  card = Pack[i] (`mov edx,[ebx+134h] ; mov esi,[edx+ebp*4]`); HideMesh(card->Thing) 0x5E9B10 (fastcall ecx);
  PositionMesh(card->Thing, GamePositionPos(+108, edx), GamePositionRHSet(+f0), pos, 0.5f, 0.5f) 0x5EABC0;
  card->LocalPos(+27) = pos; card->NewLocalPos(+33) = pos (each via `lea ecx,[esi+27h]` then 3 dword copies from pos);
  card->FinalRotation(+10) = 0.5f; card->FaceDown(+25) = true; ++i; --height.
- Epilogue: def dtor on edi WITH null test (`test edi,edi ; je ; dec [edi+4] ; jne ; call [vt+4]`).

## 2. Identity
- Name: CTCTavernGameCardBase::ShuffleCards, leaf `?ShuffleCards@CTCTavernGameCardBase@@UAEXXZ` (public virtual void ()).
  Sources: (a) Ego_r.pdb ghidra_out/egor_pdb_names.tsv 009168d0 ?ShuffleCards@CTCTavernGameCardBase@@UAEXXZ (egor address space);
  (b) rtti-vtables: 0x008F10B0 is slot 60 of CTCTavernGameCardBase vtable 0x01288294 (xref 0x01288384 = 0x01288294+0xF0), inherited
  by Sorting 0x0126F5B4 and Blackjack 0x0126F8F4 slot 60; CTCTavernGameCardPairs overrides slot 60 with 0x007D71E0, and Ego_r has
  ?ShuffleCards@CTCTavernGameCardPairs@@UAEXXZ; 0x007D71E0 also calls GetCardDef + random_shuffle. Header engine/card_sorting_types.h
  declares `virtual void ShuffleCards();`. Manifest label is CORRECT. Only callers: the 3 vtable slots (no direct calls).

## 3. Callees
| addr | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x008F0B20 | 00/8f/CTCTavernGameCardBase_GetCardDef_008f0b20.cpp | CTCTavernGameCardBase::GetCardDef | ?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z (takes CDefPointer<const CTavernGameCardBaseDef>*) | draft: non-const T -- WRONG constness |
| 0x007D7860 | 00/7d/std_randomshuffle_007d7860.cpp (Flags /Gr) | std::random_shuffle<CCard**> (__fastcall) | STLport random_shuffle (leaf ??$random_shuffle@PAPAVCCard@@...) | draft: `Algorithm_FisherYatesShuffle` -- WRONG, made up |
| 0x008F27A0 | 00/8f/CDefClassBase_GetSubDefCTavernGameCardBaseDef_008f27a0.cpp | CDefClassBase::GetSubDef<CTavernGameCardBaseDef> const | ??$GetSubDef@VCTavernGameCardBaseDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z | draft: GetSubDef(CDefPointer<CTavernGameLayoutDef>*) -- WRONG type: it is the SAME CTavernGameCardBaseDef (no "layout def") |
| 0x005E9B10 | 00/5e/global_HideMesh_005e9b10.cpp | `HideMesh` free fn on local CMeshEntity (landed spelling wrong -> NAME_FIXES candidate) | ?HideMesh@CTCTavernGame@@SIXPAVCThing@@@Z (static __fastcall) | draft CTCTavernGame::HideMesh(CThing*) OK |
| 0x005EABC0 | manifest accepted (functions.tsv) | CTCTavernGame::PositionMesh | ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z | draft OK; shared decl under FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH (engine/CTCTavernGame.h:101) |
| vt slot 1 (+4) | def Destroy on refcount 0 | | | |
- NOT a callee but INLINED: CTCTavernGameCardBase::GetPackPosition 0x008F0B80 (landed 00/8f/CTCTavernGameCardBase_GetPackPosition_008f0b80.cpp,
  PDB ?GetPackPosition@CTCTavernGameCardBase@@QAE?AVC3DVector@@XZ). The draft's GetLayoutDef()->GetCardOffset() is this function.

## 4. Members
this (CTCTavernGameCardBase, engine/card_sorting_types.h, retail sizeof 0x318; CTCTavernGame base via engine/CTCTavernGame.h with FABLE_RETAIL_CTCTAVERNGAME):
- +0x04 ParentThing (CTCBase) -> CThing +0x70 PDef (CDefClassBase*) [GetCardDef row; CTCTavernGame.h uses ParentThing->PeekBaseDef()]
- +0xF0 GamePositionRHSet (CRightHandedSet; header has opaque unsigned char[24]); +0x108 GamePositionPos (C3DVector) [EnterGame 0x007D4E30 row]
- +0x134/+0x138 Pack (std::vector<CCard*>) _M_start/_M_finish (Ego_r Pack +0x150; Cards is +0x128 retail / +0x140 Ego_r).
  NB the random_shuffle row comment says "this+0x134 = Cards" -- wrong arithmetic; Pointer +0x140 retail (=Ego_r 0x160-0x20) proves Pack=0x134.
def (CTavernGameCardBaseDef, engine/CTavernGameCardBaseDef.h, FABLE_RETAIL_CTAVERNGAMECARDBASEDEF; retail offsets = header): count +0x04,
  CardWidth +0x2c, CardHeight +0x30, CardSeparation +0x5c, TableSeparation +0x60, PackOffset +0x64 (header splits PackOffset_X/_Y floats),
  CardPack +0x78 vector<CCardDef> (header: OPAQUE unsigned char[0xc]; CCardDef sizeof 0x44, engine/CCardDef.h; landed Transfer 0x004F9A90 models it as std::vector<CCardDef>).
CCard (card_sorting_types.h): Thing +0x00 (CThingObject*), FinalRotation +0x10 (float = 0.5f; draft calls it "Alpha"), FaceDown +0x25 (bool, draft name ok),
  LocalPos +0x27, NewLocalPos +0x33 (draft "RestPosition"/"TargetPosition" -- wrong names) under FABLE_RETAIL_CCARD_POSITIONS (+ requires FABLE_RETAIL_C3DVECTOR_PACKED).
- Locally modelled in draft though shared headers exist: CTCTavernGameCardBase, CCard, CTavernGameCardBaseDef, C3DVector (engine/C3DVector.h), C2DVector,
  CDefPointer (engine/CDefPointer.h), CDefClassBase, CThing, CTCTavernGame, CRightHandedSet. Plus invented CTavernGameLayoutDef/CThingHead/CCardEntry.
- C3DVector.h opt-ins: FABLE_RETAIL_C3DVECTOR_PACKED, FABLE_RETAIL_C3DVECTOR_ADD2D / _ADD2D_DIRECT (operator+(C2DVector)), _CLEAR, etc.

## 5. Visibility
- GetCardDef 0x8F0B20 and GetPackPosition 0x8F0B80 are in this TU, defined earlier (same 0x8F09xx..0x8F2Bxx CTCTavernGameCardBase block). Retail
  INLINED GetPackPosition (and the GetCardDef inside it) in the loop, but kept the top-level GetCardDef(&def) call OUT of line. Both landed rows use
  a noinline "stand-in body" for their out-of-line callee; author must make GetPackPosition (with an inlinable GetCardDef body = `CDefPointer def;
  ParentThing->PDef->GetSubDef(&def); *out = def;`) visible, and still get the direct call emitted. Hypothesis: top-level call made through a
  non-inlined path while GetPackPosition (small) gets inlined with its nested GetCardDef -- try `inline_depth`/defining order, or GetCardDef body visible
  but `#pragma auto_inline(off)` around only... (author to probe).
- GetSubDef<CTavernGameCardBaseDef> 0x8F27A0: template COMDAT placed after this block (out of line; called, never inlined).
- random_shuffle 0x7D7860 is a COMDAT in the Pairs TU block, __fastcall (landed with Flags /Gr). fable_containers.h:1095 only declares the 3-arg
  __fastcall random_shuffle; the 2-arg form has NO shared opt-in -> declare locally (`template<class I> void __fastcall random_shuffle(I,I);` in std)
  or the TU is /Gr (no 0x8F row carries /Gr; Pairs' ShuffleCards 0x7D71E0 is the other caller, unlanded).
- HideMesh/PositionMesh: CTCTavernGame TU (0x5E....), not visible.

## 6. Siblings
- 00/8f/CTCTavernGameCardBase_GetPackPosition_008f0b80.cpp (local models; stand-in GetCardDef; dtor no null test, local count).
- 00/8f/CTCTavernGameCardBase_GetCardDef_008f0b20.cpp (CDefPointer operator= shape; stand-in GetSubDef).
- 00/7d/CTCTavernGameCardSorting_EnterGame_007d4e30.cpp (GetCardDef + PositionMesh(…GamePositionPos, GamePositionRHSet, C3DVector temp…)).
- 00/7d/CTCTavernGameCardSorting_CardsMainGame_007d5070.cpp and 00/8f/..._GetRandomCard_008f09b0.cpp: Flags none; shared-header idiom:
  #define FABLE_RETAIL_CCHARSTRING, _CWIDESTRING, _CDEFSTRINGTABLE, _DEF_CLASS_BASE, _CTCTAVERNGAME, _CTCTAVERNGAME_GETDEF_OUTOFLINE,
  _CTAVERNGAMECARDBASEDEF, _CGAMECOMPONENT, _CMAINGAMECOMPONENT, _SCOPED_PTR; #include "fable_game.h", engine/CTavernGameDef.h,
  engine/CTavernGameCardBaseDef.h, engine/card_sorting_types.h.
- 00/8f/CCard_Move_008f0660.cpp / CardsMoving_008f2280: CCard modelled under #pragma pack(1) with C3DVector.

## 7. Draft model errors
- Invented "CTavernGameLayoutDef"/GetLayoutDef/GetCardOffset: actually GetPackPosition() on the same CTavernGameCardBaseDef (PackOffset*CardWidth/Height).
- Names: Algorithm_FisherYatesShuffle -> std::random_shuffle; DeckFirst/DeckLast -> CardPack (vector<CCardDef>, size()); ShuffledFirst/Last -> Pack;
  DeckRH/DeckPosition -> GamePositionRHSet/GamePositionPos; Alpha -> FinalRotation; RestPosition/TargetPosition -> LocalPos/NewLocalPos;
  CardStackHeightStep/BaseHeight -> CardSeparation/TableSeparation; Owner/DefClass -> ParentThing/PDef; CDefPointer must be <const T>.
- Loop form: retail is `if (size) do {...} while (i < CardPack.size())` with size re-evaluated -- a `for (i=0; i<CardPack.size(); ++i)` likely
  gives the same rotated form; height is a separate counter starting 0x35 decremented each pass (int, fild).
- pos assembly: write `C3DVector pos(0,0,(float)height*CardSeparation+TableSeparation); pos = pos + GetPackPosition();` (or +=), card->LocalPos = pos;
  card->NewLocalPos = pos; -- residue (466v410) came from by-value layout temp + extra copies; the frame 0x20 needs def slot reuse (GetCardDef
  must be known not to keep &def) and the inner def held in esi.

## 0x00ADDBE0

# DOSSIER 0x00ADDBE0 -- C3DBoundingSphere::BuildFromSubSpheres (414 bytes)

## 1. Retail shape (xdis 00addbe0 0x1ae)
- __thiscall (this in ecx, never saved; all writes via [ecx]), `ret 8` (2 args). No frame pointer (no push ebp), no SEH. `sub esp,0x14` = exactly 5 float locals.
- Pushes: ebx (=spheres), esi (=count), edi pushed only inside the `count>0` guard (loop-1 down-counter).
- Locals (pre-edi offsets): [8]=max.X, [c]=min.X, [10]=max.Y, [14]=min.Y, [18]=min.Z, all init with `mov imm` 0x7149f2ca (+1e30) / 0xf149f2ca (-1e30).
  **max.Z lives in st0 for the whole loop 1**: `fld [0x0129DCA4]` (= -1e30f) is the 2nd instruction.
- Loop 1 (0xADDC30..0xADDCD0): ROLLED, `mov edi,esi ; dec edi ; jne`, pointer edx=&spheres[0].Radius stepping 0x10.
  Order per sphere: min.X, min.Y, min.Z (v = C - R; `fld min; fcomp v; test ah,41h; jne skip` => store if min > v),
  max.X, max.Y (v = C + R; `fld max; fcomp v; test ah,5; jp skip` => store if max < v),
  max.Z: v=C.Z+R spilled to the dead `count` arg slot [esp+0x28], `fcom [tmp]` against the enregistered max.Z, reload on replace.
- After: `test esi,esi` hoisted; centre = (max + min) * 0.5f per component (fld max; fadd min; fmul [0x0122F59C]=0.5f) -- MAX FIRST.
  `mov [ecx+0xc],0` (Radius = 0) is scheduled before the Center stores. centre.X/Y/Z are homed in dead arg slots
  ([esp+0x24]=count slot, [esp+0x20]=spheres slot) and min.Z's slot [esp+0x18]; each is copied to this->Center via eax/edx (integer mov).
- Loop 2 (0xADDD30..0xADDD74): ROLLED, `dec esi ; jne`, edx=&spheres[0].Center.Z, stride 0x10. d = s.Center - centre (reads the
  STACK centre, not [ecx]); magnitude = fsqrt(dz*dz + dx*dx + dy*dy) i.e. source order X*X+Z*Z+Y*Y (= the shared header's DEFAULT
  GetSquaredMagnitude `X*X+Z*Z+Y*Y`, and default GetMagnitude `(float)sqrt(GetSquaredMagnitude())`); reach = mag + s.Radius;
  `fcom [ecx+0xc]; test ah,41h; jne` => Radius = reach if reach > Radius.
- No size-opt idioms; speed-optimised (/O2-style), `lea esp,[esp]` / `lea ecx,[ecx]` loop-alignment padding.
- Constants: 0x0129DCA4 = -1e30f, 0x0122F59C = 0.5f.

## 2. Identity (label is CORRECT)
- Ego_r.pdb: `?BuildFromSubSpheres@C3DBoundingSphere@@QAEXPBV1@J@Z` (egor_pdb_names.tsv / egor_signatures.tsv, Ego_r 0x00BFA6B0).
- FableWin.pdb: same mangled name (_fablewin_api.tsv 0x017D45CE thunk, 0x0338B200 body; naming_stragglers/FableWin.exe.signatures.tsv).
- Signature: `public: void __thiscall C3DBoundingSphere::BuildFromSubSpheres(const C3DBoundingSphere* spheres, long count)`; non-const, non-virtual.
- Neighbours in Ego_r order: FastBuildFromPoints(ptr) 0xBF9FD0, AccurateBuildFromPoints(ptr) 0xBFA300, BuildFromSubSpheres 0xBFA6B0, AddToBounds 0xBFA8B0 -- same TU as retail 0xADD690 / 0xADD920 / 0xADDBE0 / 0xADDD80.
- Only caller: 0x00BDF34E in CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren (retail 0x00BDF100, parked, group G00add690):
  `push (end-begin)>>4 ; push begin ; lea ecx,[ebp+0x18] ; call` -- a vector<C3DBoundingSphere> (global at 0x0143B978) begin/size.

## 3. Callees
- NONE (no call/jmp out; sqrt is the fsqrt intrinsic). Nothing to respell. Draft's local `Length()` should be the header's `GetMagnitude()` (PDB/FableWin spelling; sibling AddToBounds 0xADDD80 already respelled Length->GetMagnitude).

## 4. Members
- this/spheres: C3DBoundingSphere (struct_layouts_egor: Center C3DVector +0, Radius float +0x0C, size 16) -> rebuild/include/engine/C3DBoundingSphere.h
  (members Center, Radius public). Opt-ins there: FABLE_RETAIL_C3DBOUNDINGSPHERE (ctor(center,radius)), _CENTER_VALUE (GetCenter),
  _FASTBUILD (FastBuildFromPoints decl). **No BuildFromSubSpheres declaration exists** -- the header needs a new opt-in decl
  (pattern of FABLE_RETAIL_C3DBOUNDINGSPHERE_FASTBUILD) for a row using the shared header.
- C3DVector X/Y/Z +0/4/8 -> rebuild/include/engine/C3DVector.h (default ops are FABLE_VECTOR_INLINE const-returning operator+/-/*,
  default GetSquaredMagnitude X*X+Z*Z+Y*Y, GetMagnitude = (float)sqrt(GetSquaredMagnitude())).
- C3DBoundingBox (MinCoord/MaxCoord, private) header exists; its default GetCenter = (MaxCoord + MinCoord) * 0.5f -- the same max-first order retail uses here (possible original idiom: build box, take its centre; unproven).
- Old draft models LOCALLY: C3DVector and C3DBoundingSphere -- both have shared headers (must switch).

## 5. Callee-body visibility
- No callees. Same-TU landed functions: AccurateBuildFromPoints 0xADD920 (defines a noinline FastBuildFromPoints stand-in), AddToBounds 0xADDD80.
  Irrelevant to codegen here except header ops being visible inline.

## 6. Siblings
- rebuild/src/compiled/00/ad/C3DBoundingSphere_AddToBounds_00addd80.cpp: `#include <math.h>` + engine/C3DVector.h, local C3DBoundingSphere class (pre-header era), GetMagnitude, no pragmas/Flags.
- rebuild/src/compiled/00/ad/C3DBoundingSphere_AccurateBuildFromPoints_00add920.cpp: engine/C3DVector.h, `extern "C" double __cdecl sqrt(double); #pragma intrinsic(sqrt)`, GetSquaredMagnitude default, no Flags.
- rebuild/src/compiled/00/ad/C3DBoundingSphere_AccurateBuildFromPoints_00addf70.cpp: thin forwarder.
- Parked same-TU twin 0x00ADD690 FastBuildFromPoints (rebuild/candidates/manual/pending/2026-10-27_linux_hand680/differ/00add690.cpp, DIFFER 646v648):
  ITS min/max loop is ROLLED in our build with `if (count > 0) for (i = 0; i != count; ++i) {...}` (`!=` test) -- the same rolled
  edx/esi-down-counter shape retail has here. Strongest lead for the unroll problem.

## 7. Old-draft model errors
- hand257 draft (1124v414): `for (i=0; i<count; ++i, ++s)` -> VC7.1 RTM/QFE unroll loop 1 by 4. Retail is rolled.
- retry2 draft (rebuild/candidates/manual/pending/2026-10-31_parked_retry2_sec5/differ/00addbe0.cpp, 430v414): guarded do/while(--i) keeps it rolled;
  left: frame 0x24 vs 0x14 (extra centre temporaries) + compare scheduling. Retail has only 5 stack floats + max.Z in st0 and puts the centre
  in dead arg slots; so the centre computation must not leave (max+min) and *0.5f temporaries on the frame -- try the shared header ops
  (const-returning FABLE_VECTOR_INLINE), component-wise centre, or C3DBoundingBox-style GetCenter.
- Local C3DVector/C3DBoundingSphere stand-ins (must use shared headers); `Length()` should be `GetMagnitude()` (default header body matches retail's X*X+Z*Z+Y*Y order).
- Compare spellings min>v / max<v / reach>Radius and loop-2 shape are already right; retail reads spheres via one base pointer, not index*16.

## 0x0065A41C

# DOSSIER 0x0065A41C CTCVillage::GetFineAmountForCurrentCrimes (415 bytes)

## 1. Retail shape (xdis 0065a41c 0x1af)
- __thiscall, no args, plain `ret` (0x65A5BA), `leave`. Frame pointer: `push ebp; mov ebp,esp; sub esp,24h` -> `#pragma optimize("y",off)`.
- /GS: cookie `mov eax,[0x0139C8A8]` -> [ebp-4], checked by `call 0x00BFE9F9` before `leave` => `// Flags: /GS` (the 10-byte bool array is the GS buffer). No SEH.
- Size idioms (`optimize("s",on)`): `push 51h; pop edi`, `and [ebp-1Ch],0`, `or [ebp-14h],-1`, memset of 10 bytes as `xor eax; stosd; stosd; stosw` at [ebp-10h].
- Registers: this=ebx (whole function), region-def ptr=edi spilled [ebp-18h], fine=[ebp-14h], hero=esi, deed log=[ebp-20h], key/effects-def=[ebp-1Ch] (shared slot), set end=[ebp-24h], cursor=edi (after push 51h).
- Exits: def null -> straight to `xor eax,eax` (0x65A5AC, no release); def->UseCrimes==0 -> release def, return 0 (0x65A5A0); main path -> release def, return fine.
- Fine==-1 abort path (0x65A55D): `or fine,-1`, release effects def WITHOUT clearing its pointer, then jumps to 0x65A57D/0x65A580 which skips the `fine<0` test (constant-folded) and does `AmountOfFineAlreadyPaid = -fine (=1); fine = 0`, returns 0.
- Normal tail: `if (fine < 0) { AmountOfFineAlreadyPaid = -fine; fine = 0; }` emitted as `mov eax,fine; neg; and fine,0; mov [ebx+A0h],eax`.
- In-loop effects-def release: `mov ecx,eax; add eax,4; dec [eax]; cmp [eax],0; jne` (DecRef()+GetRefCount()==0, separate cmp) then slot 1, then `and [ebp-1Ch],0` (pointer cleared). No null test on the effects def before deref (same as sibling 0x65A5BB).
- Region-def add-ref: `test edi,edi; je; inc [edi+4]` = CDefPointer raw-ctor (null-tested ++count); release `dec [edi+4]; jne` (`if (--m_count == 0) Destroy()`), no null test on release (already known non-null).

## 2. Name / signature (proven, 2 sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, Ego_r addr 0x0069BE20) and FableWin.pdb (ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv): `?GetFineAmountForCurrentCrimes@CTCVillage@@QAEJXZ` => public, non-const, `long CTCVillage::GetFineAmountForCurrentCrimes()`. Manifest label is CORRECT.
- Landed callers already bind `long GetFineAmountForCurrentCrimes(); // 0x0065A41C`: CTCVillage_PayFines_0065c760.cpp, CTCVillage_IsHeroCriminal_0065bfce.cpp, CTCGuard_IsAttackingToFineRatherThanKill_007b2cf0.cpp, CAIStateGroupMeleeCombatNearbyBase_RequestMoney_0093d580.cpp, CPlayerGui_UpdateCrime_004385f0.cpp.
- Not a virtual (no vtable slot). engine/CTCVillage.h does NOT declare it (grep) -> declare in a local class model or in the row.

## 3. Callees (landed file / landed name / PDB name; draft spelling)
| addr | landed file (rebuild/src/compiled/...) | landed / PDB name | old draft spelling |
|---|---|---|---|
| 0x00686D30 x3 | 00/68/CTCBase_GetWorldMap_00686d30.cpp | CTCBase::GetWorldMap() `?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ` (protected, returns CWorldMap&) | `CWorldMap* GetWorldMap() const` WRONG (ref, non-const, protected) |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const `?GetPos@CThing@@QBEABVC3DVector@@XZ` | `GetPosition` WRONG |
| vslot 16 (+0x40) = 0x004FC0C0 | 00/4f/...*_004fc0c0.cpp | CWorldMap::GetMapNumberAt(const C3DVector&) const `?GetMapNumberAt@CWorldMap@@UBEJABVC3DVector@@@Z` (rtti-vtables CWorldMap 01244aec slot16) | `GetMapAtPosition` WRONG (sibling 0x65A28F says GetMapAtPos, also wrong) |
| 0x004FC190 | 00/4f/CWorldMap_GetRegionNumberMapIsIn_004fc190.cpp | `?GetRegionNumberMapIsIn@CWorldMap@@QBEJJ@Z` (const) | name ok, missing const |
| 0x004FB490 | 00/4f/CWorldMap_GetRegion_004fb490.cpp | `?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z` -> CRegion& | `GetRegionInfo` / CRegionInfo* WRONG |
| 0x0049D9D0 | 00/49/CWorld_PeekMyPlayer_0049d9d0.cpp | CWorld::PeekMyPlayer() const `?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ`, on CMainGameComponent::Get()->World (+0x24) | `CPlayerManager::GetMainPlayer` / `g_pMainGameComponent->PlayerManager` WRONG |
| 0x00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` | `GetHeroThing` WRONG |
| 0x00410DE0 | 00/41/STLbitset_test_00410de0.cpp | std::bitset<274>::test `?test@?$bitset@$0BBC@@std@@QBE_NI@Z` (FABLE_RETAIL_STLP_BITSET_TEST_DECL) | local CBitset WRONG |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (engine/CVectorMap.h) | local CInterfaceMap WRONG |
| 0x006C58F0 | 00/6c/CTCHeroOpinionDeedLog_GetDeedFromHandle_006c58f0.cpp | `?GetDeedFromHandle@CTCHeroOpinionDeedLog@@QBEPBVCOpinionDeed@@J@Z` (const, returns const COpinionDeed*) | non-const CDeed* WRONG |
| 0x006C5630 | 00/6c/CTCHeroOpinionDeedLog_PeekPDeedEffectsDef_006c5630.cpp | `?PeekPDeedEffectsDef@CTCHeroOpinionDeedLog@@QBE?AV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@W4EOpinionDeedType@@@Z` | `GetDeedDef` / CDeedDefPointer WRONG |
| 0x004292C0 | 00/42/stdRbtreebase_Rbtreeincrement_004292c0.cpp | STLport _Rb_tree increment, `__cdecl` (siblings spell `SNode* __cdecl Successor(SNode*)`) | `IncrementNode` (descriptive; follow sibling) |
| slot 1 (+4) | - | def Destroy/DeleteThis (sibling spells `Destroy() const`) | DeleteThis |
| 0x00BFE9F9 | - | __security_check_cookie (/GS, compiler-emitted) | - |
Global 0x013B86A0 = protected static `CMainGameComponent::PMainGameComponent`, read via inline `static CMainGameComponent* __fastcall Get()` (Naming wave 18; ESTABLISHED_SPELLINGS). Draft's `g_pMainGameComponent` is a retired stand-in.

## 4. Members
- CTCVillage (engine/CTCVillage.h, opt-in FABLE_RETAIL_CTCVILLAGE gives CTCBase base + retail offsets): ParentThing +0x04 (CTCBase); PlayersCrimes +0x64 (std::set<long> only with FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES + FABLE_RETAIL_STLP_SET, else opaque byte[0xc]); AmountOfFineAlreadyPaid +0xA0 (long; PDB +0x12C; static-assert line 276) -- draft calls it `CrimeCredit` WRONG. Landed PayFines 0x0065C760 uses AmountOfFineAlreadyPaid.
- CRegion retail +0x24 = PDef `CDefPointer<const CRegionDef>` (PDB +0x2C; retail 8 lower, see CWorldMap_GetRegion_004fb490.cpp). engine/CRegion.h is PDB layout (opt-in FABLE_RETAIL_CREGION exists, check offsets) -- siblings model CRegion locally.
- CRegionDef +0x9D = `UseCrimes` (bool; PDB +0xA9), proven by landed CRegionDef_Transfer_004715ab.cpp (+0x9C WeaponOutCrime, +0x9D UseCrimes, +0x9E DisplayCrimes, +0x9F PermitCrimes). Draft `HasLaw` and sibling 0x65A28F `HasFines` are descriptive/WRONG -> use UseCrimes. Def ref count +0x04, slot 1 destroy. No engine/CRegionDef.h.
- COpinionDeedEffectsDef (engine/COpinionDeedEffectsDef.h, retail layout, static assert CrimeFineAmount==0x74): +0x74 `long CrimeFineAmount` (draft `FineAmount`), +0x7C bitfield byte, mask 0x20 = `bool OnlyGetFinedForOneInstance:1` (6th bit: CanBeHeard,ReactionRestrictToTarget,AllowSimultaneousReactionDialogue,CanApologise,MustNotRunWhenWarned,OnlyGetFinedForOneInstance). Use the bitfield, not `Flags & 0x20`.
- COpinionDeed +0x00 `EOpinionDeedType DeedType` (draft `CDeed::Type`). engine/COpinionDeed.h exists.
- CThing +0x20 bitset<274> (UsingComponents), +0x44 CVectorMap components (engine/CVectorMap.h; siblings model CThing locally with HasTC / FABLE_FORCEINLINE GetTC).
- STLport set node: value +0x10, header->_M_left = begin.
- Local bool array: 10 bytes [ebp-10h], indexed by deed->DeedType (re-read `[esi]` for each index); zeroed by memset-intrinsic shape. Real name/size unknown (10 bytes exactly from stosd,stosd,stosw).
- Classes the draft models locally that have shared headers: CTCVillage, CThing, CMainGameComponent, CWorld, CPlayer, CRegion, CWorldMap, CTCHeroOpinionDeedLog, COpinionDeed(=CDeed), COpinionDeedEffectsDef(=CDeedDef), CDefPointer, C3DVector, CVectorMap. Siblings still use local models for most (retail offsets differ from PDB headers) but use engine/C3DVector.h and engine/CVectorMap.h + stlport bitset.

## 5. Callee visibility
- All callees out of line in retail (call rel32). Inline in source: CMainGameComponent::Get(), CThing::HasTC/GetTC (GetTC FABLE_FORCEINLINE; LowerBound out of line), CDefPointer ctor (null-tested add-ref) / FABLE_FORCEINLINE dtor, the two Release forms (region def: `if (--m_count==0) Destroy()`; effects def: DecRef(); if(GetRefCount()==0) Destroy(); then Object=0).
- Opt-ins: FABLE_RETAIL_STLP_BITSET_TEST_DECL (bitset test decl), engine/CVectorMap.h, FABLE_RETAIL_CTCVILLAGE(+_MIRROREDCRIMES, FABLE_RETAIL_STLP_SET), engine/CDefPointer.h requires FABLE_RETAIL_DEF_CLASS_BASE and offers FABLE_RETAIL_CDEFPOINTER_RELEASE_{SELFDELETING_POINTEE,COUNTREF,POINTEE}, _RESET.
- Same-TU neighbours (CTCVillage.cpp): 0x65A1FD IsHeroMurderer, 0x65A28F GetNormalisedTimeLeftOnFines (immediately before), 0x65A5BB GetRegionLeavingDurationForCurrentCrimes (immediately after), 0x65A686 PlayerCanApologiseForCurrentCrimes. None are called here.

## 6. Siblings to copy (all rebuild/src/compiled/00/65/)
- CTCVillage_GetNormalisedTimeLeftOnFines_0065a28f.cpp -- NEAR-TWIN: identical prologue (3x GetWorldMap, GetPos, slot16, GetRegionNumberMapIsIn, GetRegion, region->PDef add-ref, +0x9D test, hero chain, 0x51 deed log, set walk, PeekPDeedEffectsDef). Pragmas `optimize("y",off)` + `optimize("s",on)`, `#include "fable_compiler.h"`, `#define FABLE_RETAIL_STLP_BITSET_TEST_DECL`, engine/CVectorMap.h, engine/C3DVector.h, engine/EOpinionDeedType.h; local CDefPointer with raw ctor + FABLE_FORCEINLINE dtor; `CRegion* region = &GetWorldMap().GetRegion(...)`; `CDefPointer<CRegionDef> def = region->PDef; if (def.Object && def->X) {...} return 0;`. NOTE its register allocation is this=edi/def=ebx, ours is this=ebx/def=edi.
- CTCVillage_GetRegionLeavingDurationForCurrentCrimes_0065a5bb.cpp -- same hero/deed-log/set-walk loop, `T* log; if (HasTC(0x51)) log = GetTC(0x51);`, `SNode* end = PlayersCrimes; for (it = end->_M_left; it != end; it = Successor(it))`.
- CTCVillage_PayFines_0065c760.cpp (AmountOfFineAlreadyPaid +0xA0), CTCVillage_IsHeroMurderer_0065a1fd.cpp (PlayersCrimes +0x64), CTCVillage_PlayerCanApologiseForCurrentCrimes_0065a686.cpp.
- `// Flags: /GS` is an established header line (59 rows in 00/6x).

## 7. Old draft model errors (DIFFER 383v415)
- Region pointer: draft initialises `CVillageInfoPointer village = ...->Village` in one expression; twin 0x65A28F names `CRegion* region` first, then `CDefPointer def = region->PDef`. Draft's destructor `if (P) {...}` creates the merged null-exit; retail jumps def==null directly to `return 0` (copy the twin's `if (def.Object && def->UseCrimes) { ... return fine; } return 0;` structure, release in dtor).
- `fine = -AmountOfFineAlreadyPaid` must be computed right after the UseCrimes test, before the hero chain (retail 0x65A482) -- draft does this already.
- Abort path: draft `fine = -1; break;` matches (retail folds `fine<0`); keep effects-def as a named scoped local released by dtor so the break path releases without clearing.
- Wrong callee names/types throughout (see table): GetWorldMap ref, GetPos, GetMapNumberAt const, GetRegion, PeekMyPlayer/World, GetPControlledCreature, const GetDeedFromHandle, PeekPDeedEffectsDef(EOpinionDeedType), CrimeFineAmount, OnlyGetFinedForOneInstance, UseCrimes, AmountOfFineAlreadyPaid, PMainGameComponent via Get().
- Register allocation (this in ebx vs twin's edi) is the residual; parked notes suggest writing every exit in full and a CDefPointer-style local. Difference from twin likely driven by /GS + the extra `this` use at the tail store.

## 0x0065AA6F

# DOSSIER 0x0065AA6F  CTCVillage::IsTimeToGoToTavern (415 bytes)

## 1. Identity (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x0069c4ef) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x02168319), same leaf:
  `?IsTimeToGoToTavern@CTCVillage@@QAE_NPAVCThingAICreature@@ABVCWorld@@_NM@Z`
  => `bool CTCVillage::IsTimeToGoToTavern(CThingAICreature* creature, const CWorld& world, bool run, float speed)` (public, NON-const).
- Manifest label CTCVillage::IsTimeToGoToTavern is RIGHT. The draft's parameter types are WRONG (CThingCreature*, const CWorldClock&).
- xref.py: no direct callers (0). Landed TWIN with the same body pattern: CTCVillage::IsTimeToGoToSchool 0x0065A8EE
  (rebuild/src/compiled/00/65/CTCVillage_IsTimeToGoToSchool_0065a8ee.cpp) -- COPY IT; this row = school row + leading
  IsTavernTime test + `speed` param (default -1) + facing-target + virtual max-speed instead of animation speed.

## 2. Retail shape (xdis 0065aa6f 0x1af)
- thiscall, `leave ; ret 10h` (4 stack args), ebp frame `push ebp; mov ebp,esp; sub esp,3Ch`, this spilled to [ebp-4]. No SEH.
- Size TU: unaligned entry, `push 23h ; pop edi` constant idiom => `#pragma optimize("s", on)` (school twin uses only that, no "y" off; its frame comes from /Os).
- Flow:
  1. `call 0x658C0A ; test al,al ; je ; mov al,1 ; jmp leave` -- `if (IsTavernTime()) return true;` BEFORE push ebx/esi.
  2. push ebx/esi; ebx = creature, esi = &creature->WorkBuilding (+0x170); GetPItem; temp in [ebp+8] (dead creature slot) passed by ref
     to IS_THING_ALIVE 0x65D624 (fastcall, ecx=&temp); fail -> `pop esi/pop ebx` with al already 0.
  3. push edi; position = creature->GetPos() COPIED to [ebp-18..-10]; WorkBuilding.GetPItem()->GetPos() by ref;
     distance = (position - work).GetMagnitude(): x87 order dz*dz + dy*dy then + dx*dx, fsqrt inline -> [ebp-8]. IDENTICAL to school twin bytes.
  4. HasTC(0x23) via out-of-line bitset::test 0x410DE0 (ecx=ebx+0x20); key 0x23 held in EDI (`push 23h;pop edi;push edi`, `mov [ebp+8],edi`,
     `cmp [eax],edi`) -- school twin uses immediates here (this is the 2-byte/regalloc residue the old draft could not close).
     LowerBound 0x40F020 on ebx+0x44, end-or-greater pick, `mov eax,[eax+4] ; mov ecx,eax ; call 0x7EE540` GetPVillage; test eax.
  5. village->GetTavernTimes(start,end) 0x658C5B with &end=[ebp-0xc], &start=[ebp+8] (dead arg slot -> inner-block start/end like school).
  6. start /= world.PGameTimeManager->TimeStep (`mov eax,[ebp+0Ch]; mov eax,[eax+1Ch]; fdiv [eax+10h]`), stored back to [ebp+8].
  7. target = ParentThing->PhysicsTC->GetFacingVector() + ParentThing->GetPos(): edi=[this+4]; ecx=[edi+60h]; `call [vt+118h]`
     (slot 70, hidden ret [ebp-3Ch]); GetPos(edi) 0x4C73D0; sum (facing first operand) -> [ebp-24..-1c]. Computed UNCONDITIONALLY.
  8. `fld speed ; fld [0x0122DEE0]=-1.0f ; fld st1 ; fucompp ; test ah,44h ; jp` => `if (speed == -1.0f)` (NOT 0.0f);
     then `cmp run,0 ; fstp st0 ; push &target ; je` -> run: creature vslot +0x144 (slot 81 GetMaxRunningSpeed), else +0x13c (slot 79 GetMaxWalkingSpeed).
     Probably `speed = run ? GetMaxRunningSpeed(target) : GetMaxWalkingSpeed(target);` or if/else (both branches merge at 0x65ABE4, speed on st0).
  9. `fld distance ; fmul [0x0125A9A0]=1.6f ; fild ConstantFPS(0x01375550) ; fmul st(2)(speed) ; fdivp ; fld start ; fcompp ; test ah,41h ; je false`
     => `if (start > distance * 1.6f / (CMainGameComponent::GetConstantFPS() * speed)) return false; return true;`
     (school: `float frameSpeed = GetConstantFPS()*speed; ... if (start > travel / frameSpeed) return false; return true;`).
     Shared `xor al,al` block at 0x65ABDA (placed mid-function, as in school).

## 3. Callees (landed file / landed name / PDB)
| addr | landed file | landed name | PDB / note | draft spelling |
|---|---|---|---|---|
| 0x00658C0A | 00/65/CTCVillage_IsTavernTime_00658c0a.cpp | CTCVillage::IsTavernTime() const | ?IsTavernTime@CTCVillage@@QBE_NXZ | **WRONG: IsCookTime** (IsCookTime is 0x00658B6B) |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const | ok | ok |
| 0x0065D624 | 00/65/global_ISTHINGALIVECThingBuilding_0065d624.cpp (leaf `@FableIsAlive_65d624@4`) | IS_THING_ALIVE<CThingBuilding*> | Ego_r ??$IS_THING_ALIVE@PAVCThingBuilding@@@@YI_NABQAVCThingBuilding@@@Z (fastcall, `CThingBuilding* const&`) | ok-ish; school defines it as same-TU `template<class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T&)` |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const | ?GetPos@CThing@@QBEABVC3DVector@@XZ (engine/CThing.h declares it) | **GetPosition -> GetPos** |
| 0x00410DE0 | 00/41/STLbitset_test_00410de0.cpp | std::bitset<274>::test | out of line | draft local CBitset wrong; use std::bitset<274> |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (non-const) | | draft local CInterfaceMap |
| 0x007EE540 | 00/7e/CTCVillageMember_GetPVillage_007ee540.cpp | CTCVillageMember::GetPVillage() const | ?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ | **WRONG: GetVillage** |
| 0x00658C5B | 00/65/CTCVillage_GetTavernTimes_00658c5b.cpp | CTCVillage::GetTavernTimes(float&,float&) const | ?GetTavernTimes@CTCVillage@@QBE_NAAM0@Z | ok |
| [PhysicsTC vt+0x118] slot 70 | 00/6b/CTCPhysicsBase_GetFacingVector_006b1410.cpp | CTCPhysicsBase::GetFacingVector() const (C3DVector by value, `ret 4`) | ?GetFacingVector@CTCPhysicsBase@@UBE?AVC3DVector@@XZ; vtable 0x0125D2C4 slot 70 | **WRONG: CAppearance::GetEntranceOffset** |
| [creature vt+0x13c] slot 79 | 00/83/CThingAICreature_GetMaxWalkingSpeed_00830000.cpp | GetMaxWalkingSpeed(const C3DVector&) const | ?GetMaxWalkingSpeed@CThingAICreature@@UBEMABVC3DVector@@@Z; vtable 0x0127293C slot 79 | **WRONG: GetWalkSpeedTo** (and non-const) |
| [creature vt+0x144] slot 81 | 00/82/CThingAICreature_GetMaxRunningSpeed_0082fed0.cpp | GetMaxRunningSpeed(const C3DVector&) const | slot 81 | **WRONG: GetRunSpeedTo** |
| data 0x01375550 | -- | CMainGameComponent::ConstantFPS (static long) | ?ConstantFPS@CMainGameComponent@@1JA; read via static inline `GetConstantFPS()` (FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ), as school | **WRONG: g_FramesPerSecond** |
| data 0x0125A9A0 = 1.6f, 0x0122DEE0 = -1.0f (read from Fable.exe) | | | | **draft used 0.5f and 0.0f** |

## 4. Members
- creature +0x170 = CThingAICreature::WorkBuilding (CIntelligentPointer<CThingBuilding>; Ego_r +0x198 / HomeBuilding Ego_r +0x190 = retail +0x168).
  Proof: engine/CThingAICreature.h comment under FABLE_RETAIL_CTHINGAICREATURE_SAVE (Save reads WorkBuilding at +0x170) and the
  FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING opt-in (`GetWorkBuilding()` inline, `lea ecx,[esi+170h] ; call 0x00A01B50`). **Draft's "Home" is WRONG.**
  School twin uses a local CThingAICreature model with `CIntelligentPointer<CThingBuilding> WorkBuilding; // +0x170` and calls `.GetPItem()` twice.
- CThing (engine/CThing.h): UsingComponents std::bitset<274> +0x20, Components CVectorMap +0x44, PhysicsTC (CTCPhysicsBase*) +0x60
  (static-asserted in the header), Dead bit +0x91. **Draft's "+0x60 PAppearance" is WRONG (it is PhysicsTC).**
  Opt-ins: FABLE_RETAIL_CTHING_GETTC_EXPANDED (out-param GetTC<T>(T**) with the exact pointer-local/ternary shape),
  FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (NOT wanted: retail calls bitset::test out of line here), GetPhysicsTC() accessor exists.
- CTCVillage this+4 = CTCBase::ParentThing (CThing*).
- CWorld (engine/CWorld.h): PGameTimeManager_ptr +0x1c (scoped_ptr ptr); PeekGameTimeManager()/GetGameTimeManager() inline accessors.
- CGameTimeManager (engine/CGameTimeManager.h, FABLE_RETAIL_CGAMETIMEMANAGER): TimeStep float +0x10 (but private there; school row models it locally
  with public TimeStep and reads `world.PGameTimeManager_ptr->TimeStep`). **Draft's CWorldClock::PDef->HoursPerSecond is WRONG.**
- CTCPhysicsBase (engine/CTCPhysicsBase.h, FABLE_RETAIL_CTCPHYSICSBASE + FABLE_RETAIL_CTCBASE): GetFacingVector() at slot 70.
- CThingCreatureBase.h declares GetMaxWalkingSpeed / GetMaxRunningSpeed pure virtual (const C3DVector&) const.
- C3DVector (engine/C3DVector.h): operator- / GetMagnitude; school matched with the default header spelling `(position - work).GetMagnitude()`.
- Classes the draft models LOCALLY that have shared headers: C3DVector, CThing, CVectorMap (CInterfaceMap), CBaseIntelligentPointer/CIntelligentPointer,
  CTCVillageMember, CTCBase, CTCVillage, CThingCreature(->CThingAICreature), CWorld, CGameTimeManager, CTCPhysicsBase.

## 5. Visibility
- IsTavernTime 0x658C0A, GetTavernTimes 0x658C5B, IsTimeToGoToSchool 0x65A8EE all in the same CTCVillage TU, earlier: retail still calls them (size TU).
- IS_THING_ALIVE<CThingBuilding*> 0x65D624: unaligned COMDAT template emitted in this TU; school defines it as a same-TU `__declspec(noinline)` template.
- bitset::test 0x410DE0 and CVectorMap::LowerBound 0x40F020: out-of-line library/template calls (GetTC body inline).
- GetPos 0x4C73D0 out of line (declared only). GetConstantFPS is inline static.
- C3DVector operator-/operator+/GetMagnitude must be inline (school notes: `__forceinline` needed in this size TU -- header FABLE_VECTOR_INLINE).

## 6. Siblings to copy
- 0x0065A8EE IsTimeToGoToSchool: `#include "fable_compiler.h"`, `#pragma optimize("s", on)`, `#include "rebuild_abi.h"`, `engine/CWorld.h`,
  `engine/C3DVector.h`, `engine/CVectorMap.h`, `<math.h>` + `#pragma intrinsic(sqrt)`; inner `{ float start; float end; ... }` block so start shares [ebp+8].
- 0x00658C5B GetTavernTimes / 0x00658C0A IsTavernTime: `optimize("s", on)`, FABLE_FORCEINLINE CDefPointer copy/dtor.
- 0x0065AC0E BuildingContainsPlayer, 0x00658ACB GetSchoolTimes: same TU, `optimize("s", on)`.
- 0x007EE540 GetPVillage: uses `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED` + engine/CThing.h + engine/CVectorMap.h + FABLE_RETAIL_CINTELLIGENTPOINTER.

## 7. Old draft model errors
1. IsCookTime -> IsTavernTime (wrong callee name).  2. `speed == 0.0f` -> `speed == -1.0f`.  3. `distance * 0.5f` -> `distance * 1.6f`.
4. CWorldClock/PDef/HoursPerSecond -> const CWorld& / PGameTimeManager / TimeStep.  5. CThingCreature/Home -> CThingAICreature/WorkBuilding.
6. PAppearance->GetEntranceOffset -> PhysicsTC->GetFacingVector() (slot 70).  7. GetWalk/RunSpeedTo -> GetMaxWalking/RunningSpeed const.
8. GetVillage -> GetPVillage; HasTC + pointer-returning GetTC -> out-param GetTC<CTCVillageMember>(&member) (school form).
9. g_FramesPerSecond -> CMainGameComponent::GetConstantFPS().  10. `#pragma optimize("y", off)` not used by the matched twin.
11. start/end at function scope; school puts them in an inner block (that is what shares the dead creature slot [ebp+8]).
12. Residue to watch: retail keeps the 0x23 key in edi (`push 23h;pop edi`) and the creature in ebx; edi is pushed late (after IS_THING_ALIVE).
    With the school form the extra register pressure from the leading IsTavernTime test/speed path may make VC pick this itself.

## 0x00BBCC70

# DOSSIER 0x00BBCC70 CEnginePrimitiveManagerParticleGroup::SetupPixelShader (415 bytes)

## 1. Retail shape (xdis 00bbcc70 0x1af)
- __thiscall, `ret 0Ch` (3 stack args), this in ecx read once (`mov al,[ecx+22h]`). No frame pointer, no SEH, no stack locals.
- esi/edi pushed only on the true path (shrink-wrapped after the `je`). Else path: 2 calls, no pushes.
- True path: SetUserVertexShaderConstant(reg, 1/32, 1/32, 320, 240) [push 43700000=240, 43A00000=320, 3D000000 x2];
  then TWO inline SetRenderState blocks on GPRenderManager (0x01436E18): +0x2EF8 and +0x2F98 with value = [0x01396DC8];
  core + value RE-READ after each changing block (`a1 186e4301 ; 8b35 c86d3901`). Block 2 has the odd scoped
  `push ebp ; mov ebp,[eax+2F9Ch] ; lea edx,[eax+2F98h] ; cmp ebp,esi ; mov ecx,eax ; mov edi,esi ; pop ebp ; je`.
- Then texture stage 1: `ecx=[0x01436E98] ; add ecx,2AACh ; je ->0 ; ecx=[ecx]` (= `tex ? tex->PD3DTexture : 0` on
  &GPResourceManager->StipleFadeTexture), `mov esi,eax` (core copy), `cmp [eax+3D04h],ecx` / store / device->SetTexture
  (`push ecx; push 1; push dev; call [edx+104h]` = slot 65), then `mov eax,2 ; cmp [esi+3CB8h] ; jge ; mov [esi+3CB8h],eax`.
- Then ApplyPixelShader(arg2 @[esp+10h]) on GPShaderRenderManager 0x01436E14. Else: SetUserVertexShaderConstant(reg,0,0,0,0)
  + ApplyPixelShader(arg3).

## 2. Identity
- Mangled: `?SetupPixelShader@CEnginePrimitiveManagerParticleGroup@@QAEXKABVCPixelShader@@0@Z` -- Ego_r.pdb
  (egor_pdb_names.tsv @ego 00a719c0) AND FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv 02ea6000, 234 bytes).
  => public, non-virtual, non-const: `void SetupPixelShader(unsigned long, const CPixelShader&, const CPixelShader&)`. Label CORRECT.
- Not in a vtable. Callers (xref): 0x00BBE6C2 (inside 0x00BBE090 CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail),
  0x00BBFE49, 0x00BC0E6B, 0x00BC2C77, 0x00BC4608: all pass reg=0, arg2 = shader at +0x28/+0x50 (stipple), arg3 = +0x20/+0x48 (normal).
  Pending hand723 draft 00bc04f0 calls `SetupPixelShader(0, PixelShaderStipple, PixelShader)` -> param names stipple/normal (descriptive;
  PDB has no param names). Draft's `maskedShader/plainShader` should become stipple-ish names.
- Flag at +0x22 = `WriteZBuffer` (struct_layouts_egor.tsv CEnginePrimitiveManagerParticleGroup: +0x20 ClipEffectToWeatherMask,
  +0x21 ReadZBuffer, +0x22 WriteZBuffer, +0x23 DisplayBoundingBox, +0x24 DisplayBoundingSphere).

## 3. Callees
| target | landed row | landed name | PDB | draft disagreement |
|---|---|---|---|---|
| 0x00989B00 | rebuild/src/compiled/00/98/CShaderRenderManager_SetUserVertexShaderConstant_00989b00.cpp | SetUserVertexShaderConstant(long,...) Leaf `...QAEXJMMMM@Z` | Ego_r+FableWin `?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKMMMM@Z` (unsigned long) | landed row Leaf says J (long) -> NAME_FIX for row 00989b00; engine/CShaderRenderManager.h:129 already `unsigned long` (K). Draft's unsigned long is right. |
| 0x00988140 | .../00/98/CShaderRenderManager_ApplyPixelShader_00988140.cpp | ApplyPixelShader(const CPixelShader&) | `?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z` | ok; header engine/CShaderRenderManager.h:90 |
| IDirect3DDevice9 slot 65 (+0x104) SetTexture | via inline CRenderManagerCore::AttachD3DTextureToStage (engine/CRenderManagerCore.h ~l.124) | | | draft hand-models device vtable locally |
| inline AttachTextureToStage | out-of-line COMDAT 0x009A0CF0 (CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp) -- retail EXPANDS it here | | `?AttachTextureToStage@CRenderManagerCore@@QAEXJPBVCTexture@@@Z` | draft calls it `SetTexture` (wrong name) |
| inline GetStipleFadeTexture | none (inline) | | FableWin `?GetStipleFadeTexture@CEngineResourceManager@@QAEAAVCTexture@@XZ` (returns CTexture&) | draft invents `CTextureHolder MaskTexture` / `CEngineData_01436e98` |
| inline SetTextureUVModeU/V | none (inline, header) | | Ego_r/FableWin SetTextureUVModeU/V(long, TEXTURE_UV_MODE) | draft uses free `SetState(...)` with invented `m_state2ef8` |

Globals (PDB names, as in landed sibling 0x00BBC460): `NEngineInternal::GPRenderManager` (CRenderManager*, 0x01436E18),
`NEngineInternal::GPShaderRenderManager` (0x01436E14), `NEngineInternal::GPResourceManager` (CEngineResourceManager*, 0x01436E98;
landed rows cite it). 0x01396DC8 = `CRenderStateManager::D3DTextureUVModeTable[0]` (private static, engine/CRenderStateManager.h:304),
i.e. TEXTURE_UV_WRAP (=0, engine/RenderStateEnums.h:39). Draft's `g_renderStateDefault_01396dc8`, `g_pRenderManagerCore`,
`g_pEngineData_01436e98`, `g_pShaderRenderManager` are all descriptive stand-ins.

## 4. Members
- this+0x22 WriteZBuffer (bool). No shared header for CEnginePrimitiveManagerParticleGroup; copy the local model from landed
  0x00BBC460 (pack(1), base engine/CEnginePrimitiveManager.h, CurrentWorldPos at +0x0E). Draft model puts CurrentWorldPos at +0x10 (wrong vs PDB; harmless here).
- Render core (engine/CRenderStateManager.h retail branch, turned on by engine/CRenderManagerCore.h's FABLE_RETAIL_CRENDERMANAGERCORE):
  DeviceState +0x2818; CDeviceState::TextureUVModeU[8] +0x6CC, TextureUVModeV[8] +0x76C -> stage 1 = +0x2EF8 / +0x2F98;
  CStateInfo{CurrentState+0, DesiredState+4, BookmarkMask+8, DirtyListFlag+0x10}; StateStack +0x08, StateUpdateList +0x2008,
  StateStackSize +0x2808, StateUpdateListSize +0x280C, CurrentBookmarkMask +0x2814.
- CRenderManagerCore (engine/CRenderManagerCore.h): TextureStagesInUse +0x3CB8, PD3DDevice +0x3CF0, CurrentlySelectedTextures[8] +0x3D00.
- CEngineResourceManager retail branch (FABLE_RETAIL_CENGINERESOURCEMANAGER, engine/CEngineResourceManager.h): ShaderResourcePixel +0x2A98,
  double MaxLoadingTime +0x2AA0, VertexBufferUsageFlags +0x2AA8, StipleFadeTexture (unsigned char[8], CTexture) +0x2AAC -- PRIVATE
  (after `private:` at l.76); header has NO GetStipleFadeTexture accessor yet (pattern to copy: GetGPUScratchBufferTextureManager l.72,
  `return *(CTexture*)StipleFadeTexture;`). CTexture: PD3DTexture at +0 (FABLE_RETAIL_CTEXTURE, set by CRenderManagerCore.h).
- Opt-ins available: FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS (expands AttachTextureToStage; without it the TU calls 0x009A0CF0 -> wrong),
  FABLE_RETAIL_CRENDERSTATEMANAGER_EARLY_RETURN (alternate SetRenderState spelling), FABLE_RETAIL_CENGINERESOURCEMANAGER.
- Draft LOCAL models that have shared headers: IDirect3DDevice9 (engine/IDirect3DDevice9.h), CTrackedRenderState/SCapturedRenderState/
  CRenderManagerCoreView (engine/CRenderStateManager.h + CRenderManagerCore.h / CRenderManager.h), CShaderRenderManager
  (engine/CShaderRenderManager.h), CPixelShader (via CShaderRenderManager.h), CEngineData_01436e98 (engine/CEngineResourceManager.h),
  CTextureHolder (engine/CTexture.h).

## 5. Visibility
- All state/texture helpers are header inlines (__forceinline) -- no out-of-line call in retail except 0x00989B00 / 0x00988140 (both
  ordinary out-of-line members, declared in CShaderRenderManager.h). Retail expands AttachTextureToStage -> needs the FORCEINLINE_HELPERS opt-in.
- Same-TU neighbours (ParticleGroup source file): 0x00BBC0B0 SetUpPointLights, 0x00BBC130 RenderSubPrimGroups, 0x00BBC460 RenderSubPrimitive,
  0x00BBCC70 (this), 0x00BBCEB0 CreateInternalPrimitive, 0x00BBCF30 GetSupportedTypes. No STLport helper involved.

## 6. Siblings to copy
- 0x00BBC460 CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive (rebuild/src/compiled/00/bb/...RenderSubPrimitive_00bbc460.cpp):
  `// Flags: none`, no optimize pragma; `#define FABLE_RETAIL_CENGINELIGHTINGMANAGER`, `#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS`,
  includes engine/CRenderManager.h, CShaderRenderManager.h, CEnginePrimitiveManager.h; `namespace NEngineInternal { extern CRenderManager* GPRenderManager; }`
  etc.; calls `GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP)` and `->AttachTextureToStage(1, 0)`; local pack(1) class model.
- 0x00B677D0 CEngineLandscapeRenderer::SetupForegroundStates (RELOCATION_MATCH): SAME two blocks (stage-1 U/V, value [0x01396DC8]) and the SAME
  scoped `push ebp` + `mov ecx,eax ; mov edi,esi` in block 2 -- matched by the two-level inline (public setter -> private SetRenderState ->
  AddStateToUpdateList), value as a memory operand (const& / table lookup), bookmark test written `(CurrentBookmarkMask & state.BookmarkMask) == 0`.
- 0x00B46890 CEngineLightingManager::EnableFog: single flat SetState (shows the flat form produces block-1 shape only).
- 0x009A0CF0 CRenderManagerCore::AttachTextureToStage: out-of-line copy of the texture-stage helper.

## 7. Draft model errors
- Flat free `SetState(state, value, core)` (one inline level, core passed in) instead of the retail nested inlines
  `GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP); ->SetTextureUVModeV(1, TEXTURE_UV_WRAP);` -- the block-2 core copy (`mov ecx,eax`)
  and scoped ebp temp are exactly what the nested member form produced in 0x00B677D0. This is the recorded residue (DIFFER 411v415).
- Texture: invented holder `MaskTexture.GetTexture()` with `this ? m : 0`; real = `AttachTextureToStage(1, &GPResourceManager->GetStipleFadeTexture())`
  (CTexture& -> address null-tested by `texture ? texture->PD3DTexture : 0` inside AttachTextureToStage).
- Clamp: header spells `if (TextureStagesInUse < stage + 1) TextureStagesInUse = stage + 1;` with long stage (draft casts) -- fine.
- Lane risk: header SetRenderState writes `(state.BookmarkMask & CurrentBookmarkMask)`; retail here is `mov edx,[mask] ; mov esi,[state+8] ;
  test esi,edx ; or edx,esi` (same as 0x00B677D0, which spells CurrentBookmarkMask first); header notes this order varies per block (lane gap).

## 0x007B8B10

# DOSSIER 0x007B8B10 CTCTavernTable::GetEatingPoint (417 bytes)

## 1. Retail shape (xdis 007b8b10 0x1b1)
- __thiscall, `ret 14h` (5 dword args). NO frame pointer (`sub esp,8 ; push ebx,ebp,esi,edi`), no SEH prologue (no /EHsc frame
  even though two CCharString locals are destroyed -> no EH state; ordinary /O2 /Oy, no optimize("s") idioms, no flags needed so far).
- `mov [esp+14h],ecx` : this spilled to stack (reloaded at 0x7b8bd3 into ebx, and at 0x7b8c3d into edx for ParentThing).
- AvailableForUse test `[ecx+5Ch]` first; false -> shared `return false` epilogue 0x7b8ca5.
- ebp = &pointIndex (arg4, [esp+28h]); esi ALSO loaded from [esp+28h] = the uninitialised `points` local homed in pointIndex's
  arg slot (same idiom as landed FreeSeat 0x007B8AA0 / FreePoint 0x007B9090 notes). ebx = startIndex = 0 (`xor ebx,ebx`).
- Loop head 0x7b8b33 (reached by `jmp` from 0x7b8c44 with eax = ParentThing reloaded via the stack-spilled this):
  `mov ecx,[eax+24h] ; test ch,40h` = HasTC(0x2E) (UsingComponents bit 46 -> word +0x24 bit 14).
  Hit: LowerBound INLINED (not a call to 0x0040F020!): `mov edi,[eax+48h]` (end) ; `mov eax,[eax+44h]` (start) ; count=(end-start)>>3 ;
  it=start(esi); `cdq;sub;sar` halving; `cmp [esi+eax*8],2Eh ; jge`; then `cmp esi,edi ; je ; cmp [esi],2Eh ; jle ; mov esi,edi` ;
  `mov esi,[esi+4]` -> points in esi. `mov edi,edi` 2-byte pad aligns the inner search loop at 0x7b8b50.
  Miss: falls to 0x7b8b7e with old esi; the `push ecx` there just reserves the by-value CCharString arg slot.
- GetFreeNamedActionPoint(user, pos, rh, CCharString("ACTION_POINT_SIT") [0x0126DC30, ctor 0x0099EBF0 with -1], pointIndex(ebp),
  startIndex(ebx), true): pushes `1, ebx, ebp, <slot>`, ctor in place on the arg slot, then user/pos/rh. fail -> `*ebp=-1; return false`
  (own epilogue 0x7b8c49, no dtors).
- `CCharString name` default ctor 0x0099E4B0 at [esp+28h] (pointIndex's arg slot reused); GetName(*pointIndex, name) 0x007FEFC0.
- name.GetLength() inline (`test eax,eax ; je ; mov eax,[eax+4]`), GetSubString(ActionPointSitStrlen [this+48h], len) 0x0099EC70
  into a temp at [esp+10h] (`number`), GFCharStringToInt(number) 0x0099E7F0 __fastcall (ecx=&number) -> edi = chair.
- Range: `ecx=(Food.finish[+2Ch]-Food.start[+28h])>>3 ; lea edx,[edi-1] ; cmp ecx,edx ; jbe BAD` (unsigned size <= seat)
  then `lea eax,[edi-1] ; test eax,eax ; jl BAD` (seat<0, recomputed). BAD (0x7b8c81): SetActionPointAsFree(*pointIndex);
  *pointIndex=-1; ~number; ~name; -> shared return false.
- `lea ecx,[eax+edi*8-8] ; call 0x00A01B50` (GetPItem on Food[seat]) ; nonzero -> `*seat = chair-1` (edi dec'd), ~number, ~name,
  return true (own epilogue). zero -> SetActionPointAsFree(*pointIndex); `ebx = *pointIndex + 1` (reloaded AFTER the call, inc);
  ~number; ~name; reload this->ParentThing; jmp loop head.
- Note: no GetFood (0x007B85C0) call; its size test would be redundant with the explicit one (if GetFood were inlined, its check
  folds away -- possible spelling: `if (seat out of range) {...} if (GetFood(seat))`).

## 2. Identity (2+ sources) -- manifest label is CORRECT
- Ego_r.pdb order (egor_pdb_names.tsv lines 18221-18224): GetSeat 0x7b77b0, FreeSeat 0x7b7a20, **GetEatingPoint 0x7b7ad0**,
  GetServingPoint 0x7b7c80. Retail: FreeSeat landed at 0x007B8AA0 (immediately before), GetServingPoint = 0x007B8CC0
  (engine/CTCTavernTable.h comment) right after this row -> 0x007B8B10 = GetEatingPoint.
- Caller CAIStateGroup_ServeFood::IsRunnable 0x00923070 (landed, rebuild/src/compiled/00/92/CAIStateGroupServeFood_IsRunnable_00923070.cpp:153)
  declares `bool GetEatingPoint(CThing& creature, C3DVector& pos, CRightHandedSet& rh, long& actionPoint, long& seat); // 0x007B8B10`.
  Second caller CAIStateGroup_EatAtTable::GetSeat 0x0096CB40 (call at 0x0096CCCE; xref.py).
- Mangled (Ego_r + FableWin both): `?GetEatingPoint@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z`
  = public non-virtual, non-const: `bool GetEatingPoint(CThing&, C3DVector&, CRightHandedSet&, long&, long&)`. ret 14h agrees.
  Param names: none in PDB tsvs; header sibling GetServingPoint uses (creature, pos, rh_set, action_point_index, seat_index).

## 3. Callees
| addr | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1) | same | OK |
| 0x007FF190 | 00/7f/CTCNamedUserActionPoints_GetFreeNamedActionPoint_007ff190.cpp | GetFreeNamedActionPoint(CThing&,C3DVector&,CRightHandedSet&,CCharString,long&,long start,bool checkBlocked) | ?GetFreeNamedActionPoint@CTCNamedUserActionPoints@@QAE_N...VCCharString@@AAJJ_N@Z | OK (but draft default args are invented; header has none) |
| 0x0099E4B0 | 00/99/CCharString_CCharString_0099e4b0.cpp | CCharString() | same | OK |
| 0x007FEFC0 | 00/7f/CTCNamedUserActionPoints_GetName_007fefc0.cpp | GetName(long, CCharString*) (landed uses POINTER) | ?GetName@CTCNamedUserActionPoints@@QAEXJAAVCCharString@@@Z = (long, CCharString&) | draft & is right; landed row's `*` is wrong -> NAME_FIX/signature note. Not declared in engine/CTCNamedUserActionPoints.h. |
| 0x0099EC70 | 00/99/CCharString_GetSubString_0099ec70.cpp | GetSubString(ulong start, ulong length) const | ?GetSubString@CCharString@@QBE?AV1@KK@Z | OK |
| 0x0099E7F0 | 00/99/global_GFCharStringToInt_0099e7f0.cpp | GFCharStringToInt(const CCharString&) __fastcall | ?GFCharStringToInt@@YIJABVCCharString@@@Z | OK |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | GetPItem() const | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (returns CBaseObject*) | draft returns CThing* (wrong type) |
| 0x007FEF40 | 00/7f/CTCUserActionPointBase_SetActionPointAsFree_007fef40.cpp | SetActionPointAsFree(long) | ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z | **WRONG: draft calls it CTCNamedUserActionPoints::FreeActionPoint** |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ~CCharString | OK |
| (0x0040F020) | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound | -- | NOT called here: expanded inline |
- Sibling name conflict noticed: 0x007B9090 landed as `CTCTavernTable::FreePoint` (descriptive) but engine/CTCTavernTable.h names it
  `FreeServingPoint` (?FreeServingPoint@CTCTavernTable@@QAEXJ@Z, PDB order). Not this row's callee; NAME_FIXES candidate.

## 4. Members (retail offsets; PDB +0x08 shift from 16-byte debug vectors; struct_layouts_egor.tsv CTCTavernTable, 116 PDB bytes)
- this+0x04 CTCBase::ParentThing (CThing*) -- engine/CTCBase.h.
- this+0x28 Food: vector<CIntelligentPointer<CThingObject> > (8-byte elements: vptr + PData) -- NOT in engine/CTCTavernTable.h
  (FABLE_RETAIL_CTCTAVERNTABLE_SEATS models only BeingServed +0x0C and Occupants +0x10; Tankards +0x1C, Food +0x28,
  SeatsCurrentlyUsed +0x34, NumPeopleSeated +0x40, SomeoneTalking/Laughing +0x44/45, ActionPointSitStrlen +0x48 (long),
  ...Maid +0x58, AvailableForUse +0x5C (bool) are unmodelled -> header extension needed (new opt-in) or local).
- this+0x48 ActionPointSitStrlen (long, PDB +0x5C); this+0x5C AvailableForUse (bool, PDB +0x70).
- CThing +0x20 UsingComponents (bitset), +0x44 Components (CVectorMap, start +0x44, finish +0x48) -- engine/CThing.h
  (offsetof asserts), engine/CVectorMap.h.
- CCharString PStringData +0 -> CCharStringData, length at +4 -- fable_string.h `GetLength()` = `PStringData==0 ? 0 : ...` (inline, matches).
- Opt-ins available: CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC(id,&ptc): HasTC + map pointer + LowerBound + cond-expr),
  FABLE_RETAIL_CTHING_HASTC_FORCEINLINE; CVectorMap.h FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND (inline non-const LowerBound,
  count=size() then left=begin()); fable_string.h FABLE_RETAIL_CCHARSTRING (+ _SUBSTRING for GetSubString);
  CTCNamedUserActionPoints.h FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS (needs CCHARSTRING, CTCBASE, CTHING, CINTELLIGENTPOINTER,
  CINTELLIGENTPOINTER_ASSIGN; declares CTCUserActionPointBase but NOT SetActionPointAsFree nor GetName -> declare locally or extend);
  CIntelligentPointer.h FABLE_RETAIL_CINTELLIGENTPOINTER (GetPItem const, CBaseObject*).
- Draft models LOCALLY (all have shared headers): CThing (engine/CThing.h), CCharString/CCharStringData (fable_string.h),
  CBaseIntelligentPointer/CIntelligentPointer (engine/CIntelligentPointer.h), CTCNamedUserActionPoints (engine/CTCNamedUserActionPoints.h),
  CTCTavernTable (engine/CTCTavernTable.h), interface map (engine/CVectorMap.h). GFCharStringToInt has no header decl (local decl ok).

## 5. Callee visibility
- LowerBound: same TU's FreeSeat (0x7B8AA0) and FreePoint (0x7B9090) CALL 0x0040F020 out of line, but this row expands it -> the
  inline decision differs per function in one TU: use FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND (as 14 landed rows do, e.g.
  00/79/CTCUnholyPower_FinalCleanup_00795090.cpp which also needed `// Flags: /d2inlT62` to keep the native first-call/final-inline split).
  Retail loads finish (edi) BEFORE start: matches the INLINE_LOWERBOUND `size()` spelling (finish-start) then `left=begin()`.
- GetFood 0x007B85C0 (same TU, earlier) and SetSeatState 0x007B8530 are visible; GetFood is not called -- either the explicit
  Food[seat].GetPItem() or an inlined GetFood whose test folds.
- No STLport out-of-line helpers called. CCharString ctor/dtor/GetSubString/GetName/GetFreeNamedActionPoint/SetActionPointAsFree are
  separate TUs (other address ranges).

## 6. Sibling landed rows
- 00/7b/CTCTavernTable_FreeSeat_007b8aa0.cpp (RELOCATION_MATCH): `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED`, engine/CThing.h,
  engine/CVectorMap.h; `CTCFreePointList* points; ParentThing->GetTC(TCI_NAMED_USER_ACTION_POINTS,&points);
  ((CTCUserActionPointBase*)points)->SetActionPointAsFree(point);` -- the uninitialised-points idiom; local CTCTavernTable model with
  CRetailVector<T> (_M_start/_M_finish) at +0x10/+0x1C/+0x28/+0x34, NumPeopleSeated +0x40. No pragmas/flags.
- 00/7b/CTCTavernTable_SetSeatState_007b8530.cpp (MATCH): no pragmas.
- 00/7b/CTCTavernTable_GetFood_007b85c0.cpp: `if (Food.size() > index) return (CThingObject*)((CBaseIntelligentPointer*)&Food[index])->GetPItem(); return 0;`
- 00/7b/CTCTavernTable_FreePoint_007b9090.cpp: local CThing with template GetInterface + engine/CVectorMap.h.
- 00/7b/CTCTavernTable_GetTankardName_007b8680.cpp: `#pragma auto_inline(off)` around a stand-in noinline body.
- Inline-LowerBound idiom donor: 00/79/CTCUnholyPower_FinalCleanup_00795090.cpp (`// Flags: /d2inlT62`,
  FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND + FABLE_RETAIL_CTHING ...); also 00/93/CAIStateGroupSeekScriptTarget_Update_0093c030.cpp
  and 00/77/CTCDRegionExit_HasFrameUpdate_00779960.cpp combine GETTC_EXPANDED + INLINE_LOWERBOUND.

## 7. Draft model errors vs retail
- Old residue (NOTES.md l.66, STATUS.tsv l.38): DIFFER 408v417 -- ebp/ebx web swap (retail: ebp=&pointIndex, ebx=startIndex),
  map start loaded into eax before the search pointer copy, missing `mov edi,edi` loop pad. The draft's hand-written GetTC (count
  from `last - m_first` with `last` local, `it` separate) is not retail's spelling; try the shared GetTC_EXPANDED + INLINE_LOWERBOUND
  (map pointer local, count=size(), left=begin(), cond-expr select) -- 4 of the 9 missing bytes are likely the pad/loads.
- Wrong callee name: FreeActionPoint -> CTCUserActionPointBase::SetActionPointAsFree (0x007FEF40).
- GetPItem returns CBaseObject* (const member), not CThing*.
- Invented default args on GetFreeNamedActionPoint; pass `0`-initialised startIndex and `true` explicitly.
- `startIndex = pointIndex + 1` is read AFTER SetActionPointAsFree (retail reloads [ebp]) -- draft order is right.
- The `seat` param name in the draft vs PDB-family spelling (seat_index / Seat in callers); `chair` local fine.
- `points` must stay uninitialised (retail homes it in pointIndex's slot); `if (HasTC) points = GetTC` matches GETTC_EXPANDED(id,&points).
- Local CCharString, CThing, CIntelligentPointer, CTCNamedUserActionPoints, CTCTavernTable models must move to shared headers
  (strict verifier); CTCTavernTable.h lacks Food/ActionPointSitStrlen/AvailableForUse -> needs a new opt-in block.

