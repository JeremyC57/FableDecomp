// libvorbis::sort32a(const void* a, const void* b) -- qsort comparator over pointers to 32-bit codewords: ascending unsigned order
// Leaf: sort32a
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C95560, 31 bytes. libvorbis 1.0.1 lib/sharedbook.c (static, qsort
//   callback of vorbis_book_init_decode), `__cdecl` (plain `ret`). No callees.
// Shape: the shipped source; each unsigned compare is `cmp ; sbb ; neg` and the difference is a `sub`.
typedef unsigned int ogg_uint32_t;

extern "C" int __cdecl sort32a(const void* a, const void* b)
{
    return (**(ogg_uint32_t**)a > **(ogg_uint32_t**)b) -
           (**(ogg_uint32_t**)a < **(ogg_uint32_t**)b);
}