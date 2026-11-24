# 0x00D09010 CCampHostageGuard::Main -- agent B24A notes (2026-11-24)

## Lessons that apply
1. Wine aborts (free(): invalid pointer / rc -11) while TMPDIR is set: `unset TMPDIR TMP` in every call (see ../00e20600/NOTES.md).
2. c2 inline trace works on the lane: $W/inl/run.sh <src> <tag> <overlay> [flags] (agent3009's tool; ~3 min here), then
   `python3 $W/inl/an.py $W/inl/<tag>.json Main@CCampHostageGuard` lists every plain-inline decision (sz, savings, f, score; r=1 expanded iff sc < T 74).
3. Row 1 (0x00E20600) showed Main inherits each inline callee's own-pipe body (bottom-up); a device that changes a callee's own pipe
   also changes the TU's pick-any COMDAT of it (check against the owner address before relying on it).

## State
- Baseline (Codex draft: ~CTimer defined NON-inline in the row under auto_inline(off), /d2inlS15): DIFFER(5672v5864), 17 hunks.
  The 192 missing bytes are retail's two hot exits that EXPAND both ~CTimer (pDSTGame->DeregisterTimer, slot 0x160):
  retail epilogues: (1) movie exit [both chat arms tail-merge into it]: ~CCountedPointer<Movie> call, ~CBaseObject call, Deregister x2
  inline, ~CScriptThing call, ~ScriptedThingBase call; (2) the hit-by-hero `while (true) { NSF; if (terminating) return; }` exit: Deregister
  x2 inline; (3) common exit: ~CTimer 0x00CD4470 called twice.
  Note: the draft's out-of-line non-inline `CTimer::~CTimer()` body in the row TU is a strong definition of the landed/owned 0xCD4470 symbol
  (pick-any / LNK2005 risk) -- prefer plain inline.
- Plain `inline CTimer::~CTimer()` (v1): the trace shows ~CTimer expanded at exactly retail's exit groups (sites 88-89 / 110-111 sc 40 = the
  two movie exits, 127-128 sc 20 = while(true) exit; all others sc >= 76 called) and our three epilogues have retail's shape. But the whole
  function re-allocates: stock 5944v5864 (231 hunks), with /d2inlS15 6016. Main visible difference: the GoadLevel switch -- retail stores each
  constant to [esp+0x40] per case (`mov dword ptr [esp+0x40], 6; jmp`), ours keeps GoadLevel in ebx and stores once; case-block order differs.
