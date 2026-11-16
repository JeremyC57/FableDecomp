# wfD6 notes

## 0x00B1C880 NParticleEngine::CPSCDecalRenderer::ReadBinary -- RELOCATION_MATCH, test PASS, addmatch done
- The earlier draft (hand724 lane) grades RELOCATION_MATCH against today's headers with the source unchanged (only the header
  comment rewritten); the old 4-line SizeFadeMinimum or-order residue is gone with today's CDataInputStream readers.
- Class model: the local CPSCDecalRenderer model of the landed same-class rows (InterpolateTo 0x00B1BDA0, DatabaseCopy 0x00B1B810);
  no engine/CPSCDecalRenderer.h exists (coordinator decision below). Opt-ins: FABLE_RETAIL_CDATAINPUTSTREAM(_READERS) (repo),
  FABLE_RETAIL_DEF_CLASS_BASE / FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE (repo). No flags.
- Name: FableWin + Ego_r name sets; vtable 0x012A09BC slot 15 (slot 16 WriteBinary 0x00B1BA10, 17 DatabaseSerialise, 18 DatabaseCopy).
- Test: standalone model (chunked stream + the packed layout), as the landed CPSCLight::ReadBinary test; mutant (one store dropped) FAILs.

## 0x00B1E380 NParticleEngine::CPSCDecalRenderer::DatabaseSerialise -- DIFFER(3698v3698), 4 real hunks (fg counts 8 more artifact hunks)
- Re-authored on the landed CPSCRenderSprite::DatabaseSerialise 0x00AFBCE0 idioms (GFPackFloat.h GFUnpackFloat / GFPackFloat,
  GFRoundFloatNear(x / 127.0f, 2) for the 7-bit minimums, EmptyCCharString() defaults, local manager / CPSCBase models) and the
  same-class local model. That closed the old SizeFadeMinimum residue of the hand724 draft.
- Residue: 0x00B1F0D6 PoolIncreaseSizeEnable (+0x34 bit 6) merge -- retail `mov al,[esi+34h]; mov cl,[esp+24h]; ... mov [esi+34h],dl;
  shr al,7`, ours loads cl first and does `shr al,7` before the store. Equal length, calls, slots.
- Tried (all identical residue): `!= 0` / `(bool)` reads, `!= 0` / `!!` / `? 1 : 0` stores (+3 bytes), one shared bool for
  Alpha+Size / Size+Life / all three, Size+Life in one block (212 hunks), k = 1..6 unused locals at the top or inside the block
  (no effect at all), declare-then-assign Life local.
- Header model now: engine/CPSCDecalRenderer.h (new overlay header) + engine/CParticleEmitterManager.h with the new
  FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTBLEND opt-in (GetDefaultBlendMode 2 / GetDefaultBlendOp 0); residue unchanged by both.
- More tried: function-scope Size or Life local (207 hunks: frame reslotted), `? true : false` Life read.

## 0x00B1F200 NParticleEngine::CPSCDecalRenderer::PreparePrimitives -- graded only as a probe
- The hand724 round-3 draft compiles only with its stale round-3 overlay (probe/ovl_r3 = round3 hdr + mine): DIFFER(3178v3195).
  Not landable: local stand-in classes, wrong / unproven callee names. Evidence gathered for the re-author:
  0x00AEAAA0 = ?GetGraphicBank@NParticleEngine@@YIABVCGraphicDataBank@@XZ (the draft's "CDecalBankSource" is CGraphicDataBank:
  slot 20 +0x50 GetNoFramesInGraphic, GraphicList +0x1e0, PeekGraphic, CGraphicInfoFrame::GetWidth/GetHeight as in the landed
  RenderSprite PreparePrimitives 0x00AFCC30); 0x00AEAA00 = NParticleEngine::GetOneOverTickRate (?GetOneOverTickRate@NParticleEngine@@YIMXZ
  in reloc_bindings; other spellings seen); 0x00AEA9F0 = ?GetTickRate@NParticleEngine@@YIJXZ; 0x013CD550 = GFastCosTable (engine/GFFastCos.h);
  0x00F36540 = NFractal::CPerlinNoise::GetGaussian2DNoise (PeekPerlinNoise() of the shared CParticleEmitterManager.h).
- Next step: re-author on engine/CPSCDecalRenderer.h with the RenderSprite PreparePrimitives opt-in set (its `// Flags: /d2inlT1537 /d2inlS15`).

