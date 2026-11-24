# 0x00D11930 NScript::CQ_BanditCampBossBattleScript::CAssassinMarker::Main -- agent B24A3 notes (2026-11-24)
- differ/00d11930.cpp as handed over was the BEHAVIOUR TEST (graded DIFFER(1179v4072)); the real best draft is hand868's
  rebuild/candidates/manual/pending/2026-11-08_linux_hand868/differ/00d11930.cpp (now v1.cpp = differ/00d11930.cpp; copy in prior/hand868/).
  It needs hand868's header overlay (OVL=.../2026-11-08_linux_hand868/hdr; made from OLD headers -- re-apply its additions onto the
  current engine/CQ_BanditCampBossBattleScript.h before any hand-back). With it: DIFFER(4056v4072), 3 hunks: the ONLY residue is the
  cut-scene `movie` scope end 0x00D1284D (retail expands ~MovieBase: call 0x6E7AB0, GROBase vtable store, call ~CBaseObject; ours calls
  the COMDAT). Flags: stock /Ob1 /Ot same; /Os /O1 2801; /d2inlS15 4072v4072 but 102 hunks (frame permuted, as hand868 found).
- Loop-shape levers on the wait loop (b3f1 `!(A||B)`, b3f2 for, b3f3 for+break): byte-identical (3 hunks).
- STRONG EVIDENCE (inline trace, ../00d09010/inl/am_v1.txt): the cut-scene block runs at f=0.0156; in it ~CTimer (c1 14, site 130 = the
  block's terminate return) is CALLED in retail and ours, while ~MovieBase (c1 19, site 153, same f) must be EXPANDED in retail.
  c2 expands iff c1*(1/f+6)/7 < 74, monotone in c1, so with everything else byte-identical retail's implicit ~MovieBase must cost
  less than ~CTimer: c1 <= 13 (sz <= 19), i.e. "always inline" at stock. Same root cause as 0x00D09010 / 0x00D01630 (and the
  /d2inlS15 of landed BCGameMaster 0x00D067A0 / GangDeputy2 0x00D18DF0). Next: find the header form of MovieBase/GROBase
  (/CBaseObject / CCountedPointer<Movie>) whose implicit ~MovieBase own pipe is sz <= 19 while the expansion stays
  `call 0x6E7AB0 ; mov [m],0x0126008C ; call 0x0099A430` (B24A2 probe list: ../00d09010/NOTES.md; probe kit ../00d09010/b3/pr/q0.cpp
  + INL_F=ProbeMovie inl/run.sh, seconds per run). With it this row should match at stock with no device.
- /d2inlS15 on v1 (rc5.sh relocation counts): the callee set becomes EXACTLY retail's (only change v stock: the cut-scene movie
  expanded: +1 CP<Movie> dtor, +1 ~CBaseObject, -1 ~MovieBase call), yet 102 hunks: frame-slot permutation plus one block-order
  difference (a duplicated `push 1; push 1; ...; call [eax+0x814]` group near listing line 675). So the S15 world has retail's inline
  decisions here; the residue there is frame/layout. Probe (../00d09010/b3/pr/h2): an IMPLICIT GROBase dtor makes ~MovieBase sz 19 /
  c1 13 (always-inline at stock) but then the expansion loses the GROBase vtable store (0x0126008C) -> not retail; user-declared
  GROBase dtor forms (out-of-class inline h4, non-virtual spelling h5, __forceinline h6) all stay sz 25.
- Verified on retail bytes: the cut-scene block's terminate check 0x00D124EC (right after `cmp byte ptr [eax+0x61]` =
  AssassinCutsceneTriggered) jumps to 0x00D128B6, the exit that CALLS ~CTimer 0x00CD4470; the block's movie scope end 0x00D1284D
  EXPANDS ~MovieBase. Same static frequency in our trace (straight-line block). Strong evidence (not proof: assumes both share one f).
