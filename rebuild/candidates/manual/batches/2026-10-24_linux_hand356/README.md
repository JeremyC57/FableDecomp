# Hand chunk 356 (_Insertion_sort1<CDegradableInfo*> .. _Move<CLevelLoader::CObject::CLevelFiles*>): 36 of 40 byte-exact

All 40 rows are SGI/STLport algorithm instantiations from `/Gr` TUs (`__fastcall`: first in ecx, second in edx, the rest
on the stack). The catalog's Dinkumware names (`_Insertion_sort1`, `_Med3`, `_Move`, `_Buynode`, `_Make_heap`, ...) are
propagated labels: every body here is the STLport form. 6 MATCH + 30 RELOCATION_MATCH; all 36 tests PASS under g++ and
VC7.1.

## What matched (36)

- **STLport `__insertion_sort(first, last, comp)` with an out-of-line `__linear_insert`** (00ad5b50, 00b85b30, 00a82870,
  00a82820, 00cbbe50, 005e2e00, 0076bb70, 00470dbc, 00a71810, 009e7590, 00af3700, 00cb6ea0, 00809590): `if (first ==
  last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);` with `ret 4` (`ret 2Ch` for
  00809590's 44-byte by-value targeting comparator). The by-value element copy decides the shape:
  - an explicit memberwise pair copy constructor builds it in the argument area (`sub esp,N ; mov eax,esp`);
  - a POD is pushed dword by dword;
  - a flat record (00cbbe50) or a nested pair `second` (00af3700, 00cb6ea0, 0076bb70, 00470dbc, 009e7590) decides
    whether VC7.1 adds a second strength-reduced induction pointer.

  00470dbc sits in a frame-pointer TU (`#pragma optimize("y", off)` + `("s", on)`). 00ad5b50's comparator is a
  function pointer.
- **STLport `__insertion_sort` with `__linear_insert` inlined:**
  - **004c5190 and 00adbe70** match only when the real `__copy_backward` body is in the TU under
    `#pragma auto_inline(off)` + `__declspec(noinline)` (GOTCHAS 24l). Then the empty iterator-tag temporary is homed
    over the live comparator slot, as in retail.
  - **0067ee30** (CDegradableInfo, health descending) needs `__copy_backward`, `copy_backward` and `__linear_insert` all
    `__forceinline`. Otherwise they stay out of line as cdecl calls.
  - **00711190** (list<CAnimSet> iterators by Priority at node+0x54, i.e. CAnimSet+0x4C retail) needs all four helpers
    `__forceinline`.
  - **009d3b90** (bank entry indices by CBankFile::CompareDataOffset) uses STLport's trivial copy_backward, which counts
    in BYTES: `n = (char*)last - (char*)first; n > 0 ? memmove((char*)result - n, first, n) : result`. An element count
    adds `sar/shl`.
- **SGI front ends:**
  - 00808a30 is `__unguarded_insertion_sort` → `__unguarded_insertion_sort_aux(first, last, (T*)0, comp)` @ 0x808120.
  - 00809840 is `pop_heap` → `__pop_heap_aux` @ 0x808DB0.

  The leaf must name the front end, because the `_aux` callee is only declared.
- **SGI `__unguarded_insertion_sort_aux` (00817540, THitLocationIntersectionElement):** `for (i = first; i != last; ++i)
  __unguarded_linear_insert(i, T(*i), comp)`, with the linear insert inlined. C3DVector has a user copy constructor, so
  the element's implicit copy is memberwise (eight dword moves into the frame home) while assignment stays `rep movsd`.
  Matched first try.
- **SGI `__unguarded_linear_insert<CWideString>` (00496fd0):** the landed 0x00497160 models (accessor 0x99AF10,
  `_wcsicmp`, `operator=`, destructor). The by-value string is destroyed by the callee.
- **SGI `__make_heap(first, last, comp, T*, Distance*)`** (00817950, 00a32c20, 00720710; `ret 0Ch`): `__adjust_heap(first,
  parent, len, T(*(first + parent)), comp)` with the value built memberwise straight into the argument area from a
  strength-reduced `first + parent*size + k` pointer.
  - 00720710 inlines CCompositeBlendDesc's IMPLICIT copy constructor, with CRGBColour's four-channel `Set`. The landed
    0x00720610 pop_heap of the same TU calls the out-of-line 0x005BA48A instead.
- **SGI `merge(first1, last1, first2, last2, result, comp)`** (00b85510 CSortElement, 00710310 CAnimSet iterators; `ret
  10h`): `return copy(first2, last2, copy(first1, last1, result))`, with STLport's counted `__copy(first, last, result,
  random_access_iterator_tag, Distance*)` inlined. Plain `inline` suffices; both matched first try.
- **SGI `__median(a, b, c, comp)`** (`ret 8`, returns the reference):
  - **00805fe0** (CDoNotUseBinaryPredicate) needs the coin written as `if (seed & 1) return false; return true;`. The
    forms `(x & 1) == 0`, `!(x & 1)` and `?:` all compile to `not al ; test al,1`. The same form also grades 0x00806910
    (not a chunk row) RELOCATION_MATCH: probe/uli_B.cpp.
  - **00a70020** and **004dfc42** (pair<CCharString,CCharString> by first) need `/Oa`: retail keeps the three
    string-data pointers in registers across the data-compare calls. 004dfc42 is the frame-pointer, size-optimised TU
    (`y` off, `s` on); under "s" the comparator and CCharString::operator< must be `__forceinline`.
- **STLport pair `operator<` (00a40250, cdecl, out of line):** `/Oa` too. The leaf is `??$?M` because the mangled name
  does not contain "operator<", and the TU also emits `??MCCharString`.
- **SGI `__copy_backward` (0086d5a0):** 16-byte CVectorMap records (vector op= 0x006741E0, empty comparator byte +0xC,
  Dirty +0xD), the landed 0x0086D5E0 model.
- **STLport `__uninitialized_copy(first, last, result, const __false_type&)`** (00513500, 006c2aa0 byte twins):
  CLevelFiles {LevelID; 3 × CCountedPointer<CAFile>} under placement new. VC7.1 strength-reduces both +0x10 induction
  pointers on its own.
- **STLport `list<CCountedPointer<CSpawnedFuncBase> >::insert(iterator pos, const T& x)`** (005355c0, 00aec6f0 byte
  twins; `ret 0Ch`): malloc node, placement copy of the counted pointer (RefCount++), the four link stores, and the
  iterator returned through the hidden pointer (it has user constructors).

## Parked for Fable 5.1 (4)

- 0086fac0 _STL::__insertion_sort DIFFER(122v122): `sub esp,0Ch ; lea eax,[esp+17h]` (tag in its own frame dword) vs retail `sub esp,8 ; lea eax,[esp+20h]` (tag homed over the live comparator slot) | tried: extern-only __copy_backward, __forceinline helpers, by-value tag (fixes the frame, pushes a register), iterator_category()/__false_type chain, ctor-less tag hierarchy, QFE 4035 | next: define the real __copy_backward body (0x0086EFF0) in the TU under `#pragma auto_inline(off)` + `__declspec(noinline)` -- the GOTCHAS 24l fix that matched siblings 004c5190 and 00adbe70 first try (found after this row's 4 grades)
- 004c5100 _STL::__insertion_sort DIFFER(135v135): `sub esp,0Ch ; lea edx,[esp+17h]` vs retail `sub esp,8 ; lea edx,[esp+20h]` (same tag-homing difference) | tried: as 0086fac0 | next: real out-of-line __copy_backward body (0x004C2070) under auto_inline(off)/noinline, exactly as matched 004c5190 (CSortPrimaryMeshesLast twin)
- 007212d0 _STL::__insertion_sort DIFFER(136v141): register assignment (retail first in edi / val.m_value0 in ebp / key in ebx, ours ebp / ebx / edi), retail's else arm re-reads `[esi]`/`[esi+4]`, retail aligns the loop head with `jmp ; lea ebx,[ebx]` | tried: POD record (block copy), pair<long,{short,short}> (no value home), flat record with a float third member + memberwise copy ctor + real __copy_backward body (value home and tag now match) | next: SGI `__linear_insert(first, last, T*, comp)` with `T val = *last` inside, or pair<long, {short-key word, float}>
- 006b5890 _STL::__lower_bound DIFFER(76v76): retail `fld [val+4]` hoisted above the loop + `fcom [middle+4] ; test ah,41h` + `fstp st(0)` at exit vs ours `fld [middle+4] ; fcomp [ebx+4] ; test ah,5 ; jp` per iteration | tried: STLport lower_bound + pair operator< (const refs), comparator by value, operator< by value (spills), reversed float compare `y.second > x.second` (closest: `fld [ebx+4] ; fcomp [middle+4]`, still not hoisted), QFE 4035 | next: lane gap: x87 loop-invariant hoisting of a conditional float load (look for a landed float-keyed __lower_bound to compare)