## 0x00B087C0 NParticleEngine::CPSCSingleSprite::PreparePrimitives -- DIFFER(2951v2967)
- Draft compiles unchanged on today's headers. Frame map (tools/fmap.py): retail packs sizeScale ([ebp-14h], 6 uses, first
  0x00B08D5D) between the -18 / -10 slots, ours puts it at [ebp-4] and every negative slot moves 4 down (frame 0xE0 v 0xDC);
  start / end CRGBFloatColour homes swapped (+38 v +28); retail shares [ebp+44h] among several short-lived temps (the inline
  GFFloatToLongFloor value at 0x00B08EE2, the 0.5f at 0x00B09168) that ours keeps apart. Declaring end / mid colour first and
  assigning later costs +48 bytes (copies); a hoisted `colour` declaration changes nothing.
- Still to do for landability: CGraphicDataBank / CGraphicInfo / CParticleUpdateParameters / CPSCBase come from shared headers.

## 0x00B01900 NParticleEngine::CPSCRenderMesh::AddToBoundingSphere -- DIFFER(3755v3777)
- Draft compiles unchanged on today's headers; residue as its line 3 (earlier lane). Local stand-in models remain.
- Frame detail (our /FAs listing tmp/fa_ss.asm via tools/fa.py): the 4-byte surplus is the first inline GFFloatToLongFloor's
  `value` home (ours [ebp-20h], shared only with the second instance's `half`); retail homes both in [ebp+44h], the dead
  startColour.A word that `size.Y` later reuses (retail startColour +38..+47 / size +40..+47; ours startColour +28..+37 / size
  +30..+37). sizeScale then lands at [ebp-14h] as in retail. So the question is why VC7.1 will not put the floor temps into the
  startColour / size slot here: `size` (plus `graphic`) in its own nested block (probe/ss/g.cpp, h.cpp) moved slots elsewhere but
  not the floor temp; moving the sizeScale declaration (d/e/f) changes nothing at all.
- Grades used on this row: 12.

## 0x00B1E380 follow-up (key measurement for a successor)
- `PoolIncreaseSizeEnable = poolIncreaseSizeEnable ? true : false;` (probe/ds/p2.cpp, also `!= 0`, `!!`) materialises `cmp ; setne`
  (+3 bytes) but flips the schedule to retail's: member load first, merged store before `shr al,7`, `lea edx` before the local
  store. So retail's IR has a longer bool-operand chain than ours (one extra node that later folds to a plain `mov cl,[esp+24h]`).
  Casts (short / unsigned short / char / unsigned char / unsigned long), `& 1`, `(bool)` reads do NOT add that node. Also no
  effect: TU symbol-numbering changes (extra headers before the row), unused locals.
- Grades used on this row: 37.

## SUCCESSOR session -- 0x00B1F200 CPSCDecalRenderer::PreparePrimitives re-authored (differ/00b1f200.cpp)
- Hand-written from the disassembly in the idioms of the landed CPSCRenderSprite::PreparePrimitives 0x00AFCC30 (same unpack /
  GFSmoothFadeValue / colours / GetPrimID / SetPrimitiveInfo shapes, same `// Flags: /d2inlT1537 /d2inlS15`), on shared headers only:
  engine/CPSCDecalRenderer.h (overlay), new engine/CEnginePrimitiveParticleDecalGroup.h + engine/CEnginePrimitiveParticleDecal.h,
  engine/CDecal.h FABLE_RETAIL_CDECAL_PREPARE (FableWin accessors + SetPrimitiveInfo + the CParticle.h resize specialisations),
  engine/CParticleSystem.h FABLE_RETAIL_CPARTICLESYSTEM_GETDECALLIST.
- Calls equal retail by target (resize: 2-arg 0x00AFE910 / 0x004428F0, 1-arg 0x00443F90 need the explicit vector::resize(n)
  specialisations, exactly as engine/CParticle.h does for RenderSprite). Length 3195 = retail.
- Closed: normalised-time clamp is an if / else-if / else of three assignments (retail stores MinLife / 1.0f as integer moves);
  blend_op declared before blend_mode (edi / ebx); `CRGBColour colour;` declared inside the loop body (function scope made it an
  address-taken function-scope node: colours[] moved 4 bytes).
- Open: (1) 0x00B1FC05 pool-size branch: retail evaluates decal.GetSizeFactor(interpolate) before the render-size lerp, ours after
  (operand order in source does not change it); (2) frame-slot packing of the positive-ebp region (raw diff ~190 lines).
- MATCHED (RELOCATION_MATCH, test PASS, addmatch, read-only vland RELOCATION_MATCH PASS; only WARN RELOC_ALIAS _CIfmod -> import
  thunk). What closed the frame-slot residue, in order: (a) `const float size_factor = decal.GetSizeFactor(interpolate);` named
  local (x87 order: retail evaluates the size factor first); (b) the same-TU constructor CEnginePrimitiveParticleDecal 0x00B1FE80
  defined in the row (landed text, noinline): `colour`'s address no longer escapes; (c) separate named results `alpha` (pooled
  alpha) and `pool_size` (pooled size) instead of re-assigning colour_fade / scale: they took colour_fade / scale out of the param
  home's way (c2 packer: colour_fade w8 > colour w7 joined the dead system_group home first); (d) the header-inline
  CParticleSystem::ScaleParticleDimensions(C2DVector&) body visible in the TU (noinline, the twin's text; retail calls the folded
  copy 0x00AF8F00): that re-weighted `size` (density) and closed the last +18/+1C swap -> RELOCATION_MATCH.
