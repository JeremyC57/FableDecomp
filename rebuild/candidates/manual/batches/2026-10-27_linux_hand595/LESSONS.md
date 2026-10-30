# Chunk 595 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A file-static callee-pop helper (DX SDK ImeUi.cpp GetReadingWindowOrientation 0x00CB3420) only reaches retail register
  allocation as a TU-local `static __declspec(noinline) __stdcall` WITH a same-TU caller; the global form is 330v333.
- STLport 4.6 `_M_insert_overflow(__false_type)` (0x00B80400): the `__uninitialized_copy` / `__uninitialized_fill_n` helpers
  must be `__forceinline`; plain inline leaves them out of line (226v333). `_M_clear` stays a separate out-of-line call.
- Twin bodies split on callee visibility: PKCTRRedblackTree::DeleteFixup 0x00C18C00 keeps `this` in ecx because the rotation
  bodies are compiled above it in the TU (`#pragma auto_inline(off)`), PKCTRRedblackMap::DeleteFixup 0x00C1B560 keeps it in
  edi because they are only declared. Check the twin's register for `this` before choosing.
- `p->Alloc()->Init(a, b++, c)` evaluates the arguments before Alloc; retail calling Alloc first then the `m_count++`
  arithmetic means a local: `T* t = p->Alloc(); t->Init(...)` (0x00C131C0).
- In an EH-frame function, `if (!x) return a; return b;` puts the state `-1` store before the branch; the equivalent ternary
  puts it in the join after the branch (0x00C131C0 tail).
- A 16-bit mask merge that retail does in 32-bit registers with no zero-extension (`and eax,edi; not edi; and edi,edx;
  or eax,edi`) comes from an inline helper with `unsigned short` parameters; the inline `(unsigned short)(...)` expression
  zero-extends each call result (0x00C20580, still DIFFER on register choice).
- strmbase CDeferredCommand::Invoke 0x00CAED50 inlines CBaseDispatch::GetTypeInfo and references its function-local statics
  directly: carry the landed 0x00CACD90 body in the TU.
- Tooling: lane_tools/rstr.py and iat.py hardcode another session's scratchpad path; a local pefile reader works.

Review at landing:
- 0x00CB3420 keeps a reduced GetReadingString stand-in caller (`?GetReadingString@@YGXPAUHWND__@@@Z`) in the TU, needed for the
  callee's codegen; make sure it is not bound or claimed for retail 0x00CB4340 (the real one is a separate, larger static).
- 0x012BEE90: the landed and kept spelling is the C++ `?DSPROPSETID_EAX30_ListenerProperties@@3UGUID@@B`; EAX's eax.h uses
  DEFINE_GUID, which would make it `extern "C"` `_DSPROPSETID_EAX30_ListenerProperties`. Decide once and respell both
  0x00C568F0 and 0x00C71820 together. The placeholders g_eaxListenerGuid / g_eaxListenerPropertySetGuid_012bee90 are wrong.
- 0x0143E8B8 carries both `_g_hklCurrent` and `?g_hklCurrent@@3PAUHKL__@@A` (ImeUi.cpp globals are C++ statics: the C++ one).
- vland SHARED_HEADER_AVAILABLE CBaseObject on 0x00CAED50 is a name collision (strmbase vs Lionhead CBaseObject); keep local.
- 0x00C12CD0, 0x00C29EC0, 0x00C29F70 have conflicting placeholder spellings from propagated combat / hero / trail labels on
  libpk audio code; NAME_FIXES gives one descriptive spelling each.
- No row needed more than 4 grades; 0x00C5F6A0 used its budget (parked at 327v327, scheduling only).
