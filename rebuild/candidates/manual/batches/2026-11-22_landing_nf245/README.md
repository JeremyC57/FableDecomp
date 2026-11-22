# Landing nf245 (Account A, 2026-11-22): the 0x004046B0 list<CStringParserCommentBoundaries> family as one unit (46 rows)

Agent NF245, entry 1 of its brief (nf243 ruling 2 (a)); entries 2-3 handed on. Hand-back: `pending/2026-11-22_nf245_handback/`.
- **Header:** engine/CStringParser.h gains the add-only partial mode `FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY` (one existing
  line, `#else` -> `#elif !defined(..._ONLY)`, same branch with the macro undefined; a new last branch declares only
  CStringParserCommentBoundaries, token-identical to the full mode's line). equiv_check over the 579 landed rows reaching the header:
  0 differ (sanity run with a deliberate change flags a user); pin_asserts 0. Only this batch's 11 rows define the macro.
- **Self row 0x004046B0:** `std::list<..>::~list` -> `std::_List_base<CStringParserCommentBoundaries>::~_List_base` (62-byte clear + free
  body; `~list` is the 5-byte `jmp` thunk 0x00404710, catalog-only, queued), RELOCATION_MATCH, test rewritten on the shared headers.
- **43 binders** respelled `??1?$list<CSPCB>` -> `??1?$_List_base<CSPCB>` (33 dtor-only rows: BASE_OUTOFLINE + stlport/fable_containers.h;
  7 inline-parser rows: BASE_DTOR_VISIBLE_NOINLINE + the partial mode; 0x004C9120 / 0x004CAA00 / 0x00814260: NF241's swap). 0x005DCA60
  also gets its local CStringParser model's retail 0x28 offsets; 0x009EC890 moves its local vector onto the shared one
  (VECTOR_N_OUTOFLINE keeps 0x00411910 a call, as at HEAD). Per row the HEAD v new relocation diff renames only the 0x004046B0
  relocation; pk.py: every new list copy EQ its retail address (0x00404690 / 0x004046B0 / 0x004046F0, the 43 `~list` copies EQ
  0x00404710; pk.py needs the hand-back's owners.tsv for that, see below); link check: 2 LNK4006, both at HEAD. Split 46 + 18 -> 3 + 61.
- **0x00404690** `_global::std_list_node_Init` (made-up) -> `std::_List_base<CSPCB>::_List_base(const allocator&)` (its caller 0x00463871
  already bound that spelling: 2 -> 1). **0x004046F0** `_global::ListNode_Create_Circular` (made-up) -> `std::list<CSPCB>::list(const
  allocator&)`. Files and Modules respelled for all three self rows; PassPatterns unchanged.
- **Recheck** (`logs/recheck.log`, landed tree): the 46 changed rows 46 RELOCATION_MATCH PASS; the 3 blocked `~list` binders, 0x00463871 and
  0x00998570 (unchanged `_List_base` users) 5 RELOCATION_MATCH PASS; 0 RELOC_CONFLICT.

## Coordinator rulings on NF245's questions
1. Land entry 1 as one unit: yes; 0x00501D30 / 0x00756D00 / 0x007AE3D0 stay on `~list` (ruling 2 (a)); their locks are released.
2. 0x004046F0: landed as **descriptive-by-layout** (its bytes equal 0x00404690's, so only the COMDAT-run position tells the ctors apart:
   _List_base ctor, _List_base dtor, list ctor, ~list thunk, the same run nf241 proved at 0x0042ABAF..). Its old name was made-up, it has
   no referencer, so no split risk; the STLport library spelling is closer to retail than the stand-in. Marked so in NAME_FIXES_QUEUE.
3. reloc_bindings: `??1?$list<CSPCB> -> 004046b0` re-witnessed 0x00998A30 -> 0x00501D30 (the three blocked rows still bind it). pk.py
   gap queued: a pk.py owner map that knows 0x00404710 (the hand-back's owners.tsv) or a catalog row for it; until then pk.py reports the
   43 new 5-byte `~list` copies NEQ against the 62-byte body (a tooling artifact, recorded here).
4. File / Module renames: done (renamer, make_base spelling).
5. Locks: the 3 blocked rows released; the rest released with the landing.
