# wfE4 NOTES

## 0x00D45060 NScript::CQ_GuildTrainingScript::CheckFriendlyAttacks -- RELOCATION_MATCH, test PASS, addmatch + read-only vland done
- Extent: 0x00D45060..0x00D45DCE (`ret`, 3439 bytes, int3 at 0x00D45DCF; 0x00D45DD0 is the next spawned thread). The catalog's 18 KB
  was wrong; the brief's 3439 is right (oracle unchanged).
- Name: Ego_r + FableWin name sets both hold ?CheckFriendlyAttacks@CQ_GuildTrainingScript@NScript@@QAEXXZ; reloc_bindings binds it
  at 0x00D45060 from Main 0x00D3BC60 (thread spawn).
- What closed it (from the chunk-868 draft, DIFFER 3396v3439 at diag T56):
  1. drop the draft's `if (!creatures.empty())` guard (retail has no such compare) and write the SECOND re-friend loop as
     `unsigned long i = 0; while (i < AllThings.size()) { ...; ++i; }` (FITTED: exit = header/2 halves its exit-site frequency;
     with the first loop's `for` text its exits expand where retail calls them; both loops `while` = 2939 bytes);
  2. main loop `for (;; Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return; ... }` instead of
     `for (; !T; NSF)`: retail's loop-head exit pad 0x00D45CC7 is the FIRST expanded ~CScriptThing (eax), the in-loop pads
     0x00D45CEE / 0x00D45D15 / 0x00D45D5E follow as ecx / edx / eax; with the loop-condition form the loop exit is generated last,
     the pads rotate by one and two merge (3400v3439, the last 8 hunks);
  3. `// Flags: /d2inlT53` (Inline-budget line in the source): window T53..T60; default threshold expands retail-called sites
     scoring 62..72; T52 / T62 differ.
- Helper texts as the same-script CCombatApprentice::Main (0x00D4A270, style_wfD): `if (!PImp.Data) return false; return PImp.Data->...`.
- Local names from the Anniversary PDB S_REGREL32 list (Maze, AllThings x2, seh_Hero, seh_Maze, speech_movie, mymap);
  PauseMyEntitiesPlease as the sibling. HERO_ABILITY_HEAL_LIFE_SPELL (=14) from engine/EHeroAbility.h.
- Removed from the draft: the body-less `template<> std::map<...>::map()/~map()` declarations (no effect on bytes) and the
  OUTOFLINE_ASSIGN define (no such opt-in in today's headers). vland: one RELOC_NAME warning (our ~_Rb_tree at 0x00CDBFB0, an
  ICF-folded body labelled ~map / ~CCutsceneCastMap) -- not an identity error.
- Tools: tools/ex.py (per-T-check exit-pad call signature, retail v ours) found the pad rotation in one step.

## 0x00E20600 NScript::CQR_EscortTraderScript::CTraderToEscort::Main -- near miss DIFFER(14510v14510), 51 hunks (differ/00e20600.cpp)
- Extent confirmed: 0x00E20600..0x00E23EAD (last `jmp 0xe2389e`, int3 at 0x00E23EAE; 0x00E23EB0 is a separate callee). Name: Ego_r +
  FableWin ?Main@CTraderToEscort@CQR_EscortTraderScript@NScript@@EAEXXZ; vtable 0x012E151C slot 1 (draft header).
- Header: the draft needs engine/NScript_CQR_EscortTraderScript.h with `FABLE_RETAIL_CQR_ESCORTTRADERSCRIPT_TRADERTOESCORT` (the
  CTraderToEscort class) -- re-applied in hdr/ from pending/2026-11-11_linux_handH40i (addition after the existing assert).
- Steps this run (draft 14478v14510 / 119 hunks -> 14510v14510 / 51):
  1. drop FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (retail stores the base vtable 0x012C2FF4 at 0x00E206A1 / 0x00E206BB) and the
     inline_depth(0) region round the CIsThingAlive ctor (its member copy 0x004ABE90 is out of line anyway; the region only produced
     a CPredicateBase ctor call);
  2. main loop `for (;;NSF) { TT; ...}` instead of `for (;!T;NSF)`: fixes the first pad's home (0x00E208A4 kept after the third T,
     as retail) and the length (14510);
  3. scoped `#pragma inline_depth(0)` round the two `TT;` whose pad retail shares with the ~CTimer-CALLING pad 0x00E238D4
     (0x00E20CA6 random-speech T, 0x00E210E2 QUEST_STAGE_SETUP T);
  4. /d2inlT63 (T62..T65 equal; 62 hunks at default; T60 / T80 worse): exit 0x00E209D1 then calls the movie dtor as retail.
- Open residue (in retail address order): (a) predicate: retail = temps' ~CIsThingAlive expanded with nested ~CScriptThing CALLED
  and the base dtor inline; And dtor expanded with members ~CIsThingAlive 0x00E241B0 CALLED and the base inline (ebp = 0x012C2FF4
  loaded once, pushed in the prologue). Ours: depth-0 region on an explicit `~CPredicate_And<CIsThingAlive,CIsThingAlive>` spec calls
  the members (right) and ~CPredicateBase (wrong); depth(2) expands the members; with no region the temps are called (score 78).
  With T63 and no ~CIsThingAlive region the temps are exactly retail's (K2 in probe/e20). (b) 0x00E20790 / 0x00E207E0 bonus-object
  branches: ours cross-jumps before the CDefString->CCharString call (both branches push via ecx), retail after it (ecx v eax).
  (c) exit 0x00E209D1: retail calls ~CTimer 0x00CD4470 (ours expands it); a depth-0 region there outlines the pause-entities dtor
  too. (d) loop alignment fillers at 0x00E208ED, 0x00E20AFD, 0x00E20F0E, 0x00E22B5E, 0x00E233DF, 0x00E23C6F.
- probe/e20/: L1 (loop lever), V3 (best), K2 (temps right, And base called), C1/C3/E1..E6 (region variants).
### 0x00E20600 successor session (wfE4 #2)
- NEW BEST differ/00e20600.cpp: 47 hunks, equal length (was 51). Change: `FABLE_FORCEINLINE` row-local CScriptThing::GetPos (the landed
  idiom, 15 landed rows) + IsOpenDoor (was plain inline) and scoped `#pragma inline_depth(0)` round `while (!door.IsOpenDoor())` and the
  M_TraderExit `MoveToPosition(exit_marker.GetPos(),...)` statement. Evidence: retail EXPANDS GetPos at 0x00E20BBF (TotallyLost exit
  marker, f=0.0001 in our c2 trace -> no score can expand it) and IsOpenDoor at 0x00E23308, CALLS GetPos 0x004AA980 at 0x00E237F4 and
  IsOpenDoor 0x004AB000 at 0x00E23760 / 0x00E2378B (the door-wait loop, rotated) -- first-site expansions at cold sites = forceinline.
- Exit 9 (0x00E209D1, residue c): `if (T) goto Terminate;` with `Terminate:` + inline_depth(0) `return;` at the end of Main (the
  00d0a830 sibling idiom) gives retail's pad 0x00E230FD exactly (pause inline, movie 0x6E7B80 called, jmp to the shared ~CTimer-calling
  tail 0x00E238D4) and all 147 exit signatures equal (probe/e4/G1.cpp) -- BUT c2 then cross-jumps the after-loop TT pads of the
  exit-marker scope (retail 0x00E23140/0x00E23149: [Sd][push 0; pause; Md; jmp tail]) into the exit-9 pad after their `push 0`
  (ours [Sd][push 0][jmp P9]); retail keeps the copy -> 14494v14510. Retail P9 has its `push ebx` hoisted to 0x00E209D8 (same in ours).
  The three earlier depth-0 TTs can equally be `goto Terminate` (probe/e4/G4.cpp = 47 hunks, same as best).
