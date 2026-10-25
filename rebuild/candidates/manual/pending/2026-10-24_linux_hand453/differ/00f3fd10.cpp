// _global::allshr_reference(__int64 value, unsigned int count) -- CRT __allshr: arithmetic 64-bit shift right (value in edx:eax, count in cl in the original)
// Grade: DIFFER(119v33) -- retail_asm candidate (CRT MASM llshr.asm): edx:eax + cl register convention and shrd have no C++ spelling; reference body only
// Leaf: _allshr_reference
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3FD10, 33 bytes, plain `ret`; the CRT's hand-written MASM helper
//   (intel/llshr.asm): count >= 64 gives the sign, 32..63 shifts the high word into the low, else `shrd` + `sar`.
// Reference body only: its register calling convention (edx:eax + cl) has no C/C++ spelling, and `shrd` is not
//   reachable from VC7.1 C++ without the helper itself.
extern "C" __int64 __cdecl allshr_reference(__int64 value, unsigned int count)
{
    long hi = (long)(value >> 32);
    unsigned long lo = (unsigned long)value;
    if ((count & 0xff) >= 64) {
        hi = hi >> 31;
        lo = (unsigned long)hi;
    }
    else if ((count & 0xff) >= 32) {
        lo = (unsigned long)(hi >> (count & 31));
        hi = hi >> 31;
    }
    else if ((count & 0xff) != 0) {
        lo = (lo >> (count & 31)) | ((unsigned long)hi << (32 - (count & 31)));
        hi = hi >> (count & 31);
    }
    return (__int64)(((unsigned __int64)(unsigned long)hi << 32) | lo);
}
