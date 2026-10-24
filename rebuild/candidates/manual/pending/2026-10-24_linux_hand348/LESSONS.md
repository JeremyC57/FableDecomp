# Chunk 348 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `_Construct(T* p, const T& v)` out-of-line instances are `void __fastcall _Construct(T* p, const T& v) { if (p) new (p) T(v); }` with a local `inline void* operator new(unsigned int, void*)`; wrap each TU in `namespace N<addr>` so identical element types (CSymbolInfo x3) stay one-address-one-symbol (009e5f60, 00a3fb60, 00cd4e30).
- VC7.1 /O2 will NOT inline an implicit copy constructor into `_Construct` once the record has ~3 non-trivial members or a long scalar tail: ours is the 11-byte `test ecx,ecx ; je ; push edx ; call`. Spell the memberwise copy ctor out as `__forceinline T(const T& o) : a(o.a), ... { }` (00565780, 00566180, 006b6670, 006e4bd0, 009f8780, 00cb60b0); a plain (non-forced) user copy ctor stays out of line too.
- A trivially copyable 0x30-byte record copies with `rep movsd`; retail's unrolled `mov eax,[edx+N] ; mov [ecx+N],eax` chain means a non-trivial memberwise copy (members with user copy ctors, e.g. C3DVector, plus a forced-inline spelled-out copy ctor) (0069c860, 00913590).
- `[esi+4],[esi+8],[esi+0xc]` fixed-offset moves of a C3DVector member after a string copy need C3DVector to have a user memberwise copy ctor; with the implicit one VC re-bases (`add edi,4 ; add esi,4`) and block-copies (0077e250, 0088ab90).
- Retail STLport vector / CArray members are 12 bytes (PDB says 16): CVectorMap = vector base + empty Compare (+0xC) + bool Dirty (+0xD); its implicit copy is base copy ctor + one byte (00a85360).
- `std::__uninitialized_fill_n` without the `__false_type` tag (`ret 4`, ecx = first, edx = n): `for (; n > 0; --n, ++first) if (first) new (first) T(x); return first;` -- unsigned `test edx,edx ; jbe`, cursor in eax (009fffc0, 00b6bba0).
- `delete p` of a class without a virtual destructor is a bare `push ; call operator delete ; add esp,4` (no null test); a private inline Reset() inlined at the end of Shutdown turns its static call into a tail `jmp` (004051c0).

Review at landing:
- rebuild/src/compiled/00/40/CIME_Initialise_00405650.cpp declares 0x004051C0 as `NativeCIME::Reset()`; the body is CIME::Shutdown() (PDB ?Shutdown@CIME@NUISystem@@QAEXXZ, Reset inlined). Same address, two names -- rename the declaration.
- Local models next to existing engine headers: 004ebdf1 (engine/CParentDefClassBase.h flattens the vptr into a packed 0x3D body, unusable as a polymorphic base), 004051c0 (rebuild/include/fable_ime.h keeps CIME's members private and declares no Shutdown/Reset; local class uses the PDB names, same layout).
- 0082c5e0 uses engine/CMarkerInfo.h, whose PThing is opaque bytes: the CIntelligentPointer copy is placement-built on them via reinterpret_cast.
- 00913590 models CResourceEntry with one extra trailing word (`RetailExtra`, meaning unknown): retail stride 0x30 vs PDB 0x2C.
- 00beeda0 (parked) is a scheduling-only diff; compare with the landed __pop_heap siblings 0x00CBBA60 / 0x00CBBAD0 for a model tweak.
