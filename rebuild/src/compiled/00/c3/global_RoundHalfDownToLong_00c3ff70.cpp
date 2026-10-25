// _global::RoundHalfDownToLong(float value) -- subtract one half and round to an integer with the FPU's current rounding mode, then bias negative results up by one
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?RoundHalfDownToLong@@
// Retail-asm: 0x00C3FF7F `fld dword ptr [esp+8]` + 0x00C3FF83 `fistp dword ptr [esp]` -- libpk's inline-assembly float-to-long block on the argument (value-0.5f is stored back to the argument slot because the asm reads it, the result lands in the `push ecx` word); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM and QFE, any /G or /Op) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si, so the bare dword fistp is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3FF70, 32 bytes, __cdecl, long in eax.
// Library: libpk audio (no PDB/RTTI); descriptive name. Called once, from 0x00C3FFF3. Float constant 0x0122F59C =
//   0.5f. The manifest's "GFFloatToLongFloor" is propagated.
long __cdecl RoundHalfDownToLong(float value)
{
    long result;
    value -= 0.5f;
    __asm fld value
    __asm fistp result
    if (result < 0)
        ++result;
    return result;
}