# agentC31 NOTES (acctC lane, w22) -- continues agentC28 (prev_C28/), C26, C23, C21

## Row 0x00603000 CTCInventoryMap::CreateMiniMapMarkers -- state after ~25 grades (NOT matched)
- probe/r1/d0.cpp = prev_C28/differ/00603000_diag_freq.cpp regraded: DIFFER(9571v9571), 303/303 calls, 6 real hunks (0x006031D7, 0x00603C22, 0x00604B40).
- NEW source fact (evidenced): hunk 0x006031D7 (`movsx ecx,word [ecx+8Eh]` / `mov edx,[eax]` / `push ecx`) closes with the landed sibling's
  spelling `GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber())` (landed 0x00605570 line 140 has the same statement and retail
  0x00605592..0x006055A2 has the same instruction order). probe/r1/a1.cpp = d0 + that: DIFFER(9555v9571), 303/303 calls, but 92 real hunks:
  the allocator now keeps `this` in ebx through the teleporter loop (retail reloads `this` into edi at 0x00603928 from its home [esp+38h]
  and gives ebx to the teleporter list head 0x00603949); frame home of `this` -60 v retail -136, 38 frame symbols off. Guard placement /
  count does not change that (probe/gv/p1..p5 all the same 92 hunks); no guards = 9698, 303/304 calls (a4).
  So d0's "3 hunks" state was compensating: the `ParentThing->` text and the right `this` allocation exclude each other until another
  source fact is found. Tried on a1 (no gain): `followed = 0` (t2 same), `teleporter = 0` (t3 9571 / 105 real), `house = 0` / `wife = 0`
  (t4 9586 / 47 real; t5..t7 worse), erase loop as `while` (b1 = a4), GetParentThing().GetTC(&followed) (a2 = a1).
- Frequency facts read from the trace tmp/inl_a4.txt (default Reset text, no guards): c2's estimator sums incoming edges, a `for` with an
  increment leaves the code AFTER the loop at the loop BODY frequency (the "leak"; the erase loop without increment is scored like a
  `while`: body x9, no leak), early-return arms lose their mass. Reset sites: 40 (first loop, f 1.25, called), 92 (hero block, f 0.3125,
  called), 160 (creatures, f 15.6 = 1.25 x 5 x 5 x 0.5; retail CALLS it at 0x00603838), 230 teleporters f 78 (expanded) ... the later
  loops saturate at 1.8e11 (2^64/1e8). Score = (c/f + 2c)/3 for f > 1: Reset (c 95) is called iff f <= 2.97; cost c gives the
  threshold f* = c/(222-2c): 96 -> 3.2, 98 -> 3.77, 103 -> 6.44, 104 -> 7.43, 108 -> 18, 109 -> 27.
- "One point short" pattern (unexplained, recorded for the next agent): three natural frequency changes each land EXACTLY on f = 3.125
  (score 73 v T 74) for the site retail calls: row 1 if the IsMapLoaded loop did not leak, row 2 loop 3 with one leak less, row 2 loop 1
  site 81 as it is. A Reset price of 96..98 would make all of them called; 0x00605570 (GetDef c 106 expanded at F, then Reset pairs
  called at F/2, and the first Reset pair PRECEDES GetDef) needs Reset c >= 102 under ANY frequency unless GetDef is cheaper, and with
  c = 103/104 it needs F in (10.6, 12.9 / 14.9] at GetDef (ours F = 1). So no single (price, frequency) pair explains the three rows
  with the texts we have; price 108/109 + our frequencies explains the two map rows (C21) but gives a rotated frame, three halvings
  after the hero block (C26) give the retail frame. The frame evidence favours the frequency reading.

