# wfD2 NOTES

## 0x00CFE030 NScript::CQ_AwakeningTheOracleScript::COracleTablet::Main -- parked (DIFFER 3575v3575, 4 lines)
- Name: Ego_r ?Main@COracleTablet@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ (draft identity block); extent 3575 confirmed (final `ret` 0x00CFEE26).
- Draft ported unchanged except __forceinline -> FABLE_FORCEINLINE; all opt-ins exist in today's headers.
- Residue: CreateCreature arms address `spawn_points[i]` as `[edi+ecx]` / `lea esi,[edi+ecx]` (0x00CFEC29/2C) and `[edi+eax]` /
  `lea ecx,[edi+eax]` (0x00CFECD7/DA): strength-reduced offset first; ours has start first. Compare sites (`mov edx,[ecx+edi]; add ecx,edi`) agree.
  (fg's hunk at 0x00CFE03E is a normaliser artefact: identical text.)
- Tried (no change): FABLE_RETAIL_STLP_VECTOR_INDEX_START; `long i` / `int i`; `(i + begin())->` probe; dead initialised locals
  k=1..36 and 48..288 at the top, k=1..30 inside the loop (never flips: c2 commutative order not keyed on outer count here);
  named reference before the if (3548, CSEs compare sites), per-arm reference (computes address before the "NecropolisEnemy" temp).

- SUCCESSOR (+~25 grades): arm spellings `spawn_points.begin()[i]`, `*(spawn_points.begin() + i)`, `*(&spawn_points.front() + i)`;
  INDEX_START with begin()-based spellings at the compare sites only / the arms only; dead inline expressions (`spawn_points.size();`,
  `spawn_points[i];` k = 1..12 before the loop and inside arm 1): no change, always the same 3 fg hunks. c2prio trace
  (tmp/c2.trace): the strength-reduced offset is candidate 137 (edi), the arm addresses are CSE temps 141 (esi) / 142 (ecx); the
  compare-site begin loads 139 / 140. fwsize: every helper the row expands is SAME as FableWin except the STLport debug-checked
  operator[] (41 instructions in FableWin, universal for all vector instances: not evidence). Parked: record-id tie, no lever found.

- THIRD pass (wfD2 successor 2, +~330 sweep grades, MEASURED with tools/lk/lk2.py + show2.py = wfB6's comparator log without the float filter;
  tools/lk/kk.sh prints the one comparison): the arm SIB order is c2's commutative-operand sort of {`spawn_points._M_start` field record, LSR
  temp T}. Keys: field record id S * 32; T key = (T's record id * 64) mod 65536 (T is always the 12th record of a fresh 32-record chunk at LSR
  time: id = 32*C + 11, so T's key id = (64*C + 22) mod 2048, C = chunks allocated by the whole function before LSR). Higher key first.
  Ours: S = 653, C = 67 -> T key id 214 < 653 -> begin first. Retail needs T key id > S. Record ids are per FUNCTION (preceding functions in
  the TU do not move them: measured with 1 / 40-local / 6 extra functions above the row).
  Measured S / T-key-id for k dead `long fid = 0;` before `Game->MiniMapRemoveMarker(Me)` in the 3-tablet block (same as k at the top of the
  function): k=0 653/214, 1 654/214, 2 623/214, 16 349/278, 32 685/278, 49 350/342, 64 717/342, 80 413/406, 81 382/406 (flip).
  RELOCATION_MATCH windows (dense sweeps): one position (top or anywhere up to that MiniMapRemoveMarker statement): k = 81, 82, 112..114,
  nothing below 81 (1..80 dense); positions after that statement: nothing (1..36 dense, 79..84). Two positions: 17 before that statement +
  25..32 before `AlreadyDugHere = true;` (total 42) matches. So the row is RELOCATION_MATCH with a dead-local ruler, but the smallest ruler
  found is 42 named locals (not a k <= 7 device): NOT handed back as matched. Windows by the model: >= ~97..128 FEWER records anywhere before
  LSR (C <= 63: T key id wraps to 2006..726) or >= ~220 more after the spawn block, or the mixed windows above. FableWin /Od: the row is
  +214 instructions / +846 bytes larger than ours at /Od /EHsc, but the three matched sibling Mains show the same gap (+156..+240), so it
  is not row-specific evidence. Next idea: a helper text expanded at many sites (28 T-return sites: ~CTimer, CCharString ==, operator==
  FableWin 24i/73b v ours 19i/55b) whose real text has fewer / more parameter or local records.
- READY-TO-ACCEPT package for the ruler variant (coordinator decision; NOT in payload.json / oracle.tsv): differ/00cfe030.ruler42.cpp
  (= differ/00cfe030.cpp + two marked `// Fidelity:` declarations: 17 unused `long` locals before `Game->MiniMapRemoveMarker(Me)` in the
  three-tablet block, 25 before `AlreadyDugHere = true;`) grades RELOCATION_MATCH with q.py, and a trial addmatch into the scratch dir tmp/acc +
  read-only vland gave `00cfe030 RELOCATION_MATCH PASS ... WINS: 1` (tmp/acc/vland.log). Behaviour test: differ/00cfe030.test.cpp (vt.py PASS;
  valid for any source of this row). To accept: `python3 $S/tools/addmatch.py $W 00cfe030 'NScript::CQ_AwakeningTheOracleScript::COracleTablet'
  'Main' $W/differ/00cfe030.ruler42.cpp $W/differ/00cfe030.test.cpp` after copying the two files to src/00cfe030.cpp and test/00cfe030.cpp.
  Other measured facts: the ruler positions are fixed by the model (17/18 before that statement = first-pass chunk boundary; 25..32 anywhere
  later = one more chunk before LSR); rulers inside ~CTimer (expanded ~8 times at inline time) also raise T (4 locals there replace the 25) but
  ~CTimer 0x00CD4470 is a callee retail CALLS at cold exits, so its text must stay the landed body. 2-D sweep k = 0..20 x m = 2/4/8: only k = 17, 18 match.
  FITTED / inherited items of the plain source (from the earlier chunks, unchanged): `const CScriptThing* sparkles_ptr = &sparkles;` (keeps IsAlive
  virtual), the final erase loop as for(;;)+break, FABLE_FORCEINLINE CTimer::operator= / GetTime, row-local operator== text `PImp->IsEqualTo(*other.PImp)`.

### 0x00CFE030 -- SUCCESSOR 3 (wfD2, Opus): RELOCATION_MATCH, test PASS, addmatch, read-only vland WIN (4 wins)
- src/00cfe030.cpp = differ/00cfe030.cpp + ONE dead release assert (WF_RULES device d, `// Inferred:` + FITTED) before `AlreadyDugHere = true;`:
  `FABLE_RETAIL_ASSERT(!(Me.GetDataString() == "EXPRESSION_Y" || ... 8 == terms on the row's own literals))`, overlay hdr/fable_assert.h
  (FABLE_RETAIL_ASSERT_DEAD_ARM, byte copy of the wfE3/wfB4 file) + CTimer ctor in init-list form (= landed 0x00CD4450 shape; byte-neutral).
- Why it works (kk.sh measured): each dead arm adds c2 records (8 trivial asserts at the 3-tablet site: S 653 -> 781, T rid 2155 -> 2219); after the
  spawn_points field record S is made, they raise only the LSR temp's chunk count C, so T's key id passes S (needs C >= 74).
- Windows (all RELOCATION_MATCH, single place before `AlreadyDugHere = true;`): 18..40 asserts of `AlreadyDugHere`; 15+ of a two-compare int test;
  4..11 copies of the row's `Me.GetDataString() == "BAD_ONE" || ... "GOOD_ONE"`; one assert with N `==` string terms: N = 7, 8 (one), 4..6 (two), 3 (three);
  `!(8 terms)` one assert yes, `!(7 terms)` no. Inline CCharString text == (FORCEINLINE opt-in) is what carries the records; `!=` (out-of-line 0x0099E960),
  Game calls with temporaries (3 x IsObjectInThingsPossession) and in-loop `i < spawn_points.size()` asserts are light (16+ needed in the loop).
  The same asserts at the TOP of Main change live code (3546 / 3578 bytes: the dead IL eats the inline budget of later sites): keep them late.
- Dead-arm literals are not emitted (strings -a on the obj: only the row's own literals), so the device adds no data.
- Also measured: `CScriptThing& sparkles = Game->CreateEffect(...)` (no sparkles_ptr) grades the same 3 hunks as the pointer form: kept the portable pointer form.
- Coordinator: device d (1 assert, condition / place / count FITTED) instead of the 42-local ruler (differ/00cfe030.ruler42.cpp, kept for reference).

## 0x00E27E90 NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Main -- RELOCATION_MATCH, test PASS, addmatch, vland WIN
- Name: Ego_r + FableWin name sets ?Main@CGTDI_Maze@CQS_GuardianTrophyDealerInfoScript@NScript@@EAEXXZ; vtable slot .rdata 0x012E20A8 (draft).
  Extent 3595 confirmed (ret at 0x00E28C9C region, last instruction ret).
- What made it match (from the parked draft, DIFFER 31 lines):
  1. FABLE_RETAIL_CPREDICATEBASE_NOVTABLE + `#pragma inline_depth(0)` region round the row-local CIsThingAlive ctor (the landed
     predicate idiom of 0x00D62480 / 0x00D706E0 / 0x00D8BCD0), keeping FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (retail expands the
     by-value copy in the SetIsPushableByHero argument slot 0x00E27F90 but calls the nested copy in the predicate 0x00E27EB2).
     ~CIsThingAlive stays declared (called 0x00E241B0).
  2. First wait loop written `for (;; Game->NewScriptFrame())` (EVIDENCED by the landed script-row idiom, wfA3 lesson: plain halving
     model): retail expands the implicit ScriptedThingBase ctor in the hit arm (0x00E28191..A8).
  3. Scoped `#pragma inline_depth(0)` round the talk-arm `seh_hero` declaration: retail CALLS ctor 0x007E72A0 there (0x00E28424).
     No uniform flag splits it (T30..150, S5..20 swept; T54..56 calls the copy-site, T57+ expands the talk arm, S16+ both arms).
  Inline-budget device, marked on line 2. No header changes.

## 0x00DC5F80 NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageWoman::Main -- RELOCATION_MATCH, test PASS, addmatch, vland WIN
- Full name proven: Ego_r + FableWin ?Main@CSecretPassageWoman@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ; vtable 0x012DA7A4
  slot 1 (.rdata 0x012DA7A8) stored by the CSecretPassageWoman ctor (Alloc 0x00DC5E50, reloc_bindings). The catalog's `CSecretPa...` is truncated.
  Extent 4245 confirmed (ends `ret` 0x00DC7014).
- What made it match (draft DIFFER 4259v4245, 36 lines):
  1. FABLE_RETAIL_CPREDICATEBASE_NOVTABLE + `#pragma inline_depth(0)` region round the row-local CIsThingConscious ctor/dtor (landed
     idiom), FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY instead of the draft's row-local copy-ctor body (same bytes) -> 4243v4245, 5 hunks.
  2. Second loop as `for (;; Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return; ... }` (landed idiom) instead of
     `if (T) return; while (true) { ...; NSF(); if (T) return; }`: identical layout, but the plain-halving frequency model keeps the
     const-0 piece in ebp across the loop (retail re-zeroes ebp at 0x00DC6BC7 / 0x00DC6FA2 and tests the PImp with `cmp ecx,ebp`).
  Inline-budget device marked on line 2. No header changes. Plain `inline` dtor suffices (FABLE_FORCEINLINE also matches).

## 0x00EE57B0 NScript::CV_TourGuideScript::CTourGuideGuide::Main -- parked on first pass (DIFFER 4356v4246, 60 hunks)
- Name: vtable 0x012F511C slot 1 (landed Alloc 0x00EE4BB0), Ego_r + FableWin ?Main@CTourGuideGuide@CV_TourGuideScript@NScript@@EAEXXZ.
- Draft ported (only __forceinline -> FABLE_FORCEINLINE); FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE exists in today's header.
- Call targets equal retail except ~CScriptThing 15 v 14 and ~STB 4 v 3: ours has one more exit pad (two inline ~CTimer pads at
  ours 0x00EE64C9 and 0x00EE66D9; retail has one, 0x00EE65D9, pushing ebp / ebx = the two timer indices).
- Register picture: retail ebx = 0 only up to the first wait loop, then ebx = StandStillTimer index (0x00EE58A9), ebp = TalkToHeroTimer
  index (0x00EE58BF); later zeros are `xor eax,eax` locals / immediates (WaypointCounter = 0 via eax, `cmp [..+0x164],0x12`).
  Ours: ebp = const 0 / 0x12 pieces across the body, ebx = TalkToHero index, StandStill index frame-only.
- Second pass (exit-site inline decisions; the length gap is pads, not registers):
  * Retail exit pads: P0 0x00EE5844 (first wait loop, before the timers: ~STB EXPANDED); PA 0x00EE6475 (main-loop top T check
    0x00EE590E + loop-bottom fallthrough: timers inline, ~STB inline); PB 0x00EE65D9 (Initialise first T 0x00EE594A, closing entry T
    0x00EE656F, closing wait loop 0x00EE65B8, closing post-wait T fallthrough: ~CScriptThing call, timers inline `push ebp/ebx`, ~STB call);
    PF 0x00EE6817 (everything else incl. Initialise wait loop 0x00EE5991 and rand-block T 0x00EE5C90: all calls).
  * Ours at default T: PA gets the closing-entry T check; an extra inline pad C serves Initialise first T, rand-block T and the closing
    wait loop; the closing IsPerformingScriptTask loop exits get their own inline pad.
  * /d2inlT sweep on the draft: T57..61 = 4228v4246 / 25 hunks (timer indices land in ebp/ebx as retail), T62..68 4276, T70+ 4356.
  * Best: `#pragma inline_depth(0)` round the closing block's IsPerformingScriptTask wait loop + its T check (sends them to PF as retail)
    with /d2inlT60 -> 4167v4246, 23 hunks: only P0 differs structurally (retail expands ~STB there; at T60 it is called; it expands only
    at T>=70 where the pads above break again) + the entry zero register (retail ebx, ours edi). differ/00ee57b0.cpp.
  * Tried, no gain: CTimer ctor body form, ExitMarker declared in the closing block (4196, 117 hunks), timer declaration order,
    for(;;NSF) for the StandStill / rand loops (4420), first wait loop as for(;;){if(Start)break;...} (T60 4199).
  * Next idea: find a source shape that raises the P0 sites' expansion priority at T60 (e.g. P0's two exits merged into one return via a
    flag / scope), or use default T with inline_depth(0) regions on every site retail calls that default T expands (rand-block T check,
    Initialise wait loop, closing IsPerforming loop) -- the closing-entry T check then still needs to move from PA to PB.

## 0x00EE57B0 -- SUCCESSOR: RELOCATION_MATCH, test PASS, addmatch, read-only vland WIN (src/00ee57b0.cpp, no flags, no regions)
- What closed it: the main loop written `for (; true; Game->NewScriptFrame())` (FITTED, ordinary spelling) instead of `for (;; NSF)`.
  Diagnosis (c2 inline trace, tools/inl.sh + tools/cmp_inl.sh): every exit site inside the main loop and the closing block had
  twice retail's static frequency (top T return f 1.25 v needed ~0.6; closing entry 0.47 -> ~CScriptThing expanded). A diagnostic
  extra T check before the loop (halving the entry) made all pads route as retail with no regions (only its own 11 bytes differed),
  so the frequency model, not inline budget, was the cause; the closing-block wait-loop alignment fillers (retail none) confirmed
  it. Loop forms measured: for(;;){..NSF();} / do-while / goto / while(!A&&!B) / `for(;!T;NSF)` all wrong (4628..5028);
  `for (bool loop = true; loop; NSF)` 4245 (6 hunks, loop not rotated); `for (; true; NSF)` RELOCATION_MATCH. Twin evidence: landed
  0x00EB7B00 notes `while (true)` v `for (;;)` changes VC7.1's exit-pad split the same way.
- The earlier note "a region round the inner return of the Initialise wait loop has no effect" was wrong: it does reroute that
  exit (jt.py shows it), the grade stayed 4261 only because the alignment / closing-entry residue remained.
- Test: model of Initialise waypoint choice, non-repeating response pick and the frame loop / closing branch (test/00ee57b0.cpp).

## 0x00D75060 NScript::CQ_HangingTreeGoodScript::FinalRound -- parked, first draft only (DIFFER 5237v4044, 107 hunks)
- Name: Ego_r + FableWin ?FinalRound@CQ_HangingTreeGoodScript@NScript@@QAEXXZ (public, thiscall, void). Extent 4044 (2 rets:
  0x00D75CB2 and the final one), confirmed by the extent scan; draft ported unchanged (only __forceinline -> FABLE_FORCEINLINE).
- Measured against retail (call targets): delete 39 v 22, ~CBaseClass 23 v 15, ~CBaseObject 15 v 6, ~CScriptThing 7 v 8, free 3 v 2:
  ours expands the three resources' STB dtors at 5 exits, retail at 2 (the approach-loop exit 0x00D7573A and the spawn-loop exit
  0x00D75AA3 pad), all other exits share the call pad 0x00D75FCD (~CScriptThing, 3 x ~STB 0x007E74D0, ~vector 0x008AC970, 4 x ~CScriptThing).
