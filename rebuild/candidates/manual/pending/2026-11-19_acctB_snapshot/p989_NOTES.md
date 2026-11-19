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

## Convergence test (coordinator decision 3; no grades)
- Q1 (columns under new body m+r): a9e1 col 0 off, bd2d off. Q2: Ego_d-local re-ports: a9e1 FOLLOWS (probes/a9/b0_block_s1_mr = RELOCATION_MATCH:
  no `bone` local, shared loop counter, local inside the else-block); bd2d follows in NO variant (~60 probes, probes/bd2 bd3 bd4).
  Q3: no out-of-line AlignedSetMultiply in retail (no coverage/egor name) -> nothing to own.
- Outcome: option (a) FITTED. Batch W/batches/b1: hdr/engine/CMatrix4x4AlignedSSE.h (nested opt-in ..._M_RHSREF inside ..._ACCESSORS),
  src/00989230.cpp + src/00ae1320.cpp RELOCATION_MATCH with that hdr; landed a9e1/bd2d unchanged (default path) still match.
- test/00989230.cpp + mutants: base PASS, 9/9 mutants FAIL (product counter added so no_skip_empty fails).
- 1320 Ego_d locals: sim, anim, anim_manager, bone_morphs, mesh, no_bones, mesh_vertices, no_vertices, bone_transforms, {c_bone}; NO copy
  local -> the copy lives in an inlinee. Ego_d procs: AlignedPreMultiplyTransposed is a proc (AlignedPostMultiply / AlignedPreMultiply are not);
  retail has it out of line at 0x00ADEFC0 (particle area). probes/c13: Ego_d-shaped 1320 calling bone_transforms[c_bone].AlignedPreMultiplyTransposed(
  mesh.BoneSpaceTransforms[c_bone]) with ..._FORCEINLINE: 681v681 (size exact) both bodies; column orders not yet retail.
- KEY CALIBRATOR 0x00ADEFC0 CMatrix4x4::AlignedPreMultiplyTransposed (out of line, 471 b; open row codex pass24 DIFFER 472v471). Oracle from
  Fable.exe in W/oracle/extra_oracle.tsv (tools/rbytes.py). Its size gap is ONLY x87 term encodings -> pure phase question.
  Unit sweep (probes/ef/u_*): clean period 4 on all 4 columns; mr = acc+2; retail = acc+1.
  probes/ef/egod_fi1.cpp (hdrA, ..._EGOD_LOCALS = Ego_d body `CMatrix4x4& m=*this; long c1,c2; m(c1,c2)=a(c1,k)*b(k,c2)`, no rhs copy,
  out-of-line body `__declspec(align(16)) CMatrix4x4 m; m = *this; AlignedSetMultiplyTransposed(matrix, m);` + SETMULTIPLYTRANSPOSED_FORCEINLINE)
  = RELOCATION_MATCH. -> the Ego_d body is the evidenced shared body; rows must find their remaining units in their own (Ego_d) text.
- 0x00AE1320 Ego_d-shaped (probes/c13e/egod.cpp: Ego_d names sim/anim/anim_manager/bone_morphs/no_vertices/mesh_vertices/no_bones/
  bone_transforms/c_bone, `bone_transforms[c_bone].AlignedPreMultiplyTransposed(mesh.BoneSpaceTransforms[c_bone])`, PMT + SMT FORCEINLINE)
  under hdrA EGOD_LOCALS = RELOCATION_MATCH. No fitted construct.
- SSE arm locals (hdrC, FABLE_P989_SSEF): Ego_d names/types `const float* src1 = &a.Data[0][0]; const float* src2 = &b.Data[0][0]; float* dest = &Data[0][0];`
  (decl order src1, src2, dest = retail store order [1c],[18],[14]) keeps ef + 1320 RELOCATION_MATCH (k=5/6); order src1,dest,src2 breaks the SSE stores.
  SSE arm spelling never moves the x87 phase. Row 0x00989230 under the Ego_d body: 635v636, ONLY x87 differs, needs +1 unit.
