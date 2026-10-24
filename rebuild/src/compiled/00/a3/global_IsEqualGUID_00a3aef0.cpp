// IsEqualGUID(const _GUID& rguid1, const _GUID& rguid2) -- out-of-line guiddef.h IsEqualGUID: 16-byte memcmp of two GUIDs, nonzero when equal
// Leaf: ?IsEqualGUID@@
// Retail: Fable.exe @ 0x00A3AEF0, 21 bytes (+ int3 padding). No callers by rel32 (an unreferenced out-of-line copy kept
//   beside the DirectShow / CVideoSys code); arguments arrive in ecx/edx, so the copy is __fastcall (a /Gr TU).
// Shape: guiddef.h's C++ `__inline int IsEqualGUID(REFGUID, REFGUID) { return !memcmp(&a, &b, sizeof(GUID)); }`; the
//   16-byte intrinsic memcmp is `mov ecx,4 ; repe cmpsd`, and the int result is `xor eax,eax ; sete al`.
//   The three neighbouring rows 0x00A3AEF0 / 0x00A3AF10 / 0x00A3AF30 are IsEqualGUID, operator== and operator!= in
//   guiddef.h order.
#include <string.h>
#pragma intrinsic(memcmp)

struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

int __fastcall IsEqualGUID(const _GUID& rguid1, const _GUID& rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(_GUID));
}