- Predicate (a): the inline "size" is the callee's compiled COMDAT size (LESSONS_C21): at T74 with no regions ~CIsThingAlive's COMDAT
  (84 B, ~CScriptThing expanded) is called everywhere and the And dtor (33 B, members called) expands = retail's And part; retail's
  temps still need ~CIsThingAlive expanded with ~CScriptThing called. Implicit-dtor opt-ins (hdr/ CPredicate_And.h / CIsThingAlive.h,
  FABLE_RETAIL_CPREDICATE_AND_IMPLICIT_DTOR / FABLE_RETAIL_CISTHINGALIVE_IMPLICIT_DTOR, attempt only) give And members called + base
  inline at T74 (D4) but temps' ~CScriptThing expanded. Budget inliner (/Og- /d2vol10) = 15271..15511 bytes: no.
- Tried, no effect: definition order of the dtors / explicit instantiation; while->for wait loops; bonus arm-2 spellings (cast, begin()+,
  named counter, compare operand order).
- Final state (this session): differ/00e20600.cpp = 47 hunks, /d2inlT63; also dropped the depth-0 region round the inline ~CIsThingAlive
  definition (temps now exactly retail's: expanded, ~CScriptThing called, base inline). Remaining, address order: (1) And base dtor called
  (push ebp placement -> push/pop ebp hunks at exits); (2) bonus arm 2 register (counter in ecx v eax) -> early cross-jump at 0x00E207C4;
  (3) exit 9 ~CTimer expanded (goto-Terminate variant fixes it but merges pads, probe/e4/G1.cpp 14494); (4) alignment fillers (likely follow
  from 1). Overlay unchanged (attempt-only opt-in copies removed). ~150 grades + 6 c2 traces this session (probe/e4/, tmp/inl_*.json).
### 0x00E20600 third session (wfE4 #3) -- still DIFFER(14510v14510) 47 hunks; best unchanged except spelling
- differ/00e20600.cpp: bare `__forceinline` on the row-local CTimer::operator= / GetTime respelled FABLE_FORCEINLINE (byte-neutral, 47 hunks).
- (a) predicate prologue is NOT reachable under the measured regime-0 inliner with one threshold (proof, c2 traces tmp/inl_best.txt, inl_e3.txt):
  * T_Main is pinned to 62..63 by two retail facts: the loop-head exit pad 0x00E23001 EXPANDS ~CScriptThing (our trace site 41: c1 60,
    f 0.8789, sc 61) and the temps' nested ~CScriptThing is CALLED at 0x00E206AF / 0x00E206C9 (site 7/10: c1 63, sav 3, f 1, sc 63).
  * Retail's ~CIsThingAlive COMDAT 0x00E241B0 is this TU's copy (placed among this TU's COMDATs 0x00E23F00..0x00E247A0; all 275 callers
    are at or after 0x00E20684) and is 84 B with ~CScriptThing EXPANDED: in its own compile the same site (c1 63, f 1) was expanded,
    which needs T >= 64. And retail's temps expand ~CIsThingAlive although its COMDAT is 84 B (c1 78 -> needs T >= 79), while the And
    members (c1 81) are called. No single T fits; caller-size penalty is impossible (our Main starts at a+g = 6761 tuples, < 9000, and
    a penalty would add ~sz/2 and call the temps too). So retail's Main saw a smaller ~CIsThingAlive than its final COMDAT (compile order
    or some mechanism we cannot reproduce): moving the dtor definition after Main (probe/e5/X1, X2) changes nothing (c2 orders bottom-up).
  * Implicit dtors (attempt opt-ins, removed again): temps are cost-decided like the explicit dtor (called at T64, 170 hunks); no FE expansion.
  * Named predicate locals in a block, hero first (probe/e5/N4.cpp): 31 hunks but 14494 -- inside the depth-0 And spec the ~CPredicateBase
    call AND its store disappear (dead-object call removed), retail keeps the store via ebp. Not a fix, but the only variant that removes
    the base CALL.
- (c) exit 9 is a frequency fact: retail exit 8 (0x00E2309D) expands the movie dtor (sc < 63 -> f8 > 0.0581) and exit 9 calls ~CTimer
  (f9 <= 0.0392); ours f8 = 0.1099, f9 = 0.0549 (halving per TT). Retail's TotallyLost block is colder than ours by a factor in
  (0.53, 0.71]; plain if / && / else-if give 0.5 or 1 -- 0.5 makes exit 8's movie dtor called (sc 65.7). The two depth-0 TTs
  (0x00E20CA6, 0x00E210E2) are probably the same symptom (loop body hotter in ours). Tried: main loop as while(true)/do/for(;;)+NSF at the
  end (14430, 311 hunks), two-frame waits as `for (i<2)` loops (not unrolled; 107 hunks).
- ~25 grades + 2 c2 traces this session (probe/e5/).
- (c) continued -- ONE structural cause behind all three ~CTimer-called device sites (trace tmp/inl_r0.txt = best minus the two TT
  regions, T63): exit 9 f .0549, random-speech TT (0x00E20CA6) f .0551, QUEST_STAGE_SETUP TT (0x00E210E2) f .0452; retail calls ~CTimer at
  all three (needs f <= .0392). Upper bounds from sites retail EXPANDS: exit 8 movie dtor (f .1099 -> factor > .529) and the four ~CTimer
  sites at f .0708 (decisions 138-147, expanded in retail and ours -> factor > .553). So a UNIFORM loop-body factor in (0.553, 0.711]
  removes all three devices at once (5/9 = .556, the for-v-while multiplier ratio, is inside), BUT the loop-head pad 0x00E23001 must keep
  f > 0.74 (it expands ~CScriptThing, sc 61 at f .8789): the factor must start after the head TT. `for (;true;NSF)` / `for (;1;NSF)`
  (probe/e5/F1, F2) halve everything incl. the head pad (14334, 74 hunks; head pad then calls ~CScriptThing). Next: find a loop / head-TT
  spelling whose continuation is ~0.28..0.36 instead of 0.5 (a c2 frequency trace per candidate, tools/inl.sh, 67 s).
