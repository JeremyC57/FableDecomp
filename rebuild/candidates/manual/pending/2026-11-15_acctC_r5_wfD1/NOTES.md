# wfD1 NOTES (acctC lane, quest-script coroutine rows)

## 0x00D82640 NScript::CQ_HeroSoulsBriarScript::CBriarRose::Main -- RELOCATION_MATCH, test PASS, vland PASS, addmatch done
- Identity: Ego_r + FableWin name sets; slot 1 (0x012D42FC) of entity vtable 0x012D42F8 (landed CBriarRose::Alloc 0x00D81630).
- Extent confirmed 3260 bytes (0x00D82640..0x00D832FB ret), no EH frame.
- What closed it (from the earlier lane's DIFFER(3286v3260)):
  1. the landed predicate lever (chunk H40; landed CFishermansWife::Main 0x00D8BCD0 and 19 other rows): FABLE_RETAIL_CPREDICATEBASE_NOVTABLE +
     `#pragma inline_depth(0)` round the row-local inline CIsThingAlive ctor and dtor definitions, real inline CScriptThing copy ctor / dtor
     bodies in the row (FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY dropped). 3286 -> 3270, one hunk left.
  2. closing idle loop spelled as the landed same-family twin CHeroThunder::Main 0x00D7B510 (`if (T) return; for (;;) { NSF; if (T) return; }`):
     retail 0x00D832C4 `jmp 0x00D832CE` (pre-loop test tail-merged into the loop test). while/for/do/break/for(;;NSF) spellings all duplicate
     the test (+10). Found with a byte search of retail for the jmp-to-test pattern: 3 sites (0x00D7C2A5 Thunder, 0x00D832C4 this row, 0x00DDC2A0).
- Shared enum headers engine/EHeroAbility.h + engine/ETextGroupSelectionMethod.h replace two local enum copies (still RELOCATION_MATCH).
- Devices: Inline-budget pragma (landed lever, owner rule 2026-11-08). No flags. No header changes.

## 0x00DEA3F0 NScript::CQ_RansomVictimScript::CVictimsBrother::Main -- RELOCATION_MATCH, test PASS, vland PASS, addmatch done
- Identity: Ego_r + FableWin name sets; slot 1 (0x012DD204) of entity vtable 0x012DD200 (landed CVictimsBrother::Alloc 0x00DE1E60).
- Extent confirmed 3076 bytes (0x00DEA3F0..0x00DEAFF3 ret).
- What closed it (earlier DIFFER(3056v3076)): (1) predicate lever (as row 0x00D82640) -> 3070, block placement left; (2) first loop
  `while (!ParentClass.RescuedVictim)` instead of `for(;;){ if (Rescued) break;` (fixed the marker / CScriptThing slot swap and moved the
  four first-loop exit stubs to retail's place) -> 3058; (3) second loop in the generator form of the landed same-TU sibling
  CVictimsBrotherHelper::Main 0x00DEB280: `for (; !false; Game->NewScriptFrame()) { if (T) return; ... }` (its rotated head test is the
  post-minimap check) -> RELOCATION_MATCH; returns as `TRetVoid _Fail_Val; return _Fail_Val.Return();` (sibling idiom; plain return also matches).
- Names: local enums replaced by engine/EHeroAbility.h, ETextGroupSelectionMethod.h, EScriptEntityMoveType.h; the draft's made-up
  enumerator MOVE_TYPE_RUNNING is ENTITY_MOVE_RUN (shared header). EScriptAIPriority stays row-local (as landed script rows).

## 0x00D7FE90 NScript::CQ_HeroSoulsArenaScript::WatchForRoundsInArena -- RELOCATION_MATCH (/d2inlT57), test PASS, vland PASS, addmatch done
- Identity: Ego_r + FableWin name sets; one reference from landed CQ_HeroSoulsArenaScript::Main 0x00D7C890 (0x00D7D5E9; reloc_bindings).
- Extent: 3148 bytes is right (0x00D7FE90..0x00D80ADB, three rets; the expanded ~vector exits after 0x00D80A35 are the row's own).
- What closed it (earlier DIFFER(2105v3148)): dropped the fitted `template<> vector<CScriptThing>::~vector();` (always-called) so
  the STLport dtor is plain inline; then an inline trace (tools/vsc.sh) of the 47 ~vector sites. Retail's split (exits before the
  CreatureCount wait + L119/L122/L129 + end expanded; the 11 in-wait exits, L116, the wave-scope end and L125 called) needs every
  expanded score < T <= every called score. With `do { } while (round < 3)` the in-wait exits score 53 = L129 / end (no T works);
  with `while (round < 3)` (same code) the expanded set scores 43..56 and the called set 64..73 -> RELOCATION_MATCH for /d2inlT57..64.
  Flag chosen: /d2inlT57 (smallest). Loop-shape facts measured on the way: CreatureCount loop as for/while/!(<=) identical; a
  redundant `if (CreatureCount > 0)` guard cools the in-wait exits (64) but heats L116 / scope end (46); def/wave loops as while: no change.
- Devices: per-row flag /d2inlT57 (owner rule 2026-11-08 / coordinator 2026-11-14), marked on line 2 with the trace evidence.

## 0x00EAF810 NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoAskingHeroToKillAssassin -- RELOCATION_MATCH, test PASS, vland PASS, addmatch done (successor 3, 2026-11-15)
- What closed it: `CV_MurderTwistFinalScript* parent = ParentClass; bool assassin_distrusts_hero = !parent->DoesAssassinTrustHero();`
  (FITTED, marked in the source header). Same bytes, but the named pointer stays a c2 candidate in block 9 (the talk-block entry
  block that also constructs PauseMyEntitiesPlease, K 9 -> 10, pause cost 6 there): pause priority 30 -> 36, so it is coloured
  before the answer arms' GetHero `Game` temps (#54/#58, prio 30, larger keys), takes ebx whole, and every Speak `Game` temp spills
  to the frame = retail. Found with tools/pq.sh (c2prio pause priority per variant, 17 s) after ~20 code-neutral variants that left
  the pause at 30: named intermediates of the call result (b1, forwarded), negation spellings (c1-c3), inline ~CScriptThing (a1),
  answer-loop gotos (g1/g2), copy-init / reference pause (p1/p3), TaskDeclined through a pointer / reference / bool local (q1-q3,
  forwarded: used once in the same expression), named GetHealth bools (v10/v11), `== true` loop test (v2: pause 18), /G5 /G7.
  The pointer used as the OBJECT of a call survives as a candidate; one used for a member read is forwarded.
- Flags: none. Headers: no overlay change. Test: state model (test/00eaf810.cpp, from the earlier draft test).
- Earlier best differ: tmp/00eaf810.differ_prev2.cpp (3021v2945); history below.
- Name proven (catalog label was truncated): Ego_r + FableWin name sets ?DoAskingHeroToKillAssassin@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ;
  one caller, CMurderTwistSheriff::Main at 0x00EAF039. Extent 2945 confirmed (ret at 0x00EB0390; next DoesAssassinTrustHero 0x00EB03A0 landed).
- The earlier draft ports unchanged onto today's headers (same grade). Every instruction up to 0x00EB003B matches; the first wrong
  decision is the declined arm's Speak (0x00EB0038..0x00EB0062): retail loads Game into ecx and SPILLS it to [esp+20] across
  `operator const char*` 0x0099E4C0, vtable in ebp; ours keeps Game in ebp and the vtable in ebx, which evicts the talk-block
  PauseMyEntitiesPlease.Game (ebx) -> its home store `mov [esp+20],ebx` at 0x00EAF9AD and reload `mov ebx,[esp+1c]` at the arm's end,
  +4 frame word, every later temp slot shifted 4, exits stop cross-jumping (+76 bytes).
- c2prio (tools/c2prio.py, C44 copy): the two Game temps of the accepted (line 114, id 54) and declined (line 129, id 58) Speaks both
  have priority 30; id 58 (key 788) is coloured first and finds ebp free, id 54 gets memory (as retail). The pause (id 5, lines 63..138)
  has priority 30, refs 41, coloured later (#31) -> split ebx. Retail must have either a higher pause priority or an ebp holder
  (const-0 piece?) over line 129. Next ideas: what occupies ebp at line 114 (choice cost / neighbour rule), the const-0 split pieces,
  a pause object scope / declaration change that raises its priority above 30.

- Successor (2026-11-14): structural diff (constants masked) of the whole row shows the hit block and everything else identical; the
  only differences are the declined Speak register pick and its cascade. Tried the CEndGameScythe / CDisplacedTrader SAY-block lever
  (do { if (GetHealth > 0) { Speak; wait; T } } while (0)) on all 7 speech blocks (2952v2945, same declined-arm pick, exits re-shared),
  on the two answer arms only (3021, unchanged) and on the talk block only (3027). c2prio reading: #58 (declined Game temp, key 788)
  and #54 (accepted, key 629) tie at prio 30; #58 goes first and finds ebp free; #54 is then blocked (const-0 piece #2 / pause piece
  #76). Retail needs BOTH in memory, i.e. ebp taken over line 129 when #58 is coloured. Still open; no lead closer than that.
  Swapping the answer arms (`if (answer != 1) {declined} else {accepted}`, 3020v2945) keeps the extra pause home store
  `mov [esp+N],ebx` (0x00EAF9AD) although the declined arm is then first: the spill is not just the #58/#54 key order.
  Tools: tools/sdiff.sh <addr> <src> <leaf> = constant-masked structural diff (built v retail).
- Successor 2 (2026-11-15, opus-5.5 wf pass; ~30 grades + ~20 c2prio traces/blocks dumps): STILL PARKED DIFFER(3021v2945).
  Measured mechanism (tools/pp.sh = c2prio summary of pause / Game temps): every Speak-with-CCharString site has a `Game` temp of
  prio 30 (K5 x cost 6, one block each: 67/84/114/129/149). PauseMyEntitiesPlease (#5, 63..138) is also prio 30 but key 117, so the
  declined-arm temp (key 788) is coloured first, finds ebp free, and the pause is split (home store + reload). Whenever the pause
  reaches prio >= 31 it is coloured first, takes ebx whole and ALL Game temps spill = retail: diagnostics (not sources) removing the
  declined ternary (pause 35), the accepted ternary (35) or the declined Speak (32) all give exactly the retail allocation. Without
  the hit block every Game temp gets ebp (they do not interfere with each other). So the open question is one point of pause
  priority: a source fact that removes one live-through block / one K from the talk block without changing code.
  IL-neutral spellings measured with NO effect (same 3021, same hunks or worse, pause prio 30): k=1..7 unused float locals at entry
  (keys are tuple numbers, unused locals do not move them); dead release asserts (FABLE_RETAIL_ASSERT_DEAD_ARM) in the declined arm
  or after the pause ctor (prio and keys unchanged: dead arms are gone before colouring); ternary as const char* local / if-else /
  default-then-if / copy-init; bool as `? false : true`; int answer; `> 0`; `== true` (code changes); TaskDeclined = 1 / == false;
  pause in its own { } scope; answer loop as while((answer = ...) < 0) (same), for(;;)+break (2 extra hunks), for(;cond;assign)
  (3037); answer arms as two ifs / else-if (same); swapped arms (second arm's temp takes ebp instead); return spellings
  ({ _Fail_Val.Return(); return; } / plain return: 3037, layout only, prio same); same-TU callee bodies DoesAssassinTrustHero /
  WaitForEnflameReactionIfNecessary defined above the row (landed text; same); plain `inline` CTimer bodies (operator= stops
  inlining: 53 hunks; FABLE_FORCEINLINE required, evidenced by the inlined SetTimer at the top). volatile bool (diagnostic) lowers
  the pause to 27 with identical bool code.
  Kept in differ: Anniversary Xbox PDB local names (S_REGREL32 records of this function: speech_movie, message x4, speech_movie,
  full_text_name; weak third source) and FABLE_FORCEINLINE instead of bare __forceinline (same grade). Previous differ:
  tmp/00eaf810.differ_prev.cpp. Next ideas: per-block K/live-through diff of the talk block against a matched twin with the same
  speech blocks + pause (e.g. 0x00EB0830 talk block) to find which block retail lacks; a c2 instrumented dump of the allowed sets
  at each split (the trace reuses ids after splits, so `#N` in late steps are pieces, not the table ids).
## 0x00D0A830 NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main -- RELOCATION_MATCH, test PASS, vland PASS, addmatch done (successor)
- What closed it: the residue was an inline split INSIDE one return (movie dtor expanded, ~CTimer called at the merged speech exit
  0x00D0B27A). A scan of every expanded movie dtor in the exe (tools/mvscan.py) finds this the only site where ~CTimer is called right
  after one, and c2's score c*g(f) is monotone in c at one site (movie 19 > timer 14), so the movie dtor and the timer dtors must run at
  DIFFERENT statements. Source: the two speech-wait terminate checks are `goto Terminate;` (the scope exit destroys pause + movie at the
  goto) wrapped in the statement-macro do-while(0) -- once inside the IsPerformingScriptTask loop (f 0.1648), twice after it (one
  wrapper: f 0.0183 < the movie's break-even 0.047; two: expanded) -- and `Terminate: return _Fail_Val.Return();` at the end of the
  function under in-body `#pragma inline_depth(0)` / `inline_depth()` (without it that return is hot and cross-jumps into the
  expanded-timer exit 0x00D0B227). The earlier lane's do-while(0) movie scope stays (without it 2676). All FITTED, marked in the header;
  `// Inline-budget:` on line 2. Flags: none. Headers: repo opt-ins only (no overlay change).
- Measured on the way: /d2inlS sweep (S12 already flips every ~CTimer, S15+ 2784); goto label without pragma (2700/2732: merges into
  the main-loop exit); label inside `if (false) { }` (same merge); Scythe-style inner do-while (also matches with single wrappers + an
  if(false) label, more devices); no outer do-while (2676/2704). Equivalent matches kept in tmp/: h8, h9, s2, s3, m5 (both checks
  double-wrapped), m6.
- The previous differ (2700v2720) is tmp/00d0a830.differ_prev.cpp.
