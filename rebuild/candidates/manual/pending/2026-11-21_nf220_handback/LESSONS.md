# NF220 lessons (one line each)
- A CCountedPointer ReleaseData family closes over every T a multi-type binder releases, and over the self leaves of stand-in ShareData / Reset / operator= rows: run callers.py on each new address until the set stops growing before planning (q12226: 18 rows -> about 40).
- A speed-shaped release body with no jmp thunk (0x00403750) can still be ReleaseData: grade it as the ReleaseData explicit instantiation without the size pragma; its /Os callers settle the binding.
- RELEASEDATA_DECL_ONLY is a one-line move for rows already on engine/CCountedPointer.h (8 of 8 graded); the pick-any ??1<T> copies it emits are the 5-byte jmp thunks.
- c2 keeps a local's field in a register across calls only when the address-taking callees' bodies are in the TU: header-only ports of a CWideString TU-mate fail until operator+= / MakeOwnCopyOfData / AllocStringData are visible (a dummy body is enough to prove it).
- Reach a header class's protected data through PDB-attested members (CWideStringData ctor / Append(wchar_t)) as declaration-only opt-ins, never an access hack.
- `#pragma auto_inline(off)` around same-TU bodies does not change which inline header COMDATs an object carries; check the symdiff for new pick-any copies and their table lines.
- Moving a row-head optimize("s") after fable_string.h (for a size-body opt-in) recompiles the header's inlines in command-line mode: comdiff every function COMDAT; here one differing copy (__copy_trivial) came to equal its owner.
- Including fable_string.h ahead of the engine headers can lose macros those headers set first (0x005A04F7 SRC_FAIL): check which header defines what before including first.
- Source classification by opt-in macro is a first cut, not a census: 0x00596A68 read as "default" binds ReleaseData; compile every row the plan moves and read its relocations (xref.py), and recheck a table witness line against today's object (0x00570332's ??1 line was stale).
