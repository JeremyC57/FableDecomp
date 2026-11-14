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

## 0x00EAF810 NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoAskingHeroToKillAssassin -- PARKED DIFFER(3021v2945), differ/00eaf810.cpp
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
