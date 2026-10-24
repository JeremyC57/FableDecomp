// MemCopy(void* dest, const void* source, unsigned long size) -- copy size raw bytes from source to dest (the intrinsic memcpy: dwords, then the 0..3 tail bytes)
// Leaf: MemCopy
// Retail: Fable.exe @ 0x009A0020, 29 bytes, `ret 4`: __fastcall (dest ecx, source edx, the byte count on the stack); no
// return value (eax is left holding the count). No direct call site or absolute reference in the image (reached
// through ICF folding or dead); it sits in the CBasicString<char> code beside 0x0099FFE0 (the set-length helper whose
// zero padding is the matching inline memset).
// Identity: the catalog's `_Fill<CEngineCamera::CDebugScriptElement*>` is propagated -- the body only moves bytes; the
// manifest's three-pointer prototype is wrong (one stack word).
// Shape: /O2 expands memcpy inline (`rep movsd` of size >> 2, then `rep movsb` of size & 3).
#include <string.h>
#pragma intrinsic(memcpy)

void __fastcall MemCopy(void* dest, const void* source, unsigned long size)
{
    memcpy(dest, source, size);
}