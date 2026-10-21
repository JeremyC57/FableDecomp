# Chunk 265 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Defining a callee's landed body earlier in the same TU under `#pragma auto_inline(off)` lets VC7.1 use its register-preservation info. Here the caller keeps edx live across the call (0x00557EB0 / CanRedefineAction 0x00557960).
- A class returned by value and then passed by const-ref needs a user-declared copy ctor (C2DBoxF, 0x0082BE60). Without one, VC copies the result into a second temporary and hoists the call above the earlier argument pushes.
- Implicit `const char*` -> CCharString argument conversion builds the temporary before any push, as retail does. An explicit `CCharString("..")` builds it after the later arguments (0x008DB620, 0x0082BE60).
- `p.m_p->F()` instead of an inline `operator->` stops VC preloading the object into ebp and the extra zero register (0x008DB620).
- `long v = A; if (!c) v = B;` gives retail's store-then-overwrite; the ternary gives neg/sbb/and/add (0x0082BE60).
- A discarded inline by-value getter, with AddRef/Release members on the pointee, gives the unfolded `inc [ecx+4]; dec [ecx+4]; jne` pair. A named copy lets VC fold it (0x0082BE60).
- Store-order rotation: write the final member stores in the order retail emits them (`Running = false;` before `*next = ..`) (0x004189C2, as on 0x00687540).
- An inline by-value `ToString(long)` (local, Format, copy out; no NRVO) reproduces the temporaries. CWideString::Format 0x0099BA70 is a varargs __cdecl member: `this` is the first stack argument (0x00829A30).
- Bool argument from a byte global: `if (g) f(g, 1, true)` re-reading the global gives `xor eax,eax; mov al,[g]`. A named bool local spills to the frame (0x004189C2).
- Early returns placed before the callee-saved pushes make VC duplicate epilogues; one nested condition keeps a single epilogue (0x0095FB40).
- /GS: a char-array local needs the /GS flag (cookie 0x0139C8A8, check 0x00BFE9F9) (0x00687FD0). GetAsyncKeyState goes through the import thunk 0x014402CC.
- A named C2DVector local gives retail's `lea` argument; a temporary does not (CInputProcess* rows).
- Named-float plane locals, a clamp macro and a single `float z`, and constant multiplies split as retail evaluates them reproduce the CMap height functions (0x0081FEC0, 0x008F7630).
- `&*Messages.begin()` and a cached `prev` node in list/tree insert reproduce the retail address arithmetic (0x00A64740, entity scripts).
- Wide strings are `const unsigned short*` (the engine's wide type, as in the landed CWideString sources), not wchar_t (0x00829A30, 0x00405020).
- A twin landed body is the fastest model: 0x00416C8A matched by copying CNewFrontendGameComponent::InitialiseGraphics 0x0042DDB3 in a size TU.

Review at landing:
- Rows that took more than 4 grades: 0x00557EB0 (re-opened from differ), 0x0082BE60, 0x00829A30, 0x004189C2, 0x008DB620 (parked), 0x00A09F20 (parked), 0x00537776 (parked), 0x0095FB40 (parked).
- Local models next to existing engine headers:
  - CMiniMapDisplay (0x0082BE60 uses a retail-layout local model: MapPrimitiveHandles +4, MiniMapGraphic +0x1C, flags +0x40..+0x43).
  - CManager (0x0041DF10 / 0x0041E3F6).
  - CInputProcessMain / CInputProcessInventoryBase (slot-9 accessor modelled locally).
  - CLandscapeBackgroundTreeNode (Ego_r.pdb offsets - 8).
  - Reconcile them with rebuild/include/engine/*.h.
- 0x00557EB0 carries the CanRedefineAction 0x00557960 body in the TU. At landing, either land 0x00557960 from it or keep it as a non-emitted helper; it must not duplicate a landed symbol.
- The many propagated wrong labels in NOTES.md (22 of them) should go into ghidra_out/labels_*.tsv.
- differ/00ef4320.cpp and probe/ef43_*.cpp contain the retail script-API name `IsHeroNaked` (slot 188): no lowercase forbidden token, but a case-insensitive scanner would flag it.