- Retail calls IsAlive VIRTUALLY on the local CScriptThings (`mov eax,[esp+14h]; lea ecx,[esp+14h]; call [eax+12Ch]`, 6 sites:
  0x00D751F4 Executioner, final loop Boss 0x00D75D28 / Executioner 0x00D75D55, inner loop 0x00D75E1E / 0x00D75E3E / 0x00D75E67);
  ours devirtualises them (6 direct calls 0x004AB130). Unknown source fact (reference locals? the row-1 note used a pointer to the
  local for one site); needs solving before the exit work.
- Early-exit pad (0x00D753E6 `je 0xD7558D` else ...) expands ~CScriptThing x5 + the vector dtor loop inline (element virtual dtor loop + free).
  * THIRD pass (best, differ/00ee57b0.cpp, default flags): inline_depth(0) regions round (a) the closing block's IsPerformingScriptTask
    wait loop + its T check, (b) the rand()%50 block's T check, (c) the closing block's entry T check -> DIFFER(4261v4246), 4 fg hunks of
    which 2 are fg artefacts (0x00EE5A02 / 0x00EE5B29 identical text) and 2 alignment fillers. The real residue (found by comparing
    jump targets): the Initialise wait-loop exit (retail 0x00EE5991 -> call pad 0x00EE6817) lands in the inline-timer pad here; a region
    round that whole while loop has NO effect (t10 == t9); and the closing entry T check (retail 0x00EE656F -> inline-timer pad
    0x00EE65D9, i.e. timers inline but ~CScriptThing / ~STB called) goes to the all-call pad with region (c), and without (c) expands
    ~CScriptThing(ExitMarker) and jumps into the main-loop pad (timers + STB inline). Next: find the per-site shape that gives the closing
    entry exit "timers inline, CScriptThing/STB called" (the default decisions of the Initialise first T check, which retail and ours both
    send to PB) and a region form that reaches the return inside the Initialise wait loop (wrap only the inner `if (T) return;`).
  * A region round only the inner `if (T) return;` of the Initialise wait loop (and/or the post-loop T check) also has NO effect
    (t12/t13 == 4261): the Initialise first-T return and the wait-loop returns seem to share one cleanup decision in our TU, while
    retail splits them (first T -> PB, wait loop -> PF). Look for a scope difference in retail's source there.

