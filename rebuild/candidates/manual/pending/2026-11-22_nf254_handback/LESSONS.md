# NF254 lessons
- A pick-any template COMDAT takes the optimize mode in force at the END of the TU, and so does an EXPLICIT INSTANTIATION (a row-tail
  `#pragma optimize("s", on)` turned the 249-byte explicitly instantiated leaf into a 172-byte /Os body). To give one implicit
  instantiation the owner's /Os body without touching the leaf, put an explicit SPECIALIZATION of it (a non-template function, compiled
  in place) between `optimize("s", on)` and `optimize("", on)` in a header opt-in; it still expands inline into the /O2 leaf.
- A compiler-generated member (the `??_G` behind `p->~T()`) cannot be specialized: only the end-of-TU mode decides its body. Look for a
  HEAD object that already defines the same body under the same name before calling it a new variant (cmpobj).
- A header opt-in inserted mid-file shifts `__LINE__` for every later line; tests that `#define abort() fixtureAbort(__LINE__)` then
  differ in equiv_check. Append new opt-ins at the end of the file (inside the closing namespace) when placement does not matter.
- A TU-mate whose HEAD object already emits the class's vtable / `??_G` / `??1` is the template: copy its define / include block and the
  harness rows match first time (0x00A90AE0 -> 0x00A978A0 / 0x00A978C0).
- A binder that "already binds the right spelling" can still define a stand-in BODY under that spelling (0x008ED440's 53-byte
  `_M_insert_overflow`): once the self row defines the real body, the two pick-any copies compete. pk.py every binder's HEAD object.
- sed `s/[ra]//g` on census output strips hex digits `a`; strip only a trailing suffix (`s/[ra]$//`).
