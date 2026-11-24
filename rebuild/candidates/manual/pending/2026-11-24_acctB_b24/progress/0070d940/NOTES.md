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
