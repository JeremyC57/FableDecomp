# Hand chunk 3001 (C3DAnimation2 .. CPhysicsMeshObject): 1 of 4 byte-exact

Retry pass on four rows earlier agents (chunks 2001 / 2005) parked near a match. Every grade used
`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3001/hdr` (hdr/README.md: union of the 2001 and 2005 overlays plus the 3001 opt-ins).
Row extents were checked again from Fable.exe (each ends in its `ret`, 0x00A12EE0 includes the code after its first `ret`; all four
oracle lengths are right): no SPLITS.tsv.

## What matched (1)
- **C3DAnimation2** -- 0x00A4D920 `?CalcLocalBoneTransformationRelative@C3DAnimation2@@QBEXAAVC3DKeyframe@@KMABVC3DMesh2@@@Z`
  (RELOCATION_MATCH, test PASS under g++ and VC7.1). The Relative form is
  `C3DKeyframe local; CalcLocalBoneTransformation(local, bone, time); keyframe = mesh.BoneKeyframes[bone]; keyframe.PostApplyRelativeAnimation(local);`
  The chunk 2005 source was a hand-written expansion of all of that and stuck at one x87 pair (0x00A4DAC6). How the shape was
  found: retail 0x00A4D330 is CalcLocalBoneTransformation (both PDBs, `ret 0Ch`), its frame is 0x38, and 0x38 + 0x30 (one local
  C3DKeyframe) is this row's 0x68. Its body is this row's first 0x500 bytes with the keyframe in the local. VC7.1 only expands it
  when it is forced: plain and `inline` stay called at /d2inlT100..800 (118 bytes), because its body reaches the GFFloatToLongNear
  asm helper and VC7.1 never auto-inlines a function containing asm. That is ASM_RULES rule 3.3, the same reason
  ConvertTimeToFrames (GFFloatToLongFloor) stays called at 0x00A4D93D. The tail is FableWin's
  ?PostApplyRelativeAnimation@C3DKeyframe@@QAEXABV1@@Z (chunk 2005's unused C3DKEYFRAME_POSTAPPLY opt-in; operator*= 0x00A88C10,
  then the four position adds). Grade `retail_asm` (GFFloatToLongNear helper; `// Retail-asm:` line cites 0x00A4DA78 / 0x00A4DAE9 /
  0x00A4DB59). The one-pair residue the 2005 agent chased was a by-product of the wrong IL shape: with the real nesting it is gone.
  - C++ attempts before the helper (ASM_RULES table): the helper sites are the shared GFFastArcCos / GFFastSin bodies already
    used by landed 0x00AA3C50 / 0x00AA3390 (bare `fistp dword`; `(long)x` gives __ftol2, /QIfist gives `fistp qword`); this row adds
    no new asm site.

