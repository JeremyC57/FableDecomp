## 0x004B4260

# DOSSIER 0x004B4260 CQuestManager::ActivateMultipleQuests (359 bytes) -- identity scout, wf6

## 0. Headline
* Row already RELOCATION_MATCHed in wf1 (S/w22/wf1/004b4260/ATTEMPTS.log 11:48, 12:26) but was held back only because the
  same-TU body of the private ActivateQuests 0x004B3CE0 was unproven. 0x004B3CE0 is now LANDED:
  rebuild/src/compiled/00/4b/CQuestManager_ActivateQuests_004b3ce0.cpp. Job = swap the draft's ActivateQuests body for the landed
  one (plus everything that file carries ahead of it), regrade. Test: S/w22/wf1/004b4260/004b4260_test.cpp.
* Repo headers now carry both rows' opt-ins (engine/CQuestManager.h FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS and
  _ACTIVATEQUESTS; engine/CScriptInfo.h _ACCESSORS / _FASTCALL_ALLOC). Start with an EMPTY hdr/ overlay.

## 1. Retail shape (xdis 004b4260 0x177)
* __thiscall (ecx=this -> edi), 3 stack args, `ret 0Ch`, bool in al (`mov al,bl`).
* NO frame pointer (`sub esp,2Ch ; push ebx ; push ebp ; push esi ; push edi`; ebp = quest_names), NO SEH -> no EH flags, speed TU
  (alignment `lea esp,[esp]` / `lea esp,[esp+0]` fillers at 0x4B429C, 0x4B4399). No `// Flags:` needed; matches ActivateQuests ("No flags").
* Register plan: bl=activated, edi=this (also spilled [esp+14h] and reloaded after the arm), ebp=&quest_names, index i in frame [esp+10h],
  local CArray<CActivateQuestInfo> at [esp+18h..20h]. size() recomputed each iteration ((finish-start)>>2, unsigned `jbe`/`jb`).
* Loop: guarded do-while form (`test eax,eax ; jbe end` then body) = plain `for (unsigned i=0; i<quest_names.size(); ++i)`.
* Element addr `lea esi,[ecx+edx*4]` (start + i*4).
* Progress text CCharString built at [esp+40h] (= dead quest_names arg slot homing) BEFORE pushing DisplayProgress's args -> named
  block-scoped local; DisplayProgress(text, -1.0f /*0xBF800000*/, false /*dl*/, false /*esi=0 pushed*/), then ~CCharString.
* Two arms (info != 0 / == 0) each build a 12-byte CActivateQuestInfo temporary (copy ctor of name, PScriptInfo, GameScript=byte arg2
  [esp+44h], LoadResources=byte arg3 [esp+48h]) -> push_back 0x004BB720 (OUT OF LINE call) -> ~CCharString shared tail, then `mov bl,1`
  (store merged: write `activated = true` in both arms, wf1 LESSONS).
* After loop: ActivateQuests(quests) 0x004B3CE0; inline ~vector (~CCharString loop stride 0xC, then free 0x00BFEA14 if start != 0).

## 2. Identity (two sources -- label is RIGHT)
* Leaf: ?ActivateMultipleQuests@CQuestManager@@QAE_NABV?$CArray@VCCharString@@@@_N1@Z
  = public, non-const, bool (const CArray<CCharString>&, bool, bool).
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r addr 0x004B9B70), ghidra_out/fablewin_pdb_names.tsv (FableWin 0x01C49AF0),
  egor_signatures.tsv, naming_stragglers/FableWin.exe.signatures.tsv. (egor_pdb_names' own row at "004b4260" is an Ego_r-address
  _Tree::erase -- different address space, ignore.)
* Retail binding: xref = 6 callers: 0x0049EAD1, 0x0049F24E (CWorld::ActivateWorld 0x0049F180 region), 0x004B4A5A (ActivateQuest
  0x004B4A10), 0x004B5B84 (LoadGameState 0x004B5500), 0x00892EAF / 0x00892EEF = PDB-named CGameScriptInterface::ActivateMultipleQuests
  0x00892EA0 (pushes 1,1, ecx = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass 0x013B89FC) and
  ...WithoutLoadingResources 0x00892EE0 (pushes 0,1 -> arg3=false). Declared in engine/CQuestManager.h line ~320 as
  `bool ActivateMultipleQuests(const CArray<CCharString>& quest_names, bool game_script, bool load_resources);` (names descriptive).
