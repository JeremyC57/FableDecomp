// libogg::oggpack_look(oggpack_buffer* b, int bits) -- peek the next `bits` (<= 32) bits LSb-first without advancing; -1 when they run past the end of the buffer
// Leaf: oggpack_look
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C913A0, 148 bytes. libogg src/bitwise.c (bundled with libvorbis 1.0.1),
//   compiled as C with /Gr: `__fastcall`, b in ecx, bits in edx, plain `ret`.
// Data: the low-bit mask table mask[] @ 0x0139DDB8.
// Layout: libogg oggpack_buffer {endbyte, endbit, buffer, ptr, storage}.
// Shape: the shipped source; `m = mask[bits]` is fetched before the bits total; the end test runs only when
//   fewer than four bytes remain (`endbyte*8 + bits > storage*8`); the first byte's shift re-reads endbit
//   through cl, the later ones use the cached copy in edi.
typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;

extern const unsigned long mask[];                 // 0x0139DDB8

extern "C" long __fastcall oggpack_look(oggpack_buffer* b, int bits)
{
    unsigned long ret;
    unsigned long m = mask[bits];

    bits += b->endbit;

    if (b->endbyte + 4 >= b->storage) {
        /* not the main path */
        if (b->endbyte * 8 + bits > b->storage * 8) return -1;
    }

    ret = b->ptr[0] >> b->endbit;
    if (bits > 8) {
        ret |= b->ptr[1] << (8 - b->endbit);
        if (bits > 16) {
            ret |= b->ptr[2] << (16 - b->endbit);
            if (bits > 24) {
                ret |= b->ptr[3] << (24 - b->endbit);
                if (bits > 32 && b->endbit)
                    ret |= b->ptr[4] << (32 - b->endbit);
            }
        }
    }
    return m & ret;
}