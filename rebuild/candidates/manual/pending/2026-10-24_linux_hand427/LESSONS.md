# Chunk 427 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Overloaded virtuals are laid out in REVERSE declaration order. To get SetChildPrimitive(id, prim, flags) at slot 7 and
  (id, handle, prim, flags) at slot 8, declare the 3-argument overload first (00b31f20, 00b31f70).
- A same-TU `__declspec(noinline)` read-only LowerBound body under `auto_inline(off)` lets VC7.1 keep the caller's key in ebx
  across the call. With a declaration only, the key is reloaded and pushed again (DIFFER 87v86) (00b15b50, 00b059f0).
- STLport `insert_unique`: spell `_M_insert` as `iterator* _M_insert(iterator* result, x, y, v, w)` and call
  `*_M_insert(&j, ...)` so the result shares j's slot. The by-value form adds a `push ecx` temporary (177v175) (00b2f130).
- CVectorMap::Insert must be `__forceinline` in the CollectUpdateParameters callers, else it is called (73v159) (00b1b380).
  Compute the CRC key into a named local before the descriptor temporary when retail calls CRC first (00b0f210, 00b158a0).
- A C3DVector local that retail keeps in frame words (stored, then re-read for the copy-out or across a malloc) needs
  C3DVector's memberwise USER copy ctor. With the implicit copy VC7.1 scalar-replaces it into ebx/ebp/esi
  (00b081a0 166v164, 00b10360 113v136).
- Inlined STLport list push_back: the list iterator needs its declared copy ctor (GOTCHAS retry 10i), even in a
  4-statement body. Without it VC7.1 sinks `push edi` past a `new T(...)` null-check join and gives the early exit its own
  epilogue (00b2c460, 121v121). Another list push_back with no callee-saved register after the branch (00b081a0) matched
  without it.
- `__uninitialized_fill_n` / `_M_insert_overflow` over an element whose implicit copy runs over C3DVector user copy ctors
  (CEngineCamera::CDebugScriptElement, 32 bytes) gives retail's second `lea esi,[eax+14h]` induction register. In the overflow,
  the fill_len == 1 arm is an inline `_Construct`; same-TU noinline bodies for `__uninitialized_copy` / `__uninitialized_fill_n`
  match first try (00b31a90, 00b31b10).
- In a vector<T>::_M_insert_overflow where T's copy ctor is out of line (CCamera 0x0049D380), the inline `_Construct` calls it
  after the placement null test. A wrong member layout shows up first as the size-divide magic (0x92492493 = /56) (00b29840).
- `T1 operator+(const T1&) const` member operators: VC evaluates the argument expression before the object expression, so
  `*this * (1 - t) + c * t` computes `c * t` first (00b09b90, CRGBFloatColour::Interpolate).
- A `#include` of a scratch `.inc` file makes the Wine cl.exe spin to the 200 s timeout (q.py compiles a copy elsewhere).
  Inline shared models into every source (00afe4b0).
- TLC float->long conversions in the particle engine are an inline-asm GFFloatToLong (`fstp [t] ; fld [t] ; fistp dword`,
  or the `(float, long*)` form `lea eax,[esp+0Ch] ; mov edx,[esp] ; fistp dword ptr [edx]`, or a ceiling variant with an
  ebp frame). Every row with that pattern is a retail_asm candidate (00af8d50, 00afe380, 00b05c20, 00b09aa0, 00b0a420,
  00b0a4a0).
- CDynamicObjectHeap<156,2100>: a 2100-trip free-list loop (divisible by 6) is unrolled six ways. VC7.1 RTM schedules the
  first link store as `mov [eax-9Ch],edx`, where retail forms `lea ecx,[eax-9Ch]` first (00af87c0, parked). The 529-trip
  <116,529> twin 0x00AF8B40 is not unrolled and is landed.

Review at landing:
- Rows that needed 4 grades: 00b2c460 (matched on the 4th) and 00af87c0 (parked after 4).
- Local base models where an engine header exists (the header has PDB offsets): 00b262c0 uses a local CEngine
  (RenderLayers retail +0x15C vs PDB) and 00b2fc10 a local CEngineCamera (DebugScript +0x208 / CurrentDebugScriptElement
  +0x214 / DebugScriptRunning +0x218 / start time +0x220). 00b2c460 uses engine/CEngineSurfaceCompositionManager.h but casts
  its 12 opaque CachedSurfaces bytes to a 4-byte STLport list, and the manager to its CResourceBank base.
- Already-landed source errors: rebuild/src/compiled/00/b1/NParticleEngineCPSCSpline_StartUpdate_00b1b0c0.cpp names
  0x00B215E0 "UpdateSplineData" (it is CPSCSplineBase::StartUpdate, vtable slot 8) and 0x00B216F0 "CalcDistanceAlpha" (it is
  CSplinePosParamData::GetScaleFactor).
- Manifest labels to fix outside the chunk: 0x00B31700 (CEngineCamera ctor, labelled CAnimInit::Clear), 0x00B317A0
  (CEngineCamera::ConsoleAddDebugScript, labelled CTCActionUseReadable::OnCreate), 0x00B31C10 / 0x00B2AA70 (vector push_backs),
  0x00B31C70 (CEngineCamera deleting dtor), 0x00AFE910 (vector<CArray<long>>::resize). Full list in NOTES.md.
- The six retail_asm candidates qualify for the 2026-10-24 retail_asm grade (C++ body + minimal `fistp` __asm +
  `// Retail-asm:` line). This lane bans __asm, so they hold C++ reference bodies only.
- Process note: during authoring I stopped one of my own stuck shell loops (a `until ! pgrep -f ...` wait that matched
  itself) by its explicit PID. No other process was touched.
