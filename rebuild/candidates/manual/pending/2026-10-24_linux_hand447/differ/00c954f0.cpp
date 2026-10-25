// libvorbis::bitreverse(ogg_uint32_t x) -- reverse the 32 bits of a codeword
// Grade: DIFFER(99v99) -- LTCG register argument: retail takes x in EAX (`mov ecx,eax ; shl eax,10h`), ours in ecx; the same operations with renamed registers
// Retail: Fable.exe @ 0x00C954F0, 99 bytes. libvorbis 1.0.1 lib/sharedbook.c (static; byte-identical twin 0x00C962B0 in codebook.c). Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: bitreverse
typedef unsigned int ogg_uint32_t;
extern "C" ogg_uint32_t __fastcall bitreverse(ogg_uint32_t x)
{
    x = ((x >> 16) & 0x0000ffff) | ((x << 16) & 0xffff0000);
    x = ((x >> 8) & 0x00ff00ff) | ((x << 8) & 0xff00ff00);
    x = ((x >> 4) & 0x0f0f0f0f) | ((x << 4) & 0xf0f0f0f0);
    x = ((x >> 2) & 0x33333333) | ((x << 2) & 0xcccccccc);
    return ((x >> 1) & 0x55555555) | ((x << 1) & 0xaaaaaaaa);
}
