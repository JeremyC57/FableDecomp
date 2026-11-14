# agentC33 NOTES

## Row 1: 0x0058578B CTCInventory::OpenInventory (8,081 bytes) -- DIFFER(8070v8081), parked after 12 grades (differ/0058578b.cpp)
Name proven by C23 (Ego_r 0x005F4D20 + FableWin ?OpenInventory@CTCInventory@@UAEXXZ, vtable 0x01250DE4 slot 30).
Extent confirmed: first `push ebp / lea ebp,[esp-78h]`, last `leave ; ret`.
What moved it (all EVIDENCED by retail bytes):
- a1: the vignette test is `IsCore() || (IsOptional() && !GetPDef().IsNull() && !GetPDef()->IsVignette)` in BOTH loops: retail builds TWO
  CDefPointer temporaries per test (0x00585D46 `mov esi,[ebx+0Ch] ... je` = IsNull on temp 1 (flag bit 1 / 4), 0x00585D57 temp 2 (flag 2 / 8),
  `cmp byte [ecx+59h],0`). 7948 -> 8077 bytes, frame 0xE8 -> 0xF4 = retail.
- a2: `GetParentThing().GetCurrentMapNumber()` (both mini-map statements): retail 0x00585B9E loads ParentThing before the sprite vtable.
- a3: `bool has_quests = false; for (...) { ... if (test) { has_quests = true; break; } } if (!has_quests) {text} else {reverse loop}` instead of
  the draft's goto pair: same flow as retail (0x00585DBC `test bl,bl ; jne 0x585ED9`), 8070 bytes, 331 hunks (goto variant a2 = 8077 bytes, 336
  hunks, kept as differ/0058578b_goto_variant.cpp). Not decided by bytes which of the two retail had.
Dead ends (measured): `#pragma optimize("y", off)` 8305 bytes (worse; the TU is plain optimize("s",on)); a `CList* menu` alias of top_level = no
byte change (copy merged); forced priorities through wf.sh on split PIECES do not stick (re-sort recomputes them).
RESIDUE = one allocator decision that cascades through all 331 hunks (c2prio trace tmp/pt_a1.trace, tools/lin.py lineage):
- Retail: top_level is in edi from its definition 0x0058591E to 0x005873CB with exactly two memory references (store 0x00585923, `lea` for
  Components.push_back 0x00587661), `this` stays in ebx until 0x00585CBC, `gold` lives in memory ([ebp+34h]), NO constant sits in edi (60 `push -1`
  immediates; only `push 3 ; pop esi` 0x005861E2 and `push 13h ; pop ebx` 0x005867E2), the six GetTC keys are immediates.
- Ours: top_level root #27 prio 88 is split by the entry variables (clothing_entry -> esi first), its pieces get edi everywhere EXCEPT the first
  region (source lines of CreateComponent .. the six GetTC .. mini map): there the remainder piece has prio -34 (5 references, ~40 live-through
  blocks), so `const 0x13` piece #30 (prio 24), the other GetTC keys (prio 20) and `const -1` piece #83 take edi first; the last top_level piece
  #35 (prio 55, allowed esi/edi, equal cost) then takes esi, gold gets edi, the vtable goes to ebx and evicts `this` at 0x00585C17.
- Needed: the first-region top_level piece coloured edi before the constants (or adjacent to an edi piece: copy affinity -1 would choose edi).
  Not found: the source fact that gives it. Measured after this note: one-argument PeekTC(&hero_stats) = byte-identical object (a6), so the
  one-argument forms are not the lever. Ideas NOT tried: (twins of this TU use the two-argument
  FABLE_RETAIL_CTHING_GETTC_EXPANDED form), fwsize on the row's helpers, STLport helper texts (the row has ~30 `new CAction` / push_back sites),
  statement-by-statement diff against a console sibling builder (the PC twins are small and share little).
Frame facts for the next agent (ebp-relative, retail): top_level [ebp-78h] (bottom), this [ebp-14h], hero_stats [ebp-1Ch], misc_def [ebp-18h],
clothing_tc [ebp-10h], manager [ebp+3Ch], the six TC out-locals zeroed in declaration order +4C,+34,+30,+14,+50,-10 (weapons, inventory, map,
abilities, stats, clothing; keys 13h,11h,1Bh,6Fh,14h,12h), second-loop bool on the frame [ebp+3Bh], first-loop bool in bl.

