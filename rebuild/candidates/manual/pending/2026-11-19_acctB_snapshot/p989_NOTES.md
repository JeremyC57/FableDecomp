# p989 NOTES -- TU-context (prefix) probe on 0x00989230 class and 0x008185B0 (2026-11-19)
Env: export S=...scratchpad; export W=$S/w25/p989 (two statements: `export S=.. W=$S/..` on one line leaves W empty).
hdr3c overlay: none of its files (CCoda*, C3DAnimation2/C3DMesh2, CAIStateGroup_OpinionReactionRoot, CQ_HerosOldHouseScript, SalMath)
is included by the CShaderRenderManager / CMatrix4x4 TU; all CMatrix4x4 opt-ins used by t1 are in the repo headers today
(CMatrix4x4.h, CMatrix4x4AlignedSSE.h). So 0x00989230 probes run with FABLE_HDR_OVERLAY empty (no rebase needed).
Retail object order (coverage.tsv, lib_shader_render_manager.obj): ... SetD3DMatrix 00988990, UpdatePixelShader 00988A20,
UpdateCombinedProjectionTransform 00988A50, UpdateWorldTransform 00988E30, UpdateLocalisedSphereMapping 00988E80 (TWIN, not landed),
UpdateObjectSpaceTextureTransform 00989230, UpdateWorldSpaceTextureTransform 009894B0 ... All predecessors except 00988E80 are landed
(rebuild/src/compiled/00/98/CShaderRenderManager_*).

## A. 0x00989230 -- results (probes only, 0 grades)
- Signature check (Ego_d types.txt, present at /tmp/claude-0/egod_pdb/types.txt): CMatrix4x4 = 0x9E8B (field list 0x9E8A, fwd 0x9E4E).
  operator() method list 0x9E62: 0x9E60 `float& (int,int)` (this 0x9E4F) and 0x9E61 `const float (int,int) const` (return 0x2447 =
  LF_MODIFIER const float, by value; this 0x9E5D = const CMatrix4x4*). AlignedSetMultiply and AlignedSetMultiplyTransposed both
  0x9E69 = void (0x9E68: const CMatrix4x4& (0x9E63 -> 0x9E5C const CMatrix4x4), const CMatrix4x4&), non-const this.
  Header (CMatrix4x4.h lines 28-29, 66, 76) MATCHES: no constness/return lever. Ego_d locals: AlignedSetMultiply(lhs, rhs) {src1,dest,src2}
  {CMatrix4x4& m {long c1 {long c2}}}; Transposed(lhs, rhs) only; SetVSConstants(index, data, count) + local hr, debug length 536 (=FableWin
  .obj code_size 536: the original carries assert/debug code).
- Prefix axis: function bodies above (real immediate predecessor twin 0x00988E80, control TU, twin pair AE1320/AE29D0 either order),
  real includes above/below, CMatrix4x4.h first, SetVSConstants placement/strength/text (Ego_d names, hr, dead-arm asserts): columns 1..3
  NEVER move. Functions do not carry state into each other in c2.
- What DOES move every column: the function's own CFG. Removing the inlined count test (a06 call, a11 no test) gives exactly retail's
  order 3b1b2b0b|3b1b2b0b|0b3b1b2b|3b1b2b0b -- but retail HAS the test (0x00989443 test ecx,ecx / je). Any extra branch anywhere in the
  function (a24 after the loop, a25 before it) reorders all columns. So the order is keyed to whole-function block structure, not TU context.
- 635-byte loose end: the Ego_d-shaped body (hdrA overlay, opt-in ..._EGOD_LOCALS) gives DIFFER(721v721) on landed 0x00BD2D90 and
  DIFFER(2473v2472) on landed 0x00A9E1E0 (both RELOCATION_MATCH with the repo ACCESSORS body in the same overlay). It reproduces neither:
  the ACCESSORS body is settled; the 635 form is not retail's inline text.
- VERDICT A: TU-context axis DEAD for the 0x00989230 class (row + twins 0x00988E80, 0x00AE1320, 0x00AE29D0). 0 grades spent.

## B. 0x008185B0 -- prefix axis (probes only, 0 grades)
Readout tools/pdiff.py (no ATTEMPTS, W/tmp/pdiff) + cls.py. Baseline S S S S S ndiff 10 with and without hdr4d (TU includes only
stlport/fable_containers.h). Caller: 0x00816410 CPhysicalPrimitiveCylinder::GetClosestPointToPos2D (labels_landing_2026-11-07w).
Ego_d field list 0x449D1: public ctor, IsValid, GetValue; private Ca, Cb, Cc. IsValid 13 bytes debug (return true).
b01..b10 (template order, implicit instantiation, caller above, member order, IsValid body, unrelated prefix, trig/include order):
all S S S S S with the SAME ndiff 10 -- not one instruction moved. Control sibling 0x0087AA10 stays RELOCATION_MATCH under the reorder.
VERDICT B prefix axis: DEAD.

