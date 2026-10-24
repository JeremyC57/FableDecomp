# Chunk 392 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- q.py / d.sh do NOT read the `// Leaf:` comment. Without a leaf argument the grader takes the largest function. Pass the leaf
  (`q.py <addr> <abs src> '<leaf>'`) while iterating. gall.sh does read `// Leaf:`. A leaf prefix shared by two overloads
  (`?erase@...` for erase(it) and erase(first, last)) must be the full decorated name (0x00756420).
- Link-time STLport algorithms are `__fastcall` (first arg in ecx, second in edx, the rest on the stack, `ret N`):
  __unguarded_insertion_sort_aux, __linear_insert, __partial_sort, __introsort_loop, __adjust_heap, __make_heap, __find_if,
  __uninitialized_copy / _fill_n, for_each (0x0073D720, 0x00742360, 0x00743EC0, 0x007485F0, 0x00748A40).
- An empty tag or temp type (`random_access_iterator_tag`, `__false_type`) passed by value needs a user empty ctor, or VC7.1
  emits a zero byte store. With the ctor the temporary is homed in a dead argument slot (0x0074AC80, 0x007577F0, 0x00748AA0).
- `if (find(b, e, v) == c.end())` makes VC7.1 evaluate end() first and compare the stale pre-call value. Name the result
  (`T* it = find(...); if (it == c.end())`) to get retail's post-call re-read (0x0074AC80).
- A CDefPointer dtor `if (PDef) { PDef->Release(); PDef = 0; }` needs Release as an INLINE MEMBER of the def (def kept in ecx,
  `dec [ecx+4] ; jne`). A free helper or a const-cast path reloads PDef (+14 bytes, 0x00742930).
- push_front vs push_back on an STLport list: `mov eax,[ecx] ; mov esi,[eax]` (header->_M_next) is begin(); end() keeps
  the header itself. The catalog's ModeList_PushBack was push_front (0x007421C0).
- A by-value empty comparator only folds onto its argument slot (`lea ecx,[esp+N]` on the incoming slot) when the comparator's
  operator() BODY is compiled earlier in the same TU under `#pragma auto_inline(off)`. An extern declaration makes VC copy
  the functor into a second frame slot (+4..+12 bytes; 0x00748C10, 0x00749170, 0x00749530).
- A functor applied by an inlined for_each must be `__forceinline operator()`. A plain inline operator() is CALLED
  out of line by VC7.1 (63v185 at 0x0074FBB0).
- `__declspec(novtable)` most-derived class plus an empty user dtor drops its own vtable store; members die in reverse order;
  a vector of polymorphic elements destroys through `push 0 ; call [vtbl]` (0x00742930, 0x00743D40).
- `x - y` of two calls (`UpperBound(k) - LowerBound(k)`) calls the left operand first and keeps it in a callee-saved register
  (0x0073DA50, 0x00757870).
- A pair with a user copy ctor is "non-trivially assignable" to STLport's traits: copy_backward is then the `__copy_backward`
  count loop (`n = last - first`, `*--result = *--last`), not memmove (0x00742360). Plain pointers use
  __copy_trivial_backward + memmove (0x00749170).
- `(x * k1) * k2` with literal float constants is folded by VC7.1 into one `fmul` even through an inline getter, a reversed
  operand order or `/ 2.0f`. Retail's two-`fmul` shape needs an unfoldable factor (0x00752510, parked).
- YZ angle: `atan2(dz, sqrt(dy*dy + dx*dx))` with the y square written first reproduces `fld st(1) ; fmul st(2) ; fld st(3)
  ; fmul st(4)` (0x00754E70, first grade).

Review at landing:
- 0x00749620, 0x0074A4E0, 0x0074FBB0 and 0x00756C60 each took more than 4 grades and are parked (register allocation or
  frame-slot order only). 0x00752510 is parked after 4 grades (constant fold).
- Local base models with no engine header in use: CTCLook (no `engine/CTCLook.h`; the landed CTCLook files also use local
  models); the STLport vector/list/_Rb_tree models are local by necessity (retail STLport sizes: vector 12, list 4, map
  header+count+compare; the Ego_r.pdb Dinkumware offsets differ: CVectorMap Compare/Dirty +0x0C/+0x0D retail vs +0x10/+0x11 PDB).
- Landed-source naming spellings seen while reading: the landed `CVectorMapunsignedlong_UpperBound_0073d250.cpp` line 1 reads
  `CVectorMap<unsigned_long::UpperBound` (unbalanced `<`, a generator artefact). Also, the manifest labels 0x00749390 sort_heap
  and 0x00748880 __push_heap with Dinkumware propagations (`_Sort_heap<long*>`, `_Push_heap<float*,...>`) that belong to the
  CThing* CCompare_FurthestFromInitPos2D family.
- Catalog rows holding several functions: none found in this chunk.
