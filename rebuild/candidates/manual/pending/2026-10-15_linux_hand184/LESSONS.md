# Lessons (chunk 184)
- A bool HasTC result spilled to a frame byte (`shr ecx,3 ; and cl,1 ; mov [esp+13h],cl` with no reload) is a named bool local tested twice; the second test is threaded away and the spill stays (0x00940240).
- A filter chain whose last operand is built first and stored last (`a && (b && c)`) is right-grouped; VC evaluates the inner pair first (0x00968460).
- `delete p` keeps its null test only when the pointer is re-read (`delete node->value` after `node->value->x` test); a cached local lets VC drop it (0x0053FD00).
- A local array destroyed with the end pointer cached in a register is a by-value `_Destroy(first, last)` helper inside the inline destructor (0x00717AA0).
- A called function that just returns 0 (`xor eax,eax ; ret`, no ecx set) feeding a base ctor argument is a static member (CCompressorZlib::GetWorkspaceSize).
- The STLport __false_type tag needs a user ctor, else `__false_type()` is zero-filled with stosb.
- A struct-copy C3DVector member init forms the address (`lea eax,[esi+0Ch]`); a user member-wise copy ctor stores at the member offsets.
- Real CGSI slot names live in ghidra_out/cgsi_slots_aligned.tsv.
