// operator!=(const _GUID& guidOne, const _GUID& guidOther) -- out-of-line guiddef.h GUID inequality: 16-byte memcmp, nonzero when different
// Leaf: ??9@
// Retail: Fable.exe @ 0x00A3AF30, 21 bytes. No callers by rel32; arguments in ecx/edx (__fastcall, /Gr TU).
// Shape: guiddef.h `__inline int operator!=(REFGUID a, REFGUID b) { return !(a == b); }` with operator== and IsEqualGUID
//   inlined: the double negation folds, so the 16-byte intrinsic memcmp (`mov ecx,4 ; repe cmpsd`) ends in `setne al`
//   after the `xor eax,eax` int widening.
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

inline int operator==(const _GUID& guidOne, const _GUID& guidOther)
{
    return IsEqualGUID(guidOne, guidOther);
}

int __fastcall operator!=(const _GUID& guidOne, const _GUID& guidOther)
{
    return !(guidOne == guidOther);
}