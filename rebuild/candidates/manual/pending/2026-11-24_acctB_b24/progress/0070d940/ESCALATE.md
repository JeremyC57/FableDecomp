# 0x0070D940 -- escalation notes (B24C7, 2026-11-24; replaces B24C6's compile-order hypothesis)
- B24C6's candidate (compile order / budget path for SetStartState, SetEndState, ~vector, overflow) is REFUTED by a forced-budget trace
  (inl/f1.txt): forcing those callees down c2's budget path expands the pre-loop SetStartState / SetEndState / push_back that retail calls.
  B24C6's competing one (bigger inline bodies) cannot act either: pen = 0 at every decision (caller growth <= ~4070 v the 9000 gate).
- Three of the four sites are explained and fixed by source shape (v3: transitions loop written as a while; byte evidence in NOTES.md).
- Open: the first combination pass's vector<CAnimSetIt>::_M_insert_overflow (retail calls 0x00711080 at 0x0070DF89, expands the second
  pass's). Our trace: cost 73 (c1 52 + ext 21), f 1.25 -> score 68 (< 74, expanded); second pass f 31.25 -> 49. Retail needs the first
  site at f <= 0.91 (with cost 73) or cost 80..109 at f 1.25. No loop shape tried makes the site colder (NOTES.md B24C7).
## B24C7 update (2026-11-24): devices in the current draft, and the TU-composition fact
- `/d2inlT67` (diagnostic until the row matches; EXTRA_RULES 2026-11-08 preference 2): the first combination pass's
  vector<CAnimSetIt>::_M_insert_overflow scores 68 (cost 73 = c1 52 + ext 21, f 1.25); retail calls it (0x0070DF89) and expands the
  second pass's (our score 49). Our COMDAT copy is instruction-equal to retail 0x00711080, so the callee cost is not the difference.
  No loop shape tried makes the site colder (while / guarded do-while / for(;;)+break). Max score that must stay expanded: 66; T67 and
  T68 both give retail's call sequence.
- Forced converting operator= (hdr CIVCountedPointer.h CONVERT_ASSIGN under ASSIGN_FORCEINLINE): retail expands
  `transition = transitions.back()` (op= cost 46 -> score 144 at f 0.0625) and CALLS ~vector (cost 36 -> score 113) in the same block:
  no single threshold fits both, so the expansion is forced like the existing same-type ASSIGN_FORCEINLINE.
- TU-composition fact (diagnostic c7/bis/d1.cpp = v10 + a same-TU stand-in body for CAnimSet::GetMeshAnim under auto_inline(off),
  NOT a candidate): the zero register disappears (`cmp r,ebp` 20 -> 1), the function shrinks 821 -> 794 instructions (retail 785),
  normalised-equal 451 -> 556. Probe c7/gp/p6.cpp v p4.cpp: a by-value-returned CIVCountedPointer keeps its scope-end null store when
  the callee is only declared, and drops it when the callee's body is in the TU. Retail's GetMeshAnim 0x0070C900 lies ahead of the row
  (the landed row is a separate object here), so retail's c2 knew it, and `start` / `end` were not treated as aliased. This is the
  residue the owner's retail-grouping build mode should remove (with 0x0070C900 compiled first in the same object).
## B24C8 update (2026-11-24)
- The TU-composition fact is resolved WITHOUT the build mode: the row now carries the landed 0x0070C900 body verbatim (sanctioned form
  for a landed callee; COORDINATOR_PROMPT "Landing a finished workflow"), and its emitted copy equals retail 0x0070C900. Better for
  landing: re-row 0x0070C900 onto the shared headers (same body text) and #include it here the nf251 way.
## B24C8 (2026-11-24): what remains
- Under /d2inlT67 (allowed, Inline-budget line in the draft) v16 is 2395 v 2395 with 35 raw-differing instructions, all scratch-register
  choice or store order in the combination section and the tail (NOTES.md B24C8). Only GetMeshAnim's landed body is in v16; the landed
  GetInterpolatedTime (diagnostic with a GetNonLoopingDuration stand-in), the reserve body opt-ins and the landed __stable_sort_aux text
  were each made visible in diagnostics and changed none of those instructions.
- PDB-only forms in v16 (no byte change in this row, kept for fidelity): the C3DAnimationStateAlpha ctor through SetAlphaWeight (FableWin
  sizes), the PASS23 C3DAnimationStateBase ctors (FableWin SAME), the empty CAnimSetItPriorityLessThan with its binary_function base
  (the empty class itself DOES change bytes: the comparator push).
## B24C9 (2026-11-24)
- Row bytes: RELOCATION_MATCH under /d2inlT67 (v20). The devices left are inline-budget ones (EXTRA_RULES 2026-11-08 preference 2 / 3):
  /d2inlT67 (first-pass overflow site 68 v 74; inl/v18.txt) and ASSIGN_FORCEINLINE + CONVERT_ASSIGN (converting op= 144 expanded v
  ~vector 113 called in one block). Both re-measured on v18 (NOTES).
