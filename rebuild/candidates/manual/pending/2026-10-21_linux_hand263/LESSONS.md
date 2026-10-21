# Chunk 263 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport list append inlined into a caller (0x00B3A180): a bare `push_back` that links nodes in place makes VC7.1 shrink-wrap
  the callee-saved `push edi` past the early branches. Model the real shape (an iterator with a copy ctor,
  `push_back(x){insert(end(),x);}`, `create_node`) and, in insert, read `SNode* prev = position->Prev;` before linking. That
  gives retail's `mov ecx,[edi+4] ; mov [eax+4],ecx ; mov [eax],edi ; mov [ecx],eax`.
- A call's `this` loaded before the argument pushes means the object pointer is named in its own statement first
  (`T* p = handle.Data.Object; p->Attach(...)` at 0x00B3A180; `CEngineSceneGrid* grid = ...; grid->PreloadPrimitives(...)`
  at 0x00B930A0).
- A zeroed out-param struct per loop iteration whose four `mov [esp+N],0` stores sit ABOVE the vptr load/push
  (0x00B930A0, 0x00B92070) is a function-scope local cleared member by member inside the loop. A loop-scoped local with a
  zeroing ctor (any form: init list, member ctor, copy from a temporary) lets VC7.1 sink the stores below `push eax`.
- An out-bool passed by reference (`ClipBoundingBox(bounds, inside)`) declared at function scope gets a frame byte; declared
  in the inner block, it takes the dead argument slot that retail gives a later loop counter (0x00B92070).
- `if (!cell || (cell->Mask & m)) return false;` lets VC7.1 move the shared `return false` epilogue to the end of the
  function. Two separate `if (...) return false;` statements keep retail's inline epilogue (0x00B92790).
- An STLport `erase(pos)` that retail compiles to one walking pointer is a `__forceinline erase` calling a separate inline
  `copy(first,last,result)` helper (0x00886200).
- A stream count read with a value-returning `ReadLong()` is enregistered and spills to the dead stream-argument slot; a
  local that needs its own frame slot (the CRangeCompressor) is declared first at function scope (0x00B6D940).
- `cmp size,[remaining] ; jg` is the source test `size <= remaining` (0x0081B510).
- A display target saved through a temporary CRenderTarget reuses the ctor's eax; retail's shape is a scoped NAMED
  `CRenderTarget current(...)` assigned member-wise into the saved slot (0x00B89A20, 0x00B87680).
- A `CArray::Add(unsigned long value)` by-value wrapper around `push_back(const T&)` gives retail's "value in a register
  plus a memory copy for the reference" (0x00B91140, partial: brought 481v510 to 512v510).
- Float declaration order decides which local VC7.1 keeps on the x87 stack vs spills (0x00B61B30: the angle declared before
  u). It narrowed the row but did not match it.
- Lane gaps (do not retry blindly):
  - an inline-asm `fistp` floor helper (ebp frame, fistp/fild; 0x00B52990);
  - VC7.1's knowledge of that asm callee's registers keeps `this` in ecx across the call in the caller (0x00B53F60). A
    noinline stand-in body only gets to 415v411.
  - an empty `__true_type` tag temporary that retail places on a LIVE argument slot (`lea ecx,[esp+58h]` = the primitive
    parameter, read again later). The lane gives it its own frame byte (0x00B91140).

Review at landing:
- 0x00B3A180 models CEnginePrimitiveAttachmentManager locally (the list at +0x10, the CIVCountedPointer / handle classes,
  the attached-info record) although `rebuild/include/engine/CEnginePrimitiveAttachmentManager.h` exists. Its retail list is
  a 4-byte header-node pointer; check the header before retyping. It no longer redeclares `operator new(unsigned int)`
  (only a placement new).
- 00b91140 (parked) and 00b930a0/00b92070/00b92790 model CEnginePrimitiveRendererSharedData, CEngineOcclusionManager,
  CShadowClipVolume and CEngineInternalPrimitiveBase locally (the engine headers keep these members as opaque bytes).
- Rows graded more than 4 times: 00b3a180 (7), 00b930a0 (8), 00b92070 (2), 00b92790 (3), 00b91140 (5), plus the parked
  rows listed in NOTES.md (most at 5–12 probes).
- Landed file `rebuild/src/compiled/00/b2/CVBanditTollScript_Init_00b2fa40.cpp` is named after a wrong propagated label. It
  is a CEnginePrimitiveOcclusionManager member called on 0x01436E58; its own header comment already says so.
- 0x00464ACE (vector insert-overflow) is declared with `long` elements in the landed 0x005178A0. Here it is called for
  `unsigned long` (CArray<unsigned long> OcclusionTestHandles). The two are byte-identical, but the name binding differs.
