# 0x00D0C410 escalation (B24B3)
Best: differ/00d0c410.cpp (= c1.cpp, /d2inlS15): DIFFER(5293v5373), 2 hunks = retail's expanded hot ~CTimer exit block D0D3A1..D0D3EE missing
(+ a 2-byte alignment jmp). Inline-budget conflict, measured with the c2 trace: retail = MovieBase always expanded + ~CTimer frequency-split
(expanded only at f .2283 / .0408). Stock gives the ~CTimer split exactly (a1) but calls MovieBase at cold exits; S15 expands MovieBase but
makes ~CTimer always-inline. c1: MovieBase 19, ~CTimer 14, so no threshold separates them. ~CTimer text proven by FableWin hashes
(b24b/SHARED_FORM.md). Cause open: row 0x00DC93F0 shows retail CALLING ~MovieBase at a site our model expands (f .1159), so 'MovieBase
always-inline' is not a uniform retail property; needs the frequency/cost question answered (B24A lane) or an owner-accepted per-site device that forces
~CTimer at the two hot exits (none exists in plain C++: inline_depth(0) also blocks MovieBase).

Pick-any caution (from row 0x00DB3FA0): c1.cpp keeps the declared-only CP<Movie> dtor specialization, which makes the emitted
??1CScriptGameResourceObjectMovieBase copy NEQ to retail 0x006E7B80; recheck with the visible header template before any hand-back.
