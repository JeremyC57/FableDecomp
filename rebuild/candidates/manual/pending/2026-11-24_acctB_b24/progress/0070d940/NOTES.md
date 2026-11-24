# 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim -- B24C4 (2026-11-24): baseline only
- Baseline (draft unchanged, v0.cpp = differ/0070d940.cpp, stock flags): DIFFER(2572v2395) hunks=125 -- reproduces hand785's grade.
  First hunks: frame 0x44 v retail 0x3C; retail keeps `this` in ebp (`mov ebp,ecx`) and reads `mov al,[ecx+20h]` (a byte), ours
  stores this to the stack and reads a dword then `shr eax,2` (a bit-field / flag read spelled differently).
- Header debt: the draft is a local-model source (only stlport/fable_containers.h is included); per the brief it must be ported onto
  engine/CAnimateThing.h, C3DAnimationBlendState, CIVCountedPointer etc. before residue work. hand785's residue reading (draft line 2-3):
  VC7.1 inline budget (GetProgressFromAlpha 0x0070BE40 and vector<CIVCountedPointer<C3DAnimationTransitionState> >::push_back
  0x00711C50 called before the transition loop, expanded inside it); next idea there: the pre-loop first-transition setup as its own
  inline helper, and the CIVCountedPointer(T*) adopting constructor.
- Not started beyond the baseline (context spent on 0x00402510 and the twin-row evidence).
- (B24C5) not reached: B24C5 spent its context on rows 1 and 2 (see ../00be4660/NOTES.md, ../00be3970/NOTES.md). Port onto shared headers first.
## B24C6 (2026-11-24) -- ported onto the shared headers
- Lessons applied: GOTCHAS "porting a local model onto shared headers can fix frame packing" (did not here); inline-budget residue
  class (INLINE_DEPTH_REVISIT / hand785): call-set differences are the inliner running out of budget at different sites.