## COORDINATOR CORRECTION (2026-11-19, mid-task): NO PARKING
Stop rule and 100 cap void. Dead axis -> record and move to the next idea level. Read docs/pipeline/MATCHING_PLAYBOOK.md; c2 tools in
rebuild/candidates/manual/pending/2026-11-09_linux_hand892/tools (c2dbg.py, c2prio_op.py, c2dis.py) and W/tools/sched. Escalate via
tmp/ESCALATE_<addr>.md + SendMessage to "main" ("ESCALATE <addr>: ...") and keep working. Final message only when both rows match or
nothing left but a pending escalation.
- 0x9E5C dumped: LF_MODIFIER const of 0x9E4E (const CMatrix4x4) -- confirmed.
- Bow functor field list 0x4C74E: ctor, GetValue, IsValid (GetValue BEFORE IsValid) -> FableWin emission after the template
  (Bow: GetValue, IsValid; ours: IsValid, GetValue) follows declaration order of members first referenced IN the template:
  the template body references func.IsValid() in the debug build (Tan/Cot: IsValid emitted next to the ctor = already used by the caller).
  Release form candidate: fable_assert.h dead arm ASSERT(func.IsValid()). -> B hypothesis 2 probes c*.
- Escalations written (tmp/ESCALATE_00989230.md, tmp/ESCALATE_008185b0.md) and sent to main. Key A fact: period-4 live-code count
  sets columns 1..3 (top4 branches = exact retail); FableWin /Od sizes show missing statements (row 262 v 231, ASM 567 v 537).

## A round 2 (advisor): inlinee-level lever
- Twin phase sweeps (probes/tw, probes/inl): with the ACCESSORS body, 0x00AE1320 needs +2 live units (exact retail at +2),
  0x00AE29D0 needs +2 (cols 1..3 exact; col 0 separate), the row needs +2 (exact). Same +2 placed INSIDE the inlinee x87 arm fixes
  all three at once (row.2 / 1320.2 exact) but moves landed 0x00BD2D90 (fitted) and 0x00A9E1E0 off retail.
- Ego_d AlignedSetMultiply frame (src1 -12, dest -8, src2 -4, m -16, c1 -20, c2 -24, this -28) is reproduced EXACTLY by VC7.1 /Od of:
  SSE `const CMatrix4x4* src1 = &lhs; CMatrix4x4* dest = this; const CMatrix4x4* src2 = &rhs;` + else `CMatrix4x4& m = *this; for (long c1..)
  for (long c2..) m(c1,c2) = lhs(c1,0)*rhs(0,c2)+...` (variant t0_sE_aM). The ACCESSORS body (no m, int r/c) does not fit that frame.
  Under t0_sE_aM: row +1, 1320 +1, 29d0 +1, a9e1 +2 (relative to ACCESSORS). -> rows still need +1; a9e1 (landed) would need re-authoring.
- Ego_d is a VS2005+ build (f:\dd\vctools CRT modules); FableWin sizes equal Ego_d's, so /Od size gaps are not VC7.1 evidence. Dropped.
- DIAGNOSTIC HIT (m_plus_r): inlinee x87 arm `CMatrix4x4& m = *this; const CMatrix4x4& r = b; for(long c1)for(long c2) m(c1,c2)=a(c1,k)*r(k,c2)...`
  gives RELOCATION_MATCH for 0x00989230 AND 0x00AE1320 (prior draft), 0x00AE29D0 ndiff 6 (col 0 only), 0x00A9E1E0 col 0 off, 0x00BD2D90 off (fitted).
  `r` is NOT an Ego_d local -> search natural spellings with the same effect.
- FableWin.pdb names contain `$$Q` rvalue refs (std::forward): FableWin is a VC10+ build; its sizes/emission order are not VC7.1 evidence.
- B round 2: live rulers before site 4 / at top never flip the factoring (not a count lever). Forced C trees at site 4 (20 forms) always
  evaluate sin first; retail's site 4 evaluates cos first (spills cos, keeps sin twice) with a C tree -> the factoring in retail happened
  on a tuple stream that already had cos before sin; no source tree reproduces that. Next level needs a c2 trace of the factoring pass.
- 19:46 official grade: probes/multi/m_plus_r_row.cpp with HO=hdrB -> RELOCATION_MATCH (diagnostic; rhs copy unevidenced). Escalation
  update sent to main with decision (a) nested Inferred opt-in for the three rows / (b) keep searching.
- STATE AT HANDBACK: no row landed; no batch snapshot (nothing landable without the coordinator decision). Grades: 00989230 1, 008185B0 0.