### 0x00E20600 fourth session (wfE4 #4) -- still DIFFER(14510v14510) 47 hunks; best (differ/00e20600.cpp) unchanged; parked
- New devices re-measured (WF_RULES owner's acceptance test): (c) k = 1..8 unused named locals at the top of Main (probe/e6/K1..K8):
  47 hunks each, no movement of the bonus arm-2 register residue (2). The k-device does not reach this row's ties.
- Predicate (1), new measurements (probe/e6/):
  * T80 world: with no And region, /d2inlT80 gives retail's And part by COST alone (members ~CIsThingAlive 0x00E241B0 c1 81 called,
    base inline) and expands the temps (c1 78 < 80) -- but the temps' nested ~CScriptThing (c1 63) is then expanded (A0 @T80, 107 hunks).
    A `#pragma inline_depth(1)` (or 0) region round the row-local `inline ~CIsThingAlive() {}` (D1 @T80, 59 hunks) calls the nested
    ~CScriptThing as retail but ALSO calls the nested ~CPredicateBase (retail stores 0x012C2FF4 inline). So both worlds (T63 + And
    region, T80 + ~CIsThingAlive region) leave exactly one wrong base-dtor call; no threshold / tag splits a member dtor from the base
    dtor of the same class (the epilogue calls take the inline_depth active at the closing `}`: E1/E2/E3 pragma-position probes).
  * `__forceinline` on ~CPredicateBase (probe header copy only) does not escape an inline_depth tag (P2 @T63 47, D1 @T80 59: unchanged).
  * `#pragma inline_depth(1)` round the SetPredicate statement limits only Main's direct sites (P1: And members expand, 56 hunks).
  * GFPredicateAnd (Ego_r: ??$GFPredicateAnd@VCIsThingAlive@NScript@@V12@@NScript@@YI?AV?$CPredicate_And...@ABVCIsThingAlive@0@0@Z,
    a by-value And factory beside operator&&) used directly: byte-neutral (G1 47 / G2 56). ORIGINAL_MEMBERS / ORIGINAL_ACCESS opt-ins:
    neutral (O1 47 / O2 56). ~CScriptThing defined after Main: neutral (A1/A2). QFE-4035 compiler (FABLE_VC71=$S/wine/vc71): identical.
  * Retail facts that make (1) a model contradiction, not a spelling: the predicates TU's ??_G 0x00CDF160 expands ~CIsThingAlive with
    ~CScriptThing CALLED (like Main's temps) while this TU's COMDAT 0x00E241B0 expands ~CScriptThing at the same site (c1 63, f 1);
    a caller-size penalty cannot fit (temps sz 84 would be called first: 78 + 84r < T <= 63 + 66r has no r >= 0).
- (3) under a step penalty: ~CTimer (c1 14 + pen 10) at T74 would split exactly at f 0.064 (retail: called at .0452/.0549/.0551,
  expanded at .0708) and keep exit 8's movie dtor expanded, but the same penalty calls the head pad's ~CScriptThing (0x00E23001,
  retail expands) and our Main never passes a+g 7774 (< 9000), so it is not the explanation either.
- ~35 grades this session (ATTEMPTS.log 'e6 ...'). Parked: no device or flag found that splits the base dtor from the member dtors.
### 0x00E20600 fifth session (wfE4 #5) -- still DIFFER(14510v14510) 47 hunks; best (differ/00e20600.cpp) unchanged; parked
- NEW LEVEL, c2's BUDGET inliner (GOTCHAS landing fw / landed 0x00D17A80): the session-2 probe B1/B2 ("/Og- /d2vol10 = 15271..15511")
  had NO `#pragma optimize("g", on)`, so it was an unoptimised build, not the budget path. Done properly (probe/e7/G1.cpp = best
  minus every inline_depth region + `#pragma optimize("g", on)`, flags `/Og-`): the PREDICATE PROLOGUE IS RETAIL'S BY ITSELF -- And
  dtor expanded, members ~CIsThingAlive 0x00E241B0 CALLED, base 0x012C2FF4 stored via ebp, temps expanded with ~CScriptThing CALLED,
  only frame offsets differ (0x294 v 0x290 frame). So residue (1) is a budget-path shape. But the rest of Main is 15271v14510 / 415
  hunks under the budget path: 24 partial ~CScriptThing expansions (nested ~CCountedPointer<CScriptThing> called; retail has none),
  14 partial movie-dtor expansions (retail calls 0x006E7B80 22x); /d2vol5..100 identical; an extra empty Game accessor at all 81
  Game uses (G2) makes it 17152. The heuristic world (best, T63) reproduces every call count of Main (47x ~CScriptThing, 22x movie
  dtor, 1 CTimer ctor, 3 ~CTimer), so Main is heuristic and only its predicate sites look budget-like.
- Mixed world (heuristic for Main's sites, budget for the predicate dtors) is NOT reachable: c2 takes the budget path only when the
  callee lacks c1's inline-info record, and per-definition pragmas do not remove it: `#pragma optimize("g", on)` (alone or after
  "g", off) before the ~CIsThingAlive definition = byte-identical (P1/P2 56, P3 47); a definition inside an optimize("g", off)
  region is never inlined at all (M1 ~CIsThingAlive called everywhere, 170 hunks; M3 And dtor called, 45 hunks / 14478).
  A PCH build (tools/pchg.py: head of the source as /Yc pch.h, rest /Yu) is byte-identical to the plain build (b0 / P1 lengths equal).
- Retail placement note: this TU's COMDATs (~CIsThingAlive 0x00E241B0, ??_G And 0x00E245A0, And ctor 0x00E24760) lie AFTER Main, as in
  every script TU (EmptyScript: ~And 0x00EF5B50 right after its first user Crate2::Main); our obj is bottom-up (Main last) even with
  the dtor defined after Main, and matched rows show the same, so section order is no evidence of retail compile order.
- ~20 grades this session (ATTEMPTS.log 'e7 ...'). Open: what mechanism lets retail's heuristic Main see ~CIsThingAlive at a size of
  66..68 bytes (window: temps sav 6 expand, And members sav 3 call, at T62..63) while its final COMDAT is 84 bytes.
### 0x00E20600 sixth session (wfE4 #6) -- still DIFFER(14510v14510) 47 hunks; best (differ/00e20600.cpp) unchanged
- c2 decision code read (c2.dll 13.10.3077, tmp/pdis.py): 0x1078CF54 takes the heuristic 0x1078D1AA only if [0x108A91EC] (caller global
  opts, per-function option decode 0x10774EBC) AND callee sym+0x99 (inline-info record) AND callee sym+0x69 bit 2 AND NOT bit 0x10
  (bit 0x10 = "own inline pass in progress", set 0x10772476 / cleared 0x107727DC; bit 0x2000 = forceinline accept). Otherwise BUDGET
  path 0x107284A3. Heuristic extras: caller-size penalty pen = (sz/2)*(g+a)/9000 when g+a > 9000 (0x10742BA5; our Main g 6761..7755,
  never); regime 1 (accept iff cost < inlS) when [0x108A9208] == 0 (option-word bit 23) or site flag +0x18 bit 2 (no Main site has it).
- WHAT-IF (tools/force.sh + tools/inl/force.py: gdb sets esi=0 at 0x1078CF85 for callees matching FORCE_BUDGET; diagnostic only, NOT a
  source): probe/e8/C0.cpp (= best minus the And depth-0 region) /d2inlT63 with ONLY ??1CIsThingAlive@ and ??1?$CPredicate_And@ sent
  down the budget path gives retail's predicate prologue byte-exact (frame 0x294, push ebp in the prologue, And members 0x00E241B0
  called, base vtable via ebp, temps expanded with ~CScriptThing called) and the WHOLE row drops to 13 normalized hunks (tools/hd.py):
  bonus arm 2 (0x00E207C4..0x00E207EE), exit 9 (0x00E23114 / 0x00E23167: ~CTimer expanded), alignment fillers (0x00E208ED, 0x00E233DF),
  length 14494 (exit-9 pad merge). T62 = T63; T64+ explode (98+ hunks). So residue (1) is EXACTLY "these two callees take c2's budget
  path inside a heuristic Main" -- the remaining question is only the source fact that does that per callee.
- Per-callee flags measured (tools/fl.sh, hook 0x1078CF7F; probe/e9/f1..f4): every Main callee has a record, bit 2 set, bit 0x10 clear;
  so do in-class / out-of-class inline / implicit / template (with or without `inline`) / non-inline TU / static / throw() / nothrow /
  inline_recursion / optimize("t"/"p"/"s"/"y"/"w"/"a") / inline_depth(0) / check_stack definitions. auto_inline(off) or try/catch or
  varargs remove the function from the candidate list entirely (A1/A2 = 170 hunks, never inlined). PCH with /Og- at /Yc: PCH bodies
  never inlined (35522); /Og- at /Yu: everything budget (15303). The clone routine 0x107FF615 (clears record + bit 2) never runs here.
- More flag probes (probe/e9/f6, f7): a 200-function TU before the user frees no record and sends nothing to the budget path (hook
  0x1070AC4B record free / 0x107284A3 budget entry: tools/fl5.sh); explicit instantiation, static inline, local class, block-scope
  extern: record + bit 2 as usual. ONLY a `__declspec(dllimport)` class's inline dtor arrives with bit 2 CLEAR (flags 0x18C8) -> budget
  path; sym+0x69 comes from the c1 IL (0x10706F29 / 0x10708C56 = call 0x1076E2A6), so the per-callee switch is a c1 attribute. Retail
  calls 0x00E241B0 directly (no IAT), so dllimport is not the retail spelling; noted only as the one c1 attribute found that does it.
