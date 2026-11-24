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
