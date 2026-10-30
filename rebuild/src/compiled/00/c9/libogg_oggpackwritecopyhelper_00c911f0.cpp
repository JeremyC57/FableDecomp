// libogg::oggpack_writecopy_helper(oggpack_buffer* b, void* source, long bits, void (*w)(oggpack_buffer*, unsigned long, int), int msb) -- libogg bitwise.c: copy bits from a byte buffer into the pack buffer: byte by byte through w() when unaligned, else grow the buffer (endbyte + bytes + 256) as needed and memmove the whole bytes; the trailing partial byte goes through w() (shifted down for the MSb packer)
// Shape (retry1): parked as an "LTCG custom register convention" gap; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the libogg static retail's private register convention itself (chunk 585/586/593 lever).
// Leaf: _oggpack_writecopy_helper
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C911F0, 190 bytes, `ret 0Ch`. libpk's statically linked Xiph libogg (src/bitwise.c). Retail
// convention: b arrives in ESI and source in EBX (neither is loaded in the body), bits / w / msb on the stack; VC7.1 /O2 picks
// it itself for the same-TU static. As shipped, the aligned branch advances b->buffer (not b->endbyte) by bytes.
// Identity: the bits/8 split, the endbit test, the BUFFER_INCREMENT 256 realloc and the w() callback with the msb shift are bitwise.c
// oggpack_writecopy_helper exactly. Catalogued as gap-scan sub_00c911f0.
// Callees: realloc (IAT 0x01440054, _ogg_realloc), memmove (IAT 0x01440144), the w callback.
extern "C" __declspec(dllimport) void* __cdecl realloc(void* block, unsigned int size);   // [0x01440054]
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // [0x01440144]
#define _ogg_realloc realloc
#define BUFFER_INCREMENT 256
typedef struct {
    long endbyte;
    int endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long storage;
} oggpack_buffer;

extern "C" {
static __declspec(noinline) void __stdcall oggpack_writecopy_helper(oggpack_buffer* b, void* source, long bits,
                                                    void (__fastcall* w)(oggpack_buffer*, unsigned long, int), int msb)
{
    unsigned char* ptr = (unsigned char*)source;

    long bytes = bits / 8;
    bits -= bytes * 8;

    if (b->endbit) {
        int i;
        /* unaligned copy.  Do it the hard way. */
        for (i = 0; i < bytes; i++)
            w(b, (unsigned long)(ptr[i]), 8);
    } else {
        /* aligned block copy */
        if (b->endbyte + bytes + 1 >= b->storage) {
            b->storage = b->endbyte + bytes + BUFFER_INCREMENT;
            b->buffer = (unsigned char*)_ogg_realloc(b->buffer, b->storage);
            b->ptr = b->buffer + b->endbyte;
        }

        memmove(b->ptr, source, bytes);
        b->ptr += bytes;
        b->buffer += bytes;
        *b->ptr = 0;
    }
    if (bits) {
        if (msb)
            w(b, (unsigned long)(ptr[bytes] >> (8 - bits)), bits);
        else
            w(b, (unsigned long)(ptr[bytes]), bits);
    }
}

// Same-TU caller (the oggpack_writecopy_helper calls of bitwise.c oggpack_writecopy / oggpackB_writecopy), present so the static gets retail's private
// convention. Not graded.
void __cdecl oggpack_writecopy_helper_caller(oggpack_buffer* b, void* source, long bits,
                                             void (__fastcall* w)(oggpack_buffer*, unsigned long, int), int msb)
{ oggpack_writecopy_helper(b, source, bits, w, msb); }
}