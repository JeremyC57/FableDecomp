# Chunk 344 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- VC7.1 gives overloaded virtuals vtable slots in REVERSE declaration order. Declaring `ChangeState(K)` then `ChangeState(K,float)` puts (K,M) at slot 47 and (K) at slot 48. GetState follows the same rule: const at slot 134, non-const at slot 135 (0x0041C5A0, 0x0052CEB0).
- An unaligned 0x41xxxx body (one that starts mid-paragraph, no 16-byte padding) needs `#pragma optimize("s", on)` (0x0041CB9D COk::ProcessEvent).
- A manager member loaded straight into ecx (`mov ecx,[eax+0xb8]`) needs an inline accessor such as `GetMouseCursor()`. A raw member read loads into eax (0x0055A5B0).
- For `std::find` followed by `!= end()` that must re-read end, keep the find result in a NAMED iterator. Comparing the temporary directly compares against a cached end (0x0055CA40, 0x0052D940).
- A bool tail `test ; jne ; ... ; mov al,1 ; ret ; xor al,al` is two sequential `if (...) return true;` and then `return false;` (0x0055CAE0).
- When a float spills into a dead argument slot (`fstp [esp+4]`) instead of its own frame word, write the computation and the store as one statement (0x00545DF0).
- In a secondary-base override that touches primary-class members (`add esi,-4` once), move those accesses into an inline member of the PRIMARY class. Written in place, the subobject pointer is kept and the header is cached in edi (0x0055BD50).
- `std::find` over a set with the tag temporary homed in the live parameter slot needs a same-TU `__declspec(noinline)` `__find(first, last, val, input_iterator_tag)` with an unnamed tag, under `#pragma auto_inline(off)` (0x0052D900, 0x0052D940, 0x0055BD50).
- A by-value smart-pointer return (CDefPointer) in a test needs a ref-counting copy ctor. VC7.1 has no NRVO, so the local copy is destroyed (test lane only, 0x0055C810).

Review at landing:
- 0053dcd0 and 0053dd50 share a dead `push ecx` / `mov [esp+8],0` with about 24 sibling pushers. Solve it once for the family.
- The copied list in 0055cb10 SetEvent: retail calls the out-of-line list copy ctor 0x55CF50 and dtor 0x42AC25. Both are mislabelled in the manifest (see NOTES.md Label corrections).
- The landed 0x00556580 comment names CObserver slot 4 "SetState". It should say IgnoreEvent.
- These rows use local class models with vfunc filler, because the rows needed retail-offset layouts (CComponent members 8 below the PDB, CSprite-derived members 0x30 below). Check them against `rebuild/include/engine/<Class>.h` when the generated headers carry the retail offsets.
