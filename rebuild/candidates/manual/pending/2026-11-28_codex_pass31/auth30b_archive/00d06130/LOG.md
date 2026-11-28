g1: c0 with the strong Alloc / GetParentScript (landed rows 00D05FE0 / 00D06080: new LNK2005) replaced by the anonymous-namespace scaffolding helper of 00d05a60 g4; hypothesis: leaf stays 1400, no strong duplicate.
final = g1.cpp with the Leaf line moved first: RELOCATION_MATCH 1400, pick-any EQ 2 / pk_all EQ 4 (same as g1).
Gates 2/6/7: t1_test.cpp = the 00d05a60 shared-header fixture with g1 prologue, CAssassin3 and ASSASSIN3 strings, a link-stub body for the out-of-line Movie counted dtor and GetParentScript; vt.py with g1.obj PASS, without LINK_FAIL. purity genuine. NAME_GATE PASS.
## Round 2 (coordinator, 2026-11-27)
Facts: see 00d05a60/LOG.md round 2 (FableWin "marked inline" on every CAssassin3 member; Main at hpp lines 1053-1092).
g2 hypothesis: Main in the class (overlay opt-in FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_INCLASS_MAIN_PASS30) gives final's bytes; the row's
`#pragma inline_depth(2)` before Main is dropped (it cannot sit before an in-class body); the helper still triggers emission.
g2 (compile fix, same hypothesis): the Movie counted-dtor specialisation declaration moved before the includes (the in-class body instantiates it).
g2 (compile fix 2): fable_compiler.h before CCountedPointer.h, as 00d05a60 g4 does.
g3 hypothesis (coordinator item 2): Main out of line (final minus the helper, Main not inline) with flags /O2 /Oy /d2inlS15, the same-TU landed Main rows' flag.
g3 result: DIFFER(1448v1400); pickany NEQ ??1CScriptGameResourceObjectMovieBase 25 (not leaf-referenced). Out of line + the neighbour flag does not match.
g2 result: RELOCATION_MATCH 1400 (/O2 /Oy), pickany EQ 2 / pk_all EQ 4; test PASS with g2.obj; purity genuine; NAME_GATE PASS (NAME_GATE_g2.txt). The row-level inline_depth(2) before Main is not needed on the in-class route.
Round 3: R1 fails (see PAIR_FACTS.md round 3); the pair stays escalated as object-mode.
r4a hypothesis: 00d05a60 g5 prologue (its MovieBase/STB counted specialisations) on CAssassin3 gives the sibling counts 3/5 with 0x006E7AB0 EQ.
r4a result: RELOCATION_MATCH 1400; pickany EQ 3, pk_all EQ 5 (006e7ab0 53 EQ); vtchk all EQ but movie ??_G 46 (debt). The 2/4 v 3/5 difference is the declaration-only MovieBase specialisation (UND in g2.obj).
r4b hypothesis (R1): no counted-pointer specialisations at all, plain inline ~CIsThingAlive, no inline_depth.
r4b result: DIFFER(1491v1400): ~ScriptedThingBase is expanded into Main (call counted dtor 53 + vtable store + ~CBaseObject) where retail calls 0x007E74D0 (66, counted release expanded); ??_G STB 46 v 87 NEQ. The STB FORCEINLINE specialisation is load-bearing.
r4c hypothesis: the shared-header opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (dtor { ReleaseData(); }, Data-first; 354 landed users; FableWin 420 ReleaseData instantiations) replaces the row specialisations.
r4d hypothesis: the same shape through FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22.
r4c result: DIFFER(1491v1400): ~CCountedPointer inlined, ReleaseData called out of line; STB dtor still expanded into Main. r4d: same DIFFER(1491v1400).
r4e hypothesis: the existing header opt-in FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR (15 landed NScript TUs) makes Main call 0x007E74D0.
r4f hypothesis: only the STB ReleaseData / ~CCountedPointer FORCEINLINE pair is load-bearing (MovieBase specialisations dropped, = 00d05a60 g7 shape).
