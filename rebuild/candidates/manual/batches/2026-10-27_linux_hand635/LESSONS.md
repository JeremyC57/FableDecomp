# Chunk 635 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- Same-TU STLport COMDAT bodies (noinline, defined before the caller) give VC7.1 mod-ref knowledge, so a local vector / out-param no longer escapes: push_back's value is homed in a dead argument slot (0x00693140, `_M_insert_overflow` body); zero stores move before wcslen and the insert position is known zero (0x0099BA70, `_M_fill_insert`/`_M_insert_overflow`/`fill`).
- Inside an inline random helper, a named `double result = fmod(...)` local schedules the integer seed update (`imul/add/ror`) before the fmul/fmod (0x00A146C0; the landed GFFloatRandom 0x004978A0 still matches with it).
- VirtualAlloc retry loop: `regionSize = 0;` at the loop top plus a named `unsigned long protect` local before the call gives retail's `xor ebp,ebp ; push ebp` placement (0x00A65720).
- A named parent-object pointer local (`T* parent = ParentClass;`) in place of re-reading the member gives `mov eax,[esi+14h] ; push edi ; lea edi,[eax+164h]` before IsValid/Release (0x00D3AE40).
- VC canonicalises the operand order of a commutative x87 add inside a member operator+ (a source swap does nothing); switching member to friend operator+ flips the load order of every lane at once, so mixed lane orders cannot come from a single helper (0x00A17690).
- Respell the memory allocator to Ego_r.pdb's nested classes CMemoryAllocatorVariableSize::CMemoryAreaChunk / ::CMemoryArea, with the PDB inlines CMemoryArea::Create (static __fastcall) and CMemoryAreaChunk::InsertAreaIntoAllocatedList (0x00A65480).
- A function whose `this` arrives adjusted (+4, primary vtable at [esi-4]) is a secondary-base vtable slot. Check rtti-vtables.tsv for the owning class before trusting a "vfunc_N" label (0x0055C6F0 = CMouseCursor::ProcessEvent).

Review at landing:
- Rows past 4 grades: 0099ba70 (~14), 00a65720 (~16), 00a17690 (~9), 00a65480 (~6), 008c7280 (~6).
- 0055c6f0 differ still uses placeholder v000.. virtual slots on a local model: header debt (no engine/CMouseCursor.h with real slots).
- 00d3ae40 passes the priority as `(EScriptAIPriority)4` (no enumerator name proven).
- 008c7280 differ uses a local GetSubDef<CTrollBattleDef> stand-in and local CDefPointer/CThing models; the landed 0x008C78F0 header (FABLE_RETAIL_DEF_CLASS_BASE) should replace them.
- Landed memory-allocator siblings (0x00A65060/0x00A650F0/0x00A65410) use flat class names and the wrong globals g_LastAllocatorUsage_013d28d4 / g_LastAllocatorChunkCounter_013d28d8; respell per NAME_FIXES.tsv.
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound has ~32 different landed spellings; sweep needed.
- 8 landed sources spell the C3DVector inline GetSquaredMagnitude; 168 use GetLengthSquared (used here). Respell the 8.
