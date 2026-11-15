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
