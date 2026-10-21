# Chunk 262 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Implicit const char* -> CCharString argument temporaries are constructed BEFORE the other call arguments are pushed; an explicit `CCharString tmp("x")` is constructed after. The choice also changes which stack slots the temporaries share (009b0ac0, 00847df0, 00649e8b).
- VC7.1 parks uninitialised empty-class temps (STLport `__true_type` tags, empty exception objects) and spilled locals in dead parameter slots. `__true_type()` with an implicit ctor emits a zero byte store; giving the empty class a user ctor `T(){}` removes the store. `throw T()` of such a class reproduces the retail byte copy out of the dead slot (009c0e50).
- CCharString GetString/GetLength must be written null-first (`p == 0 ? "" : p->Data`, `p == 0 ? 0 : p->Length`) to get retail's `jne` ordering (009b0ac0).
- A map flag that is stored and then immediately re-tested needs separate __forceinline Add and Sort members, not one combined helper (009b0ac0).
- Auto-pointer assign `operator=(T* p){ T* old = m_p; m_p = p; delete old; }` evaluates the new-expression first, which matches retail's order (009c0e50).
- Read width/height/bitDepth into locals before the clamp ifs to reproduce retail's grouped loads (009c0e50).
- Make the base class polymorphic so the derived inline ctor overwrites the vptr and sizeof comes out right (CRenderManagerCore `_body[0x3ea0]`, giving 0x3ea4) (009c0e50).
- CEnginePrimitive2DSprite (0x00493AC2) is 0x54 bytes. DrawIcon (00645292) matched only with 0x54, because 0x58 gave a 0x78 frame. DrawStatBox (0064619f) fitted a 0x58 model, which really means one extra 4-byte local.
- In the 0x64xxxx CDraw region use `#pragma optimize("y",off)` + `#pragma optimize("s",on)`. Using `"s"` alone gives a biased (ebp-offset) frame; 0064b6da is the exception that wants `"s"` only.
- Write a GetTC lookup as its own statement, not inline in the call, so its key slot can share with the returned-name temp (00847df0 frame size).
- An implicit T* -> CCountedPointer<T> argument conversion builds the argument in place (`sub esp,8; mov ecx,esp`). An explicit named temp instead copies it.
- A long vs unsigned long mismatch in push_back forces a conversion temporary and keeps the source variable in a register (009adae0).
- Some TUs need __forceinline on vector helpers (insert/reserve wrappers), or they are emitted out of line (008c61c0: 357 bytes before, 456 after).
- STLport erase on POD goes through `__copy_trivial`; that is the source of the unfolded `cmp ecx,ecx`.
- Placement `operator new(unsigned int, void*) throw()` must be declared at the top of the TU, before any use (a90900 SRC_FAIL fix). Do not redeclare the plain operator new.
- Use `unsigned short` for wchar_t buffers, and give C3DVector a user copy ctor to get retail's copy elision.

Review at landing:
- Several rows use local `// Layout:` models even though PDB engine headers exist: CEngineLandscapeRenderer, CEngine, and CDefinitionManager (the PDB header layout does not fit the retail offsets used here). Reconcile them with rebuild/include/engine before landing.
- Manifest naming: AddChildPrimitiveWithID vs SetChildPrimitive needs a decision; see NOTES "Label corrections".
- The CAC_Owner vtable slot 18 is labelled "Accepts" but is really MsgIsKilledBy.
- The sprite size conflict: 0x58 in differ/0064619f vs 0x54 (matched) in src/00645292. Re-try 0064619f with a 0x54 sprite plus an extra local.
- test/00b68610 and test/00bbcf70 include STL headers. They pass under both g++ and VC7.1, but check that they are allowed under the test policy.
- src/009c0e50 uses `throw` in a TU with no EH frame (retail has none either). Check the landing build flags keep it that way.
- The lane gaps (00b41a80 inline-asm fistp, 00b99bb0 inline-asm SSE) cannot be expressed without asm. Treat them as asm_bake-only candidates and do not retry them by hand.
