// BuildKernelRows8(const short* kernel, long phase, short* rows, long gain) -- clear the 320-short row buffer, write the 25-tap kernel reversed and scaled by gain / 32768 into the first row at (base - phase), then copy those 25 shorts to the seven other rows (stride 41)
// Leaf: ?BuildKernelRows8@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C7BA00, length 240 bytes, __cdecl (`ret`).
// Callers: the software mixer's resampler setup (0x00C7105A, 0x00C7109D, 0x00C7110F), passing the kernel, the fractional phase, the row
// buffer at +0x110 and a 15-bit gain. No callees: memset (640 bytes) and the 7 50-byte memcpys are the intrinsics.
// Library: libpk software mixer (no PDB/RTTI); descriptive name. The manifest's "Audio_RenderMultichannelData_Alt" is a guess.
// Shape: the signed product divided by 32768 is VC's `cdq ; and edx,7FFFh ; add ; sar 15`; the loop walks two
// pointers (`*out++ = .. *in-- ..`; indexing `first[i] = kernel[24 - i]` keeps the counter in ecx and the gain in a
// register) and the unused counter becomes the `dec edi ; jne` countdown; every
// row start is `rows + (constant - phase)` (`mov ecx,K ; sub ecx,phase ; lea edi,[rows+ecx*2]`).
#include <string.h>

void __cdecl BuildKernelRows8(const short* kernel, long phase, short* rows, long gain)
{
    memset(rows, 0, 640);
    short* first = rows + (8 - phase);
    short* out = first;
    const short* in = kernel + 24;
    for (int i = 0; i < 25; i++)
        *out++ = (short)((*in-- * gain) / 32768);
    memcpy(rows + (49 - phase), first, 25 * sizeof(short));
    memcpy(rows + (90 - phase), first, 25 * sizeof(short));
    memcpy(rows + (131 - phase), first, 25 * sizeof(short));
    memcpy(rows + (172 - phase), first, 25 * sizeof(short));
    memcpy(rows + (213 - phase), first, 25 * sizeof(short));
    memcpy(rows + (254 - phase), first, 25 * sizeof(short));
    memcpy(rows + (295 - phase), first, 25 * sizeof(short));
}