- Flags: /d2inlT1156 alone (smallest; T1155 calls SetPrimitiveInfo, nothing else changes); the twin's /d2inlT1537 /d2inlS15 also match.
- Retail-asm line needed (shared GF* helpers): purity classify -> retail_asm.
- Spellings: EVIDENCED = calls / members / accessors (FableWin + Ego_r), same-TU ctor, ScaleParticleDimensions body (twin text);
  FITTED (chosen by grading, ordinary C++) = the named locals size_factor / alpha / pool_size, `CRGBColour colour;` declared at
  function scope before the loop (loop-body scope grades the same), the if / else-if / else clamp.
- Tools made: tools/g.sh (grade + struct/raw diff), tools/sd.py (patched masking), tools/rd.py, tools/fl.sh (frame groups from /FAs),
  tools/pkev.sh + tools/pk/ (c2 frame-packer probe from wfB3: item weights and join order; it is what found (c)).

## SUCCESSOR -- 0x00B1E380 (still DIFFER 3698v3698, one 3-line scheduling hunk at 0x00B1F0D6; 14 more grades)
- Re-graded unchanged on the current overlay: identical residue (sd.py: 5 structural lines, raw 50 incl. artifact lines).
- Tried this session (all identical residue): Size store `!= false` / `== true` (+3/+4 bytes, like `? true : false`); Life read
  `(x & 1) != 0`, `!= 0`, `== 1`; no-code tuples earlier in the same basic block (the block starts after the FlickerBias pack loop
  at 0x00B1EFE9): parenthesised / `(bool)` Pooling and Alpha reads and stores, parenthesised FlickerBias store, `(bool)false` default.
- Measured model (wfB3 tools/sched2/sim.py documents c2's list scheduler: prio = ((height + outdeg) << 10) + flags, ties by IL index
  ascending): the bool-local load has height h+1 / outdeg 1, the member load height h / outdeg 2 -> equal priority, so the order is
  the IL index order. Ours emits the bool load first (as retail does for bits 4 and 5); retail emits the member load first only for
  bit 6, whose merged byte also feeds the forwarded bit-7 (PoolIncreaseLifeEnable) read. A successor should dump the DAG with
  tools/sched2/t13.py (copy from wfB3) to see which IL index / outdeg differs, instead of more spellings.
- MATCHED (successor, same session): src/00b1e380.cpp = differ source + `#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT` and
  `#define FABLE_RETAIL_GFUNPACKFLOAT_GROUP_PRODUCT` before engine/GFPackFloat.h (existing repo opt-ins; the exact pair the landed
  sibling CPSCEmitterGeneric::DatabaseSerialise 0x00B13530 defines; ROUNDFLOATNEAR alone also matches). How found: the sched2 DAG
  probe (tools/sched2/t13.py) showed the 83-node scheduling region that holds the bit-6 merge ENDS right after the forwarded bit-7
  store (`shr al,7 ; mov [esp+24h],al`), whereas retail interleaves that store with the next statement's `lea` -> retail's region cut
  is later -> retail has fewer... (different) tuples before it in the same basic block; the region starts in the FlickerBias pack
  (tuples 0x2a0 / 0x269 = no-code grouping tuples), so the GFRoundFloatNear grouping opt-in was the natural lever. RELOCATION_MATCH,
  test PASS (test/00b1e380.cpp: save / load / legacy-flag paths over the real packed layout; mutants fail), addmatch, vland PASS.

