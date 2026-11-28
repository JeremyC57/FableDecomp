g1: remove the strong Alloc / ctor / GetParentScript bodies (landed rows 00D05910 etc. own them: LNK2005) and make Main out-of-line; hypothesis: Main bytes do not depend on them.
g2: c0 with Main out-of-line only (Alloc/ctor/GetParentScript kept); separates the two g1 effects.
g3: g1 (no strong Alloc/ctor/GetParentScript, out-of-line Main) + the GOTCHAS-374 budget-inliner device (/Og- /d2vol10 + optimize("g",on)); hypothesis: the inline-Main expansions are the budget inliner path.
g4: probe only: c0 with the strong Alloc / GetParentScript replaced by an anonymous-namespace scaffolding helper that constructs CAssassin2 (emits the vftable and the inline Main); hypothesis: Main stays 1400 and no strong duplicate remains.
results: c0 RELOCATION_MATCH but defines ?Alloc@CAssassin2 strong (landed row 00D05910 owns it: a new LNK2005) and GetParentScript strong.
  g1 DIFFER(1365v1400) (no Alloc/ctor/GetParentScript, Main out of line); g2 DIFFER(1365v1400) (Main out of line only) -> the
  `inline` Main (compiled at the end of the TU, emitted through the vftable) is what makes the leaf match: out of line, VC7.1 calls
  the CIsThingAlive ctor/dtor and the scripted-thing base ctor instead of expanding them (udiff g2.out).
  g3 DIFFER(1842v1400) (/Og- /d2vol10 device): not the path; no further flag probes (no sweeps).
  g4 RELOCATION_MATCH 1400: inline Main kept, the strong Alloc replaced by an anonymous-namespace scaffolding helper that constructs
  CAssassin2 (vftable 0x012C8D28 emitted, 7/7 slots agree); no strong duplicate left; pick-any EQ; movie ??_G 46 = HEAD debt.
  Needs a ruling (ESCALATE.md): inline-Main + constructing scaffolding helper.
Gates 2/6/7 (no grade): t1_test.cpp = Codex fixture minus its embedded Main, typed-ctor macro set to the overlay name; vt.py with g4.obj PASS, without LINK_FAIL. purity genuine. name_gate (with --test) RELOCATION_MATCH + NAME_GATE PASS.
## Round 2 (coordinator, 2026-11-27)
Facts: FableWin.pdb (FableWin.4k.pdb, module 11 Debug\register_scripts.obj) S_FRAMEPROC flags: CAssassin2/3 Alloc, ctor, GetParentScript,
Init and Main are all "marked inline"; register_scripts.cpp's own LoadDataFromMarker / ConsolePlayCutscene / RegisterScripts are not.
FableWin line blocks (q_banditcampbossbattle.hpp): Alloc, ctor and GetParentScript all at line 987 (one macro line); Init 990-994; Main
997-1037; CAssassin3: 1043 / 1046-1050 / 1053-1092. Ego_d egod_lines: Main file id 4944 lines 997-1037 (CAssassin3 1053-1092).
No PDB on disk has an IPI stream (Ego_d, Ego_r.4k, FableWin.4k: Has IDs false), so no LF_UDT_SRC_LINE records exist.
g5 hypothesis: Main in the class (new add-only overlay opt-in FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_INCLASS_MAIN_PASS30 in
CQ_BanditCampBossBattleScript.h) gives the same bytes as g4's out-of-class inline Main; the constructing helper still triggers emission.
g6 hypothesis (coordinator item 2): Main out of line (g1 source: no helper, no strong Alloc) with the /d2inlS15 the same-TU landed Main rows carry (CAssassin1::Main 0x00D04AE0, CBCGameMaster::Main 0x00D067A0, build_candidates.ps1).
g6 result: DIFFER(1365v1400), the same as g1 without the flag: /d2inlS15 does not reach the out-of-line Main.
g7 hypothesis (coordinator item 3): landed 0x00E70BC0 (CV_DemonDoorsScript::CDF_DoDamage::Main, flags base) emits the retail 87-byte ??_GCScriptGameResourceObjectMovieBase; it has no CCountedPointer<MovieBase> specialisation (header default). g7 = g4 minus the row's MovieBase ReleaseData / ~CCountedPointer specialisations.
g7 result: RELOCATION_MATCH 1400, movie ??_G still 46 (the row's MovieBase counted specialisations are not the cause). Witness: HEAD 0x00E70BC0 compiled here emits the 87-byte copy EQ (head/00e70bc0.obj).
g8 hypothesis: the 00E70BC0 state's other opt-in, FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR, on top of g7.
g8 result: RELOCATION_MATCH 1400, ??_G movie still 46. Item 3 summary in PAIR_FACTS.md. In-class route g5 also: test PASS (g5.obj), purity
genuine, NAME_GATE PASS (NAME_GATE_g5.txt).
Round 3: R1 fails (see PAIR_FACTS.md round 3); the pair stays escalated as object-mode.
