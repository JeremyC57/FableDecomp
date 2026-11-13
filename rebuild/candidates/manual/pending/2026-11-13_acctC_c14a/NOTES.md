# agentC14 (acctC lane) -- NOTES

## Row 1 0x0060C900 CTCInventoryMap::ConstructMapList -- RELOCATION_MATCH 9,635, test PASS (vt.py VC7.1 + g++), vland 1 win
- Name: NOT BuildMapTeleportUI (forge-autoname2 label, not a PDB name). Ego_r.pdb 0x0065F950 / FableWin.pdb 0x0208A2A0
  ?ConstructMapList@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@_NPA_N@Z; donor order ConstructMapList / ConstructPCMapList
  = retail 0x0060C900 / 0x0060EEB0; `ret 0Ch` (3 args); returns the list (eax = ebx). The first argument is never read.
- Extent: 0x0060C900..0x0060EEA3 (`ret 0Ch` at 0x0060EEA0), no jump table; oracle as built (9,635).
- How: written from the matched sibling ConstructPCMapList (agentC4 src/0060eeb0.cpp, same TU): the head / gather / guild-recall
  swap are identical; the console list differs after the zoom setup (UI_* component names, ring centre instead of the PC
  conversion, zoom stick / mini-map viewport / text-area viewport in the list, name text component, descriptions handed to the
  text-area viewport, UI_BACK_FOR_LIST, map viewport, select / scroll texts and the opaque unselect state at the end).
  Call outline from tools/outl.py (retail call + pushed strings), diff against the PC outline.
  Grade 1 DIFFER(9630v9635) 13 aligned diffs; grade 2 RELOCATION_MATCH after two retail facts:
  1. teleporter action: `const CWorldMap& teleporter_world_map = PeekWorldMap();` before the push_back (retail calls
     PeekWorldMap 0x00686D20 BEFORE reading teleporter->GetCurrentMapNumber, 0x0060E563).
  2. tail: each FindChild result in its own named local (assign_text / unselect_text / unselect) -- retail calls FindChild before
     building the text-symbol CCharString (0x0060EB95 / 0x0060EBF6) and stores the operator[] key after FindChild (0x0060EC63).
- Same FITTED `if (regions.empty())` guard as the sibling (folds to no code; without it DIFFER 9795v9635) and the same same-TU
  CreateMapMarkers body (without it DIFFER 9633v9635). PAIR: when row 3's CreateMapMarkers text changes, regrade rows 0x0060C900 and
  0x0060EEB0.
- Header: NEW opt-in FABLE_RETAIL_CTCINVENTORYMAP_MAPLIST (engine/CTCInventoryMap.h, declaration only, after the PCMAPLIST block;
  every assert of the file was already pinned with FABLE_STATIC_ASSERT_AT, pin_asserts.py: 0 to pin). No flags.
- Test: test/0060c900.cpp built by tools/mktest_c900.py (tmp/t1/prefix.cpp + the row's GetGuildSealRecallRegion + ConstructMapList
  text verbatim + tmp/t1/suffix.cpp); mutants (close action unconditional, no opaque unselect, no sprite type test) FAIL.
- Descriptive local names (zoom_stick, mini_map_viewport, text_viewport, name_text, text_area, back, assign_text, unselect_text,
  unselect, map_viewport): no PDB source for locals.
- vland WARNs are catalogue labels only (GetRegionCount, GetTeleporterRegionName, GetDef, DELTA_NOTIONAL_ZERO, DeleteData<CAction>).

## Row 2 0x00603000 CTCInventoryMap::CreateMiniMapMarkers -- IN PROGRESS
- Name proven (Ego_r 0x00652B10 / FableWin 0x02093750; header already declares it). Extent 9,571 (`ret 8` last, no table).
- Port: probe/r2/p1.cpp = row-1 defines/includes (map TU) + the draft's extra defines + draft body (ref/00603000/draft.cpp);
  the draft's row-file template bodies (_Rb_tree::find const, CVectorMap::LowerBound) replaced by the shared opt-ins
  FABLE_RETAIL_STLP_TREE_CONSTFIND / FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND / _INLINE_CONST_LOWERBOUND.
