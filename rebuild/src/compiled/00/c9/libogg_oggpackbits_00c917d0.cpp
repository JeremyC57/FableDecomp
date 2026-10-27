// libogg::oggpack_bits(oggpack_buffer* b) -- Xiph bitwise.c: the bit position (endbyte * 8 + endbit)
// Leaf: @oggpack_bits@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C917D0, 9 bytes, __fastcall (b in ecx), plain ret. No callees.
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: bitwise.c's oggpack_bits verbatim; it follows the landed oggpack_bytes 0x00C917B0 (bitwise.c order) and
//   oggpackB_bits 0x00C917F0 tail-jumps to it (libogg 1.1: `return oggpack_bits(b);`). The catalog label is a placeholder.
typedef struct oggpack_buffer {
    long           endbyte;             // +0x00
    int            endbit;              // +0x04
    unsigned char* buffer;              // +0x08
    unsigned char* ptr;                 // +0x0c
    long           storage;             // +0x10
} oggpack_buffer;

extern "C" long __fastcall oggpack_bits(oggpack_buffer* b)
{
    return (b->endbyte * 8 + b->endbit);
}