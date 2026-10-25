// BuildKernelRows4(const short* kernel, long phase, short* rows, long gain) -- clear the 128-short row buffer, write the 25-tap kernel reversed and scaled by gain / 32768 into the first row at (base - phase), then copy those 25 shorts to the three other rows (stride 33)
// Leaf: ?BuildKernelRows4@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C78970, length 156 bytes, __cdecl (`ret`).
// Callers: the software mixer's resampler setup (0x00C7068A, 0x00C706CD, 0x00C7073F), passing the kernel, the fractional phase, the row
// buffer at +0x110 and a 15-bit gain. No callees: memset (256 bytes) and the 3 50-byte memcpys are the intrinsics.
// Library: libpk software mixer (no PDB/RTTI); descriptive name. The manifest's "Audio_RenderMultichannelData" is a guess.
// Shape: the signed product divided by 32768 is VC's `cdq ; and edx,7FFFh ; add ; sar 15`; the loop walks two
// pointers (`*out++ = .. *in-- ..`; indexing `first[i] = kernel[24 - i]` keeps the counter in ecx and the gain in a
// register) and the unused counter becomes the `dec edi ; jne` countdown; every
// row start is `rows + (constant - phase)` (`mov ecx,K ; sub ecx,phase ; lea edi,[rows+ecx*2]`).
#include <string.h>

void __cdecl BuildKernelRows4(const short* kernel, long phase, short* rows, long gain)
{
    memset(rows, 0, 256);
    short* first = rows + (4 - phase);
    short* out = first;
    const short* in = kernel + 24;
    for (int i = 0; i < 25; i++)
        *out++ = (short)((*in-- * gain) / 32768);
    memcpy(rows + (37 - phase), first, 25 * sizeof(short));
    memcpy(rows + (70 - phase), first, 25 * sizeof(short));
    memcpy(rows + (103 - phase), first, 25 * sizeof(short));
}