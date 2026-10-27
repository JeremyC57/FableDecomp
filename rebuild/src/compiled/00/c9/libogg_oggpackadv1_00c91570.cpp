// libogg::oggpack_adv1(oggpack_buffer* b) -- Xiph bitwise.c: advance the read position by one bit (carry into the next byte after bit 7)
// Leaf: @oggpack_adv1@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C91570, 28 bytes, __fastcall (b in ecx), plain ret. No callees.
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: bitwise.c's oggpack_adv1 verbatim (endbit +4, ptr +0xc, endbyte +0); it sits between the landed oggpack_adv
//   0x00C91530 and oggpack_read 0x00C915A0 (bitwise.c order), and oggpackB_adv1 0x00C91590 tail-jumps to it (libogg 1.1:
//   `oggpackB_adv1(b){ oggpack_adv1(b); }`). The catalog label (gap-scan) is a placeholder.
typedef struct oggpack_buffer {
    long           endbyte;             // +0x00
    int            endbit;              // +0x04
    unsigned char* buffer;              // +0x08
    unsigned char* ptr;                 // +0x0c
    long           storage;             // +0x10
} oggpack_buffer;

extern "C" void __fastcall oggpack_adv1(oggpack_buffer* b)
{
    if (++(b->endbit) > 7) {
        b->endbit = 0;
        b->ptr++;
        b->endbyte++;
    }
}