- LANDED 0x00BD2D90 under the Ego_d body (hdrC, EGOD + SSEF 5): probes/bde/r1m0a0b1_acc.cpp = RELOCATION_MATCH. Its locals are EXACTLY Ego_d's
  (render_info = GetRenderInfo() [needs ..._ANIMATEDMESH_RENDERINFO], mesh, no_bones, interpolate, bone_morphs, c_bone, bone_transform; no
  mesh_pointer / animation_manager). Landed text (fitted named locals) does NOT follow (DIFFER 721v721) -> re-port = Ego_d text.
- Row 0x00989230 under Ego_d body: needs exactly +1 (probes/ru/u1 = retail columns). Byte-neutral tries that do NOT move it (probes/rv, rv2):
  hr local in SetVSConstants (3 forms), count tests (!=0, >0), no cast, rows via if / >, flags swapped, direct AlignedSetMultiply, named
  texture ref. Flags as one statement = +3 (636 bytes, wrong phase).
- 0x00AE29D0 under Ego_d body: probes/d29e/ego.cpp (1320-style Ego_d names; `bone_transforms[c_bone].AlignedPreMultiplyTransposed(mesh.BoneSpaceTransforms[c_bone])`,
  no copy local) = RELOCATION_MATCH; prior text with only the loop swapped to PMT also matches (prior_pmt). Ego_d 7-arg record incomplete.
- Landed 0x00A9E1E0 under Ego_d body: Ego_d-faithful text (probes/a9e/b0_func_s0_acc: no_bones + local_matrix at function scope, c_bone per
  loop, parent_index, no `bone`) = cols 1..3 retail, col 0 "2a 0a 3b 1b" v retail "2a 3b 1b 0a" (b1 = with `bone`: "2a 3b 1b 0b").
- LANDED 0x00A9E1E0 under Ego_d body: probes/a9f/t0n1.cpp = RELOCATION_MATCH (Ego_d locals b0_func_s0 + the non-transposed branch written
  `dest[c_bone].AlignedSetMultiplyTransposed(dest[parent_index], local_matrix)` = same product as AlignedSetMultiply(local_matrix, parent)).
  t1n1 also matches; t0n0 / t1n0 (plain AlignedSetMultiply in the normal branch) leave col 0 off. Col 0 = which forwarder the site expands
  through (row `direct` variant also moved only col 0). The forwarder choice is evidence-free -> mark Fitted (col 0) in that row.
- STATUS under the single Ego_d body (no rhs copy): ADEFC0 ok, 1320 ok, 29d0 ok, bd2d ok (Ego_d re-port), a9e1 ok (re-port + fitted call),
  row 0x00989230 +1 missing, 0x00988E80 structure not looked at yet.

