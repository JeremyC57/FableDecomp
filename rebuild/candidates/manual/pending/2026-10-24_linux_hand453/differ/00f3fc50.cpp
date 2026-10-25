// _global::alldiv_reference(__int64 dividend, __int64 divisor) -- CRT __alldiv: signed 64-bit quotient (callee pops 16 bytes)
// Grade: DIFFER(112v170) -- retail_asm candidate (CRT MASM alldiv.asm, not compiler output): reference body only, the helper call / frame shape cannot reach the hand-scheduled sign tests and shr/rcr loop
// Leaf: _alldiv_reference@16
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3FC50, 170 bytes, `ret 10h`; the CRT's hand-written MASM helper
//   (intel/alldiv.asm, statically linked): sign fix-up of both operands counted in edi, the divisor-high == 0 fast path
//   of two `div`s, the shift-normalised estimate with one correction (`dec esi`), the sign restored with neg/neg/sbb.
// Reference body only (C++ that computes the same result; the original is not compiler output, so no C++ form can
//   emit these bytes: the `or eax,eax ; jge` sign tests and the `shr/rcr` normalising loop are hand scheduling).
extern "C" __int64 __stdcall alldiv_reference(__int64 dividend, __int64 divisor)
{
    int negate = 0;
    unsigned __int64 a = (unsigned __int64)dividend;
    unsigned __int64 b = (unsigned __int64)divisor;
    if (dividend < 0) { ++negate; a = 0 - a; }
    if (divisor < 0) { ++negate; b = 0 - b; }
    unsigned __int64 q = a / b;
    return negate == 1 ? -(__int64)q : (__int64)q;
}
