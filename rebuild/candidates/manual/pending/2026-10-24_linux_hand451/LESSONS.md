# Chunk 451 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `return (a && b && c) ? true : false;` gives retail's `mov al,1 ; jmp ; xor al,al` bool tail; a plain `return a && b && c;`
  gives `xor eax,eax ; inc eax` (0x00CBE10F, 0x00CBE150, 0x00CBE2FF).
- Size-optimised script-helper TU: `#pragma optimize("s", on)` alone. Adding `optimize("y", off)` puts a frame on bodies
  that have none (0x00CBE2FF); small C3DVector/C2DVector operators must be `__forceinline` or "s" calls them.
- C3DVector length: `X*X + Y*Y + Z*Z` multiplies in z, x, y order; `(float)sqrt(X*X + Y*Y + Z*Z)` in z, y, x order
  (0x00CBE2FF v 0x00CBE512).
- A script loop that stops when the thing dies is `do { if (!thing.IsAlive()) break; ... } while (alpha > 0.0f);`
  (retail `je` past the loop, 0x00CBE716).
- One CCountedPointer model fits a whole STLport TU: the D-form release `if (Info) { --Info->RefCount; if (Info->RefCount == 0)
  { DeleteFunc(Data); delete Info; } } Data = 0; Info = 0;` matches the C rows (where the short `if (Info && --Info->RefCount
  == 0)` form also matched) and the D rows, whose retail bytes show the re-read and the two zero stores
  (`mov [esp+10h],esi ; mov [esp+14h],esi` in 0x00CBB860): 0x00CBA840, 0x00CBB860.
- VC7.1 emits the implicit `pair` operator= out of line when the pair holds a CCountedPointer/CCharString; retail calls
  it too (0x00CBA520 for pair<u64, CCountedPointer>, 0x00CBA700 for pair<CCharString, CCountedPointer>) -- do not inline
  it by hand (0x00CBA840, 0x00CBA8E0).
- A same-TU `__declspec(noinline)` body of an out-of-line read-only callee (CVectorMap::LowerBound) under
  `#pragma auto_inline(off)` lets VC7.1 keep the caller's key/`this` in registers instead of re-reading argument slots
  (0x00CB82F0); the same trick on the vector copy constructor keeps the snapshot's start pointer in edi (0x00CB8170).
- A local copy of a CVectorMap (vector + empty Compare + bool Dirty) is 16 bytes: its frame is 4 bytes bigger than a bare
  vector copy and the dead Dirty copy is dropped -- a `sub esp,18h` with only a 12-byte vector in use is the whole-map
  snapshot (0x00CB8170).
- x87 (polar helper, parked rows): a float local used in two products is homed in memory (`fstp dword [ebp-4]`); a
  single-use one is forward-substituted into its use; constant cos(0) is hoisted above an unrelated call once homed; VC7.1
  reassociates `a * b * c` FP chains (puts the intrinsic first), so operand order decides which product is shared
  (0x00CBE5F0, 0x00CBE693).
- `__fastcall` GFFloatRandom(float, unsigned long* seed) 0x004978A0: the float goes on the stack, the seed pointer in ecx.

Review at landing:
- Rows with more than 4 grades: none. 0x00CB82F0 matched on its 4th grade; the C-element STL rows got one extra regrade
  for the shared D-form CCountedPointer model (all still RELOCATION_MATCH / MATCH).
- Local models next to existing engine headers: `CScriptThing` (rebuild/include/engine/CScriptThing.h models the vptr as a
  data member, so the helpers use a local polymorphic model with the retail slot order and the PDB member names
  PImp_Data/PImp_Info) and `CSpawnedFuncBase` (0x00CB7BC0, 0x00CB8170: local model with its CASuspendableProcess base;
  Interrupted +0x29 agrees with the header).
- Already-landed sources with wrong identity: 0x00CB8220 is `CScriptBase::RunSubThreads` (landed as
  Destval_CRegisteredMusicEntry / `Close`); 0x00CB8D30 calls 0x00BFE9BC operator delete but its landed source declares
  `helper_bfeb1c` (an operator-new import) and names the function `operator_new`; 0x00CBD5B0 is CVectorMap<u64,
  CPersistInfo>::Sort calling `_STL::sort` 0x00CBD370, landed with the callee as "GuiVarBool_Apply". The landed
  0x00CBB1B0 / 0x00CBB2D0 files carry Dinkumware `std::` names but this TU is STLport (`_STL::__destroy_aux`,
  `_STL::__unguarded_insertion_sort_aux`).
- Manifest label 0x00CBE583 "CScriptBase::Activate" is GetSquaredDistanceBetweenThings (no fsqrt, two GetPos calls).
- No catalog row of this chunk holds two functions.
