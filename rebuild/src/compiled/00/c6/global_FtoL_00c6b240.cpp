// FtoL(float) -- round a float to long with the current FPU rounding mode
// Leaf: FtoL
// Retail-asm: 0x00C6B241 `fld dword [esp+8]` + 0x00C6B245 `fistp dword ptr [esp]` (4-byte slot, no _ftol2 call); VC7.1 C++ `(long)f` calls _ftol2 and /QIfist emits `fistp qword` with an 8-byte slot, so the bare dword fistp is the original's inline asm.
extern "C" long __cdecl FtoL(float f)
{
    long result;
    __asm fld f
    __asm fistp result
    return result;
}
