# 0x00D01630 NScript::CQ_BanditCampScript::CGate1GuardOuter::Main -- agent B24A3 notes (2026-11-24)

## Lessons that apply (from 0x00D09010, same family)
1. Wine: `unset TMPDIR TMP`; `mkdir -p /run/user/0` if free(): invalid pointer.
2. Retail pattern of the family: implicit ~CScriptGameResourceObjectMovieBase expanded at EVERY site, ~CTimer split by static
   frequency (expanded only at the hottest terminate exits, called through one shared exit elsewhere). With our headers the movie
   dtor is sz 25 / c1 19 and ~CTimer sz 20 / c1 14 (inline trace), so: stock flags = timers right, cold movie sites called;
   /d2inlS15 = movies right, every timer expanded. No single flag gives both; no per-site pragma separates them inside one return.
3. Frame slots follow IL reference counts before cross-jumping: turning returns into gotos (BanditKing 0x00D0A830 device) removes
   one destructor reference per local per site and reshuffles the frame (see a1 below).

## State
- Codex pass 25 source (prior/00d01630.cpp; differ/00d01630.cpp) needs its open header FableScriptInlineBanditPass25.h; v0.cpp is the
  same source with that header's block inlined row-locally (CTimer ctor/accessors, ~CTimer under auto_inline(off), CIsThingAlive
  ctor and dtor). Baseline v0 /d2inlS15:
  DIFFER(5161v5178) 29 hunks = Codex's grade. Residue: retail expands ~CTimer (DeregisterTimer slot 0x160) on ONE shared exit
  (0xD02851, plus stub 0xD02865) reached from the four hottest checks (loop-top T 0xD01769 / 0xD02844, T after
  `if (!ParentClass.Gate1Open) {` 0xD01786, T after its `} else {` 0xD025C2); v0 calls ~CTimer there (17 bytes short).
- b3/n0.cpp = v0 with plain inline ~CTimer (no auto_inline pragma). S sweep: stock 5134 (Deregister expanded 2x = retail's two,
  but 4 MovieBase sites called), S10..S14 5138, S15..S40 5247, S60 5720. T sweep at stock (T80..1000): 5233..5503, no window.
- b3/a1.cpp (tx.py goto): natural ~CTimer + S15 + depth(0) on cold returns outside movie scopes + 32 movie-scope cold T -> goto
  Terminate (depth(0) return): 5161v5178, 221 hunks: Deregister expansions == retail (2) but frame 0x14c v 0x150 and slots shuffled.
  b3/a2.cpp (tx.py depth: movie-scope T also depth(0) returns): 5008 (movies called).
- Best remains v0 (= Codex). Best next idea: the family-level question in ../00d09010/NOTES.md (B24A3): which header form makes the
  implicit ~MovieBase always-inline at STOCK (c1 <= 13) while it still expands GROBase (vptr store + ~CBaseObject call); with it,
  n0.cpp at stock flags is expected to match retail's exits.

## B24A4 (2026-11-24)
- Same block as 00d09010 (see its B24A4 section): movie-scope cold exits need ~MovieBase expanded and ~CTimer called at one return;
  S15 + inline_depth(0) separates them only where no movie is in scope (that is how 0x00D11930 matched). Waits on lane B's CTimer
  text (a ~CTimer of c1 > 19 with retail's bytes would make n0.cpp + /d2inlS15 the candidate).
- B24A5: existence-proof search done (ESCALATE.md, shared with 0x00D09010): only precedent is the goto form (a1 here). Blocked.
