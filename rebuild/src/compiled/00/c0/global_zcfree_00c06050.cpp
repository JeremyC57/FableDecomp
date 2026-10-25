// _global::zcfree(voidpf opaque, voidpf ptr) -- zlib 1.2.1 default deallocator: free(ptr)
// Leaf: _zcfree
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C06050, 13 bytes. zlib 1.2.1 zutil.c (32-bit branch), C linkage, __cdecl.
// Identity: not called directly; its address is stored by z_deflateInit2_ (0x00C01FE3) and z_inflateInit2_ (0x00C0228A)
//   as the default strm->zfree (`if (strm->zfree == (free_func)0) strm->zfree = zcfree;`), the twin of zcalloc 0x00C06030.
//   Catalog/manifest "operator_new" was a propagated name.
// Callee: MSVCR71 free through its import slot 0x01440154 (`call [__imp__free]`), so free is declared
//   extern "C" __declspec(dllimport).
// Shape: `if (opaque) return;` is zlib's "make compiler happy" no-op and folds away.
extern "C" __declspec(dllimport) void __cdecl free(void* ptr);

typedef void* voidpf;

extern "C" void __cdecl zcfree(voidpf opaque, voidpf ptr)
{
    free(ptr);
    if (opaque) return; /* make compiler happy */
}