- Header: current engine/CQ_BanditCampBossBattleScript.h has only `class CAssassinMarker;` (forward); hand868's overlay class body
  for CAssassinMarker must still be re-applied onto the current header as an opt-in before any hand-back.

## B24A4 (2026-11-24) -- step 1 + cost model of the MovieBase chain
- Step 1 as written (h2 implicit-GROBase overlay on hand868 overlay, stock, v1): DIFFER(4168v4072) 55 hunks. CONFOUNDED: the implicit
  GROBase dtor also cheapens ~STB (sz 66->60): ~STB calls 7 -> 0, ~CBaseObject 11 v retail 8 (rc5.sh). Not a test of the hypothesis.
- MovieBase-only test (overlay s1b/: MovieBase derives from a test-only implicit-dtor base, so ONLY ~MovieBase gets sz 19 always-inline,
  no 0x0126008C store): DIFFER(4024v4072) 54 hunks: callee set = retail minus stores, but frame slots permuted from the first CIsThingAlive
  temp (0x3c v 0x70) and the STB-ctor scheduling hunk (listing lines 88-103). Forceinline world (MOVIEBASE_FORCEINLINE, extra MovieBase
  vptr store): 4120v4072, 79 hunks, same STB-ctor hunk, other permutation. /d2inlS15: 4072v4072 102 hunks. /Ob2, /Ob2 /Ot, /Oi: = stock 3 hunks.
  => every "MovieBase always-inline" world tried permutes the frame although the baseline (cut-scene site CALLED) matches retail's frame.
- c2 inline-trace cost table (probes pr/p2..p4, q0 base; sz = post-inline pipe size, c1 = sz - 6, always iff c1 <= 13 (S=9), T=74):
  ~MovieBase (implicit, declared-only CP<Movie> dtor spec) 25 | GROBase `virtual ~(){}` 11 | implicit GROBase 5 | class:GROBase{CP P} 25
  (same at any member offset) | class{CP P} at +0: 5, at +4: 8 | non-poly base w/ declared dtor + CP member: 19 | two CP members: 20 |
  poly class w/ CP member, no base: 8 | base `virtual ~L(){}` (store only, 7) + CP member: 19. => correct body = 8 (call CP(this+8)) + 11
  (GROBase) + 6 (this live across the call) = 25; every 19 form either loses the store or CALLS ~GROBase.
- P1 (generic inline CP<Movie> dtor, no specialization): ~MovieBase sz 66, nested ~CP 53 re-decided in the caller -> CP would be
  expanded at hot sites, retail calls 0x6E7AB0 at all sites: CP<Movie> dtor is not inline-visible in this TU (the repo model is right there).
- c2 order is bottom-up on demand: q1/q2 (GROBase dtor declared in class, defined inline after MovieBase / at TU end) -> still compiled
  before ~MovieBase, sz 25. q3 (MovieBase class brace under inline_depth(0)): ~MovieBase sz 19, but the caller inherits the body with
  ~GROBase CALLED (nested call not re-decided). So no source order gives "sz 19 + GROBase expanded".
- FableWin debug sizes (ghidra_out/fablewin_obj_functions.tsv; NOT in the signatures tsv): ~MovieBase 75 (= ours implicit, fwsize),
  ~STB 75, ~GROBase 28 (= ours), ~CP<Movie> 19 (= `{ ReleaseData(); }`), ReleaseData 265, ??_G MovieBase 44. Header text is right.
- NOTE: v1's 3-hunk frame match depends on its `#pragma inline_depth(0)` round ~CIsThingAlive (+ round the final check) and the two
  FORCEINLINE_CTOR defines: without them (v1nd.cpp) stock 136 hunks, s1b 145, S15 159. So the frame permutation of the always-inline
  worlds is NOT evidence against "~MovieBase always-inline in retail" (the frame was tuned by a device in the called world).
- Implicit ??0MovieBase = ??0STB = sz 26 / c1 20 (pr/q4): retail expands the movie ctor where it calls the 4 STB ctors (same block):
  the MovieBase ctor must be cheaper too -> class-level effect on MovieBase's implicit members, not a dtor text.
