// libogg::oggpack_writecopy_helper(oggpack_buffer* b, void* source, long bits, void (*w)(oggpack_buffer*, unsigned long, int), int msb) -- libogg bitwise.c: copy bits from a byte buffer into the pack buffer: byte by byte through w() when unaligned, else grow the buffer (endbyte + bytes + 256) as needed and memmove the whole bytes; the trailing partial byte goes through w() (shifted down for the MSb packer)
// Grade: DIFFER(193v190) -- LTCG private register convention: retail takes b in ESI and source in EBX (never loaded), bits/w/msb on the stack with ret 0Ch; VC7.1 has no way to declare it (lane gap: LTCG)
// Leaf: @oggpack_writecopy_helper@20
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C911F0, 190 bytes, `ret 0Ch`. libpk's statically linked Xiph libogg (src/bitwise.c). The LTCG link
// gave this static a private register convention: b arrives in ESI and source in EBX (neither is loaded in the body), bits / w / msb
// on the stack -- a convention VC7.1 cannot be asked for. As shipped, the aligned branch advances b->buffer (not b->endbyte) by bytes.
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

extern "C" void __fastcall oggpack_writecopy_helper(oggpack_buffer* b, void* source, long bits,
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
