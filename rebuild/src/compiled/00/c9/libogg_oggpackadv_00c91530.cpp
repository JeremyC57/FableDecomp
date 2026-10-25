// libogg::oggpack_adv(oggpack_buffer* b, int bits) -- advance the read position by `bits` bits
// Leaf: oggpack_adv
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C91530, 41 bytes. libogg src/bitwise.c (bundled with libvorbis 1.0.1),
//   compiled as C with /Gr: `__fastcall`, b in ecx, bits in edx, plain `ret`. No callees.
// Layout: libogg oggpack_buffer {endbyte, endbit, buffer, ptr, storage}.
// Shape: the shipped source; the total lives in esi (`mov esi,edx ; add esi,eax`), `bits/8` is a signed divide
//   (`cdq ; and edx,7 ; sar 3`) shared by the ptr and endbyte updates; VC schedules the endbit store before
//   the endbyte store.
typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;

extern "C" void __fastcall oggpack_adv(oggpack_buffer* b, int bits)
{
    bits += b->endbit;
    b->ptr += bits / 8;
    b->endbyte += bits / 8;
    b->endbit = bits & 7;
}