## Skipped

None. All 40 rows are single functions, and the scanner extents match the bodies: each ends at its `ret` and is padded
with int3.

## Label corrections

| Row | Catalog label (propagated) | Actual |
|---|---|---|
| 00a82870 | CMeshInfo | pair<unsigned __int64, long> |
| 00cbbe50 | CMeshInfo | {u64, long, long, CCharString} |
| 00809590 | CMeshInfo | pair<C3DVector, long> (targeting comparator) |
| 00a82820 | pair<long,long> | pair<unsigned __int64, bool> |
| 00b85b30 | pair<long,long> | CEngineSubPrimitiveRenderer::CSortElement |
| 0076bb70 | pair<long,long> | pair<ulong, CParticleUpdateParamDesc<ulong,ulong>> |
| 00470dbc, 0086fac0 | pair<long,long> | pair<ulong/long, {long, long}> |
| 00ad5b50 | CKeyPairCompareLess<long,long> | SKeyPair8 with a function-pointer comparator |
| 00af3700 | pair<float,CCharString> | pair<CCharString, {long, long, CCharString}> |
| 00cb6ea0 | pair<float,CCharString> | pair<CCharString, CScriptInfo> |
| 007212d0, 00adbe70 | pair<float,CCameraTag> | 12-byte records keyed at +4 |
| 00808a30 | `_Insertion_sort` | `__unguarded_insertion_sort` front end |
| 00809840 | `_Insertion_sort` | `pop_heap` front end |
| 00817540 | `_Insertion_sort1` | `__unguarded_insertion_sort_aux` |
| 00805fe0, 00a70020, 004dfc42 | `_Med3` | SGI `__median` (returns a reference, never swaps) |
| 00496fd0 | `_Med3<CWideString*>` | `__unguarded_linear_insert<CWideString>` |
| 00a40250 | `_Med3<pair<CCharString,CCharString>*>` | out-of-line pair `operator<` (cdecl) |
| 0086d5a0 | `_Move<CHeroLogBook::CQuestObjectiveDesc*>` | `__copy_backward` of 16-byte CVectorMap records (CQuestObjectiveDesc is 12 bytes in Ego_r.pdb) |
| 00513500, 006c2aa0 | `_Move<CLevelFiles*>` | `__uninitialized_copy` |
| 005355c0, 00aec6f0 | `_List_val<...>::_Buynode` | the whole STLport `list::insert` |

Callee labels that are also propagated:

| Address | Manifest label | Actual |
|---|---|---|
| 0xAD4880 | "std::_InsertionSort_PartitionHelper" | `__linear_insert` |
| 0xB85870 | "Insertion_HeapShift" | `__linear_insert` |
| 0x808120 | "_Copy_impl<CNavigationPosition*>" | `__unguarded_insertion_sort_aux` |
| 0x808DB0 | "_Pop_heap_0<...CSoundPair>" | `__pop_heap_aux` |
| 0x8175D0 | "_Adjust_heap<CDebugBox*,int,CDebugBox>" | `__adjust_heap<THitLocationIntersectionElement>` |
| 0x6741E0 | "Vector_Assign" | the 12-byte vector's `operator=` |

These callees are `__linear_insert` instances whose manifest names are propagated: 0xA81C50, 0xA81CC0, 0xCBB920,
0x76B5D0, 0x470E08, 0xAF35A0, 0xCB6CE0.
