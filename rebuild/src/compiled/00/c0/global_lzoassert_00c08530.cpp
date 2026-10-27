// _global::lzo_assert(int expr) -- LZO 1.08 lzo_assert: 1 when expr is non-zero, else 0 (the library's self-check helper)
// Leaf: _lzo_assert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C08530, 12 bytes. LZO 1.08 lzo_init.c, C linkage, __cdecl (LZO_PUBLIC). No callees.
// Identity (proven): the LZO 1.08 body `return (expr) ? 1 : 0;` (`xor eax,eax ; test ecx,ecx ; setne al`), in the LZO 1.08
//   code next to the landed _lzo1x_999_compress (0x00C08130); the catalog already names it _global::lzo_assert.
typedef int lzo_bool;

extern "C" lzo_bool __cdecl lzo_assert(int expr)
{
    return (expr) ? 1 : 0;
}