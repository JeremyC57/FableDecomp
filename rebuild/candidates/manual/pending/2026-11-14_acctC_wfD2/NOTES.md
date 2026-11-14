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
