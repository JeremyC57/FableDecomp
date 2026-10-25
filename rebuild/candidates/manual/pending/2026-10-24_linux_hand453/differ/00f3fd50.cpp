// _global::aulldvrm_reference(unsigned __int64 dividend, unsigned __int64 divisor, unsigned __int64* remainder) -- CRT __aulldvrm: unsigned 64-bit quotient and remainder (quotient in edx:eax, remainder in ebx:ecx in the original)
// Grade: DIFFER(72v149) -- retail_asm candidate (CRT MASM ulldvrm.asm): quotient edx:eax + remainder ebx:ecx return has no C++ spelling; reference body only
// Leaf: _aulldvrm_reference
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3FD50, 149 bytes, `ret 10h`; the CRT's hand-written MASM helper
//   (intel/ulldvrm.asm): divisor-high == 0 fast path, else shift-normalised estimate, one correction, remainder =
//   dividend - quotient * divisor, returned in ebx:ecx.
// Reference body only: the two-register-pair return has no C/C++ spelling.
extern "C" unsigned __int64 __cdecl aulldvrm_reference(unsigned __int64 dividend, unsigned __int64 divisor, unsigned __int64* remainder)
{
    unsigned __int64 q = dividend / divisor;
    *remainder = dividend - q * divisor;
    return q;
}
