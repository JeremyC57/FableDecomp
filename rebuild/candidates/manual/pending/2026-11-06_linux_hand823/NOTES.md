# Chunk 823 notes: six retries, all parked again

Nothing matched. Every row was first re-graded unchanged against the current repo headers. Then each got 11-20 new attempts, listed in
STATUS.tsv notes and ATTEMPTS.log. hdr/ is empty: no shared header changed and there is no header README. Every name in the six rows is
a PDB name already bound in the labels; none is descriptive. The six existing labels are correct, so LABELS.tsv has a header only.
NAME_FIXES.tsv has three landed misspellings found while checking callers and callees (0x00B5A3F0, 0x00B5AF60, 0x00CBE2FF).

## 0x00E90BE0 NScript::CV_GuildMasterScript::CGuildMasterGameFlow::Main -- DIFFER(4977v4914), 60 lines (unchanged best)
- Shape: guild-master focal-site game flow. There are five CScriptGameResourceObjectMovieBase speech_movie sites: 3 outer sites and 2
  inside the focal-site loop.
- Residue: retail expands the MovieBase ctor (CBaseObject + vtable + zeroed members) and the ~CCountedPointer<MovieBase> release at the
  2 inner sites. At the 3 outer sites it calls ctor 0x006E7B60 and dtor 0x006E7B80. OUTOFLINE_CTOR (this source) makes our code call the
  ctor at all 5 sites.
- Chunk 823 tried a full 2x2x2x2 matrix: ctor OUTOFLINE x dtor OUTOFLINE x declared-only ~CCountedPointer<MovieBase> spec x do-while(0)
  "heat" round the two inner SAY blocks. Results:
  - c1d0sp0h0 = parked a7: 60 lines (best).
  - c1d1: 202 lines. c0d1: 320. Every c0 combination: 774-1115. Heat always inlines more everywhere.
- Also tried: a row-local plain `inline` MovieBase ctor (p1/p2). It is always expanded, at any inline threshold, also with diagnostic
  zero thresholds and /Ob1 (diagnostic only, not in the row). So no inline-budget setting gives retail's mixed per-site result. This is
  the documented MovieBase per-site blocker.
- Callees: unchanged from hand808 and all bound in reloc_bindings.tsv. IsDistanceFromThingToPositionUnder 0x00CBE4B7 has one spelling.

## 0x00D3D150 NScript::CQ_GuildTrainingScript::CAppleGirl::Main -- DIFFER(4252v4252), 28 lines (unchanged best)
- Residue: register colouring only, in the MANY_MORE_APPLES / NO_MORE_APPLES arms of the apples-given chain:
  - retail uses eax (Game vtable) and ecx (string);
  - ours uses edx / eax, the ONE_MORE_APPLE arm's registers.
- c1-c11 changed the following; all gave 28-38 lines:
  - Conversation declared int, or hoisted once before the chain;
  - a do-while round the chain;
  - CurrentApples+= before Pause;
  - post-pause test written >0;
  - a nested chain, or ==0 tested first;
  - loop-condition operand order;
  - ++ApplesGiven after TakeObjectFromHero;
  - ApplesGiven as int with postfix ++;
  - ApplesGiven initialised before the T check.
- New test: differ/00d3d150.test.cpp (state model: HELP timer, chat/question, apple hand-over, needed lines, pie). It passes under g++
  and under VC7.1 (verify_and_land.behaviour_of).
- Callee: IsDistanceBetweenThingsUnder 0x00CBE2FF uses the correct PDB spelling. Two landed callers misspell it; see NAME_FIXES.tsv.

## 0x00B5A3F0 CEngineScreenEffectColourFilter::ApplyColourFilter -- DIFFER(2923v2916), 21 lines (was 23)
- Parked source: hand792 with the unused `CRenderManager* rm` local removed (q17). The parked text had 23 lines; the re-grade with
  current headers gave 21.
- Residue:
  - the third SetPixelShaderConstant's `UpdateFlags |= 2`: retail does `or [eax+3D8h],ecx`, ours does load/or/store;
  - the CRenderStateManager bookmark test/or operand flip lands one block late;
  - the alignment nop.
