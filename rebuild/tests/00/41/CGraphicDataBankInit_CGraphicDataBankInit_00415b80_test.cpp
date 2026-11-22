// Test for 0x00415B80 CGraphicDataBankInit::CGraphicDataBankInit(): on the shared engine/CGraphicDataBankInit.h, a default-constructed
// object (over a 0x5A-filled buffer) has the seven pixel formats and both maxima -1, GenerateMipmaps false, AllowDither true,
// AllowCompressedTextures false and AllowDXT1ForBooleanAlpha true.
#include <cstdio>
#include <cstring>
#include <new>
#include "engine/CGraphicDataBankInit.h"
static double g_buf[8];
int main()
{
    std::memset(g_buf, 0x5a, sizeof(g_buf));
    CGraphicDataBankInit* d = ::new (g_buf) CGraphicDataBankInit();
    // CPixelFormat::FormatIndex is protected: read the seven 4-byte formats (+0x00..+0x18) from the object's storage.
    const long* f = reinterpret_cast<const long*>(g_buf);
    for (int i = 0; i < 7; ++i)
        if (f[i] != -1) { std::printf("00415b80_TEST FAIL format %d\n", i); return 1; }
    if ((const void*)&d->SignedFormat != (const void*)(f + 6)) { std::printf("00415b80_TEST FAIL layout\n"); return 1; }
    if (d->GenerateMipmaps || d->MaxGraphicWidth != (unsigned long)-1 || d->MaxGraphicHeight != (unsigned long)-1 || !d->AllowDither ||
        d->AllowCompressedTextures || !d->AllowDXT1ForBooleanAlpha) { std::printf("00415b80_TEST FAIL flags\n"); return 1; }
    std::printf("00415b80_TEST PASS\n");
    return 0;
}
