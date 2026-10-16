# Chunk 173 lessons
- A propagated "X::operator=" on a def body: the true class is the slot-19 `jmp` thunk 5 bytes before it (rtti-vtables.tsv); a def operator= inside the CDefClassBase TU (0x432E60) inlines CDefClassBase's own op= (root op= call, two dwords, `x ^ ((x ^ y) & mask)` per 1-bit field) -- write the base ops as `__forceinline` user copies.
- Implicit operator= of CVectorMap / small wrapper classes is emitted out of line in size TUs; `__forceinline` user copies of the implicit form inline them.
- A non-trivial implicit copy ctor emitted out of line (arrays copied as `lea` blocks, members memberwise) appears when a member type (C3DVector) has a user copy ctor; a placement-new copy in the TU emits it (0xAD0100).
- STLport `resize(n)` rows hide behind any label: T comes from the 2-arg callee's stride (`imul magic ; sar`) and the caller's member; some TUs' counted-pointer dtor zeroes both words after the release.
- STLport __median over a by-value comparator is `__fastcall` (a,b in ecx/edx, `ret 4+sizeof(comp)`).
- `x && x->GetDef()->Flag` with a by-value CDefPointer getter gives retail's conditional temporary (flag zeroed at entry, set to 1 on the building arm, tested before the release).
- Out-param GetInterface(&tc, k) keeps a component in eax for a field test where a returned GetTC value takes ecx.
- A float angle multiplied by a `qword` float-pool constant is `fraction * (double)FLOAT_CONST` feeding intrinsic cos/sin.
- The CDefPointer release without re-read and "component used as this" lane gaps recur (0x6631C0, 0x6632A0, 0x663BA0, 0x665510, 0x6A8560); the ReleaseTemporary `add eax,4 ; dec ; mov [eax]` form appears whenever the released pointer is a fresh local or `this` of an inline Release().
