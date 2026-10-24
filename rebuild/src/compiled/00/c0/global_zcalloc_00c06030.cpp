// _global::zcalloc(voidpf opaque, unsigned items, unsigned size) -- zlib 1.2.1 default allocator: malloc(items * size)
// Leaf: _zcalloc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C06030, 20 bytes. zlib 1.2.1 zutil.c (32-bit branch), C linkage, __cdecl.
// Callee: MSVCR71 malloc through its import slot 0x01440158 (`call [__imp__malloc]`; the thunk 0x00BFEA0E jumps
// through the same slot), so malloc is declared extern "C" __declspec(dllimport).
// Shape: `if (opaque) items += size - size;` is zlib's "make compiler happy" no-op and folds away; the
// `sizeof(uInt) > 2` choice picks malloc; `imul eax,[esp+0Ch]` is items * size and the result is returned (add esp,4).
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int items, unsigned int size);   // dead branch on 32-bit

typedef void* voidpf;

extern "C" voidpf __cdecl zcalloc(voidpf opaque, unsigned items, unsigned size)
{
    if (opaque) items += size - size; /* make compiler happy */
    return sizeof(unsigned int) > 2 ? (voidpf)malloc(items * size) :
                                      (voidpf)calloc(items, size);
}