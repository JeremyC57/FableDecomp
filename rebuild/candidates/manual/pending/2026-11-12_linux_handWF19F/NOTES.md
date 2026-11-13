## 0x00D17A80

## 0x00D17A80 NScript::CQ_BountyHuntScript::WatchForBoasts -- DIFFER(705v785), 4 hunks (wf19 author, 2026-11-12)
Best source: differ/00d17a80.cpp (ported onto shared headers: CQ_BountyHuntScript.h + overlay opt-in FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS,
CLevelScriptBase Game/MasterScriptData, CGameScriptInterfaceBase, CQ_SunnyvaleMasterData::BountyHuntTimeLimitExceeded, stlport vector,
CSpawnedFunc; vector ctor 0x0074F680 declared as an explicit out-of-line specialization; CTimer::GetTime inline via g_pGameScriptInterface).
Fixed on the way: the spawn temps' frame slots (name at esp+0x18, region "" at esp+0x1c) need the IMPLICIT conversion for the name --
`new CSpawnedFunc<CQ_BountyHuntScript>("WatchForQuestInfoDisplayConditions", 0, this, &...)` with an explicit `CCharString("")` region arg.
Residue: only the boast-vector destruction at the 5 early returns. Retail: 5 separate exits; for-top return = inline elem loop calling
??_G pair 0x004DA68A(0) + ~_Vector_base 0x004D9C5D call; the two arm returns = inline ~CCharString loop + 0x004D9C5D call; expiry and
post-loop returns fully inline. Ours (default flags) inlines everything and tail-merges all exits (705 bytes).
Proven with /d2inlT sweeps (T1..T80): our compile spends the inline budget on the return sites in FORWARD source order (site1 most
inlined, last site first to fall back to a ~vector / __destroy_aux call; b1 while-loop form at T57 = sites 1-4 full, site 5 calls ~vector).
Retail's split is the exact REVERSE order (and retail even calls the trivial vector ctor out of line, the earliest call of all) -> the
retail inliner walked this function bottom-up. Also: ours never emits the ??_G(pair) call at any T (it rejects __destroy_aux before ~pair).
Tried (no match): /d2inlS 1..40 (no effect), /d2inlT 0..80 on for/while forms, T+S combos, /Ob1 /Ob2; vector ctor inline (always inlined,
retail calls it); forceinline overlays for ~vector / _Destroy(range) / __destroy / __destroy_aux / _Destroy(T*) (then everything inlines);
plain-inline CSpawnedFunc ctor; loop forms: for(++i / i++), while with ++i at body end, do-while with guard, `<` vs `!=`, int index,
boast declared outside the loop, outer for(;;)+check; switch for the 15/16 arms; nested ifs for the expiry check; inverted
term-check polarity (`if(!term){...}else return;`, normalised by c1).
Best next idea: find what makes VC7.1 process inline sites bottom-up (a probe: tiny function with 3 early returns of a vector at
/d2inlT~40 under candidate source shapes: function defined in-class / as a template member / after a `#pragma auto_inline`, local
class wrappers, the script macro shape). QFE-4035 checked (
probe done: QFE-4035 default build of v1 and b1 = same 705-byte all-inline result, so the compiler build is not it).

## attempt 2 (wf19 author, 2026-11-12) -- still DIFFER(705v785), same residue; best source unchanged (differ/00d17a80.cpp)
New facts (probes, logged in ATTEMPTS.log):
- `for (; !term; Game->NewScriptFrame()) { vector ...; if (term) return; }` (x1.cpp, vector in the loop-body scope) compiles to the SAME
  705 bytes as the while + inner-block form: an equally valid spelling, no gain.
- Per-statement `#pragma inline_depth(0)` round the for-top return makes that site call ~vector whole; inline_depth(1..7) there =
  fully expanded (the scoped pragma does not limit nested inlinee depth, so it cannot give the partial ??_G / ~_Vector_base mix).
- c2.dll 13.10.3077 inliner switches (from its option table): -inlT -inlS -inld -inlld -inlcrmax -inlnobr -inlnold -inlnoln -inlnild
  -inlniln -inlflcsw -inlfcsw -inlocsa1..4 -inlfcsa -inluserinl -isize. Swept /d2<opt>{0,1,4,16,100} x T{40,50,60}: only inld/inlld move
  anything; none gives a ??_G(pair) call. /d2inlS below default (S0..S5) = default (floor); S only RAISES the budget.
- Budget model in OUR compile (probes pr2/pr5): a cumulative, frequency-weighted budget spent in FORWARD order. 10 extra in-loop
  exits: the first ~6 in-loop exits expand fully, later ones call ~vector; 14 extra exits after the for loop: all expand (cheap,
  cold). Ours never rejects the inline vector ctor at any T, and never emits the ??_G(pair) leaf call at any T (54 generated
  structural variants x T20..80, gen.py / big1.txt: outer while/for/do, elem read via [] twice / reference / direct, for/while/`<`
  index loops, ctor inline or declared).
