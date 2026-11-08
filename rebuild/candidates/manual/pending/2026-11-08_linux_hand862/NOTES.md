# chunk 862 notes

## Matched (7/9, all RELOCATION_MATCH on the first grade, tests PASS)
- 0x0121AC70 / 0x0121ACA0 / 0x0121ADC0 / 0x0121ADF0: the `_$E1` dynamic initializers of four anonymous-namespace
  `const CRightHandedSet ..._FORTY_FIVE_DEGREES_ORIENTATION(C3DVector(+-sqrt(0.5f), -sqrt(0.5f), 0), C3DVector(0, 0, -+1))` objects
  (0x0137CF8C / 0x0137CFA4 / 0x0137D01C / 0x0137D034). Spelling copied from the landed sibling 0x0121AD80: `#pragma intrinsic(sqrt)`,
  FABLE_RETAIL_CRIGHTHANDEDSET(+_CTOR_INLINE), FABLE_RETAIL_C3DVECTOR. Object names proven by reloc_bindings.tsv through the landed
  GetRHSetFromOrientation 0x005D8EF0 and the retail .data image; the function names follow the 858-style index (2/3/10/11).
- 0x0121F590 / 0x0121F5C0: `_$E1` of hit_pos / hit_dir (CEnginePrimitiveHandle, 0x013BAF44 / 0x013BAF4C); the landed `_$E2` TU text
  with the `_$E1` leaf. Both PDBs name them "dynamic initializer for 'hit_pos'/'hit_dir'".
- 0x01224CE0: `_$E1` of `CCharString NScript::CQ_SunnyvaleMasterData::RelationSuffixes[3]` (0x0143E934) via the
  FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS opt-in. The array name is DESCRIPTIVE (chunk 780 spelling, in no PDB).

## Parked 0x00531EC0 NUISystem::CComponent::Update(float) -- DIFFER(3455v3642), md 407 (structural 193)
- Shape: clock + UpdateZoom/Position/Colour; PositionChildren set loop; Children index loop; ChildrenToDelete index loop. Each loop
  pushes RenderZoom / RelativeRenderZoom and a parent / relative-parent position computed by IsPositionIndependent x IsZoomIndependent
  x IsUsingRelativePosition (eight arms, local-copy spelling `C2DVector position = Position; position.X *= ...; position += ...`).
- Key spellings: `ParentPosition + Position`, `Position + RelativeParentPosition`, `ConvertVector(Position) + RelativeParentPosition`
  (VC7.1 member operator+ loads the reference operand first, so retail's left-first order needs the swapped text: 597 -> 414);
  `this == C->PParent` for `cmp esi,[ecx+0C8h]`; deletion tests in the order 0, ALL_INSTANCES, THIS_INSTANCE, PER_PARENT as an
  if-chain (a switch becomes a dec chain, 550+); ALL_INSTANCES shares the PER_PARENT RemoveChild tail (retail `je 0x00532745`), which
  only a goto (407) or a `bool remove` flag (415) reproduces; do-not-delete CDeletion as a named local with `Method = UI_DO_NOT_DELETE`
  (the default ctor 0x0042BED4 leaves Method alone).
- Residue (all inliner budget, VC7.1 RTM): (1) retail calls ConvertVector at the 8 sites of loops 1-2 and expands it at the 4 of
  loop 3; with ConvertVector declared only, we call all 12; with any 0x0052E580-exact inline body (landed spelling, ConvertX/ConvertY
  helpers, copies; 15 bodies) we call loop 1 and expand loops 2-3. Bodies built through a `C2DVector(x, y)` temporary give retail's
  exact split but compile to 101-110 bytes out of line (retail 80). /d2inlT52 (diagnostic) gives the split. (2) retail calls
  ~_List_base 0x0042ABCA for the THIS_INSTANCE local and expands it only for `deletion` at block end; we expand it there too, so the
  local's list head takes ebx (c2prio: id 147, prio 548) and the deletion head spills.
- Callees: RemoveChild is own slot 64 (+0x100), Get/SetDeletion slots 63/62, ChangedState 49, GetCurrentStateIndex 86, ChangeState
  48, IsBastardChild 100, SetParentColour 22 (header names from Ego_r.pdb); CDeletion copy ctor 0x0042CD84 (out of line at the
  PER_PARENT site only), list insert 0x0042B4C3 (`_M_insert_dispatch`), __find 0x00534EB0 and __copy 0x00535000 (__fastcall, /Gr).
- Labels: 0x0042BED4 is CDeletion::CDeletion() (labels say CAction::CAction); 0x00534EB0 / 0x00535000 are the STLport __find /
  __copy instantiations (LABELS.tsv).

## Parked 0x00DAADD0 NScript::CQ_NewOakValeIntroScript::Init() -- DIFFER(3406v3306), md 350
- Shape: 26 member resets (TalkIntermittentTimer through the inline CTimer::operator= -> pDSTGame slot 89 SetTimer), the 5-slot
  WhichBadDeedsPerformed loop (dword + byte stores), then 42 push_backs of TEXT_QST_048 literals in groups FemaleBad 6, MaleBad 6,
  FemaleBoth 4, MaleBoth 4, FemaleGood 6, MaleGood 6, FemaleNeither 5, MaleNeither 5. TU is /Gr.
- Residue: retail expands std::_Construct<CCharString,CCharString> at the first 22 push_backs (through the third FemaleGood) and calls
  the out-of-line __fastcall copy 0x0044B110 (its only caller) at the last 20; we expand all 42. First 609 instructions identical.
- Nothing moved the cut: fillers before/after Init, 126 sites, if() round late sites, 150 inline CTimer op=, named / explicit /
  reference spellings, insert(end()), memset, __forceinline CTimer, _Construct / push_back cost probes, /Os /O1 /Ob1 /Ot /EHsc /GX and
  every /d2inl knob (all-or-nothing). Owner question: is a per-TU inline knob acceptable? Otherwise retail's TU has something we do not
  see that spends the budget.

## Header debt / ideas
- A 0x0052E580-exact ConvertVector body with the inline cost of `result = C2DVector(v.X / ScreenWidth, ...)` would likely close
  residue (1) of 0x00531EC0. A same-TU ConvertVector body is legitimate: 0x0052E580 sits inside the CComponent TU.
- FableWin.pdb has ??4CDeletion@NUISystem@@QAEAAV01@ABV01@@Z (operator=) and ??1CDeletion; engine/NUISystem_CDeletion.h has neither
  (implicit ones behave the same here).
