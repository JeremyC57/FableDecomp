# 0x00D0D910 CGate2Guard1::Main -- escalation (B24B8, 2026-11-24)

## Proven: the row is conflict-only. Nothing but one inline decision class differs.
Two c2 decision-override compiles (gdb on c2.dll's plain-inline decision 0x1078D1AA, result register rewritten at 0x1078CFDA;
kit in b8/inl/: inl_force.py, inl_force_mb.py, inl_host.py with INL_PY/OBJ_OUT; graded with b8/objgrade.py, relocation-masked):
1. Source b8/p0.cpp (= differ text: plain inline CTimer, header texts unchanged) with **STOCK flags**, every one of the 21
   ??1CScriptGameResourceObjectMovieBase decisions forced to "expand": **RELOCATION_MATCH** (b8/inl/forcemb.obj).
2. Same source with /d2inlS15, the 31 ??1CTimer decisions at static frequency f < 0.032 forced to "call": **RELOCATION_MATCH**
   (b8/inl/force.obj).
So retail's compiler made exactly (decision set proven; which cost causes it is NOT): ~CTimer by the stock frequency rule (c1 14: expand iff 14*(1/f+6)/7 < 74, i.e. f > 0.032),
and the implicit ~MovieBase expanded at every site (f down to 0.0001), as if forced or of cost c1 <= 13 (ours: sz 25 / c1 19).
Every hunk of every earlier form (r0 36 hunks, a1 117, f1 165) is this one decision class; slot shifts and the edi/TimerIndex
hunk follow from it (with the CTimer map right, f1 shows `mov edi,eax` = retail).

## Why no source form reaches it (measured this session, B24A4 / B24B3 before)
- Cost order: c2 decides "always" iff c1 <= S+4; MovieBase 19 > CTimer 14 under every S, so no /d2inlS<N> gives
  MovieBase-always + CTimer-split; /d2inlT cannot either (MovieBase at f 0.0001 needs T > 27000, CTimer at f .0195 needs T <= 114).
- Penalty term (c2 0x1078D1DA..: pen = (sz/2)*(g+a)/9000 only when sz > S and g+a > 9000): g <= 2675 here, pen 0; and it would
  raise MovieBase more than CTimer.
- inline_depth(0) per site (B24A recipe) blocks MovieBase too at the 16 in-movie cold exits (00d0c410 d1: 169 hunks).
- FABLE_RETAIL_..._MOVIEBASE_FORCEINLINE (user-declared forced dtor): forces the expansion (CTimer map then = retail, 2+1) but adds
  a MovieBase vtable store per site and a 4-byte larger frame (b8/f1.cpp, 165 hunks). Retail has no such store.
- Forceinline GROBase dtor (probe b8/g1.cpp + overlay b8/hg1): no effect on the implicit ~MovieBase decision (= a1).
- Non-`inline` ~CTimer under S15 (b8/n1.cpp): identical to inline (S applies to both).
## Question for the owner / lander
What makes retail's implicit ~CScriptGameResourceObjectMovieBase inline at every site with no MovieBase vptr store? A text of
the class (or of CCountedPointer<MovieBase> / GROBase) that VC7.1 costs <= 13 or treats as forced while keeping the implicit
dtor's bytes. Same question blocks 0x00D0C410, 0x00D09010, 0x00D01630 (family). Counterexample to a plain 'MovieBase always inline': 0x00DC93F0 (same object) CALLS ~MovieBase at its helmet-arm exit.
Not a compile-order effect on our side (c2 compiles callees first); Ego_d (unseen) may show the class text.
