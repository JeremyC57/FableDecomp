Overlay for row 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim (B24C6, 2026-11-24). Full copies of the CURRENT repo headers plus
additive #ifdef sections (default branches unchanged); the B24C4 overlay is kept as ../hdr_old.
- engine/CAnimSet.h FABLE_RETAIL_CANIMSET_MESHANIM: GetMeshAnim (0x0070C900, decl), GetFirstMeshAnim (inline), private
  GetInterpolatedAlpha / GetInterpolatedInternalAlpha (inline), GetInterpolatedTime (0x0070C800, decl). Ego_r / FableWin names.
- engine/CAnimateThing.h FABLE_RETAIL_CANIMATETHING_PROGRESSFROMALPHA: GetProgressFromAlpha inline (0x0070BE40 is its COMDAT copy;
  retail calls it before the transition loop and expands it inside), body = the landed row's.
- engine/C3DAnimationTransitionState.h FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE_SETTERS: SetStartState / SetEndState inline
  (`StartState = state;` / `EndState = state;` = landed 0x00710560 / 0x007105A0 COMDAT copies; expanded in the loop at 0x0070DBD6 /
  0x0070DC0C), SetProgress inline (FableWin 0x01E95FF0).
- engine/C3DAnimationStateAlpha.h FABLE_RETAIL_C3DANIMATIONSTATEALPHA_INDEX_CTOR: ??0C3DAnimationStateAlpha@@QAE@KMM@Z (FableWin 0x01E99920).
- engine/C3DAnimationBlendState.h FABLE_RETAIL_C3DANIMATIONBLENDSTATE_ADDCOMBINATIONBLEND: AddCombinationBlend(const C3DAnimationStateAlpha&)
  (Ego_r 0x00B96B60; retail 0x00A3C310).
- engine/C3DAnimationInfo.h FABLE_RETAIL_C3DANIMATIONINFO_ISLOADED: IsLoaded() declaration (out of line 0x00A23F50).
## B24C7 additions (2026-11-24)
- engine/CAnimSet.h (FABLE_RETAIL_CANIMSET_MESHANIM section, own B24C6 addition): GetInterpolatedAlpha / GetInterpolatedInternalAlpha now
  lerp through engine/GFGetInterpolatedValue.h (Ego_r / FableWin ?GFGetInterpolatedValue@@YIMMMM@Z, by-value floats). Evidence: retail
  `fld last ; fld cur ; fsub st,st(1) ; fmul alpha ; fadd st,st(1) ; fstp st(0)` at every site (0x0070DA92, 0x0070E01B, ...) = a
  by-value lerp (VC7.1 probe c7/gp/o1.cpp); FableWin GetInterpolatedAlpha 19i/45b = VC7.1 /Od 16i/36b of this text + FableWin's
  3-instruction float pushes (x3). Includes engine/GFGetInterpolatedValue.h under the section.
- engine/CIVCountedPointer.h (copy of the current repo header) + FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT_ASSIGN (nested in CONVERT):
  FableWin.pdb ??$?4VC3DAnimationTransitionState@@@?$CIVCountedPointer@VC3DAnimationStateBase@@@@QAEAAV0@ABV?$CIVCountedPointer@
  VC3DAnimationTransitionState@@@@@Z (0x01EA5040, 35i/100b; this text 33i/93b); retail 0x0070DCFA..0x0070DD0D (one `inc [eax+4]` and
  the store, no converted temporary). Forced under ASSIGN_FORCEINLINE like the same-type operator= (retail expands it; site score 144).
- stlport/fable_algorithm.h (copy) + FABLE_RETAIL_STLP_STABLE_SORT_AUX_DECL: STLport stable_sort forwarding to the __fastcall
  __stable_sort_aux (landed 0x00711B80; same mangled name as self_symbols.tsv) -- was row-local.
- engine/GFWorldDistance.h (copy) + FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER_DECL: declaration of the out-of-line
  ?GFWorld3DDistanceSmallerThan@@YI_NABVC3DVector@@0M@Z (0x006614F0, bound) -- was row-local.
- engine/CAnimSetItPriorityLessThan.h (NEW, no repo header) + FABLE_RETAIL_CANIMSETITPRIORITYLESSTHAN: Ego_r ??RCAnimSetItPriorityLessThan@@..
  class; 4-byte functor carrying alpha (retail pushes `alpha` as the comparator, 0x0070DF9F / 0x0070E16A); Alpha / ctor descriptive.
  (stable_sort is spelled over pointer iterators, `_Tp*`, not STLport's `_RandomAccessIter`: the value type is the pointee; marked
  FABLE_FASTCALL as retail's out-of-line copy 0x00711C90. Inlined in this row, no bytes change.)
## B24C8 additions (2026-11-24)
- engine/CAnimateThing.h + FABLE_RETAIL_CANIMATETHING_PEEKANIMINFO: forward declarations of CMeshDataBank / C3DAnimationInfo (top) and
  public `const CMeshDataBank& GetMeshBank()const;` / `const C3DAnimationInfo& PeekAnimInfo(long anim)const;` (access restored to private
  after). Ego_r.pdb release animate_thing.obj #25 (12 bytes) / #27 (16 bytes), FableWin.pdb 0x01E92060 / 0x01E97850:
  ?GetMeshBank@CAnimateThing@@QBEABVCMeshDataBank@@XZ, ?PeekAnimInfo@CAnimateThing@@QBEABVC3DAnimationInfo@@J@Z. GetMeshBank's retail
  out-of-line copy is 0x0070C0D0 (landed as GetMeshBank_2: NAME_FIXES.tsv).
- engine/CAnimateThing_PeekAnimInfo.h (NEW): the two inline bodies (needs the PEEKANIMINFO, CWORLD_MESH_BANK, CMAINGAMECOMPONENT_GETWORLD,
  CMESHDATABANK_PEEKANIMATION opt-ins; #error otherwise). GetMeshBank = `return *CMainGameComponent::Get()->GetWorld().GetMeshBank();`
  (the 12-byte retail body), PeekAnimInfo = `return *GetMeshBank().PeekAnimation(anim);` (FableWin 13i/32b = /Od of this text).
- engine/CAnimSetItPriorityLessThan.h REWRITTEN (B24C8): the class is EMPTY (Ego_r struct_layouts size 1, no members) and derives from
  std::binary_function<list<CAnimSet>::const_iterator, list<CAnimSet>::const_iterator, bool> (Ego_r lists that struct); operator() declared
  (FableWin 0x01EAB0B0 ??RCAnimSetItPriorityLessThan@@QBE_NABV..0@Z; body compares the private CAnimSet::Priority, expanded in the landed
  0x00711B80 chain). The B24C7 text (a float Alpha member, descriptive ctor) is superseded: it is kept as ../c8/CAnimSetItPriorityLessThan.h.b24c7.
- engine/C3DAnimationStateAlpha.h (FABLE_RETAIL_C3DANIMATIONSTATEALPHA_INDEX_CTOR, own B24C6 section): the constructor body calls the new
  inline SetAlphaWeight (FableWin.pdb ?SetAlphaWeight@C3DAnimationStateAlpha@@QAEXM@Z 0x01E99AA0); FableWin ctor 31i/104b v this text 29i/98b
  (+2 three-instruction float pushes in FableWin), the init-list form 19i/50b.
