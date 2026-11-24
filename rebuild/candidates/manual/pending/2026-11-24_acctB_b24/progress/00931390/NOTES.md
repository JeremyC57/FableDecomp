# 0x00931390 CAIStateGroup_MinionReactToCriminal::Update -- B24A8 (2026-11-24)

## Lessons that apply (read before grading)
- CODEX_P28 #2/#3: a "register colouring / frame order" residue is a source-shape or callee-visibility fact; toggle body visibility.
- GOTCHAS 335: frame-slot ORDER follows block-frequency weights; read the /FAs `_sym$` map instead of esp arithmetic.
- GOTCHAS 114 / Q25: declare long-lived locals before the inner blocks; scope decides slot sharing.
- GOTCHAS 366 / 91: a local vector's push_back reload and _M_finish register depend on _M_insert_overflow body visibility (here: default
  visible body is best; OVERFLOW_OUTOFLINE same, DECL_ONLY far worse).
- B24A7: GetTC/PeekTC ORIGINAL_CHAIN texts -- measured here: byte-identical to GETTC_EXPANDED (not the talk-site lever).

## (RESOLVED 2026-11-24: coordinator re-cut the oracle to 3863; fg now grades DIFFER(3863v3863) 29 hunks) ORACLE LENGTH WAS WRONG
The registered oracle is 3891 bytes; the real function is 3863 bytes (0xF17: `ret` at 0x009322A6). The extra 28 bytes are 9 int3 pad
plus an unreferenced 19-byte COMDAT at 0x009322B0 (fastcall `CIntelligentPointer != T*` shape). fg/q can never print MATCH; a match =
only the trailing delete hunk left + raw compare of the first 3863 bytes. Correct length: 3863.

## Baseline (today's headers)
draft = DIFFER(3863v3891) 30 hunks (29 real + oracle tail). Residues: (a) doors vector at esp+0x38 vs 0x34, its _M_start zero not in esi,
PHome 4th read keeps esi (retail `lea ecx,[edi+0x1c]`), dtor reload; (b) the 4 address-taken bools packed below doors (ours) vs above
(retail): retail block2=0x40 flee=0x41 nav=0x42 marker=0x43, ours marker=0x34 block2=0x35 nav=0x36 flee=0x37; (c) 3 talk-site GetTC
thing pointer in eax (ours) vs esi (retail); (d) loop-alignment pads follow.

