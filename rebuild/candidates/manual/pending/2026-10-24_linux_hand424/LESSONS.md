# Chunk 424 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU noinline callee bodies settle escape analysis.** Give the callee that takes `&local` (or a `const T&` of a
  frame local) a same-TU `__declspec(noinline)` body. VC7.1 then sees the local is not captured and keeps its fields in
  registers or packs temporaries into its slot.
  - 00ad4720 insert_unique: an `operator--` body packs `_M_insert`'s temporary into j's slot (177 → 175).
  - 00ac96e0 UpdateContractions: UpdateCost / ComputeContraction bodies keep the counted-pointer local in eax/esi
    (252 → 236).
  - 00ab3b40 AddStream: an `_M_insert_overflow` body homes the tag on the live flag slot.
- **STLport `pair` needs both user constructors.** That is `pair(const pair<_T1,_T2>&)` plus the template converting
  constructor. Without them a node clone in `_M_copy` loads the source after malloc instead of before it (00ad3500,
  184v184).
- **STLport `less<T>` must derive from `binary_function<T,T,bool>`.** With an empty `less`, `__less()`'s
  `less<T>()` is value-initialised: a zero byte store, then the comparator slot is reloaded (00ac8960, 204 → 196).
- **/Gr free algorithm templates with a tag.** `__copy(first, last, result, const input_iterator_tag&, _Distance*)`
  puts the hidden return pointer in ecx and the tag reference in edx (the first register-eligible argument). Struct
  iterators go on the stack, ending `ret 14h` (00ad48f0).
- **/Gr with a leading 8-byte by-value argument.** That argument goes on the stack and the next 4-byte argument
  (comp) takes edx (00ad08b0 `__unguarded_linear_insert`).
- **Iterator `++` over an `_Rb_tree`.** Write `_M_node = _Rb_global_inst::_M_increment(_M_node)` with an inline
  static body: the walk runs on a register copy and is stored once (00ad4810, 00ad48f0).
- **Switch case grouping.** In a return-per-case switch, grouping only one value class (the three 8-byte types, in
  second place) keeps a direct jump table and retail's cross-jumped block order. Grouping every case gives a byte index
  table (98v108); keeping all cases separate moves the 8-byte block last (00ab3740).
- **POD `vector<unsigned short>(n)`.** STLport's generic `fill_n` loop compiles to
  `rep stosd ; adc ecx,ecx ; rep stosw` (00aadd80).
- **`and esp,-8` frames.** An 8-byte value copy (CGroup) in an inlined `__unguarded_linear_insert` gives an
  `and esp,-8` aligned frame with the comparator re-read from [ebp+0Ch] (00ad1ca0).
- **String literal pushes.** `push_back("x")` on a `vector<CCharString>` builds an implicit-conversion temporary that is
  re-addressed with `lea` for both the placement copy and the overflow call (00ab1660).
- **No NRVO for a named by-value result.** Write the named local: the record is built in the frame and block-copied
  (`rep movsd` x15) into the hidden result (00ab9dd0 GetBone, 00ad1e80 GetRoot).
- **CopyTo-style counted-pointer destructor.** `if (Info) { --Info->RefCount; if (Info->RefCount == 0) {...} }
  Data = 0; Info = 0;` re-reads Info after the decrement store. It is the shape inside `_Rb_tree::erase` (00ad3c30).
- **x87 sum canonicalisation is a lane gap.** A quadric form v^T Q v is factored and reordered by VC7.1 in every
  source order, even under /Op, while retail keeps each product separate (00ac6d50, 96v104). Park it early.

Review at landing:
- 00ab3b40 and 00ac96e0 needed the full 4 grades.
- These rows carry same-TU noinline stand-in bodies that are not under their leaf:
  - 00ab3b40: `_M_insert_overflow`.
  - 00ac96e0: UpdateCost (the retail code) and ComputeContraction (a stand-in that only tests Unsplit).
  - 00ad4720: `operator--`.
  - 00ad48f0: insert_iterator::operator=.
- The `_M_copy` / `_M_insert` / `_M_lower_bound` members are modelled public (leaf letters QAE / QBE), although SGI
  declares them private. The landed STLport `_M_copy` / `_M_insert` rows (0x006310E0, 0x00990830 and siblings) use the
  same convention; check the lander accepts it.
- All rows use local models (STLport in `std`, CCountedPointer, CCharString, CArray, QuadricOptimiser classes), not
  `rebuild/include/engine/<Class>.h`. Check whether headers exist for C3DMeshFile, CChunkedFile,
  C3DMeshFileBoneChunk, CVertexStreamFormat, CVertexShaderInput and C3DGroup2. The retail layouts differ from Ego_r
  where STL members are involved (12-byte vectors, 4-byte lists).
- These already-landed sources use wrong callee names:
  - CopyTo 0x00ACD5A0: "GetMeshChunk" for 0x00AD1E80 (GetRoot).
  - PerformContraction 0x00ACD840: "UpdateSubPair_00ac96e0" (UpdateContractions).
  - 0x00AD52A0 / 0x00AD5030: non-template "RecutTree" models.
  - 0x00AD3720 / 0x00AD3E30: generic CRetailTree operator= rows.
  - 0x00AD1B20 (`__uninitialized_copy<const COptimisedBone*>`) is catalogued as `_Uninit_copy<CFeatDef*>`.
- No catalog row in this chunk holds several functions.