## Row 0x00601310 CTCInventoryMap::ConstructQuestsScreenPC -- RELOCATION_MATCH (probe/r3/n1.cpp), what closed it
- Caller-size window re-derived from traces (tools/inl.sh + ag.py): the dtor penalty is 26*(a+g)/9000 added to c 47; the LAST end
  destructor (pastEntry, retail call 0x00602E09) is called iff its a+g >= 9347 and the one before (56 less) is not, and the two (T*)
  ctors (pastList / curList) stay expanded iff a+g <= 9000 there: last-dtor a+g must be in [9347, 9403). c0 (C23's text) = 8645.
- Budget is STL text, measured per knob (tuples before the last dtor, relative to c0): REBIND +940 (16 x 3 allocate sites: per `new
  CAction` two list heads + one node per push_back = 48 sites x ~19.6) -> overshoots (both (T*) ctors called, 6705 bytes);
  NEW FABLE_RETAIL_STLP_MALLOC_ALLOC_ALLOCATE (allocator<T>::allocate through the malloc-based __sgi_alloc layer, STLport 4.6
  _alloc.h; body `void* __result = malloc(__n); return __result;`) +672 (14/site; `return malloc(__n);` is 9/site = +432);
  existing FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER (_M_create_node through _Construct) +192 (16 x 12);
  NEW FABLE_RETAIL_STLP_LIST_BASE_CONVERT_ALLOCATOR (_List_base ctor builds the node allocator from `__a` through allocator's
  converting ctor = _STLP_CONVERT_ALLOCATOR for a member-template compiler, instead of a default-constructed one) -160 (32 x -5).
  The three STLport-verbatim layers together = +704 -> 9349: inline decisions = retail (c6, DIFFER 6922v6931, 43 real hunks left).
  REBIND is NOT what STLport 4.6 does for VC7.1 (the proxy's own allocate/deallocate exist only without member template classes).
- Then three statement idioms of the landed twin 0x005FE090 (ConstructQuestsScreen) closed the code:
  (1) named `CText* descriptionText = static_cast<CText*>(description.Get());` used for SetValue / AddFakeChild, declared AFTER
      `frame->AddPositionChild(description.Get())` (retail 0x00602972 `mov eax,[esp+50h]; push eax` then 0x0060297F `mov edi,[esp+50h]`);
      it also gave `menu` its ebp piece up to the ConstructPastQuestsListPC push (0x006026A8 `mov edi,[ebp]`, 0x00602708 `push ebp`);
  (2) FABLE_RETAIL_CCOUNTEDPOINTER (ctor(T*) tests the member `Data`, as the twin defines): c2 then threads `if (!curList.IsNull())`
      through the ctor's null arm (retail 0x006027F4 `je 0x6028D0`, 0x00602806 `je 0x6028C7`);
  (3) e4 = all of that: DIFFER(6967v6931), 2 real hunks = the last dtor again expanded: a+g 9341, SIX tuples short of 9347.
- Closer (INFERRED / FITTED inside a 56-wide window, needs +6..+61): the 16 `new CActionParamULONG(...)` named before push_back
  (`CActionParamULONG* observeUpParam = new ...; observeUp->UIActionParams.push_back(observeUpParam);`) = n1, RELOCATION_MATCH.
  Supporting hint only: FableWin's /Od row is 2048i v 1897i for e4 (+151i) and 1929i with the named locals (+119i) (fwsize, /EHsc /Os).
- Row 0x00601310 hand-back: src/00601310.cpp RELOCATION_MATCH (q.py and read-only vland: `00601310 RELOCATION_MATCH PASS`, WINS 1, 0
  RELOC_IDENTITY; WARN RELOC_NAME 0x00429F43 = DeleteData<CAction> v catalog 'CTCMapwho::OnDie' -> NAME_FIXES.tsv; WARN RELOC_CONFLICT
  ~CCountedPointer<CComponent> 0x00429C53 here v 0x004291DE in landed 00570332 -> LABELS.tsv, coordinator). test/00601310.cpp: vt.py
  PASS; five one-token mutants each FAIL natively (selected/inactive swap, IsNull negated, entry order, a missing AddPositionChild,
  AddParam target). addmatch.py done (payload.json / oracle.tsv). Sibling prev_C28/src/0061e6d0.cpp regraded RELOCATION_MATCH on the
  changed overlay. No flag, no pragma, no assert, no DIAG guard. Name proven: reloc_bindings + Ego_r + FableWin PDB sets.
- Tuple ledger for the next budget row of this TU (trace-measured on this row): c0 8645 | +V1 allocate 9077 | +CONSTRUCT 9269 |
  V2 allocate +CONSTRUCT 9387 (ctors called) | V1+CONSTRUCT+CONVERT 9109 | V2+CONSTRUCT+CONVERT 9349 | +twin idioms (e4) 9341 |
  +16 named params (n1) 9357. One named pointer local = +1 tuple.

## Rows 0x00603000 / 0x005FF190 -- what the next agent should know (not matched; ~27 grades on row 1, row 2 not opened)
- Do NOT keep `ParentThing->GetCurrentMapNumber()` in row 1's region compare: the sibling spelling is evidenced (above); the price
  is that the `this` allocation in the teleporter loop must then be found again (retail: `this` reloaded into edi at 0x00603928 /
  0x00603C22, ebx = the teleporter list head). differ/00603000_diag_freq_getparentthing.cpp = that state (DIAG guards, 92 real hunks).
- The two remaining d0 hunks are allocator facts of the same kind: 0x00603C22 (`this` reload before `it`) and 0x00604B40 (the
  traders loop reloads `it` from [esp+24h] at the loop top: the back edge does not carry it in eax).
- Reset split, all readings measured so far: (a) price 108/109 with our frequencies (C21) -- no natural text; (b) default price 95
  with the code after the hero block 5.5..6.5x less frequent (C26) -- no natural construct, and 0x00605570 contradicts it unless
  GetDef<CUIMiscThingsDef> (c 106) is cheaper there (first Reset pair is BEFORE GetDef, so no divisor can sit between);
  (c) price 103/104 (= the 103-byte body with NO `this` saving, or a 111/112-byte body) explains row 2's loop 1 with no change at
  all (f 6.25 -> score 74 = called), needs /2.5../4.8 before row 2's third loop and before row 1's creatures loop (one `for` leak
  less = /5 is one point off again), and puts 0x00605570's GetDef at F in (10.6, 12.9] (e.g. two leaking `for` search loops in
  place of the landed `while` loops would give F = 25 / 5, not tried). None is proven; the coordinator's row-level fatal-check macro
  was NOT realised (row 1 would still be DIFFER on the allocator hunks, so nothing could be handed back as matched).
- Untested idea worth one trace: the STLport layers of this session change a+g only, not Reset's price; but a row-level macro is
  not the only no-byte lever -- an inline layer changes neither price nor frequency, so the lever must be one of (a)-(c).
