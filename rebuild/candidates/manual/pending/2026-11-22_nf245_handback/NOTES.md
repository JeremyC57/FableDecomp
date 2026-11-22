# NF245 notes (2026-11-22, round 15b): the 0x004046B0 list family as one unit (entry 1); entries 2-3 not started (context)

## 1. Entry 1: 0x004046B0, nf243 ruling 2 (a): DONE as one unit, split 46 + 18 -> 3 + 61
**Header:** engine/CStringParser.h gets the add-only partial mode `FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY` (hdr/README.md):
line 183 `#else` -> `#elif !defined(..._ONLY)` (the one edited existing line) + a new final `#else` branch that declares only
CStringParserCommentBoundaries (token-identical to the full mode's class line). equiv_check over the 579 landed rows that reach the
header (7-header include closure, funclet include chains followed): 0 differ; a deliberate-change sanity run flags a CStringParser.h
user and not a non-user. pin_asserts: 0 pinned. None of the 11 rows that define the macro includes another header of the closure
(CPersistTraits.h, CPersistContext.h, CPersistContextReadSection.h, fable_persist_context.h, COpinionReactionManagerDef.h,
CReactionFrequencyTraitsArray.h), so the overlay grade equals the landed-tree grade (no repo header re-includes CStringParser.h there).
**Self row 0x004046B0:** onto the partial mode + stlport/fable_containers.h, `template std::_List_base<CSPCB, allocator<CSPCB> >::~_List_base();`
(the 0x0063F360 pattern); RELOCATION_MATCH; besides its leaf it defines only shared-header inlines (clear, deallocate, alloc_proxy /
allocator dtors). New test on the shared headers (populated + empty list, clear / destroy); the HEAD host-model test (a local
CStringParserCommentBoundaries) is replaced.
**Binders (43 changed, recipe per NF243, tools/xl3.py):**
- 33 dtor-only rows: the row-local `namespace std { allocator; list { ~list(); } }` block -> `FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE` +
  stlport/fable_containers.h (forward-declared element kept; 0x004047A0 also drops its local element class for the partial mode).
- 7 rows that build the parser in place (inline list ctor: 0x004C88E0, 0x004C8A30, 0x00686E10, 0x009ADE90, 0x009E5AD0, 0x00A38E50;
  and 0x009AF1E0 with its local _List_node models): `FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE` + the partial mode (complete
  element). Their pick-any list copies are all EQ to retail: _List_base ctor = 0x00404690, ~_List_base = 0x004046B0, list ctor =
  0x004046F0 (8 / 8 / 10 copies) and ~list = 0x00404710 (43 copies), pk.py with owners.tsv.
- 3 DTOR_OUTOFLINE rows (0x004C9120, 0x004CAA00, 0x00814260): NF241's swap -> `FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE`.
- Two needed a second form: **0x005DCA60** (its local list stand-in was 8 bytes, `m_pHead` + `m_size`, with ErrorCallback +0x1C and
  CursorPos[2]: on the 4-byte shared list the parser local shrank to 0x24 and graded DIFFER 123 v 123; form 2 corrects the local
  CStringParser model to the retail 0x28 layout, ErrorCallback +0x18, CursorPos[3] +0x1C: RELOCATION_MATCH). **0x009EC890** (its second
  `namespace std` block, a local vector<char> stand-in, collides with the shared vector: removed, as NF243's draft did; form 1 then
  emitted a NEW pick-any `vector<char>(size_type)` copy, 88 v 88 NEQ against owner 0x00411910; form 2 adds the existing
  `FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE`, so the call stays UND as at HEAD: RELOCATION_MATCH, no new copy). Its local `CArray<char>`
  model (engine/CArray.h exists) is HEAD debt, untouched; its `??0?$CArray@D@@QAE@I@Z` copy is NEQ at HEAD too.
- Every row's relocation diff HEAD v new (same-day compile of both, sc.sh): only the 0x004046B0 relocation renamed
  `??1?$list<CSPCB>` -> `??1?$_List_base<CSPCB>` (count unchanged). Two rows also show an anonymous-namespace hash change in a data
  symbol name (0x00814260 BOASTING_*, 0x009EB2C0 OPEN_BRACKET: `?A0x<hash>` depends on the file being compiled), same targets.
- Defined symbols: the HEAD UND `~list` becomes a pick-any DEF (5-byte jmp, EQ 0x00404710); the 7 inline-ctor rows trade HEAD's
  local `??0?$list<CSPCB>@QAE@XZ` (no retail owner) for the shared ctor / dtor copies (all EQ). The HEAD pick-any `??1CStringParser`
  copies in 42 rows were NEQ (their ~list relocation) and are EQ now.
- pk.py NEQ copies left: 0x004C9120 SerialiseThingInfo (299 v 330), 0x00520570 LoadHeroStateScript (reloc), 0x00521AE0
  CCharStringSet::_M_erase, 0x006BA8B0 / 0x009EC890 `CArray<char>` ctor: all NEQ at HEAD too (debt, nf203).
- Link (all 46 new objects): 2 LNK4006, both also at HEAD (HEAD objects linked the same way): SerialiseThingInfo 0x004C88E0 v
  0x004C9120, CPersistTraits<float>::TransferIn 0x00816000 v 0x00816150. 0 new duplicates.
**Catalog rows (nf243 ruling 3):** 0x00404690 off the made-up `std_list_node_Init` (a __fastcall free function) onto
`template std::_List_base<CSPCB>::_List_base(const allocator&)` (two sources: the list copy ctor 0x00463871 calls it as STLport's
`: _List_base(x.get_allocator())` and already binds this spelling; first of the COMDAT run): 2 spellings -> 1. 0x004046F0 off
`ListNode_Create_Circular` onto `template std::list<CSPCB>::list(const allocator&)`: no referencer, so no split either way, but only
ONE discriminating source (the COMDAT-run position: its bytes equal 0x00404690's), see decision 2. Both tests rewritten on the
shared headers (same PassPatterns). 0x00404710 (~list, the jmp thunk) is catalog-only: NAME_FIXES.
**Census:** 0x004046B0 has 91 retail call / jmp sites, 64 landed referencer rows (callers.py today = NF243's 64). After this
hand-back: 61 bind `~_List_base` (43 changed here, compiled today; 18 unchanged), 3 bind `~list` (0x00501D30 / 0x00756D00 /
0x007AE3D0, blocked: NF243 NOTES section 1); the self row spells `~_List_base`. The second recheck payload (recheck_final.log part 2)
compiles the 18 + 3 unchanged sources today: its RELOC_NAME lines give the same-day spelling of each.
**Locks:** the 65 family rows (NF245) + 0x00404690 / 0x004046F0 (claimed NF245). Release the 3 blocked rows or keep them for the
vector / map ports, as you rule.

## 2. Entries 2 and 3: not started (context; entry 1 is a full hand-back as the brief allows)
Starting census for entry 2 (refs_e2_census.tsv, callers.py today): 0x004AB4F0: 7 landed rows (0x004A3740, 0x00516F10, 0x00519560,
0x0051A2F0, 0x0051BF90, 0x005EB590, 0x006C5E90; tree / map rows among them, the blocker class of 0x00756D00); 0x005127A0: 13 rows
(heap / sort / vector templates of pair<unsigned __int64, list<C2DLineF> >); 0x00685BC0: 0x00682810 + 0x00986510 (port 0x00986510's
global-namespace `list` model first). Do each family's hardest rows first.

## 3. Decisions needed from the coordinator
1. Land entry 1 as one unit (46 rows + the CStringParser.h partial mode), leaving the 3 blocked binders on `~list` (ruling 2 (a)).
2. 0x004046F0: land `list(const allocator&)` as descriptive-by-layout on one discriminating source (no referencer, no split), or keep
   the old leaf and rename only the catalog entry later (the 0x004B8940 precedent was "undecided" on one source).
3. reloc_bindings: re-witness the `~list -> 004046b0` line to 0x00501D30 (TABLE_CHANGES); pk.py needs owners.tsv (or a catalog row for
   0x00404710) to judge the 43 new ~list copies.
4. File / Module renames: 00/40/stdlistCStringParserCommentBoundaries_list_004046b0.cpp (+ test) -> a `_List_base` name (Module
   std::_List_base<CStringParserCommentBoundaries>); 00/40/Global_StdListNodeInit_00404690.cpp (+ test), Module `_global` ->
   std::_List_base<CStringParserCommentBoundaries>; 00/40/Global_ListNodeCreateCircular_004046f0.cpp (+ test), Module `_global` ->
   std::list<CStringParserCommentBoundaries>. PassPatterns unchanged.

## Flags lines / funclets / tests / descriptive names
No row needed new flags (0x004138D0 /Gr and 0x00998A30 /Oa come from their catalog entries). No funclet includes a changed source.
Tests: new shared-header tests for 0x004046B0, 0x00404690, 0x004046F0 (PASS); the 43 binders keep their HEAD tests (PASS).
Descriptive names: the three STLport spellings (descriptive-by-library; 0x004046F0 also by layout).
selfcheck.py does not exist on this lane ($S/w22/nftools: absrefs, apply_rows, callers, mkpay, xref, xrow); the checks were done by hand.

## Recheck
recheck_final.log part 1: the 46 changed rows, 46 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT. Part 2: the 18 + 3 unchanged referencers
and 0x004046F0 (re-run after a comment-only evidence reword): 22 RELOCATION_MATCH PASS; one RELOC_CONFLICT, pre-existing and not
from this hand-back: unchanged 0x004A0D90 binds `??0?$vector@DV?$allocator@D@std@@@std@@QAE@I@Z` at 0x00410F30 (the CArray<char>
ctor's address) where 0x009EC890 binds it at 0x00411910 at HEAD and after (its relocation list is unchanged).