- v2 (natural + no Movie/WideScreen forceinline ctors) 5800/239; v3 (natural + plain CTimer accessors) 5928/232; v4 both 5768/239.
- Best next idea: keep natural inline ~CTimer (structurally right) and work the GoadLevel switch / register pressure (the switch arms'
  `GoadLevel++` vs constant stores; GoadLevel's type/scope), then re-add devices one at a time with the trace.

## Callee counts (relocations, $W/rc.sh <src> [flags]; retail from dis_all.txt in retcalls.txt)
retail: ~CScriptThing 16, ~CCountedPointer<Movie> 9, CBaseObject() 6, ~CBaseObject 6, ~ScriptedThingBase 3, ~CTimer 2, ~MovieBase 0.
- v0 baseline (/d2inlS15, out-of-line ~CTimer): 14 / 7 / 6 / 5 / 1 / 2 / 0  (exits with expanded timers missing).
- v1 natural inline ~CTimer, stock: 16 / 4 / 6 / 2 / 3 / 2 / ~MovieBase CALLED 8  -> MovieBase (implicit dtor, sz 25) split by frequency.
- v1 + /d2inlS15: 18 / 10 / 6 / 7 / 4 / ? / 0 -> MovieBase now always expanded (right) but ONE extra exit copy (+2 ~CScriptThing, +1 CP<Movie>,
  +1 ~CBaseObject, +1 ~Scripted): retail tail-merges the two chat-arm movie exits (first arm: `mov [esp+0x5c],0x126008c; lea ecx,[esp+0x5c];
  jmp` into the second arm's `call 0x99a430`), so the two arms' cleanups must become identical up to that call. Best next idea: v1 + /d2inlS15
  and find why our two movie-arm exits are not cross-jumped (movie slot / timer expansion order in each arm), then the GoadLevel switch.
- Why S15 overshoots: /d2inlS15 makes ~CTimer (c1 14 <= 15) always-inline -> v1+S15 expands the timers at FOUR exits (retail two + one common
  called exit). Why stock undershoots: implicit ~MovieBase (sz 25: declared-only CCountedPointer<Movie> dtor call + ~CScriptGameResourceObjectBase
  sz 11) is frequency-split (sc 54 at hot sites, called at cold ones) where retail expands it at every site. Retail therefore has a CHEAPER
  MovieBase destructor (c1 <= 9 always-inline at stock S9, i.e. sz <= ~15) with ~CTimer staying frequency-split at stock flags. Next: find the
  MovieBase / GROBase / CBaseObject destructor declaration form (implicit vs user-declared, virtual or not, which bodies visible) whose trace
  sz is <= 15 while the expansion stays `call 0x6E7AB0; store 0x0126008C; call 0x0099A430`; then stock flags, no S15.

## B24A2 (2026-11-24) -- continuation
- v5/v6/v7 (do-while(0) round the speak block / its T checks / every T check): 6152 / 6120 / 6284, ~MovieBase still called 7x. Dropped.
- Probes (probe/*.cpp, trace via INL_F=<fn> inl/run.sh): the implicit ~MovieBase is sz 25 on first use in a TU whatever the CP<Movie>
  dtor form (declared-only 25; visible body 66). Base forms: out-of-line base dtor call -> derived 19; inline base `{}` w/o base call 19;
  GROBase-like base (vptr store + ~CBaseObject call, sz 11) -> 25; base defined after use / inline keyword: no change.
  Always-inline (sc '-') at stock: c1 13 yes (sz 19), c1 14 (CTimer dtor) no; /d2inlS15 makes c1 19 always.
  An implicit dtor first compiled under #pragma inline_depth(0) is then sz 19 always-inline elsewhere but KEEPS GROBase as a call (K3).
- Decisive argument: retail's movie-arm terminate exits (0xD0A1B9 -> 0xD0A4C8 -> 0xD0A60F -> common exit 0xD0A693) destroy the movie
  EXPANDED and the timers CALLED on one path. Same statement = same f, and MovieBase (c 19) always scores worse than CTimer (c 14), so
  no frequency / flag gives that from one `return`: either ~MovieBase always-inline (needs sz <= 19) or the movie is destroyed by a
  different statement than the timers (CBanditKing 0x00D0A830 `goto Terminate` + inline_depth(0) on `Terminate: return`).
- pick-any on v1: ??1MovieBase NEQ (leaf-referenced), ??1CIsThingAlive NEQ v 0x00E241B0 (row-local inline dtor) -> must be fixed.
- Compile-order probe: ~MovieBase stays sz 25 even when an earlier function calls it out of line (o.cpp). Hypothesis (A) dead.
- v8 (S15 + every cold T after the locals -> `goto Terminate`, `Terminate:` return under inline_depth(0); keep plain returns at the
  three hot exits, v1 lines 199/217/235): 5868v5864, callee counts == retail (~CScriptThing 16, CP<Movie> 9, CBaseObject() 6,
  ~CBaseObject 6, ~CTimer 2) but frame layout off (gotos reweight).
- v10 (S15; movie-scope cold T (v1 lines 202,220,307,310,324,327,344,347) -> goto Terminate; other cold T -> return wrapped in
  #pragma inline_depth(0)): 5864v5864, 40 hunks: GoadLevel/Hostage slot swap (ours GoadLevel 0x34/Hostage 0x38; retail Hostage 0x34,
  GoadLevel 0x40) + arm1-after-loop exit tail-merge + block order. Retail ret positions reproduced.
- vE (= v14; same but the 8 movie-scope exits are returns under #pragma inline_depth(1): movie expanded, GROBase expanded, timers
  called): 5864v5864, slot layout == retail, callee counts == retail except ~CBaseObject 5 v 6 (one more merge); residue = block
  placement: the two hot expanded-timer exits (arm2 in-loop 0xD09DE6.., hit-by-hero 0xD09E50..) sit at the END in ours (merged
  tails), retail keeps them in place after the second switch; arm1-after-loop block cross-jump direction reversed.
- Mixed goto/depth variants vG/vI/vK/vL: 5800..6008, worse.
- CORRECTION on vE: its movie-scope returns under inline_depth(1) EXPAND the timers (depth 1 allowed, S15 always-inline) and the
  cross-jumper folds them into the hot tails, so the matching callee counts were a coincidence; vE is structurally wrong.
  v10 (differ/00d09010.cpp now) is the structurally right form: movie exits -> movie expanded, then the called-timer common exit.
- v10 residues (normalized diff, nd.sh): (1) density tie Hostage(12 B) v GoadLevel(4 B) (MATCHING_PLAYBOOK frame packer: density
  w*1000/size, w = IL refs before cross-jumping): the 8 gotos remove 8 Hostage dtor refs and GoadLevel wins; tried GoadLevel decl
  moves (v11/v12/vV), int (v13), constant stores (vH), function-scope Conversation (vT), Hostage first (vU 5800): none flips it.
  (2) cross-jump survivor of the movie@0x5C group: retail keeps arm1 after-loop block (0xD09DBA; 324/327 jump back to it = the
  playbook's "second pass, earlier survives"), ours keeps the LATE_SPEAK_SECOND copy (first-pass "last edge absorbs").
  (3) retail's two `pause; jmp` stubs (0xD0A4C8/0xD0A4DA) sit after the case-5 loop; ours elsewhere.
  Mixed goto/return(depth1) subsets (vG vI vK vL vO vP vQ): 5800..6024 (merges change), all worse.
- pick-any on v10/vW: ??1CIsThingAlive NEQ v 0x00E241B0 (25-byte copy; FABLE_RETAIL_CISTHINGALIVE_BANDIT_OWNERS_PASS27 in vW does
  not fix it under S15) and ??1MovieBase NEQ v 0x006E7B80 (emitted via ??_7/??_G because the FORCEINLINE_CTOR stores the vptr; our
  copy calls 0x006E7AB0, retail's inlines the release). Both must be solved before any hand-back (probably lane-level: check how
  the landed BanditCamp siblings' objects fare with the checker).
- Best next ideas: (a) find a spelling for the 8 movie exits that keeps a Hostage reference per exit but still leaves the movie scope
  separately (e.g. per-site `{ ... }` scope ends / break out of a do-while(0) movie block followed by the existing next statement)
  and test the frame map with fmap.sh first (no grade needed: Hostage must precede GoadLevel); (b) for the survivor, the playbook
  rule says the earlier copy survives only in the second pass: make arm1's after-loop exit (v1 line 202) a return-shaped edge.
Tools added in this dir: xf.py / xg.py / xh.py / xk.py (source transforms by v1 line numbers), nd.sh (normalized diff, relocations
masked), fas.py + fmap.sh (/FAs frame map of Main), probe/ (inline-trace probes; inl/run.sh now takes INL_D / INL_F env).

## B24A3 (2026-11-24) -- continuation
Tools added: g3.sh <src> "<note>" [flags] (fg grade + full q listing q_<b>.txt + nd.sh stats), pa.sh <addr> <src> [flags]
(q.py compile + pickany_vs_retail.py; OVL=/nonexistent for landed rows). Note: nd.sh needs a q.py listing, not fg output.
- Regrade v10 (S15): DIFFER(5864v5864) 40 hunks (unchanged). v10 at STOCK: 5944, ~MovieBase called 8x (goto sites too): S15 needed.
- Slot-margin probes (b3/): pk1..pk8 = v10 + k extra UNREACHABLE `if(T) return` after a while(true): byte-identical (c1 drops
  unreachable code; such statements do not count). gr-k = last k of the 8 movie gotos -> plain return: gr1..gr4 slots still swapped,
  gr6 flips to retail order (Hostage 0x34 / GoadLevel 0x40; count of `[esp + 0x40], N` lines in q_*.txt = 22). So retail has ~5-6
  more reachable Hostage-destroying exits than v10 => the movie-scope exits are most likely plain `return`s in retail.
- Pick-any on LANDED siblings (pa.sh, landed sources, no overlay): BanditKing 0x00D0A830 and BCGameMaster 0x00D067A0 both emit
  NEQ ??1CIsThingAlive (v 0x00E241B0) and NEQ ??1MovieBase (v 0x006E7B80), "not referenced by the leaf"; CCROWDBANDITS 0x00D0B3E0
  NEQ ??1CIsThingAlive; CCampHostage2 0x00D08790 NEQ ??1CIsThingAlive AND a LEAF-REFERENCED NEQ ??1CTimer (10 B v 0x00CD4470).
  => same defect family-wide; per EXTRA_RULES 2026-11-21 "not leaf-bound and bound at HEAD: debt, queued" (coordinator decides).
- Slot margin, GoadLevel side (b3/gl*, gL*): dropping ONE `GoadLevel++` (early or late switch) keeps the swap; dropping TWO flips to
  retail's order (gl2: 13 hunks!). Unweighted (early/late same). So retail = v10 with ~2 fewer GoadLevel IL refs or ~6 more Hostage
  refs (density w*1000/size, 12 v 4 bytes). vH (const stores) = v10 slots, so `++` and `= c` are one ref each.
- In-class (no `inline` keyword) and non-inline out-of-class ~CTimer bodies (b3/t1, t2; overlay b3/hdrT): identical to v1 at S15
  (6016, every ~CTimer expanded) and at stock (5944, MovieBase called 8x). The "-7 inline-declared" term does not separate them.
- inl trace v1 at /d2inlS15 (inl/v1s15.txt): every MovieBase AND every ~CTimer site r=1, pen=0 everywhere -> no caller-size
  penalty under S15; at stock (inl/v1.txt) the split is pure frequency: ~CTimer c1 14 expands iff f > ~0.032 (early in-loop 0.0707,
  early hit loop 0.2459 = retail's three hot exits exactly), MovieBase c1 19 iff f > ~0.047 (late sites f<=0.0003 -> called).
  Retail = MovieBase expanded at EVERY site + ~CTimer frequency-split, i.e. retail's movie dtor behaves always-inline at stock
  while ~CTimer does not; with our headers that needs sz(MovieBase) <= 19 (stock "always" iff sz <= S+10), which B24A2/Codex only
  got with GROBase called (inline_depth captured at the class brace). Open: the header form that gives an always-inline implicit
  ~MovieBase with GROBase still expanded; with it, plain returns everywhere at STOCK flags would likely give retail's slots too.
- Codex pass 25 (00d01630, same family, had FableWin donor evidence: 123 decisions / 294 lifetime events matched) writes PLAIN
  returns at every movie-scope terminate check -> supports "returns", not gotos, as the original text.
- STRONG EVIDENCE (from 0x00D11930; exit target verified on retail bytes, whose draft is byte-identical except one movie site): at one f, retail CALLS ~CTimer (c1 14) and
  EXPANDS ~MovieBase (c1 19) -> retail's implicit ~MovieBase is cheaper than ~CTimer (c1 <= 13 = always-inline at stock). This is
  the family root cause (rows 00d09010, 00d01630, 00d11930). With such a header form, v1 (plain returns, plain inline ~CTimer) at
  STOCK flags is expected to give retail's exits AND retail's slots (gr6 / vE showed returns give retail's slot order). Probe kit:
  b3/pr/q0.cpp, `INL_D=$W/inl INL_F=ProbeMovie bash inl/run.sh b3/pr/q0.cpp <tag> $W/hdr` (~20 s): baseline sz 25 c1 19.
- Header probes (b3/pr/h2..h6, overlay of CScriptGameResourceObjectBase.h, trace INL_F=ProbeMovie): implicit GROBase dtor -> ~MovieBase
  sz 19 c1 13 ALWAYS at stock, but no GROBase vptr store in the expansion (retail has `mov [m],0x0126008C`); user-declared GROBase
  dtor in-class / out-of-class inline / non-virtual spelling / __forceinline -> sz 25 c1 19. ~MovieBase sz = 14 + sz(~GROBase);
  the vptr store costs 6 of GROBase's 11. Still open: a form with the store whose cost is <= 5, or a cheaper CP<Movie> member part.

## B24A4 (2026-11-24)
- 0x00D11930 MATCHED with /d2inlS15 + inline_depth(0) round the one terminate check whose exit retail sends to the CALLED ~CTimer
  (that exit has no movie in scope). Here the same recipe cannot work: v1 (plain returns, plain inline ~CTimer) at S15 = 6016v5864,
  147 hunks, DeregisterTimer (slot 0x160) expanded 9x v retail 4x. The extra 5 are most likely cold exits inside movie scopes (inferred, not mapped site by site; measured
  support: B24A3's v10 goto runs and 00d01630's a1, where moving movie-scope exits to a shared pragma'd return fixes the timer count but
  reshuffles the frame), where retail expands ~MovieBase AND calls ~CTimer at the same return: inline_depth(0) on that return blocks both, S15 expands both, stock calls both.
  Only a cost reversal separates them: ~CTimer c1 > 19 while ~MovieBase <= 19 (S15). Our ~CTimer is c1 14 and every byte-identical
  spelling measured is c1 12..14 (00d11930/NOTES.md, pr/t0..t11). FableWin's four CTimer bodies are each +1 /Od instruction (+2 bytes)
  longer than ours: lane B (b24b/SHARED_FORM.md) owns that text. If it yields a ~CTimer of c1 20..23 at /O2 with retail's bytes,
  v1 + S15 should give retail's exits here (00d09010 hot exits f 0.0707 / 0.2459 still expand for c1 < 25.7). Blocked on that.
- Lane B (b24b/SHARED_FORM.md, B24B3): today's CTimer text is FableWin-hash-exact (the +1 /Od instruction is VC10's split virtual
  call), ~CTimer stays c1 14: the cost reversal is NOT on the CTimer side either. Row stays blocked (Ego_d questions in ESCALATE.md).
