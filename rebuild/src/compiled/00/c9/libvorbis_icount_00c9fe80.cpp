// libvorbis::icount(unsigned int v) -- libvorbis res0.c: population count (number of set bits) of a residue cascade mask
// Leaf: @icount@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9FE80, 20 bytes (+12 int3). libvorbis lib/res0.c static helper (libpk's statically
//   linked Xiph libvorbis, "Xiph.Org libVorbis I 20030909"); v arrives in ecx, plain `ret` = the LTCG/`/Gr` `__fastcall`
//   shape of the library's register-argument functions (agent446: API functions are `extern "C" __fastcall`).
// Callers: res0_pack 0x00C9FF39 / res0_unpack 0x00CA0047 (the cascade popcount `acc+=icount(info->secondstages[j])`).
// Shape: the shipped source; `ret+=v&1` keeps v in ecx and copies it to edx for the mask.
// Label: manifest "Util_PopCount" is a descriptive placeholder; the real name is the res0.c static `icount`.
extern "C" int __fastcall icount(unsigned int v)
{
    int ret = 0;
    while (v) {
        ret += v & 1;
        v >>= 1;
    }
    return (ret);
}