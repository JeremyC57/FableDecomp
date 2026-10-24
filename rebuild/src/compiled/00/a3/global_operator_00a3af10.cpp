// operator==(const _GUID& guidOne, const _GUID& guidOther) -- out-of-line guiddef.h GUID equality: 16-byte memcmp, nonzero when equal
// Leaf: ??8@
// Retail: Fable.exe @ 0x00A3AF10, 21 bytes (+ int3 padding). No callers by rel32; arguments in ecx/edx (__fastcall, /Gr TU).
// Shape: guiddef.h `__inline int operator==(REFGUID a, REFGUID b) { return IsEqualGUID(a, b); }` with IsEqualGUID inlined:
//   the 16-byte intrinsic memcmp (`mov ecx,4 ; repe cmpsd`) and an int result (`xor eax,eax ; sete al`) -- byte-identical
//   to the IsEqualGUID copy at 0x00A3AEF0 just before it.
#include <string.h>
#pragma intrinsic(memcmp)

struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

inline int IsEqualGUID(const _GUID& rguid1, const _GUID& rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(_GUID));
}

int __fastcall operator==(const _GUID& guidOne, const _GUID& guidOther)
{
    return IsEqualGUID(guidOne, guidOther);
}