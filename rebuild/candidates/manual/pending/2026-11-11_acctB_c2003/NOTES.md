# Hand chunk 2003 (NScript::CQ_WaspBossScript::CWaspChaseWoman .. NUISystem::CFrontEndManager): 3 of 4 byte-exact

All three matched rows grade RELOCATION_MATCH under VC7.1 RTM (q.py, with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2003/hdr`)
and pass the read-only verifier vland.py (RELOCATION_MATCH + behaviour PASS for all three, no RELOC_IDENTITY; five RELOC_NAME
warnings on catalog labels, listed below). Behaviour tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).
NOTE: g.sh / gall.sh need `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2003/hdr` first (0x0051F480 and 0x00598463 use
overlay opt-ins; without it they SRC_FAIL).

## What matched (3)
- **NScript::CQ_WaspBossScript::CWaspChaseWoman::Main 0x00E10E60** (1306 bytes, RELOCATION_MATCH, 14 grades over two passes).
  Wasp Menace chased woman: wait for StartChase, follow the ChasedWomanNav0..5 route ((nav + 1) % 6) while the WaspChaser
  lives, then thank the hero, morality for the first two villagers, run to VillagerEscapePos, FadeOutAndKillEntity. Built on
  the landed sibling CWaspVictim::Main's shared-header model. Shape lessons, in the order found:
  1. the chaser is `const CScriptThing& chaser = Game->GetThingWithScriptName(..)`: IsAlive dispatches through the vtable
     (0x00E11192 `call [edx+12Ch]`), a by-value local devirtualises and expands it;
  2. same-TU `__declspec(noinline)` CTimer ctor (`#ifdef _MSC_VER`, as CWaspVictim::Main): the timer no longer conflicts with
     every earlier temporary, so the "WaspChaser"/Nav3/Nav2 string temporaries share its word [esp+10h];
  3. the saved-by-hero code lives in the not-alive arm of the main loop (`if (!chaser.IsAlive()) { ...; break; }`): escape_pos
     becomes a block local and shares [esp+3Ch] with the route temporaries (frame 0x64 -> 0x58);
  4. main loop `for (;; Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return; ... }`, CTimer dtor plain inline:
     the entry and bottom terminate tests reach one expanded DeregisterTimer pad (0x00E111B3) that jumps to the shared tail;
  5. `if ((!task || IsDistanceBetweenThingsUnder(..)) && true)` on the route-update arm (Inline-budget line 3): halves that arm's
     frequency so its terminate return calls ~CTimer (0x00E110BE `jne 0x00E1134E`), as retail. c2 inline trace: ~CTimer c1 14,
     expand iff f > 0.032; arm return 0.044 -> 0.022, loop-top return 0.117.
- **NThingManager::CEntityRuntimePersistence::ClearPersistenceDataForSectionInEntityScript 0x0051F480** (1270 bytes,
  RELOCATION_MATCH, 2 grades). Cuts the named `XXXSectionStart <name>; ... XXXSectionEnd;` block out of the script text
  (CStringParser) and the matching {name, size, bytes} entry out of the zlib-compressed binary save data (CCompressorZlib,
  CMemoryDataInputStream, inline ReadULONG / ReadSLONG / ReadConstData, inline vector::erase), writes count - 1 back,
  updates UncompressedSize and recompresses. Shared headers throughout; the one lever: the new count is a named local
  (`unsigned long new_count = count - 1;`), which keeps count's register (retail `lea ecx,[ebp-1]`, not `dec ebp`).
- **NUISystem::CFrontEndManager::RefreshAvailableSavedGamesForProfile 0x00598463** (1335 bytes, RELOCATION_MATCH, 3 grades).
  Rebuilds the profile's saved-games screen (title TEXT_GUI_MENU_LOAD_GAME with the profile name, old UI_FRONTEND_LIST_FOR_SAVES
  marked UI_DELETE_THIS_INSTANCE, new list + text-area + screenshot viewports, one button per save at y += 30 skipping the
  empty slot, action UI_ACTION_TYPE_LOAD_GAME / UI_ACTION_TYPE_NEXT_SCREEN_INVALID_SAVE from the save's and its ".hs" header's
  signatures, NumberSaveGames counting, ConstructFileDescription per button). Size TU (`#pragma optimize("s", on)`), shared
  headers only (CFrontEndManager full-layout branch, NUISystem_CComponent, CText, CDeletion, CAction, CActionParamString,
  CUserProfileManager, CDiskFileWin32, CAFile, NGameText_CDataBank, CNewFrontendGameComponent). Levers:
  FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ (retail expands CWideString::operator== in place) and FABLE_RETAIL_STLP_MAP (visible
  map::operator[] body: the screen-key temporary shares [ebp+68h] with old_list and the file-name string, frame 0xD0 -> 0xCC).