- Finding: spelling GFLimit by value, `template<class T> inline void __fastcall GFLimit(T& value, T lower, T upper)`
  (probe/b5a_q3.cpp, q5, q8), gives 15 lines and fixes the test/or flip. But the PDB symbol is ??$GFLimit@MMM@@YIXAAMABM1@Z
  (template<T,L,H>, const L&, const H&). So it was not taken as the source. Details:
  - It needs at least 2 clamp sites changed; single-site probes z1-z3 give 21.
  - Copying lower/upper into T locals inside a const-ref body (q10) does not reproduce it: 21.
  - Int-literal bounds (q12-q16, GFLimit<float,int,int> and variants) give 21-27.
- Header debt / name fix: landed 0x00B5AF60 CEngineScreenEffectColourFilter::Render uses a local pad struct and placeholder free
  functions. Its call to this row is bound as ?CEngineScreenEffectColourFilter_DoRender@@...; see NAME_FIXES.tsv.

## 0x00EC68A0 NScript::CV_SickChildScript::CSickChildsSister::Main -- DIFFER(5969v5962), 67 lines (k1, store order fixed)
- The re-grade with current headers gave 67 (the parked text said 101).
- k1 wraps the run-to-mother block in an inner `{ }`: the WavedAtHero / MoveState / WaveTimer locals, ClearCommands, MoveToPosition,
  the loop and the post-loop T check. That makes ~WaveTimer run before `LeadHeroToSickChild=true;`, which is retail's store order (a0
  stored first). The line count stays 67.
- Residue: retail places the rotated loop test and loop exit after the far-from-hero branch, then the near branch after the exit. Ours
  lays near before the test (+7 bytes). There is also a GetTimer register swap.
- Tried, none closer:
  - s1-s3 (arm order): identical;
  - g1-g3 (plain-inline CTimer members): out-of-line calls; g1 is size-exact but not retail;
  - k2/k3 (do-while heat): identical;
  - k4-k6 (inline operator==): called or identical;
  - L1/L3 (for/break, bool-done): identical;
  - L2 (trailing continue): 6380 bytes.

## 0x00EC9D30 NScript::CV_SickChildScript::CMansLover::Main -- DIFFER(4708v4708), 58 lines (unchanged best)
- Residue: block layout around the give-letter question.
  - Retail keeps the answer==1 terminate test separate from the ShownLetter-else terminate test. It falls through `cmp edi,1; jne` into
    answer==1 -> give-letter, and the ShownLetter arm jumps back there.
  - Ours cross-jumps the two identical T tests, so the NOT_SEEN arm comes first.
- a1-a11 gave 58 or worse (60, 62, 98): plain return; function-scope flag (with and without reset); goto join; do-while round NOT_SEEN;
  store before T; inverted `if(!T)`; GIVE block inside the answer==1 arm with a goto from ShownLetter; answer!=1 written first.
- New test: differ/00ec9d30.test.cpp (state model: question / shown-letter paths, both answer2 outcomes, state answers, leaving walk +
  fade). It passes under g++ and VC7.1.
- Callee: IsDistanceBetweenThingsOver 0x00CBE3EA has one spelling.

## 0x007DB6A0 CTCTavernGameCardBlackjack::CardsMainGame -- DIFFER(2544v2556), 63 lines (unchanged best)
- Residue:
  - case 0x1c's `mov ecx,esi; call SetState` tail is cross-jumped into the 0x17-empty block's identical tail. Retail keeps its own copy
    and jumps to the late ebx=0 epilogue copy 0x007DC03E.
  - Two register picks in the 0x1d money check.
- Diagnostic d1 (not a candidate): one extra store after the 0x17-empty SetState breaks the identical tail. 0x1c then jumps to the late
  epilogue as in retail, so retail's 0x17-empty tail was not identical at cross-jump time.
- b1-b13 gave 63 or worse: break forms, default:return, do-while, iterator loop, 0x1d operand order and inversions, case order.
- Callees: CCard::Turn 0x008F0620 is bound. Split / DealCard / CanSplit come from hand801.
