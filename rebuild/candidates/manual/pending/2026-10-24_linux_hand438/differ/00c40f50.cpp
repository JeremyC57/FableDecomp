// _global::AmplitudeToMillibels(float amplitude) -- -10000 mB (silence) for a zero amplitude; otherwise 2000 * log10(amplitude) converted to an integer with a bare `fistp` (an inline-asm block in retail) and clamped at -10000
// Grade: DIFFER(61v64) -- retail_asm candidate: bare `fld [value] ; fistp dword [esp]` after the intrinsic log10 * 2000 is stored back to the argument slot (an inline-asm block); the C++ reference body truncates through _ftol. 1 grade.
// Leaf: ?AmplitudeToMillibels@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C40F50, 64 bytes, __cdecl, long in eax.
// Library: libpk audio (no PDB/RTTI); descriptive name. Called once, from 0x00C424FD. Constants: 0x0122DEDC = 0.0f,
//   0x012A2AD4 = 2000.0f. The manifest's "Math_ComputeLogValue" is propagated.
// Retail-asm candidate: the `log10(amplitude) * 2000.0f` (intrinsic `fldlg2 ; fxch ; fyl2x`) is stored back to the
//   argument slot, then a bare `fld dword [value] ; fistp dword [result]` -- an inline-asm block in the original.
//   This C++ reference body (truncating cast, _ftol) is NOT byte-exact.
#pragma intrinsic(log10)
extern "C" double __cdecl log10(double x);

long __cdecl AmplitudeToMillibels(float amplitude)
{
    long result = -10000;
    if (amplitude != 0.0f) {
        amplitude = (float)log10(amplitude) * 2000.0f;
        result = (long)amplitude;
        if (result < -10000)
            result = -10000;
    }
    return result;
}
