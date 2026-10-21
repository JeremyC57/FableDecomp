# Chunk 274 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Empty STLport tag/allocator types need user constructors: `struct __false_type { __false_type() { } };` (the same for `allocator<T>`). A value-initialised empty temporary otherwise emits `mov byte [esp+n],0` stores that retail lacks (00ad9b20, 00ad9980).
- Index loops that write through a `CArray<unsigned short>` match only with a hoisted base pointer, `unsigned short* index = &indices[0];` and `const unsigned short* listIndex = &listIndices[0];`. Retail steps the pointer, `add edi,6`, and keeps the index in ecx (00f3f3a0).
- An STLport `list::insert` needs a named `prev` local: `_Node* prev = pos._M_node->_M_prev;`. Without it the link stores come out in the wrong order (00ada240).
- When retail calls a destructor out of line (0xA96DA0 ~CVertexGroupBlock), declare that dtor without a body. An inline body expands it (00ada240).
- Write rounded-up block arithmetic inline: `full = (n - 1) / (MaxN - 1); rem = n - full * (MaxN - 1);`. A named per-block local changes the register picks (00ad9d70). A member read early goes into a named value local: `unsigned long maxN = MaxN;` (00ada240).
- Watch loops that retail expands into one inline ~CScriptThing at the loop exit: write `if (!IsActiveThreadTerminating()) { do { ... } while (!IsActiveThreadTerminating()); }`. This rotated form gives the single expansion (00e6a340/00edc010, the advisor's v2).
- VC7.1 makes the inline-dtor decision function-wide. A retail function that calls ~CScriptThing out of line at one exit and inlines it at the others cannot be matched; park it as a lane gap (00e36390, 00e53d90, 00ed9790, 00ee6a40).
- A held-zero register (`push edi; xor edi,edi`) used for inline-dtor compares did not reproduce. Neither the killed-test spellings, a bool local nor the do-while wrapper gave it (00e6ad20/00edc980).
- CWaterPatchMesh::Render: a named camera local with a per-field Position copy, plus a named vertexBuffer local, reproduces retail's frame layout (00beaca0).
- An inline function that returns a class temporary by value keeps the temporary in the frame, as retail does. Give it a single trailing return; multiple returns split the epilogue (00407e10).
- Use explicit `CCharString("")` temporaries at the point where retail builds them, rather than an implicit `const char*` conversion (00407e10).

Review at landing:
- Rows that needed more than 4 grades:
  - 00407e10 (8 probes);
  - 00ad9b20/00ad9d70/00ada240 (the optimiser family);
  - 00f3f3a0, 00beaca0;
  - 00e6a340 (advisor).
- Already-landed ReorderTriangleListInPlace models 0xF3EA50 as `GenerateStrips(CArray<long>&)`. 0xF3EA50 is really the `CArray<unsigned short>` x3 index-list overload of GenerateTriangleListAndStrip. Check its relocation identity against 00f3f3a0's binding.
- Local struct models are used for CVertexGroupBlockOptimiser/CVertexGroupBlockList. The layout is retail-proven in a `// Layout:` comment: 12-byte STLport vectors, Blocks +0x10 ... Iterations +0x54. Reconcile them with any engine header at landing.
- Twin rows that must land as distinct symbols:
  - 00e6a340 (ChapelOfEvil) and 00edc010 (TempleOfLight) WatchForWeaponUnsheathed. Their members are offset by +1.
  - 00e6ad20 and 00edc980 WatchForAcolyteDeath. These are byte-identical and both parked.
- The catalog lists 00e44cc0 and 00e538f0 under the same name. 00e538f0 is CV_BeardyBaldyScript::IsHeroWearingAnyTash.
- 00c9cd20 is not CVsyncCallbackTimerInfo. It is a libpk stream-decoder init (C code).