### 0x00D75060 -- SUCCESSOR pass: 5237 -> DIFFER(4041v4044), 24 hunks, call-target histogram EQUAL to retail (differ/00d75060.cpp)
- Virtual IsAlive (7 x `call [+12Ch]` as retail): bind Executioner, Boss and Nearest as `CScriptThing& X = Game->...(...)` (VC binds the
  returned temporary; precedent: landed 0x00CE0920 `CScriptThing& effect = Game->CreateEffect(...)`). Binding ALL locals that way is
  wrong (GetPos / GetDataString on FinalAmbushPoint / ExecutionerStandPoint / ChoppingBlock are inlined in retail, so those stay by value).
- Spawn loop `long n = 0; while (n < 4) { ... ++n; }` (any while / do-while spelling: same code) instead of `for (long n...)`: the for
  form gets VC7.1's "for" frequency model (every exit in the loop and after it at 2.5 x entry -> 5269 bytes). FITTED, not yet final.
- Residue (exit-site -> pad map, retail v ours): retail approach post-loop T 0x00D75700 -> call pad PF 0x00D75FCD, spawn post-loop T
  0x00D75AAA -> its own pad (3 STB + 5 CScriptThing expanded, ~vector called) + ret 0x00D75CB2. Ours swaps them: approach post T gets
  the B/S-expanded pad + ret, spawn post T goes to PF. Trace (tools/inl.sh + tools/cmp2.sh): approach header f 1.875 (in-loop return
  0.9375 expands all incl. V = retail; post return 0.4688 expands B/S = wrong, needs <= 0.405); spawn while-loop in-loop return 0.2344
  (called = retail), post return 0.1172 (called, retail needs 0.405..0.49). Retail therefore has spawn post > spawn in-loop
  frequency, which neither the while model (post = in-loop / 2) nor the for model (post = in-loop) gives. Tried: switch default /
  case order (no change), n++ / n = n + 1 / do-while / for(;n<4;) (same as while), while(true)+break (4073), for(;n<4;++n) (5269),
  approach loop as for(;;)+break (4050) / while(true)+break (3858) / for(;!X;) (same). Next: find the loop shape that boosts the
  post-spawn frequency (diagnostic extra statements first, as for 0x00EE57B0), and check the ~vector dtor's inline cost text.