## Row 3: 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList (2,652 bytes) -- DIFFER(2649v2652), unchanged (differ/005f59c7.cpp = C12's text)
Grades this session: 5 (baseline + four STLport helper-text variants). Measured:
- FABLE_RETAIL_STLP_MALLOC_ALLOC_ALLOCATE / STLP_LIST_CONSTRUCT_HELPER / STLP_LIST_BASE_CONVERT_ALLOCATOR (one each and all three): NO byte change
  (CAction's constructor is called out of line here; no list code is expanded in the row).
- Opt-in set v the PC twins 0x005F1B10 / 0x005EEFF2: only STLP_VECTOR_PUSHBACK_OUTOFLINE v _NOINLINE (C12: no effect) and the two CDEFPOINTER
  opt-ins the sorting loop needs. Statement idioms already equal the PC twins (slot loop) and the landed console twin 0x005F4C8F
  GetExpressionsListUIComponent (sorting loop, textBank / categoryTitle tail).
- c2prio (tmp/pt_b0.trace): contribution of a candidate in a block = cost x K (K = candidates referenced in the block; a write costs 3, a read 1);
  live-through = -K. textBank 56 = 7 x K 8 (tail block 87: this 5, parent 4, const -1 12, textBank 7, subList 8 + PeekComponent result #78,
  stealth _M_start #31, const 0). const 0 = 48, of which +60 is block 10 (the 15 vector zero stores, K 4: allItems local, i, const 0, inventory).
  C10's what-if values are exactly the K 5 numbers: textBank 35 = 7 x 5, -1 piece 40 = 12 x 5 + 6 - 26. So retail's tail block holds ONLY the five
  essential candidates: no temporary for the PeekComponent() result AND the first vector-free compare (0x005F63C0 `cmp [ebp-34h],edi`) is not in
  the same c2 block as the tail, although retail has no branch between 0x005F6182 and 0x005F63C0. Open: what source construct ends a c2 block
  there without a machine branch (a second return path whose clean-up c2 merged? an inline-return label?). Not tested this session.
- Frame facts (retail v ours, same frame size 0xD4, 43 slots each): retail gives the `inside` arm's CCharString temporary its own slot [ebp-8]
  (0x005F5E38) and shares the n+1 spill [ebp+58h] (0x005F5C65 / 0x005F610F) with the tail's CWideString temporary; ours shares the two arm
  temporaries ([ebp+0]) and gives the spill its own slot [ebp-58h]. Consequence of the colouring, not a missing local.

## Row 2: 0x006096E0 CTCInventoryMap::ConstructLogbookListPC -- not worked (extent checked, draft does not compile today)
- EXTENT: the catalog length 12,832 is RIGHT. Code ends `ret 4` at 0x0060C8DA (next byte 0x0060C8DD), 3 bytes of `lea ecx,[ecx]` padding, then the
  8-entry switch table 0x0060C8E0..0x0060C8FF (targets 0x0060B0A1, B0BD, B0D7, B003, B025, B047, B066, B085; used by
  `jmp dword ptr [ecx*4+0x60C8E0]` at 0x0060AFFC); the next function (matched row 0x0060C900) starts at 0x0060C900. 0x0060C900 - 0x006096E0 =
  0x3220 = 12,832. The scan's last "instruction" `pushal` is table data. No oracle rebuild needed (rows.tsv unchanged).
- ref/006096e0/draft.cpp (DIFFER(12864v12832), 444 hunks, graded by its chunk WITH `// Flags: /d2inlT71`, which is not allowed in a hand-back) is
  SRC_FAIL on today's rebuild/include + this overlay, with or without the flag: seven opt-ins it defines exist nowhere today
  (FABLE_RETAIL_CHEROLOGBOOK_LOGBOOKSCREEN [CHeroLogBook::LogEntries is private, today's header has the FableWin accessor GetLogEntries() by
  value], FABLE_RETAIL_CTCINVENTORYITEM_STARTINGRIGHTHANDEDSET [GetStartingRightHandedSet], FABLE_RETAIL_CTHINGOBJECTDEF_GRAPHIC,
  FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_BODY, FABLE_RETAIL_STLP_LIST_CREATE_CONSTRUCT, FABLE_RETAIL_STLP_MAP_COPY,
  FABLE_RETAIL_STLP_TREE_FIND_OUTOFLINE); ref/006096e0/hdr_STALE holds only a README, so the opt-in bodies must be re-derived
  (pending/2026-11-08_linux_hand883/hdr is the place the draft's own note points to). First compile errors: line 118 (LogEntries private),
  line 370 (GetStartingRightHandedSet). differ/006096e0.cpp = the draft unchanged (line 2 annotated).
- Starting points for the next agent: twins 0x0060EEB0 / 0x0060C900 (c14a), the STLport-verbatim layers of C31 (the draft's /d2inlT71 symptom
  "CCountedPointer<CAction>(T*) of groups 1-3 called v expanded, 590 v 591 calls" is exactly the caller-size window class C31 closed in
  0x00601310 with MALLOC_ALLOC_ALLOCATE + LIST_CONSTRUCT_HELPER + LIST_BASE_CONVERT_ALLOCATOR + named `new` results; ledger in prev_C31/NOTES.md).
