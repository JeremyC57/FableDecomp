# Chunk 620 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- By-value class argument after an out-of-line default ctor (`CDeletion d; d.Method = 1; it->second->SetDeletion(d);`): VC7.1 sinks the
  member store toward the copy-ctor call only as far as the first load it cannot disambiguate. Reading the callee object into a
  named local AFTER the ctor (`CDeletion d; CComponent* c = it->second; d.Method = 1; c->SetDeletion(d);`) moves the store past the
  node load and the first `push ecx`, but not past the vtable load retail also clears (0x005726A0; same shape in 0x0058F649's
  photojournal block). Partial lever only: record the remaining residue as store scheduling.
- A vector destroy loop written as `for (p = _M_start; p != _M_finish; ++p) p->~T();` reloads the end from the frame every iteration;
  STLport's `_Destroy(first, last)` with by-value iterators caches the end in a register (`mov ebp,eax ... cmp edi,ebp`) as retail
  does (0x00622A00, items vector of CIntelligentPointer).
- Residue counting: a normaliser that masks only `call`/`[abs]` operands counts every `push offset string` (retail imm32 v object
  `push 0`) as a diff; 0x005726A0 read as 108 lines but is 4. Mask `push imm32`/`mov reg, imm32` against `0` before judging a row.
- Unnamed C2DVector argument temporary (`ChangePosition(C2DVector(x, y), 0.0f, false)`) compiles identically to the named local in a
  size-optimised frame TU: no lever for the push-ecx/fsubp order (0x0047E9AE).

Review at landing:
- All ten rows are parked; each has been graded well over 4 times across passes (sec/retry, chunk 537, this chunk). Treat them as
  VC7.1 scheduling / register / frame-slot lane gaps unless a sibling with the same shape lands.
- Landed 0x00A6CBB0 (rebuild/src/compiled/00/a6/CEngineInternalPrimitive_AddChildPrimitive_00a6cbb0.cpp) models a CPhysicsMesh
  private const getter as a global `int* AddChildPrimitive()` over two fake globals: respell as
  CPhysicsMesh::GetSingleInstanceCollisionSet (NAME_FIXES.tsv).
- Local models next to existing engine headers: 0x009FB750 (CTexture.h), 0x005726A0 (CTCInventoryExperience.h), 0x0057DF85
  (CTCHeroStats.h), 0x0058F649 (CTCInventory.h) use local descriptive classes (retail offsets differ from the PDB headers for
  several of these, e.g. CPhysicsMesh is PDB minus 0x1c); 0x0047E9AE and 0x00489D40 include engine headers.