- Session 6 total: 6 grades (fg) + ~14 c2 traces / what-ifs (ATTEMPTS.log 'e8 ...', tmp/fl*_*.json, tmp/force_*). Open question for
  the coordinator / a c1 reader: which c1 attribute (IL function flags via 0x1076E2A6, or the 0x1000 inline-info record) differs for
  ~CIsThingAlive and ~CPredicate_And<CIsThingAlive,CIsThingAlive> in retail's EscortTraderScript TU. Once that is known the row is at
  13 normalized hunks (bonus arm 2, exit 9, two fillers) -- the residues the earlier sessions already mapped.
### 0x00E20600 seventh session (wfE4 #7) -- still DIFFER(14510v14510) 47 hunks; best (differ/00e20600.cpp) unchanged; parked
- Owner devices re-measured: (c) k = 1..3 unused `long` locals inside bonus arm 2 and inside the `if (!AddedBonusObject)` block
  (probe/e11/A1..A3, O1..O3): 47 hunks each, arm-2 register unchanged (counter ecx v retail eax), as at the top of Main (session 4).
  (d) dead release asserts: no target -- no entry-store residue, and the fable_assert.h dead arm moves no static frequency (lessons).
  (b) already used (/d2inlT63 + regions); `for (;!T;NSF)` re-swept at T63/68/74 = 73/195/195 hunks (probe/e11/W1).
- Loop lever of wfE3 (`for (;!false;NSF)`, 0x00E50FD0): = `for(;true;NSF)` here, 14334 / 74 hunks with or without the TT regions (L1..L3).
- Arm 2 spellings: FABLE_RETAIL_STLP_VECTOR_INDEX_START, `.begin()[i]`, a block-scoped `long index` (I1..I3): byte-identical arm (verified).
- fwsize with FW_EXTRA=/EHsc: ~CPredicate_And, CIsThingAlive ctor, CScriptThing ctor are SAME as FableWin; operator&& is 1 instruction
  longer than FableWin's and equal to FableWin's GFPredicateAnd (53b/19i) -> FableWin's operator&& probably forwards
  `return GFPredicateAnd(p1, p2);`. Measured as a probe opt-in (FABLE_RETAIL_CPREDICATE_AND_GFPREDICATEAND: GFPredicateAnd fastcall
  inline template + forwarding operator&&): byte-neutral (G1 47 at T63; G2 without the And region 56 at T63 / 167 at T74). Not kept.
- c1 attribute search for the budget-path callees (tools/fl.sh, probe/e10/f8..f10): extern template, noinline (never a candidate),
  novtable, dllexport (in-class and out-of-class), deprecated, __stdcall dtor, uuid, code_seg(".text") / (".text$m"), align(8),
  __single_inheritance, pack(1): all record + bit 2 (0x1cea / 0x18ea). Only dllimport (class or dtor alone: 0x1ccc / 0x18c8)
  clears it. ??_G of a class inlines its dtor heuristically in our TU (callee ~TA flags 0x1cea, nested ~CP re-decided in ??_G):
  no vtable-cycle effect; c2 order is plain bottom-up (~CP, ??_GCP, ~TA, ??_GTA, ctors, user).
- ~28 grades + 3 flag probes this session (ATTEMPTS.log 'e11 ...'). Still open, unchanged: the c1 attribute behind the predicate budget
  path, bonus arm 2 register, exit-9 frequency (length 14494 in the what-if world).
### 0x00E20600 eighth session (wfE4 #8) -- NEW BEST 28 hunks (was 47), DIFFER(14510v14510), differ/00e20600.cpp (old best: probe/e12/base47.cpp)
- Bonus-object arms swapped (probe/e12/B1.cpp): `if ((unsigned long)counter < size) { TT; BonusObject = v[counter]; } else { TT; BonusObject = v[rand()%size]; }`
  (FITTED: retail lays arm 1 first with `jb` to arm 2; ours `jae` to arm 1). No more cross-jump before 0x00415D70; arm 1 is byte-equal; arm 2
  regs rotate with the arm source order (arm1-first: ptr eax / val ecx / temp edx; arm2-first: ecx / edx / eax; retail edx / eax / ecx = the
  compare block's regs). Neutral: removing the (unsigned long) cast (D1/D4), `size <= counter` (C3/D3 49), `!(a<b)` with arm 1 first (C1 47),
  `size > counter` arm 2 first (C2 29), named const CDefString& in either arm (E1/E6 47), explicit (CCharString) in arm 2 (E5 32 / 14542).
- What-if with B1 minus the And region (tools/force.sh, FORCE_BUDGET ~CIsThingAlive + ~CPredicate_And): 12 normalized hunks and LENGTH 14510
  (the old what-if was 14494): arm order, exit 9 ~CTimer, fillers only.
- Residue (a) research, c1/c2 facts (probe/e12/, tools/scan69.py = capstone scan of c2.dll for an operand regex):
  * the decision bit is sym+0x69 & 0x2 (test cl,2 at 0x1078CF90), not 0x4; normal inline 0x1cea, dllimport 0x1ccc (0x2 and 0x20 clear, 0x4 set).
    sym+0x69 is copied from the c1 IL flag word at symbol read (0x10708C4C, 0x4 masked off); the record (+0x99) is allocated at symbol read
    iff the IL flags have 0x1000 (0x10708C86 -> 0x107091B4). Both are c1 attributes fixed per compile.
  * /Og- on the command line clears 0x1000 and 0x2 for EVERY function (flags 0xcc8, no record), also PCH functions in a /Yu compile
    (s2/s3: PCH built with /O2, .cpp with /Og- [+ pragma optimize("g", on)]). #pragma optimize regions never change the c1 flags (o5, o6).
    So no mixed record/no-record world by flags or pragmas. PCH-defined functions get flag 0x1 (0x1ceb); /EHsc adds 0x104 to dtors.
  * c2 compile/emission order (o1..o8 in tmp/ord): call-graph bottom-up (leaves first, callers last) for /O2 with any of /Ob0..2, /Gy, /Zi,
    /Z7, /Gm, /EHsc, /GR, /Os, /Ot, /G6, /G7, /GF, /Zm, /GS, 400 functions or a 1.9 MB obj; only /Og- gives source order with inline COMDATs
    emitted after their first user. Retail's TU is source-ordered everywhere (Main 0x00E20600 precedes all 11 of its same-TU callees, also
    the ordinary members ForceEnableWait 0x00E23EB0 .. TravellingWrongPath 0x00E245E0), so retail's layout is not our c2's order; treat it
    as no evidence (as session 5 said).
  * PCH correction: a PCH build whose split keeps CIsThingAlive.h / CPredicate_And.h and the predicate dtor definitions OUT of the PCH
    (probe/e12/P1.cpp, split at `// PCHSPLIT`) is NOT byte-identical: 187 hunks (And members ~CIsThingAlive expanded, And temp slot 0x1c0);
    non-PCH P1 = 56. Session 5's identical result used another split. tools/pchg.py BUG: `/Fp<linux path>` is ignored by cl under Wine, the
    PCH is written as pch.pch in the CWD (it wrote /home/user/FableDecomp/pch.pch; moved out to tmp/pch/x.pch) -- run it from $W/tmp or
    pass /Fp as Z:\... .
- Constructs probed for the 0x2 bit (all keep it): use-before-definition inline / non-inline dtors, __inline, anonymous namespace, out-of-class
  non-inline dtor defined after the user (0x146a), PCH-defined, templates instantiated after a PCH, pragma optimize("g", off) around the
  definition or at the end of the TU.
