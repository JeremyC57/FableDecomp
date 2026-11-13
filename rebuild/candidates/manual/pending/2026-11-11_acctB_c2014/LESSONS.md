# Chunk 2014 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A50510) The unreferenced COMDAT run a TU keeps for its inline members (here 0x00A4B820..0x00A4BE70: SetMultiplyNoTranslation,
  PreScale(MMM), SetTranslateOnly, GetType, GetFrameCount, GetSamplesPerSecond, SetParentIndex, C3DBone2::GetParent /
  GetOriginalNoChildren / PeekLocalizationMatrix, C3DMesh2::GetBoneCount / GetBone, GetNonLoopingDuration, InitialiseShared 0x00A4C140)
  lists the accessors the TU's rows really call; using them (instead of direct members) and defining the TU's ordinary members in the
  row (C3DAnimationSequence::InitialiseAsConstant, out-of-line 0x00A4FE30) is part of the match. The run is in header-definition order,
  not use order. Search Fable.exe for a one-line getter body (`8b 41 54 c3` ...) to find such a run.
- (0x00A50510) A bool-mask copy written as a value ternary makes one store; retail's two stores are an if / else, and the ternary also
  changed the loops' static frequencies (retail's loop heads carry no alignment padding; with the ternary ours did): 252 -> 154 lines.
- (0x00A50510) VC7.1 converts a generic `for (; n > 0; --n, ++p) *p = x;` byte fill into `test n ; jbe ; rep stosd ; rep stosb`: that
  guarded memset shape is STLport vector<bool>(n, false) (Fable's STLport keeps vector<bool> a byte vector) -- vector<char> uses the
  unguarded memset overload. std::vector<bool> vs CArray<bool> is code-identical but moves c2's x87 operand order (one more inline level).
- (0x00A50510) x87 commutative-sum order in a matched shape is a c2 per-function order, but it is MEASURABLE FOR FREE: in c2prio's JSON
  (`--json`) the x87 `local` candidates' `ref` events give the write tuple (maps a struct field: tuples are numbered last-to-first in a
  block, so IL order = descending write tuple) and the read tuple of each operand in each sum; the emitted term order is the read order.
  tools/e41fit.py (SetInverse translation row) and tools/pmfit.py (PostMultiply copy-field read pattern) scored ~70 natural spellings in
  ~20 s each without a grade; the two winners graded RELOCATION_MATCH. Spellings that moved it: std::vector vs CArray, `++data_index;` as
  its own statement, a named loop-1 bone pointer, `C3DVector(local.E41, local.E42, local.E43)` vs GetTranslation(), a named
  `long original_index` in loop 1. Spellings that did not: SetInverse via GetDeterminant / SetInverseNoTranslate, SetMultiply written in
  any term order or through SetMultiplyNoTranslation, Peek vs direct member, hoisting the local matrix, the PDB union CMatrix3x4.
- (0x00A50510) Written term order of a plain `a*b + c*d + e*f` never matters in VC7.1 (only parentheses fix an association); dead
  never-read locals at entry do move the order (diagnostic only).
- (0x00A50510) The struct field at offset 0 of a local matrix gets its c2 candidate id with the symbol itself (much earlier than the other
  fields: E11 id 43, E21.. 51..), which is why E11 terms are the odd ones out in the reassociated sums.
- (0x00A4E300) A retail delta = start^T * end with the translation row cleared then rewritten is SetMultiplyNoTranslation (the TU's
  COMDAT 0x00A4B820 stores E41..E43 = 0 last) + SetTranslateOnly; the zero stores survive (stores through the reference are not killed)
  and appear early in the first row's scheduling region (0x00A4E71A).
- (0x00A4E300) C3DKeyframe::ToMatrix and SetMultiplyNoTranslation are plain inline in the PDBs (COMDATs exist) but cost 267 / 217 in the
  inline trace: retail expands them, so the header bodies are FABLE_FORCEINLINE (a forced inline still emits its COMDAT when referenced,
  probe/frag5/c1.cpp).
- (0x00A4E300) A member read in place where the class has only a by-value getter (`fld st(0) ; fmul [edi+12h]`, no copy) means the
  derived class used the member directly (protected): the by-value C3DVector getter always materialises its copy first (GetMovementAtTime
  0x00A4E1A7, and every fragment form tried).
- (0x00A4E300) VC7.1 reassociates the product sums by c2 order; a fragment (two keyframes from an opaque call + the transpose + the
  product, probe/frag) reproduced the full row's sum orders exactly and is a free lab. Of ~200 transpose spellings only "lower triangle
  into three temps, then E21 = E12 ..." gives retail's row 1; none gives rows 2 / 3.
- (tooling) tools/x87sym.py (symbolic x87 walk of retail or our object: stores with full expressions, call arguments, keyframe regions),
  tools/xcmp.py (per-store term-order comparison) and tools/terms.py (quaternion-matrix element naming) turn an x87 diff into
  "row 2: retail b11*a12 + b31*a32 + b21*a22, ours ..." in seconds.

Review at landing:
- 0x00A50510's object also emits out-of-line C3DAnimationSequence::InitialiseAsConstant (retail 0x00A4FE30, not landed) and the inline
  COMDATs (InitialiseShared 0x00A4C140, the C3DBone2 / C3DMesh2 accessors, AllocateType<bool> / <C3DVector>, STLport vector<bool>
  helpers): check the link for duplicate / mismatched COMDAT selection.
- 0x00A4BB40 (GetType) and 0x00A4BD40 (C3DBone2::GetOriginalNoChildren) are byte-identical `mov eax,[ecx+8] ; ret`: the labels rest on
  their place in the COMDAT run.
- NAME_FIXES.tsv: three landed spellings around 0x00A4DEA0 / 0x00A4C1F0 / 0x00A4FD70 disagree with the PDB name sets.
- hdr/: merge with the acctB_r3001 overlay (same base files) and with repo HEAD's C3DAnimationStats.h / CSimpleMemoryPool.h.
- 0x00A4E300 (parked) depends on FABLE_RETAIL_C3DANIMATIONSTATS_PROTECTED (MovementVector protected: unproven access, evidence above).
