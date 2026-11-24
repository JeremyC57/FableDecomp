# SHARED_FORM.md -- MovieBase / GROBase side (agent B24A4, 2026-11-24) -- (final for B24A4)

Status: NO cost-lowering byte-neutral header form for ~CScriptGameResourceObjectMovieBase found (none found, not proven absent:
Ego_d unseen); the inequality is more likely closed from the ~CTimer side (lane B) plus the family's /d2inlS15.

## Facts (c2 inline trace, probes in $S/w22/b24a/00d11930/pr/, trace tool 00d11930/inl/run.sh)
- Stock: always-inline iff c1 <= 13 (S=9), else expand iff c1*(1/f+6)/7 < 74. c1 = sz - 6 at these thiscall dtor sites.
- ~MovieBase (implicit; CP<Movie> dtor declared-only specialization; GROBase `virtual ~(){}`) sz 25 / c1 19. Decomposition by probes:
  call ~CP(this+8) 8 + inlined ~GROBase 11 + 6 ("this" live across the first call) = 25, at any PImp offset.
  Every 19-sized variant either drops the 0x0126008C store (implicit GROBase dtor) or leaves ~GROBase CALLED in the caller
  (inline_depth(0) at the class brace: nested call is inherited, not re-decided).
- Implicit ??0MovieBase = ??0STB = sz 26 / c1 20. Retail expands the MovieBase ctor at the cut-scene site of 0x00D11930 where it
  calls the 4 STB ctors in the same block: MovieBase ctor must ALSO be cheaper/forced (FORCEINLINE_CTOR opt-in), dtor-only stories
  cannot be the whole cause.
- c2 compiles callees before callers (on demand), whatever the definition order (GROBase dtor defined after MovieBase / at TU end:
  unchanged 25). Several callers in one TU: unchanged.
- A visible inline CP<Movie> dtor (no specialization): ~MovieBase sz 66, nested CP re-decided in the caller -> CP expanded at hot
  sites; retail calls 0x006E7AB0 at every site, so the declared-only CP<Movie> dtor is right for these TUs (the standalone ??1 copy
  this TU emits then calls 0x6E7AB0 and is NOT retail's 0x006E7B80, which comes from game_script_interface's TU: not leaf-bound debt).
- FableWin debug sizes agree with our MovieBase chain text: ~MovieBase 75, ~STB 75, ~GROBase 28, ~CP<Movie> 19, ReleaseData 265
  (ghidra_out/fablewin_obj_functions.tsv). CTimer ctor/dtor/op= are each +1 instr / +2 bytes in FableWin (lane B's question).

## Inequality restated
Retail at one site (0x00D11930 cut-scene block, f 0.0156) expands ~MovieBase and calls ~CTimer; elsewhere ~CTimer is frequency-split.
Under /d2inlS15 (always iff c1 <= 19) ~MovieBase (19) is always-inline. If retail's ~CTimer costs c1 20..23 (sz 26..29) instead of
our 14, S15 keeps ~CTimer frequency-split with EXACTLY retail's hot/cold sites in 00d09010 (expand at f 0.0707 / 0.2459 needs
c1 < 25.7) and 00d11930 (f 0.0625 expanded needs c1 < 23.5). So S15 + a ~CTimer body of c1 20..23 WOULD fit all three rows -- but this depends on lane B's CTimer result and is unlikely:
c2's sz is POST-optimisation (probes t0..t11: every ~CTimer spelling with retail's /O2 bytes is sz 18..20, c1 12..14).
Our S15 trace of 0x00D11930 flips exactly 3 decisions v stock: MovieBase site 153 (wanted) and ~CTimer 130 / 160 (unwanted; they
would stay called with c1 >= 20).

## RESULT (B24A4): use the family flag, not a header form
0x00D11930 RELOCATION_MATCH (vland PASS) with /d2inlS15 + one extra scoped `#pragma inline_depth(0)` round the terminate check whose
exit retail sends to its CALLED ~CTimer. Recipe for the family (00d09010, 00d01630, B24B's rows): S15 makes every MovieBase site
always-inline (= retail); then put inline_depth(0) round exactly those `if (IsActiveThreadTerminating()) return;` checks whose retail
exit calls ~CTimer 0x00CD4470 (S15 alone would expand the timer there). Header texts unchanged (FORCEINLINE_CTOR opt-in kept).
Limit of the recipe: it works only where the CTimer-calling exit has NO movie in scope. In 00d09010 / 00d01630 the cold exits inside
movie scopes need ~MovieBase expanded AND ~CTimer called at the same return -> needs ~CTimer c1 > 19 (S15) with retail's bytes.
Lane B: if your original CTimer text measures c1 >= 20 in the c2 trace (00d11930/inl/run.sh, probe pr/t0.cpp, INL_F=ProbeTimer),
tell the coordinator: 00d09010's v1.cpp + /d2inlS15 is then the first thing to grade.
Lane B's result (b24b/SHARED_FORM.md): the CTimer text is right and costs c1 14, so the cost reversal 00d09010 / 00d01630 need is on
neither side as far as the lane can see; both rows stay blocked (Ego_d questions: 00d09010/ESCALATE.md). One header text per class
kept: no MovieBase / GROBase / CTimer header was changed by B24A4.