* No vtable slot (non-virtual).

## 3. Callees
| addr | landed row | landed / PDB name | draft OK? |
|---|---|---|---|
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long) ("QuestManager: Activate Quest" @0x01239440, -1) | ok |
| 0x009E9F40 | 00/9e/NProgressDisplay_DisplayProgress_009e9f40.cpp | ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z (__fastcall) | ok |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
| 0x004B00C0 | 00/4b/CQuestManager_IsQuestRegistered_004b00c0.cpp | ?IsQuestRegistered@CQuestManager@@QBE_NABVCCharString@@@Z (const) | ok |
| 0x00CB5AD0 | 00/cb/CScriptInfoManager_GetPScriptInfo_00cb5ad0.cpp | ?GetPScriptInfo@CScriptInfoManager@@QAEPAVCScriptInfo@@ABVCCharString@@@Z (non-const, returns CScriptInfo*) | draft stores into `const CScriptInfo*` -- harmless, PDB ctor takes `const CScriptInfo*` |
| 0x0099EC30 | 00/99/CCharString_CCharStringcopy_0099ec30.cpp | CCharString copy ctor (inline CActivateQuestInfo ctor) | ok |
| 0x004BB720 | 00/4b/stdvector_pushback_004bb720.cpp | vector<CQuestManager::CActivateQuestInfo>::push_back (out-of-line COMDAT; grows via _M_insert_overflow 0x004BB2E0, landed 00/4b/stdvector_Minsertoverflow_004bb2e0.cpp) | see risk 7a |
| 0x004B3CE0 | 00/4b/CQuestManager_ActivateQuests_004b3ce0.cpp | ?ActivateQuests@CQuestManager@@AAEXABV?$CArray@VCActivateQuestInfo@CQuestManager@@@@@Z (private) | draft body WRONG/old: replace |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free (inline ~vector deallocate) | ok |
CScriptInfoManager has no shared header: the draft's local `class CScriptInfoManager { CScriptInfo* GetPScriptInfo(const CCharString&); }`
is a declaration-only stub (allowed; header CQuestManager.h only forward-declares it, line 515).

## 4. Members
* this+0x78 ScriptInfoManager (CScriptInfoManager*): engine/CQuestManager.h line 421/538 under FABLE_RETAIL_CQUESTMANAGER(+_ACTIVATEMULTIPLEQUESTS)
  ("ActivateMultipleQuests 0x004B42E4 `mov ecx,[edi+78h]`"). Ego_r layout (struct_layouts_egor.tsv) has it at 212 -- PDB name
  ScriptInfoManager, retail offset differs (smaller STLport containers).
* CQuestManager::CActivateQuestInfo (nested, 12 bytes): QuestName +0, PScriptInfo +4, GameScript +8, LoadResources +9; inline ctor
  (const CCharString&, const CScriptInfo*, bool, bool) -- header opt-in FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS.
* CArray<CCharString> start/finish (+0/+4) via engine/CArray.h.
* No locally modelled class with a shared header in the draft except CCodeSectionManager (no shared header; same local decl as landed
  ActivateQuests) -- keep exactly the landed file's local declarations.

## 5. Visibility (the deciding fact)
* ActivateQuests 0x004B3CE0 < 0x004B4260: same retail .cpp, defined earlier. With its body visible VC7.1 knows `quests`' address
  is not kept -> bl/edi/ebp plan. Declaration only = 349 v 359 (wf1 README). Copy the LANDED body verbatim, together with what the
  landed file places ahead of it: the noinline real bodies of CBasicString<char>::operator< 0x00429950 and
  CQuestManager::LoadResourcesForQuests 0x004B2510 (both in `#pragma auto_inline(off)` blocks), the local CCodeSectionManager /
  CTCDayOrNightOnlySupport / g_QuestScriptsEnabled declarations, and its include list (+ engine/NProgressDisplay.h for this row).
