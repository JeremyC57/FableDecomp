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
