# agent903 -- near-miss retry pass 3 (chunks 422-466 parks)

## Recipes that matched
- map::operator[] late pair store (0x00AD6420, 0x00ADBFF0): the landed 0x009B7BC0 recipe ports directly: map -> _Rb_tree
  layering, `_M_lower_bound` returning the node, `value_type(k, _Tp())`, and a same-TU `auto_inline(off)` +
  `__declspec(noinline)` `insert_unique(hint, v)` stand-in that READS v (one byte of first/second) but never keeps its
  address. First grade each.
- 0x00AD64C0 (pair holding a CCountedPointer): the read-only stand-in lets VC7.1 fold the pair temporary's release away
  (it knows Info is still 0): 131 v 158. The stand-in must also WRITE through v (`const_cast<_Value&>(v)` store to the
  count word) so the caller re-reads Info after the call; then the late Data store and the folded `dec [esi] ; jne`
  release both match retail. Writing through a pointer loaded from v is not enough (still folded).
- 0x00A85930 `__destroy_aux<CNavigationLayer*>`: two empty-tag temporaries packed into ONE frame byte as retail once
  both out-of-line `__destroy_aux` instances (0x00A83420, 0x00A827D0) get same-TU `auto_inline(off)` +
  `__declspec(noinline)` explicit-specialisation bodies that never read the tag. First grade.
- 0x00B806E0 CalculateFadeOut (x87): (1) `SimFadeBegin * Zoom * SimRadius` in one expression (or through inline
  GetSimRadius/GetSimFadeBeginDistance accessors) is reassociated by VC7.1 to SimFadeBegin*SimRadius*Zoom, and
  `Zoom * SimRadius` alone comes out SimRadius-first; split statements `b = FadeBegin * Zoom; b *= SimRadius;` plus a
  named in-branch `radius = Zoom * SimRadius` give retail's order. (2) The distance store after the three x87 pops
  (`fstp st(3); fstp; fstp; fst [esp]`) came with `<math.h>` + a C3DVector with a user default ctor (landed
  0x00BAA4E0 model) instead of a hand-declared `sqrt` and no default ctor. 4 grades.

