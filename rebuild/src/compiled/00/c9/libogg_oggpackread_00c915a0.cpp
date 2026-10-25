// libogg::oggpack_read(oggpack_buffer* b, int bits) -- read the next `bits` (<= 32) bits LSb-first and advance; -1 (and still advance) when they run past the end of the buffer
// Leaf: @oggpack_read@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C915A0, 188 bytes, plain `ret` (b in ecx, bits in edx). libogg src/bitwise.c.
//   Data: mask[] @ 0x0139DDB8.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): the library past libpk is Xiph libogg +
//   libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240); bitwise.c is libogg's oggpack_* over the 20-byte
//   oggpack_buffer {endbyte, endbit, buffer, ptr, storage} with the shared mask[] table @ 0x0139DDB8, and the
//   neighbouring landed agent447 rows oggpackB_write 0x00C910C0, oggpack_look 0x00C913A0, oggpack_adv 0x00C91530,
//   oggpack_bytes 0x00C917B0 use the same model. Compiled as C with /Gr: `__fastcall`, b in ecx.
// Label: catalog "CBitstream::ReadBits" is a descriptive model of this libogg function.
// Shape: the shipped source; `m = mask[bits]` is fetched before the bits total; the end test runs only when fewer
//   than four bytes remain (`endbyte*8 + bits > storage*8` jumps to the advance with ret = -1).
typedef struct oggpack_buffer {
    long           endbyte;                        // +0x00
    int            endbit;                         // +0x04
    unsigned char* buffer;                         // +0x08
    unsigned char* ptr;                            // +0x0c
    long           storage;                        // +0x10
} oggpack_buffer;

extern const unsigned long mask[];                 // 0x0139DDB8

extern "C" long __fastcall oggpack_read(oggpack_buffer* b, int bits)
{
    unsigned long ret;
    unsigned long m = mask[bits];

    bits += b->endbit;

    if (b->endbyte + 4 >= b->storage) {
        /* not the main path */
        ret = (unsigned long)-1;
        if (b->endbyte * 8 + bits > b->storage * 8) goto overflow;
    }

    ret = b->ptr[0] >> b->endbit;
    if (bits > 8) {
        ret |= b->ptr[1] << (8 - b->endbit);
        if (bits > 16) {
            ret |= b->ptr[2] << (16 - b->endbit);
            if (bits > 24) {
                ret |= b->ptr[3] << (24 - b->endbit);
                if (bits > 32 && b->endbit) {
                    ret |= b->ptr[4] << (32 - b->endbit);
                }
            }
        }
    }
    ret &= m;

overflow:

    b->ptr += bits / 8;
    b->endbyte += bits / 8;
    b->endbit = bits & 7;
    return (ret);
}
