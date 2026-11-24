# 0x00D0C410 CBanditForger::Main -- B24B3 notes
## Lessons that apply
- (B24B3, hash oracle) CTimer text = the landed one (FableWin hashes, $S/w22/b24b/SHARED_FORM.md); ~CTimer c1 = 14 at every site here (pen 0).
- B24A4 (b24a/SHARED_FORM.md): ~MovieBase c1 19; stock always-inline iff c1 <= 13 (S9), /d2inlS15 -> <= 19.
- Codex pass21 (pending/2026-11-11_codex_pass21/return/parked/00d0c410/PROBES.md): restored TalkedTo loop, !IsObjectInThingsPossession,
  operator<= via GetTime (donor), frame 0xa4 with S15 + called ~CTimer.
## B24B3 results
- Baseline regrade (draft = Codex s10): DIFFER(5293v5373), 2 hunks: missing D0D3A1..D0D3EE (hot exit block with EXPANDED ~CTimer)
  + 2-byte jmp before an aligned loop (D0D4AB).
- Retail exit map (exits.py on q logs): retail expands ~CTimer at exactly TWO exits -- the loop-1 wave wait loop (D0C80A) and the first
  movie-block wait loop (D0CA72), cross-jumped into one copy at D0D3A1/D0D3C8 -- and calls 0x00CD4470 at every other exit (common
  tail D0D8C8). MovieBase cleanup is EXPANDED at every in-movie exit (D0D377 block), even at f 0.0003.
- Stock + plain inline ~CTimer (a1): 5304, 112 hunks, but the ~CTimer exit map is EXACTLY retail's (c2 trace forger_a1.txt: r=1 at
  f .2283 / .0408, r=0 at f <= .0278); what is wrong is MovieBase: called at cold exits (c1 19 > 13). With S15 (a1+S15, 5331)
  MovieBase is right but ~CTimer always-inline (c1 14 <= 19) at all ~60 sites.
