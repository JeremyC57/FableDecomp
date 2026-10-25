// _global::AmplitudeToMillibels(float amplitude) -- -10000 mB (silence) for a zero amplitude; otherwise 2000 * log10(amplitude) rounded to an integer with the FPU's current rounding mode and clamped at -10000
// Grade: RELOCATION_MATCH (2 grades) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?AmplitudeToMillibels@@
// Retail-asm: 0x00C40F78 `fld dword ptr [esp+8]` + 0x00C40F7C `fistp dword ptr [esp]` -- libpk's inline-assembly float-to-long block (the product is stored back to the `amplitude` argument slot because the asm reads it, the result lands in the `push ecx` word); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`. The log10 (`fldlg2 ; fxch ; fyl2x`) is the compiler's intrinsic expansion, not asm.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C40F50, 64 bytes, __cdecl, long in eax.
// Library: libpk audio (no PDB/RTTI); descriptive name. Called once, from 0x00C424FD. Constants: 0x0122DEDC = 0.0f,
//   0x012A2AD4 = 2000.0f. The manifest's "Math_ComputeLogValue" is propagated.
#pragma intrinsic(log10)
extern "C" double __cdecl log10(double x);

long __cdecl AmplitudeToMillibels(float amplitude)
{
    if (amplitude != 0.0f) {
        long result;
        amplitude = (float)log10(amplitude) * 2000.0f;
        __asm fld amplitude
        __asm fistp result
        if (result >= -10000)
            return result;
    }
    return -10000;
}