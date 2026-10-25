// libvorbis::ilog(unsigned int v) -- libvorbis res0.c: bit length of v (the number of shifts until it is zero)
// Leaf: @ilog@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9FE70, 14 bytes (+ int3 padding). libvorbis lib/res0.c static helper (libpk's
//   statically linked Xiph libvorbis, "Xiph.Org libVorbis I 20030909"); v in ecx, plain `ret` = the LTCG `__fastcall` shape of
//   the library's register-argument functions, as agent448's landed res0.c icount 0x00C9FE80 right after it.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 448 "0xC9FE70 = res0 ilog"): its call sites 0x00C9FF04 and 0x00CA0159 are in
//   res0_pack (whose icount call is 0x00C9FF39) and res0_look, res0.c's two `ilog(info->secondstages[j])` uses; the body is
//   res0.c's `while(v){ ret++; v>>=1; }`.
// Label: manifest "Util_CountHighBits" is a descriptive placeholder; the real name is the res0.c static `ilog`.
extern "C" int __fastcall ilog(unsigned int v)
{
    int ret = 0;
    while (v) {
        ret++;
        v >>= 1;
    }
    return (ret);
}
