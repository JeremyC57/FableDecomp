# NF208 notes (round 10, family pass FP-VECTORINSERT; handed back for context after the 0x00433530 part)

Counts (from STATUS.tsv / APPLIED.tsv): 4 entries: q6332 / q6341 (one unit) blocked: partial (8 binders moved, 4 left);
q6333 / q6342 (one unit) open (not started). Rows: 8 ported (0x008AE5E0 0x008AE810 0x008AEA40 0x006FC8E0 0x00CB5C90 0x00A72EE0
0x00A72F40 0x0044BFF0, all RELOCATION_MATCH + PASS), 2 not-a-binder (0x00433530 self, 0x004B2850), 12 blocked (4 left 0x00433530
binders, 8 0x00485DBC rows not reached). Final recheck: recheck_final.log.

## Decision items for the coordinator
1. **The brief's "declaration-only first" plan does not hold for the five QAE rows.** Measured on 0x008AE5E0: the in-place local model
   with `_M_insert_overflow` made protected and declaration-only gives DIFFER(92v115) (the __false_type tag gets its own `push ecx`
   word; retail homes it on the live by-value `value` slot). 0x00CB5C90's HEAD comment records the same for that row (esp+30h v
   esp+34h) and 0x006FC8E0's cites GOTCHAS b48. What matches: the rows on the shared headers with the EXISTING opt-ins
   FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE (real stl/_vector.c body visible, noinline) + FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
   (the self row 0x00433530's own macro). The forceinline opt-in is used here only so the pick-any IAE copy the visible body emits is
   the owner's: cmpbody against a fresh 0x00433530 object reads `bytes equal True, relocs equal True` (316 bytes) for all five. Measured on
   0x008AE5E0: with the forceinline macro removed the row's own body still grades RELOCATION_MATCH and only that COMDAT changes (280
   bytes, cmpbody bytes/relocs False against the owner), so the macro shapes nothing but the pick-any copy (work/nofi_008ae5e0.q.txt). Please confirm this use of an existing
   forceinline opt-in (no header gained a FABLE_FORCEINLINE).
2. **push_back pick-any vs the 0x0044BFF0 owner.** The ported rows define vector<CCharString>::push_back pick-any (the table maps it to
   0x0044BFF0). At HEAD the 0x0044BFF0 row was a free __fastcall stand-in over an overlay (binding ?InsertRange_00433530@VectorPushBackOverlay@@
   and ?CopyConstruct_0099ec30@CWideString@@), so no owner object defined the symbol. NF208 ported 0x0044BFF0 itself onto the header
   (self rename, RENAMES s1); its push_back COMDAT is cmpbody-equal (bytes + relocs) to the copies 0x008AE5E0 / 0x006FC8E0 / 0x00CB5C90 /
   0x00A72EE0 emit, and grades RELOCATION_MATCH at 0x0044BFF0. If 0x0044BFF0 does not land with this set, the copies in 0x00A72EE0 /
   0x00A72F40 (new symbols there; the trio / 0x006FC8E0 / 0x00CB5C90 objects already defined the symbol at HEAD from their local models)
   would be compared against a stand-in owner: land 0x0044BFF0 with them.
3. **0x0044BFF0 self rename: census done, no decision needed.** Fable.exe scan: 23 landed rows reference 0x0044BFF0
   (work/refs_44bff0.tsv). Each was compiled at HEAD (work/probe_44bff0.tsv): all 23 grade RELOCATION_MATCH and bind
   ?push_back@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXABVCCharString@@@Z, the new self leaf. No landed row binds
   the old leaf ?push_back@@YIXPAUVectorPushBackOverlay@@PAXPAVCWideString@@@Z (source grep). So the address goes from 2 spellings to 1.
   This is a whole-row port with a changed parameter list (a free __fastcall (overlay*, void*, CWideString*) becomes a thiscall member
   taking const CCharString&). Two sources for the signature: the body (CCharString copy constructor 0x0099EC30 under the placement-new
   test, then _M_insert_overflow 0x00433530 with (finish, x, tag, 1, true), thiscall `ret 4`) and the STLport 4.6 header spelling,
   which the 23 referencers and the reloc_bindings witness 0x00CBFACA already bind. The `CopyConstruct_0099ec30@CWideString` stand-in
   appeared only in this row (grep), so 0x0099EC30 only loses a spelling.
4. **The other new symbols have no owner.** None of the symbols the eight objects newly define (shared-header inlines: _Construct, the
   __false_type ctor, _M_clear / _M_set / size, allocator<CCharString> allocate / deallocate, the cdecl __uninitialized_* / _Destroy /
   __destroy* instances, max<unsigned>, ??_GCCharString, CBasicString<char>::GetLength, CVectorMap<CCharString,CScriptInfo>::Insert) has
   a reloc_bindings line or a landed `// Leaf:` line (grep of rebuild/src/compiled). The only ones with table owners are the IAE overflow
   (0x00433530, cmpbody equal) and push_back (0x0044BFF0, cmpbody equal to the new owner) and the <new> placement operator
   ??2@YAPAXIPAX@Z (0x00405C10, new only in 0x0044BFF0's object; 5 bytes, cmpbody equal to the copies in the HEAD 0x008AE5E0 / 0x00A72EE0
   objects and the 0x00433530 owner object).

