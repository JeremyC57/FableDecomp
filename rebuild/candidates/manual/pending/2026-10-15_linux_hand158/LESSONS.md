# Lessons from chunk 158 (for GOTCHAS)
- The inline counted release `mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test/cmp edx ; mov [eax],edx` (listed as a lane gap in chunk 147) is reproducible: a CDefPointer dtor calling the def's inline `ReleaseTemporary()` with NO null test, whose DecRef()/GetRefCount() accessors live on a ref-count BASE at +4 (the `add eax,4` is the base adjustment) -- 0x00545720, 0x00544480.
- A member-function-pointer call `(obj->*pfn)()` compiles to `mov ecx,[blk+8] ; call [blk+4]` only when the class is COMPLETE; a forward-declared class gives the 16-byte general member-pointer thunk (0x00A3D460).
- A dtor with no own vtable store but the base's store at the end is a `__declspec(novtable)` derived class over a normal base (0x00A3D460).
- A tail-recursive `return Allocate(size)` becomes the backward jump to the top of the function, so retail re-runs the size rounding (0x00A5C910).
- STLport vector(n) of a 12-byte element: a user memberwise copy ctor keeps the zero initialisers as immediates and re-reads the T() words each pass; the implicit copy uses one zero register and hoists them into ebx/edx/ecx with the cursor in ebp (0x007C2AB0 vs 0x008F69B0).
- Six stores into a request struct come out in source order: write the assignments in retail store order (0x00A3E4B0); a ctor whose only init-list member is the argument and whose constants are body assignments explains an argument store between zero runs (0x00B59B60).
- `index - 1 > 0 ? index - 1 : 0` is VC's branchless `setle ; dec ; and` (0x00B21450).
- `(first++)->second` in a copy loop puts the pointer increment between the two dword copies (0x0081AE50).
- Lead (unconfirmed): retail's `fstp [dead slot] ; mov eax,[slot] ; push eax` float argument appears under /Op with a named float local (0x00544210).