- S15 trace v stock (inl/s15.txt): exactly 3 decisions flip: ~CTimer 130 (f .0156), ~MovieBase 153, ~CTimer 160 (f 0). S15 is not an
  exact-IL test (two timer sites expand).
- ~CTimer cost (pr/t0..t11): sz is POST-optimisation (dead copies/compares vanish): every spelling with retail's /O2 bytes is sz 18..20
  (c1 12..14); `if (pDSTGame)` 23. So "retail ~CTimer costs 20+" is unlikely, and likewise retail's ~MovieBase pipe IS 25 if it had been
  compiled before Main. Large-TU probe (1500 extra functions, pr/big1): no change.
- CONJECTURE (fails on this row, see below) -- model for the pick-any copies (0x006E7B80 / 0x00E241B0 fully expanded, Main's expansions
  calling the member dtor): Main inlined the RAW (not yet compiled) bodies of ~MovieBase / ??0MovieBase / ~CIsThingAlive (raw sz 19 ->
  always; nested calls re-decided: GROBase always -> expanded, CP<Movie> declared / ~CScriptThing c1 60 cold -> called), while STB's
  members were compiled before Main. Our c2 always compiles callees first (q1/q2/q4/p5/big1), so no header text reproduces it.
  The closest landable devices: inline_depth(0) at the class brace / definition (q3: sz 19 but nested GROBase NOT re-decided -> call).
- The raw-body conjecture FAILS at 0x00D11964 (SetPredicate temporary, f = 1.0): a fresh re-decision of the nested ~CScriptThing
  (c1 60) would score 60 < 74 and expand, yet retail calls 0x004AA840 there. So nested callees in Main's expansions are not freshly
  re-decided at site frequency either; the v1 device (inline_depth(0) at ~CIsThingAlive's definition) reproduces it by inheriting a call.

## B24A4 MATCH (2026-11-24): RELOCATION_MATCH, vland PASS, no WARN/RELOC_ lines
- What made it match: v1 (hand868) + /d2inlS15 (the landed family flag: every MovieBase dtor site always-inline) + ONE more scoped
  `#pragma inline_depth(0)` round the cut-scene block's `if (IsActiveThreadTerminating()) return;` (S15 alone expands ~CTimer there,
  retail calls it at its one shared exit 0x00D128BA; the BanditKing 0x00D0A830 shape). v2.cpp = that; v4.cpp = hand-back source
  (FABLE_FORCEINLINE spellings, Inline-budget/Flags lines, map::operator[] declaration-only so the object has no leaf-referenced NEQ copy).
- Pick-any (v4): NEQ, none leaf-referenced: ??1MovieBase 0x006E7B80, ??4STB 0x008ABD10, ??1pair 0x00CD27B4, ??1CIsThingAlive 0x00E241B0,
  _M_erase 0x00CD344A, clear 0x00CD3699 -- all in Account A's audit list (family debt, queued). Before the operator[] declaration,
  ??A map 0x00CD3D2E was a LEAF-REFERENCED NEQ.
- Test: test_00d11930.cpp (v0's model; added a zeroing World() ctor: VC7.1 does not value-initialise a non-POD, so `World()` left
  garbage and the test failed under vt.py although g++ passed). vt.py PASS.
- payload.json / oracle.tsv written (flags /d2inlS15); hdr/README.md lists the one opt-in.
- LESSON: in the BanditCamp family, a cold movie-scope MovieBase dtor that stock calls is the S15 family (no header form found -- why retail's MovieBase ctor AND dtor behave always-inline is unexplained; see
  hdr/README.md); the S15 side effect -- ~CTimer expanded at cold terminate exits -- is undone with inline_depth(0) round exactly the
  checks whose exits retail routes to its CALLED ~CTimer (find them: `call 0xcd4470` site in dis_all.txt and the jumps into it).