## Matched rows were re-graded against plain repo headers (empty overlay): both RELOCATION_MATCH (no overlay dependence).

### 0x00D75060 -- SUCCESSOR 2 (+~25 grades, 8 inline traces): frequency model measured, one exit left, PARKED (structure fact unknown)
- tools/pm.py (T-exit pad map, retail v ours, fall-through pads included) + tools/fq.sh (per-return-site f / score of ~STB and ~vector).
  Scores: ~STB / ~CScriptThing sc = 51 + 8.44 / f, ~vector sc = 54 + 9.375 / f; a callee expands iff sc <= ~70..73 (69 and 70 expand, 74 does not).
- c2 static-frequency model, measured on 8 variants (c = running frequency): `if (T) return;` in straight-line code or a `while` body: site f = c / 2 and
  c halves; a return nested in an `if` block: f = c / 4, c -> 0.75 c after the block; a counted `for (init; cond; inc)`: body and everything AFTER it x5
  (site f = 2.5 c, an in-loop return does not reduce c); `for (;cond;)`, `while`, do-while, for(;;)+break: no boost; one `do { } while (0)` wrapper: x1;
  two nested: content x10, code after x2; a one-trip counted `for` wrapper: content x5, code after x10; empty wrappers are removed (no effect).
  Ours (differ/00d75060.cpp): s1 = s2 = 1.875 (guard for), s3 approach in-loop 0.9375, s4 approach post 0.4688, s5 spawn in-loop 0.2344,
  s6 spawn post 0.1172, later <= 0.0891.
