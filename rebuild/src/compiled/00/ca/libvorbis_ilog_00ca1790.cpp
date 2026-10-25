// libvorbis::ilog(unsigned int v) -- libvorbis floor1.c: bit length of v (the number of shifts until it is zero)
// Leaf: @ilog@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA1790, 14 bytes (+ int3 padding). libvorbis lib/floor1.c static helper (libpk's
//   statically linked Xiph libvorbis 20030909), the floor1.c twin of res0.c's ilog 0x00C9FE70; v in ecx, plain `ret` (LTCG
//   `__fastcall`).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 448 "0xCA1790 = floor1 ilog"): it follows floor1_free_info 0x00CA1750 (renamed
//   this pass) in the floor1.c object, and its four call sites 0x00CA2DD4 / 0x00CA2E00 / 0x00CA31B1 / 0x00CA31C8 are in the
//   floor1 packet code (floor1.c's `ilog(look->quant_q-1)` bit widths); the body is floor1.c's `while(v){ ret++; v>>=1; }`.
// Label: manifest "Util_CountBits" is a descriptive placeholder; the real name is the floor1.c static `ilog`.
extern "C" int __fastcall ilog(unsigned int v)
{
    int ret = 0;
    while (v) {
        ret++;
        v >>= 1;
    }
    return (ret);
}
