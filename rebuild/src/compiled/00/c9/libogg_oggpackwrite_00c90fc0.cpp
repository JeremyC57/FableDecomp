// libogg::oggpack_write(oggpack_buffer* b, unsigned long value, int bits) -- LSb-first bit packer: grow the buffer by 256 bytes when fewer than four remain, then OR/store the masked value into the next five bytes and advance
// Leaf: @oggpack_write@12
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C90FC0, 243 bytes, `ret 4` (b in ecx, value in edx, bits on the stack).
//   libogg src/bitwise.c. Callee: realloc (MSVCR71 import [0x01440054]). Data: mask[] @ 0x0139DDB8.
//   Callers: oggpack_writealign 0x00C911B0 and vorbis_book_encode 0x00C96140.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): the library past libpk is Xiph libogg +
//   libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240); bitwise.c is libogg's oggpack_* over the 20-byte
//   oggpack_buffer {endbyte, endbit, buffer, ptr, storage} with the shared mask[] table @ 0x0139DDB8, and the
//   neighbouring landed agent447 rows oggpackB_write 0x00C910C0, oggpack_look 0x00C913A0, oggpack_adv 0x00C91530,
//   oggpack_bytes 0x00C917B0 use the same model. Compiled as C with /Gr: `__fastcall`, b in ecx.
// Label: catalog "CBitstream::WriteBits" (comment said libpk) is a descriptive model of this libogg function.
// Shape: the shipped source; `bits` becomes the running total, the first byte's shift is formed before the add, and
//   the advance `bits/8` is signed (`cdq ; and edx,7 ; sar 3`).
extern "C" __declspec(dllimport) void* __cdecl realloc(void* block, unsigned int size);   // [0x01440054]

typedef struct oggpack_buffer {
    long           endbyte;                        // +0x00
    int            endbit;                         // +0x04
    unsigned char* buffer;                         // +0x08
    unsigned char* ptr;                            // +0x0c
    long           storage;                        // +0x10
} oggpack_buffer;

#define BUFFER_INCREMENT 256
extern const unsigned long mask[];                 // 0x0139DDB8

extern "C" void __fastcall oggpack_write(oggpack_buffer* b, unsigned long value, int bits)
{
    if (b->endbyte + 4 >= b->storage) {
        b->buffer = (unsigned char*)realloc(b->buffer, b->storage + BUFFER_INCREMENT);
        b->storage += BUFFER_INCREMENT;
        b->ptr = b->buffer + b->endbyte;
    }

    value &= mask[bits];
    bits += b->endbit;

    b->ptr[0] |= value << b->endbit;

    if (bits >= 8) {
        b->ptr[1] = (unsigned char)(value >> (8 - b->endbit));
        if (bits >= 16) {
            b->ptr[2] = (unsigned char)(value >> (16 - b->endbit));
            if (bits >= 24) {
                b->ptr[3] = (unsigned char)(value >> (24 - b->endbit));
                if (bits >= 32) {
                    if (b->endbit)
                        b->ptr[4] = (unsigned char)(value >> (32 - b->endbit));
                    else
                        b->ptr[4] = 0;
                }
            }
        }
    }

    b->endbyte += bits / 8;
    b->ptr += bits / 8;
    b->endbit = bits & 7;
}