## SUCCESSOR 2 -- 0x00B087C0 CPSCSingleSprite::PreparePrimitives -- DIFFER(2967v2967), 5 hunks, one cause (differ/00b087c0.cpp)
- Ported onto shared headers (CPSCBase / CParticleUpdateParameters with two new additive opt-ins in hdr/: FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM,
  FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_GETUPDATEPARAM; CGraphicDataBank::PeekGraphic + CGraphicInfoFrame::GetWidth/GetHeight as the twin
  0x00AFCC30; CEnginePrimitiveSprite.h with FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS). Same-TU noinline bodies: SmoothFadeValue
  0x00B05E10, the three ScaleParticleDimensions overloads, CEnginePrimitiveSprite ctor (folded COMDAT 0x006558CE; no byte effect alone).
- Flags /d2inlT1537 /d2inlS15 (twin's) needed: without them CParticleUpdateParameters::GetUpdateParam is called, not expanded. (smallest N: todo)
- Closers so far: frame 0xE0 -> 0xDC came with the port; midColour.A = MidColour.A*(1/255) right after the ctor, then *= fadeAlpha (x87 order);
  `CIEngine* engine = info.Engine;` inside the non-crossed set arm (FITTED: retail holds the engine in edi across the ctor, primID spills);
  trail colour as the implicit conversion `CRGBColour trailColour = colour * colour.A;` (existing opt-in FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR:
  retail converts R,G,B, stores, then A) -> length 2967 = retail, frame map aligned except the TransformPoint Z-row slots.
- Open (13 hunks): 0x00B08AC4 movzx MidColour.A scheduled after the fmul; 0x00B08AFD x87 start/end alpha kept live (retail fxch st(2)+destructive fmul);
  0x00B08C04 Z row E33,E13,E23 order; trail flags/blendOp regs at 0x00B092C6.
- Later closers: TransformPoint FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS (same-TU landed SetTrailPositionsFromSpline uses it; LTR / SUMS /
  PARTICLE_POINT give the same bytes) fixed the Z row and the -4C/-58 slots; trail `Flags` stored before `BlendOp` fixed the trail register hunks.
  `/d2inlT90` (T86 smallest; T85 calls GetUpdateParam); SmoothFadeValue / SetTrailPositionsFromSpline declared protected (PDB IBEMM / IAE).
- OPEN (only residue): the fade alpha. Retail 0x00B08AFA: `fld [edi+58h] ; fxch st(2) ; fmul st(2) ; fstp startA ; fmul st(1) ; fstp endA ;
  fild ; fmul 1/255 ; fmul st(1) ; fstp midA ; fstp st(0)` = ONE load used three times, start / end alpha killed at their multiply.
  Unnamed `x.A *= system.GetFadeAlpha()` (current text): three `fmul [edi+58h]` (no CSE even with no stores between: o1 probe); named
  `float fadeAlpha` (any position, const, block, reuse of `alpha`, GetA()): loaded once but the start / end values are kept (`fld st(0) ;
  fmul st(3)`) and popped at the block end, plus a movzx scheduling hunk at 0x00B08AC4. Declared before midColour (d2 probe) it is loaded early
  (wrong place) but THEN killed like retail: the pinning depends on the variable being defined in the same block. Tried also: fa first operand,
  /255.0f, combined mid statement, CRGBFLOATCOLOUR_INITIALISE_CTOR, CRGBCOLOUR_CTOR4_INITIALISE. Next idea level: c2 x87 variable pinning
  (why a same-block float variable is pinned), or an inline helper whose parameter is the fade alpha.
- MATCHED (RELOCATION_MATCH, test PASS (test/00b087c0.cpp: body verbatim over a model; 6 scenarios; mutants on flags / angle spread / trail
  length fail), addmatch, read-only vland RELOCATION_MATCH PASS; WARNs only: _CIfmod import alias, and RELOC_NAME 0x0129BA3C labelled
  g_NormaliseEpsilon in ghidra_out/labels_landing_2026-11-11cl.tsv while reloc_bindings / Ego_r / 213 landed rows say ?DELTA_NOTIONAL_ZERO@@3MB).
  The last closer: `float fadeAlpha = 0.0f;` defined with the locals at the top, assigned system.GetFadeAlpha() at its use. c2 orders the
  commutative x87 multiply operands by symbol number, and a float local is numbered at its FIRST DEFINITION (a bare declaration, e1..e3,
  does nothing); defined before the colours, `startA * fa` puts startA first -> retail's `fxch st(2) ; fmul st(2)` kill. Not needed in the
  end (removed): the visible CEnginePrimitiveSprite ctor, the SmoothFadeValue body. Needed: the three ScaleParticleDimensions bodies (frame).
  Flags /d2inlT86 (smallest; T85 calls GetUpdateParam). FITTED: fadeAlpha's `= 0.0f` definition, `CIEngine* engine = info.Engine;`.

## SUCCESSOR 2 -- 0x00B01900 CPSCRenderMesh::AddToBoundingSphere -- DIFFER(3749v3777), 19 structural hunks (differ/00b01900.cpp)
- Ported off the all-local draft onto shared headers (C3DVector / CMatrix3x4 ZYX_SUMS / CQuaternion / CRightHandedSet / C3DBoundingSphere /
  CMeshDataBank + C3DMeshInfo BOUNDINGSPHERE / CParticle SPRITE / CParticleSystem UPDATE / CPSCBase / CParticleUpdateParameters), local
  CPSCRenderMesh model only (as the landed RenderMesh siblings). Body changes: GFUnpackFloat (11 / 10 bits, 2 places) for the draft's UnpackParam,
  system.TransformPosIfLocal(pos / trailEnd) for the three StayWithEmitter blocks, system.ScaleParticleDimensions(size) (new opt-in),
  `particle->IsActive() && particle->IsActive(timeOffset)` (retail tests the Active byte +0x7A first, 0x00B01C3B; fixed 10 hunks).
  The inline GFFastArcCos / GFFastSin / SetSlerpFromIdentity texts are the landed 0x00AA0090's (FORCEINLINE, as there).
- New opt-ins (hdr/README.md): C3DBOUNDINGSPHERE_ADDTOBOUNDS, CPARTICLESYSTEM_SCALEVECTOR, CPARTICLE_INTERPOLATEDORIENTATION (FORCEINLINE: plain
  inline is called out of line, -500 bytes), CQUATERNION_MUL. Flags /d2inlT1537 /d2inlS15 (twin's; without them the particle helpers are called).
- Open: frame 0x298 v 0x2B0; pad 0x00B01C38; x87 blocks 0x00B025A1..0x00B026A3 (trail-end TransformPoint orders and the !CentredOnPos
  matrix Set(-x,-y,z) + TransformPoint(src,dst): retail negates with fchs late and holds values on x87); GetInterpolatedLifetimer operand
  order 0x00B026FF (retail loads CreationTime byte and MaxLife before the subtract); 0x00B02493 ours uses qword constants (sqrt path).
  Next: the shared GetInterpolatedPos / GetRecordedPos texts are Interpolate-based, the draft's were `(P0 - P1) * t + P1` (try a twin opt-in),
  then the 087c0 lesson (define-order numbering of float locals) on the x87 blocks.

## SUCCESSOR 3 -- 0x00B01900 CPSCRenderMesh::AddToBoundingSphere -- progress log (differ/00b01900.cpp, working files w4/)
- 3749 -> 3777v3777 (equal length, 1078 instructions each, 70 differing lines) from five facts:
  1. GFFastArcCos had lost the landed text (`(cosine + 1.0f) * 512.5f` and the `* 0.15915494f`, 0x00B022FB / 0x00B0233C): now the landed 0x00AA0090 block.
  2. ToRightHandedSet scale is single precision in retail (0x00B02493 `fdivr dword [2.0f]`, 0x00B0249D `fld dword [1.0f]`): new nested opt-in
     FABLE_RETAIL_CQUATERNION_TORIGHTHANDEDSET_FLOAT (the shared text has double literals; no landed row expands it).
  3. Mesh-centre block (0x00B02595..0x00B026C6) = CalcObjectMatrix's shape, not Set + TransformPoint: `right *= size.X * meshScale;` in place,
     `forward = rhs.GetForward() * (..)`, `up = rhs.GetUp() * (..)`, per-member `matrix.E11 = -right.X ..`, then
     `matrix.TransformVector(meshSphere.Center, offset); pos += offset;` (FableWin ?TransformVector@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z; new opt-in
     FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SOURCE_DEST). Frame became 0x2B0.
  4. The six start / end render sizes are six `const float` locals, not two C3DVectors (retail homes -0x5C / -0x58 / -0x68 / -0xA4 / -0xD0 and
     endSizeZ in +0x30, the fild-temp slot; the Anniversary PDB lists start_render_size_x.. as const float): raw diff 323 -> 77.
  5. FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT (operator*(float) with the named result, FableWin 31i/82b): fixes the forward-scale x87 shape 0x00B025DA.
- Anniversary PDB memory-homed locals (tmp/anniv_b01900.txt): end/start_render_size_x/y/z (const float), trail_width, trail_length_f, mesh_bsphere,
  size, trail_pos2, it_particle, pos, rhset, right, orient. No named matrix / forward / up / trail vector there.
- Tried and worse: named-result operator- / + / ^ (FableWin +6i/+20b each): 39..66 structural hunks. XY_THEN_Z / SQUAREDMAGNITUDE_XYZ / _ZYX: no change at the trail magnitude.
- Tools: w4/mk.py (template base.tpl + block / pre files), w4/gr.sh, w4/fo.py (retail->our frame slot map), w4/rmap.py (retail slots labelled with our
  packer items; needs tools/pkev.sh first), w4/ld.py (line diff when counts are equal), w4/sbs.sh, w4/mag.sh, w4/pkw.sh (packer weights by name).

## SUCCESSOR 4 -- 0x00B01900 CPSCRenderMesh::AddToBoundingSphere -- RELOCATION_MATCH (src/00b01900.cpp, test/00b01900.cpp)
- Base: successor 3's w4/s7d.cpp (= differ/00b01900.cpp body; 3777v3777, 17 lines at three x87 sum-term order sites 0x00B01CF5 / 0x00B02135 /
  0x00B024CF, all keyed on c2 symbol-record ids; successor 3 showed a +33..+75 parenthesis "record shift" closes them -- diagnostic only).
- Closed with owner device d (WF_RULES 2026-11-14): 3 dead FABLE_RETAIL_ASSERT arms (new overlay hdr/fable_assert.h, the shared copy used by
  wfB4 / agentC6 / C20 etc., md5 5caeed..). Sweep (w4/a<n>.cpp, same site): n = 1,2 DIFFER (7 lines, 0x00B02135 left), 3..6 RELOCATION_MATCH,
  7,8 DIFFER (12 lines). Position-independent before the loop (function top, before the trail scale, inside `if (BankIndex)`: all match at 3).
  Device c first: k = 1..7 unused initialised float locals (w4/l<k>.cpp) leave 0x00B02135 wrong at every k (k >= 4 fixes the other two sites).
- Final conditions (FITTED text, plausible invariants): trailWidth >= 0.0f, recordedLength <= 2.0f (before the trail scale), meshSphere.Radius >
  0.0f (before 1.0f / Radius). FableWin /Od: row 604i/2310b v ours 474i (0 asserts) / 537i (3 asserts): the debug row is ~130i larger, consistent
  with asserts in the original (FableWin assert arms are ~60i each; ours ~21i).
- Test: the row body verbatim over a model world (trail short / long / width floor, size lerp + both clamps, render-size param present / absent,
  dimension scale, oriented mesh-centre offset v CentredOnPos, dead-particle clear, handle clear); mutants (width floor, CentredOnPos, MaxLife
  test removed) all FAIL.
- Grades used this session: ~30 (incl. sweeps).
- Read-only vland (overlay set): all 5 batch rows RELOCATION_MATCH + behaviour PASS; only WARN RELOC_NAME DELTA_NOTIONAL_ZERO (NAME_FIXES row 0129BA3C) and the _CIfmod thunk alias. lv/ removed.
- Fix-up (inline-budget audit): Flags measured, not borrowed: /d2inlT332 alone (was the twin's /d2inlT1537 /d2inlS15). Sweep: default 3239v3777, T100 3291,
  T300..T331 3522v3777 (one extra call: CQuaternion::ToRightHandedSet, retail expands it at 0x00B02455..0x00B02595), T332..T1537 RELOCATION_MATCH; S15 alone = default, S not needed. // Inline-budget: line added; q.py + vt PASS + addmatch + read-only vland (5/5 RELOCATION_MATCH PASS) redone.