- So no single S/T value fits: retail needs MovieBase always-inline AND ~CTimer frequency-split, i.e. c1(MB) <= 13 < ... or
  c1(~CTimer) > 19; the CTimer text is proven (hash) and auto-inline (non-`inline` definition, e1) behaves the same. The open cause
  is the ~MovieBase cost (B24A's side). T sweep (a1 + /d2inlT80..200): 5336..5624, no window.
- Device probes (diagnostic, not landable): a2 inline_depth(0) round `CTimer ForgeTimer;` blocks the ctor, not the dtors (5363);
  d1 inline_depth(0) on every non-hot timer exit (S15) also blocks MovieBase at in-movie exits (5253, 169 hunks).
- c1.cpp = landable spelling of the best: declared-only CP<Movie> dtor (53-row spelling) + `__declspec(noinline) inline ~CTimer`
  (landed precedent 0x00E0F8A0 etc.) instead of strong out-of-line definitions under auto_inline(off): same DIFFER(5293v5373) 2 hunks.
  differ/00d0c410.cpp = c1.
State: escalated (ESCALATE.md). Next idea: the ~MovieBase decision (cause open, see ESCALATE.md; 0x00DC93F0 contradicts a plain 'always-inline'); also recheck pick-any
(declared-only CP<Movie> dtor gives a NEQ ~MovieBase copy, row 0x00DB3FA0).
## B24B4 (2026-11-24)
- B24A family recipe applied (r1.cpp: S15 + plain inline ~CTimer + inline_depth(0) round the 16 terminate checks with NO movie in scope,
  2 hot exits left plain): DIFFER(5358v5373) 131 hunks -- the caveat bites: 36 terminate checks sit inside the two speech_movie scopes
  (retail: MovieBase expanded, ~CTimer called), so ~CTimer expands there. Recipe does not apply to this row. Best stays c1.cpp (2 hunks).
- Compile-order probe (diagnostic, strong out-of-line ~CTimer): non-inline ~CTimer defined AFTER Main (o1) = before Main (o2) =
  5331 (S15) / 5304 (stock), identical to the inline forms -> definition order inside one TU is not the lever (agrees with B24A).
- Structural argument (for ESCALATE): in c2's regime-0 rule, expansion is monotone in c1 at equal f; c1(~MovieBase) 19 > c1(~CTimer) 14
  at every site (pen 0), so "MovieBase expanded + ~CTimer called at the same return" is impossible for any S/T and any frequency
  unless the two dtors see different costs or frequencies at that return. Retail does it at ~36 returns here.
## B24B6 (2026-11-24): visible-body lever checked on the shared MovieBase cost (probe in ../00dc93f0/v1.cpp): a VISIBLE noinline
CP<Movie> dtor specialization leaves implicit ~MovieBase at sz 25 / c1 19 (same as declared-only), so it cannot change this row's
MovieBase-v-~CTimer conflict. Not worked further by B24B6 (context).
## B24B8 (2026-11-24): PROVEN conflict-only; pick-any checked
- c2 decision override (kit ../00d0d910/b8/inl, inl_force_mb.py): a1.cpp at STOCK flags + all 38 ~MovieBase decisions forced to
  expand = RELOCATION_MATCH (b8/forcemb.obj). So c1's 2 hunks (missing hot ~CTimer block D0D3A1..D0D3EE AND the 2-byte jmp at D0D4AB)
  are both that one decision class; no independent residue. Same family answer as 0x00D0D910 (ESCALATE.md there, SHARED_FORM.md).
- Pick-any on c1.obj (/d2inlS15): pickany_vs_retail.py: 9 EQ; NEQ ??1CScriptGameResourceObjectMovieBase (0x006E7B80: retail's copy,
  from game_script_interface's TU, inlines the CP<Movie> release; ours calls the declared-only ??1CCountedPointer<MovieBase>) and
  NEQ ??1CIsThingAlive (0x00E241B0: retail inlines ~CScriptThing; ours calls it) -- neither referenced by the leaf. Every-copy check
  (../00d0d910/b8/pkx.py, ??_G by vtable slot 0): ??_GCScriptGameResourceObjectMovieBase 0x006E7BD0 NEQ (same CP cause),
  ??_GCIsThingAlive 0x00CDF120 NEQ (same ~CScriptThing cause), ??_GGROBase 0x006E70A0 EQ, ??_GSTB 0x007E7520 EQ.
  All four NEQ come from the shared declared-only CP<Movie> dtor (53 landed rows) and `~CIsThingAlive() {}` with declared
  ~CScriptThing (78 landed rows emit the same copy, owner row 0x00E241B0 landed): HEAD debt under EXTRA_RULES 2026-11-21, not new.
  The declared-only CP<Movie> is REQUIRED for the leaf (a visible CP dtor makes ~MovieBase sz 66 and expands the CP at hot sites; B24A4).
## B24B9 (2026-11-24) -- FableWin read (debug build), no decision moves
- FableWin Main 0x036A66E0 (../00dc93f0/b9/fw_036a66e0.txt): no asserts, no for-scope prolog. Locals (FableWin.pdb, register_scripts
  module): NotAttacked, seh_me, PsstAmount, HasPass, TalkedTo, _Fail_Val, ForgeTimer, Conversation x2, speech_movie / PauseMyEntitiesPlease x2,
  answer x2 = ours. Two text differences found by aligning /Od call sequences (b9/ours_seq.txt v b9/fw_seq.txt):
  (1) both answer waits are `long answer; while (!((answer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) { NSF; CHECK }` (FableWin
      0x036A7902: call slot 0x9C at the loop head, store, `setge`, jne exit) -- ours calls it before the loop and at the loop end;
  (2) tail `while (true)` (FableWin `mov edx,1; test`) instead of `for (;;)`.
  b9/q0.cpp (1) and b9/q1.cpp (1)+(2) on a1 (stock): DIFFER(5304v5373) 112 hunks = a1 bytes; traced MB/~CTimer f identical to a1.
  Keep q1's spelling as the FableWin-evidenced text. MovieBase decisions unchanged (f .0000..0408, called; retail expands all).
- b9/c1q.cpp = differ/ (S15 best c1 + FableWin answer loops + while(true) tail): DIFFER(5293v5373) 2 hunks, unchanged bytes; previous differ kept as differ/00d0c410_pre_b9.cpp.
- B24B9 calls + conditional-jump alignment (../00dc93f0/b9/cjalign.py on b9/_fwsize/c1q.obj v FableWin): identical except the trailing EH funclets the script also reads (ours only): Main's CFG is FableWin's.
- B24B9 swap check: differ/00d0c410_pre_b9.cpp v b9/c1q.cpp (/d2inlS15) RAW-EQUAL (5293); a1 v b9/q1.cpp (stock) RAW-EQUAL (5304)
  (b9/*.obj). The answer-wait spelling `while (!((answer = X()) >= 0))` is read off a VC10 /Od idiom (setge): inferred, not proven text.
