// FtoL(double) -- round a double to long with the current FPU rounding mode (__stdcall, ret 8)
// Leaf: FtoL
// Retail-asm: 0x00C89BE1 `fld qword [esp+8]` + 0x00C89BE5 `fistp dword ptr [esp]` (4-byte slot, no _ftol2 call); VC7.1 C++ `(long)d` calls _ftol2 and /QIfist emits `fistp qword` in the argument slot, so the bare dword fistp is the original's inline asm.
// Helper (2026-11-05 asm migration): the bare `fld qword` / `fistp dword` block is libvorbis os.h's vorbis_ftoi asm in the shared
//   libvorbis/vorbis_ftoi.h (VORBIS_FTOI; byte-matching path + FABLE_PORTABLE twin); the row has no asm of its own. C++/intrinsic
//   attempts: the batch ATTEMPTS.tsv, family FISTP.
#include "libvorbis/vorbis_ftoi.h"
extern "C" long __stdcall FtoL(double d)
{
    long result;
    VORBIS_FTOI(result, d);
    return result;
}
