# Chunk 136 lessons (candidates for GOTCHAS)

- **Primitive constructors**: CEnginePrimitive* PDB signatures (`??0CEnginePrimitiveX@@QAE@_K...`) give the exact parameter order. The leading `_K` handle is never stored, and the `type` immediate names the class. The catalog swaps Light/ParticleLine/Line labels, so check the type constant. C3DVector needs a memberwise copy constructor (direct dword moves); the 48-byte matrix stays a POD (`rep movsd`); CRGBColour copies through the inline `Set(r, g, b, a)`.
- **`fldz ; fstp [mem]`** for a zero float member in a constructor marks a size TU (5 bytes against a 7-byte immediate store): `optimize("s", on)`, plus `("y", off)` when there is a frame pointer.
- **Secondary-base virtuals**: a FrameEnd whose offsets look like another class's is compiled with `this` at the secondary base. Find the subobject offset from the constructor's second vtable store (here +0x268) and model `Derived : PrimaryBase(0x268 bytes), CEngineComponent`.
- **Engine sub-primitives**: CEngineSubPrimitiveBase is `__declspec(novtable)` (Parent/Next stores before the derived vtable). The pool allocator 0x00BEB540 is `static void* __fastcall operator new(unsigned int)` (size in ecx).
- **Store sinking**: a zero-initialised local (an 8-byte chain) that retail stores after the first argument pushes needs any earlier-fetched object (the manager) read into a local BEFORE the chain is declared.
- **Call order**: `GFGetSystemManager()->member->Method(args)` with retail calling first and loading the member straight into ecx needs a local for the system manager. A local for the member itself goes through eax + `mov ecx,eax`.
- **Register choice through an accessor**: a counted pointer used as `ptr->field` (via `operator->`) picks eax where `ptr.Data->field` picks edx.
- **Self copy** `mov eax,[esp+N] ; mov [esp+N],eax` of a 4-byte record = a named local assigned from a default-constructed temporary of the same type (`desc = T();`); the two share a slot.
- **pack(1)**: unaligned word loads at odd offsets (+0xe) are a PDB member at an unaligned offset; reproduce with `#pragma pack(1)` and explicit pads.
- **Destructor release**: a counted member released with `dec [ecx+10h] ; jne ; mov eax,[ecx] ; push 1 ; call [eax]` = a pointer local plus `delete` through a virtual destructor (VC drops the null test because the local was just tested).
- **Wide memberwise copies** in a `__forceinline` constructor chain (a group with STL members) stay out of line unless the constructor is `__forceinline`.
- **Lane gaps seen**:
  - an empty tag homed in a dead by-value class argument slot (deque uninitialized_copy);
  - an empty tag over a live byte argument;
  - a pushed value and tag sharing a dead argument slot;
  - software-pipelined record copies;
  - x87 square order Y, Z, X;
  - ebx/ebp swap between a zero register and a local.
