// libvorbis::bitreverse(ogg_uint32_t x) -- reverse the 32 bits of a codeword
// Leaf: _bitreverse
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C954F0, 99 bytes: libvorbis I 20030909 lib/sharedbook.c `static ogg_uint32_t bitreverse(ogg_uint32_t x)`
//   (byte-identical static copies in sharedbook.c 0x00C954F0 and codebook.c 0x00C962B0), x arriving in eax.
// Shape (retry1, chunk 586 lever): parked as an "LTCG register argument" gap; inside `extern "C" { static __declspec(noinline) }`
//   with a same-TU caller VC7.1 picks retail's private convention (x in eax) itself.
typedef unsigned int ogg_uint32_t;
extern "C" {
static __declspec(noinline) ogg_uint32_t bitreverse(ogg_uint32_t x)
{
    x = ((x >> 16) & 0x0000ffff) | ((x << 16) & 0xffff0000);
    x = ((x >> 8) & 0x00ff00ff) | ((x << 8) & 0xff00ff00);
    x = ((x >> 4) & 0x0f0f0f0f) | ((x << 4) & 0xf0f0f0f0);
    x = ((x >> 2) & 0x33333333) | ((x << 2) & 0xcccccccc);
    return ((x >> 1) & 0x55555555) | ((x << 1) & 0xaaaaaaaa);
}

// Same-TU caller (the bitreverse call of sharedbook.c _make_words (0x00C95618 / 0x00C957D0 / 0x00C95861: _make_words, _book_unquantize, vorbis_book_init_decode)), present so the static gets retail's
// private convention. Not graded.
ogg_uint32_t __cdecl vorbis_sharedbook_bitreverse_caller(ogg_uint32_t entry, int length)
{
    return bitreverse(entry << (32 - length));
}
}