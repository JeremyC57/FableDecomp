// libogg::oggpack_bytes(oggpack_buffer* b) -- number of bytes touched so far (whole bytes plus a partial one)
// Leaf: oggpack_bytes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C917B0, 20 bytes. libogg src/bitwise.c (bundled with libvorbis 1.0.1),
//   compiled as C with /Gr: `__fastcall`, b in ecx, plain `ret`. No callees.
// Layout: libogg oggpack_buffer {endbyte, endbit, buffer, ptr, storage}.
// Shape: the shipped source; `(endbit + 7) / 8` is a signed divide (`cdq ; and edx,7 ; sar 3`).
typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;

extern "C" long __fastcall oggpack_bytes(oggpack_buffer* b)
{
    return b->endbyte + (b->endbit + 7) / 8;
}