## Batch b2 (coordinator: claims posted for a9e1, bd2d, adefc0; b1 / option (a) dropped)
- hdr: W/batches/b2/hdr/engine/{CMatrix4x4AlignedSSE.h, CMatrix4x4.h} = TODAY's repo copies + nested opt-in
  FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD (inside ..._ACCESSORS, #error pins for ACCESSORS / ELEMENT_ACCESS); SSE locals
  `&lhs.E11` style; PMT inline text renamed to Ego_d (`matrix`, `m`).
- src (all RELOCATION_MATCH under b2 hdr, tools/regress.sh): 00adefc0 (new), 00a9e1e0 + 00bd2d90 (re-ports, landed names), 00ae1320 + 00ae29d0
  (retyped onto engine C3DMesh2 / C3DAnimationManager / CParticleContext headers; 29d0 context slots = GetMaximumIterations / GetMaximumDistanceError).
- Landed regress under b2 hdr (default path): a5cb90 (/Zi), a9d750, a9d580, a9d480, aa2110, aa0fa0, a9e1e0, bd2d90 all RELOCATION_MATCH.
- 0x013D2880: Ego_d.pdb datanames has ?GEnableSIMD@@3_NA; g_SSEAvailable is in no PDB -> NAME_FIXES; b2 rows use ..._SIMD_ORIGINAL_FLAG.
- GOTCHA: VC7.1 cl.exe under Wine HANGS (99% CPU, no exit) on any fatal `#error` (bare `#error` TU with /Zs: timeout). The b2 #error pins
  cannot be exercised in the lane; checked by inspection only. Always wrap pin checks in `timeout` and kill only own cl.exe.
- Host has no 32-bit libc/libstdc++ headers: gxx -m32 portable steps cannot run here. VC7.1 /DFABLE_PORTABLE run of the ADEFC0 test: PASS.
- Tests (btest, b2 hdr): a9e1 re-port base PASS, 6/6 mutants FAIL; bd2d re-port base PASS, 7/7 FAIL; adefc0 base PASS, 3/3 FAIL.
- b2 COMPLETE: README, NAME_FIXES, payload.json/oracle.tsv (tools/mkpayload_b2.py), tests 1320 7/7, 29d0 7/7; official grades 20:35-20:36
  all five RELOCATION_MATCH (adefc0 via tools/qw.py = q.py + W/oracle). b1 marked SUPERSEDED.
- 0x00988E80 structure SOLVED to 915v917 (was 889): Ego_d text (wvmatrix = View*World via AlignedSetMultiply; uvect(row0).Normalise();
  spheremap_matrix[0] = uvect*0.5, W 0.5 BEFORE vvect; vvect(-row1).Normalise(); spheremap_matrix[1]; `long hr = D3DDevice->SetVertexShaderConstantF(
  layout->SphereMapMatrix.Offset, &spheremap_matrix[0][0], 2)`; UpdateFlags &= ~0x8000). Best tail: C3DVECTOR_NORMALISE_PAREN_SQUARES_{XYZ,ZYX}
  + GFFASTONEOVERSQRT_NEWTON_PAREN (probes/e8/w_*: 7 tail diffs = first normalise keeps s live and multiplies Y*s / Z*s as `fld [Y]; fmul st(2)`).
  Multiply columns: retail 3b1b0b2b|3b1b2a0a|0b3b1b2a|2b3a1a0a; no +k (0..3) of our text and no Transposed-forwarder +k gives it (probes/e8/u*, t*).
- e80 readout for the coordinator: under ACCESSORS (acc0..3) and Ego_d body (+0..3) and 14 tail variants, the multiply columns stay on ONE
  4-cycle (acc = ego-1); retail e80 columns (3102|3120|0312|2310 positions) are on none of its phases (mixed family, like bd2d's, which its own
  Ego_d text produced). So no unit shift can be read yet. e80 never expands SetVSConstants (Ego_d `hr` local = direct device call), so a
  SetVSConstants inlinee cannot be a shared cause with the row. Early W stores (probes/e8/A_*, B_*): tail worse (14).
- 0x008185B0 advisor checks done (ESCALATE_008185b0.md round 3): (1) cos is already memory-homed at site 4 in our build (DAG trace) -> not
  the lever; factor choice is tree-level. (2) siblings show the same site-4 shape from the same template; residue = GetValue text; Ego_d
  GetValue record has float locals sin_x / cos_x, so the siblings' double-expression lever is ruled out by evidence.
- Final round: SetVSMatrixConstantTransposed(4x4) ruled out (retail 0x00988840 transposes); option (a) re-checked on b2 copy (hdrD) =
  RELOCATION_MATCH, only inside the shared x87 arm (conflicts with coordinator condition). Units-from-inlinees claim downgraded (bd2d caller
  locals moved columns). e80 dref/vref stay on the 4-cycle. b2 left untouched (under audit). Known b2 comment nits for the coordinator:
  a9e1 Shape comment still says the row-vector loop is plain AlignedSetMultiply(local, parent); 29d0's Inferred note sits in a `Locals:` line,
  not a `// Inferred:` line.
- STATE AT HANDBACK: b2 ready (5 rows); 0x00989230, 0x00988E80, 0x008185B0 waiting on escalations (ESCALATE_*.md).
- b3 (option (a), coordinator ruling): W/batches/b3 -- row-only macro FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD_RHSREF_00989230 nested in
  EGOD; row RELOCATION_MATCH (official 20:56); all 5 b2 rows + 8 landed expanders RELOCATION_MATCH under b3/hdr; test 9/9 mutants FAIL.
  Row-text axis (probes/rv4) exhausted first.
