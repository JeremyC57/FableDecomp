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