- ~12 full grades + ~20 flag/order probes this session (ATTEMPTS.log 'e12 ...').
- On the new best: /d2inlT62 = T63 = 28, T64 182 (14494), T74 180. `goto Terminate` for exit 9 (T2) or for exit 9 + the three depth-0 TTs (T1)
  still merges pads: 14478, 44 / 34 hunks. Block-local `index` in arm 1 / both arms (F1..F3): neutral. Rotation trace of the arm-1-first
  source: tmp/rot_base47.txt (tools/C44/c2prio_f.py --rotation, ~6 min): arm 2 starts with the rotating pointer at eax, retail's arm 2
  needs it at edx (one step earlier / two later). Arm 1 and the compare block are byte-equal, so the missing rotation step is a codegen-order
  or tail (lea/push after 0x00415D70) difference; next: trace B1 the same way and count temps from the compare block to arm 2 in both orders.
- Next ideas, (a): only a c1 reader can name the attribute (IL flag 0x2 / 0x1000 writer in c1xx.dll); everything per-source is measured out.
- Rotation traces read (tmp/rot_base47.txt, tmp/rot_B1.txt; events are keyed by the arm's TT line: then-arm = 110, else-arm = 114):
  arm 2's codegen order is temp-address, PMasterData, counter, pGlobalScriptDef, begin, this. Ours (arm 1 first) gives edx, eax, ecx, edx,
  eax, ecx; retail's arm 2 is ecx, edx, eax, ecx, edx, ecx -> retail's rotating pointer at arm-2 start is ecx, ours edx: retail has ONE
  FEWER rotation temp (or two more) between the compare block and arm 2, with arm 1's visible code identical (ours: eax, ecx, eax [edx busy,
  skipped], ecx). Candidates: arm 1's post-call `lea ecx,[esp+0x98]; push ecx` generated inside arm 1 in retail (its own tail) but in the
  join block in ours, or the hoisted `mov ecx,esi` before the jb. Next: find a spelling that keeps the CCharString temp per arm (e.g. the
  conversion + assignment as one expression per arm that c2 cannot sink) and re-trace (6 min per trace).
### 0x00E20600 ninth session (wfE4 #9) -- residue (b) CLOSED structurally; differ/00e20600.cpp = DIFFER(14526v14510) 43 hunks (q.py), 9 normalized hunks in the predicate what-if world
- NEW BEST STRUCTURE (differ/00e20600.cpp, probe/e13/DR.cpp): bonus-object block `if ((unsigned long)counter >= size) { TT; BonusObject = v[rand()%size]; Game->AddItemToContainer(Me,BonusObject); } else { TT; BonusObject = v[counter]; Game->AddItemToContainer(Me,BonusObject); }`
  -- the rand arm first gives retail's `jb` layout (EVIDENCED by the layout), the AddItemToContainer written in BOTH arms is FITTED (GOTCHAS Q24
  tail-duplication; c2 tail-merges it back into ONE join, no code added). Both arms and the join are now byte-identical to retail 0x00E20781..0x00E20834.
  The q.py count went 28 -> 43 and 14510 -> 14526 only because the predicate residue's 1-byte-early `push ebp` now shifts every loop-alignment
  filler (the old 28-hunk B1 source happened to compensate). In the what-if world (tools/force.sh, FORCE_BUDGET='??1CIsThingAlive@,??1?$CPredicate_And@'
  on probe/e13/DRn.cpp = DR minus the And depth-0 region, /d2inlT63) the row is 14510v14510 with ONLY exit 9 (0x00E23114 / 0x00E23167) and the
  filler 0x00E233DF left (tools/hd.py: 9 hunks, the others are `ret`/`nop` normalisation lines). Old 28-hunk best kept as probe/e13/base.cpp.
- Why it works (c2prio_f.py --rotation, tmp/rot_R1.txt): every arm value is a rotation temp (pointer reset per function, all 642 events "rotation").
  With the rand arm first the v[counter] arm started at edx (retail ecx). The pointer after an event is fixed by the register it got, so retail's rand
  arm had to END on an eax event: its post-call `lea ecx,[esp+0x98]; push ecx` is not a rotation event in retail. Duplicating the trailing statement
  reorders c2's tail-merge blocks so that push leaves the rand arm's codegen. 30+ spellings of the arms / condition / index / conversion
  (begin()[], *(begin()+i), named index / size / const CDefString&, casts, operator[] / operator CCharString() calls, GetMasterData() accessor (Ego_r
  has ?GetMasterData@CTraderToEscort@...; byte-neutral), bool flags) never moved the arm-2 start: only block structure did.
- (c) exit 9 measured in the what-if world: `if (true && ParentClass.TotallyLost)` (c2 if-probability .25) fixes exit 9 but makes exit 8 CALL the
  movie dtor (14478); `|| false` (.75) is too hot (14606). Needed then-probability of the TotallyLost test in (.2555, .3565] (exit 8 movie sc < 63
  needs f8 > .0556; exit 9 ~CTimer called needs f9 <= .0392); plain if = .5. Head-TT/loop restructures (M1..M3 `TT; for(;;){...NSF;TT;}` /
  while / do: 14462, 103 hunks; M4 `while(!T)`: 311) fix exit 9 but cool the head pad (its ~CScriptThing then called) and exit 8.
- T stays 63 even in the what-if world: the temps' nested ~CScriptThing inside the budget-inlined ~CIsThingAlive is still decided by the heuristic
  (c1 63, f 1 -> called only at T <= 63); what-if at T66 / T70 / T74 = 113 hunks.
- Hidden c2 switches swept on the no-And-region source (T63): only /d2inld<N> (inline depth) changes anything (d1 262, d2 111, d3 80, d10 85, d40 126
  hunks; d0 / d5 = default); /d2inlnild, inlniln, inlnold, inlnoln, inlocsa1..4, inlfcsa, inlflcsw, inlfcsw, inlcrmax, inluserinl, inlnobr, inlnlw,
  isize: byte-identical; /d2il fails to compile. In the budget world (/Og- /d2vol10 + optimize("g", on)) isize / inlS / inlT / inld: identical (15303).
- Budget world cannot be Main's: under the budget path a callee <= 40 IL is always expanded, so every ~CScriptThing exit is (partly) expanded
  (tmp/ex_bud.txt) while retail calls 0x004AA840 at 47 sites. Main is heuristic; only the two predicate dtors look budget.
- Contrast evidence: landed CCrate2::Main 0x00EF5670 (EmptyScript TU, no flags) has the same predicate with temps CALLED (0x00E241B0) and the And
  dtor expanded, base store immediate = plain heuristic at T74; EmptyScript's own ~CPredicate_And COMDAT is 0x00EF5B50 (reloc_bindings). So the
  budget-path predicate is particular to the EscortTrader TU.
- Emission order probe (tools/ord.sh): /O1 /O2 /Ox /Og /Ob0 /Gh /Ge /Gs0 /GT /Fr all emit leaves first (a leaf defined after Main precedes it); only
  /Og- is source order -- retail's layout (Main before its callees, COMDATs after first user) matches /Og- emission, but /Og- for the whole TU is
  ruled out above.
- ~70 grades + 8 what-ifs + 1 rotation trace this session (ATTEMPTS.log 'sw ...', probe/e13/). Next: (a) still needs the c1 attribute (unchanged);
  (c) find a natural TotallyLost-test / loop shape with then-probability ~.3 (c2 frequency trace per candidate, tools/inl.sh).
- Exit-9 lead for the next session: with X5 (`true &&` on the TotallyLost test = an extra x0.5 on that block only) the what-if world's ONLY
  non-filler residue is exit 8's movie dtor being called (sc 65 = c1 19 x (18+6)/7). If retail's ~CScriptGameResourceObjectMovieBase costs c1 18
  (one IL unit less than our header text: sc 61.7 < 63), a plain halving on the TotallyLost block would close exit 8 AND exit 9 together. So:
  (1) fwsize the movie dtor / its base / CCountedPointer<MovieBase> texts against FableWin, (2) look for a natural statement that halves the
  TotallyLost block (an extra guarded statement or nested if on the block entry, e.g. a `TT` structure) -- `true &&` itself is only a probe.
  The four ~CTimer sites at f .0708 that retail expands are outside the TotallyLost block (X5 left them right).
  fwsize (FW_EXTRA=/EHsc): ~CScriptGameResourceObjectBase, MovieBase ctor, ResourceObjectBase ctor, CCountedPointer<MovieBase> ctor SAME as FableWin; ~CScriptGameResourceObjectMovieBase has no FableWin copy (no size evidence either way).
