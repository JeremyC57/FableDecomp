#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// CTextureManager::Initialise(const vector<CTexturePoolSpec>& specs) -- once only: size one preallocation pool per spec (texture byte size x count), sort the pools by texture size, total the buffer and initialise each pool
// Leaf: ?Initialise@CTextureManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6AAA0, 304 bytes (+16 int3), `ret 4`. RENDER LAYER (texture pools).
// Callees: the (size, count) array's sized constructor @ 0x00A6B620 (manifest "CSimpleMemoryPool::New<...>" is
//   propagated), CalculateTextureSize(unsigned long, unsigned long, bool, const CPixelFormat*) @ 0x00A698F0 (`__fastcall`),
//   the pair sort @ 0x00A6C080 (`__fastcall` first/last), CPreallocTexturePool::Initialise(unsigned long, unsigned long,
//   unsigned long) @ 0x00A6A540, free @ 0x00BFEA14 (the array's inline destructor).
// Layout: retail CTextureManager (see the landed GetPoolIndex 0x00A69730): Initialised +0x04, PoolCount +0x08,
//   PreallocSizes[16] +0x0C, PreallocPools[16] +0x4C (0x54-byte CPreallocTexturePool, PDB 0x58), FailedAllocations[16]
//   +0x58C, BufferSize +0x5CC
//   (PDB +0x60C); engine/CTextureManager.h has the PDB offsets past the pools, so a local model is used. A spec is 20 bytes
//   {Width, Height, CPixelFormat Format, bool MipMapped, Count}.
// Shape: PoolCount = specs.size() (the /20 magic multiply); specs[i] re-reads the vector's start each pass; the sizes
//   array is a frame object freed at the end; the two pool loops are separate `for` statements over PoolCount; the
//   totals loop reads the entry's size AND count into named locals before the PreallocSizes store (reading the count
//   after the store forces a reload past the possible alias and recolours this/specs and the zero register).
#include "engine/CPixelFormat.h"
struct CTexturePoolSpec {                        // 20 bytes
    unsigned long Width;                         // +0x00
    unsigned long Height;                        // +0x04
    CPixelFormat  Format;                        // +0x08
    bool          MipMapped;                     // +0x0c
    unsigned long Count;                         // +0x10
};
template<class T> struct vector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned int i) const { return *(_M_start + i); }
};
struct SPoolSize {
    unsigned long Size;                          // bytes per texture
    unsigned long Count;
};
struct CPoolSizeArray {                          // a sized array of (size, count) pairs
    SPoolSize* _M_start;
    SPoolSize* _M_finish;
    SPoolSize* _M_end_of_storage;
    CPoolSizeArray(unsigned int count);          // 0x00A6B620
    ~CPoolSizeArray() { if (_M_start) free(_M_start); }
    SPoolSize& operator[](unsigned int i) { return *(_M_start + i); }
};
SPoolSize* AllocatePoolSizes_CPoolSizeArray_0(unsigned int count);   // stand-in callee of the stand-in body below
// Stand-in body (the real sized constructor is out of line in retail, in this module): it fills the three words only
// inline, so VC7.1 knows the frame array's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) CPoolSizeArray::CPoolSizeArray(unsigned int count)
{
    _M_start = AllocatePoolSizes_CPoolSizeArray_0(count);
    _M_finish = _M_start + count;
    _M_end_of_storage = _M_finish;
}
#pragma auto_inline(on)
unsigned long __fastcall CalculateTextureSize(unsigned long width, unsigned long height, bool mipMapped,
                                              const CPixelFormat* format);   // 0x00A698F0
void __fastcall SortPoolSizes(SPoolSize* first, SPoolSize* last);           // 0x00A6C080
struct CPreallocTexturePool {                    // 0x54 bytes
    unsigned char _body[0x54];
    void Initialise(unsigned long index, unsigned long textureSize, unsigned long count);   // 0x00A6A540
};
struct CTextureManager {
    void*         __vftable;                     // +0x00
    bool          Initialised;                   // +0x04
    unsigned long PoolCount;                     // +0x08
    unsigned long PreallocSizes[16];             // +0x0c
    CPreallocTexturePool PreallocPools[16];      // +0x4c
    unsigned long FailedAllocations[16];         // +0x58c
    long          BufferSize;                    // +0x5cc
    void Initialise(const vector<CTexturePoolSpec>& specs);
};