- Baseline regrade v0 (local model): DIFFER(2572v2395) hunks=125, 129 dd blocks (c6/g.sh, c6/retail.dis = dis_all trimmed to 2395 bytes).
- Port c6/v1.cpp -> c6/v2.cpp = differ/0070d940.cpp: engine/CAnimateThing.h (CALCCACHEDANIMS: ParentThing is CThing&,
  CalcInterpolatedMeshAnim declared), CAnimSet.h (IsValid() = the main-set test; PeekAnimInfo body = landed 0x0070CE10's),
  C3DAnimationTransitionState.h, C3DAnimationStateAlpha.h, C3DAnimationBlendState.h (TYPED_DATA + SHAREDDATA + CTOR_BODY_PASS26 +
  GENERATED_SPECIALS_PASS23), CIVCountedPointer.h (FROM_RAW adopting ctor: `CIVCountedPointer<C3DAnimationTransitionState>
  first(new C3DAnimationTransitionState)` replaces the draft's address-cast stand-in; push_back(first) passes the never-addref'd
  slot exactly as retail), CThing.h HasTC(TCI_PHYSICS) (bit 2 of +0x20), CWorld / CMainGameComponent / CMeshDataBank /
  C3DAnimationInfo as landed 0x0070CE10. In-loop SetStartState(transitions.back()) through a converting temporary = retail's
  0x0070DBC8..0x0070DC09 (copy, assign, release). New overlay opt-ins: hdr/README.md. Still local: CAnimSetItPriorityLessThan and the
  std::stable_sort -> __stable_sort_aux forwarder (no header has them), GFWorld3DDistanceSmallerThan declared out of line (GFWorldDistance.h
  only has the inline body under FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER; retail calls 0x006614F0).
- v1 DIFFER(2673v2395) 127 hunks; v2 (+ FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE: the one out-of-line operator= call gone,
  retail expands every assignment) DIFFER(2689v2395) 125 hunks / 128 dd blocks.
- Call set v retail (v2): EQUAL except (a) ours expands ~vector<CIVCountedPointer<C3DAnimationTransitionState>> (retail calls
  0x00711760), (b) ours expands both CAnimSetIt push_back overflows (retail calls 0x00711080 once), (c) ours calls SetStartState /
  SetEndState once each (retail twice: before the loop AND for the main transition). GetProgressFromAlpha and the transitions
  push_back are called once each like retail. So retail's inliner runs out of budget earlier: its inline bodies are bigger.
- FableWin /Od sizes (c6/_fw, FW_EXTRA=/EHsc) that differ: C3DAnimationStateBase ctor 12i v ours 14i, its dtor 11i v 9i,
  CIVCountedPointer default ctor 12i v 10i, copy / converting ctors 22i v 18i, C3DAnimationBlendState() 24i v 25i, GetWorld /
  GetMeshBank 10i v 9i (FableWin is a later Dinkumware build: compare with care). SAME: the TransitionState ctor/dtor, SetStart/End/
  Progress, IsValid, HasTC, C3DAnimationState(K,M), the blend-state copy ctor. Next: (1) texts of the differing CIVCountedPointer /
  C3DAnimationStateBase members (a bigger IL body for each of the ~20 counted-pointer sites is the most likely budget eater);
  (2) GetProgressFromAlpha FableWin 89i/299b (plain body ~30i: asserts -> release dead arms, Inferred, ESCALATE only).
## B24C7 (2026-11-24)
- Lessons applied: MATCHING_PLAYBOOK "Inline decisions" (score = cost*(1/f+6)/7 < 74 for f<=1; `for` body 5x and code after keeps 5x, `while` body 9x and
  code after drops back); GOTCHAS 374/375 (two inliners, c2 trace); b24a/00e93eb0 ESCALATE (budget path = callee compiled after caller).
- c2 trace (inl/run.sh, inl.py + B24C7 hook at 0x1078D2FB logging ext/fb/k2) of B24C6 v2: pen = 0 at every decision, caller growth g peaks
  ~4070 (pen gate 9000) -> body-size / caller-growth explanations cannot move any score. Fuller score rule read from c2 0x1078D2EE..0x1078D354:
  cost = c1 + ext, ext = fb*sz/255 (fb = callee byte [rec+0x4E], e.g. 24 for _M_insert_overflow, 93 for list::size); sc = (cost/f + 6 cost)/7
  for f <= 1, (cost/f + 2 cost)/3 for f > 1 (k2 = [0x10881998] = 3); expanded iff sc < 74. Nested site f = local f * max(1, outer f).
- Forced-budget run (inl/runf.sh = B24A5 inlf.py, FORCE regex; B24C7 added FORCE_AFTER=<pipe count> so only the caller's sites are forced):
  forcing SetStartState/SetEndState/push_back/~vector/overflow down the budget path EXPANDS the pre-loop SetStartState / SetEndState /
  push_back that retail CALLS -> compile order does NOT explain this row. ESCALATE.md hypothesis refuted.
- v2 trace: main-transition SetStartState sc 58 (f 0.375 = 0.3125 loop leak + 0.0625), ~vector sc 47 (f 0.3125): all from the transitions
  `for` loop leaking 5x past its end. v3 = transitions loop as `++it; while (it != end) { ...; ++it; }`: post-loop f drops back -> main
  SetStartState sc 94 / SetEndState 100 / ~vector 113 all CALLED like retail, in-loop expansions kept. Byte evidence for the while form:
  retail advances `it` (0x0070DCCF..0x0070DCD9) BEFORE next's release dec (0x0070DCDD); v3 has that order (0x394..0x3A2), v2 the opposite.
  v3 DIFFER(2593v2395) 153 hunks (v2 2689 / 125): more hunks but 468 equal instructions v 379 (c7/dd.py DDSUM=1, absolute addresses
  normalised) and the call sequence = retail except ONE site: the first combination pass's CAnimSetIt _M_insert_overflow (sc 68, cost 73
  at f 1.25; needs >= 74 -> f <= 0.91 or cost 80..109). Loop shapes tried for the first push loop (traces only): while (v4: body 9x,
  sc 59), guarded do-while (t_c: 2.5, sc 68), for(;;)+break (t_e: 2.5, sc 68): none colder. Second pass sc 49 (expanded = retail).
- Tools: c7/callseq.py <obj> <dis> (direct-call names), c7/dd.py (normalised diff, DDSUM=1 summary), c7/c2d.py <hexaddr> <hexlen> (c2.dll listing).
- (B24C7 cont.) Grade log: v3 + GFGetInterpolatedValue alphas (hdr CAnimSet.h: retail's `fld last; fld cur; fsub st,st(1); fmul; fadd
  st,st(1); fstp st(0)` x87 form = a by-value lerp; FableWin GetInterpolatedAlpha 19i/45b = VC7.1 /Od 16i/36b of
  `return GFGetInterpolatedValue(LastAlpha,CurrentAlpha,alpha);` + FableWin's 3-instruction float pushes x3) stock 2628v2395 109 hunks.
  /d2inlT67 (diagnostic): call SEQUENCE = retail exactly (42 calls). v6 = + CIVCOUNTEDPOINTER_DTOR_DIRECT (scope-end releases without
  null store, as retail 0x0070DD19 / 0x0070DD29) 2458v2395, structural 701/785. v7 = v6 + new overlay opt-in
  FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT_ASSIGN (FableWin ??$?4VC3DAnimationTransitionState@@@?$CIVCountedPointer@VC3DAnimationStateBase
  @@@@QAEAAV0@ABV?$CIVCountedPointer@VC3DAnimationTransitionState@@@@@Z 0x01EA5040, 35i/100b; ours 33i/93b; forced under
  ASSIGN_FORCEINLINE like the same-type op=: retail expands it at a site scoring 144) -> `transition = transitions.back()` = retail's
  single `inc [eax+4]` + store: 2442v2395 /d2inlT67, structural 701 (c7/sdd.py: registers / stack offsets normalised).
  v9 (FableWin-sized texts: dtor = Release() 9i SAME, RELEASE_ISNULL 17i SAME, CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT 24i SAME,
  ASSIGN_EARLY_RETURN) 2482v2395 127 hunks, structural 669: worse than v7 structurally, fewer hunks.
- Main residue now (v7): ours materialises 0 in ebp through the transition loop (xor ebp,ebp at 0x1CA) and keeps `end` in memory
  ([esp+0x14]); retail keeps `end` in ebx in the loop (0x0070DB34 `mov ebx,[esp+20h]` -- no AddRef -- after passing &end to the
  pre-loop SetEndState call) and reuses [esp+0x20] for the loop's GetMeshAnim temporary. So every null test is `cmp r,ebp` and the
  scope-end releases load the count (`mov esi,[eax+4] ... dec esi ... cmp edx,ebp`) where retail does `test r,r` / `dec [r+4]`.
  Open question: what source lets retail enregister `end` after its address went to SetEndState.
- (B24C7) Base re-banked on v10 (advisor: DTOR_DIRECT contradicted -- retail keeps a null store after the loop temporary's release
  0x0070DBC0 and the blend state's 0x0070E27E, and FableWin's dtor is 9i = `Release();`): v10 = FableWin-sized CIVCountedPointer texts
  (dtor Release() 9i SAME, RELEASE_ISNULL 17i SAME, CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT 24i SAME, ASSIGN_EARLY_RETURN) + CONVERT_ASSIGN
  + GFGetInterpolatedValue alphas + the three local items moved into overlay headers (stlport/fable_algorithm.h STABLE_SORT_AUX_DECL,
  GFWorldDistance.h SMALLER_DECL, new engine/CAnimSetItPriorityLessThan.h; code identical to v9). DIFFER(2482v2395) 127 hunks
  /d2inlT67, stock 2610v2395 122 hunks. differ/ = v10.
- Our COMDAT copy of vector<CAnimSetIt>::_M_insert_overflow is instruction-equal to retail 0x00711080 (c7/ovf_*.dis): the first-pass
  call is purely the site score (68 v 74) -> /d2inlT67 (EXTRA_RULES 2026-11-08 preference 2) is the only device found; T68 also works.
- Pick-any: only NEQ is ??0C3DAnimationBlendState@@QAE@XZ v 0x00C59BA0 -- a wrong binding (NAME_FIXES.tsv: the class ctor is
  0x004C2DD0, byte-equal to our copy). AddCombinationBlend const&: second source = FableWin.pdb (NAME_FIXES.tsv).
- Null-store probes (c7/gp/p3.cpp, p4.cpp; gp/opt.py = sub-second compile, byte-identical to fg's object): VC7.1 drops a scope-end
  Release null store only when no later call follows (pA/pC/pF/pG/pI drop it; pB/pD/pE/pH -- a later call or a second destructor --
  keep it). Retail drops start's store although first's release (a virtual Delete call) follows -> in retail `start`/`end` are not
  memory-aliased at that point (end is in ebx). Row bisection (c7/bis/b1..b5, c1, c3): removing the pre-loop Set*State calls, the
  loop, the combination section, the main transition: our stores stay. Unresolved: the source that makes start/end non-aliased.
- Fast loop: c7/q.sh <src> [xflags] (compile + normalised/structural sums), c7/gt.sh (fg grade + listing + call sequence).
- (B24C7, last finding -- the lead for the next agent) The whole remaining residue is ONE fact: in retail `start` / `end` are not
  memory-aliased (end lives in ebx through the loop, start's scope-end null store is dropped, no zero register). Probe c7/gp/p5.cpp:
  VC7.1 keeps a CIVCountedPointer initialised from a by-value return (`P end = Get();`) in memory for its whole life even with no
  escaping call (the hidden return-slot address aliases it). Diagnostic c7/bis/e2.cpp (= c7/e2_lead.cpp; NOT a candidate):
  `CIVCountedPointer<..> start; start = it->GetMeshAnim(alpha);` (same for end) -> structural 713/785 and normalised-equal 584 (v10:
  669 / 451): every block after the pre-loop now matches structurally (end in ebx, test r,r, `dec [r+4]`, no null stores) EXCEPT two
  inserted 20-instruction op= collapses (temp -> var AddRef + temp release) right after each pre-loop GetMeshAnim, which retail does not
  have (retail passes the return slot [esp+20h] itself to SetEndState and then moves it into ebx with no AddRef, 0x0070DB34).
  Next idea: a source form that adopts the returned temporary into an enregisterable variable with no AddRef / Release pair (e.g. an
  ownership-taking assignment / swap from a by-value parameter, if a PDB-evidenced member exists: check FableWin / Ego_r for
  CIVCountedPointer members beyond those in the header -- Swap, Adopt, Reset(CIVCountedPointer), operator=(by value)), then
  re-check the pre-loop with c7/q.sh. Stop line reached here (context).
  PDB check done: CIVCountedPointer<C3DAnimationStateBase> members in Ego_r / FableWin are only ctor(), copy ctor, dtor, Get,
  InternalGetObject, Release, Reset(T*), CastToBase, op= / converting op= -- no Swap / Adopt; Reset(T*) (RESET_ADOPT opt-in exists)
  takes a raw pointer, so a `start.Reset(...)` spelling would need a raw-pointer source. Rows 2 (0x00BE3970) and 3 (0x00BE4660) were
  not reached by B24C7.
- (B24C7, superseding the Swap/Adopt idea) FIRST STEP for the next agent: the aliasing of start/end is a TU-composition fact. With a
  same-TU body for CAnimSet::GetMeshAnim (diagnostic c7/bis/d1.cpp; stand-ins are banned) the zero register goes and the row shrinks
  to 794 instructions (retail 785); see ESCALATE.md. Write it up for the retail-grouping build mode, then work what is left in
  c7/q_d1.dis v retail (scheduling around the pre-loop alphas, c7/q_d1.sdd).