## Parked for Fable 5.1 (3)
- 00a9eb90 C3DAnimationManager::BlendAnimations DIFFER(1337v1337): 0x00A9EC5A retail `fld dword ptr [eax+edx]` (d9 04 10), ours `fld dword ptr [edx+eax]` (d9 04 02) | tried: the prior ~960 neutral spellings plus here: unused-local count k=0..16 (inert), dead-statement sweeps at entry / inside the SSE arm / at the end (identical thresholds: position-independent), a 2-D grid (dead named locals x dead stores), natural levers (named GFInterpolateVectors / GFFinishPartialSlerp pointers, a stride constant, `bool sse`, a cycles local, signed / unsigned counters and globals, a LARGE_INTEGER-style union, `weights = 0` first, `one_minus_t`, shared index, CalculateSlerpWeights and SetSlerp written out in their loops, three RAII cycle-timer classes), start / end GFGetCycleCount() forms | measured: the source shape is right. Adding 6 dead named locals and 24 dead stores (probe/a9_diag_keyed.cpp, diagnostic only, never a deliverable) grades RELOCATION_MATCH. The SIB order is a per-function c2 key that dead IL moves, and every copy-propagated natural spelling leaves it alone. The scalar loop's pairs flip at a lower count than the SIB (n = 12 / 13 v 14 in the one-symbol sweep), so a single count cannot satisfy both; the grid's zero region needs >= 5-6 symbols AND >= 24 statements. next: c2 trace of where dead IL changes the record / id order the SIB ADD is canonicalised by (candidate constructor 0x1077C1DE + the address-mode builder), then the natural construct that frees the same records. Test: differ/00a9eb90.test.cpp (chunk 2001, PASS).
- 00ab2520 C3DAnimationBuilder::BuildMeshAnimation DIFFER(1265v1265): x87 only, 19 lines -- SetInverse E22 0x00AB2698 (retail `fld [eax+0Ch] ; fmul [eax+2Ch]`, ours swapped), E41's last product 0x00AB2707 (retail `fld [edx] ; fmul [eax+30h]`, ours swapped), PostMultiply E22 / E23 term order 0x00AB27E0..0x00AB2815 (retail a.E22, a.E23, a.E21; ours a.E21 first) | tried: chunk 2006's nesting applied to PostMultiply (FableWin donor order: ?PostMultiply@CMatrix3x4@@QAEXABV1@@Z 0x02604F80 = 0x40 bytes, right before ?SetMultiply 0x02604FC0, so `CMatrix3x4 copy(*this); SetMultiply(copy, m);`): 66 -> 23 lines; a `CMatrix3x4& transform` in the identity arm: 19; SetInverse through FableWin GetDeterminant (neutral, kept) / SetInverseNoTranslate (45), SetMultiplyNoTranslation (neutral), C3DMesh2::GetBone accessors (neutral), named parent / bone_info / count-type / operand-order spellings, four GetDeterminant expansions, E22 / E41 operand spellings (canonicalised, neutral) | measured: the remaining pairs are keyed like 0x00A9EB90 (a dead-local sweep 0..60 moves them chaotically between 8 and 61 lines, never 0); the E22 pair never flips in any variant, so it is probably structural (the inline SetInverse / GetDeterminant nesting in this TU) | next: c2 trace of the cofactor load-node creation order for E22 = E11*E33 - E13*E31; the pair-landing dependency on 0x00AB1CC0 (same-TU noinline BuildBoneInfoTree body, RELOCATION_MATCH for its 612 bytes) stands.
- 00a12ee0 CPhysicsMeshObject::GetNavigationLevelVolumes DIFFER(1118v1144): retail keeps a second outer IV it+8 in [esp+14h] (0x00A12F08 `lea eax,[ecx+8] ; mov [esp+14h],eax`, 0x00A12F16 `mov eax,[esp+14h] ; mov eax,[eax]`, 0x00A1329C..0x00A132AF `add edi,10h`; frame 0x64 v 0x60, so every frame offset is shifted by 4), plane-point fld scheduling 0x00A12F62 / 0x00A12F73, TransformPoint term order + AccurateNormalise copy interleave 0x00A13014..0x00A13078 (29 lines with frame offsets masked) | tried: FableWin TransformPlane / TransformVector(src, dst) as inline bodies (39), TransformPoint row spellings (SUMS / LTR), point spellings (operator*, *=, assignment), `normal.AccurateNormalise();`, index loops over levels[i] with < / != / begin()+i tests (93-103), references to it->second / *it / the volume, explicit `(const char*)it + 8` pointer, two GetConvexVolume calls, dead sweeps (fully insensitive), minimal OSR probes (probe/osr/: every form folds it+8 into the address) | next: a c2 strength-reduction trace on the minimal probe to learn when VC7.1 materialises a derived IV (none of the source shapes tried does), then retry with the real STLport / CVectorMap iterator class or a callee that takes &it->second.

## Skipped
None.

## Names
- Proven (two sources each, LABELS.tsv): the four row symbols, CalcLocalBoneTransformation 0x00A4D330, ConvertTimeToFrames
  0x00A52650, CQuaternion::operator*= 0x00A88C10, BuildBoneInfoTree 0x00AB1CC0; inline-only PDB members used:
  C3DKeyframe::PostApplyRelativeAnimation, CMatrix3x4::SetMultiply / PostMultiply / GetDeterminant (FableWin).
- Descriptive (unchanged from chunk 2001, parked row only): g_BlendAnimationsCycles 0x013D2D88 / g_BlendAnimationsKeyframes
  0x013D2D90. g_ArcCosTable 0x013CB530 / g_CosTable 0x013CD550 are bound spellings, but reloc_bindings.tsv also binds
  g_ACosTable / g_cosTable to the same addresses (MULTINAME; no PDB data-symbol dump on this machine: coordinator's call).
- NAME_FIXES.tsv: 0x00A52650 (landed leaf C3DAnimation2_Methods::ConvertTimeToFrames; repeated from chunk 2005, the matched row
  binds the PDB spelling).

## Header debt
- All overlay changes are opt-ins (hdr/README.md). The matched row needs the chunk 2005 opt-ins C3DANIMATION2_CALCLOCAL,
  C3DANIMATIONSEQUENCEDATA_INTERPOLATE, CQUATERNION_SLERP, CQUATERNION_MULASSIGN, C4DVECTOR_ASSIGN3D, C3DKEYFRAME_POSTAPPLY plus the
  new C3DANIMATION2_CALCLOCALBONE.
- engine/CMatrix3x4.h now carries the chunk 2006 SETMULTIPLY block; merge it once with agent2006's copy (identical text).
- The row defines GFFastArcCos / GFFastSin / CQuaternion::Dot / SetSlerp / InlineGetKeyframeInterpolate / ConvertTimeToFrames bodies
  row-locally (as the landed 0x00AA3C50 / 0x00AA3390 do): candidates for one shared header.

## Tools (tools/, copy freely)
- fit.py: VC7.1 compile + relocation-normalised instruction diff (~0.5 s; FITMASK=1 also masks [esp+N]); gen.py: lever-product
  generator over it; sweep.py / grid.py: diagnostic insertion sweeps (1-D / 2-D); hvar.sh: fitness against a modified overlay copy;
  ours.py: disassemble our object; c2.sh: c2prio wrapper.
