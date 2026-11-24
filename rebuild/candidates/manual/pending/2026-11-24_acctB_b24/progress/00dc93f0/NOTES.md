# 0x00DC93F0 CSecretPassageSkeleton::Main -- B24B3 notes
- Baseline regrade: DIFFER(4485v4714) 18 hunks (stock). The oracle cut runs past the function (int3 at 0x00DCA58B, next function at
  0x00DCA590): the real body is 4507 bytes, as on record. Real residue (rawcmp -v on q_base.log), two items:
  1. MISSING cold cleanup 0x00DCA575..0x00DCA58A (22 b): the HELMET arm's terminate exit (jne at 0x00DCA354, 4th else-if arm of the
     REMIND chain in the not-collected_all block) does PauseAll(false) + CALLS ~CScriptGameResourceObjectMovieBase 0x006E7B80, then
     joins the common tail; every other in-movie exit expands the movie cleanup.
  2. Slot order in the collected_all block (0x00DCA0A3..0x00DCA13D): retail MovieBase at esp+0x40.., ours at esp+0x50.. (Codex: "same-scope
     movie and hero slots reversed").
- c2 inline trace (inl/skel_base.txt, kit in ../00f1eed0/inl, INL_F=Main@CSecretPassageSkeleton): the four REMIND arms' ~MovieBase sites
  have f .2747 / .2060 / .1545 / .1159 (x0.75 per else-if, && = .25), c1 19, sc 26..39, all expanded (stock threshold sc < 74 <=> f > .047).
  Retail = armour expanded, helmet called -> retail's not-collected_all block entry frequency is ~0.30..0.40 of ours (ours E ~1.10 =
  loop body 2.2 x .5); a uniform x.5 or x.25 does not fit (helmet .058 expanded / armour .039 called). The retail bytes of the chain
  (0x00DCA20D.. possession test, returned[] test, terminate check) equal ours, so the lower frequency comes from control flow outside
  the chain (loop construct / branch structure of `while (!collected_all)` and `if (collected_all)`), not from the arms. Next idea:
  read the Lua port (work/aeon_lua_ports, OpeningGraveyardSecretPassage) for the original loop shape, then trace f again.
