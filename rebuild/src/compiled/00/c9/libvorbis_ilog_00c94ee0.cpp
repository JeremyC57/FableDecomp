// libvorbis::_ilog(unsigned int v) -- libvorbis sharedbook.c: bit length of v (the number of shifts until it is zero)
// Leaf: @_ilog@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C94EE0, 14 bytes, v in ecx, plain `ret`. libvorbis 1.0.1 lib/sharedbook.c
//   (exported `int _ilog(unsigned int)`, used by _book_maptype1_quantvals and the codebook packers).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): Xiph libvorbis 1.0.1 inside libpk
//   ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`); the neighbouring landed
//   agent447 rows (vorbis_staticbook_clear 0x00C95370, vorbis_book_decodev_set 0x00C96700, vorbis_book_errorv 0x00C96170)
//   use the same libvorbis 1.0.1 structs.
// Label: catalog "Bitcount_00c94ee0" is descriptive. Twin of res0.c's static ilog 0x00C9FE70 (same body, own symbol).
extern "C" int __fastcall _ilog(unsigned int v)
{
    int ret = 0;
    while (v) {
        ret++;
        v >>= 1;
    }
    return (ret);
}