## B24A8 grades so far (see ATTEMPTS.log)
- Byte-identical to the draft (no lever): GetTC/PeekTC ORIGINAL_CHAIN typed forms; drop invented `thing` / `chosen` locals (adopted:
  v/h3.cpp = draft minus those two inventions, same bytes); front()/*begin()/size()>0; MALLOC_ALLOC_DEALLOCATE / ALLOC_PROXY_REBIND;
  _M_insert_overflow OUTOFLINE (DECL_ONLY far worse, 136 hunks); home ref used/unused in block 1, home declared in the do body.
- `PHome != tracker->GetPThingInside()` (FableWin instantiates ??$?9VCThingBuilding@@@@YI_NABV?$CIntelligentPointer@VCThingBuilding@@@@PBVCBaseObject@@@Z
  in this TU, = retail's unreferenced COMDAT 0x009322B0) with FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_ITEM_NE: one extra hunk,
  `cmp eax,ebp` v retail `cmp ebp,eax` (0x00931A64). The `inside` local stays (it gives retail's operand order) -- open doubt.
- No home ref at all: 50 hunks (blocks 1 and 3 lose the PHome CSE).
- Frame packer trace (tools/pk, pkshow.py): bools are four 1-byte slots, w=1 each (density 1000); doors 12 bytes w=11 (916) -> bools
  below doors. Retail has one more doors memory reference (dtor reload `mov eax,[esp+34h]` at 0x0093170D) -> w=12 -> density tie 1000,
  doors first. So the frame order follows the register allocation of doors._M_start (see c2prio below), not a separate cause.
- c2prio (v/prio_h3.txt): PHome address is one CSE temp #177 (lines 92..232, prio -1295) split in pieces; block-1 piece #188 got esi
  and covers the PeekTC use. Retail: the PeekTC use is a separate (dropped, rematerialised `lea ecx,[edi+1Ch]`) piece and
  doors._M_start takes esi from its zero init through the no-owner path to size()/sort/[0] and the dtor test.

## More grades (B24A8), all on v/h3.cpp base (= differ/00931390.cpp)
- Byte-identical (no lever): typed GetTC(&ptc_talk); a `CThingAICreature& creature = GetCreature();` at the 3 scream sites;
  const home ref; home used for every PHome access; explicit instantiation of _M_insert_overflow before Update (compile-order probe).
- Much worse (so the shape IS sensitive to loop structure): scream retry loops as for(;;) like the cower site (3994 bytes, 170 hunks);
  door gathering as a for loop (3948, 173 -- this is Codex's 3948/172 family); home ref declared after block 1 (42 hunks).
- KEY PROBE (v/p1.cpp): with block 1 free of the PHome CSE (esi unused there), doors._M_start STILL is not given esi at its zero
  init. So the PHome piece is not the cause: the cause is that c2 does not treat the ctor's zero as a def of the start candidate
  whose piece reaches size() along the no-owner edge (retail: `xor esi,esi` init, no-owner `je 0x009314F1` skips the reload at
  0x009314ED, start in esi across PeekTC's GetPItem/LowerBound calls). In ours both paths reload at the size block. Retail's
  behaviour means c2 knew doors was not modified by PeekTC's opaque calls (doors not exposed / flow-sensitive), ours did not.
  Once start owns esi from the init, the PHome block-1 piece must split (retail's `lea ecx,[edi+1Ch]` at the PeekTC), the dtor
  reload appears (doors w=12 -> packer tie -> retail frame order) -- i.e. one cause explains most hunks.

## State for the next agent (row in progress, 30 grades by B24A8)
Best: differ/00931390.cpp (= v/h3.cpp) DIFFER(3863v3891), 29 real hunks + the oracle tail. Header overlay: none needed (hdr/ empty).
Best next ideas, in order:
1. Find what makes doors "exposed" before the loop in our TU: test each callee that receives &doors or a member address
   (push_back -> _M_insert_overflow this; _IsPODType temp; allocator/alloc_proxy ctors from the default argument
   `vector(const _Alloc& a=_Alloc())`; ~_Vector_base deallocate) for visibility/inline. Retail evidence: start in esi across
   GetPItem 0x00A01B50 and CVectorMap::LowerBound 0x004365B0 calls at 0x009313FD..0x00931437.
2. Then re-check the 3 scream-site GetTC thing registers (0x00931541 / 0x00931868 / 0x00931F35: retail esi + add esi,44h; ours
   eax + lea esi,[eax+44h]); the cower site (inline LowerBound) already matches.
3. The `inside` local is invented (not in the Ego_d list); FableWin's operator!=<CThingBuilding>(ptr, CBaseObject*) says the source
   compared `PHome != tracker->GetPThingInside()`; that form flips one cmp (0x00931A64). Revisit once the allocation matches.
4. `home` reference is Codex's fit (not in the Ego_d list); without it 50 hunks. Revisit after 1.
Tools in tools/: g.py (grade + save listing), slots.py (esp-slot map), fas.py (/FAs symbol map), pk/ (frame packer trace,
pkshow.py), c2prio runs in v/prio_h3*.txt.
- s1: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_FINISH_LOCAL_PASS24 push_back text: byte-identical (no lever).

## B24A9 (2026-11-24), 10 grades
- Baseline after the re-cut: differ/ (= v/h3.cpp) DIFFER(3863v3863) 29 hunks; MATCH is now printable.
- Listing reading (retail v ours, v/last_h3.cpp.txt): &doors goes ONLY to _M_insert_overflow in both (retail `lea ecx,[esp+48h]` with 5
  pushes = esp+34h at 0x009314C9; ours esp+4Ch -> 38h). From the doors init through the gather-loop exit (read by eye) no other reference takes &doors; push-shifted references after the
  loop (sort, `door` ctor, movement loop) were NOT searched. Both listings keep _M_finish in ebp ACROSS the loop's
  GetPItem calls without reload (only the overflow call forces `mov ebp,[esp+38h]`), i.e. c2 already treats GetPItem as not modifying
  doors in ours too. So (INFERENCE, resting on finish staying in ebp across GetPItem in both listings) it is not alias/exposure but the register choice for start's INIT piece (retail start is a candidate def
  `xor esi,esi`; ours stores the shared ebp zero). Trace v/prio_h3.txt (the trace labels them only `local`; mapping INFERRED from their source-line sets = decl/push_back/size/sort/dtor lines): start = candidate #32 (lines 96..142, prio -238, split: eax
  #186, esi #21, eax #155, 1 piece memory); finish = #174 (prio 98, ebp); end_of_storage = #175 (prio -38, memory).
- No lever (all 29 hunks or worse): owner declared before doors (o1); owner = NULL (o3); owner hoisted to function scope (o4);
  `bool has_owner = PeekTC(..)` (q5). `!doors.empty()` 3847 / 30 hunks (o2: retail tests size, `sub; test edx,0FFFFFFFCh`).
  doors declared in the do body before the block-1 if: 3843 / 199 hunks (q1; retail destroys doors at 0x00931705, inside block 1).
  push_back(*it) implicit conversion: SRC_FAIL (q6).
- Talk sites (idea 2), read only: retail `mov esi,[eax+18h]` ... `add esi,44h` (thing and the TC map coalesced in esi: the inlined
  GetTC `this` is a register variable); ours `mov eax,[eax+18h]` ... `lea esi,[eax+44h]`. Next: an inline layer whose `this` param is
  the creature (e.g. a CThing member called on GetCreature()'s reference through one more inline function) -- only with real header text.
- State: best unchanged = differ/00931390.cpp. Moved to the next B24D row on the coordinator's queue with this row still open.
