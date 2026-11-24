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
## B24B4 (2026-11-24): regrade on the corrected oracle (4507): DIFFER(4485v4507) 18 hunks (r0.cpp = differ/ unchanged) -- residue as B24B3 said
- Trace kit copied to inl/ (tr.sh <src> <tag>: 20 s c2 trace, prints Return / ~MovieBase / ~STB decisions with f; + expanded, - called).
- NEW: a UNIFORM scale r of the whole main-loop body frequency fits every inline site at once: r in (0.32, 0.403) (helmet ~MB .1159r < .0467,
  armour .1545r > .0467, CScriptThing copy ctor .2197r > .068, STB/MB ctors .1562r > .0503, all called STB sites stay called). Our loop body
  is 2.5 (first R in the loop); retail looks like ~0.8..1.0, i.e. no loop weight. The slot-order residue (STB v MovieBase density) is
  probably the same frequency fact (both block-weight driven) -- untested.
- Tried, frequencies UNCHANGED (c2 finds the loop from the CFG): for(;!c;), if(!c) do{}while(!c), for(;;){if(c)break;}, while(true){if(c)break;},
  do{if(c)break;}while(true), goto loops (G1/G2). Bytes equal (18..22 hunks).
- Tried, bytes change: for-j / for-i as `while(j<LAST){...++j;}` (w1/w2/w3): cools the code after the loop ~10x (too cold), 3683..4000 b.
- else -> `else if (!collected_all)` (a1): else-block x0.5 -> helmet .0579 still expanded (need < .0467); swapping if/else flips layout.
- Next idea: what source makes c2 give the main loop body weight ~1 (r 0.32..0.40): the loop factor 2.5 may come from its exit count or nesting;
  or this is retail-object compile context (owner 2026-11-24 build mode) -- not proven, so not escalated as compile order.
- Bound note: the helmet ~MovieBase site is the ONLY upper bound on r; every other site is a lower bound. So shapes that cool only the
  else block or only the 4th arm (x < 0.403 there, armour arm >= 0.302) are as valid as a uniform loop scale.
- ONE CAUSE (B24B4 diagnostic): `#pragma inline_depth(0)` round only the helmet arm's terminate check (x1) or only its `return` (x4)
  -> DIFFER(4509v4507) 3 hunks: ~MovieBase now CALLED there (= retail) and the whole STB/MovieBase slot-order residue VANISHES. So the
  slot order is a consequence of the helmet ~MovieBase decision. Left: depth(0) also blocks ~CWideScreenMagicPauseEntities at that return
  (called + spill of its Game pointer at 0x00DCA1DE, eax/edx at 0x00DCA4F4); depth(1) same; depth(2) 171 hunks; a forced WS dtor (overlay
  hdrx FORCEINLINE_DTOR, diagnostic) is blocked by depth 0 too. differ/ = x4 (3 hunks; the device is NOT landable while WS is wrong).
  Next: a form that lowers ONLY the 4th arm's frequency below 0.403x (or the whole else block 0.30..0.40x) with no device; or a scoped
  control that blocks the implicit ~MovieBase but not the WS dtor.
## B24B6 (2026-11-24)
- Owner decision 2026-11-08 (EXTRA_RULES) allows inline controls for inline-budget residues, but no scoped control separates the two
  dtors at the helmet return: both ~WS (needed expanded, retail 0x00DCA575 PauseAll inline) and implicit ~MovieBase (needed called)
  sit at depth 1 of the same return full-expression; inline_depth(0) blocks forced bodies too (B24B4 hdrx).
- Per-row flags /d2inlT34 / T37 / T39 on r0 (threshold between armour sc 33.9 and helmet 39.7): 213 / 217 / 205 hunks -- no.
- Trace (inl/tr.sh): for->while on the j loop (w1) scales the collected_all block and the arms x0.1 (2.1973 -> .2197; arms
  .2747.. -> .0275..), i loop (w2) x0.067: too cold (needs x0.302..0.403). No source form found for that window.
- Cap ruled OUT: g at every ~MovieBase decision is 1239..2701 (r0 trace; max 2832) << 9000, pen 0 everywhere.
- Visible-body lever (DIAG v1.cpp): the CP<Movie> dtor as a VISIBLE `__declspec(noinline)` specialization `{ ReleaseData(); }` instead of
  the declared-only one: ~MovieBase still sz 25 / c1 19 at every site, same f, same decisions -> no lever on the MovieBase cost.
  (A single MovieBase c1 cannot fit both rows anyway: here helmet needs c1 in [36,41] at f .1159 v armour .1545; 0x00D0C410 needs it
  expanded at f .0003.) State unchanged: differ/ = x4 (3 hunks, diagnostic depth(0) device; WS dtor wrongly called).
