// libogg::oggpack_reset(oggpack_buffer* b) -- rewind the writer to the start of its buffer and empty the first byte
// Leaf: @oggpack_reset@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C912F0, 23 bytes, plain `ret`. libogg src/bitwise.c.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): the library past libpk is Xiph libogg +
//   libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240); bitwise.c is libogg's oggpack_* over the 20-byte
//   oggpack_buffer {endbyte, endbit, buffer, ptr, storage} with the shared mask[] table @ 0x0139DDB8, and the
//   neighbouring landed agent447 rows oggpackB_write 0x00C910C0, oggpack_look 0x00C913A0, oggpack_adv 0x00C91530,
//   oggpack_bytes 0x00C917B0 use the same model. Compiled as C with /Gr: `__fastcall`, b in ecx.
// Label: catalog "CTextBuffer_00c912f0::Clear" / manifest "CIndexBuffer" ctor are descriptive/propagated.
typedef struct oggpack_buffer {
    long           endbyte;                        // +0x00
    int            endbit;                         // +0x04
    unsigned char* buffer;                         // +0x08
    unsigned char* ptr;                            // +0x0c
    long           storage;                        // +0x10
} oggpack_buffer;

extern "C" void __fastcall oggpack_reset(oggpack_buffer* b)
{
    b->ptr = b->buffer;
    b->buffer[0] = 0;
    b->endbit = b->endbyte = 0;
}
