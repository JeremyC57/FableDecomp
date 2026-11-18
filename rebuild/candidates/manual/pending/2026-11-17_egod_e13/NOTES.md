# e13 notes (egod lane, 2026-11-18)

## 0x009BCA20 DisplayCenteredText -- RELOCATION_MATCH (1104v1104), test PASS (3 grades), vland RELOCATION_MATCH PASS
- Same text as e12's matched twin CProgressDisplay::DisplayCenteredText 0x00497FD0 minus its two manager null checks, with this
  function's Ego_d names (pdisplay, MaxNumLines, NumLines; loop block left / length / start_draw_pos / draw_pos), the receiver as
  `pdisplay->GetRenderManager()` per draw (retail re-reads PRenderManager each time).
- First grade: 4 hunks (0x009BCC63 fstp after push; 0x009BCD83 receiver reload order). FITTED the existing
  FABLE_RETAIL_CFONTBANK_VFONTMAXHEIGHT_PAREN (as the twin): RELOCATION_MATCH. No flags (the old draft's /Gr replaced by explicit __fastcall).
- Header: CWStringParser INLINE_CTOR block copied from e12 (hdr/README.md). Test: standalone model (twin's test adapted, flags argument recorded).
- vland: one WARN RELOC_NAME (??BCWideString@@QBEPBGXZ bound to 0x0099AF00, labelled 'CWideString::operator unsigned short const *': same function, PDB spelling).

## 0x007E8A40 CTCFlourishTarget::ProcessActionMovementDelta -- RELOCATION_MATCH (482v482), test PASS (7 grades), vland PASS
- Rewritten from Ego_d: `const`, parameter `C3DVector& movement_delta` (the old draft had `C3DVector*` non-const: wrong leaf), x_axis in
  the outer block, components / z_axis / y_axis in the inner block; members PAttacker / CullNegativeXAxisMovement / CullAllYAxisMovement
  (Ego_d + Ego_r; the draft's NoBackwardMovement / NoSidewaysMovement were descriptive). Callees by their bound PDB names:
  CThing::GetPos 0x004C73D0, C3DVector::AccurateNormalise 0x00A14440, DELTA_NOTIONAL_ZERO via IsNonZero.
- First compiling grade 8 hunks (IsAlive's not/test) -> `!PAttacker->IsDead()` 4 hunks (y dot order) -> FITTED existing
  FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP: RELOCATION_MATCH. Dot receiver swaps / memberwise components: no effect.
- vland WARN RELOC_NAME: DELTA_NOTIONAL_ZERO 0x0129BA3C labelled '?g_NormaliseEpsilon@@3MB' in the label table (the bound PDB name is
  ?DELTA_NOTIONAL_ZERO@@3MB, as every shared header spells it).
- New header engine/CTCFlourishTarget.h; coordinator may want to retype the landed 0x007E8A00 / 0x007E8A20 local models onto it.

## 0x00803540 CTCTargetingPlayer::UpdateTargetedPositionRandomWalk -- OPEN, DIFFER(1108v1108) (12 grades), differ/00803540.cpp
- Ported the 2026-11-05 draft (compiles on today's headers, 7 hunks) onto Ego_d's names: ptc_ranged (function scope); block: build_up
  (GetPullBackAmount), accuracy (unused GetShotAccuracyFraction), shake_radius, pcam_man, line_to_target_pos, dist_to_target, main_radius
  (dist / 7), second_order_radius (main * 0.1 / * RangedAccuracyShakeFractionOfRadius), ptc_exp, creature (CThingCreatureBase&),
  hero_combat_def (const CHeroCombatDef&), weight, x_axis, y_axis, angle, random_walk_mag. Removed the draft's non-Ego_d named locals
  (thing, def, parameter, sine, cosine): same 7 hunks, so the named sine/cosine were not needed.
- Residue (unchanged by everything below): 0x0080380E `mov [esi+0CCh],edx` (the C2DVector(sin,cos) temp copy) scheduled before `fcos`
  in ours; 0x00803890 `lea eax,[esi+9Ch] ; mov ecx,eax ; pop edi` placed 3 x87 instructions later in ours. The two `mov [esp+..],0`
  "replace" hunks are identical text (fg noise).
- Tried: offset_scale = 0.0f local for the literal 0 multiply (same / 8 hunks); offset_scale_second_order = 1.0f (1126 bytes: a variable
  1.0 is not folded, so it is not that); y_axis * (scalar product) grouping (1066); C3DVector op opt-ins INITIALISE_ADD / ADD_NAMED_RESULT
  (7, no change), ADD_XY_GROUP (8), INITIALISE_SCALE / SCALE_NAMED_RESULT (1118), SCALE_X_PAREN (11).
- Evidence for the next level: fwsize says FableWin/Ego_d's body is 786i/3123b, ours 278i/990b at /Od; Ego_d lists a nested scope
  (size, tan_angle, max_angle, rad_outer, rad_inner, rad_use, line [C3DVector], offset_size, angle_use) and the unused-looking
  need_new_desired_pos / offset_scale / offset_scale_second_order: a ~500-instruction block that release compiles away (constant-false
  need_new_desired_pos?). Its symbols / tuples shift c2 numbering; without its text this row stays a scheduling tie. Coordinator: needs
  the debug body (no FableWin.exe / Ego_d.exe binary on this machine) or an owner call.

## 0x009FB750 CTexture::LoadFromDataStreamToPreallocatedSurface -- RELOCATION_MATCH (789v789), test PASS (22 grades), vland PASS
- Pass 1 (17 grades) ended at the draft's 5 hunks only with width / height locals Ego_d does not list; Ego_d's data_length flipped
  stream / data_length between ebx and ebp. Pass 2 (advisor lever): retail keeps locked_rect.Dimensions.X in edi across the
  GetByteColourDepth calls, so VC7.1 must know Lock's hidden result slot does not escape: a noinline stub Lock body matched at once
  (probe), then the REAL Lock body (Ego_d locals dimensions / rect / locked_rect / hr / lock_info; GetDimensions inline,
  WaitWhileResourceIsBusy, LockRect) matched too.
- Final text = Ego_d-faithful: no width / height, data_length, src before dst, up-counting cy, `c1 < GetNoLevels()` and `Unlock(c1)`
  (the class's own same-TU members), compressed block `hr = GetLevelDesc(c1, &desc)` + `stream.Read(locked_rect.PMem, GetLevelByteLength(c1))`.
- Same-TU bodies in the row, Ego_d order, each graded with this file at its own address: GetPixelFormat 0x009F9D80 RELOCATION_MATCH,
  GetDimensions 0x009F9DB0 MATCH, Lock 0x009FA450 RELOCATION_MATCH, Unlock 0x009F9DE0 MATCH, GetNoLevels 0x009F9DF0 MATCH,
  GetLevelByteLength 0x009F9E80 RELOCATION_MATCH (all six are landed today with local models; these are shared-header texts).
- Inline-budget: __declspec(noinline) on Lock only. Headers: Direct3DTexture9.h (PASS22_D3D_TEXTURE_ORIGINAL_BASES), the render-manager
  set for WaitWhileResourceIsBusy (CRENDERMANAGERCORE + PASS22_CORE_COMPLETE_ORIGINAL_DECLARATION + its BUFFER_ACCESSORS / RENDERSTATE /
  SHADER method-order companions + CVERTEXSHADER / CVERTEXBUFFERWIN32 / CINDEXBUFFER), existing CTEXTURE_GETNOLEVELS, NEW
  CTEXTURE_LEVELBYTELENGTH, NEW CTextureFileInfo.h / CPixelFormatInit.h. A local `typedef long HRESULT;` (winnt.h spelling) for Ego_d's hr.
- vland WARN RELOC_CONFLICT: ??0?$vector@EV?$allocator@E@std@@@std@@QAE@I@Z is 0x005136B0 here (retail's call target at 0x009FB95F) and
  0x00514E20 in landed 0x00996610: two retail COMDAT copies of one symbol (coordinator decision).
- Test: a model with recorded memory traffic; both overflow tests just above and exactly on 0xFFFFFFFF, signed width / height
  (kill zero-extension mutants), level-count bound, pitch != line_length, per-level compressed sizes.

## 0x00A3C3B0 C3DAnimationBlendState::SetInterpolate -- OPEN, DIFFER(777v777) 9 hunks (16 grades), differ/00a3c3b0.cpp
- Rewritten on the shared headers (the draft was an all-local model, 48 hunks / 745 bytes on first port): C3DAnimationBlendState
  TYPED_DATA / SHAREDDATA / ACCESSORS (PeekBaseAnimation, GetCombinationBlendCount, PeekCombinationBlend), the landed CreateInterpolate's
  opt-in set, new SETINTERPOLATE and C3DANIMATIONSTATEALPHA_CTOR opt-ins, STLP_VECTOR_RESIZE_OUTOFLINE (retail calls 0x00A3D500).
  Ego_d names: anim1, anim2, weight2, no_blends, c_blend; loop block dest / blend1 / blend2.
- What moved it: the lerp as the shared GFGetInterpolatedValue (keeps old_val on the x87 stack: 745 -> 780), `dest` declared before
  blend1 / blend2 (44 -> 12 hunks), the morph test as `!anim1.Data->SkeletalMorph.IsNull()` (retail has no Data null test; the header's
  HasSkeletalMorph adds one: 781 -> 777), FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED (FableWin's /Od-sized text; 11 -> 9 hunks).
- Residue: 4 integer loads placed before the held old_val's `fstp st(0)` in the unrolled loop (retail after it): 0x00A3C4FC, 0x00A3C54F,
  0x00A3C583, 0x00A3C5A5. Tried without effect: accessors (GetAnimationIndex / GetAnimationTime / GetAlphaWeight), other declaration
  orders (23-46 hunks), ++c_blend + an inline null/size count, the same-TU CreateInterpolate body defined in the row.
- Pass 2 (2 grades): FABLE_RETAIL_CARRAY_INDEX_ASSERT (dead / dead-arm asserts in operator[]): no change. fwsize DIFF helpers still open:
  C3DAnimationStateAlpha ctor 14i/41b v FableWin 15i/39b (AlphaWeight(1.0f) / (1.0) / (1) / assignment all 14i/41b), CIVCountedPointer
  operator= / Release, CCountedPointer operator= (texts differ from FableWin; not on the residue's path).
- Next: the MakeOwnCopyOfData body (same TU, landed with a local layout) defined in the row; the dest index store after the lerps.

## 0x008EEC70 Mass::Collide -- OPEN, DIFFER(1129v1129) 2 hunks = 1 instruction (8 grades), differ/008eec70.cpp
- Ego_d confirms every Collide local of the draft (div1, move, bounce_impulse, verlet_impulse, sliding_friction_impulse,
  max_sliding_friction_impulse) and the TU order Right, CopySign, Mass ctor/dtor, Update, GetVel, ApplyImpulse, ApplyMove, ForceToVel,
  BackplaneFriction, Collide. Defining the four members plainly in that order reproduces retail's inline split (ApplyImpulse / ForceToVel
  called, GetVel / ApplyMove expanded) with NO `#pragma auto_inline(off)` and no /Gr: same 2 hunks as the draft, cleaner source.
- Residue unchanged: 0x008EEFA8 `mov eax,esp` (arg-slot copy of Right(normal) for the denominator's ForceToVel) hoisted to 0x008EEF2E.
- Tried without effect: the landed BackplaneFriction body defined before Collide (Ego_d order), inline CopySign, non-inline Right.
  Worse: C2DVector NAMED_RESULT_ADD/SUB/MUL (FableWin-sized operator texts: 12-15 hunks), Right with a named result (6 hunks).
  fwsize: FableWin Right is 17i/40b v ours 16i/37b, Dot 18i/45b v 16i/37b (texts differ; not yet found). Pass 2: a named-float Dot
  (17i/42b, still DIFF) made it worse (13 hunks, 1125 bytes); overlay dropped.