### 0x00E20600 tenth session (wfE4 #10, successor) -- best unchanged (differ/00e20600.cpp, DIFFER(14526v14510) 43 hunks); probes in probe/s10/
- Tools: tools/s10/qp.sh <src> [flags] (q.py grade + exit-8/9 pad summary via tools/s10/pads.py), tools/s10/leaf.py <src> <flags|-> <leaf..>
  (compile, print a COMDAT's size + calls).
- (a) predicate, new measurements: FABLE_FORCEINLINE on the row-local ~CIsThingAlive (F1) = base (43); forceinline ~CIsThingAlive or the
  And spec without the depth-0 region (F2/F3) = N1 (no region) 53 hunks: And members ~CIsThingAlive EXPANDED (slot 0x1c0). Our
  ~CIsThingAlive COMDAT is 25 B (~CScriptThing CALLED) at T63 and also in the landed same-TU sibling CM_BanditTrigger::Main 0x00E1E960
  (default flags; its temp ~CIsThingAlive is expanded with ~CScriptThing called, like ours) -- retail's 0x00E241B0 is 84 B (expanded).
  Same-TU fact: landed Fisticuffs / Fisherman Mains get retail's And shape (members called, base inline) only because their sources leave
  ~CIsThingAlive without a visible body; this row cannot (retail expands its temps).
- (c) exit 9, new measurements: condition spellings `== true` / `== 1` (C1/C4: cmp byte,1 -- not retail), `!= false`, `(int)x > 0`: pads
  unchanged. Loop-head spellings W1 `if (T) return; else {body}`, W2 `if (!T) {body} else return;` (14494), W3 `if (T) break;`: pads
  unchanged; W5 `TT; do {body} while (false);` 15454; W6 `TT; if (!T) {body}` (redundant guard, a dead second T call -- not usable): exit 9
  right (jmp tail) but exit 8 then calls the movie dtor (factor .5 too cold, as `true &&`).
- (c) inline control: `#pragma inline_depth(0)` round the exit-9 TT (P1) gives retail's pad shape (movie called, jmp to the shared
  ~CTimer tail) EXCEPT the pause dtor is then CALLED (retail expands PauseAllNonScriptedEntities(false), vtable 0x5EC, at all 32 sites;
  ours too). A probe opt-in FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_DTOR (tmp/hdr_s10/hdr, NOT in hdr/) is byte-neutral
  without the region (B2 = base) and does NOT escape the in-body depth-0 tag (P2 = P1). So no inline control isolates ~CTimer here.
- (c) more: `if (T) break;` for exit 9 (X1) = base (c2 treats break-to-end like return); X1 + depth-0 (X2) 14574. TotallyLost block as
  `switch (b) { case true: {...} }`, `switch (b) { case false: break; default: {...} }`, `while (b) {...; break;}`, `if (!b) {} else {...}`
  (S1..S4): all byte-identical to base (c2's static frequency is flow-graph only; .5 per two-way branch, no opcode heuristic).
  `#pragma auto_inline(off)` round the row-local ~CTimer definition (Q2, diagnostic): exit 9 pad becomes retail's (6 insns, jmp tail) but
  ~CTimer is then called at exit 8 and the four f .0708 sites retail expands (14350). auto_inline inside a function body: C4177 (SRC_FAIL).
- (a) more: named predicate locals in a block (N4, session 3's idea re-measured on today's best): 44 hunks / 14510 -- the And's base
  ~CPredicateBase call disappears but so does its store AND the hero local's base store (dead-store removal at block end); retail keeps
  all base stores (via ebp). N5 (N4 without the And depth-0 region): 53. Not a fix.
- Conclusion this session (~35 grades/probes, probe/s10/): every new device (forceinline on the predicate dtors / pause dtor, in-body
  inline_depth / auto_inline at exit 9, k-locals and asserts do not apply to inline/frequency residues) is measured out. Both open
  residues are unchanged: (a) needs the c1 attribute that sends ~CIsThingAlive / ~CPredicate_And<CIsThingAlive,CIsThingAlive> down c2's
  budget path inside a heuristic Main; (c) needs a loop-body frequency factor in (0.553, 0.711] after the head TT (no natural spelling found;
  powers of .5 / .75 from if / && / || and every if/switch/while/else shape give .5 or 1). Parked.
- (a) NEW FACT (callee-body visibility level): retail's TU emits ALL CPredicate_And<CIsThingAlive,CIsThingAlive> members as COMDATs
  (ctor 0x00E24760, operator() 0x00E23F00, Clone 0x00E247A0, ??_G 0x00E245A0), so their bodies were visible to retail's c2. Defining
  them in the row with the real texts (landed 0x00E7E020 / 0x00E7E060 / 0x00E7DE30 idioms: `: Predicate1(predicate1), Predicate2(predicate2) { }`,
  `return Predicate1() && Predicate2();`, `return new CPredicate_And(*this);`): operator() + Clone visible (V3) = byte-neutral (43);
  the visible ctor (V1/V2) is EXPANDED into Main at T63 (74 hunks, 14558; V4 without the And region 55 / 14574) while retail CALLS
  0x00E24760. So a THIRD And member behaves non-heuristically in retail (budget path: > 40 IL -> called), consistent with the what-if
  (every CPredicate_And<CIsThingAlive,CIsThingAlive> member + ~CIsThingAlive off the heuristic). Our row keeps the ctor declared only
  (header default) -- that is a model gap the lander accepts today (callee retail calls, no visible body), noted for the coordinator.
  Lead for a c1 reader: what the predicate template instance and ~CIsThingAlive share that the engine inlines (CScriptThing, CTimer,
  movie, pause) do not -- the most plausible build fact is the PCH boundary (engine headers in the PCH, flag 0x1; the predicate headers
  after it), which a single-file row cannot express (session 8: a PCH split moves 187 hunks, measured with the /Fp bug noted there).
- Untested lead (context ran out): a single-file PCH world, `/Yc` + `#pragma hdrstop` after the engine includes (predicate headers and
  the row-local predicate definitions after it), compiled from $W/tmp so the .pch lands there (cl writes it into the CWD; never the repo).
  If the /Yc compile gives the pre-hdrstop functions flag 0x1 and the post-hdrstop predicate members take the budget path, it is
  expressible as a per-row flag; it needs the coordinator's OK (a PCH flag is a build fact, not a byte-evidenced c2 switch).
- Session 10 total: ~38 grades / probes. Overlay hdr/ unchanged (the pause-dtor opt-in lives only in tmp/hdr_s10/hdr as a probe).

### 0x00E20600 eleventh session (wfE4 #11) -- best unchanged (differ/00e20600.cpp, DIFFER(14526v14510) 43 hunks); parked again
- Baseline re-graded (tools/s10/qp.sh, 1.6 s): 43 hunks, exit-8 pad 24 insns, exit-9 pad 20 insns (~CTimer expanded), as session 10.
- NEW TOOL tools/fq.sh <src> <tag>: c2 inline trace of a tiny standalone TU's F1 at /d2inlT63, printed as callee:f:r:sc per site
  (seconds per run; probe sources in probe/s11/fq/, base.h = distinct inline X0..X9 so every TT pad is one named site). Lesson: a probe
  .cpp must be self-contained (inl_host copies only the .cpp; a missing #include makes cl spin until the 1500 s timeout).
- c2 static frequency model, MEASURED (probe/s11/fq): every conditional branch .5, joins sum (flow-conserving), `if (T) return` keeps .5;
  a `for(;;)` loop whose only exits are returns has head = 10 x entry (f4: entry .5 -> head-pad 2.5; the row: entry .1758 -> head pad .8789);
  a `while (c)` loop: body = entry, exit = entry x .5 (c1/c2); int switch: 1/(cases+default) per target (c3: 1/4); a switch on a bool
  collapses to .5 whatever the case order (e1..e3); `x ? true : false`, inline bool helpers (both spellings) fold to .5 (d1, d5, d6);
  `x || false` .75 and `x && true` .25 (constants are NOT folded before the estimate, d2..d4). Default /d2inlT is 74 (fqdef).
- PROOF that residue (c) cannot close by any spelling at /d2inlT63 with today's callee texts: score = c1 x (6 + 1/f) / 7 (fits every
  traced site). Retail facts: head pad 0x00E23001 expands ~CScriptThing (c1 60) -> f_hp > .7407; exit 8 0x00E2309D expands the movie
  dtor (c1 19) -> f8 > .0581; exit 9 0x00E230FD calls ~CTimer (c1 14) -> f9 <= .0392, and f9 = f8 / 2 (one straight TT apart). In
  retail's straight-line structure (head TT; if (TotallyLost) {TT; ...; NSF; TT8; NSF; TT9}) f8 = f_hp / 8, so f_hp <= .627 --
  contradicts f_hp > .7407 for EVERY loop-entry frequency. Closing (c) needs an extra flow factor in (.529, .714] between the head-TT
  continue edge and exit 8 that emits no code; the only code-free factors are .5 / .75 / .25 / 1/n(int switch) -- none in range
  except contrived compounds. The c1 values are pinned by retail COMDAT sizes (~CTimer 0x00CD4470 = 20 B = our sz 20; movie dtor implicit,
  25 B). So the uniform-threshold world (/d2inlT63) is itself not retail's mechanism for this row: (a) and (c) are both artefacts of
  that emulation, not spelling problems. Same signature as the batch's other row (0x00D45060 needed /d2inlT53, default T74).
- Generalised to ANY uniform /d2inlT<T>: head pad needs f_hp > 1/(7T/60-6); exit 9 needs f8 <= 2/(T/2-6); with f8 = f_hp/8 these
  are compatible only for T > 65.9, while the temps' nested ~CScriptThing (c1 63, f 1, CALLED at 0x00E206AF / 0x00E206C9) needs T <= 63.
  So no per-row flag value, and no loop / pre-loop spelling, can give retail's pads; only a code-free extra factor in the TotallyLost
  path (none natural) or a different inliner mechanism.
- Hypothesis left for a c2 reader (unifies (a) and (c), untested -- needs c2 internals): retail's TU was compiled in SOURCE order (its
  layout: Main before all same-TU callees, COMDATs after their first user, which only /Og- reproduces in our c2), so when Main was
  compiled its later-defined inline callees had no compiled size yet (sz = IL estimate, not the bottom-up COMDAT size the trace shows).
  That would explain the per-row T fits (53 / 63) and the temps-expanded / And-members-called split of (a) (~CIsThingAlive compiled
  84 B, as retail 0x00E241B0, only AFTER Main). Our c2 compiles bottom-up for every flag set but /Og- (session 8), so no row-level
  device reproduces it.
