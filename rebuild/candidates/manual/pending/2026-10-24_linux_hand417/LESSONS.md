# Chunk 417 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A 13-dword prefix block copy is a trivial base class, not two members.** 0x00A31EC0, 0x00A32100, 0x00A32170 use
  C3DGeneratorObject `: C3DDummyObject`.
  - In an implicit copy or assignment, VC7.1 does not merge separate POD members into one `rep movsd`.
  - `CMatrix3x4 Matrix; long ID;` gives `rep movsd` x12 plus a dword move. A 0x34-byte trivial base gives x13.
  - A PDB layout that lists `_padding_` rows from +0 is a base-class hint.
- **Same-TU `__declspec(noinline)` bodies for the tag-taking callees** let VC7.1 home every `__false_type()`
  temporary on the live `atend` slot. Without them the first temporary gets its own frame slot (`sub esp,8` instead of
  `push ecx`). Example: STLport `vector::_M_insert_overflow` 0x00A33D90, the 0x00569E30 lever. Dropping the tag's user
  constructor is worse (zero stores).
- **CCountedPointer conversions in the chunk-file family are Fable's `SetStaticCast` + `ShareData`.** In
  `C3DMeshFile::GetRoot() const` 0x00A32620 (twin 0x00A537F0) the body is
  `CCountedPointer<const Root> r; r.SetStaticCast(CCountedPointer<const Super>(RootChunk)); return r;`.
  - The `Info != info` test against the empty local is why a null block yields `{0,0}` (`xor ebx,ebx ; xor edi,edi`).
  - The inc, inc, dec sequence is the converting temporary, ShareData and the temporary's release.
  - FableWin.pdb's per-type instantiation names (SetStaticCast/ShareData/ReleaseData) identify the shape.
- **One inline record `operator=` can be both inlined and called within one function.** The STLport pair loop inlines
  it, while the final `*last = val` calls the COMDAT (0x00A32960). The landed 0x00A333A0 model reproduces this for the
  whole 16-byte C3DMeshVolume sort family (0x00A33250, 0x00A34260 ... 0x00A35490), 9 rows first try.
- **`iter_swap` codegen.** For POD pairs (0x00A329B0 / 0x00A32A70) `__iter_swap`'s `T tmp = *a` is inline and homed
  in the pivot's dead argument slot. For the 16-byte pair it is `swap(*a, *b)` calling the out-of-line `swap` COMDAT
  0x00A331B0.
- **SGI `__partial_sort` inlines `__pop_heap` only for the POD 0x38 pair** (0x00A34360: frame local + `__adjust_heap`
  call). The 20-byte (0x00A342F0) and 16-byte (0x00A35260) instantiations call `__pop_heap` out of line. Declare or
  define it accordingly.
- **CVectorMap<K,V,C>** is a vector subclass in retail: the STLport vector at +0, Compare at +0xc and Dirty at +0xd.
  The PDB shows +0x10 and +0x11 because of the 16-byte Dinkumware vector. `Insert(x)` is `push_back(x); Dirty = true;`,
  with the overflow tag homed in the dead x slot (0x00A34950 / 0x00A349B0).
- **A counted-pointer release on an escaped temporary is not the folded `dec [esi] ; jne`.** For `map::operator[]`'s
  value_type, the member form re-reads Info after the `dec` because the pair's address went to `insert_unique`. A
  local copy gives `mov eax,..; mov esi,eax; dec [eax]; cmp [esi],0`. Retail loads esi once. This is unresolved: see
  the parked 0x00A2D810.
- **x87 operand order in an inlined CrossProduct** (parked 0x00A358F0): `a.X * b.Y` loads b.Y first in one of three
  products. Try writing that product reversed before parking it as a lane gap.
- **`resize(n)` default T()**: CMatrix3x4's default constructor is the chained SetIdentity (nine descending zero
  stores, then the 1.0f diagonal ZAxis.Z..XAxis.X), the landed 0x00B32100 model. It matched first try in 0x00A34DF0.

Review at landing:
- Two rows took more than one grade: 0x00A33D90 (3 grades) and the three C3DGeneratorObject rows (2 each). None needed
  more than 4.
- Local base models stand in for missing or mismatched headers. There are no engine headers for C3DGeneratorObject,
  C3DDummyObject, C3DMeshVolume, CVectorMap or C3DMeshFile. CMatrix3x4 is modelled as `float M[12]` in the copy rows
  and as XAxis..Position in the resize row; both are 0x30 bytes.
- 0x00A537F0 C3DAnimFile::GetRootChunk is a byte twin of the matched 0x00A32620 and is not landed. Reuse the 0x00A32620
  source with C3DAnimFile / C3DAnimFileRootChunk.
- The landed `std_swap_00a331b0` calls the element `pair<long, C3DMeshVolume>`. Every sort helper of this family
  compares the key unsigned (`jae`/`jb`), so the key is `unsigned long`. The swap body itself is unaffected.
- 0x00A34DF0 pins its leaf to the full mangled name `?resize@?$vector@VC3DGeneratorObject@@@_STL@@QAEXI@Z`, because the
  TU also emits the 2-argument `resize(n, x)` COMDAT.