## Parked for Fable 5.1 (0)
None.

## Skipped (1)
- 0x00D2988B (catalog CHookCoastTeleporter::Main, 1308 bytes): not a function start. The row begins 11 bytes inside the function
  at 0x00D29880 (`sub esp,38h ; mov edx,[0143E90Ch] ; push ebx ; push ebp` precede it; its epilogue pops ebx/ebp the row never
  pushes). True function 0x00D29880..0x00D29DA6 = 1319 bytes (SPLITS.tsv). Not attempted, per the brief.

## Names
Proven (two sources each, LABELS.tsv): the three row functions; callees IsDebugProfile 0x00407030, GetEmptySlotName 0x00406A80,
GetSaveDisplayNameFromFileName 0x00407E10, ConstructFileDescription 0x00595CC1, SkipPastString 0x009B9BA0, InternalCompress
0x00A3ADD0; every other callee uses its existing reloc_bindings.tsv / shared-header spelling (vland: no RELOC_IDENTITY).
Descriptive: none at linkage level (only local variable names: chaser, next_nav, nav, timer, escape_pos, section_start,
decompressed, entry_start/entry_end, new_count, save_names, title_text, header_name, save_valid / header_exists / header_valid).

## Label corrections (NAME_FIXES.tsv)
- 0x00A3ADD0 is CCompressorZlib::InternalCompress(const void*, unsigned long, CArray<unsigned char>&) -- landed 0x00523860
  binds it under the non-PDB class CCompressorZlibNative.
- 0x009B9BA0 is CStringParser::SkipPastString -- bindings also carry FindKeyword / NextItemIs (no such PDB members), and the
  landed file rebuild/src/compiled/00/9b/CStringParser_FindKeyword_009b9ba0.cpp holds an unrelated CThingCreatureBase body.
- 0x00407030 is CUserProfileManager::IsDebugProfile (catalog `_global::ReturnFalse_ret0_00407030`).
- 0x0059B5D7 / 0x00412130: the full STLport template spellings (4-arg map, 2-arg vector) emitted by the shared headers; the
  table also carries 2-arg / stand-in spellings.
- 0x0042BED4 / 0x0042CD84 are the CDeletion constructors (stand-in spellings SActionParamPair seen in landed local models).

vland RELOC_NAME warnings for 0x00598463 (labels, not identities): 0x0042ABCA our `??1?$_List_base<CComponent*>` vs label
`list<CComponent*>::~list` (the shared CDeletion/STLport headers emit _List_base; the landed 0x00545B10 binds the same; which one
retail's 0x0042ABCA is stays a header question); 0x00407030 (above); 0x0040F0A0 (our spelling = the existing binding, label is
the demangled form); 0x00429160 / 0x0042925C are ReleaseData<CAction> / <CText> (as the CCountedPointer header and the landed
0x0041E6D3 bind them), labelled ~CCountedPointer in the catalog.

## Header debt / overlay (hdr/README.md)
Five overlay headers, additions only, each behind a new opt-in: NScript_CQ_WaspBossScript.h (_CHASEWOMAN), 
NThingManager_CEntityRuntimePersistence.h (_CLEARINSCRIPT), CCompressorZlib.h (_COMPRESS_ARRAY), CFrontEndManager.h
(_SAVEDGAMES, in the _DESTRUCTOR branch), CUserProfileManager.h (_SAVENAMES; own pad, not combinable with the VOLUME /
TARGETINGSTATUS / SCOREBOARD layout blocks).