- New header opt-ins (hdr/README.md): CThingFilter_HasInformation.h (new header, empty filter, op() on CThingAICreature per both
  PDBs), FABLE_RETAIL_CDEFSTRING_INLINE_EQ, FABLE_RETAIL_CPLAYERGUI_GETMINIMAP, FABLE_RETAIL_CTHINGCREATUREBASE_ISDEMONDOOR,
  FABLE_RETAIL_CMINIMAPDISPLAY_GETNONAUTOMARKERS (all first found by chunk 729, re-applied to today's headers; asserts pinned).
- p2: + STLP_FIND_RANDOM_DISPATCH (inline find -> fastcall __find 0x00612DB0 as retail), CTHINGFILTER_ENEMYCREATURE_CTOR,
  CCOUNTEDPOINTER_RESET_ELSE_NULL (Reset called at UI_INTERESTING_MARKER 0x00603838 as retail), CVECTORMAP_INLINE_CONST_LOWERBOUND
  -> call sequence = retail except the return-path cleanup merge (a register consequence). DIFFER(9675v9571).
- p3 (= v/b2): + FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY + _ALLTHINGSBODY (+ CTHINGAICREATURE_STATTYPEFLAGS/_STATGETTYPE): the
  same-TU GetAllLeafThings / GetAllThings bodies (retail instances 0x00617780..0x00618790 follow this TU) -> VC7.1 hoists the
  CFilter_Bind2 pointer stores above each per-map loop as retail (0x00603928..0x0060393B). DIFFER(9682v9571), D 1760 DR 1173.
- Residue: register allocation from the head: retail this=ebx (spill [esp+38h]), region=edi in the head, manager in memory
  [esp+60h], courtyard1/2 iterators in memory; ours this=edi, region re-read from [ebp+0Ch], courtyard1 in ebx.
  Head-order variants (flags first, marker first, hero first, casts) do not move it (v/v1.py).
- BEST: differ/00603000.cpp (= probe/r2/v/e1.cpp: p3 without the neutral CWORLD_TYPED_MEMBERS) DIFFER(9682v9571), D 1760 DR 1173.
- Tried without effect (grades in ATTEMPTS.log; batch files probe/r2/v/v1..v4.py, variants alongside):
  head order (flags before the gui-def ref, marker before manager, hero first, no casts), compare operand order, a named
  regionName ref, hero-region test order (v3); the preceding same-TU bodies of row 1 (CreateMapMarkers / GetDef / GetGuildSeal,
  c1/c2: identical); HASTC_OUTOFLINE (b3 = b2); GETTC_ORIGINAL_CHAIN off (= same). Worse: LEAFBODY alone (2761), visible noinline
  __find body (e3 2098), default Reset text (e2 2726, Reset decisions change), separate per-loop counters (f1: 9654 bytes, closer in
  size, D 2005).
- Analysis tools: tools/cc.py + alias.tsv (call targets by address; = retail), tools/seg2.py (segment sizes between marker-string
  anchors ours v retail), tools/drd.py (register/frame-masked diff hunks), tools/ostr.py (our string literals in order),
  c2prio traces tmp/prio_p2.txt / prio_p3t.txt (`this` split esi/edi/ebx pieces, region one ebx piece + memory, manager memory).
- Also no effect: DIAG dead-local ruler 2/5/8/12 (v5: dead bools vanish), a local `CRegion& r` / named graphic-name ref in the head (v6 h1/h3), unnamed find (h2 worse).
- Next for a successor: the first wrong decision is the head colouring (retail: region piece edi 0x00603031..0x006031E1 then
  `add edi,18h` = &region.Name; this = ebx from 0x0060300E, spilled once to [esp+38h]); find in the c2 trace which piece of `this`
  takes edi in the head and what outranks region's head piece (their K / w per block); then the frame slots (retail marker
  [esp+10h], flags [esp+18h], CCharString temps at [esp+24h], hero [esp+88h]; ours temps at [esp+58h], hero [esp+64h]).
  Structure, call targets and string order already equal retail.

## Header overlay check (agentC14)
- equiv_check.py over every landed row (sources + tests), old = rebuild/include, new = rebuild/include + this hdr/: 0 differing
  rows, after pinning the 3 asserts the inherited agentC11 CExpressionSubDef.h block had moved (0x004DE881 differed before).
- Rows regraded after every overlay change: 0x0060C900 RELOCATION_MATCH; sibling 0x0060EEB0 (agentC4 src) RELOCATION_MATCH.

## For the coordinator
- 0x0060C900 is ConstructMapList, not BuildMapTeleportUI (NAME_FIXES.tsv; reloc_bindings carries two wrong BuildMapTeleportUI
  manglings for it). Land with FABLE_RETAIL_CTCINVENTORYMAP_MAPLIST (new) + the same opt-in set as 0x0060EEB0.
- Same FITTED `if (regions.empty())` guard and same-TU CreateMapMarkers body as 0x0060EEB0 (both needed; see row 1 notes).
- 0x006A7140 operator() of CThingFilter_HasInformation: both PDBs take `const CThingAICreature&`; the landed local model in
  0x00617A00 spells `const CThing&` (NAME_FIXES.tsv).
- Row 3 0x005FF190 not opened (brief: one row at a time; row 2 still DIFFER).
