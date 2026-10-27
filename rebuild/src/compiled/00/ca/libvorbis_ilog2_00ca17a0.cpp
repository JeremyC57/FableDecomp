// libvorbis::ilog2(unsigned int v) -- libvorbis floor1.c static helper: the bit length of v - 1 (0 for 0 and 1)
// Leaf: @ilog2@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ca17a0, 24 bytes, v in ecx, plain `ret` (the LTCG __fastcall shape of the library's
//   register-argument functions). libvorbis 1.0.1 lib/floor1.c `static int ilog2(unsigned int v)` (libpk's statically linked
//   Xiph libvorbis, "Xiph.Org libVorbis I 20030909").
// Identity (probable): its caller 0x00CA17C0 is floor1.c's floor1_pack: `oggpack_write(opb, info->mult - 1, 2)` (0x00CA18C2, `dec edx ; push 2`) then `ilog2(maxposit)` (0x00CA18D0). Each libvorbis file that needs it has its own static copy (no identical-code folding).
// Shape (family L136): the libvorbis body verbatim; `if (v) --v;` folds into `test ecx,ecx ; je ; dec ecx ; je`, the loop
//   head aligned with a 7-byte `lea esp,[esp]`.
extern "C" int __fastcall ilog2(unsigned int v)
{
    int ret = 0;
    if (v)
        --v;
    while (v) {
        ret++;
        v >>= 1;
    }
    return (ret);
}