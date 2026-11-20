// UninitializeStreamingFontTextureCache() -- frees the streaming-font glyph cache: its four lookup arrays and the cache block taken from the global memory allocator
// Leaf: ?UninitializeStreamingFontTextureCache@@YIXXZ
// Flags: /d2inlT114
// Inline-budget: /d2inlT114 (EXTRA_RULES 2026-11-08 preference 2; replaces the nf140 FABLE_FORCEINLINE in the header) -- retail
//   expands the header inlines CMemoryAllocator::Deallocate and Destroy (engine/CMemoryAllocator.h opt-in
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE, plain inline) here; at the default threshold both stay calls (DIFFER 163v188).
//   Sweep NF145: T74..T113 all DIFFER, T114 is the smallest match (T114..T2000 match on 0x00BD9B90; /d2inlS85 also matches there).
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
#include "fable_compiler.h"
// NF140 (q4197 / q4198): the allocation is the inline public static CMemoryAllocator::Allocate(size, flags) of engine/CMemoryAllocator.h
// (Ego_r.pdb / FableWin.pdb ?Allocate@CMemoryAllocator@@SIPAXKK@Z, inline in release): ?Create@CMemoryAllocator@@KIXXZ 0x009F2870, the protected
// statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2000 / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2010 (both PDBs),
// PMemoryAllocator 0x013CAA90 and InternalAllocate 0x009F2A40, as the landed 0x009A3640 binds them.
// NF140 (q4197 / q4198): the frees are the inline public static CMemoryAllocator::Deallocate(block) (FableWin.pdb ?Deallocate@CMemoryAllocator@@SIXPAX@Z,
// out of line at 0x009A4350; engine/CMemoryAllocator.h opt-in FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE, the body of the landed 0x009F2B20).
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#include "engine/CMemoryAllocator.h"
void __cdecl operator delete[](void* block);    // 0x00BFEB1C (MSVCR71 import thunk; the lane <new> lacks it)


// NF193 (q4195): the cache block 0x013BDA24 is the TU's anonymous-namespace `textureCacheMem` (Ego_d.pdb PUB32
//   ?textureCacheMem@?A0xfe46a155@@3PAEA, unsigned char*; the landed InitializeStreamingFontTextureCache 0x009A3640 declares it so; Ego_r
//   0x00AECA70 / 0x00AEE990 and FableWin 0x02FB9B10 / 0x02FB40E0 keep Uninitialize beside that TU). Was the extern g_pStreamingFontCacheMemory (void*).
namespace {
extern unsigned char*  textureCacheMem;                    // 0x013BDA24
}
extern unsigned long*  g_pStreamingFontCacheDwordTable;    // 0x013BDA34 ((n + 1) dwords)
extern unsigned char*  g_pStreamingFontCacheByteTable;     // 0x013BDA38
extern unsigned long*  g_pStreamingFontCachePairTable;     // 0x013BDA3C (8-byte records)
extern unsigned short* g_pStreamingFontCacheWordTable;     // 0x013BDA40

void __fastcall UninitializeStreamingFontTextureCache()
{
    if (textureCacheMem != 0) {
        delete[] g_pStreamingFontCacheWordTable;
        g_pStreamingFontCacheWordTable = 0;
        delete[] g_pStreamingFontCacheDwordTable;
        g_pStreamingFontCacheDwordTable = 0;
        delete[] g_pStreamingFontCacheByteTable;
        g_pStreamingFontCacheByteTable = 0;
        delete[] g_pStreamingFontCachePairTable;
        g_pStreamingFontCachePairTable = 0;
        CMemoryAllocator::Deallocate(textureCacheMem);
        textureCacheMem = 0;
    }
}