- Retail decisions need: s3 >= 0.625 (~vector expanded), s4 < ~0.40 (all called), s5 < ~0.40 (all called), s6 in [~0.40, ~0.49] (~STB / ~CScriptThing
  expanded, ~vector CALLED; ours' s4 pad is byte-for-byte this pad), every later site < ~0.40. So retail has f(s6) > f(s4), f(s5): a persistent boost of
  x3.4..4.2 between the s5 halving and s6 (or s4 / s5 nested one level deeper AND a boost). No loop spelling gives it: counted for = s5 = s6 = 1.17
  (5269 bytes), while = 0.1172; if + do-while approach = s3 0.4688 (wrong); for(;;)+break spawn = while numbers with one extra T site (4073).
- Devices tried: scoped `#pragma inline_depth(0)` round the s4 check fixes s4 and puts the s3 pad at retail's offset (differ/00d75060.region_s4.cpp,
  DIFFER(3561v4044), 1 misrouted exit = s6). s6 cannot be forced: /d2inlT124..131 expands s0 / s4 / s5 and many helpers first (4782 / 5254 bytes);
  a one-trip `for` or triple do-while(0) wrapper round the s6 check gives f 0.586+ (sc(~vector) = 70 -> expanded, wrong) and leaks x10 to later sites;
  double do-while(0) = 1.17. None is a retail-plausible spelling anyway.