## Per row
- 0x008AE5E0 / 0x008AE810 / 0x008AEA40 (CTCAIScratchpad::AddGossip*): the local CCharString / vector / map / CTCAIScratchpad models are
  replaced by the shared headers (GOSSIP_MAPS for the static maps; three new declaration-only opt-ins for the members). The map's
  operator[] stays the out-of-line 0x007AEBE0 declaration (fable_map.h without FABLE_RETAIL_STLP_MAP). Scripted from 0x008AE5E0.
- 0x006FC8E0 (std::for_each<const NAnimationEvents::CEvent*, CFillEventNames>): onto fable_string.h, engine/CSymbolMap.h (new opt-in for
  the one-argument FindFirstSymbolWithVal), stlport/fable_containers.h, stlport/fable_algorithm.h (FOR_EACH_BODY). The header-less
  NAnimationEvents::CEvent / CFillEventNames stay local with their HEAD shape (grep of `class|struct CEvent|CFillEventNames` over all
  rebuild/include finds only libpk/CEventScript.h's forward declaration of the unrelated global libpk `struct CEvent`).
  Leaf line completed from the bare `??$for_each@` prefix to the full mangled symbol (same symbol; not a rename).
- 0x00CB5C90 (CScriptInfoManager::AddScript): onto engine/CScriptInfoManager.h (+ CVectorMap.h, CScriptInfo.h), engine/CTBaseSingleton.h
  (GET_DECL), fable_string.h, stlport/fable_containers.h. The row-local stand-in CVectorMap::Add becomes Ego_r.pdb CVectorMap::Insert(key,
  value) (existing INSERT_KEYVALUE + INSERT_VALUE_TYPE, forced inline there); inline, no address, so not in RENAMES.
  The header-less CCodeSectionManager stays local (no header anywhere). vector<pair<CCharString,CScriptInfo> >::push_back keeps its
  explicit-specialization declaration (out of line, 0x00CB7210, as at HEAD). Test: Dirty is private in CVectorMap.h, so the rewritten
  test reads it with memcpy at the manager's retail offset +0x11 (Scripts +4, Dirty +0x0D), as the HEAD test checked it.
- 0x00A72EE0 / 0x00A72F40 (stand-in ForEach_Add*Name rows): only the CCharString / __false_type / CCharStringVector stand-ins move onto
  the headers (overflow declaration-only, as at HEAD); the record structs SHelperPoint / SDummyObject and the CAddName functor stay (header-
  less stand-ins; the self leaves are stand-ins too, unchanged; their caller rows 0x00A6D050 / 0x00A6D090 bind them). Leaf lines completed to
  the mangled self symbols (same symbols).
- 0x0044BFF0: see decision items 2 and 3.

## Left for the next pass (q6332 / q6341)
- 0x004333B3 (vector<CCharString>::_M_fill_insert, a one-parameter std::vector model binding ?_M_insert_overflow@?$vector@VCCharString@@@std@@QAE..):
  a self rename of its own (Leaf cut at `?_M_fill_insert@?$vector@VCCharString@@`), needs its census first.
- 0x004B2FE0 (CQuestManager::OnQuestStartScreenDisplayed, CStringVector stand-in), 0x004B4A10 (CQuestManager::ActivateQuest,
  CCharStringVector), 0x00A2ED10 (C3DMeshStats::UnpackNames, CCharStringVector with `K_N`): whole-row stand-in ports. The 0x00A72EE0
  recipe (fable_string.h + the STLport vector, overflow declaration-only when the HEAD row was) should carry where the rows only need
  the vector; 0x004B2FE0 / 0x004B4A10 model CQuestManager locally (header debt noted in 0x004B2850).
- q6333 / q6342 (0x00485DBC): see APPLIED.

## Descriptive names
- None new. 0x0044BFF0 takes the STLport library spelling (descriptive-by-library: STL instance, no PDB holds it).

## Flags lines
- None added or changed (all eight rows grade with their catalog flags: none).

## Funclets
- None of the eight changed rows is included by an EH funclet row (grep of rebuild/src/compiled/01/21 for their file names: none). The
  0x0044BFF0 self rename keeps its file name in this hand-back (rows/0044bff0.cpp); the landing names the file.

## Locks
- Claimed this pass (plain mkdir, owner NF208): 0x0044BFF0, 0x00A2ED10, 0x00A72EE0, 0x00A72F40, 0x006116E0 (added to rows.txt).

## Rule slips
- One test grade went through vland twice before passing (0x00CB5C90: TCC_FAIL on the private Dirty, then LINK_FAIL on
  CCharString::operator=); counted in grades_used. No other slip known.
