#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
static int g_deleted = 0;
struct CMemoryBlock {
    virtual ~CMemoryBlock() { ++g_deleted; }
    unsigned long  IVRefCount;
    unsigned char* BlockData;
    bool           OwnsData;
    unsigned long  BlockSize;
    CMemoryBlock*  NextBlock;
    CMemoryBlock(bool owns, unsigned long size, CMemoryBlock* next) : IVRefCount(1), BlockData(0), OwnsData(owns), BlockSize(size), NextBlock(next) { }
};
struct CArea { unsigned long Size; CArea* NextFreeArea; long Status; };
class CSimpleMemoryPool { public:                      // the header layout, Root as its CArea type (member access only)
    void*         __vftable;
    CMemoryBlock* FirstMemoryBlock;
    unsigned long Capacity;
    unsigned long GrowthGranularity;
    CArea         Root;
};
struct CSimpleMemoryPool_Methods : CSimpleMemoryPool {
    void ClearMemoryBlocks()
    {
        CMemoryBlock* block = FirstMemoryBlock;
        if (block) {
            if (block->OwnsData) {
                do { CMemoryBlock* next = block->NextBlock; delete block; block = next; } while (block);
            } else {
                delete block;
            }
            FirstMemoryBlock = 0;
        }
    }
    void Reset() { ClearMemoryBlocks(); Root.NextFreeArea = 0; Capacity = 0x400; }
};
int main()
{
    CArea free1; free1.Size = 32; free1.NextFreeArea = 0;
    CSimpleMemoryPool_Methods p; p.FirstMemoryBlock = new CMemoryBlock(true, 16, new CMemoryBlock(true, 32, 0));
    p.Capacity = 0x80; p.Root.Size = 5; p.Root.NextFreeArea = &free1;
    p.Reset();
    if (g_deleted != 2 || p.FirstMemoryBlock != 0 || p.Root.NextFreeArea != 0 || p.Capacity != 0x400 || p.Root.Size != 5) { std::printf("00a5c790_TEST FAIL\n"); return 1; }
    p.Capacity = 3; p.Root.NextFreeArea = &free1; p.Reset();
    if (g_deleted != 2 || p.Capacity != 0x400 || p.Root.NextFreeArea != 0) { std::printf("00a5c790_TEST FAIL\n"); return 1; }
    std::printf("00a5c790_TEST PASS\n");
    return 0;
}