- Retail's split fits exactly a budget spent in REVERSE IL order with depth-first expansion inside each inlinee (normal-end,
  post-loop, expiry full; the two arms: chain incl. ~pair inlined, then ~_Vector_base out of budget; for-top: chain down to
  _Destroy(T*), ~pair (??_G 0) and ~_Vector_base out; ctor last, called). No flag tried changes our order: /Ob1 /Og- /Oi- /Ot- /Oy- /Gy
  /Ox /G5-7 /Oa /Ow /GF /GR /Op /arch:SSE /EHsc(adds EH frame) x T sweeps; an `inline` address-taken definition; a TU-filler function
  before/after (budget is not TU-wide).
Best next idea: find the source fact that makes VC7.1 spend the budget bottom-up -- most likely the ORIGINAL TU context (the function
compiled after the rest of CQ_BountyHuntScript.cpp: Main 0x00D13580 etc. instantiate the same vector/pair COMDATs first, so c2 may
already hold compiled bodies of ~vector / ~_Vector_base / ??_G pair and cost them differently) -- needs Main or another real same-TU
user of vector<pair<long,CCharString> > reconstructed into the TU (no stand-ins). Alternatively disassemble c2.dll inline.c's
site ordering (string "INL:\tInlining %s (%d instrs) into" marks the logger) to learn what orders call sites.

## attempt 3 (wf19 author, 2026-11-13) -- still DIFFER(705v785), same residue; best source unchanged (differ/00d17a80.cpp)
New facts (all graded, ATTEMPTS.log):
- TU context is NOT it: a same-type vector<pair<long,CCharString> > user before/after the row (z1-z3), and `delete pair*` so the
  ??_G pair COMDAT exists in the TU (z4/z5, also x T30/45/57): all identical to the plain row.
- Callee-chain shape is NOT it: real STLport allocator::deallocate -> __malloc_alloc<0>::deallocate level (y1, x T40/50/60);
  #pragma inline_depth(0) / auto_inline(off) round the in-class ~vector (in-class = explicit inline, auto_inline ignored) and
  round the whole _construct.h destroy chain (y_cd0, x T40/50): no change.
- /Zi /Z7 /Zd: no change. /Os /O1 /Ob2+/Os: 444 bytes (wrong code); /Os /Ot = default.
- FUNCTION-WIDE `#pragma inline_depth(N)` before the definition DOES limit nested depth (unlike the scoped in-body form):
  N=5 makes EVERY exit an inline loop of `push 0; call ??_G pair` (the first time ours emits retail's ??_G(0) call) with the
  free inline; N=4/5 = 722 bytes, N>=7 = default. Uniform per function, so it cannot give retail's per-site mix alone;
  N=5/6 x T30..80 swept: no match (best 710/722).
- Which sites lose the budget in OUR compile (read at /d2inlT50, sites: 1 for-top, 2 arm15, 3 arm16, 4 expiry, 5 post-loop,
  0 normal end): 1,2,5,0 full, 3 and 4 partial (`call _Destroy(range)`, free inline). That is exactly a DFS POSTORDER of the CFG
  that visits the return path FIRST at every term check (5,0,1,2,4,3). Retail's split (0,5,4 full; 2,3 partial; 1 least; vector
  ctor called) is the same postorder with the CONTINUE path visited first at the term checks (0/5, 4, 2/3, 1, ctor).
  This order is very robust in ours: unchanged by `if(!term){...}else return;` everywhere (inv1/inv2), `while(term) return;`,
  `if(term != false)`, `if(term){return;}else{}`, and `goto done;` with the label at function end (gd1): c1/c2 normalise all.
  A goto-reordered loop (g_rev: post-loop check placed textually before the loop body) DOES move the split (in-loop sites full,
  post-loop/normal partial) -- so block order/CFG shape is the lever, not budget size.
- /EHsc: all exits share one cleanup funclet path (593 bytes) -- not retail.
Best next idea: find the CFG shape that makes c2's (postorder) inline walk reach the expiry site before the arm returns and the
for-top return last: i.e. at each IsActiveThreadTerminating() check the CONTINUE successor must be the first-visited one. Since
polarity/goto spellings are normalised, try shapes that change the successor list itself: the term test as a `switch` (case
labels are jump targets), the check folded into a compound condition with the following statement, the arms' `if (!flag)` and
term test merged (`if (!BoastActiveTimeLimit) { if (IsActiveThreadTerminating()) return; ...}` vs `if (!BoastActiveTimeLimit &&
...)`), or a for-top check that is the inner loop's own condition with the post-loop return re-test; read each at /d2inlT50 with
the site map above (q.py --flags /d2inlT50, map jne targets of the 5 term calls) before grading at default.
- (attempt 3, last) `switch (IsActiveThreadTerminating()) { case true: return; default: break; }` at all 5 checks (sw1): the branch
  sense flips to `je` (retail has `jne`, so wrong anyway) and the inline order is UNCHANGED (sites 3,4 still least at T55: whole
  ~vector call). Struck from the best-next-idea list.

