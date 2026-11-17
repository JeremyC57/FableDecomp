# NF79 lessons (round 7)

- An out-of-line STL COMDAT (deque back / pop_back) re-authors as an explicit member instantiation, with the STLport body behind a per-TU
  header opt-in and its own out-of-line helpers declared only; graded RM first time (0x0059B039 / 0x0059AEE5).
- A template member defined in-class under an opt-in and explicitly instantiated in the row (`template void FABLE_FASTCALL
  CTBaseSingleton<CFileInstaller>::Release();`) also works for static members (0x00404490).
- A tail `jmp` stub (0x0059AAE3 / 0x0059AC1A: 5 bytes, `jmp 0x0059A8F3`) is the out-of-line operator- of a deque iterator; the target is
  the STLport _M_subtract, not operator-.
- Before adding an opt-in, grep the header for the member: CEngineSubPrimitiveBase.h already had the private static MemoryPool and
  CBankFile.h already had READENTRYDATA; my duplicate cost a SRC_FAIL each.
- A call `p->Member()->F(arg)` evaluates the argument before the object pointer; retail that loads the object first needs the pointer in a
  local first (0x00894AA0: DIFFER 232v232 until `CMeshDataBank* bank = World->PMainMeshBank_Data;`).
- IS_THING_ALIVE on an intelligent pointer: retail fetches GetPItem once for the null and dead tests; `p.GetPItem() != 0 && !p.Get()->IsDead()`
  fetches twice (DIFFER 181v174). Use the one-fetch inline helper on the pointer, as the landed rows do.
- `#pragma auto_inline(off)` at the end of a row also governs the template members instantiated at the end of the TU (Get() went out of
  line, DIFFER 777v755); close it with `#pragma auto_inline(on)`.
- When the shared CCountedPointer<T> replaces a non-template stand-in, VC7.1 may cross-jump identical `out = x; return true;` tails that it
  kept apart before (0x00A25E70, blocked): budget for it, or try the field read (`.Data`) the stand-in used.
- Tests that link a row's stand-in names fail LINK_FAIL after a respelling; rewrite them on the new mangled names (link stand-in classes in
  the test are fine) and keep the PassPattern.
- Rows that include shared headers under renaming macros (0x00B90270) or that pass a respelled type into several other stand-ins
  (0x00B82100) cannot be respelled one callee at a time; name the classes to move first and block early.
