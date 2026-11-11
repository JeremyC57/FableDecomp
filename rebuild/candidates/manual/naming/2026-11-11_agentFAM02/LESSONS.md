# agentFAM02 lessons (one line each)
- Explicit instantiation over the shared STLport model + `// Flags: /Gr` reproduces every retail algorithm COMDAT; no local algorithm body is needed.
- A `#error` in a header hangs Wine cl.exe with no output: scan the include chain for `#ifndef X / #error` and define X up front; give every compile a timeout.
- Under `#pragma optimize("s", on)` the public uninitialized_fill_n keeps its `__false_type` helper as a call unless the helper is forceinline (overlay opt-in FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE).
- Retail does fold identical COMDATs: one address can be called with `T*` and `const T*`, or from vectors of two different T (0x00B8EED0); such rows have no provable single name.
- Landed STLport binders are a good first source but ~1% carry a wrong class key / back-reference; take the instantiation's own symbol when the binder names none.
- A def class's header that declares `virtual ~T();` makes `p->~T()` a direct call; retail's virtual call through slot 0 needs the implicit dtor.
- Landed behaviour tests for this family are self-contained models; they stay valid under any rename (but spell `__fastcall`, so x86-64 g++ prefails).