## attempt 4 (wf19f author, 2026-11-13) -- RELOCATION_MATCH (strict headers), source 00d17a80.cpp, test 00d17a80_test.cpp, payload.json written
What made it match: retail's per-exit inline split is c2's BUDGET inliner, not the frequency heuristic. Source = old differ body +
`#pragma optimize("g", on)` at the top, flags `/Og- /d2vol10`, the inline vector ctor (no out-of-line declaration), the two master-data
accesses through `GetMasterData()` and the two Game calls through one empty inline accessor.
Needs: overlay engine/CQ_BountyHuntScript.h (FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS); `// Flags: /Og- /d2vol10`; no other header change.
How it was found (all reproducible; scripts in a4/): c2.dll 13.10.3077 inline.c was read (a4/c2x.py) and TRACED under gdb
(a4/trace.py + a4/trace_gdb.py / btrace_gdb.py: hardware breakpoint at 0x107724b7, then breakpoints in the decision code).
- c2 has two inliners. Decision 0x1078CF54: if the caller has global opts AND the callee has an inline-info record (sym+0x99) it
  calls the heuristic 0x1078D1AA (accept if adjusted size < inlS+5 = 14, else score = (adj*1e8/site-frequency + 6*adj)/7 < inlT = 74;
  hot sites favoured). Otherwise the BUDGET path 0x107284A3: accept iff callee IL size (sym+0x6D) <= 40 or the budget covers it.
  Budget: max(1000, 2*caller size) per function; each inline site's nested budget = remaining / remaining-sites; a callee > 40
  instrs and its nested expansions are subtracted. The share therefore GROWS site by site -- retail's "earliest site gets least".
- c1 sets the function flag (0x1000) that creates the inline-info record ONLY when /Og is on the cl COMMAND LINE. With global opts
  enabled by `#pragma optimize("g", on)` instead, every decision takes the budget path. In that mode c2 also applies the old
  inliner's body check 0x107284D5 (limit = (args+2[+1 for speed]) * -vol#, default vol 0): `/d2vol10` lets ~vector through
  (vol 7 fails, 10/14/17/20 match). How retail's build passed a volume is NOT known (cl.exe has no switch for it); the flags are an
  emulation of the proven mechanism, like the /d2inlS rows.
- Numbers for this function (budget trace): vector ctor 44, _Vector_base ctor 58, ??_G pair 41, ~_Vector_base 74, ~vector 46.
  With 18 inline sites the shares are: ctor < 58 (-> `call 0x0074F680`), for-top return < 82 (??_G and ~_Vector_base called),
  the two arm returns 82..114 (~_Vector_base called), later returns >= 115 (all expanded). Our model had 14 sites; retail needs 4
  more zero-code inline sites after the vector ctor. Probes: (Game x2 + master data x2) matches; so do other placements of the
  4th site (arm 15 / arm 16 instead of NewScriptFrame); 3 sites or 2 nested ones do not.
Names: `CLevelScriptBase<T>::GetMasterData()` is real (FableWin signatures; declared in engine/CLevelScriptBase.h; its inline body
`return MasterScriptData;` is defined in the row like CTimer::GetTime). `ScriptGameInterface(CGameScriptInterfaceBase*)` is a
MADE-UP, descriptive row-local inline pass-through: retail has one more empty inline call at each Game use, but no Game accessor
is in Ego_r/FableWin extracts, FSE manifest or the repo -- its real spelling is unknown (open question for the coordinator).
Callee identities proved by the matching object's relocations: 0x0074F680 = `_Vector_base<pair<long,CCharString>,allocator<..> >::
_Vector_base(const allocator&)` (manifest label CIndexBuffer::CIndexBuffer is a folded COMDAT), 0x004D9C5D = `~_Vector_base<..>`
(landed under a descriptive placeholder, see NAME_FIXES.tsv), 0x004DA68A = ??_G pair<long,CCharString>.
differ/00d17a80.cpp is the superseded DIFFER draft (kept, line 2 annotated).

## repair (wf19f repair agent, 2026-11-13) -- still RELOCATION_MATCH (also strict headers), vt PASS, payload.json rewritten
1. Global 0x0143E8F8 respelled `g_pGameScriptInterface` -> `pDSTGame` (?pDSTGame@@3PAVCGameScriptInterfaceBase@@A: labels_naming_wave18_2026-11-01.tsv,
   SetDSTGame 0x00CBE0C0 label, 211+ landed `extern CGameScriptInterfaceBase* pDSTGame;` declarations). Relocation name only; bytes unchanged.
   (The older attempt notes above still say g_pGameScriptInterface: history, superseded.)
2. The source header now explains `/d2vol10` (which retail bytes need it, and that it emulates an unknown build setting). Re-measured for
   the comment (ATTEMPTS.log, repair/): `/Og-` alone = DIFFER(496v785), every boast-vector destruction a `call ~vector`; /d2vol7 =
   DIFFER(675v785); /d2vol8, 9, 10, 14, 17, 20 = RELOCATION_MATCH. CORRECTION to attempt 4: the threshold is 8, not "between 7 and 10".
   Flags left at `/Og- /d2vol10` as authored (any value >= 8 is equivalent).
Verifier findings not applied: none (both applied).

