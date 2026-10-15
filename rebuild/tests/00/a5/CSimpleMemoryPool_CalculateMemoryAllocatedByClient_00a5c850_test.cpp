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
struct CSimpleMemoryPool {                      // the header layout, Root as its CArea type (member access only)
    void*         __vftable;
    CMemoryBlock* FirstMemoryBlock;
    unsigned long Capacity;
    unsigned long GrowthGranularity;
    CArea         Root;
};
struct CSimpleMemoryPool_Methods : CSimpleMemoryPool {
    unsigned long CalculateMemoryAllocatedByPool()
    {
        unsigned long total = 0;
        for (CMemoryBlock* block = FirstMemoryBlock; block != 0; block = block->NextBlock) total += block->BlockSize;
        return total;
    }
    unsigned long CalculateMemoryAllocatedByClient()
    {
        unsigned long total = CalculateMemoryAllocatedByPool();
        for (CArea* area = Root.NextFreeArea; area != 0; area = area->NextFreeArea) total -= area->Size + 16;
        return total;
    }
};
int main()
{
    CMemoryBlock b2(true, 0x200, 0); CMemoryBlock b1(true, 0x400, &b2);
    CArea a2; a2.Size = 0x30; a2.NextFreeArea = 0; CArea a1; a1.Size = 0x100; a1.NextFreeArea = &a2;
    CSimpleMemoryPool_Methods p; p.FirstMemoryBlock = &b1; p.Root.NextFreeArea = &a1;
    if (p.CalculateMemoryAllocatedByClient() != 0x600 - 0x130 - 32) { std::printf("00a5c850_TEST FAIL\n"); return 1; }
    p.FirstMemoryBlock = 0; p.Root.NextFreeArea = 0;
    if (p.CalculateMemoryAllocatedByClient() != 0) { std::printf("00a5c850_TEST FAIL\n"); return 1; }
    std::printf("00a5c850_TEST PASS\n");
    return 0;
}