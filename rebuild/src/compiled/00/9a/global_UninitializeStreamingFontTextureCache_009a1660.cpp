// UninitializeStreamingFontTextureCache() -- frees the streaming-font glyph cache: its four lookup arrays and the cache block taken from the global memory allocator
// Leaf: ?UninitializeStreamingFontTextureCache@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A1660, 188 bytes (+ int3 padding), plain `ret`. Ego_r PDB name:
// `?UninitializeStreamingFontTextureCache@@YIXXZ` (a global `__fastcall` with no arguments).
// Callees: operator delete[] @ 0x00BFEB1C (MSVCR71 import thunk, x4), CMemoryAllocator::EnterSynchronisation
// @ 0x009F2000 / LeaveSynchronisation @ 0x009F2010 (Enter/LeaveCriticalSection on 0x013CAA70),
// CMemoryAllocator::InternalDeallocate(void*) @ 0x009F2920, GetNoAllocations() @ 0x009F1F10 (`[this+40Ch]`),
// ~CMemoryAllocator @ 0x009F2310 and the class's static `__fastcall` operator delete @ 0x009F1F90.
// Globals: the cache block @ 0x013BDA24 (also the "cache initialised" test), the four `new[]` arrays built by
// InitializeStreamingFontTextureCache @ 0x013BDA40 / 0x013BDA34 / 0x013BDA38 / 0x013BDA3C (no PDB names; named by
// element size), the global allocator @ 0x013CAA90 and its destroy-when-empty flag @ 0x013CAA94.
// Shape: everything sits under `if (cache block)`; each array is `delete[]`d and nulled (the four `__cdecl` pops are
// merged into one `add esp,10h`); the block goes back through GFFreeFromGlobalAllocator (landed out of line
// @ 0x009A4350; expanded here, which needs `__forceinline` -- a visible body or plain `inline` stays a call): the
// block is loaded into esi before the lock call, the allocator `delete` keeps the pointer in esi for the operator
// delete call; then the block pointer is nulled.
void __cdecl operator delete[](void* block);    // 0x00BFEB1C (MSVCR71 import thunk; the lane <new> lacks it)

class CMemoryAllocator {
public:
    void InternalDeallocate(void* block);                   // 0x009F2920
    unsigned long GetNoAllocations();                       // 0x009F1F10
    ~CMemoryAllocator();                                    // 0x009F2310
    static void __fastcall operator delete(void* block);    // 0x009F1F90 (VirtualFree(p, 0, MEM_RELEASE))
    static void __fastcall EnterSynchronisation();          // 0x009F2000
    static void __fastcall LeaveSynchronisation();          // 0x009F2010
};

extern CMemoryAllocator* g_pGlobalAllocator;               // 0x013CAA90
extern bool              g_bDestroyAllocatorWhenEmpty;     // 0x013CAA94

extern void*           g_pStreamingFontCacheMemory;        // 0x013BDA24
extern unsigned long*  g_pStreamingFontCacheDwordTable;    // 0x013BDA34 ((n + 1) dwords)
extern unsigned char*  g_pStreamingFontCacheByteTable;     // 0x013BDA38
extern unsigned long*  g_pStreamingFontCachePairTable;     // 0x013BDA3C (8-byte records)
extern unsigned short* g_pStreamingFontCacheWordTable;     // 0x013BDA40

__forceinline void __fastcall GFFreeFromGlobalAllocator(void* block)   // 0x009A4350 (inlined here)
{
    CMemoryAllocator::EnterSynchronisation();
    if (block && g_pGlobalAllocator) {
        g_pGlobalAllocator->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty) {
            if (g_pGlobalAllocator->GetNoAllocations() == 0) {
                delete g_pGlobalAllocator;
                g_pGlobalAllocator = 0;
            }
        }
    }
    CMemoryAllocator::LeaveSynchronisation();
}

void __fastcall UninitializeStreamingFontTextureCache()
{
    if (g_pStreamingFontCacheMemory != 0) {
        delete[] g_pStreamingFontCacheWordTable;
        g_pStreamingFontCacheWordTable = 0;
        delete[] g_pStreamingFontCacheDwordTable;
        g_pStreamingFontCacheDwordTable = 0;
        delete[] g_pStreamingFontCacheByteTable;
        g_pStreamingFontCacheByteTable = 0;
        delete[] g_pStreamingFontCachePairTable;
        g_pStreamingFontCachePairTable = 0;
        GFFreeFromGlobalAllocator(g_pStreamingFontCacheMemory);
        g_pStreamingFontCacheMemory = 0;
    }
}