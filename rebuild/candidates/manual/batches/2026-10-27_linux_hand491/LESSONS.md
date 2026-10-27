# Chunk 491 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (2026-10-27, chunk 491) `g.sh <addr>` ignores the `// Leaf:` line and grades the largest function in the object. In a TU
  that carries a same-TU stand-in body (009e7380's `__find_if`), it graded the wrong function: DIFFER 189v110 on a row
  that is RELOCATION_MATCH. Grade with `gall.sh` or `q.py <addr> <file> '<leaf>'` once a TU has more than one body.
- (2026-10-27, chunk 491) An implicit copy constructor (`??0X@@QAE@ABV0@@Z`, retail CatchableType 0x01369390) is emitted
  by a same-TU helper `void f(const X& e) { throw e; }`, with no /EHsc needed. The constructor lands as its own leaf; the
  helper is a model device.
- (2026-10-27, chunk 491) STLport `remove_if`'s inline `find_if` passes a `random_access_iterator_tag` to the out-of-line
  `__find_if`. Retail homes that tag in a live argument slot (the predicate's dead stack word). VC7.1 reproduces this only
  when the `__find_if` body is visible in the same TU under `#pragma auto_inline(off)` plus `__declspec(noinline)`. A
  declaration alone builds a fresh frame temporary.
- (2026-10-27, chunk 491) CVectorMap LowerBound/UpperBound are the explicit `count / 2` loop (`cdq ; sub eax,edx ; sar`).
  UpperBound must make the "step past" arm the if body: `if (!Compare(key, *it)) { first = it + 1; count -= step + 1; }
  else count = step;`. The `if (Compare(key, *it)) count = step; else ...` order flips jae and jb.
- (2026-10-27, chunk 491) Console colour setters that load the CGenericVar values as dwords and store bytes need
  long-typed parameters narrowed at the store. Uchar parameters (the PDB CRGBColour ctor/Initialise) narrow the loads to
  `mov al,[..]`.
- (2026-10-27, chunk 491) `CCharString::GetAsCharArray()` must be written `PStringData == 0 ? "" : PStringData->Data`. The
  reversed ternary swaps the branch layout (009ecc70).
- (2026-10-27, chunk 491) For `__pop_heap` and other STLport routines that copy a `pair<K, V>` by value, retail copies the
  key separately from the value block. Give the local `std::pair` a user copy constructor `first(o.first), second(o.second)`;
  the implicit copy is one `rep movsd`.
- (2026-10-27, chunk 491) VC7.1 /GS adds the cookie for a local char array even when the optimiser deletes the array
  entirely (0x009F2AC0: a 4-byte `push ecx` frame that holds only the cookie). A retail /GS function with no visible
  buffer use is a debug-build buffer (here the tracking record's char[48] Filename) compiled out.
- (2026-10-27, chunk 491) `CCountedPointer<T>::DeleteData(void*)` is a static __fastcall deleter, `delete (T*)p`. The
  counted pointer's `Reset` stores its address in the pointer-info block, so the one reference is a `mov [..], offset`,
  not a call. Look there to name the many tiny `if (p) { ~T inline; operator delete }` gap rows.

Review at landing:
- 009d47e0 lands the implicit CBankCreatorException copy constructor through the throw helper
  `ThrowBankCreatorException_009d47e0` (no retail address of its own). Check that the landing keeps only the leaf.
- 009f8860 spells `list::_M_create_node` public, as the bound list rows do. STLport 4.6 has it protected; the mangling
  (`QAE`) shows retail's is public.
- 009eaae0 uses a model-only inline `CRGBColour::SetChannels(long, long, long, long)`, not a PDB member.
- 009e7380 carries a same-TU stand-in body of `__find_if` (0x009E6A70). Its symbol must not bind as a second definition.
- 009f2ac0 keeps a dead `char filename[48]` so that the /GS cookie appears. The tracking record it stands for is real
  Ego_r layout (CTrackingDebugRecord::Filename), but the local is a device.
- 00a446a0 ThreadEntry is mis-cut into five catalog rows (see NOTES "Skipped"). Re-cut it as one 158-byte /Od + /EHsc row.
- RELOC_NAME warnings for 0x00A43F80, 0x00A33C50 and 0x00A34F00: the new STLport names replace descriptive catalog labels.
