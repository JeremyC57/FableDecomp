# Chunk 349 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `_Construct(p, v)` rows labelled `_Cons_val<allocator<pair<...>>>`: `new (p) T(v)` in a /Gr TU gives the retail `test esi,esi ; je` null test; the implicit element copy inlines (13 rows: 004cc3c0, 007aca30, 00a60950 ...).
- A map value whose members retail copies at fixed offsets from the ELEMENT pointers (008eba50, 0047c250) must be modelled flat in the element; a nested value struct (implicit, user or pair-level copy) makes the lane re-base both pointers (`lea esi,[edx+4]`).
- A nested std::pair<long, POD28> inside a pair needs a user memberwise copy ctor, else the long and the POD merge into one 8-dword `rep movsd` (0071f4e0).
- Size TU + placement copy of a polymorphic class: under `#pragma optimize("s")` the implicit copy ctor is emitted out of line (`push edx ; call`); spell the memberwise copy ctor `__forceinline` (004e9c39, `push 7 ; pop ecx`).
- `(long)(x * K1 * K2)` with literal floats folds K1*K2 into one constant even under /Op; retail's two `fmul`s need the first product in a named local (00c1ba20).
- A vector-index accessor that loads `first` before the index (`mov eax,[ecx+20h] ; mov ecx,[esp+4] ; imul`) wants a named `first` local; `Categories[index]` copies `this` to eax first (005bd328).
- SGI `__copy` forward: `*result = *first; ++first; ++result;` (first stepped first) -- the comma form `++result, ++first` swaps the two `add`s (00ad6ff0).
- __unguarded_linear_insert with a greater-than comparator compiles to `test ah,41h ; jne` exit, the less one to `test ah,5 ; jp` (0067e550 vs 004cfd40).

Review at landing:
- 004cfd40 defines `__unguarded_linear_insert(SFloatKeyedRecord*, SFloatKeyedRecord, SFloatKeyLess)` -- the very symbol the landed 0x004D1000 __linear_insert declares; keep the types identical.
- 00c1ba20 EncodeFixed16 is also a file-static body inside the landed CChickenSign::Init (0x00C1BC90); that static copy folds the two constants (it is not graded, but its comment claims `x*65535*0.0001`). No symbol clash (internal linkage).
- 004e9c39 / 00d34ec0 model CShopItemDef / CDefClassBase locally: the generated engine headers are flat byte-packed PODs (no base class, no vtable) and cannot express the base copy constructor.
- 008eba50 / 0047c250 flatten the map value's members into the element (see lesson 2); the real CContainedBank / CDefClassInfo layouts are not recovered.
- 00812840 and 008132d0 are byte-identical instantiations given distinct descriptive types (one address, one symbol).