- Probes this session: L1 (`for (int wait=0;wait<2;wait++) {NSF;TT;}` for exits 8/9: not unrolled, wrong), 20 fq traces. ~25 grades/traces.

### 0x00E20600 twelfth session (wfE4 #12, successor) -- RESIDUE (c) SOLVED IN PRINCIPLE: `for (;true;NSF)` at DEFAULT flags (T74)
- fq probes (probe/s12/fq): `for (;true;NSF)` / `for (;1;NSF)` give the loop head HALF of `for (;;NSF)` (head pad 1.25 v 2.5 for entry .5):
  the constant condition is estimated as a .5 exit branch before it is folded (no code emitted). `while (true) {..NSF;}`, `for (;;) {..NSF;}`,
  goto loops give a flat ~.06 everywhere (other regime, head pad cold); a `break` exit, `continue`, `for (int i=0;;i++,NSF)`: same as for(;;NSF).
- With the loop entry halved the head pad / exit 8 / exit 9 bounds are all satisfied at T in [71, 84.8): default /d2inlT74 is inside.
  Probe h1 (extra TT before the loop, T74): exit 9 pad = 6 insns jmp shared tail 0xe238d4 (retail's), exit 8 movie expanded.
- WHAT-IF (tools/s12/force2.sh = force.sh + FORCE_REJECT_F1: reject f=1 sites of a callee; tools/inl/force2.py): probe/s12/t1n.cpp
  (= probe/e13/DRn.cpp with `for (;true;NSF)`) at /d2inlT74 with FORCE_BUDGET ~CIsThingAlive + ~CPredicate_And and the f=1 ~CScriptThing
  sites rejected: 14510v14510, 8 hd.py "hunks" that are all ret/nop normalisation lines (identical text) -- i.e. byte-equal modulo (a).
- Real compile probe/s12/t1.cpp (differ + `for (;true;NSF)`) at T74: 14494v14510, 170 q.py hunks (predicate region at T74: temps' nested
  ~CScriptThing expanded etc.). So the row now reduces to residue (a) at default T. Next: verify the rejection set (9 rejections logged; expect 2
  in Main), re-measure (a) at T74, strip the T63-era devices, cross-check 0x00D45060 with a constant loop condition.
- Rejection set verified (FORCE_REJECT_AFTER='??_GCIsThingAlive@'): exactly the 2 temporaries' nested ~CScriptThing sites in Main; result
  unchanged (14510v14510, the 8 hd.py lines are 'ret '/'ret' text artefacts of hd.py's normaliser). What-if T window: T72 = T74 = clean;
  T70 37 hunks (14350), T80 / T84 24 hunks (14590).
- (a) at T74 needs BOTH devices of the what-if: budget-only 98 hunks (14654: temps' nested ~CScriptThing expanded), reject-only 188 / 178.
- Device strip in the what-if world (probe/s12/strip, tools/s12/wi.sh): the three in-body depth-0 TT regions (random-speech TT, SETUP TT,
  answer==1 TT) are NOT needed any more (each and all removed: still clean). Still needed: the door-wait and M_TraderExit depth-0 regions
  (17 / 13 hunks without), FABLE_FORCEINLINE on CTimer::operator= / GetTime / CScriptThing::GetPos / IsOpenDoor (22 / 16 / 15 / 14 without).
  Trace (tmp/inl_q3.txt, T74): the expanded GetPos / IsOpenDoor sites 0x00E20BBF / 0x00E23308 sit at c2 frequency ~1e-8 (sc 3e4 / 2e8):
  no heuristic expands them, so retail's mechanism there is not the score (forceinline, or the budget path "<=40 IL always expanded" with
  the budget spent by the end of Main, where the door-wait / exit sites are CALLED) -- the same path that residue (a) points at.
- NEW BEST differ/00e20600.cpp (= probe/s12/r1.cpp): for(;true;NSF) (FITTED), DEFAULT flags (no /d2inlT), three depth-0 TTs removed, NEW
  depth-0 region round the row-local ~CIsThingAlive definition (temps' member ~CScriptThing CALLED as retail, base also called).
  q.py 43 hunks / fg 45 at default flags, 14526v14510: every hunk is residue (a) -- prologue frame offsets -4, the 3 ~CPredicateBase calls
  where retail stores 0x012C2FF4 via ebp, push/pop ebp order at every exit, loop-alignment fillers. Old best (needs /d2inlT63, residues
  (a)+(c)): probe/s12/old_differ_s11.cpp (169 fg hunks at default, 43 at T63).
- (a) probes s12: forceinline ~CPredicateBase (temp overlay tmp/hdr_s12) inside the depth-0 regions: unchanged (does not escape depth 0);
  in-body inline_depth(1) round SetPredicate: byte-identical to none; ~CScriptThing definition before ~CIsThingAlive / after Main:
  unchanged; named predicate locals in a block: 44 (with region) / 166 (without).
- Cross-check 0x00D45060 (probe/s12/k1.cpp, main loop for(;true;NSF)): RELOCATION_MATCH only at /d2inlT64 (default 3564v3439; T53..T60
  3327) -- so the constant condition does NOT give that row a default-T story; for(;true;NSF) stays FITTED here (src/00d45060.cpp untouched).
- (a) is the only open residue: the c1/c2 fact that sends ~CIsThingAlive / ~CPredicate_And<CIsThingAlive,CIsThingAlive> (and, in Main,
  the temporaries' nested ~CScriptThing; and probably GetPos / IsOpenDoor) off the heuristic is still unknown. ~40 grades / traces this session.
- Leaf check (tools/s10/leaf.py, default flags): the row-local ~CIsThingAlive COMDAT is 84 B without the new depth-0 region (= retail 0x00E241B0, 84 B; 25 B at /d2inlT63) -- independent support for default T; with the region it is 25 B (side effect on that COMDAT only).
## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).
### 0x00E20600 thirteenth session (wfE4 #13) -- best unchanged (differ/00e20600.cpp, DIFFER(14526v14510), fg 45 hunks at default flags); parked
- Residue (a) SHARPENED (probe/s13/dxx.cpp = best minus both predicate depth-0 regions, default flags, prologue read from tmp/gq_dxx.txt):
  with no predicate regions the And dtor is ALREADY retail's (expanded, members ~CIsThingAlive CALLED at sc 81, base vtable stored inline);
  ONLY the two CIsThingAlive temporaries are wrong (CALLED at sc 78 because the compiled ~CIsThingAlive COMDAT is 84 B with ~CScriptThing
  expanded). Retail expands the temps with a body shaped like UNinlined ~CIsThingAlive (~CScriptThing called, base store inline) while its
  own COMDAT 0x00E241B0 is the 84 B inlined form. Since c2 inlines a callee's COMPILED body (bottom-up) and re-decides only the calls left
  in it, no uniform T and no depth tag gives that: T in (78,81] expands the temps but with the 84 B body (dxx T79/T80/T81 = 14750, 105
  hunks, the nested ~CScriptThing expanded inline); a depth-0 tag on the ~CIsThingAlive definition also blocks the base dtor (3 calls).
- Measured nulls this session (~50 grades, probe/s13/, ATTEMPTS.log 'gq ...'/'s13 ...'):
  * region depth sweep on the two predicate definitions (d<CIsThingAlive><And> in {0,1,2,none}): depth 1 or 2 = no region (171..184 fg hunks);
    only depth 0 on ~CIsThingAlive gives the 25 B body; nothing between.
  * in-body `#pragma inline_depth(1)` / `(2)` round the SetPredicate statement (b1/b2, no def regions, T80): byte-identical to none (the nested
    ~CScriptThing is already part of the compiled 84 B body, so Main has nothing left to block); in-body depth 0 (b0) blocks everything (86 hunks).
  * hidden c2 switches at default flags on dxx: /d2inlld0..20 (not in session 9's list), /d2inlnild, niln, nold, noln, fcsw, flcsw (0/1/3/50):
    all byte-identical (177 gq lines). /d2inlS0 / S5 = default; S20 / S60 only expand more (15076+).
  * `#pragma optimize("s", on)` / `optimize("t", off)` round the ~CIsThingAlive definition (o1/o3): its body still inlines ~CScriptThing and the
    temps expand FULLY (14622, 96 hunks); also round the And spec (o2): the And dtor becomes CALLED (14830, 148).
  * PCH world (tools/pg.sh = tools/pchg2.py run from tmp/pch, the .pch stays there): splits before CIsThingAlive.h, before the row-local
    definitions, and before Main are all BYTE-IDENTICAL to the non-PCH build of the same source. This closes session 10's PCH lead; session 8's
    187-hunk PCH result came from another source / split, not from the PCH boundary itself.
- Third-source fact (Anniversary 360 PDB types, tmp/anniv_types.txt): CIsThingAlive and CPredicate_And<CIsThingAlive,CIsThingAlive> have
  USER-declared virtual dtors and user ctors; only the copy ctor, operator= and __vecDelDtor are compiler-generated; Thing / Predicate1 /
  Predicate2 are PRIVATE (engine/CPredicate_And.h has them public: access does not change bytes, noted for the coordinator).
- What would close (a), for a c1/c2 reader: a mechanism that lets heuristic Main inline ~CIsThingAlive's PRE-inlining body (session 6: the
  budget path, taken when the callee lacks an inline record or has sym+0x69 bit 0x10 "own inline pass in progress"). Every per-source,
  per-flag, region, pragma and PCH device is now measured out. Parked.
### 0x00E20600 fourteenth session (wfE4 #14, successor) -- best unchanged (differ/00e20600.cpp, DIFFER(14526v14510), fg 45 hunks at default flags); parked
- Re-graded baseline (probe/s14/base.cpp = differ): DIFFER(14526v14510), gq h=173; dxx (no predicate regions) DIFFER(14494v14510) h=177.
- Retail fact: in all of Main ~CScriptThing is EXPANDED at exactly ONE site, the loop-head pad 0x00E23001 (vtable 0x01238C8C store at
  0x00E23009; the other 0x01238C8C store, 0x00E20724, is the exit_point default ctor), and CALLED (0x004AA840) at 47 sites.
- Cross-TU survey of 40 landed single-temp rows `SetPredicate(CIsThingAlive(Me))` (tmp/single.txt): every row in a TU BEFORE
  0x00E20600 (0x00D2.. .. 0x00E0F8A0) has the temp ~CIsThingAlive EXPANDED with ~CScriptThing CALLED (the shape our two temps need);
  every row AFTER 0x00E241B0 (0x00EB.., 0x00ED..) CALLS 0x00E241B0 (consistent with the 84 B COMDAT at T74). The "before" rows that
  define ~CScriptThing inline (0x00D8BCD0, 0x00D706E0; 0x00D706E0 even expands ~CScriptThing once at 0x00D71146) get the temp shape
  only from a depth-0 region round ~CIsThingAlive PLUS FABLE_RETAIL_CPREDICATEBASE_NOVTABLE -- in those TUs retail has NO base
  vtable store (0x012C2FF4 absent in both functions), so NOVTABLE fits there. Here retail stores 0x012C2FF4 via ebp at both temps
  and the And temp, so that device (= session 1 / 12's depth-0 region, base dtor then called) does not apply. Lead closed.
- Probes p1 / p2 (dxx + a free function BEFORE Main that destroys a CIsThingAlive temp / an And of two, i.e. a preceding same-TU user
  like CM_BanditTrigger::Main 0x00E1E960): byte-identical to dxx (h=177). A preceding user does not change Main's decisions.
- Third source (Anniversary 360 PDB, module 142 = Unity_spawned_functionEtAl.cpp.obj; tmp/m142_lines.txt, tmp/anniv_syms.txt):
  the 360 Main (code size 18948, heavily edited: teleportTrader, FarmDoors, OR_GoodHSP locals) has the predicate as ONE statement on
  qr_escorttrader.hpp line 515 (0xB65A28..0xB65B3C: And ctor call, SetPredicate, ~And call, two ~CScriptThing calls for the temps);
  confirms `SetPredicate(CIsThingAlive(Me) && CIsThingAlive(GetHero()))` as one full-expression; no decision evidence for VC7.1.
- ~5 grades this session. Nothing new to try at source level; (a) still needs the c1/c2 fact (unchanged from session 13).