## Parked
- 0x00AD8AF0 CVertexGroupBlock::SharesGroupWith: see STATUS.tsv; best source in differ/.
- 0x00B96350 CShadowClipVolume::ClipSphere (x87 dot order): GOTCHAS' "dot Z,X,Y = lane gap" is a spelling: VC7.1
  reassociates EVERY single-expression sum of products (X+Y+Z, X+Z+Y, Z+(X+Y) all give Y, X, Z here); write the Dot
  as two statements `dot = Z*v.Z + X*v.X; dot += Y*v.Y;` and the order (and the induction pointer at the plane's Y)
  match. MATCH on grade 3. Same family as 0x00B806E0's split product.
- 0x00C22080 OnAllocationFailed (shared zero exit at the function end): every early-return / `||` / positive-`if`
  spelling put the `xor eax,eax` exit right after the last failure test. Nulling the rejected candidate
  (`if (buffer && buffer->Covers(key)) buffer = 0;`) and ONE `if (buffer) { ...; return 1; } return 0;` lets VC7.1
  thread both failure paths to a trailing zero exit after the success epilogue (retail). Grade 2.
- Chunk 431 pool walk 0x00C12300: the landed 0x00C123B0 recipe (inline GetNumItems()/GetItem(i) owner accessors over
  the pool's out-of-line Get) fixes the edi-index/ebx-item colouring; only the +0x370 copy register (ebp vs eax) is
  left -> differ/.
- 0x00C35990 AudioObject_SetSource: the parked next idea (checked local assigned in both if/else arms) matched first grade.
- 0x00C75F80 CReverbTap::Allocate (parked, 5 grades): the stores in the caller + inline ring Clear() fix the parked
  cursor/count CSE; only the m_pos store / m_data re-read vs memset-count order is left (differ/).
- Same-TU stand-in device closed three tag-slot rows on the FIRST grade: 0x00CB8170 (counted `__copy` 0x00CBB120, plus the
  STLport 4.6 erase `__copy_ptrs(last, finish, first, _TrivialAss())`), 0x009C2900 (`__uninitialized_copy` 0x0040DDC0),
  0x00476F9B (`__destroy_aux` 0x00450B1B). Rule: an empty tag temp homed in a LIVE argument/shared slot = give the
  tag-taking out-of-line callee a noinline body that never reads the tag.
- 0x00F36F40 FreeResourceEntry: the `cmp eax,ecx` (finish first) is STLport 4.6 __copy_trivial's own `(__last == __first) ?`
  test; a __copy_trivial without it leaves erase's `cmp ecx,eax`. First grade.
- 0x00B6CC90 / 0x00AB44D0 one-arg resize(n): the interleaved zero store + erase start in edx = a POD element with NO ctor
  (value-initialised `_Tp()`, landed 0x0066E6D0); the parked pair<long,long> user ctor gave both stores late + eax.
  Elements from the callees' other call sites: 0x00B6C840 at 0x00B68EDA (renderer +0x5E4 SortedSubPatchList ->
  CSubPatchSorterEntry), 0x00AB4260 at 0x00AB3927 (GenerateD3DDeclaration's +0x10 D3DDeclaration -> _D3DVERTEXELEMENT9).
- 0x00BC9B80 FindID: direct `(node + 1)` casts (helpers are left as calls by inline_depth(1)) + a named `nodeKey` local
  for the leaf words (retail's key-pointer walk with a fixed leaf-minus-key difference). Grade 2.
- 0x00B9D670 SetupFirstTextures: the parked next idea (the 0x00BA7DB0 __forceinline SetTexture(long, const CTexture*)
  overload, -1 tests outside) matched first grade.
- 0x00BEC8D0 InitialiseHiddenMaterials: same-TU read-only `_M_find` stand-in -> count in ebp, &set in a frame word. Grade 1.
  NOTE: $S/batch/chunk432_oracle.tsv is missing, so $S/tools/q.py says "no oracle" for chunk-432 rows; I grade with
  $D/bin/q903.py (q.py + $S/w22/chunk903.tsv after the catalog oracle). finish.py reads chunk903.tsv, unaffected.
- 0x00CB36E0 SetImeApi (/GS): the four ImeUi.cpp globals as file STATICS defined in the TU -> g_hklCurrent loaded into ecx
  between the cookie load and store (extern: after the store, eax). Grade 2.
- 0x00CDD0A0 TransferableVector<bool>::Resize parked (4 grades, byte 3 vs byte 0 of the push-ecx word).
- 0x00A834B0 __linear_insert<pair<long, CCountedPointer>>: the pair's IMPLICIT operator= (STLport pair declares none,
  CCountedPointer has an inline operator=) -> VC7.1 emits and calls the COMDAT (retail 0x00A7FAF0) knowing its body;
  the user noinline operator= hoisted the count-block load. Rewritten as _STL templates. Grade 4 (callee stand-ins
  for __copy_backward/__unguarded_linear_insert changed nothing; an empty user ~pair is needed if a by-value stand-in is
  defined under auto_inline(off), else the implicit dtor goes out of line).
- 0x00A83770 CVectorMap<u64,bool>::Insert: same-TU `_M_insert_overflow` stand-in (reads x). Grade 1.
- 0x00BEF2B0 __linear_insert<pair<pair<long,long>, CEngineTextureDecal>> (/Gr): verbatim STLport body plus the vector
  copy ctor / operator= declared in-class with noinline out-of-class bodies under auto_inline(off) (operator= does
  dealloc / allocate / __uninitialized_copy); the comparator's (key, entry) overloads removed. Grade 4.
- 0x00C28B80 CHandleBasedMemoryPool::InternalAllocate (C27xxx build): retail's `lea eax,[edx+eax*4] ; mov [eax],edi`
  slot store = the area's handle word written THROUGH the slot (`slot.Header = area; slot.Header->Handle = *handle;`):
  the slot address is a CSE of the store and the (forwarded) re-read so VC7.1 materialises it. Grade 4 (an inline slot
  setter, a union value copy and QFE all fold).
- 0x00AF87C0 CDynamicObjectHeap<156,2100> ctor: the same lever in the six-way unrolled push-front loop -- index the
  element twice (`Blocks[i].NextEntry = FreeList; FreeList = &Blocks[i];`) instead of an `entry` pointer local, so every
  entry address is formed first (`lea ecx,[eax-9Ch] ; mov [ecx],edx`). Grade 1.
- 0x0121AC30 / 0x0121ACD0 CRightHandedSet initialisers (inventory sets 1 and 5): `sqrt(0.5f)` -- the <math.h> FLOAT
  overload (inline `(float)sqrt((double)x)`) -- keeps one fsqrt but negates per inline expansion, so the negative words
  CSE only per position (two `fchs`, retail); `-sqrt(0.5)` CSEs all into one. Grades 2 / 1. Named like the landed
  siblings: `_global::$E_InventoryOrientationRHSet<n>`.
- 0x00A79240 CNavQuadTree::SetSwitchableNodesAsBlocked: the parked next idea (same-TU noinline LowerBound/UpperBound
  read-only bodies under auto_inline(off)) matched first grade: the by-value 64-bit ID stays in ebx:ebp.
- 0x00C34440 CVoiceParams::GetGain: ONE `return value;` with the positioned branch overwriting `value` keeps the
  parameter-block result in st0 (`fst [esp+8]`, `fld 0 ; fld st(1) ; fucompp`, `fstp st0` on the long path, shared
  epilogue); the early `return value` spills it. Tail sequenced: `falloff = 1 - ...; scale = attenuation * value;
  value = scale * falloff;` (the one-expression product is reassociated into memory fmuls). Grade 3.
- 0x00CBE693 GetRandomPositionInsideRadius: the polar helper is Ego_r.pdb `C3DVector::InitialiseFromAngles(float angleXY,
  float angleYZ, float length)` (?InitialiseFromAngles@C3DVector@@QAEABV1@MMM@Z) as a __forceinline member:
  `float cosYZ = (float)cos(angleYZ * (double)TWO_PI); X = (float)(sin(angleXY * (double)TWO_PI) * (cosYZ * length));`
  (Y with cos, Z = sin(angleYZ*2pi) * length): cos homed in [ebp-4], cos*length one CSE kept on the stack. Grade 3. The
  same body should close 0x00CBE5F0 (its budget was spent before the grouping was found; its best, 113v114, is the
  ungrouped `* cosYZ * length`, in differ/). Other copies of the helper: 0x006CBF9D, 0x00723E7E, 0x0074E1E3.
- Parked this segment: 0x0122A550 (atexit-thunk address formation, 3 grades), 0x00C0B7B0 (x87 dying-operand copy;
  statement-sequenced sums give retail's rate block, differ/), 0x00C35DD0 (dead re-test, volatile cap 1 grade),
  0x00C36B90 (`(1.0f / size)` CSE temp fixes the operand order -> 219v219, one scheduling swap, differ/), 0x00C425E0
  (named bytesPerBlock local fixes the colouring, the Convert `this` hoist is left, differ/), 0x00BF32B0 (FirstKey as
  `return NextKey(BaseNode)` gives the merged xor exit, 194v196, differ/), 0x00C73830 (failure-block layout),
  0x00CBE5F0 (see above).
