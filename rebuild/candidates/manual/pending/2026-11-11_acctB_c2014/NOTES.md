# Hand chunk 2014 (C3DAnimation2 .. C3DAnimation2): 1 of 2 byte-exact

Both rows were checked against Fable.exe first: each starts at a real function start and ends at its own `ret` (0x00A4E950
`ret 0Ch`, 0x00A50BE1 `ret 10h`); the oracle lengths (1619, 1748) are right, so there is no SPLITS.tsv. Every grade used
`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2014/hdr` (hdr/README.md).

## What matched (1)
- **C3DAnimation2::InitialiseAnimationSequences** 0x00A50510 `?InitialiseAnimationSequences@C3DAnimation2@@AAE_NAAV1@ABVC3DMesh2@@_NAAK@Z`
  -- RELOCATION_MATCH (1748 v 1748), test PASS under g++ and VC7.1; every call target checked against retail (tools/rel.py).
  What it does: relative flag = bit 2 of GetType(); a local C3DAnimationBuilder and N3DAnimationBuilder::COutputAnimation; for a mesh
  with bones the builder's BuildMeshAnimation result goes to the out parameter; SequenceCount / Sequences / SequenceDataBufferSize /
  SequenceDataBuffer from the output (New<T> out of line); per sequence the bone's parent index, then shared data
  (InitialiseShared: data, shared, parent -1) or the next private block initialised as a constant; BoneMask / RigidBoneMask /
  RigidBonePositions through AllocateType<T>; a local std::vector<bool> of rigid flags; per bone SetInverse of the localization matrix,
  PostMultiply by the parent's, rigid = parent rigid || parent had more than one child, rigid flag and translation stored, bone mask
  copied through the original sequence index.
  Shape lessons that made it match (in the order they were found; each graded):
  1. The accessors are the ones whose unreferenced COMDATs this TU keeps in its inline run (0x00A4BB40..0x00A4BD70: GetType,
     GetFrameCount, GetSamplesPerSecond, SetParentIndex, C3DBone2::GetParent / GetOriginalNoChildren / PeekLocalizationMatrix,
     C3DMesh2::GetBoneCount / GetBone); C3DAnimationSequence::InitialiseShared is a header inline (COMDAT 0x00A4C140) and
     C3DAnimationSequence::InitialiseAsConstant an ordinary member defined in the TU (out-of-line copy 0x00A4FE30) that /O2 expands --
     the row defines it above itself.
  2. The bone-mask copy is an if / else of two stores: the value ternary moved the static frequencies and gave the loops retail's
     missing alignment padding (252 -> 154 diff lines).
  3. `info` (the output's CSequenceInfo) taken before `sequence` in loop 1: fixes the esi / edi roles of shared_animation / i and the
     loop-1 entry (154 -> 34 lines).
  4. The rest was x87 operand order of the SetInverse cofactors, its translation row and the PostMultiply column-1 sums. VC7.1
     reassociates each sum; the order is the IL read order of the inverse / copy fields after reassociation, which c2prio's ref events
     expose for free (tools/e41fit.py: the nine SetInverse result fields mapped by their write tuple, the read order per translation sum;
     tools/pmfit.py: the copy-field read pattern per PostMultiply sum). With that metric (no grade) I searched natural spellings and only
     graded the winners: std::vector<bool> instead of CArray<bool> (34 -> 22), a separate `++data_index;` statement (22 -> 4), then the
     named `const C3DBone2* bone = mesh.GetBone(i);` in loop 1 + the explicit `C3DVector(local.E41, local.E42, local.E43)` translation
     + a named `long original_index = info.OriginalIndex;` in loop 1 (4 -> 0).

## Parked for Fable 5.1 (1)
- 00a4e300 C3DAnimation2::GetDeltaMovement DIFFER(1609v1619): first wrong decision 0x00A4E4F4.. -- retail keeps the start rotation's
  E11 / E12 / E13 on the x87 stack through its ToMatrix and spills E12 / E13 only at 0x00A4E567 (`fxch st(1) ; fstp [esp+8] ;
  fstp [esp+18h]`), ours stores one of them in place (`fstp [esp+30h]` / `fsubr st(1)`); then the product rows 2 / 3 term order
  (retail 0x00A4E766 b11*a12 + b31*a32 + b21*a22, 0x00A4E7CE b21*a23 + b11*a13 + b31*a33; ours a22 / a33 first), the translation
  0x00A4E90A / 0x00A4E90D term order, the second norm's term order (0x00A4E578 X, W, Z, Y), frame 0x100 v 0x104 (97 frame-masked diff
  lines) | tried: in-place Transpose + SetMultiplyNoTranslation (zeroing translation, as retail's COMDAT 0x00A4B820) with forced
  C3DKeyframe::ToMatrix / CQuaternion::ToMatrix (both needed: plain inline is called, c1 267 / 217 in the inline trace), protected
  MovementVector read in place (the getter's copy is GetMovementAtTime's shape), and ~250 free fragment probes of the transpose:
  48 swap orders, 72 three-temp forms, 20 six-temp forms, copy-based / Set / ctor / hand-written / loop forms, SetMultiply written
  orders and nestings. The T1 form (lower triangle into temps, then E21 = E12 ...) is the only one that gives retail's row 1
  (`b1j*a11 + b3j*a31 + b2j*a21`) | next: the free c2 read-order metric that closed 0x00A50510 (tools/gdfit.py is the start of a
  GetDeltaMovement version; its candidate mapping still mixes the start / end fields) over transpose and accessor spellings, then the
  first-ToMatrix register promotion of E11..E13. Passes 2 and 3 of this row were not used: the working budget went into 0x00A50510.
  Best source + test: differ/00a4e300.cpp, differ/00a4e300.test.cpp (PASS under g++ and VC7.1).

## Skipped
None.

## Names
Proven (two sources each; LABELS.tsv):
- The two rows: both PDB name sets; `ret 0Ch` / `ret 10h`; GetDeltaMovement is a vtable entry (0x0129D560), InitialiseAnimationSequences
  is called once, from InitialiseAnimationInstance 0x00A50D1A.
- Callees: C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe&, float) const 0x00A4DEA0 (both PDBs; bound by landed 0x00A4E250),
  C3DAnimationBuilder ctor 0x00AB1660 / BuildMeshAnimation 0x00AB2520 (Ego_r / FableWin spellings), New<C3DAnimationSequence> 0x00A533F0 /
  New<C3DAnimationSequenceData> 0x00A53440 (landed binders), CSimpleMemoryPool::Allocate 0x00A5C910, C3DAnimationSequenceData::
  InitialiseAsConstant 0x00A4FD70 (Ego_r spelling QAEXABVCMatrix3x4@@KMAAVCSimpleMemoryPool@@@Z, `ret 10h`), ~CCharString 0x0099EAE0,
  ~CBaseClassNonCopyable 0x0099A300 (protected virtual MAE), malloc / free.
