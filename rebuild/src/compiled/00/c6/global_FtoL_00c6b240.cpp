// FtoL(float) -- round a float to long with the current FPU rounding mode
// Leaf: FtoL
// Retail-asm: 0x00C6B241 `fld dword [esp+8]` + 0x00C6B245 `fistp dword ptr [esp]` (4-byte slot, no _ftol2 call); VC7.1 C++ `(long)f` calls _ftol2 and /QIfist emits `fistp qword` with an 8-byte slot, so the bare dword fistp is the original's inline asm.
// Helper (2026-11-05 asm migration): the bare `fld` / `fistp dword` block is libpk's FtoL in the shared libpk/FtoL.h (byte-matching
//   path + FABLE_PORTABLE twin); the row has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family FISTP.
#define FABLE_LIBPK_FTOL_MACRO_ONLY   // this row defines the C symbol _FtoL itself
#include "libpk/FtoL.h"
extern "C" long __cdecl FtoL(float f)
{
    long result;
    LIBPK_FTOL(result, f);
    return result;
}
