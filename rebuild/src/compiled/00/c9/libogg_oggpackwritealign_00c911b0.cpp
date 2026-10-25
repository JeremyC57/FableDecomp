// libogg::oggpack_writealign(oggpack_buffer* b) -- pad the current byte with zero bits up to the next byte boundary
// Leaf: @oggpack_writealign@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C911B0, 24 bytes, plain `ret`. libogg src/bitwise.c. Callee: oggpack_write
//   0x00C90FC0 (`__fastcall`, b passed on in ecx, 0 in edx, the pad count pushed).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): the library past libpk is Xiph libogg +
//   libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240); bitwise.c is libogg's oggpack_* over the 20-byte
//   oggpack_buffer {endbyte, endbit, buffer, ptr, storage} with the shared mask[] table @ 0x0139DDB8, and the
//   neighbouring landed agent447 rows oggpackB_write 0x00C910C0, oggpack_look 0x00C913A0, oggpack_adv 0x00C91530,
//   oggpack_bytes 0x00C917B0 use the same model. Compiled as C with /Gr: `__fastcall`, b in ecx.
// Label: catalog "CBitstream_00c911b0::FlushByte" / manifest "CEditEngine2DWorldMap::RemoveMap" are descriptive/propagated.
typedef struct oggpack_buffer {
    long           endbyte;                        // +0x00
    int            endbit;                         // +0x04
    unsigned char* buffer;                         // +0x08
    unsigned char* ptr;                            // +0x0c
    long           storage;                        // +0x10
} oggpack_buffer;

extern "C" void __fastcall oggpack_write(oggpack_buffer* b, unsigned long value, int bits);   // 0x00C90FC0

extern "C" void __fastcall oggpack_writealign(oggpack_buffer* b)
{
    int bits = 8 - b->endbit;
    if (bits < 8)
        oggpack_write(b, 0, bits);
}
