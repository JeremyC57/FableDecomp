// libvorbis::ilog2(unsigned int v) -- libvorbis block.c static helper: the bit length of v - 1 (0 for 0 and 1)
// Leaf: @ilog2@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c8f060, 24 bytes, v in ecx, plain `ret` (the LTCG __fastcall shape of the library's
//   register-argument functions). libvorbis 1.0.1 lib/block.c `static int ilog2(unsigned int v)` (libpk's statically linked
//   Xiph libvorbis, "Xiph.Org libVorbis I 20030909").
// Identity (probable): its caller 0x00C8F200 is block.c's _vds_shared_init: `b->modebits = ilog2(ci->modes)` (0x00C8F253) and `ilog2(ci->blocksizes[0..1]) - 6` for the window shapes (0x00C8F2AD / 0x00C8F2BB, `sub eax,6`). Each libvorbis file that needs it has its own static copy (no identical-code folding).
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