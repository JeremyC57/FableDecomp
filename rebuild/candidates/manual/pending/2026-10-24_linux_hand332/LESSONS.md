# Chunk 332 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `GFSerialiseMapBinaryIn<V,K,C>` (`__fastcall` stream, map; `and esp,-8` from the 8-byte pairs) matches with inline clear(), the count through a forceinline long reader, a value-initialised `std::pair<K,V>` per pass and per-call `CPersistTraits<T>()` temporaries -- the element traits need same-TU `auto_inline(off)` noinline specialisation stand-ins (read stream, write `*out`) or the frame is 8 bytes larger (0067c020/0067c110/0067c200/006dba50).
- A count that retail homes in the dead stream-argument slot although the stream stays live must be read through an inlined `CPersistTraits<long>::TransferBinaryIn` (its own `value` local, then `*out = value`); reading straight into the caller's local gives it a frame word (00479369).
- A counted pointer's `GetRefCount() { return Data ? Data->RefCount : 0; }` compared with a constant gives `mov eax,[eax+4] ; cmp eax,1` (null arm folded into the false exit); `p && p->RefCount == 1` gives `cmp dword ptr [eax+4],1` (00a24fc0). Release that keeps the pointee in a register across `dec` reads Data into a local first.
- SGI `__median` with a `__fastcall` function-pointer comparator: elements by value (pointer T) load both words before each `call esi`; by-reference comparators pass the element addresses straight in ecx/edx (009322d0 vs 00a45a10).
- A by-value-pointer `__median` needs T to be a real pointer typedef, a 4-byte struct is passed on the stack (009322d0).
- Linear find + push_heap: `element == *it` with the push and `return` inside the loop; break + `it != end` adds a second compare (00a21c30).
- CPreAllocVectorMap bound walks: for 0x14-byte elements declare `long len = Size;` before `first = Elements;` (register swap otherwise); the 0x38-byte twins match either way (00a316b0).
- Nested `CPredicate_And<A,B>` operands by value put each nested vptr 4 bytes after its parent's; a user `~CPredicate()` on the base gives the merged `mov eax,base_vtbl` + three stores in the ??_G (00ef6e40).
- A pair<CCharString,long> binary reader builds no element-traits temporaries (a CPersistTraits<CCharString> temporary adds DefaultVal ctor/dtor calls); the text writer does build both traits objects first (006d3b90 vs 006d3880).
- `template <> void C::ReadVar<int>(int*, ...);` (explicit specialisation declaration of a member template) ICEs VC7.1 (C1001); declare non-template overloads instead (008269a0).
- Size-TU (ebp-frame) map reader: function-scope key traits value-initialised once at entry; the value traits get a frame word but no stores (modelled as an empty specialisation, against the PDB's 8-byte DefaultVal) (004e9156).

Review at landing:
- 004e9156 models CPersistTraits<CWeaponTrailGraphicSet> as an empty specialisation although Ego_r.pdb gives it an 8-byte DefaultVal (retail constructs none here).
- Stand-in bodies for other retail addresses in the TU: 0067c020/0067c110/0067c200 (CPersistTraits<EOpinion*>::TransferBinaryIn 0x45E011/0x45ED25/0x67AA40/0x40EFB0) and 006dba50 (CPersistTraits<long>::TransferBinaryIn 0x40FE60) -- check relocation identity, the stand-ins are not the retail bodies.
- Local stream models next to existing headers: 006d3b90 and the GFSerialiseMapBinaryIn rows model CMemoryDataInputStream locally (006d3b90 needs a real virtual at slot 6; the header carries the vptr as a plain member). The scalar readers use the header through a methods view.
- 0051a240 needed more than 4 forms (parked at 3 attempts + base).
- Already-landed sources of poor quality seen while looking for twins: 0x00C203C0 / 0x00C20400 CPersistTraits<long>::TransferOut (raw pad structs, no header comment), 0x00850670 CPhysicsLeaf ??_G (a `MakeOne` helper that is not part of the retail body), 0x00B32FF0 GFPredicateAnd (pad[0x298] model), 0x005D8167 CPersistTraits<EInputKey>::TransferIn (free fastcall function in place of the thiscall member).