- Held by the pick-any rule (2026-11-21): the emitted vector<CIVCountedPointer<C3DAnimationTransitionState>>::_M_insert_overflow copy
  differs from retail 0x00711A50 (leaf-bound). Not a row-text problem: the COMDAT's own inline split / registers. Needs a shared-header
  text (or a decision that the landed 0x00711A50 definition is the one linked).
- (B24C9, supersedes the T67 device above) /d2inlT67 also flips the overflow<CIVCountedPointer<TS>> COMDAT's __uninitialized_fill_n
  site (score 68: retail expands it, 0x00711A50 = the stock-threshold body). At stock + DESTROY_AUX_FASTCALL + UNINIT_COPY_FALSE_FASTCALL_PASS26
  all copies are EQ and the row differs only at the first-pass CAnimSetIt overflow site (68, retail calls). A threshold cannot serve both;
  the row's site score in retail is >= 74 for a reason not yet found. Do NOT land the T67 payload.
## B24C10 (2026-11-24): the last site is a static-frequency fact -- matches only with an Inferred `&&` operand
- Exact form (c10/inferred.cpp, packaged in c10/inferred_pkg/: payload.json + oracle.tsv + strict vland log):
  `if (NGlobalConsole::EnableCombinationAnimations && true) {` in place of `if (NGlobalConsole::EnableCombinationAnimations) {`.
  Stock flags, no /d2inl*: RELOCATION_MATCH, raw diff 0 (c9/r.sh), every emitted COMDAT copy EQ (c9/pk.py and
  pickany_vs_retail.py; only the known wrong binding 0x00C59BA0 NEQ, NAME_FIXES.tsv), strict vland RELOCATION_MATCH PASS with the two
  explained warnings (DELTA_NOTIONAL_ZERO label, TU-local _$E2). `true && EnableCombinationAnimations` matches too (c10/esc2.cpp).
- Why: retail calls vector<CAnimSetIt>::_M_insert_overflow 0x00711080 at 0x0070DF89 (first push loop) but expands it in the second.
  VC7.1 decides per site: score (c/f+6c)/7 for f <= 1, c = 73 (c1 52 + ext 21). Plain `if`: block 0.5 -> site f 1.25 -> 68 (expanded).
  With the `&&`: block 0.25 -> f 0.625 -> 79 (called); second site f 15.6 -> 63 (expanded). Admissible block window from the a1 trace
  (inl/a1.txt): (0.173, 0.365] -- exactly one `&&` level (two levels push clear() / list::size() out of line).
- Evidence it is not something else: compile order (inl/f150.txt: FORCE_AFTER=150, the row's pipe index -- the budget path rejects
  BOTH sites, retail expands the second); /d2inlT67 (calls the 0x00711A50 copy's inner fill_n, retail expands it); loop shapes
  (B24C7); an empty() guard (+4 instructions, B24C9); an early `return state;` (72 hunks); nested `if (true)` blocks change the
  local static's mangled scope number from 23 (?BH@, = Ego_r's `CalcInterpolatedMeshAnim'::`23'` atexit name) to 25 / 26, so the
  extra condition is an operand of the existing `if`, not a statement.
- Unknown: the operand's real wording. The flag is a console bool (Ego_d.pdb ?EnableCombinationAnimations@NGlobalConsole@@3_NA,
  in the NGlobalConsole cluster 0x013756F4.. of the retail .data); a release-constant build switch ANDed with it is the likeliest
  original. No PDB / FableWin / Ego_r text in reach names it (FableWin and Ego_r binaries are not on this machine; FableWin's object
  order lists no candidate predicate). Owner/coordinator decision needed: land as `// Inferred:` (EXTRA_RULES 2026-11-12 / 11-19:
  "a dead test ... remains Inferred with the evidence") or keep the row DIFFER.
- (B24C10, after review) Graded: nested `if (true)` blocks (c10/n1g, n2g) ALSO match (RELOCATION_MATCH) -- they differ only in the
  local static's mangled scope number (?BJ@ / ?BK@ v ?BH@ = 23; Ego_r's atexit name carries `23'; Ego_r is another build, 2686 bytes,
  so this is a hint for the operand form, not proof). Two `&&` levels (aa2) DIFFER(2379v2395), as the window predicts.
  The owner's 2026-11-08 inline controls do not reach it: `#pragma inline_depth(1)` / `(2)` around the first push loop changes nothing
  (c10/d1, d2: DIFFER(2507v2395) 21 hunks); no /d2inlT threshold works (the row's site and the 0x00711A50 COMDAT's site both score 68
  with opposite retail decisions). Package: c10/inferred_pkg/payload_INFERRED_NEEDS_OWNER.json (+ oracle.tsv, vland.log).
- Name doubt: NGlobalConsole::EnableCombinationAnimations for 0x013756F7 rests on ONE extract (Ego_d publics, no address) plus the
  address neighbours 0x013756F4 / F5 (bound NGlobalConsole::DrawProximityFadeOutMeshes / CombatParticleEffects); 0x013756F6 is bound
  g_bRegionLoadingEnabled and 0x013756F0 g_LightingChannelDrawFilter, so the .data run is not purely NGlobalConsole. Treat as unproven.
