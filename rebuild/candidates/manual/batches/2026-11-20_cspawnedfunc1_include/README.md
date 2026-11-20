# Header fix (Account A, 2026-11-20): engine/CSpawnedFunc1.h includes fable_compiler.h

NF162 reported that engine/CSpawnedFunc1.h defines FABLE_SPAWNEDFUNC_INLINE as FABLE_FORCEINLINE without including
fable_compiler.h, which defines that macro. Any includer that had not already pulled in fable_compiler.h failed to compile
(C2143 / C2501).

- Change: one `#include "fable_compiler.h"` before the CSpawnedFuncBase.h include. fable_compiler.h is guarded, so includers that
  already had it see no change, and the token reaching cl is still `__forceinline`. The header has no static asserts, so the
  shifted line numbers move nothing.
- Recheck (VC7.1, byte match + behaviour test) of all 67 landed rows that include the header (`includer_rows.txt`; no other
  header includes it):
  - before: 62 good, 5 SRC_FAIL (0x00D16460, 0x00E0DAA0, 0x00E76920, 0x00F09420, 0x00F09530; `logs/before_bad.tsv`).
  - after: 67 good, 0 bad (`logs/after_summary.txt`).
