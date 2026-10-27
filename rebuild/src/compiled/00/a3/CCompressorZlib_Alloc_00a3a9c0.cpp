// CCompressorZlib::Alloc(void*, unsigned int, unsigned int) -- zlib zalloc callback: malloc(items*size) with an overflow guard
// Retail: Fable.exe 0x00A3A9C0, 63 bytes: 64-bit `mul` of items*size; returns 0 when the product is 0, negative or
// above 0x7FFFFFFF, else malloc(items*size) (recomputed with `imul`).
// Identity: Ego_r.pdb ?Alloc@CCompressorZlib@@CAPAXPAXII@Z (private static, cdecl). Its address is stored as the
// z_stream zalloc (z_stream +0x20) by CCompressorZlib::Initialise 0x00A3AA10 (`mov [ecx+3Ch],0A3A9C0h`, stream at
// +0x1C) and by InternalCompress / InternalDecompress 0x00A3AA40 / 0x00A3AB00 for their local streams, next to Free
// 0x00A3AA00 (zfree, +0x24).
// Callees: malloc thunk 0x00BFEA0E.
// Leaf: ?Alloc@CCompressorZlib@@CAPAXPAXII@Z
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

#define CCompressorZlib DonorCCompressorZlib
#include "engine/CCompressorZlib.h"
#undef CCompressorZlib
class CCompressorZlib : public DonorCCompressorZlib {
private:
    static void* Alloc(void* opaque, unsigned int items, unsigned int size);
};

void* CCompressorZlib::Alloc(void* opaque, unsigned int items, unsigned int size)
{
    __int64 total = (__int64)((unsigned __int64)items * size);
    if (total == 0 || total < 0 || total > 0x7fffffff)
        return 0;
    return malloc(items * size);
}