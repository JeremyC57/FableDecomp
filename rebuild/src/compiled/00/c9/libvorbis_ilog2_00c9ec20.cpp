// libvorbis::ilog2(unsigned int v) -- libvorbis mapping0.c static helper: the bit length of v - 1 (0 for 0 and 1)
// Leaf: @ilog2@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c9ec20, 24 bytes, v in ecx, plain `ret` (the LTCG __fastcall shape of the library's
//   register-argument functions). libvorbis 1.0.1 lib/mapping0.c `static int ilog2(unsigned int v)` (libpk's statically linked
//   Xiph libvorbis, "Xiph.Org libVorbis I 20030909").
// Identity (probable): it sits after the landed mapping0_free_info 0x00C9EC00 and is called twice each by 0x00C9EC40 (mapping0_pack) and 0x00C9ED80 (mapping0_unpack) -- mapping0.c's `ilog2(vi->channels)` field widths. Each libvorbis file that needs it has its own static copy (no identical-code folding).
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