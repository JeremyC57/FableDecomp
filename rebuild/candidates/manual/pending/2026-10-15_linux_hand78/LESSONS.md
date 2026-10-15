# Chunk 78 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A loop-invariant load retail keeps across a call (009e50c0, the key's data block) reproduces with the end pointer and
  the key block in locals read inside the guard of a `do/while`; a `for` loop puts the load above the guard.
- An inline map Find reads `end()` once into a local and uses it as the default result:
  `(it == last || key < it->first) ? last : it`; the caller re-reads `end()` itself.
- A value multiplied as a register after a call comes from an inline getter (GetPitch's `GetWidth()`); a direct field read
  multiplies from memory after the call.
- `size = sizeof(T); if (p) size += f() + k;` = the hoisted `mov eax,8` with `add eax,0Ch` on the taken path (a ternary gives separate arms).
- An `if` nested inside the `&&` test (not one longer `&&` chain) stops VC hoisting a tail store above the second branch (SetAsActive).
- The DirectShow SDK base classes match verbatim: COM methods are `__stdcall` with `this` at the IBaseFilter sub-object
  (member offsets shift by -0xc). 00ca77f0 / 00ca7930 / 00ca8b50 are CBaseFilter::QueryFilterInfo, CBaseFilter::NotifyEvent,
  CBaseInputPin::BreakConnect.
- CSurface needs a local polymorphic model for its copy constructor (the header's `__vftable` data word cannot express the
  vtable store); the other rows use `engine/CSurface.h` through a methods struct.
- The two CSuspendableProcessMicrothread sources need `#pragma optimize("", off)`.

Review at landing: the landed 0x00C35F20 types its second parameter as bool but its setter (0x00C445C0) reads a float
(bytes identical; fix the model). Parked 004e06c3 is really CShopDef::CShopDef with the "24i" shape: retry with the
STLport `_Vector_base` model (hand75 LESSONS). 009f31c0: try an inline `GetPixelAddress` helper on the locked rect.