void CTextureManager::Initialise(const vector<CTexturePoolSpec>& specs)
{
    if (Initialised)
        return;
    PoolCount = specs.size();
    CPoolSizeArray sizes(PoolCount);
    for (unsigned long i = 0; i < PoolCount; ++i) {
        const CTexturePoolSpec& spec = specs[i];
        sizes[i].Size = CalculateTextureSize(spec.Width, spec.Height, spec.MipMapped, &spec.Format);
        sizes[i].Count = spec.Count;
    }
    SortPoolSizes(sizes._M_start, sizes._M_finish);
    BufferSize = 0;
    for (unsigned long i = 0; i < PoolCount; ++i) {
        unsigned long size = sizes[i].Size;
        unsigned long count = sizes[i].Count;
        PreallocSizes[i] = size;
        BufferSize += count * size;
    }
    for (unsigned long i = 0; i < PoolCount; ++i) {
        unsigned long size = sizes[i].Size;
        PreallocPools[i].Initialise(i, size, sizes[i].Count);
        PreallocSizes[i] = size;
    }
    Initialised = true;
}

// ---- test stubs ----
static SPoolSize g_buf[16];
static int g_allocs, g_sortCalls;
SPoolSize* AllocatePoolSizes_CPoolSizeArray_0(unsigned int count) { g_allocs++; return (SPoolSize*)malloc(count * sizeof(SPoolSize) + 1); }
unsigned long __fastcall CalculateTextureSize(unsigned long width, unsigned long height, bool mipMapped, const CPixelFormat* format)
{
    unsigned long s = width * height * (unsigned long)format->FormatIndex;
    return mipMapped ? s + s / 3 : s;
}
void __fastcall SortPoolSizes(SPoolSize* first, SPoolSize* last)
{
    g_sortCalls++;
    for (SPoolSize* a = first; a < last; ++a)
        for (SPoolSize* b = a + 1; b < last; ++b)
            if (b->Size < a->Size) { SPoolSize t = *a; *a = *b; *b = t; }
}
static unsigned long g_initIdx[16], g_initSize[16], g_initCount[16]; static int g_inits;
void CPreallocTexturePool::Initialise(unsigned long index, unsigned long textureSize, unsigned long count)
{
    g_initIdx[g_inits] = index; g_initSize[g_inits] = textureSize; g_initCount[g_inits] = count; g_inits++;
    (void)this;
}
static int fails;
static void check(int ok, const char* what) { if (!ok) { fails++; printf("FAIL %s\n", what); } }
static CTextureManager tm;
int main()
{
    CTexturePoolSpec specs[3];
    specs[0].Width = 8; specs[0].Height = 8; specs[0].Format.FormatIndex = 4; specs[0].MipMapped = false; specs[0].Count = 3;   // 256
    specs[1].Width = 4; specs[1].Height = 4; specs[1].Format.FormatIndex = 3; specs[1].MipMapped = true;  specs[1].Count = 5;   // 48+16=64
    specs[2].Width = 16; specs[2].Height = 2; specs[2].Format.FormatIndex = 2; specs[2].MipMapped = false; specs[2].Count = 1;  // 64
    vector<CTexturePoolSpec> v; v._M_start = specs; v._M_finish = specs + 3; v._M_end_of_storage = specs + 3;
    memset(&tm, 0, sizeof tm); tm.BufferSize = 77;
    tm.Initialise(v);
    check(tm.Initialised && tm.PoolCount == 3, "flags");
    check(g_allocs == 1 && g_sortCalls == 1, "calls");
    check(tm.PreallocSizes[0] == 64 && tm.PreallocSizes[1] == 64 && tm.PreallocSizes[2] == 256, "sizes");
    check(tm.BufferSize == 64 * 5 + 64 * 1 + 256 * 3 || tm.BufferSize == 64 * 1 + 64 * 5 + 256 * 3, "buffer");
    check(g_inits == 3 && g_initIdx[0] == 0 && g_initIdx[2] == 2 && g_initSize[2] == 256 && g_initCount[2] == 3, "pools");
    check(g_initSize[0] == 64 && g_initSize[1] == 64 && g_initCount[0] + g_initCount[1] == 6, "pools2");
    // already initialised: nothing happens
    g_inits = 0; g_allocs = 0; tm.BufferSize = 5; tm.Initialise(v);
    check(g_inits == 0 && g_allocs == 0 && tm.BufferSize == 5, "once");
    // empty spec list
    memset(&tm, 0, sizeof tm); tm.BufferSize = 9; v._M_finish = specs; g_inits = 0;
    tm.Initialise(v);
    check(tm.Initialised && tm.PoolCount == 0 && tm.BufferSize == 0 && g_inits == 0, "empty");
    if (!fails) printf("00a6aaa0_TEST PASS\n");
    return fails;
}