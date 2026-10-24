# Chunk 353 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Propagated `_Dest_val` / `_Destroy_range` labels in this chunk covered sorts, wrappers, timers and destructors. The fastest
  route to a name was the callers plus an Ego_r.pdb name in `ghidra_out/egor_pdb_names.tsv` (other build, real method
  names): CMemoryDataOutputStream::Clear, C2DBoxI::GetDistance, LTimer::MSeconds, C3DSkeletalMorph::Reset,
  CBaseObject::ClearIntelligentPointers and the CParticleProgram GetUsedBones / RemapBoneIndices pairs all came from it.
- A by-value `T(*i)` sort value over a nested STLport pair needs the nested `pair<A, pair<B, C>>` type with the
  memberwise user copy ctor; a flat three-member struct addresses the third source from the second (004ba100, 79v81).
- A by-value comparator holding C3DVector members is copied member by member only when C3DVector has a user copy ctor;
  an all-POD comparator is copied with `rep movsd` (007fa290, 55v130).
- The STLport vector inside an element (`_Vector_base` with the end pointer in `_STLP_alloc_proxy`, a trivial
  `_Destroy(start, finish)`, `deallocate(p, n)`) is what gives retail's strength-reduced element pointer in
  `__destroy_aux` loops; a flat three-pointer vector does not (009b8070 / 00a5e900, 52v55).
- STLport `CArray::Clear()` = `if (capacity()) { clear(); swap(empty temporary); }` with `erase` through
  `__copy_trivial(last, finish, first)` gives the unfolded `cmp ecx,ecx ; je` and the three-word zeroing through one
  register, plus the temporary's null-tested free (00996b60).
- A box clamp `nearest = pos; if (nearest.X < TLX) ...` must test the LOCAL copy so the coordinates stay in registers;
  testing the reference re-reads it after each store and VC duplicates the call tail (00a3a180, 108v87).
- A range `insert_unique(first, last)` member template needs `pair<iterator, bool>` and the iterator with user ctors
  so the discarded result is returned through an 8-byte frame slot (004b9f10). Instantiate explicitly:
  `template void _Rb_tree<...>::insert_unique<const CCharString*>(const CCharString*, const CCharString*);`.
- A `for_each` whose callers pass a 1-byte hidden result slot (`lea ecx,[esp+0Fh]`) takes a functor deriving from
  std::unary_function (non-POD); declared as a `__fastcall` template it reproduces ecx = result, edx = first,
  `ret 8` (007f8760).
- A thiscall wrapper that calls the same static `__fastcall` helper twice without reloading ecx = LTCG knowledge that the
  helper preserves ecx. The helper's body, written from its retail bytes above the wrapper under
  `#pragma auto_inline(off)`, makes VC7.1 drop the reload (00ae15d0, 39v33). Grade with a mangled leaf prefix
  (`?GetUsedBones@CParticleProgram@@QBE`) since both share the name.
- The retail CParticleSimulation (it holds a double) is 8-aligned, so a flattened CParticleProgram model needs a 4-byte
  gap after the vptr: InitialSimulation +8, SimGroups +0xFC, NonSimGroups +0x108 (00ae15d0 / 00ae3a30).
- `fild ; jge ; fadd [0x122DCB4]` (2^32) is VC7.1's unsigned long to float conversion; a float to unsigned long return
  tail-jumps `__ftol2` (008f8030).
- Tests on the 64-bit g++ host: `unsigned long` is 64-bit there, so a wrap-around test of a 32-bit tick counter needs a
  32-bit typedef (008f8030).

Review at landing:
- 00ae15d0 carries a same-TU stand-in body of 0x00AE1210 (`#pragma auto_inline(off)`), written from retail bytes but
  not graded: there is no oracle row for 0x00AE1210, and it is only there for the ecx knowledge. It is accepted by
  the GOTCHAS 1063 / 0x75D2B0 precedent; land 0x00AE1210 separately and keep the two in step.
- 00ae15d0 / 00ae3a30 use a local retail-offset CParticleProgram model next to engine/CParticleProgram.h, whose
  PDB offsets put SimGroups at +0x11C; retail is +0xFC. The landed ctor 0x00AE3550 and PrepareForRendering 0x00AE1600
  do the same. The header needs a retail opt-in like CMemoryDataOutputStream's.
- 00996b60 opts into `FABLE_RETAIL_MEMORY_OUTPUT_STREAM` in engine/CMemoryDataOutputStream.h (the header's own comment
  already cites this row as layout evidence).
- 009b31a0 and 00c91320 have unidentified owners (`_unattributed`); 00633bb0 and 00add330 names are inferred (mode-table
  pairing; vtable slot 3 of 0x129F094).
- Already-landed CEngineInternalPrimitiveChanger_ClearAllChildPrimitives_00bc5c00 models CEnginePrimitiveHandle as
  {m_id, m_pPrimitive}; Ego_r.pdb and the 00bc5f50 bytes show it is polymorphic (vptr + CIVCountedPointer Data, vtable
  0x0122F598).
- Nine headers in this chunk first stated lengths 2 bytes short (the `ret N` was not counted); they are corrected, but
  check the stated lengths against the oracle when landing.