- Portability note: `CScriptThing& Executioner = Game->GetThingWithScriptName(...)` (non-const reference bound to a returned temporary) is an MSVC extension
  (other compilers reject it); landed precedents 0x00CE0920 and the cast-only I-44 form of 0x00DA91A0. Kept as the predecessor wrote it; a coordinator decision
  if the row ever closes (retail needs IsAlive virtual on Executioner / Boss / Nearest, 7 sites, and inline GetPos / GetDataString on the by-value locals).
- Next idea for a successor: find the source construct between the spawn loop's T check and the post-loop T check that multiplies frequency by ~4 (a helper
  macro expanding to nested loops, or the spawn block living in a counted loop whose T check sits in a colder nested block), using tools/fq.sh on candidates.

### 0x00D75060 -- SUCCESSOR 3 (wfD2, Opus, ~35 grades + 4 traces): inline-control devices measured, still PARKED (best stays differ/00d75060.cpp, 4041v4044)
- tools/cseq.py (new): call sequence retail v ours by target (retail targets named through our own DISP32 symbols + reloc_bindings), difflib opcodes.
- Per-site device test: /d2inlT125 (s6 ~STB / ~CScriptThing sc 124 -> expanded, s6 ~vector sc 132 -> called) + scoped inline_depth(0) regions round the s0 T check,
  s4 (existing), the spawn-loop T check (s5) and the spawn-loop scope end (FreedomFighter dtor, retail calls 0x004AA840 at 0x00D75A92): ALL 16 T exits reach
  retail's pads (tools/pm.py clean) -- differ/00d75060.t125_regions.cpp, DIFFER(4316v4044). Two leftovers: (1) the s0 statement
  `Executioner = Game->CreateCreature(...)`: its temporary's ~CScriptThing (f 0.25, sc 85) expands at T >= 85 (retail calls it at 0x00D752AB); a region over
  that statement, or a pragma between the expression and its `;`, also outlines GetPos and the FORCEINLINE CScriptThing::operator= (inline_depth(0) disables
  __forceinline too): 4044v4044 / 59 hunks with wrong calls; a named `CScriptThing created = ...; Executioner = created;` + region at the block end moves the two
  CCharString temp dtors before the assignment (4060v4044); inline_depth(1..4) round the statement: no change. (2) Retail tail-merges the guard-loop post pad into
  the in-loop pad (and the s0' / s3 pads into the s6 pad tail 0x00D75C97); ours keeps one extra copy -- it follows from (1) (fixed together in the 4044 variant).
- Frequency inequalities (default threshold 70..73, scores ~STB 51 + 8.44/f, ~vector 54 + 9.375/f): retail needs f(s3) > f(s6) > max(f(s4), f(s5), f(s0 temp = 0.25)).
  Under the measured loop model a counted `for` spawn loop gives s6 = 2.5 c4 > s3 = 2 c4 (while approach) and a `while` gives s6 = c4 / 4 < s4: neither ordering
  is retail's, and inline flags cannot fix it (s0 temp 85 < s6 124 for every T). The missing fact is a source shape that raises s6 alone (not s5) by ~4x
  without exceeding s3; not found. Next successor: look for a generator macro whose post-loop check sits in a one-trip loop / goto exit of its own.

### 0x00D75060 -- SUCCESSOR 4 (wfD2, Opus, ~30 inline traces + 3 grades): loop-model map completed, still PARKED (best stays differ/00d75060.cpp, 4041v4044)
- tools/ffq.sh (new): one trace -> f and decision of every STB-pad group (s3, s4, s5, s6, final loop ...) plus the in-loop FreedomFighter dtor (FF);
  tools/batch.sh runs a list of tmp/v/<tag>.cpp variants sequentially into tmp/batch.out (~30 s each). Variants kept in tmp/v/.
- Measured (c' = flow after s4 = 0.4688): `while (n < 4)` / do-while / goto loop: s5 0.2344, FF 4.22 (body x18, hot), s6 0.1172. Counted
  `for (init; cond; inc)` (any bound, `!=`, `<=`, n declared outside, n2/n10, `if (!T) {..} else return`, `{..; continue;} return;`, do-while(0)-wrapped T's,
  ref-bound FF): s5 = FF = s6 = 2.5 c' EXACTLY (all three tie for every spelling). `for (n = 0; ; ) {..; if (++n >= 4) break; }` / `while (true)` + bottom break:
  FF cold (= s5 = c'/2 or c'/3) but s6 <= s5 AND c2 emits a 17th T call (4073 bytes) -- not retail's shape. `for (;; ++n)` + break: s5 5c', FF 2.5c', s6 1.25c'.
- Retail needs f(s6) > f(s4), f(s5), f(FF), f(s0 temp = 0.25) (S/B expanded, V called at s6 only). Every loop family above gives s6 <= s5 or s6 = FF, so the
  missing fact is NOT a loop spelling of the spawn loop. Approach-loop shapes measured: while (s3 0.9375 / s4 0.4688), `for(;;){if(dist){T;break;}NSF;T}` (same
  values), while(true)+top break (0.625 / 0.625), guard-if + do-while (0.4688 / 0.703): none is return-dominant, so s4 < 0.40 with s3 >= 0.59 also has no spelling.
- Devices: dead release asserts (FABLE_RETAIL_ASSERT, in the spawn body or before s6) change NO frequency (D1..D3 identical to base). Wrapping every T check in
  `do { } while (0)` changes the model drastically (wrapped in-loop returns stop draining the loop: W5a gives s6 = 0.4688 exactly right, but s4 0.9375, s5/FF 4.2, final
  loop sites expanded) -- not a landed idiom and a dead structure, not acceptable. The /d2inlT125 + regions route is blocked by the s0 full-expression temporary
  (op= / GetPos must stay inline in the same statement; inline_depth(0) kills them; hoisting GetPos reorders the CCharString ctors).
- Next (if anyone reopens): the frequency facts say retail's FF / s5 / s0-temp are colder than s6 by a construct none of the families above has; compare with a
  landed row of ANOTHER script whose post-loop T pad is expanded while its in-loop scope-end dtor is called (search retail for `call 0x4aa840; inc; cmp; jl` followed
  by a `je` + fall-through expanded pad) and copy that row's loop text.

### 0x00D75060 -- MATCHED (successor 4 continued): RELOCATION_MATCH, test PASS, addmatch + read-only vland WIN (src/00d75060.cpp, test/00d75060.cpp)
- What closed it (all decisions were frequency facts): (1) docs/pipeline/INLINE_DEPTH_REVISIT.md lever 2/3 -- the statement macro
  `do { if (IsActiveThreadTerminating()) return; } while (0);` on the approach-loop terminate check (s3) and the spawn-loop check (s5): a wrapped
  in-loop return does not drain its loop in c2's model, so the post-spawn check s6 gets f 0.4688 = retail's pad (STB / CScriptThing expanded at score 69,
  ~vector called at 74, default threshold, no flag). (2) the spawn loop `do { ... } while (++n < 4);` -- EVIDENCED by retail destroying FreedomFighter
  (0x00D75A92) BEFORE `inc ebp` (a `while` with `++n;` in the body schedules the inc before the dtor call). (3) three scoped `#pragma inline_depth(0)`
  regions (device b, `// Inline-budget:` lines): the post-approach check (s4, f 0.94 hot after the wrap), the wrapped s5 check, and the spawn-loop scope end
  (FreedomFighter dtor); each one necessary (4361 / 4361 / 4076 without it). Wrapping s6 too is a no-op (removed); the A11 `while (true) { if (dist) { T; break; } ... }`
  approach also fixes the decisions but moves the guard-loop pad (layout, 4050v4044) -- not used.
- Devices for the coordinator: 2 wrapped terminate checks (FITTED, lever 3 precedent, retail construct unproven), 3 inline_depth regions, `CScriptThing&` bound to
  returned temporaries for Executioner / Boss / Nearest (MSVC extension, needed for the virtual IsAlive calls; landed precedent 0x00CE0920). vland WARN RELOC_ALIAS
  `_rand` -> import thunk 0x00BFEB16 (same as landed 0x00D26E60).
- The old best sources (differ/00d75060*.cpp) moved to tmp/old_differ/.