* push_back<CActivateQuestInfo> 0x004BB720 is a COMDAT called out of line here.
* Opt-in macros the landed ActivateQuests uses (must all be defined in the combined TU): FABLE_RETAIL_CARRAY, _CCHARSTRING,
  _CCHARSTRING_CONCAT_STRING, _CCHARSTRING_LESS_INLINE, _CWORLD, _NSCRIPT, _CWORLD_GAMESCRIPTINTERFACE, _CWORLD_MESSAGEEVENTMANAGER,
  _STLP_VECTOR, _STLP_SET, _STLP_TREE_CONSTFIND, _STLP_TREE_BOUNDS_OUTOFLINE, _STLP_LIST_SIZE_DISTANCE, _CSCRIPTINFO_ACCESSORS,
  _CQUESTMANAGER, _LOADGAMESTATE, _ACTIVEQUEST, _SAVEGAMESTATE, _ACTIVATEMULTIPLEQUESTS, _ACTIVATEQUESTS, _CWORLDMAP,
  _CWORLDMAP_LOADTHINGSWITHSECTIONNAMES, _CWORLDMAP_UPDATENAVMAPS.
* Draft macros to DROP/REVIEW: FABLE_RETAIL_CSCRIPTINFO_FASTCALL_ALLOC (superseded by _ACCESSORS; they are #ifdef/#else exclusive
  in CScriptInfo.h, ACCESSORS wins); FABLE_RETAIL_STLP_TREE_ERASE / _TREE_ERASE_KEY (landed ActivateQuests doesn't use them -- it uses
  TREE_BOUNDS_OUTOFLINE); FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (see 7a).

## 6. Siblings
* 00/4b/CQuestManager_ActivateQuests_004b3ce0.cpp -- the donor; no flags, auto_inline(off/on) brackets for same-TU bodies.
* 00/4b/CQuestManager_LoadResourcesForQuests_004b2510.cpp, CQuestManager_DeactivateQuest_004b43d0.cpp (next function, 0x004B43D0),
  CQuestManager_DeactivateAllQuests_004b4450.cpp, CQuestManager_Update_004b4490.cpp -- all speed TU, no `// Flags:`.
* 00/4b/CQuestManager_ActivateQuest_004b4a10.cpp -- caller; old local-struct style (do NOT copy its idioms; it carries a stand-in
  ActivateMultipleQuests body).
* 00/89/CGameScriptInterface_ActivateMultipleQuests_00892ea0.cpp (caller, uses private CQuestMgr stand-in -- a NAME_FIXES candidate:
  it should call CQuestManager::ActivateMultipleQuests(quests, true, true)).

## 7. Old draft model errors / risks
a. FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (fable_containers.h:466) marks EVERY vector::push_back noinline; the landed ActivateQuests
   EXPANDS push_back<CCharString> (calls _M_insert_overflow 0x00433530 directly, copy ctor at 0x004B3D69). Combined TU: if the
   macro is kept the noinline push_back<CCharString> would change ActivateQuests' body (only matters via visibility/escape info, since
   ActivateQuests isn't graded here -- but it may alter what c2 concludes). First try WITHOUT the macro (does VC7.1 call
   push_back<CActivateQuestInfo> out of line by itself? two call sites + larger element); if it inlines, re-add the macro and check.
b. The draft's ActivateQuests body is not the landed one (for-loops, direct PScriptInfo->MasterScript / AllocFunc, CurrentQuestID++,
   GetMessageEventManager()) -- won't compile against _ACCESSORS; replace wholesale.
c. FABLE_RETAIL_CARRAY (needed by ActivateQuests) changes CArray::operator[] to *(begin()+i) for this row too; wf1 matched without it.
   Watch the `lea esi,[ecx+edx*4]` at 0x004B42D1; should be same for 4-byte elements but verify.
d. Keep: named block-scoped progress CCharString; `activated = true` in both arms; plain for loop; local CArray<CActivateQuestInfo>.

