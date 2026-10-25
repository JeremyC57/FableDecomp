// libogg::oggpackB_write(oggpack_buffer* b, unsigned long value, int bits) -- big-endian (MSb-first) bit packer: grow the buffer by 256 bytes when fewer than four remain, then OR/store the masked value into the next five bytes and advance
// Leaf: oggpackB_write
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C910C0, 240 bytes. libogg (bundled with libvorbis 1.0.1) src/bitwise.c,
//   compiled as C with /Gr: `__fastcall`, b in ecx, value in edx, bits on the stack (`ret 4`).
// Callees: realloc (MSVCR71 import [0x01440054]). Data: the low-bit mask table mask[] @ 0x0139DDB8 (shared with
//   oggpack_write 0x00C90FC0 and oggpack_look 0x00C913A0).
// Layout: libogg oggpack_buffer {endbyte, endbit, buffer, ptr, storage}.
// Shape: the shipped source; the masked value is shifted to the top (`shl eax,cl` with cl = 32 - bits), the
//   bits parameter becomes the running total (`add edi,ecx`), and each byte is the value shifted right by
//   24/16/8/0 + endbit (endbit re-read per byte); the advance `bits/8` is signed (`cdq ; and edx,7`).
extern "C" __declspec(dllimport) void* __cdecl realloc(void* block, unsigned int size);   // [0x01440054]

typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;

#define BUFFER_INCREMENT 256
extern const unsigned long mask[];                 // 0x0139DDB8

extern "C" void __fastcall oggpackB_write(oggpack_buffer* b, unsigned long value, int bits)
{
    if (b->endbyte + 4 >= b->storage) {
        b->buffer = (unsigned char*)realloc(b->buffer, b->storage + BUFFER_INCREMENT);
        b->storage += BUFFER_INCREMENT;
        b->ptr = b->buffer + b->endbyte;
    }

    value = (value & mask[bits]) << (32 - bits);
    bits += b->endbit;

    b->ptr[0] |= value >> (24 + b->endbit);

    if (bits >= 8) {
        b->ptr[1] = (unsigned char)(value >> (16 + b->endbit));
        if (bits >= 16) {
            b->ptr[2] = (unsigned char)(value >> (8 + b->endbit));
            if (bits >= 24) {
                b->ptr[3] = (unsigned char)(value >> (b->endbit));
                if (bits >= 32) {
                    if (b->endbit)
                        b->ptr[4] = (unsigned char)(value << (8 - b->endbit));
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