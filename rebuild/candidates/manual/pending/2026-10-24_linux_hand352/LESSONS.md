# Chunk 352 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Member intelligent pointer read through a pointer: an inline accessor on the pointee class
  (`CThing* GetWorkBuilding() const { return WorkBuilding.GetPItem(); }`) gives `lea ecx,[eax+170h]`. Chaining
  `p->WorkBuilding.GetPItem()` at the call site gives `add eax,170h ; mov ecx,eax` instead (0x006C1C70).
- Compiler-generated destructor as the target: emit it with a qualified call in a helper,
  `void Destroy(T* p) { p->T::~T(); }` (a typedef works for templates: `e->TPair::~TPair();`), plus `// Leaf: ??1T`.
  The helper is not part of the retail body (0x004420D0, 0x006F48A0, 0x00A326D0).
- Implicit default constructor as the target: emit it through placement new,
  `inline void* operator new(unsigned int, void* where) { return where; }` then `new (p) T;` (0x004A7710).
- Leaf names use the exact mangling: a struct template argument is `U`, a class is `V`, an enum is `W4`
  (`??1?$pair@JUC3DMeshVolume`, `??1?$pair@VCCharString`, `??1?$pair@W4EPSCType`). When nothing matches, q.py silently
  grades the largest function; check `objdump -t` on the .obj (0x00A326D0).
- STLport count_if is `long __stdcall` (`ret 0Ch`); a loop that enters with a `jmp` to the bottom test and has no
  alignment padding is a size TU: `#pragma optimize("s", on)` (0x006600EC, 0x0065F6A6).
- NPlayerGui CDraw region (0x0064xxxx-0x0065xxxx) frame-pointer bodies: `#pragma optimize("y", off)` + `("s", on)` (0x00657037).
- `CIVCountedPointer<T>::Attach` in the size form: `#pragma optimize("s", on)`; the tell is `and dword ptr [esi],0`
  (0x00655230, 0x00CD23DA).
- Vector of elements with a virtual destructor whose destroy loop is inline (`mov edx,[esi] ; push 0 ; mov ecx,esi ;
  call [edx]`, `last` held in ebx): write STLport's `template<class T> inline void _Destroy(T* first, T* last)
  { for (; first != last; ++first) first->~T(); }` and call it from `~vector()`. A loop on `_M_finish` re-reads the member
  after every virtual call (+2 bytes) (0x004420D0).
- No vtable store in a destructor of a polymorphic class means neither it nor any base declares a destructor. Check the
  vtable: NPlayerGui::CDrawBase (0x0125A52C) has only ClearPrimitivesIfNecessary + PrepareForLoad and no virtual dtor
  (0x004420D0).
- CBaseIntelligentPointer's slot 0 is a virtual destructor (??_G thunks such as 0x006548A5 call 0x00A01C10). Declare
  `virtual ~CBaseIntelligentPointer(); // 0x00A01C10` and leave CIntelligentPointer<T> with no destructor:
  - a member of a complete object is destroyed with a direct call to 0x00A01C10;
  - an element destroyed by an explicit `p->~T()` goes through the vtable with `push 0`.
- A base with an inline user `virtual ~Base() {}` (CDefPointeeBase) adds the closing base-vtable store in derived
  destructors and temporaries (0x007D26E0).
- STLport list destructor in two shapes:
  - inline: `~list() { clear(); if (_M_node) free(_M_node); }` with `clear()` out of line gives `lea edi ; call clear ;
    mov edi,[edi] ; test ; push ; call free`;
  - fully out of line (`~list(); // 0x004AB420`), shared by several list<T*> types through `jmp`.
  (0x004AC800)
- Record destructors whose vector members use the landed 0x006FD040 form (a `__fastcall` DestroyRange with an
  `SNonscalarPtrIteratorTag()` temporary by reference, then `if (_M_start) free(_M_start)`) match first try. The tag
  occupies the high byte of the `push ecx` slot (`lea ecx,[esp+7]`) (0x00AB34C0, 0x00AECB80, 0x006F48A0).

Review at landing:
- No row needed more than 4 grades (two rows needed 2: 0x006C1C70, 0x004420D0).
- Local models sit next to existing engine headers, because engine/CIVCountedPointeeBase.h and engine/CBaseObject.h
  flatten their vptrs into plain data members:
  - local polymorphic CIVCountedPointeeBase in 0x004AE3E0, 0x00655230, 0x00696560, 0x007550C0, 0x00ABEFA0, 0x00AC6210
    and 0x00CD23DA;
  - local CBaseObject in the count_if rows 0x006600EC, 0x0065F6A6 and 0x006C1C70.
  Switch them to the headers once those model the vptr.
- Already-landed sources seen while reading relatives (0x0056C820, 0x004AF220, 0x004C34A0, 0x00C4B040) use weak
  local models and names. Re-check them against the engine headers and PDB names.
- The landed CSurfaceCompositor ctor declares a user `virtual ~CSurfaceCompositor()`, but the retail destructor
  (0x00B2E6C0) is compiler-generated in shape. Harmless for the ctor bytes, but the two models disagree.
- The landed CPlayerGui::PrepareForLoad (0x004382C0) models NPlayerGui::CDrawBase slot 0 as `virtual ~CDrawBase()`. Retail
  slot 0 is ClearPrimitivesIfNecessary (0x00644627) and the class has no virtual destructor (vtable 0x0125A52C). Only slot
  1 is called there, so the bytes are unaffected, but the comment is wrong.
- The landed CDrawMeleeActionButtonEffects ctor (0x00654258) calls its emitter vector "VC6 Dinkumware". The destructor
  (0x004420D0) shows the STLport three-word form with inline `_Destroy`, like every other vector in this chunk.
- No catalog row in this chunk held several functions. The chunk `length` column includes trailing int3 padding on 15 rows
  (finish.py strips it).
