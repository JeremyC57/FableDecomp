# Chunk 493 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An out-of-line STLport/CArray accessor COMDAT (`operator[]`, `size`) is emitted by explicitly instantiating just that
  member (`template T& CArray<T>::operator[](unsigned int);`); no caller or full `template class` needed.
- A tail call that writes BOTH argument slots (`mov [esp+8],1 ; mov [esp+4],edx ; jmp`) proves a two-argument caller
  frame: keep the PDB's unused second parameter (AAEXKK), else VC emits `call ; ret 4`.
- A value read twice from an unmodified member is CSE'd into one register; writing it into a named local instead can
  swap which register a loop's two induction values get (CheckStructure 0x00BF2E00: 69v69 with the local, MATCH without).
- `if (n != &Sentinel) return n->Key; return 0;` vs the reversed test decides which tail VC7.1 places first after a
  search loop; match retail's fall-through order.
- In a comparator, `ka == kb ? a < b : ka < kb` gives retail's shared `sbb ; neg` tail over two different compares.
- An `int` result of `!b` (bool member) is `xor ecx,ecx ; test ; sete cl ; mov eax,ecx`; a `bool` result would end in `sete al`.
- VC7.1 strength-reduces a `[node + i*4 + 0x1c]` child-table walk around a recursive call (GetTreeMemSize 0x00BF2980);
  retail kept the scaled index: four source shapes all gave the same 113v96 -- park, do not burn more grades.
- LTCG static helpers with register arguments (zlib syncsearch: edi/esi) stay a lane gap: grade once, park.
- When the target symbol is a callee of a landed class whose name is address-suffixed, write the caller on that SAME
  class name (ForEach on PKCTRRedblackTree_00c18d50, GetCount on _00c18df0) so the relocation binds to the landed symbol.
- vland warns SHARED_HEADER_AVAILABLE even for an unrelated same-named class (the steering library's CWorld vs the
  game's engine/CWorld.h): rename the local model (CLibWorld_00c1c620) instead of including the wrong header.
- A shell loop variable named `S` silently clobbers the scratchpad path: lc/objdump leaf checks then report NONE.

Review at landing:
- PKCTRRedblackTree is split across address-suffixed names (_00c18b20, _00c18d50, _00c18df0, _00c18d90): one class.
  Its +0x24 member is a node pointer (Find stores the hit node there), not the `Count` the landed models call it.
- CScriptHost is also landed as CCombatAbilityStrafeDef_00c0edf0 (its constructor); the host IS an event of
  CEventQueue_00c08940 (Post at slot 0).
- 0x00C16750 is spelled CDebugBreak (landed GetData/Append) but CTrace / CAssertTrace elsewhere; 0x0143BB2C is
  g_pfnAssertHook here and g_pfnLogSink elsewhere; 0x0139C900 is g_pchNil here and g_pDebugBuffer in 0x00C39830.
- Landed 0x00C0B380 binds 0x00C0B570 as SetTarget(void*, long, void*) while 0x00C3A950 and this chunk's 0x00C0B880
  use (float, float, float); same address, two mangled names.
- CRampedParam (0x00C0B880) and CInterpolatedFloat_00c3a950 (0x00C3A950) are the same 0x34-byte class.
- 0x00B8D280/0x00B8D2F0 write the folded occlusion pixel count as the literal 0.0f it compiles to (no PC occlusion query).
- CVtable_012b2ab8 derives LHA::CAllocator (RTTI 0x0129D264) and mirrors CBBBSoundAllocator's slots 1/2.
