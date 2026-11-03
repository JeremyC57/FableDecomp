// FtoL(double) -- round a double to long with the current FPU rounding mode (__stdcall, ret 8)
// Leaf: FtoL
// Retail-asm: 0x00C89BE1 `fld qword [esp+8]` + 0x00C89BE5 `fistp dword ptr [esp]` (4-byte slot, no _ftol2 call); VC7.1 C++ `(long)d` calls _ftol2 and /QIfist emits `fistp qword` in the argument slot, so the bare dword fistp is the original's inline asm.
#include "fable_x87_twins.h"
extern "C" long __stdcall FtoL(double d)
{
#ifdef FABLE_PORTABLE
    return fable_portable::X87FistpNear(d);              // fistp dword under RC = nearest-even (fable_x87_twins.h)
#else
    long result;
    __asm fld d
    __asm fistp result
    return result;
#endif
}