- Inline-member COMDATs identified in the TU (PDB name + body + TU run; LABELS.tsv): 0x00A4B820 SetMultiplyNoTranslation, 0x00A4B910
  PreScale(MMM), 0x00A4B970 SetTranslateOnly, 0x00A4BB40 GetType, 0x00A4BB50 C3DAnimation2::GetFrameCount, 0x00A4BB60
  C3DAnimation2::GetSamplesPerSecond, 0x00A4BC60 C3DAnimationSequence::SetParentIndex, 0x00A4BD30 C3DBone2::GetParent, 0x00A4BD40
  C3DBone2::GetOriginalNoChildren, 0x00A4BD50 C3DBone2::PeekLocalizationMatrix, 0x00A4BD60 C3DMesh2::GetBoneCount, 0x00A4BD70
  C3DMesh2::GetBone, 0x00A4BE10 C3DAnimation2::GetNonLoopingDuration, 0x00A4C140 C3DAnimationSequence::InitialiseShared, 0x00A4FE30
  C3DAnimationSequence::InitialiseAsConstant. Caveat: 0x00A4BB40 and 0x00A4BD40 are byte-identical (`mov eax,[ecx+8] ; ret`); they are
  told apart only by their place in the run (the animation getters vs the bone getters) -- confirm at landing.
Descriptive: the parameter names of both rows (the PDB name sets carry no parameter names); no descriptive function names.

## Header debt
- All overlay changes are opt-ins (hdr/README.md); the default paths are untouched. Copied from the acctB_r3001 hand-back (its
  C3DAnimation2.h / C3DAnimationSequence(Data).h / C3DKeyframe.h / CQuaternion.h / C4DVector.h / CMatrix3x4.h / C3DMesh2.h /
  C3DAnimationBuilder.h / N3DAnimationBuilder_COutputAnimation.h / C3DBone2.h) or from repo HEAD (C3DAnimationStats.h,
  CSimpleMemoryPool.h); merge with r3001 once.
- C3DAnimationStats: the generated header keeps MovementVector as `unsigned char[12]` and every member private; the opt-ins type it
  (C3DVector, needs FABLE_RETAIL_C3DVECTOR_PACKED) and make it protected for the parked row.
- C3DAnimationBuilder: the r3001 header has no base and a declared virtual dtor; retail derives it from CBaseClassNonCopyable with the
  compiler-generated dtor (opt-in FABLE_RETAIL_C3DANIMATIONBUILDER_BASE).
- The landed 0x00A4FD70 row (rebuild/src/compiled/00/a4/C3DAnimationSequenceData_InitialiseAsConstant_00a4fd70.cpp) uses a local
  (unsigned short, long) spelling that is not the PDB's; see NAME_FIXES.tsv.
