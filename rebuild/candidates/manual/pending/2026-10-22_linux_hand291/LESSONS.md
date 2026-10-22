# Chunk 291 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU stand-ins can fix the frame layout.**
  - A `#pragma auto_inline(off)` noinline template body for GetSubDef that writes only `out.m_p` lets VC sink the def zero-store after the PDef load (008d9e30, 008dd7e0).
  - Advisor C1 extended this on 008e0060 with three more stand-ins, plus the push_back tag written as an uninitialised, ctor-less `__false_type isPOD;`. The stand-ins are both `_M_insert_overflow` instantiations and CalcAndLookup (writing through its long*). Neither change works alone; together they collapse the two tag slots into one.
  - The CalcAndLookup stand-in alone also matched 008ddd70: placed ahead of the GetSubDef stand-in, it moves the `effect = 0` store after the sub-def zero store. On 008e03b0 the trio keeps the vector finish cached in ebx; on 008daa80 it did nothing.
  - A same-TU GetPosition stand-in makes things worse: VC then hoists loads across every GetPosition call.
- **Frame-slot order is not declaration order.** Re-declaring floats in another order never moved a slot (0085c470, 005542f0).
  - What did move slots: turning two floats into one struct copy (`C2DVector speed = Speed;`) or into a struct local (`C2DVector size;` in CMovie::Draw).
  - Also: putting a section in its own `{ }` block, so later locals and temporaries can reuse its words.
  - Also: adding a `T* p = m_p` local in a dtor, which moved CDefPointer from 0x14 to 0x1c.
- **Overloaded virtuals change the vtable layout.** Two virtuals with the same name (AddPrimitive) were laid out in reverse slot order. Give each one a distinct name in local models, or the slot offsets come out swapped (CMovie::Draw: 0x5c/0x60 vs 0x70).
- **x87 fadd operand order follows single-definition operands (advisor, 00547f60).** In a commutative fadd with two memory operands, VC7.1 loads the operand that has a single definition first. Swapping the operands in source never changes the order. An in-place `size.X *= Scale.X` gives size several definitions, so pos was loaded first. The fix: give the multiple definitions to a separate `extent` vector and build `C2DVector size(extent.X * Scale.X, extent.Y * Scale.Y)` with a ctor. The C2DVector user copy ctor also had to go.
- **Inline accessors reorder argument evaluation.** VC7.1 evaluates call-shaped arguments first, so `GetParentThing()` instead of `ParentThing` moves a member load ahead of a GetPosition call (advisor C1, 008dd7e0).
- **Box coordinates via named corner objects.** Building C2DBoxI from named C2DCoordI corners (`lower`, `upper`) gives in-place `add ebp,eax` where the four-argument form gives `lea` (008dd7e0).
- **CAnimInit.** 0x5B33F4 is the out-of-line CAnimInit 7-argument ctor.
  - A temporary `CAnimInit(...)` passed as an argument gives retail's late zero stores.
  - A named `CAnimInit init; init.Set(...)` covers the other call site.
- **CCharString temporaries.** `CCharString idle("ST_IDLE"); name = idle;` gives `lea ecx; push ecx`; a direct assignment pushes the ctor's eax instead.
- **`find == end` reuse.** An inline `IsNotIn(first, last, v) { return Find(first, last, v) == last; }` lets VC reuse `last`. The iterator tag must be a named local, because `T()` zero-initialises.
- **Ternary that keeps retail's branch.** `index = count ? r % count : 0` keeps retail's `test; je; div; ...; xor`.
- **C3DVector assignment.** Leave C3DVector's copy assignment compiler-generated; a user operator= changes the final block copy's addressing (advisor A3, 0085c470).
- **Placement new in tests.** A test that copies a source declaring `operator new(unsigned int, void*)` must guard it with `#ifdef _MSC_VER` and include `<new>` under g++, where size_t is 8 bytes.
- The shape of a call chain can drive the frame-slot order. On 005542f0 (advisor RST2), moving from a free inline `GetFirstFrame(const unsigned long&)` with a local bank copy to a by-value `GetGraphicBank()` plus a bank-member `__forceinline GetFirstFrame(unsigned long)` fixed the slot order, the map pointer register and the `sbb ; and` ternary. None of the loop-variable spellings had changed any of those.

Review at landing:
- The CalcAndLookup stand-ins in 008e0060 and 008ddd70 have a synthetic body (`*value = text[0]`). Only the call shape matters for the grade, but a reviewer should confirm the landed form keeps the call out of line without inventing behaviour. The tests rely on the stand-in result.
- 00547f60 calls WaitForSingleObjectEx / EnterCriticalSection / LeaveCriticalSection through direct IAT imports, and declares a local CRGBColour default ctor plus a body-assigning copy ctor. Check these against the shared headers before landing.
- 00862780 restates CModeStandBase locally, because the engine header is a flattened pack(1) record (IdleCounter +0x1c, ForceAddNewAnims +0x24, PoseAnimName +0x34).
- 008e0060 and 008d9e30 contain same-TU noinline stand-ins for callees whose retail bodies sit in other TUs. They are compiler-emitted C++, but check that the verifier's strict mode accepts the `auto_inline` pragmas.
- 00eb15c0 is landed as a CMurderTwistSheriff method although the row label says CMurderTwistAssassin.
- 0x00BFEA14 is `free()`.
- LowerBound 0x4365B0 goes through the const overload (00862780).
- For a CThing const getter, use GetPosition 0x4C73D0, not 0x6A4D00.
