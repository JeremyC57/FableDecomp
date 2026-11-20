# NF189 lessons (one line each)
- The size-pragma scaffolding is cheap when the owner row already sets optimize("s"): an explicit specialization (member template: `template<> template<>`; plain member: `template<>`) under the pragma pair made both pick-any copies byte-identical in the first grade.
- A row with its own row-head optimize pragmas can still use such an opt-in when it includes the header before the pragmas (the header's reset runs first); check by grading with and without the macro.
- Taking a counted-pointer deleter's address emits the header's inline DeleteData; with only a forward declaration of T the copy is an 8-byte incomplete-type delete. Include the class header so the copy equals the owner.
- Before moving binders onto a CCountedPointer destructor claim, scan the exe for E9 jmp thunks to the address: seven claims here were ReleaseData bodies. The 53-byte /O2 form has no thunk and 976 twins, so its identity needs a decision, not a census.
- A one-symbol witness table goes stale: three reloc_bindings witnesses for 0x0099EFB0 bound the right spelling when recompiled.
- A header-less local wrapper that a callee's declaration uses (CRetailHandleVector) can keep its token on a shared std::vector base; respelling the member type instead would have moved the callee binding too.
- An explicit instantiation of one member (`template void X<T>::f();`) keeps the object to that leaf plus what it needs; `template class` would emit every member, including ones other rows own.
