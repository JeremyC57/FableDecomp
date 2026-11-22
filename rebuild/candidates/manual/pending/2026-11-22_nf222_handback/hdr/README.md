# NF222 header overlay (2026-11-22)

One header: `engine/CCountedPointer.h`, copied from the repo tree of today (base copy in `../orig/engine/CCountedPointer.h`, untouched since
Codex pass 26, mtime 2026-11-22 00:22). Two add-only, member-specific opt-ins; the default path is token-identical (each opt-in wraps the
existing text in `#ifdef X <declaration> #else <unchanged text> #endif`).

- `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY`: `static void FABLE_FASTCALL DeleteData(void* data);` declared only. Users: the five
  family Reset rows 0x00419428 / 0x0041947A / 0x004194D1 / 0x0042FC13 / 0x0042FC86. They only store the deleter's address (retail owners
  0x0041901D / 0x00419028 / 0x00419036 / 0x0042F864 / 0x00428AE7, each the virtual delete `test ecx,ecx; je; mov eax,[ecx]; push 1;
  call [eax]; ret`). T is only declared in those TUs, so the inline body emits a pick-any copy that is a plain `operator delete`, NEQ against
  the owner (measured: 0x0041947A's copy v 0x00419028). With the opt-in each row declares DeleteData as it did at HEAD, and no copy is emitted.
- `FABLE_RETAIL_CCOUNTEDPOINTER_CONVERTING_ASSIGN_DECL_ONLY`: the member template `template <class U> CCountedPointer& operator=(const
  CCountedPointer<U>&)` declared only. User: 0x0042DDB3. That row calls 0x0042FE34 (`??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@...`)
  out of line. Its frame-pointer TU (`optimize("y", off)`) emitted a pick-any copy with `push ebp; mov ebp,esp`, 26 bytes like the owner but
  NEQ. With the opt-in no copy is emitted.

pin_asserts.py: `engine/CCountedPointer.h pinned 0` (the opt-ins add no FABLE_STATIC_ASSERT; the one assert in the file did not move).
Header users: engine/CCountedPointer.h is reached by about 8,700 files under rebuild/ (grep). No file under rebuild/ defines either new macro
(grep of rebuild/ is empty), so the opt-ins change none of them. Each of the six rows includes engine/CCountedPointer.h itself, as its first
include, so the overlay copy is the one it reads (NF213 lesson).
Other overlays: no other agent's hdr/ under $S/w22 has a CCountedPointer.h newer than landing nf220, and no overlay defines DELETEDATA.
